#pragma once
#include <windows.h>
#include <tchar.h>
#include <io.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>  
#include <stdlib.h> 
#include "../DlltpSO2/dll.h"



//#define nVidas 5
#define SinalUser (WM_USER + 1)


#define MinClientes 1
#define MinInvaders 10

#define vidasPlayerMax10
#define vidasPlayer 5
#define vidasPlayerMin 1

#define ComprimentoInvaderMax 10
#define ComprimentoInvader 25
#define ComprimentoInvaderMin 1

#define AlturaInvaderMax 10
#define AlturaInvader 25
#define AlturaInvaderMin 1

#define velocidadeDisparoMax 10
#define velocidadeDisparo  5
#define velocidadeDisparoMin 1

#define velocidadeInvaderBaseMax 10
#define velocidadeInvaderBase 5000 //A esquiva tem velocidadeInvaderBase + (velocidadeInvaderBase*0.10)
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
HANDLE hThreadBombas;
HANDLE hThreadPowerups;
HANDLE hThreadTiros;
HANDLE hThreadJogadores;


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

DWORD WINAPI ThreadConsumidor(LPVOID param); //LADO DO GATEWAY
DWORD WINAPI ThreadInvadersBase();
DWORD WINAPI ThreadInvadersEsquivo();
DWORD WINAPI ThreadInvadersExtra();
DWORD WINAPI ThreadBombas();
DWORD WINAPI ThreadPowerups();
DWORD WINAPI ThreadTiros();
DWORD WINAPI ThreadJogadores();


void InicializaJogo();
void ColocaInvaders();
void ColocaDefenders();
Input RecebeInput();
void IniciaDefenders();
void GeraPowerup(int x, int y);

void IniciaTiros();
void IniciaBombas();
void IniciaDenfenders();
void IniciaPowerUp();

int MoveInvaders(int verifica_sentido);
void TiroAntigueInvader(int id);
void BombaAntigueDefender(int x, int y);
void MoveBomba(int id);
void MoveTiro(int id);
void MovePowerUp(int id);
void MoveDefender(int id);

void RecebeConfiguracao(HWND hWnd);

void IniciaInvaders(); //Inicialização dos invaders
								//Função que lança as Threads que vão controlar as funcionalidades dos Invaders
								//Uma Thread por tipo de Invader

