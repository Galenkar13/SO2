#pragma once
#include <windows.h>
#include <tchar.h>
#include <io.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>  
#include <stdlib.h> 
#include "../DlltpSO2/dll.h"

#define nVidas 5


#define MinClientes 1
#define MinInvaders 10

#define vidasPlayerMax10
#define vidasPlayer 5
#define vidasPlayerMin 1

#define ComprimentoInvaderMax 10
#define ComprimentoInvader 5
#define ComprimentoInvaderMin 1

#define AlturaInvaderMax 10
#define AlturaInvader 5
#define AlturaInvaderMin 1

#define velocidadeDisparoMax 10
#define velocidadeDisparo  5
#define velocidadeDisparoMin 1

#define velocidadeInvaderBaseMax 10
#define velocidadeInvaderBase 5 //A esquiva tem velocidadeInvaderBase + (velocidadeInvaderBase*0.10)
#define velocidadeInvaderBaseMin 1

#define velocidadePowerUPMax 10
#define velocidadePowerUP 5
#define velocidadePowerUPMin 1

#define ProbabilidadeTiroInvaderMax 10
#define ProbabilidadeTiroInvader 5 //Esquiva ProbabilidadeTiroInvader - (ProbabilidadeTiroInvader * 0.40)
#define ProbabilidadeTiroInvaderMin 1

#define DuracaoPowerUPMax 10
#define DuracaoPowerUP 5//igual para todos 
#define DuracaoPowerUPMin 1

#define ProbDisparo 10

#define ProbPowerUP 10

#define velocidadeMax 10
#define velocidadeDisparo  5
#define velocidadeDisparoMin 1

#define velocidadeDisparoMax 10
#define velocidadeDisparo  5
#define velocidadeDisparoMin 1


#define MAXVIDASJOGADOR 10
#define VIDASJOGADOR 5
#define MINVIDASJOGADOR 1


int CreateThreadsInvaders();


//PBufferMensagens mensagens = NULL;
//PJogo jogo = NULL;


//Estrutura "servidor" que leva o ponteiro do Jogo , Input Handlers e o tal continua


int continua = 0; //variavel temporaria enquanto não está defenido ciclo jogo


HANDLE hThreadLeitor;
HANDLE hThreadInvadersBase;
HANDLE hThreadInvadersEsquivos;
HANDLE hThreadsInvadersExtras;


typedef struct _Input {
	int numInvaders;
	int numInvadersBase;
	int	numInvadersEsquivo;
	int	numInvadersOutros;

	int numVidasDefenders; //numero de vidas jogadores

	int probPowerUP;   //probablidade de acontecer um power up
	int duracaoPowerUP;//quanto tempo é que esse power up dura

	int probDisparo;

} Input, *PInput;

void IniciaBuffer();

DWORD WINAPI ThreadConsumidor(LPVOID param); //LADO DO GATEWAY
DWORD WINAPI ThreadInvadersBase(Input inp);
DWORD WINAPI ThreadInvadersEsquivo(Input inp);
DWORD WINAPI ThreadInvadersExtra(Input inp);

void InicializaJogo();
void ColocaInvaders(Input inp);
Input RecebeInput();

void IniciaInvaders(Input inp); //Inicialização dos invaders
 //Função que lança as Threads que vão controlar as funcionalidades dos Invaders
								//Uma Thread por tipo de Invader

