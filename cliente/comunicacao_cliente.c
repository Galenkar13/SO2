#include <windows.h> 
#include <stdio.h>
#include <conio.h>
#include <tchar.h>
#include "cliente.h"


#define BUFSIZE 512


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


	RecebeUpdates();

	return 0;
}

BOOL RecebeUpdates()
{
	MsgCliGat update;
	int x = 0;
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

		switch(update.tipo)
		{
		case PRIMEIRA:
		{
			idJogador = update.mensagensCliente.id;
		}
			break;
		case PONTUACAO:
			vaisDesenharTop10(update);
			break;
		case ATUALIZACAO:
		{
			VaisDesenharCRL(update);
			//ReproduzirMusica(MUSICA1);
		}
			break;
		default:
			break;
		}
	}

	CloseHandle(hPipe);
	return 0;
}

BOOL EnviaMensagemCLI(MsgCLI cli)
{
	
	DWORD cbWritten = 0;
	OVERLAPPED overlWr = { 0 };
	HANDLE writeReady = CreateEvent(NULL, TRUE, FALSE, NULL);

	ZeroMemory(&overlWr, sizeof(overlWr));

	ResetEvent(writeReady);

	overlWr.hEvent = writeReady;

	BOOL fSuccess = WriteFile(
		hPipe,                  // pipe handle 
		(void*)&cli,             // message 
		sizeof(cli),             // message length 
		&cbWritten,             // bytes written 
		&overlWr);                  //  overlapped 

	WaitForSingleObject(writeReady, INFINITE);

	GetOverlappedResult(hPipe, &overlWr, &cbWritten, FALSE);

	if (!fSuccess || cbWritten < sizeof(cli))
	{
		TCHAR yo[512];
		_stprintf_s(yo, 512, TEXT("WriteFile to pipe failed. GLE=%d\n"), GetLastError());
		OutputDebugString(yo);
	}

	return fSuccess;
}