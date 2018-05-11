#include <windows.h>
#include "dll.h"


PBufferMensagens mensagens = NULL;
PJogo jogo = NULL;

void IniciaSinc() {
	MutexRead = CreateMutex(NULL, FALSE, NULL);
	MutexWrite = CreateMutex(NULL, FALSE, NULL);
	SemaforoEscrever = CreateSemaphore(NULL, MAX, MAX, NomeSemaforoPodeLer); //vazios
	SemaforoLer = CreateSemaphore(NULL, 0, MAX, NomeSemaforoPodeEscrever); //is
	hMutexJogoSer = CreateMutex(NULL, FALSE, NULL);
	hMutexJogoCli = CreateMutex(NULL, FALSE, NULL);
	hEventActiva = CreateEvent(NULL,TRUE, FALSE, NULL);
	hEventLida = CreateEvent(NULL,TRUE, FALSE, NULL);

	if (SemaforoEscrever == NULL || SemaforoLer == NULL || MutexRead == NULL || MutexWrite == NULL || hMutexJogoCli == NULL || hMutexJogoSer == NULL || hEventActiva == NULL || hEventLida == NULL) {
		_tprintf(TEXT("[Erro]Criação de objectos do Windows(%d)\n"), GetLastError());
		return -1;
	}

}

void AcabaSinc() {
	CloseHandle(SemaforoEscrever);
	CloseHandle(SemaforoLer);
	CloseHandle(hMemoriaBuffer);
	CloseHandle(hMemoriaJogo);
	CloseHandle(hMutexJogoSer);
	CloseHandle(hMutexJogoCli);
	CloseHandle(hEventActiva);
	CloseHandle(hEventLida);
}

void EnviaMensagem() {
	int x;
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
	int x;
	WaitForSingleObject(SemaforoLer, INFINITE);
	WaitForSingleObject(MutexWrite, INFINITE);  //isto é que fica como função na dll
	_tprintf(_TEXT("Mensagem: %s \n"), mensagens->buffer[mensagens->out].tecla);
	mensagens->buffer[mensagens->out].id = 0;
	mensagens->contadorMensagens--;
	mensagens->out = (mensagens->out + 1) % MAX;
	ReleaseMutex(MutexWrite);
	ReleaseSemaphore(SemaforoEscrever, 1, NULL);
}
