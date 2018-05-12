#define DlltpSO2_EXPORTS 
#include <windows.h>	
#include "dll.h"

PBufferMensagens mensagens = NULL;
PJogo jogo = NULL;



int IniciaSinc() {
	MutexRead = CreateMutex(NULL, FALSE, Mutex4);
	MutexWrite = CreateMutex(NULL, FALSE, Mutex3);
	SemaforoEscrever = CreateSemaphore(NULL, MAX, MAX, NomeSemaforoPodeLer); //vazios
	SemaforoLer = CreateSemaphore(NULL, 0, MAX, NomeSemaforoPodeEscrever); //is
	hMutexJogo = CreateMutex(NULL, FALSE, Mutex1);
	hMutexJogo2 = CreateMutex(NULL, FALSE, Mutex2);
	hEventActiva = CreateEvent(NULL,TRUE, FALSE, EventoManda);
	hEventLida = CreateEvent(NULL,TRUE, FALSE, EventoRecebe);

	if (SemaforoEscrever == NULL || SemaforoLer == NULL || MutexRead == NULL || MutexWrite == NULL || hMutexJogo2 == NULL || hMutexJogo == NULL || hEventActiva == NULL || hEventLida == NULL) {
		_tprintf(TEXT("[Erro]Criação de objectos do Windows(%d)\n"), GetLastError());
		return -1;
	}

	return 0;
}

void AcabaSinc() {
	CloseHandle(SemaforoEscrever);
	CloseHandle(SemaforoLer);
	CloseHandle(hMemoriaBuffer);
	CloseHandle(hMemoriaJogo);
	CloseHandle(hMutexJogo);
	CloseHandle(hMutexJogo2);
	CloseHandle(hEventActiva);
	CloseHandle(hEventLida);
}

void EnviaMensagem() {
		TCHAR msg[TAM];
	_tprintf(TEXT("Inserir mensagem teste memoria partilhada?"));
	_fgetts(msg, TAM, stdin);
	WaitForSingleObject(SemaforoEscrever, INFINITE);
	WaitForSingleObject(MutexRead, INFINITE);
	//mensagens->buffer[mensagens->in] = cenas;
	//	_tcscpy_s(mensagens->buffer[mensagens->out].tecla, sizeof(mensagens->buffer[mensagens->out].tecla), cenas);
	_tcscpy_s(mensagens->buffer[mensagens->out].tecla, (sizeof(TCHAR)* TAM), msg);
	mensagens->contadorMensagens++;
	mensagens->in = (mensagens->in + 1) % MAX;
	ReleaseMutex(MutexRead);
	ReleaseSemaphore(SemaforoLer, 1, NULL);
}

void TrataMensagem() {
	WaitForSingleObject(SemaforoLer, INFINITE);
	WaitForSingleObject(MutexWrite, INFINITE);  //isto é que fica como função na dll
	_tprintf(_TEXT("Mensagem: %s \n"), mensagens->buffer[mensagens->out].tecla);
	mensagens->buffer[mensagens->out].id = 0;
	mensagens->contadorMensagens--;
	mensagens->out = (mensagens->out + 1) % MAX;
	ReleaseMutex(MutexWrite);
	ReleaseSemaphore(SemaforoEscrever, 1, NULL);
}




void MoveInvaderBase(int id, int x, int y, int num) {
	//	_tprintf(TEXT("cenas2345 \n"));

	int nRandonNumber = rand() % 3;
	int i;

	//	_tprintf(TEXT("cenas123 \n"));

	WaitForSingleObject(hMutexJogo, INFINITE);
	SetEvent(hEventActiva);


	for (i = 0; i < num; i++)
	{

		if (jogo->Invaders[i].id_invader == id) {

			if (nRandonNumber == (int)direita)
			{
				jogo->Invaders[i].area.x = jogo->Invaders[i].area.x + MOVEMENT_INCREMENT;
			}
			if (nRandonNumber == (int)esquerda)
			{
				jogo->Invaders[i].area.x = jogo->Invaders[i].area.x - MOVEMENT_INCREMENT;
			}
			if (nRandonNumber == (int)baixo)
			{
				jogo->Invaders[i].area.y = jogo->Invaders[i].area.y + MOVEMENT_INCREMENT;
			}
			if (nRandonNumber == (int)cima)
			{
				jogo->Invaders[i].area.y = jogo->Invaders[i].area.y - MOVEMENT_INCREMENT;
			}
		}
	}
	WaitForSingleObject(hMutexJogo2, INFINITE);

	ReleaseMutex(hMutexJogo2);
	ResetEvent(hEventActiva);


	ReleaseMutex(hMutexJogo);


	WaitForSingleObject(hEventLida, INFINITE);
}


void RecebeAtualizacao(int id) {

	WaitForSingleObject(hEventActiva, INFINITE);
	ResetEvent(hEventLida);
	ReleaseMutex(hMutexJogo2);


	WaitForSingleObject(hMutexJogo, INFINITE);
	WaitForSingleObject(hMutexJogo2, INFINITE);

	_tprintf(TEXT("Invader %d: x - %d    y - %d \n"), jogo->Invaders[10].id_invader, jogo->Invaders[10].area.x, jogo->Invaders[10].area.y);

	SetEvent(hEventLida);
	ReleaseMutex(hMutexJogo);

}
