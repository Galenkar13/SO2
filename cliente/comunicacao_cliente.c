#include <windows.h> 
#include <stdio.h>
#include <conio.h>
#include <tchar.h>
#include "cliente.h"


#define BUFSIZE 512
/*
int arrancaComunicacaoCliente()
{

	HANDLE hPipe;
	LPTSTR lpvMessage = TEXT("Default message from client.");
	//TCHAR  chBuf[sizeof(MsgCliGat)]; //trocar por sizeofMesgCliGAT
	BOOL   fSuccess = FALSE;
	DWORD  cbRead, cbToWrite, cbWritten, dwMode;
	LPTSTR lpszPipename = TEXT("\\\\.\\pipe\\mynamedpipe");

	// Try to open a named pipe; wait for it, if necessary. 

	while (1)
	{
		hPipe = CreateFile(
			lpszPipename,   // pipe name 
			GENERIC_READ |  // read and write access 
			GENERIC_WRITE,
			0,              // no sharing 
			NULL,           // default security attributes
			OPEN_EXISTING,  // opens existing pipe 
			0,              // default attributes 
			NULL);          // no template file 

							// Break if the pipe handle is valid. 

		if (hPipe != INVALID_HANDLE_VALUE)
			break;

		// Exit if an error other than ERROR_PIPE_BUSY occurs. 

		if (GetLastError() != ERROR_PIPE_BUSY)
		{
			_tprintf(TEXT("Could not open pipe. GLE=%d\n"), GetLastError());
			return -1;
		}

		// All pipe instances are busy, so wait for 20 seconds. 

		if (!WaitNamedPipe(lpszPipename, 20000))
		{
			printf("Could not open pipe: 20 second wait timed out.");
			return -1;
		}
	}

	// The pipe connected; change to message-read mode. 

	dwMode = PIPE_READMODE_MESSAGE;
	fSuccess = SetNamedPipeHandleState(
		hPipe,    // pipe handle 
		&dwMode,  // new pipe mode 
		NULL,     // don't set maximum bytes 
		NULL);    // don't set maximum time 
	if (!fSuccess)
	{
		_tprintf(TEXT("SetNamedPipeHandleState failed. GLE=%d\n"), GetLastError());
		return -1;
	}

	// Send a message to the pipe server. 

//	_tprintf(TEXT("Sending %d byte message: \"%s\"\n"), cbToWrite, lpvMessage);

	MsgCLI Teste;
	Teste.tipo_mensagem = INICIO;
	_tcscpy_s(Teste.nome, _countof(Teste.nome)  ,_T("ZE FINOS"));
	Teste.id = 123;
	//_tcscpy(Teste.tecla, TEXT("esquerda"));
	while (1) {
		fSuccess = WriteFile(
			hPipe,                  // pipe handle 
			(void *)&Teste,             // message 
			sizeof(MsgCLI),              // message length 
			&cbWritten,             // bytes written 
			NULL);                  // not overlapped 

		if (!fSuccess)
		{
			_tprintf(TEXT("WriteFile to pipe failed. GLE=%d\n"), GetLastError());
			return -1;
		}

		printf("\nMessage sent to server, receiving reply as follows:\n");

		do
		{
			// Read from the pipe. 

			MsgCliGat cenas;

			fSuccess = ReadFile(
				hPipe,    // pipe handle 
				(void *)&cenas,    // buffer to receive reply 
				sizeof(MsgCliGat),  // size of buffer 
				&cbRead,  // number of bytes read 
				NULL);    // not overlapped 

			if (!fSuccess && GetLastError() != ERROR_MORE_DATA)
				break;

			_tprintf(TEXT("\"%d\"\n"), cenas.cenas3.altura);
		} while (!fSuccess);  // repeat loop if ERROR_MORE_DATA 

		if (!fSuccess)
		{
			_tprintf(TEXT("ReadFile from pipe failed. GLE=%d\n"), GetLastError());
			return -1;
		}
	}

	printf("\n<End of message, press ENTER to terminate connection and exit>");
	_getch();
	CloseHandle(hPipe);

	return 0;
}
*/


	int idJogador;
	HANDLE hThreadCliente;
	HANDLE hPipe;
	HANDLE eventWriteReady;
	HANDLE eventReadReady;




DWORD WINAPI arrancaComunicacaoCliente()
{
	BOOL   fSuccess = FALSE;
	DWORD  dwMode;
	TCHAR* lpszPipename = TEXT("\\\\.\\pipe\\mynamedpipetestes");

	// Try to open a named pipe; wait for it, if necessary. 

	while (1)
	{
		hPipe = CreateFile(
			lpszPipename,   // pipe name 
			GENERIC_READ |  // read and write access 
			GENERIC_WRITE,
			0,              // no sharing 
			NULL,		    // default security attributes
			OPEN_EXISTING,  // opens existing pipe 
			0 | FILE_FLAG_OVERLAPPED,              // default attributes 
			NULL);          // no template file 

							// Break if the pipe handle is valid. 

		if (hPipe != INVALID_HANDLE_VALUE)
			break;

		// Exit if an error other than ERROR_PIPE_BUSY occurs. 

		if (GetLastError() != ERROR_PIPE_BUSY)
		{
			TCHAR yo[512];

			_stprintf_s(yo, 512, TEXT("Could not open pipe. GLE=%d\n"), GetLastError());
			OutputDebugString(yo);
			return -1;
		}

		// All pipe instances are busy, so wait for 20 seconds. 

		if (!WaitNamedPipe(lpszPipename, 20000))
		{
			OutputDebugString(TEXT("Could not open pipe: 20 second wait timed out."));
			return -1;
		}
	}

	OutputDebugString(TEXT("Ligado NAMED PIPE \n"));

	// The pipe connected; change to message-read mode. 

	dwMode = PIPE_READMODE_MESSAGE;
	fSuccess = SetNamedPipeHandleState(
		hPipe,    // pipe handle 
		&dwMode,  // new pipe mode 
		NULL,     // don't set maximum bytes 
		NULL);    // don't set maximum time 
	if (!fSuccess)
	{
		OutputDebugString(TEXT("SetNamedPipeHandleState failed"));
		return -1;
	}

	EnviaMensagemCLI();
	Sleep(1000);
	RecebeUpdates();
	Sleep(10000000);
	return 0;
}

BOOL RecebeUpdates()
{
	MsgCliGat update;
	
	OVERLAPPED overlRd = { 0 };
	DWORD cbRead;

	HANDLE readReady = CreateEvent(NULL, TRUE, FALSE, NULL);

	while (1)
	{
		ZeroMemory(&overlRd, sizeof(overlRd));
		overlRd.hEvent = readReady;
		ResetEvent(readReady);

		// Read from the pipe.
		ReadFile(hPipe, &update, sizeof(MsgCliGat), &cbRead, &overlRd);

		OutputDebugString(TEXT("\n\n\nA receber update \n\n\n"));

		WaitForSingleObject(readReady, INFINITE);

		OutputDebugString(TEXT("\n\n\nWaitForSingleObject \n\n\n"));

		BOOL fSuccess = GetOverlappedResult(hPipe, &overlRd, &cbRead, FALSE);

		OutputDebugString(TEXT("\n\n\nGetOverlappedResult \n\n\n"));

		if (!fSuccess && GetLastError() != ERROR_MORE_DATA)
			break;

		OutputDebugString(TEXT("\n\n\nRecebi update \n\n\n"));

		_tprintf(TEXT("\"%d\"\n"), update.cenas3.altura);
	}

	CloseHandle(hPipe);
	return 0;
}

BOOL EnviaMensagemCLI()
{
	
	DWORD cbWritten = 0;
	OVERLAPPED overlWr = { 0 };

	
	MsgCLI msg;
	msg.tipo_mensagem = INICIO;
	_tcscpy_s(msg.nome, _countof(msg.nome), _T("ZE FINOS"));
	msg.id = 123;

	HANDLE writeReady = CreateEvent(NULL, TRUE, FALSE, NULL);

	ZeroMemory(&overlWr, sizeof(overlWr));

	ResetEvent(writeReady);

	overlWr.hEvent = writeReady;

	BOOL fSuccess = WriteFile(
		hPipe,                  // pipe handle 
		(void*)&msg,             // message 
		sizeof(msg),             // message length 
		&cbWritten,             // bytes written 
		&overlWr);                  //  overlapped 

	WaitForSingleObject(writeReady, INFINITE);

	GetOverlappedResult(hPipe, &overlWr, &cbWritten, FALSE);

	if (!fSuccess || cbWritten < sizeof(msg))
	{
		TCHAR yo[512];
		_stprintf_s(yo, 512, TEXT("WriteFile to pipe failed. GLE=%d\n"), GetLastError());
		OutputDebugString(yo);
	}

	return fSuccess;
}