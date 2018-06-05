#include "servidor.h"



int _tmain(int argc, LPTSTR argv[]) {


	DWORD threadId;
	Input Input;
	HANDLE hEvento, hMutexJogo;
	TCHAR command[80];

#ifdef UNICODE 
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);

#endif

	continua = 1;
	hMutexJogo = CreateMutex(NULL, TRUE, TEXT("GoMutex"));
	hEvento = CreateEvent(NULL, TRUE, FALSE, TEXT("GoEvent"));

	hThreadLeitor = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadConsumidor, NULL, 0, &threadId);
	if (hThreadLeitor != NULL)
		_tprintf(TEXT("Lancei uma thread com id %d\n"), threadId);
	else {
		_tprintf(TEXT("Erro ao criar Thread Escritor\n"));
		return -1;
	}

	_tprintf(TEXT("Command:"));
	//_tscanf_s(TEXT(" %s"), command, 80);
	_fgetts(command, 80, stdin);
	command[_tcslen(command) - 1] = '\0';

	

	//Isto futuramente vai estar dentro de um ciclo de jogo 
	//Ter em anteção os niveis na criação desse mesmo ciclo

	Input = RecebeInput();


	_tprintf(TEXT("Numero Esquivos: %d \n"), Input.numInvadersEsquivo);

	_tprintf(TEXT("Numero Extras: %d \n"), Input.numInvadersOutros);

	_tprintf(TEXT("Numero Base: %d \n"), Input.numInvadersBase);

	IniciaInvaders(Input); //Inicializa valores dos invaders
	ColocaInvaders(Input);		//Coloca invaders nas posições correctas


	WaitForSingleObject(hMutexJogo, INFINITE);

	jogo->altura = 2345;
	_tprintf(TEXT("[Event Generated] \n"));

	SetEvent(hEvento);
	ResetEvent(hEvento);
	//Sleep(3*1000);
	ReleaseMutex(hMutexJogo);


	for (int j = 0; j < Input.numInvaders; j++) {
		_tprintf(TEXT("Invader %d: x - %d    y - %d \n"), j, jogo->Invaders[j].area.x, jogo->Invaders[j].area.y);
	}

	CreateThreadsInvaders(); //Cria Thread por tipo de Invader


	WaitForSingleObject(hThreadLeitor, INFINITE);
	_tprintf(TEXT("[Thread Principal %d]Finalmente vou terminar..."), GetCurrentThreadId());


	//AcabaSinc();
	return 0;
}

int CreateThreadsInvaders() {

	hThreadInvadersBase = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadInvadersBase, 0, 0, NULL);
	hThreadInvadersEsquivos = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadInvadersEsquivo, 0, 0, NULL);
	hThreadsInvadersExtras = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadInvadersExtra, 0, 0, NULL);

	if (hThreadInvadersBase == NULL || hThreadInvadersEsquivos == NULL || hThreadsInvadersExtras == NULL) {
		_tprintf(TEXT("[Erro]Criação de objectos do Windows(%d)\n"), GetLastError());
		return -1;
	}

	return 0;

}

DWORD WINAPI ThreadInvadersBase() {
	_tprintf(TEXT("Iniciei Thread Tipo Invaders Base \n"));

	while (continua == 1) {

		MoveInvaderBase(jogo->Invaders[10].id_invader, jogo->Invaders[10].area.x, jogo->Invaders[10].area.y, MaxInvaders);
		_tprintf(TEXT("%d %d %d \n"), jogo->Invaders[10].id_invader, jogo->Invaders[10].area.x, jogo->Invaders[10].area.y);

		Sleep(velocidadeInvaderBase);
	}
	return 0;
}

DWORD WINAPI ThreadInvadersEsquivo() {
	_tprintf(TEXT("Iniciei Thread Tipo Invaders Esquivo \n"));
	/*
	int i;
	do
	int contador=0
	for (i = 0; i < inp.numInvaders; i++) {
		if (jogo->Invaders[i].tipo == ESQUIVO) {
			//se tiver espaço
			//movimentoInvadersEsquivo();

			//calcular probabilidade para disparar
			//se sim CreatheThreadDisparo();


		}
		Sleep(jogo->Invaders[i]);
	}
	while (contador != inp.numInvadersEsquivos)

	*/
	return 0;
}

DWORD WINAPI ThreadInvadersExtra() {
	_tprintf(TEXT("Iniciei Thread Tipo Invaders Extra \n"));
	/*
	int i;
	do
	int contador=0
	for (i = 0; i < inp.numInvaders; i++) {
	if (jogo->Invaders[i].tipo == Extra) {
	//se tiver espaço
	//movimentoInvadersEsquivo();

	//calcular probabilidade para disparar
	//se sim CreatheThreadDisparo();


	}
	Sleep(jogo->Invaders[i]);
	}
	while (contador != inp.numInvadersExtra)

	*/
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
		else
		{
			aux.numInvadersEsquivo = 20;
			aux.numInvadersOutros = 0;
			aux.numInvadersBase = aux.numInvaders - (aux.numInvadersEsquivo + aux.numInvadersOutros);
		}

	return aux;
}

void InicializaJogo() { //acabar isto
	int i;

	for (i = 0; i < MaxClientes; i++) {
		jogo->Defenders[i].area.x = 0;
		jogo->Defenders[i].area.y = 0;
		jogo->Defenders[i].area.altura = 0;
		jogo->Defenders[i].area.comprimento = 0;
		jogo->Defenders[i].pontos = 0;
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


void IniciaInvaders(Input inp) {
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
			jogo->Invaders[i].id_invader = i;
		}
		else
			if (i < inp.numInvadersBase + inp.numInvadersEsquivo) {
				jogo->Invaders[i].area.x = 0;
				jogo->Invaders[i].area.y = 0;
				jogo->Invaders[i].area.altura = AlturaInvader;
				jogo->Invaders[i].area.comprimento = ComprimentoInvader;
				jogo->Invaders[i].tipo = ESQUIVO;
				jogo->Invaders[i].velocidade = (int)(velocidadeInvaderBase * 0.40);
				jogo->Invaders[i].vidas = 3;
				jogo->Invaders[i].id_invader = i;
			}
			else
			{
				jogo->Invaders[i].area.x = 0;
				jogo->Invaders[i].area.y = 0;
				jogo->Invaders[i].area.altura = 0;
				jogo->Invaders[i].area.comprimento = 0;
				jogo->Invaders[i].tipo = EXTRA;
				jogo->Invaders[i].velocidade = 0;
				jogo->Invaders[i].vidas = 0;
				jogo->Invaders[i].id_invader = i;
			}
	}
}

void ColocaInvaders(Input inp) { //Esta funcao nao esta automatizada porque ainda não temos o jogo totalmente incializado
	int i;
	int espaco = 2;
	int flag = 1;
	int fila = 1;
	int comp = ComprimentoJanelaMAX;
	for (i = 0; i < inp.numInvaders; i++) {

		if (flag == 1) {
			jogo->Invaders[i].area.x = espaco;
			if (fila == 1)
				jogo->Invaders[i].area.y = (fila * espaco);
			else
				jogo->Invaders[i].area.y = (fila * espaco) + (jogo->Invaders[i].area.altura *(fila - 1));
			flag = 0;
		}
		else
		{
			jogo->Invaders[i].area.x = jogo->Invaders[i - 1].area.x + jogo->Invaders[i].area.comprimento + espaco;
			if (fila == 1)
				jogo->Invaders[i].area.y = (fila * espaco);
			else
				jogo->Invaders[i].area.y = (fila * espaco) + (jogo->Invaders[i].area.altura *(fila - 1));

			if ((jogo->Invaders[i].area.x + jogo->Invaders[i].area.comprimento) >= (comp - espaco)) {
				fila++;
				flag = 1;
				i--;
			}
		}
	}
}
