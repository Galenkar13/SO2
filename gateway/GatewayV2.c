#include "gateway.h"


int _tmain(int argc, LPTSTR argv[]) {

//	TCHAR resp;
	DWORD threadId;

	//	Jogo Jogo;


	IniciaSinc();

#ifdef UNICODE 
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);

#endif

	hMemoriaBuffer = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, TAMANHOBUFFER, mPartilhadaMensagens); //onde está o invalid tbm posso guardar num dados.txt
	hMemoriaJogo = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(Jogo), TEXT("memPartilhadaJogo"));
//	_tprintf(TEXT("[Erro]Criação de objectos do Windows(%d)\n"), &hMemoriaJogo);


	if (hMemoriaBuffer == NULL || hMemoriaJogo == NULL) {
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

		hThreadEscritor = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadProdutor, NULL, 0, &threadId);
		if (hThreadEscritor!= NULL)
			_tprintf(TEXT("Lancei uma thread com id %d\n"), threadId);
		else {
			_tprintf(TEXT("Erro ao criar Thread Escritor\n"));
			return -1;
		}

		hThreadRecebeAtualizacao = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadAtualizacao, NULL, 0, 0);
		if (hThreadRecebeAtualizacao != NULL)
			_tprintf(TEXT("Lancei uma thread com id %d\n"), threadId);
		else {
			_tprintf(TEXT("Erro ao criar Thread Escritor\n"));
			return -1;
		}

		jogo->Invaders[20].id_invader = 123;


	WaitForSingleObject(hThreadEscritor, INFINITE);
	_tprintf(TEXT("[Thread Principal %d]Finalmente vou terminar..."), GetCurrentThreadId());

	UnmapViewOfFile(mensagens);
	UnmapViewOfFile(jogo);
	AcabaSinc();
	return 0;
}



DWORD WINAPI ThreadProdutor(LPVOID param) { //LADO DO GATEWAY

	while (1) { //Isto é para passar para a DLL
		EnviaMensagem();
	}
	return 0;
}

DWORD WINAPI ThreadAtualizacao(LPVOID param) { //LADO DO GATEWAY

	while (1) { //Isto é para passar para a DLL
		RecebeAtualizacao(10);
	}
	return 0;
}


