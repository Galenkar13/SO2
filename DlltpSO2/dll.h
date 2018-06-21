#pragma once
#include <windows.h>
#include <tchar.h>
#include "comunicacao.h"



#ifdef DlltpSO2_EXPORTS 
#define DlltpSO2 __declspec(dllexport)
#else
#define DlltpSO2 __declspec(dllimport)
#endif


#define mPartilhadaMensagens TEXT("memPartilhadaMensagens")
#define TAMANHOBUFFER sizeof(BufferMensagens)



HANDLE SemaforoEscrever;
HANDLE SemaforoLer;
HANDLE MutexRead;
HANDLE MutexWrite;
HANDLE hMemoriaBuffer;
HANDLE hMemoriaJogo;
HANDLE hEventActiva;
HANDLE hEventLida;
HANDLE hEvento, hMutexJogo;



#ifdef __cpluplus
extern "C" {
#endif

	DlltpSO2 PBufferMensagens mensagens;
	DlltpSO2 PJogo jogo;
	//Funções a serem exportadas/importadas
	DlltpSO2 int IniciaSinc();
	DlltpSO2 void AcabaSinc();
	DlltpSO2 MsgCLI TrataMensagem();
	DlltpSO2 void EnviaMensagem(MsgCLI dados);
	DlltpSO2 void IniciaBuffer();
	DlltpSO2 MsgCliGat EnviaUpdate(int id);
	BOOL APIENTRY DllMain(BOOL hModule, DWORD ul_reason_for_call, LPVOID lpreserved);

#ifdef __cplusplus
}
#endif
