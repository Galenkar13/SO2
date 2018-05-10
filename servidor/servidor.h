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

//faltam defines

#define velocidadeTiro  5
#define velocidadeDefensor 5
#define velocidadeInvaderBase 5 //A esquiva tem velocidadeInvaderBase + (velocidadeInvaderBase*0.10)
#define velocidadePowerUP 5
#define ProbabilidadeTiroInvader 5 //Esquiva ProbabilidadeTiroInvader - (ProbabilidadeTiroInvader * 0.40)
#define DuracaoPowerUP 5//igual para todos 

#define ComprimentoJanela 100
#define AlturaJanela 80

#define ComprimentoInvader 5
#define AlturaInvader 5

//int continua = 1;

int CreateThreadsInvaders();


//PBufferMensagens mensagens = NULL;
//PJogo jogo = NULL;


//Estrutura "servidor" que leva o ponteiro do Jogo , Input Handlers e o tal continua


int continua = 0;
HANDLE hThreadLeitor;
HANDLE hThreadInvadersBase;
HANDLE hThreadInvadersEsquivos;
HANDLE hThreadsInvadersExtras;

void IniciaBuffer();

DWORD WINAPI ThreadConsumidor(LPVOID param); //LADO DO GATEWAY
DWORD WINAPI ThreadInvadersBase(Input inp);
DWORD WINAPI ThreadInvadersEsquivo(Input inp);
DWORD WINAPI ThreadInvadersExtra(Input inp);

void InicializaJogo();
void ColocaInvaders();
Input RecebeInput();

void IniciaInvaders(Input inp); //Inicialização dos invaders
 //Função que lança as Threads que vão controlar as funcionalidades dos Invaders
								//Uma Thread por tipo de Invader

