#include "gateway.h"
#include "../DlltpSO2/dll.h"


int _tmain(int argc, LPTSTR argv[]) {

//	TCHAR resp;
	DWORD threadId;

	//	Jogo Jogo;


#ifdef UNICODE 
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);

#endif

	

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


