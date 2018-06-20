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


#define MAXNUMEROINVADERS 20
#define NUMEROINVADERS 16
#define MINNUMEROINVADERS 8


#define ComprimentoInvaderMax 10
#define ComprimentoInvader 25
#define ComprimentoInvaderMin 1

#define AlturaInvaderMax 10
#define AlturaInvader 25
#define AlturaInvaderMin 1

#define velocidadeDisparoInvaderMax 3000
#define velocidadeDisparoInvader  1000
#define velocidadeDisparoInvaderMin 500

#define velocidadeInvaderBaseMax 3000
#define velocidadeInvaderBase 1000 //A esquiva tem velocidadeInvaderBase + (velocidadeInvaderBase*0.10)
#define velocidadeInvaderBaseMin 500

#define velocidadePowerUPMax 3000
#define velocidadePowerUP 2000
#define velocidadePowerUPMin 500

#define ProbabilidadeTiroInvaderMax 20
#define ProbabilidadeTiroInvader 3 //Esquiva ProbabilidadeTiroInvader - (ProbabilidadeTiroInvader * 0.40)
#define ProbabilidadeTiroInvaderMin 2

#define DuracaoPowerUPMax 60000
#define DuracaoPowerUP 30000//igual para todos 
#define DuracaoPowerUPMin 20000


#define MAXVIDASJOGADOR 10
#define VIDASJOGADOR 5
#define MINVIDASJOGADOR 1


#define MAXVELOCIDADEDEFENDER 3000
#define VELOCIDADEDEFENDER 500
#define MINVELOCIDADEDEFENDER 500

#define MAXPROBABILIDADEPOWERUP 70
#define PROBABILIDADEPOWERUP 50
#define MINPROBABILIDADEPOWERUP 40

#define MAXVELOCIDADETIRODEFENDER 3000
#define VELOCIDADETIRODEFENDER 500
#define MINVELOCIDADETIRODEFENDER 500


int CreateThreadsInvaders();





int continua = 0; //variavel temporaria enquanto não está defenido ciclo jogo


HANDLE hThreadLeitor;
HANDLE hThreadInvadersBase;
HANDLE hThreadInvadersEsquivos;
HANDLE hThreadsInvadersExtras;
HANDLE hThreadBombas;
HANDLE hThreadPowerups;
HANDLE hThreadTiros;
HANDLE hThreadJogadores;



DWORD WINAPI ThreadConsumidor(LPVOID param); //LADO DO GATEWAY
DWORD WINAPI ThreadInvadersBase();

DWORD WINAPI ThreadBombas();
DWORD WINAPI ThreadPowerups();
DWORD WINAPI ThreadTiros();
DWORD WINAPI ThreadJogadores();


void InicializaJogo();
void ColocaInvaders();
void ColocaDefenders();

void IniciaDefenders();
void GeraPowerup(int x, int y);

void AtiraBomba();
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

BOOL RecebeConfiguracao(HWND hWnd);

void IniciaInvaders(); //Inicialização dos invaders
								//Função que lança as Threads que vão controlar as funcionalidades dos Invaders
								//Uma Thread por tipo de Invader

void IniciaInvadersNivel2();
void IniciaInvadersNivel3();