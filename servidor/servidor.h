#pragma once
#include <windows.h>
#include <tchar.h>
#include <io.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>  
#include <stdlib.h> 
#include "../DlltpSO2/dll.h"


//#define _CRT_SECURE_NO_WARNINGS

//Passar os mutexs e a sua criação para a dll


#define nVidas 5


#define MinClientes 1
#define MinInvaders 10


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




//PBufferMensagens mensagens = NULL;
//PJogo jogo = NULL;


//Estrutura "servidor" que leva o ponteiro do Jogo , Input Handlers e o tal continua


	int continua = 0;
	HANDLE hThreadLeitor;
	HANDLE hThreadInvadersBase;
	HANDLE hThreadInvadersEsquivos;
	HANDLE hThreadsInvadersOutros;

	void IniciaBuffer();

DWORD WINAPI ThreadConsumidor(LPVOID param); //LADO DO GATEWAY
DWORD WINAPI ThreadInvadersBase(int num);
DWORD WINAPI ThreadInvadersEsquivo(int num);
DWORD WINAPI ThreadInvadersOutro(int num);

void InicializaJogo();
void ColocaInvaders();
Input RecebeInput();

void IniciaInvaders(Input inp);
//void CreateThreadsJogo();

