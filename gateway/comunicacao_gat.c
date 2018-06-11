#include <windows.h> 
#include <stdio.h>
#include <tchar.h>
#include <strsafe.h>
#include <fcntl.h>
#include <Windows.h>
#include <tchar.h>
#include <fcntl.h>
#include <io.h>
#include <stdio.h>
#include "gateway.h"

#define CONNECTING_STATE 0 
#define READING_STATE 1 
#define WRITING_STATE 2 
#define INSTANCES 4 
#define PIPE_TIMEOUT 5000
#define BUFSIZE 4096



DWORD WINAPI ThreadAtualizacao(LPVOID param) { //LADO DO GATEWAY


	while (1) { //Isto é para passar para a DLL
	//	RecebeAtualizacao(10);
	}
	return 0;
}

#define PIPE_BUFFER 4096
#define PIPE_TIMEOUT 5000

//Passar isto para estrutura

int index = 0;

HANDLE hThreadCliente[MaxClientes];
HANDLE hPipe[MaxClientes];
HANDLE eventWriteReady;
HANDLE eventReadReady;
HANDLE hThreadCriaNamedPipes;


DWORD WINAPI RecebeMensagensClientes(LPVOID param)
{   
	int aux = (int)param;
	
	BOOL fSuccess = FALSE;

	OVERLAPPED overlRd = { 0 };
	DWORD cbBytesRead = 0;
	MsgCLI msg;

	if (hPipe[aux] == NULL)
	{
		_tprintf(TEXT("Named Pipe Inválido!\n"));
		return -1;
	}

	while (1)
	{
		ZeroMemory(&overlRd, sizeof(overlRd));
		ResetEvent(eventReadReady);
		overlRd.hEvent = eventReadReady;

		fSuccess = ReadFile(hPipe[aux], &msg, sizeof(MsgCLI), &cbBytesRead, &overlRd);

		WaitForSingleObject(eventReadReady, INFINITE);

		BOOL erro = GetOverlappedResult(hPipe, &overlRd, &cbBytesRead, FALSE);

		_tprintf(TEXT("Recebido %d %d\n"), cbBytesRead, erro);

		if (cbBytesRead < sizeof(MsgCLI))
		{
			DWORD result = GetLastError();
			_tprintf(TEXT("Ocorreu um erro a comunicar com o cliente. ( %d )\n"), result);

			if (result == ERROR_BROKEN_PIPE)
			{
				break;
			}
		}
		else
		{

		//	RecebeMensagem(msg);

			EnviaMensagem(msg);

			_tprintf(TEXT(" TIPO %d \n"), msg.tipo_mensagem);
			_tprintf(TEXT(" NOME  %s\n"), msg.nome);
			_tprintf(TEXT(" ID %d \n"), msg.id);

			_tprintf(TEXT("Mensagem recebida \n"));
		}
	}

	// remover cliente 
	FlushFileBuffers(hPipe[aux]);
	DisconnectNamedPipe(hPipe[aux]);
	CloseHandle(hPipe[aux]);

	_tprintf(TEXT("Thread de comunicação terminada\n"));

	return 0;
}


//HANDLE hEvento, hMutexJogo;

DWORD WINAPI arrancaComunicacaoGateway()
{

	_tprintf(TEXT("\n\n Listen for pipe clients ON AND OK \n\n"));


	BOOL fConnected = FALSE;
	TCHAR* lpszPipename = TEXT("\\\\.\\pipe\\mynamedpipetestes");
	HANDLE hPipeAux = INVALID_HANDLE_VALUE, hThreadAux = NULL;

	while (1)
	{
		hPipeAux = CreateNamedPipe(
			lpszPipename,
			PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED,
			PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
			PIPE_UNLIMITED_INSTANCES,
			PIPE_BUFFER,
			PIPE_BUFFER,
			PIPE_TIMEOUT,
			NULL); 

		if (hPipeAux == INVALID_HANDLE_VALUE)
		{
			_tprintf(TEXT("Error creating a new pipe instance! ( %d )"), GetLastError());
			continue;
		}

		fConnected = ConnectNamedPipe(hPipeAux, NULL) ? TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);

		// TODO limitar n clientes
		if (fConnected)
		{
			hPipe[index] = hPipeAux;
			eventWriteReady = CreateEvent(NULL, TRUE, FALSE, NULL);
     		eventReadReady = CreateEvent(NULL, TRUE, FALSE, NULL);
			hThreadCliente[index] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)RecebeMensagensClientes, (LPVOID)index, 0, NULL);
			index++;
		}
		else
		{
			CloseHandle(hPipeAux);
		}
	}

	return 0;
}


DWORD WINAPI  EnviaUpdateCliente() {
	DWORD cbWritten = 0;
	OVERLAPPED overlWr = { 0 };
	hMutexJogo = OpenMutex(SYNCHRONIZE, FALSE, TEXT("GoMtex"));
	hEvento = OpenEvent(SYNCHRONIZE, FALSE, TEXT("GoEvent"));
	
	while (1) {
		MsgCliGat update;
		int i;
		//WaitForSingleObject(cliente->hMutex, INFINITE);

		//_tprintf(TEXT("[GATEWAY] a enviar ao cliente %d\n"), cliente->idJogador);

		ZeroMemory(&overlWr, sizeof(overlWr));

		ResetEvent(eventWriteReady);

		overlWr.hEvent = eventWriteReady;

		WaitForSingleObject(hEvento, INFINITE);
		WaitForSingleObject(hMutexJogo, INFINITE);
		update = EnviaUpdate(jogo->CicloDeVida);
		ReleaseMutex(hMutexJogo);

		_tprintf(TEXT(" ALTURA %d \n"), jogo->Dados.nDefenders);
		_tprintf(TEXT(" ALTURA %d \n"), jogo->Dados.nDefenders);


		if (jogo->CicloDeVida == ASSOCIACAO) {

			WriteFile(
				hPipe[update.mensagensCliente.id],     // pipe handle 
				(void*)&update,     // message 
				sizeof(update),		// message length 
				&cbWritten,         // bytes written 
				&overlWr);          // overlapped 

		}
		else
			if (jogo->CicloDeVida == DECORRER) {
				for (int j = 0; j < index; j++) {
					WriteFile(
						hPipe[j],     // pipe handle 
						(void*)&update,     // message 
						sizeof(update),		// message length 
						&cbWritten,         // bytes written 
						&overlWr);
				}
			}
			else
				WriteFile(
					hPipe[update.mensagensCliente.id],     // pipe handle 
					(void*)&update,     // message 
					sizeof(update),		// message length 
					&cbWritten,         // bytes written 
					&overlWr);

		WaitForSingleObject(eventWriteReady, INFINITE);

		BOOL fSuccess = GetOverlappedResult(hPipe, &overlWr, &cbWritten, FALSE);

		if (!fSuccess || cbWritten < sizeof(update))
		{
			_tprintf(TEXT("WriteFile to pipe failed. GLE=%d\n"), GetLastError());
		}
		else
		{
			_tprintf(TEXT("Enviado UPDATE %d\n"), cbWritten);
		}

		//ReleaseMutex(cliente->hMutex);
	}
	return 0;
}

MsgCliGat EnviaUpdate(int estado) {
	MsgCliGat teste;

	switch (estado)
	{
	case ASSOCIACAO:
	{	
		teste.tipo = PRIMEIRA;
		teste.mensagensCliente.id = jogo->Dados.nDefenders - 1;
	}
		break;
	case FINAL:
	{
		teste.tipo = PONTUACAO;
	}
		break;
	case DECORRER:
	{
		memcpy(&teste.JogoCopia, jogo, sizeof(&jogo));
		teste.tipo = ATUALIZACAO;
	}
		break;
	default:
		break;
	}

	return teste;
}

/*
BOOL BroadcastUpdatesCliente(PGATEWAY gateway, UPDATE update)
{
	_tprintf(TEXT("[GATEWAY] A enviar update aos clientes\n"));

	for (int i = 0; i < gateway->clientesLigados; i++) {
		if (gateway->clientes[i] != NULL && gateway->clientes[i]->hPipe != INVALID_HANDLE_VALUE) {
			if (update.tipo != _IDJOGADOR || update.idJogador == gateway->clientes[i]->idJogador)
			{
				EnviaUpdateCliente(gateway->clientes[i], update);
			}
		}
	}

	return FALSE;
}


DWORD WINAPI LeUpdateServidor(LPVOID param)
{
	PGATEWAY gateway = (PGATEWAY)param;

	while (gateway->continuar) {
		UPDATE update = LeProximoUpdate();

		BroadcastUpdatesCliente(gateway, update);
	}

	return 0;
}

BOOL InicializaThreadsUpdates()
{
	_tprintf(TEXT("[GATEWAY] A criar thread para receber updates do servidor \n"));

	hThreadLeUpdates = CreateThread(NULL, 0, LeUpdateServidor, gateway, 0, NULL);

	if (gateway->hThreadLeUpdates == NULL)
	{
		_tprintf(TEXT("[GATEWAY] Ocorreu um erro ( %d ) \n"), GetLastError());
		return FALSE;
	}

	_tprintf(TEXT("[GATEWAY] Thread criada: receber updates do servidor \n"));

	return TRUE;
}*/