#include "gateway.h"



int _tmain(int argc, LPTSTR argv[]) {

	TCHAR resp;
	DWORD threadId;

	//	Jogo Jogo;

	MutexRead = CreateMutex(NULL, FALSE, NULL);
	MutexWrite = CreateMutex(NULL, FALSE, NULL);
	SemaforoEscrever = CreateSemaphore(NULL, MAX, MAX, NomeSemaforoPodeLer); //vazios
	SemaforoLer = CreateSemaphore(NULL, 0, MAX, NomeSemaforoPodeEscrever); //is


#ifdef UNICODE 
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);

#endif

	hMemoriaBuffer = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, TAMANHOBUFFER, mPartilhadaMensagens); //onde está o invalid tbm posso guardar num dados.txt
	hMemoriaJogo = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(Jogo), NULL);


	if (SemaforoEscrever == NULL || SemaforoLer == NULL || hMemoriaBuffer == NULL || hMemoriaJogo == NULL) {
		_tprintf(TEXT("[Erro]Criação de objectos do Windows(%d)\n"), GetLastError());
		return -1;
	}

		mensagens = (PBufferMensagens)MapViewOfFile(hMemoriaBuffer, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);

		if (mensagens == NULL) {
			_tprintf(TEXT("[Erro]Mapeamento da memória partilhada no buffer (%d)\n"), GetLastError());
			return -1;
		}

		jogo = (PJogo)MapViewOfFile(hMemoriaJogo, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);

		if (jogo == NULL) {
			_tprintf(TEXT("[Erro]Mapeamento da memória partilhada ´do Jogo (%d)\n"), GetLastError());
			return -1;
		}

		hThreadLeitor = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadProdutor, NULL, 0, &threadId);
		if (hThreadLeitor != NULL)
			_tprintf(TEXT("Lancei uma thread com id %d\n"), threadId);
		else {
			_tprintf(TEXT("Erro ao criar Thread Escritor\n"));
			return -1;
		}

		//	PtrJogo = MapViewOfFile(hMemoriaJogo, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, sizeof(Jogo)); 
		//Isto será só assim???????????????????????????????????????????????????'

		//	if (PtrJogo == NULL) {
		//	_tprintf(TEXT("[Erro]Mapeamento da memória partilhada no jogo (%d)\n"), GetLastError());
		//	return -1;
	
	//}

	WaitForSingleObject(hThreadLeitor, INFINITE);
	_tprintf(TEXT("[Thread Principal %d]Finalmente vou terminar..."), GetCurrentThreadId());

	UnmapViewOfFile(mensagens);
	//UnmapViewOfFile(PtrJogo);
	CloseHandle(SemaforoEscrever);
	CloseHandle(SemaforoLer);
	CloseHandle(hMemoriaBuffer);
	CloseHandle(hMemoriaJogo);
	return 0;
}

/*
DWORD WINAPI ThreadProdutor(LPVOID param) { //LADO DO GATEWAY
	TCHAR cenas[SIZE];
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
*/

DWORD WINAPI ThreadProdutor(LPVOID param) { //LADO DO GATEWAY
	int x;
	while (continua == 1) { //Isto é para passar para a DLL
		_tprintf(TEXT("Introduzir numero memoria partilhadas?"));
		_tscanf_s(TEXT("%d"), &x);
		WaitForSingleObject(SemaforoEscrever, INFINITE);
		WaitForSingleObject(MutexRead, INFINITE);
		//mensagens->buffer[mensagens->in] = cenas;
	//	_tcscpy_s(mensagens->buffer[mensagens->out].tecla, sizeof(mensagens->buffer[mensagens->out].tecla), cenas);
		mensagens->buffer[mensagens->in].id = x;
		mensagens->contadorMensagens++;
		mensagens->in = (mensagens->in + 1) % MAX;
		ReleaseMutex(MutexRead);
		ReleaseSemaphore(SemaforoLer, 1, NULL);
	}
	return 0;
}