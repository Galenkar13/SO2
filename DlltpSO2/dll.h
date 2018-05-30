#pragma once
#include <windows.h>
#include <tchar.h>
#include "comunicacao.h"

//Definir uma constante para facilitar a leitura do prot�tipo da fun��o
//Este .h deve ser inclu�do no projeto que o vai usar (modo impl�cito)
#define TAM 256
//Esta macro � definida pelo sistema caso estejamos na DLL (<DLL_IMP>_EXPORTS definida)
//ou na app (<DLL_IMP>_EXPORTS n�o definida) onde DLL_IMP � o nome deste projeto

//


#ifdef DlltpSO2_EXPORTS 
#define DlltpSO2 __declspec(dllexport)
#else
#define DlltpSO2 __declspec(dllimport)
#endif

#define MAX 50 //tamanho do buffer 
#define SIZE 20 //dimens�o da mensagem e do nome 
#define mPartilhadaMensagens TEXT("memPartilhadaMensagens")
#define TAMANHOBUFFER sizeof(BufferMensagens)





HANDLE SemaforoEscrever;
HANDLE SemaforoLer;
HANDLE MutexRead;
HANDLE MutexWrite;
HANDLE hMemoriaBuffer;
HANDLE hMemoriaJogo;
HANDLE hMutexJogo;
HANDLE hMutexJogo2;
HANDLE hEventActiva;
HANDLE hEventLida;







//Apenas uma coisa de cada tipo de obecto do jogo porque vai ser feita a actualiza��o 1 a 1



//Gateway manda para o cliente uma estrutura com uma cena de cada
//e actualiza a cada altera�aao 



typedef enum _Direcao {
	direita,
	baixo,
	cima,
	esquerda
} Direcao;

#define MOVEMENT_INCREMENT 2

TCHAR NomeSemaforoPodeLer[] = TEXT("Sem�foro Pode Ler");TCHAR NomeSemaforoPodeEscrever[] = TEXT("Sem�foro Pode Escrever");
TCHAR EventoManda[] = TEXT("EventoManda");TCHAR EventoRecebe[] = TEXT("EventoRecebe");

TCHAR Mutex1[] = TEXT("MutexEnvia");
TCHAR Mutex2[] = TEXT("MutexRecebe");

TCHAR Mutex3[] = TEXT("MutexEnviaBuffer");
TCHAR Mutex4[] = TEXT("MutexRecebeBuffer");



#ifdef __cpluplus
extern "C" {
#endif

	DlltpSO2 PBufferMensagens mensagens;

	 DlltpSO2 PJogo jogo;
	//Fun��es a serem exportadas/importadas


	DlltpSO2 int IniciaSinc();
	DlltpSO2 void AcabaSinc();
	DlltpSO2 void TrataMensagem();
	DlltpSO2 void EnviaMensagem(); 
	DlltpSO2 void MoveInvaderBase(int id, int x, int y, int num);
	DlltpSO2  void RecebeAtualizacao(int id);
	DlltpSO2 void IniciaBuffer();


#ifdef __cplusplus
}
#endif
