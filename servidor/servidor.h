#pragma once

#include <windows.h>
#include <tchar.h>
#include <io.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>  
#include <stdlib.h> 

#define _CRT_SECURE_NO_WARNINGS

#define MAX 50 //tamanho do buffer 
#define SIZE 20 //dimensão da mensagem e do nome 


	
//MsgCLI(* PtrBuffer); //ponteiro de memoria para a memoria partilhada

//Jogo(*PtrJogo)[sizeof(Jogo)]; //ponteiro de memoria para a memoria partilhada
//Acho que este ponteir o para o buffer est+a bom
//Mas para o jogo não


//DWORD WINAPI ThreadEscreve(LPVOID param);
//DWORD WINAPI ThreadReadAndRemove(LPVOID param);

HANDLE SemaforoRead;
HANDLE SemaforoWrite;
HANDLE MutexRead;
HANDLE MutexWrite;
HANDLE hMemoriaBuffer;
HANDLE hMemoriaJogo;
HANDLE hThreadEscritor;

int continua;
int in;
int out;


/*
//Mecanismos de sincronização para o buffer
HANDLE SemLer, SemEscrever, MutexLer, MutexEscrever;

//Lado do  Getaway
SemEscrever = CreateSemaphore(NULL, 0, MAX, TEXT("SemEscrever"));
MutexEscrever = CreateMutex(NULL, FALSE, TEXT("MutexEscrever"));

//Lado do Servidor
SemLer = CreateSemaphore(NULL, MAX, MAX, TEXT("SemLeR " ));
MutexLer = CreateSemaphore(NULL, FALSE, TEXT("MutexLer"));

*/

typedef enum _TipoMensagemCLI {
	INICIO,
	JOGANDO
} TipoMensagemCLI;

//Mensagem que vai do Cliente para o Gateway
typedef struct MsgCLI {
	TipoMensagemCLI mensagem;
	int id;
	TCHAR nome[SIZE];
	TCHAR tecla[SIZE];
} MsgCLI;

typedef struct Jogo {

	int altura, comprimento;

	//Invader Invaders[NumInvaders];

	//Defender Defenders[NumClientes];

	//PowerUP PowerUP[NumPowerUP];

	//Disparo Disparos[MaxDisparos];
	//Ver como são os power ups e os disparos !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

//	CicloDeVida CicloDeVida;

}Jogo;

MsgCLI(* PtrBuffer)[MAX][sizeof(MsgCLI)];
Jogo(* PtrJogo)[sizeof(Jogo)];