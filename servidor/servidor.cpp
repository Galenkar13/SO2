
#include "../dllTP/stdafx.h"
#pragma comment(lib,"../x64/Debug/dllTP.lib")
#include "servidor.h"

int soma(int x, int y);

int _tmain(int argc, LPTSTR argv[]) {

	_tprintf(TEXT("RESULTADO DA SOMA %d\n"),soma(4,5));

//	TCHAR resp;
	DWORD threadId;
	int numInvaders = 0, numDefenders = 0, numDisparos = 0, numPowerUps = 0;
	int numInvadersBase = 0, numInvadersEsquivo = 0, numInvadersOutros = 0;

	MutexRead = CreateMutex(NULL, FALSE, NULL);
	MutexWrite = CreateMutex(NULL, FALSE, NULL);
	SemaforoEscrever = CreateSemaphore(NULL, MAX, MAX, NomeSemaforoPodeLer); //vazios
	SemaforoLer = CreateSemaphore(NULL, 0, MAX, NomeSemaforoPodeEscrever); //is

#ifdef UNICODE 
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);

#endif

	//Input dos valores para o Jogo
	_tprintf(TEXT("Introduza o número de Invaders: \n"));
	//scanf("%d", &numInvaders);
	_tscanf_s(TEXT("%d"), &numInvaders);


	numInvadersEsquivo = (int) (numInvaders * 0.10); //Pode ser porque o MIN está defenido com 10
	numInvadersOutros = (int) (numInvaders * 0.10);
	numInvaders = numInvaders - numInvadersEsquivo;
	_tprintf(TEXT("Numero Esquivos: %d \n"), numInvadersEsquivo);

	
	numInvaders = numInvaders - numInvadersOutros;
	_tprintf(TEXT("Numero Outros: %d \n"), numInvadersOutros);

	numInvadersBase = numInvaders;
	_tprintf(TEXT("Numero Base: %d \n"), numInvaders);

	_tprintf(TEXT("Introduza o número de Defenders:\n"));
	_tscanf_s(TEXT("%d"), &numDefenders);
	_tprintf(TEXT("Introduza o número de Disparos:\n"));
	_tscanf_s(TEXT("%d"), &numDisparos);
	_tprintf(TEXT("Introduza o número de PowerUps:\n"));
	_tscanf_s(TEXT("%d"), &numPowerUps);

	hMemoriaBuffer = CreateFileMapping(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,TAMANHOBUFFER,mPartilhadaMensagens); //onde está o invalid tbm posso guardar num dados.txt
	hMemoriaJogo = CreateFileMapping(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,sizeof(Jogo),NULL);

		if (SemaforoEscrever == NULL || SemaforoLer == NULL || hMemoriaBuffer == NULL || hMemoriaJogo == NULL) {
			_tprintf(TEXT("[Erro]Criação de objectos do Windows(%d)\n"), GetLastError());
			return -1;
		}

		mensagens = (PBufferMensagens)MapViewOfFile(hMemoriaBuffer, FILE_MAP_READ | FILE_MAP_WRITE,0,0,0);

		if (mensagens == NULL) {
			_tprintf(TEXT("[Erro]Mapeamento da memória partilhada no buffer (%d)\n"), GetLastError());
			return -1;
		}

		mensagens->in = 0;
		mensagens->out = 0;
		mensagens->contadorMensagens = 0;
	

		hThreadLeitor = CreateThread(NULL, 0,(LPTHREAD_START_ROUTINE)ThreadConsumidor,NULL,0,&threadId);
		if (hThreadLeitor != NULL)
			_tprintf(TEXT("Lancei uma thread com id %d\n"), threadId);
		else {
			_tprintf(TEXT("Erro ao criar Thread Escritor\n"));
			return -1;
		}

		jogo = (PJogo)MapViewOfFile(hMemoriaJogo, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);

		if (jogo == NULL) {
			_tprintf(TEXT("[Erro]Mapeamento da memória partilhada ´do Jogo (%d)\n"), GetLastError());
			return -1;
		}
		
		hThreadInvadersBase = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadConsumidor, NULL, 0, &threadId);

	WaitForSingleObject(hThreadLeitor,INFINITE);
	_tprintf(TEXT("[Thread Principal %d]Finalmente vou terminar..."), GetCurrentThreadId());

	UnmapViewOfFile(mensagens);
	UnmapViewOfFile(jogo);
	CloseHandle(SemaforoEscrever);
	CloseHandle(SemaforoLer);
	CloseHandle(hMemoriaBuffer);
	CloseHandle(hMemoriaJogo);
	return 0;
}

/*
DWORD WINAPI ThreadProdutor(LPVOID param) { //LADO DO GATEWAY
	TCHAR cenas[1];
	_tprintf(TEXT("Mensagem para memoria partilhada?"));
	_tscanf_s(TEXT("%c"), &cenas, 1);
	while (continua == 1) { //Isto é para passar para a DLL
		WaitForSingleObject(SemaforoEscrever, INFINITE);
		WaitForSingleObject(MutexRead, INFINITE);
		//mensagens->buffer[mensagens->in] = cenas;
		_tcscpy_s(mensagens->buffer[mensagens->out].tecla, sizeof(mensagens->buffer[mensagens->out].tecla), cenas);
		mensagens->contadorMensagens++;
		mensagens->in = (mensagens->in + 1) % MAX;
		ReleaseMutex(MutexRead);
		ReleaseSemaphore(SemaforoLer, 1, NULL);
	}
	return 0;
}

DWORD WINAPI ThreadConsumidor(LPVOID param) { //LADO DO SERVIDOR
	TCHAR cenas[SIZE];
	while (continua == 1) { //Isto é para passar para a DLL
		WaitForSingleObject(SemaforoLer, INFINITE);
		WaitForSingleObject(MutexWrite, INFINITE);
		//cenas = mensagens->buffer[mensagens->out];
		_tcscpy_s(cenas, sizeof(cenas), mensagens->buffer[mensagens->out].tecla);
		_tprintf(_TEXT("Mensagem: %c /n"), cenas);
		_tcscpy_s(mensagens->buffer[mensagens->out].tecla, sizeof(mensagens->buffer[mensagens->out].tecla),TEXT("\0"));
		// TCHAR tecla[SIZE];
		mensagens->contadorMensagens--;
		mensagens->out = (mensagens->out + 1) % MAX;
		ReleaseMutex(MutexWrite);
		ReleaseSemaphore(SemaforoEscrever, 1, NULL);
	}
	 
	return 0;
}

*/
DWORD WINAPI ThreadConsumidor(LPVOID param) { //LADO DO SERVIDOR
	int x = 0;
	while (continua == 1) { //Isto é para passar para a DLL
	
		WaitForSingleObject(SemaforoLer, INFINITE);
		WaitForSingleObject(MutexWrite, INFINITE);
		x = mensagens->buffer[mensagens->out].id;
		_tprintf(_TEXT("Mensagem: %d \n"), x);
		mensagens->buffer[mensagens->out].id = 0;
		mensagens->contadorMensagens--;
		mensagens->out = (mensagens->out + 1) % MAX;
		ReleaseMutex(MutexWrite);
		ReleaseSemaphore(SemaforoEscrever, 1, NULL);
	}

	return 0;
}

DWORD WINAPI ThreadInvadersBase(LPVOID param) { 
	return 0;
}

DWORD WINAPI ThreadInvadersEsquivo(LPVOID param) {
	return 0;
}

DWORD WINAPI ThreadInvadersOutro(LPVOID param) {
	return 0;
}