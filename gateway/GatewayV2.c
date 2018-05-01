#include "gateway.h"



int _tmain(int argc, LPTSTR argv[]) {

	TCHAR resp;
	DWORD threadId;

	//	Jogo Jogo;

	MutexRead = CreateMutex(NULL, FALSE, NULL);
	MutexWrite = CreateMutex(NULL, FALSE, NULL);
	SemaforoVazio = CreateSemaphore(NULL, MAX, MAX, TEXT("SemLeR ")); //vazios
	SemaforoItem = CreateSemaphore(NULL, 0, MAX, TEXT("SemEscrever")); //itens


#ifdef UNICODE 
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);

#endif


		hMemoriaBuffer = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, TAMANHOBUFFER, mPartilhadaMensagens);
		//hMemoriaJogo = CreateFileMapping(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,sizeof(Jogo),NULL);

		if (SemaforoVazio == NULL || SemaforoItem == NULL || hMemoriaBuffer == NULL) {
			_tprintf(TEXT("[Erro]Criação de objectos do Windows(%d)\n"), GetLastError());
			return -1;
		}

		mensagens = (PBufferMensagens)MapViewOfFile(hMemoriaBuffer, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);

		if (mensagens == NULL) {
			_tprintf(TEXT("[Erro]Mapeamento da memória partilhada no buffer (%d)\n"), GetLastError());
			return -1;
		}

		mensagens->in = 0;
		mensagens->out = 0;
		mensagens->contadorMensagens = 0;


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
	CloseHandle(SemaforoVazio);
	CloseHandle(SemaforoItem);
	CloseHandle(hMemoriaBuffer);
	CloseHandle(hMemoriaJogo);
	return 0;
}


DWORD WINAPI ThreadProdutor(LPVOID param) { //LADO DO GATEWAY
	MsgCLI msg;
	while (continua == 1) {
		WaitForSingleObject(SemaforoVazio, INFINITE);
		_tprintf(TEXT("Mensagem para memoria partilhada?"));
		_tscanf_s(TEXT("%c"), &msg.tecla, 1);
		if (_tcsncmp(msg.tecla, TEXT("sair"), 5) == 0)
			continua = 0;
		WaitForSingleObject(MutexRead, INFINITE);
		
		mensagens->buffer[mensagens->in] = msg;
		mensagens->in = (mensagens->in + 1) % MAX;
		ReleaseMutex(MutexRead);
		ReleaseSemaphore(SemaforoItem, 1, NULL);
	}

	return 0;
}

DWORD WINAPI ThreadConsumidor(LPVOID param) { //LADO DO SERVIDOR
	MsgCLI msg;
	while (continua == 1) {
		WaitForSingleObject(SemaforoVazio, INFINITE);
		WaitForSingleObject(MutexWrite, INFINITE);
		msg = mensagens->buffer[mensagens->out];
		_tprintf(_TEXT("Mensagem: %s /n"), msg.tecla);
		strcpy_s(mensagens->buffer[mensagens->out].tecla, sizeof(mensagens->buffer[mensagens->out].tecla), NULL);
		mensagens->out = (mensagens->out + 1) % MAX;
		ReleaseMutex(MutexWrite);
		ReleaseSemaphore(SemaforoItem, 1, NULL);
	}

	return 0;
}