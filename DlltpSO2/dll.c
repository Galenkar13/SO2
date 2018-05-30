#define DlltpSO2_EXPORTS 
#include <windows.h>
#include <fcntl.h>
#include "dll.h"
#include "comunicacao.h"


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


	int nRandonNumber = rand() % 3;
	int i;



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

void IniciaBuffer() {
	int i;

	mensagens->in = 0;
	mensagens->out = 0;
	mensagens->contadorMensagens = 0;

	for (i = 0; i < MAX; i++) {
		mensagens->buffer[i].id = 0;
		mensagens->buffer[i].tipo_mensagem = '0';
		_tcscpy(mensagens->buffer[i].nome, TEXT("nada"));
		_tcscpy(mensagens->buffer[i].tecla, TEXT("nada"));
	}
}


// CRIA A MEMORIA PARTILHADA COM O TAMANHO PARA RECEBER 100 MENSAGENS E 100 UPDATES
BOOL APIENTRY DllMain(BOOL hModule, DWORD ul_reason_for_call, LPVOID lpreserved)
{
#ifdef UNICODE
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);
#endif

	// ReSharper disable once CppDefaultCaseNotHandledInSwitchStatement
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		// MENSAGENS


		hMemoriaBuffer = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, TAMANHOBUFFER, mPartilhadaMensagens); //onde está o invalid tbm posso guardar num dados.txt

		if (hMemoriaBuffer == NULL)
		{
			_tprintf(TEXT("\nErro na memória partilhada. (Erro: %d)"), GetLastError());
			return FALSE;
		}


		mensagens = (PBufferMensagens)MapViewOfFile(hMemoriaBuffer, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);


		if (mensagens == NULL)
		{
			_tprintf(TEXT("\nErro na view da memória partilhada. (Erro: %d)"), GetLastError());
			CloseHandle(hMemoriaBuffer);
			return FALSE;
		}

		BOOL primeiroProcesso = GetLastError() != ERROR_ALREADY_EXISTS;


		// DADOS DO JOGO
		hMemoriaJogo = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(Jogo), TEXT("memPartilhadaJogo"));


		if (hMemoriaJogo == NULL)
		{
			_tprintf(TEXT("\nErro na memória partilhada. (Erro: %d)"), GetLastError());
			return FALSE;
		}

		
		jogo = (PJogo)MapViewOfFile(hMemoriaJogo, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);

		
		if (primeiroProcesso) {
			IniciaBuffer();
			IniciaSinc();
		}

	
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH: // apenas limpa memória quando todos os processos fecharem
		AcabaSinc();
		break;
	}
	return TRUE;
}