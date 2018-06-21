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
	//TCHAR* lpszPipename = TEXT("\\\\.\\pipe\\mynamedpipetestes"); //alterar o pipe ponto passa para %s 
	TCHAR lpszPipename[255];
	HANDLE hUserToken = NULL;


	_stprintf_s(lpszPipename, 255, TEXT("\\\\%s\\pipe\\mynamedpipetestes"), IP_PIPE);

	//conta cenas como administrador
	BOOL log = LogonUser(TEXT("cenas"), IP_PIPE, TEXT("cenas"), LOGON32_LOGON_NEW_CREDENTIALS, LOGON32_PROVIDER_DEFAULT, &hUserToken);

	log = ImpersonateLoggedOnUser(hUserToken);

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

		// All pipe instances are busy, so wait for 20 seconds. 

		if (!WaitNamedPipe(lpszPipename, 20000))
		{
			OutputDebugString(TEXT("Could not open pipe: 20 second wait timed out."));
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

	
		ReadFile(hPipe, &update, sizeof(MsgCliGat), &cbRead, &overlRd);
		WaitForSingleObject(readReady, INFINITE);
		BOOL fSuccess = GetOverlappedResult(hPipe, &overlRd, &cbRead, FALSE);

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
			Desenhar(update);
			ReproduzirMusica(MUSICA1);
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
	HANDLE writeReady = CreateEvent(NULL, FALSE, FALSE, NULL);

	ZeroMemory(&overlWr, sizeof(overlWr));

	ResetEvent(writeReady);

	overlWr.hEvent = writeReady;

	//if(cli.tecla == ESPAÇO)
		//ReproduzirSom(MUSICA2);

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
		TCHAR debug[254];
		_stprintf_s(debug, 254, TEXT("Error Writing to PIPE: %d\n"), GetLastError());
		OutputDebugString(debug);
	}

	return fSuccess;
}