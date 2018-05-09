#include <windows.h>
#include "dll.h"

//Para verificar ao carregar a dll que a aplicação irá ocupar mais memória
char ponteiro[40960];
//Definição da variável global
int nDLL = 1234;
//Exportar a função para ser utilizada fora da DLL
int UmaString(void) {
	TCHAR str[TAM];
	_tprintf(TEXT("Dentro da Dll\nIntroduza uma frase:"));
	_fgetts(str, TAM, stdin);
	if (_tcslen(str) > 1) //Introduzir mais caracteres do que apenas <enter>
		return 1;
	else
		return 0;
}

PBufferMensagens mensagens = NULL;
PJogo jogo = NULL;

void IniciaSinc() {
	MutexRead = CreateMutex(NULL, FALSE, NULL);
	MutexWrite = CreateMutex(NULL, FALSE, NULL);
	SemaforoEscrever = CreateSemaphore(NULL, MAX, MAX, NomeSemaforoPodeLer); //vazios
	SemaforoLer = CreateSemaphore(NULL, 0, MAX, NomeSemaforoPodeEscrever); //is

	if (SemaforoEscrever == NULL || SemaforoLer == NULL || MutexRead == NULL || MutexWrite == NULL) {
		_tprintf(TEXT("[Erro]Criação de objectos do Windows(%d)\n"), GetLastError());
		return -1;
	}

}

void EnviaMensagem() {
	int x;
	_tprintf(TEXT("Introduzir numero memoria partilhadas?"));
	_tscanf_s(TEXT("%d"), &x);
	WaitForSingleObject(SemaforoEscrever, INFINITE);
	WaitForSingleObject(MutexRead, INFINITE);
	//mensagens->buffer[mensagens->in] = cenas;
	//	_tcscpy_s(mensagens->buffer[mensagens->out].tecla, sizeof(mensagens->buffer[mensagens->out].tecla), cenas);
	mensagens->buffer[mensagens->in].id = x;
	mensagens->contadorMensagens++;
	mensagens->in = (mensagens->in + 1) % MAX;
	ReleaseMutex(MutexRead);
	ReleaseSemaphore(SemaforoLer, 1, NULL);
}

void TrataMensagem() {
	int x;
	WaitForSingleObject(SemaforoLer, INFINITE);
	WaitForSingleObject(MutexWrite, INFINITE);  //isto é que fica como função na dll
	x = mensagens->buffer[mensagens->out].id;
	_tprintf(_TEXT("Mensagem: %d \n"), x);
	mensagens->buffer[mensagens->out].id = 0;
	mensagens->contadorMensagens--;
	mensagens->out = (mensagens->out + 1) % MAX;
	ReleaseMutex(MutexWrite);
	ReleaseSemaphore(SemaforoEscrever, 1, NULL);
}