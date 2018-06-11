#define DlltpSO2_EXPORTS 
#include <windows.h>
#include <fcntl.h>
#include "dll.h"
#include "comunicacao.h"





int IniciaSinc() {
	MutexRead = CreateMutex(NULL, FALSE, TEXT("MutexRecebeBuffer"));
	MutexWrite = CreateMutex(NULL, FALSE, TEXT("MutexEnviaBuffer"));
	SemaforoEscrever = CreateSemaphore(NULL, MAX, MAX, TEXT("Semáforo Pode Ler")); //vazios
	SemaforoLer = CreateSemaphore(NULL, 0, MAX, TEXT("Semáforo Pode Escrever")); //is
	hEventActiva = CreateEvent(NULL,TRUE, FALSE, TEXT("EventoManda"));
	hEventLida = CreateEvent(NULL,TRUE, FALSE, TEXT("EventoRecebe"));
	//hEvento = CreateEvent(NULL, TRUE, FALSE, TEXT("GoEvent"));
    //hMutexJogo = CreateMutex(NULL, FALSE, TEXT("GoMutex"));

	if (SemaforoEscrever == NULL || SemaforoLer == NULL || MutexRead == NULL || MutexWrite == NULL || hEventActiva == NULL || hEventLida == NULL) {
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
	CloseHandle(hEventActiva);
	CloseHandle(hEventLida);
	CloseHandle(hEvento);
	CloseHandle(hMutexJogo);
}

void EnviaMensagem(MsgCLI dados){
	WaitForSingleObject(SemaforoEscrever, INFINITE);
	WaitForSingleObject(MutexRead, INFINITE);
	_tcscpy_s(mensagens->buffer[mensagens->out].nome, (sizeof(TCHAR)* TAM), dados.nome);
	mensagens->buffer[mensagens->out].id = dados.id;
	mensagens->buffer[mensagens->out].tipo_mensagem = dados.tipo_mensagem;
	mensagens->buffer[mensagens->out].tecla = dados.tecla;
	//_tcscpy_s(mensagens->buffer[mensagens->out].tecla, (sizeof(TCHAR)* TAM), dados.tecla);
	//_tprintf(TEXT(" NOME  %s\n"), mensagens->buffer[mensagens->out].nome);

	_tprintf(_TEXT("Mensagem: %s \n"), mensagens->buffer[mensagens->out].nome);

	_tprintf(TEXT(" TIPO %d \n"), mensagens->buffer[mensagens->out].tipo_mensagem);

	_tprintf(TEXT(" ID %d \n"), mensagens->buffer[mensagens->out].id);
	mensagens->contadorMensagens++;
	mensagens->in = (mensagens->in + 1) % MAX;
	ReleaseMutex(MutexRead);
	ReleaseSemaphore(SemaforoLer, 1, NULL);
}

MsgCLI TrataMensagem() {
	MsgCLI aux;
	WaitForSingleObject(SemaforoLer, INFINITE);
	WaitForSingleObject(MutexWrite, INFINITE);  //isto é que fica como função na dll
	_tprintf(_TEXT("Mensagem: %s \n"), mensagens->buffer[mensagens->out].nome);
	_tprintf(TEXT(" ID %d \n"), mensagens->buffer[mensagens->out].id);
	aux.id = mensagens->buffer[mensagens->out].id;
	_tcscpy_s(aux.nome, _countof(aux.nome) , mensagens->buffer[mensagens->out].nome);
	aux.tipo_mensagem = mensagens->buffer[mensagens->out].tipo_mensagem;
	
	//aux.tecla = mensagens->buffer[mensagens->out].tecla;
	mensagens->buffer[mensagens->out].id = 0;
	mensagens->contadorMensagens--;
	mensagens->out = (mensagens->out + 1) % MAX;
	ReleaseMutex(MutexWrite);
	ReleaseSemaphore(SemaforoEscrever, 1, NULL);
	return aux;
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
		mensagens->buffer[i].tecla = ESPAÇO;
	}
}


// CRIA A MEMORIA PARTILHADA COM O TAMANHO PARA RECEBER 100 MENSAGENS E 100 UPDATES
BOOL APIENTRY DllMain(BOOL hModule, DWORD ul_reason_for_call, LPVOID lpreserved)
{
#ifdef UNICODE
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);
#endif
	BOOL primeiroProcesso;
	// ReSharper disable once CppDefaultCaseNotHandledInSwitchStatement
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		// MENSAGENS
		//PBufferMensagens mensagens = NULL;
		//PJogo jogo = NULL;
		
		//mensagens = NULL;

		primeiroProcesso = GetLastError() != ERROR_ALREADY_EXISTS;

		if (primeiroProcesso) {
			mensagens = NULL;
			primeiroProcesso = GetLastError() != ERROR_ALREADY_EXISTS;

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


			//jogo = NULL;


			// DADOS DO JOGO
			hMemoriaJogo = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(Jogo), TEXT("memPartilhadaJogo"));


			if (hMemoriaJogo == NULL)
			{
				_tprintf(TEXT("\nErro na memória partilhada. (Erro: %d)"), GetLastError());
				return FALSE;
			}


			jogo = (PJogo)MapViewOfFile(hMemoriaJogo, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);
			IniciaSinc();
			IniciaBuffer();

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