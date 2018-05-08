#include "servidor.h"
#include "../DlltpSO2/dll.h"


int _tmain(int argc, LPTSTR argv[]) {

//	TCHAR resp;
	DWORD threadId;
	Input Input;

	MutexRead = CreateMutex(NULL, FALSE, NULL);
	MutexWrite = CreateMutex(NULL, FALSE, NULL);
	SemaforoEscrever = CreateSemaphore(NULL, MAX, MAX, NomeSemaforoPodeLer); //vazios
	SemaforoLer = CreateSemaphore(NULL, 0, MAX, NomeSemaforoPodeEscrever); //is

#ifdef UNICODE 
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);

#endif


	Input = RecebeInput();

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

//Tenho de pensar melhor sobre estes algoritmos
//Provavelmente uma função é que vai inicializar os arrays e não aqui

DWORD WINAPI ThreadInvadersBase(int num) { 
	int i ;

	return 0;
}

DWORD WINAPI ThreadInvadersEsquivo(int num) {
	return 0;
}

DWORD WINAPI ThreadInvadersOutro(int num) {
	return 0;
}

Input RecebeInput() {
	Input aux;
	//Input dos valores para o Jogo
	_tprintf(TEXT("Introduza o número de Invaders: \n"));
	//scanf("%d", &numInvaders);
	_tscanf_s(TEXT("%d"), &aux.numInvaders);

	aux.numInvadersEsquivo = aux.numInvaders * 0.10; //Pode ser porque o MIN está defenido com 10
	aux.numInvadersOutros = aux.numInvaders * 0.10;
	aux.numInvaders = aux.numInvaders - aux.numInvadersEsquivo;
	_tprintf(TEXT("Numero Esquivos: %d \n"), aux.numInvadersEsquivo);


	aux.numInvaders = aux.numInvaders - aux.numInvadersOutros;
	_tprintf(TEXT("Numero Outros: %d \n"), aux.numInvadersOutros);

	aux.numInvadersBase = aux.numInvaders;
	_tprintf(TEXT("Numero Base: %d \n"), aux.numInvaders);

	_tprintf(TEXT("Introduza o número de Defenders:\n"));
	_tscanf_s(TEXT("%d"), &aux.numDefenders);
	_tprintf(TEXT("Introduza o número de Disparos:\n"));
	_tscanf_s(TEXT("%d"), &aux.numDisparos);
	_tprintf(TEXT("Introduza o número de PowerUps:\n"));
	_tscanf_s(TEXT("%d"), &aux.numPowerUps);

	return aux;
}

/*
PInput RecebeInput() {
PInput aux;
//Input dos valores para o Jogo
_tprintf(TEXT("Introduza o número de Invaders: \n"));
//scanf("%d", &numInvaders);
_tscanf_s(TEXT("%d"), aux->numInvaders);

aux->numInvadersEsquivo = aux->numInvaders * 0.10; //Pode ser porque o MIN está defenido com 10
aux->numInvadersOutros = aux->numInvaders * 0.10;
aux->numInvaders = aux->numInvaders - aux->numInvadersEsquivo;
_tprintf(TEXT("Numero Esquivos: %d \n"), aux->numInvadersEsquivo);


aux->numInvaders = aux->numInvaders - aux->numInvadersOutros;
_tprintf(TEXT("Numero Outros: %d \n"), aux->numInvadersOutros);

aux->numInvadersBase = aux->numInvaders;
_tprintf(TEXT("Numero Base: %d \n"), aux->numInvaders);

_tprintf(TEXT("Introduza o número de Defenders:\n"));
_tscanf_s(TEXT("%d"), aux->numDefenders);
_tprintf(TEXT("Introduza o número de Disparos:\n"));
_tscanf_s(TEXT("%d"), aux->numDisparos);
_tprintf(TEXT("Introduza o número de PowerUps:\n"));
_tscanf_s(TEXT("%d"), aux->numPowerUps);

return aux;
}
*/

//É preciso fazer isto?

void InicializaJogo() { //acabar isto
	int i;
	for (i = 0; i < MaxInvaders - 1; i++) { //mudar isto tudo, é só exemplo
			jogo->Invaders[i].area.x = 0;
			jogo->Invaders[i].area.y = 0;
			jogo->Invaders[i].area.altura = 0;
			jogo->Invaders[i].area.comprimento = 0;
			jogo->Invaders[i].tipo = BASICO;
			jogo->Invaders[i].velocidade = 0;
			jogo->Invaders[i].vidas = 0;

	}

	for (i = 0; i < MaxClientes; i++) {
		jogo->Defenders[i].area.x = 0;
		jogo->Defenders[i].area.y = 0;
		jogo->Defenders[i].area.altura = 0;
		jogo->Defenders[i].area.comprimento = 0;
		jogo->Defenders[i].pontos.pontos = 0;
		jogo->Defenders[i].velocidade = 0;
		jogo->Defenders[i].vidas = 0;
		jogo->Defenders[i].powerUP.area.x = 0;
	}
	return 0;

}