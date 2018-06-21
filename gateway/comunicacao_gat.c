#include <windows.h> 
#include <stdio.h>
#include <tchar.h>
#include <strsafe.h>
#include <fcntl.h>
#include <Windows.h>
#include <sddl.h>
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

#define PIPE_BUFFER 4096
#define PIPE_TIMEOUT 5000

int index = 0;

HANDLE hThreadCliente[MaxClientes];
HANDLE hPipe[MaxClientes];
HANDLE eventWriteReady;
HANDLE eventReadReady;
HANDLE hThreadCriaNamedPipes;

SECURITY_ATTRIBUTES sa;
TCHAR * szSD = TEXT("D:")
TEXT("(A;OICI;GA;;;BG)")
TEXT("(A;OICI;GA;;;AN)")
TEXT("(A;OICI;GA;;;AU)")
TEXT("(A;OICI;GA;;;BA)");


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



			EnviaMensagem(msg);


			_tprintf(TEXT("Mensagem recebida \n"));
		}
	}


	FlushFileBuffers(hPipe[aux]);
	DisconnectNamedPipe(hPipe[aux]);
	CloseHandle(hPipe[aux]);

	_tprintf(TEXT("Thread de comunicação terminada\n"));

	return 0;
}


//HANDLE hEvento, hMutexJogo;

DWORD WINAPI arrancaComunicacaoGateway()
{

	_tprintf(TEXT("\n Gateway Ligado!!! Esperando Clientes \n"));


	BOOL fConnected = FALSE;
	TCHAR* lpszPipename = TEXT("\\\\.\\pipe\\mynamedpipetestes");
	HANDLE hPipeAux = INVALID_HANDLE_VALUE, hThreadAux = NULL;

	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.bInheritHandle = FALSE;

	ConvertStringSecurityDescriptorToSecurityDescriptor(szSD, SDDL_REVISION_1, &(sa.lpSecurityDescriptor), NULL);

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
			&sa);  //trocar por &sa

		if (hPipeAux == INVALID_HANDLE_VALUE)
		{
			_tprintf(TEXT("Error creating a new pipe instance! ( %d )"), GetLastError());
			continue;
		}

		fConnected = ConnectNamedPipe(hPipeAux, NULL) ? TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);

		if (fConnected)
		{
			hPipe[index] = hPipeAux;
			eventWriteReady = CreateEvent(NULL, TRUE, TRUE, NULL);
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
	hMutexJogo = OpenMutex(SYNCHRONIZE, FALSE, TEXT("GoMutex"));
	hEvento = OpenEvent(SYNCHRONIZE, FALSE, TEXT("GoEvent"));
	
	while (1) {
		MsgCliGat update;
		//WaitForSingleObject(cliente->hMutex, INFINITE);

		//_tprintf(TEXT("[GATEWAY] a enviar ao cliente %d\n"), cliente->idJogador);

		ZeroMemory(&overlWr, sizeof(overlWr));

		ResetEvent(eventWriteReady);

		overlWr.hEvent = eventWriteReady;

		WaitForSingleObject(hEvento, INFINITE);
		WaitForSingleObject(hMutexJogo, INFINITE);
		update = EnviaUpdate(jogo->CicloDeVida);
		ReleaseMutex(hMutexJogo);

		switch (jogo->CicloDeVida)
		{
		case ASSOCIACAO:
			WriteFile(
				hPipe[update.mensagensCliente.id],     // pipe handle 
				(void*)&update,     // message 
				sizeof(update),		// message length 
				&cbWritten,         // bytes written 
				&overlWr);
			break;
		case DECORRER:
			for (int j = 0; j < index; j++) {
					WriteFile(
						hPipe[j],     // pipe handle 
						(void*)&update,     // message 
						sizeof(update),		// message length 
						&cbWritten,         // bytes written 
						&overlWr);
			}
				break;
		case FINAL:
			for (int j = 0; j < index; j++) {
				WriteFile(
					hPipe[j],     // pipe handle 
					(void*)&update,     // message 
					sizeof(update),		// message length 
					&cbWritten,         // bytes written 
					&overlWr);
			}
			break;
		default:
			break;
		}

		WaitForSingleObject(eventWriteReady, INFINITE);

		BOOL fSuccess = GetOverlappedResult(hPipe, &overlWr, &cbWritten, FALSE);

		if (!fSuccess || cbWritten < sizeof(update))
			_tprintf(TEXT("WriteFile to pipe failed. GLE=%d\n"), GetLastError());
		else
			_tprintf(TEXT("Enviado UPDATE %d\n"), cbWritten);

		}
		//ReleaseMutex(cliente->hMutex);
	
	return 0;
}

MsgCliGat EnviaUpdate(int estado) {
	MsgCliGat teste;
	int i;
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
		TCHAR pontos[20];
		teste.pontuacaoFinal.index = jogo->Dados.nDefenders;
		for (i = 0; i < jogo->Dados.nDefenders; i++)
		{
			teste.pontuacaoFinal.TOP[i].pontos = jogo->Defenders[i].pontos;
			_tcscpy_s(teste.pontuacaoFinal.TOP[i].quem, _countof(jogo->Defenders[i].nome), jogo->Defenders[i].nome);

		}
	}
		break;
	case DECORRER:
	{
		teste.tipo = ATUALIZACAO;
		memcpy(teste.JogoCopia.Invaders, &jogo->Invaders, sizeof(Invader)*MaxInvaders );
		memcpy(teste.JogoCopia.Defenders, &jogo->Defenders, sizeof(Defender)*MaxClientes);
		memcpy(teste.JogoCopia.Bombas, &jogo->Bombas, sizeof(Bombas)*MaxnumBombas);
		memcpy(teste.JogoCopia.Tiros, &jogo->Tiros, sizeof(Tiros)*MaxnumTiros);
		memcpy(teste.JogoCopia.PowerUP, &jogo->PowerUP, sizeof(PowerUP)*MaxPowerUP);
		memcpy(&teste.JogoCopia.Dados, &jogo->Dados, sizeof(Dados));
		memcpy(&teste.JogoCopia.CicloDeVida, &jogo->CicloDeVida, sizeof(CicloDeVida));
		
	}
		break;
	default:
		break;
	}

	return teste;
}
