#include "servidor.h"

DWORD WINAPI ThreadRead(LPVOID param) {
	return 0;
}

DWORD WINAPI ThreadWrite(LPVOID param) {
	return 0;
}

void EscreveMensagem(MsgCLI cli) {

}

int _tmain(int argc, LPTSTR argv[]) {

	TCHAR resp;
	DWORD threadId;
	MsgCLI Buffer[MAX];
	Jogo Jogo;

	MutexRead = CreateMutex(NULL, FALSE, NULL);
	MutexWrite = CreateMutex(NULL, FALSE, NULL);
	SemaforoRead = CreateSemaphore(NULL, MAX, MAX, TEXT("SemLeR "));
	SemaforoWrite = CreateSemaphore(NULL, 0, MAX, TEXT("SemEscrever"));

	in = 0;
	out = 0;
	continua = 1;


#ifdef UNICODE 
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);

#endif

	_tprintf(TEXT("Mapear memória?"));
	_tscanf_s(TEXT("%c"), &resp, 1);
	if (resp == 'S' || resp == 's') {

		hMemoriaBuffer = CreateFileMapping(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,sizeof(Buffer),NULL);
		hMemoriaJogo = CreateFileMapping(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,sizeof(Jogo),NULL);

		if (SemaforoRead == NULL || SemaforoWrite == NULL || hMemoriaBuffer == NULL || hMemoriaJogo == NULL) {
			_tprintf(TEXT("[Erro]Criação de objectos do Windows(%d)\n"), GetLastError());
			return -1;
		}

		PtrBuffer = (MsgCLI(*)[MAX][sizeof(MsgCLI)])MapViewOfFile(hMemoriaBuffer, FILE_MAP_READ | FILE_MAP_WRITE,0,0,sizeof(Buffer));

		if (PtrBuffer == NULL) {
			_tprintf(TEXT("[Erro]Mapeamento da memória partilhada no buffer (%d)\n"), GetLastError());
			return -1;
		}

		hThreadEscritor = CreateThread(NULL, 0,(LPTHREAD_START_ROUTINE)ThreadWrite,NULL,0,&threadId);
		if (hThreadEscritor != NULL)
			_tprintf(TEXT("Lancei uma thread com id %d\n"), threadId);
		else {
			_tprintf(TEXT("Erro ao criar Thread Escritor\n"));
			return -1;
		}

	//	PtrJogo = MapViewOfFile(hMemoriaJogo, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, sizeof(Jogo)); 
		//Isto será só assim???????????????????????????????????????????????????'

		if (PtrJogo == NULL) {
			_tprintf(TEXT("[Erro]Mapeamento da memória partilhada no jogo (%d)\n"), GetLastError());
			return -1;
		}
	}

	WaitForSingleObject(hThreadEscritor,INFINITE);
	_tprintf(TEXT("[Thread Principal %d]Finalmente vou terminar..."), GetCurrentThreadId());
	return 0;

	UnmapViewOfFile(PtrBuffer);
	//UnmapViewOfFile(PtrJogo);
	CloseHandle(SemaforoRead);
	CloseHandle(SemaforoWrite);
	CloseHandle(hMemoriaBuffer);
	CloseHandle(hMemoriaJogo);
	return 0;
}