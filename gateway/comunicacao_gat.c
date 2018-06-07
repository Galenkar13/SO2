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
/*
typedef struct
{
	OVERLAPPED oOverlap;
	HANDLE hPipeInst;
	MsgCLI chRequest;
	DWORD cbRead;
	MsgCliGat chReply;
	DWORD cbToWrite;
	DWORD dwState;
	BOOL fPendingIO;
} PIPEINST, *LPPIPEINST;


VOID DisconnectAndReconnect(DWORD);
BOOL ConnectToNewClient(HANDLE, LPOVERLAPPED);
VOID GetAnswerToRequest(LPPIPEINST);

PIPEINST Pipe[INSTANCES];
HANDLE hEvents[INSTANCES];

int arrancaComunicao(Jogo jogo)
{
	DWORD i, dwWait, cbRet, dwErr;
	BOOL fSuccess;
	LPTSTR lpszPipename = TEXT("\\\\.\\pipe\\mynamedpipe");
	HANDLE hEvento, hMutexJogo;

	hMutexJogo = OpenMutex(SYNCHRONIZE, FALSE, TEXT("GoMtex"));
	hEvento = OpenEvent(SYNCHRONIZE, FALSE, TEXT("GoEvent"));

	// The initial loop creates several instances of a named pipe 
	// along with an event object for each instance.  An 
	// overlapped ConnectNamedPipe operation is started for 
	// each instance. 

	for (i = 0; i < INSTANCES; i++)
	{

		// Create an event object for this instance. 

		hEvents[i] = CreateEvent(
			NULL,    // default security attribute 
			TRUE,    // manual-reset event 
			TRUE,    // initial state = signaled 
			NULL);   // unnamed event object 

		if (hEvents[i] == NULL)
		{
			printf("CreateEvent failed with %d.\n", GetLastError());
			return 0;
		}

		Pipe[i].oOverlap.hEvent = hEvents[i];

		Pipe[i].hPipeInst = CreateNamedPipe(
			lpszPipename,            // pipe name 
			PIPE_ACCESS_DUPLEX |     // read/write access 
			FILE_FLAG_OVERLAPPED,    // overlapped mode 
			PIPE_TYPE_MESSAGE |      // message-type pipe 
			PIPE_READMODE_MESSAGE |  // message-read mode 
			PIPE_WAIT,               // blocking mode 
			INSTANCES,               // number of instances 
			BUFSIZE * sizeof(TCHAR),   // output buffer size 
			BUFSIZE * sizeof(TCHAR),   // input buffer size 
			PIPE_TIMEOUT,            // client time-out 
			NULL);                   // default security attributes 

		if (Pipe[i].hPipeInst == INVALID_HANDLE_VALUE)
		{
			printf("CreateNamedPipe failed with %d.\n", GetLastError());
			return 0;
		}

		// Call the subroutine to connect to the new client

		Pipe[i].fPendingIO = ConnectToNewClient(
			Pipe[i].hPipeInst,
			&Pipe[i].oOverlap);

		Pipe[i].dwState = Pipe[i].fPendingIO ?
			CONNECTING_STATE : // still connecting 
			READING_STATE;     // ready to read 
	}

	while (1)
	{
		// Wait for the event object to be signaled, indicating 
		// completion of an overlapped read, write, or 
		// connect operation. 

		dwWait = WaitForMultipleObjects(
			INSTANCES,    // number of event objects 
			hEvents,      // array of event objects 
			FALSE,        // does not wait for all 
			INFINITE);    // waits indefinitely 

						  // dwWait shows which pipe completed the operation. 

		i = dwWait - WAIT_OBJECT_0;  // determines which pipe 
		if (i < 0 || i >(INSTANCES - 1))
		{
			printf("Index out of range.\n");
			return 0;
		}

		// Get the result if the operation was pending. 

		if (Pipe[i].fPendingIO)
		{
			fSuccess = GetOverlappedResult(
				Pipe[i].hPipeInst, // handle to pipe 
				&Pipe[i].oOverlap, // OVERLAPPED structure 
				&cbRet,            // bytes transferred 
				FALSE);            // do not wait 

			switch (Pipe[i].dwState)
			{
				// Pending connect operation 
			case CONNECTING_STATE:
				if (!fSuccess)
				{
					printf("Error %d.\n", GetLastError());
					return 0;
				}
				Pipe[i].dwState = READING_STATE;
				break;

				// Pending read operation 
			case READING_STATE:
				if (!fSuccess || cbRet == 0)
				{
					DisconnectAndReconnect(i);
					continue;
				}
				Pipe[i].cbRead = cbRet;
				Pipe[i].dwState = WRITING_STATE;
				break;

				// Pending write operation 
			case WRITING_STATE:
				if (!fSuccess || cbRet != Pipe[i].cbToWrite)
				{
					DisconnectAndReconnect(i);
					continue;
				}
				Pipe[i].dwState = READING_STATE;
				break;

			default:
			{
				printf("Invalid pipe state.\n");
				return 0;
			}
			}
		}

		// The pipe state determines which operation to do next. 

		switch (Pipe[i].dwState)
		{
			// READING_STATE: 
			// The pipe instance is connected to the client 
			// and is ready to read a request from the client. 

		case READING_STATE:
			fSuccess = ReadFile(
				Pipe[i].hPipeInst,
				(void *)&Pipe[i].chRequest,
				BUFSIZE * sizeof(TCHAR),
				&Pipe[i].cbRead,
				&Pipe[i].oOverlap);

			// The read operation completed successfully. 
			

		/*	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadProdutor, &Pipe[i].chRequest, 0, NULL);

			if (hThreadEscritor == NULL){
				_tprintf(TEXT("Erro ao criar Thread Escritor\n"));
				return -1;
			}
			_tprintf(TEXT(" TIPO %d \n"), Pipe[i].chRequest.tipo_mensagem);
			_tprintf(TEXT(" NOME  %s\n"), Pipe[i].chRequest.nome);
			_tprintf(TEXT(" ID %d \n"), Pipe[i].chRequest.id);

			//hThreadEscritor = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadProdutor, &Pipe[i], 0, NULL);
			EnviaMensagem(Pipe[i].chRequest);

			//EnviaMensagem(Pipe[i].chRequest);

			if (fSuccess && Pipe[i].cbRead != 0)
			{
				Pipe[i].fPendingIO = FALSE;
				Pipe[i].dwState = WRITING_STATE;
				continue;
			}

			// The read operation is still pending. 

			dwErr = GetLastError();
			if (!fSuccess && (dwErr == ERROR_IO_PENDING))
			{
				Pipe[i].fPendingIO = TRUE;
				continue;
			}

			// An error occurred; disconnect from the client. 

			DisconnectAndReconnect(i);
			break;

			// WRITING_STATE: 
			// The request was successfully read from the client. 
			// Get the reply data and write it to the client. 


			

		case WRITING_STATE:
			//GetAnswerToRequest(&Pipe[i]);
			WaitForSingleObject(hEvento, INFINITE);
			WaitForSingleObject(hMutexJogo, INFINITE);
			Pipe[0].chReply.cenas3.altura = jogo.altura;
			ReleaseMutex(hMutexJogo);

			_tprintf(TEXT(" ALTURA %d \n"), Pipe[0].chReply.cenas3.altura);
		

			fSuccess = WriteFile(
				Pipe[0].hPipeInst,
				(void *)&Pipe[0].chReply,
				sizeof(MsgCliGat),
				&cbRet,
				&Pipe[i].oOverlap);

			// The write operation completed successfully. 
			

			if (fSuccess && cbRet == Pipe[i].cbToWrite)
			{
				Pipe[i].fPendingIO = FALSE;
				Pipe[i].dwState = READING_STATE;
				continue;
			}

			// The write operation is still pending. 

			dwErr = GetLastError();
			if (!fSuccess && (dwErr == ERROR_IO_PENDING))
			{
				Pipe[i].fPendingIO = TRUE;
				continue;
			}

			// An error occurred; disconnect from the client. 

			DisconnectAndReconnect(i);
			break;

		default:
		{
			printf("Invalid pipe state.\n");
			return 0;
		}
		}
	}

	return 0;
}


// DisconnectAndReconnect(DWORD) 
// This function is called when an error occurs or when the client 
// closes its handle to the pipe. Disconnect from this client, then 
// call ConnectNamedPipe to wait for another client to connect. 

VOID DisconnectAndReconnect(DWORD i)
{
	// Disconnect the pipe instance. 

	if (!DisconnectNamedPipe(Pipe[i].hPipeInst))
	{
		printf("DisconnectNamedPipe failed with %d.\n", GetLastError());
	}

	// Call a subroutine to connect to the new client. 

	Pipe[i].fPendingIO = ConnectToNewClient(
		Pipe[i].hPipeInst,
		&Pipe[i].oOverlap);

	Pipe[i].dwState = Pipe[i].fPendingIO ?
		CONNECTING_STATE : // still connecting 
		READING_STATE;     // ready to read 
}

// ConnectToNewClient(HANDLE, LPOVERLAPPED) 
// This function is called to start an overlapped connect operation. 
// It returns TRUE if an operation is pending or FALSE if the 
// connection has been completed. 

BOOL ConnectToNewClient(HANDLE hPipe, LPOVERLAPPED lpo)
{
	BOOL fConnected, fPendingIO = FALSE;

	// Start an overlapped connection for this pipe instance. 
	fConnected = ConnectNamedPipe(hPipe, lpo);

	// Overlapped ConnectNamedPipe should return zero. 
	if (fConnected)
	{
		printf("ConnectNamedPipe failed with %d.\n", GetLastError());
		return 0;
	}

	switch (GetLastError())
	{
		// The overlapped connection in progress. 
	case ERROR_IO_PENDING:
		fPendingIO = TRUE;
		break;

		// Client is already connected, so signal an event. 

	case ERROR_PIPE_CONNECTED:
		if (SetEvent(lpo->hEvent))
			break;

		// If an error occurs during the connect operation... 
	default:
	{
		printf("ConnectNamedPipe failed with %d.\n", GetLastError());
		return 0;
	}
	}

	return fPendingIO;
}
*/

HANDLE hEvento, hMutexJogo;



DWORD WINAPI ThreadAtualizacao(LPVOID param) { //LADO DO GATEWAY


	while (1) { //Isto é para passar para a DLL
		RecebeAtualizacao(10);
	}
	return 0;
}

#define PIPE_BUFFER 4096
#define PIPE_TIMEOUT 5000

//Passar isto para estrutura

HANDLE hThreadCliente;
HANDLE hPipe;
HANDLE eventWriteReady;
HANDLE eventReadReady;
HANDLE hThreadCriaNamedPipes;

DWORD WINAPI RecebeMensagensClientes()
{
	
	BOOL fSuccess = FALSE;

	OVERLAPPED overlRd = { 0 };
	DWORD cbBytesRead = 0;
	MsgCLI msg;

	if (hPipe == NULL)
	{
		_tprintf(TEXT("Named Pipe Inválido!\n"));
		return -1;
	}

	while (1)
	{
		ZeroMemory(&overlRd, sizeof(overlRd));
		ResetEvent(eventReadReady);
		overlRd.hEvent = eventReadReady;

		fSuccess = ReadFile(hPipe, &msg, sizeof(MsgCLI), &cbBytesRead, &overlRd);

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
	FlushFileBuffers(hPipe);
	DisconnectNamedPipe(hPipe);
	CloseHandle(hPipe);

	_tprintf(TEXT("Thread de comunicação terminada\n"));

	return 0;
}

DWORD WINAPI arrancaComunicacaoGateway()
{


	/*TCHAR * szSD = TEXT("D:")
	TEXT("(A;OICI;GA;;;BG)")
	TEXT("(A;OICI;GA;;;AN)")
	TEXT("(A;OICI;GA;;;AU)")
	TEXT("(A;OICI;GA;;;BA)");

	SECURITY_ATTRIBUTES sa;

	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.bInheritHandle = FALSE;*/

	_tprintf(TEXT("\n\n Listen for pipe clients ON AND OK \n\n"));

	//ConvertStringSecurityDescriptorToSecurityDescriptor(szSD, SDDL_REVISION_1, &(sa.lpSecurityDescriptor), NULL);

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
			NULL); // &sa

		if (hPipeAux == INVALID_HANDLE_VALUE)
		{
			_tprintf(TEXT("Error creating a new pipe instance! ( %d )"), GetLastError());
			continue;
		}

		fConnected = ConnectNamedPipe(hPipeAux, NULL) ? TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);

		// TODO limitar n clientes
		if (fConnected)
		{

			
			hPipe = hPipeAux;

			eventWriteReady = CreateEvent(NULL, TRUE, FALSE, NULL);
		eventReadReady = CreateEvent(NULL, TRUE, FALSE, NULL);

		hThreadCliente = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)RecebeMensagensClientes, NULL, 0, NULL);

			
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

		_tprintf(TEXT(" ALTURA %d \n"), update.JogoCopia.altura);


		WriteFile(
			hPipe,     // pipe handle 
			(void*)&update,     // message 
			sizeof(update),		// message length 
			&cbWritten,         // bytes written 
			&overlWr);          // overlapped 

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

	return 0;
}

MsgCliGat EnviaUpdate(int estado) {
	MsgCliGat teste;
	int temp = jogo->nDefenders;
	
	switch (estado)
	{
	case ASSOCIACAO:
	{
		teste.mensagensCliente.id = temp;
		teste.tipo = PRIMEIRA;
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