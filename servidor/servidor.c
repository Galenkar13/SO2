#include "servidor.h"



int _tmain(int argc, LPTSTR argv[]) {


	DWORD threadId;
	Input Input;

	IniciaSinc();

#ifdef UNICODE 
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);

#endif

	continua = 1;
	
	Input = RecebeInput();

	_tprintf(TEXT("Numero Esquivos: %d \n"), Input.numInvadersEsquivo);

	_tprintf(TEXT("Numero Outros: %d \n"), Input.numInvadersOutros);

	_tprintf(TEXT("Numero Base: %d \n"), Input.numInvadersBase);

	hMemoriaBuffer = CreateFileMapping(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,TAMANHOBUFFER,mPartilhadaMensagens); //onde está o invalid tbm posso guardar num dados.txt
	hMemoriaJogo = CreateFileMapping(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,sizeof(Jogo),NULL);

	if (hMemoriaBuffer == NULL || hMemoriaJogo == NULL) {
		_tprintf(TEXT("[Erro]Criação de objectos do Windows(%d)\n"), GetLastError());
		return -1;
	}
		mensagens = (PBufferMensagens)MapViewOfFile(hMemoriaBuffer, FILE_MAP_READ | FILE_MAP_WRITE,0,0,0);

		if (mensagens == NULL) {
			_tprintf(TEXT("[Erro]Mapeamento da memória partilhada no buffer (%d)\n"), GetLastError());
			return -1;
		}

		IniciaBuffer();
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


		IniciaInvaders(Input);
		ColocaInvaders();

		//Aqui é que se cria Threads por tipo

		//CreateThreadsJogo(threadIdJogo, Input);
//		server->hThreadInvadersBase = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadInvadersBase, Input.numInvadersBase, 0, &threadIdJogo);
	//	server->hThreadInvadersEsquivos = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadInvadersEsquivo, Input.numInvadersEsquivo + Input.numInvadersBase, 0, &threadIdJogo);
		//server->hThreadsInvadersOutros = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadInvadersOutro, Input.numInvaders, 0, &threadIdJogo);

		//Uma so thread para criar todos faz mais sentido

		//função para os colocar

		//função movimento das threads só arranca depois

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

DWORD WINAPI ThreadInvadersBase(int num) { 
	int i ;
	for (i = 0; i < num; i++) {
		jogo->Invaders[i].vidas = 1;
		jogo->Invaders[i].velocidade = velocidadeInvaderBase;
		jogo->Invaders[i].tipo = BASICO;

	}
	return 0;
}

DWORD WINAPI ThreadInvadersEsquivo(int num) {
	int i;
	for (i = 0; i < num; i++) {
		if (jogo->Invaders[i].tipo != BASICO) {
			jogo->Invaders[i].vidas = 1;
			jogo->Invaders[i].velocidade = velocidadeInvaderBase;
			jogo->Invaders[i].tipo = ESQUIVO;
		}
	}
	return 0;
}

DWORD WINAPI ThreadInvadersOutro(int num) {
	int i;
	for (i = 0; i < num; i++) {
		if (jogo->Invaders[i].tipo != BASICO && jogo->Invaders[i].tipo != ESQUIVO) {
			jogo->Invaders[i].vidas = 1;
			jogo->Invaders[i].velocidade = velocidadeInvaderBase;
			jogo->Invaders[i].tipo = OUTRO;
		}
	}
	return 0;
}

Input RecebeInput() {
	Input aux;
	//Input dos valores para o Jogo
	_tprintf(TEXT("Introduza o número de Invaders: \n"));
	//scanf("%d", &numInvaders);
	_tscanf_s(TEXT("%d"), &aux.numInvaders);

	if (aux.numInvaders <= 20) {
		aux.numInvadersEsquivo = 2;
		aux.numInvadersOutros = 0;
		aux.numInvadersBase = aux.numInvaders - (aux.numInvadersEsquivo + aux.numInvadersOutros);
	}
	else 
		if (aux.numInvaders <= 40) {
			aux.numInvadersEsquivo = 10;
			aux.numInvadersOutros = 0;
			aux.numInvadersBase = aux.numInvaders - (aux.numInvadersEsquivo + aux.numInvadersOutros);
		}

	_tprintf(TEXT("Introduza o número de Defenders:\n"));
	_tscanf_s(TEXT("%d"), &aux.numDefenders);
	_tprintf(TEXT("Introduza o número de Disparos:\n"));
	_tscanf_s(TEXT("%d"), &aux.numDisparos);
	_tprintf(TEXT("Introduza o número de PowerUps:\n"));
	_tscanf_s(TEXT("%d"), &aux.numPowerUps);

	return aux;
}
/*
void CreateThreadsJogo(DWORD *threadIdJogo, Input aux) {

	hThreadInvadersBase = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadInvadersBase, aux.numInvadersBase, 0, threadIdJogo);
	hThreadInvadersEsquivos= CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadInvadersEsquivo, aux.numInvadersEsquivo + aux.numInvadersBase, 0, threadIdJogo);
	hThreadsInvadersOutros = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadInvadersOutro, aux.numInvadersOutros, 0, threadIdJogo);
	//fazer o mesmo para Defenders, disparos e powerups
}
*/
//É preciso fazer isto?

void InicializaJogo() { //acabar isto
	int i;

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

}

DWORD WINAPI ThreadConsumidor(LPVOID param) { //LADO DO SERVIDOR passar po servidor
	int x = 0;
	while (continua == 1) { //Isto é para passar para a DLL

		TrataMensagem();
	}

	return 0;
}

void IniciaBuffer() {
	int i;
	for (i = 0; i < MAX; i++) {
		mensagens->buffer[i].id = 0;
		mensagens->buffer[i].mensagem = '0';
		_tcscpy(mensagens->buffer[i].nome, TEXT("nada"));
		_tcscpy(mensagens->buffer[i].tecla, TEXT("nada"));
	}
}

void IniciaInvaders(Input inp) {
	_tprintf(TEXT("[Erro]Mapeamento da memória partilhada ´do Jogo (%d)\n"), GetLastError());
	_tprintf(TEXT("%d"), inp.numInvaders);
	int i = 0;
	for (i = 0; i < inp.numInvaders; i++) {
		if (i < inp.numInvadersBase) {
			jogo->Invaders[i].area.x = 0;
			jogo->Invaders[i].area.y = 0;
			jogo->Invaders[i].area.altura = AlturaInvader;
			jogo->Invaders[i].area.comprimento = ComprimentoInvader;
			jogo->Invaders[i].tipo = BASICO;
			jogo->Invaders[i].velocidade = velocidadeInvaderBase;
			jogo->Invaders[i].vidas = 1;
		}
		else
			if (i < inp.numInvadersBase + inp.numInvadersEsquivo) {
				jogo->Invaders[i].area.x = 0;
				jogo->Invaders[i].area.y = 0;
				jogo->Invaders[i].area.altura = AlturaInvader;
				jogo->Invaders[i].area.comprimento = ComprimentoInvader;
				jogo->Invaders[i].tipo = ESQUIVO;
				jogo->Invaders[i].velocidade = velocidadeInvaderBase * 0.40;
				jogo->Invaders[i].vidas = 3;
			}
			else
			{
				jogo->Invaders[i].area.x = 0;
				jogo->Invaders[i].area.y = 0;
				jogo->Invaders[i].area.altura = 0;
				jogo->Invaders[i].area.comprimento = 0;
				jogo->Invaders[i].tipo = OUTRO;
				jogo->Invaders[i].velocidade = 0;
				jogo->Invaders[i].vidas = 0;
			}
	}
}

void ColocaInvaders() {

}