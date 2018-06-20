#include "gateway.h"
#include "../DlltpSO2/dll.h"


int _tmain(int argc, LPTSTR argv[]) {



#ifdef UNICODE 
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);

#endif

	

	hThreadEscritor = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)arrancaComunicacaoGateway, NULL, 0, NULL);

	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)EnviaUpdateCliente, NULL, 0, NULL);

		jogo->Invaders[20].id_invader = 123;


	WaitForSingleObject(hThreadEscritor, INFINITE);
	_tprintf(TEXT("[Thread Principal %d]Finalmente vou terminar..."), GetCurrentThreadId());

	return 0;
}




