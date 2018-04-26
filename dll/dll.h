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
#define SIZE 20 //dimens�o da mensagem e do nome 

#define NumClientes
#define NumInvaders


/*
//Mecanismos de sincroniza��o para o buffer
HANDLE SemLer, SemEscrever, MutexLer, MutexEscrever;

//Lado do  Getaway
SemEscrever = CreateSemaphore(NULL, 0, MAX, TEXT("SemEscrever"));
MutexEscrever = CreateMutex(NULL, FALSE, TEXT("MutexEscrever"));

//Lado do Servidor
SemLer = CreateSemaphore(NULL, MAX, MAX, TEXT("SemLeR " ));
MutexLer = CreateSemaphore(NULL, FALSE, TEXT("MutexLer"));

*/

//O cliente s� envia dois tipos de mensagem: ou quer iniciar o jogo ou quer fazer uma jogada

typedef enum _TipoMensagemCLI {
	INICIO,
	JOGANDO
} TipoMensagemCLI;

//O Servidor s� envia 3 mensagens: quando est� a come�ar o jogo e envia o id para o cliente
//Quando quer actualizar o cliente de uma nova mudan�a no jogo
//Quando o jogo j� acabou e quer enviar os 10 jogadores com melhor pontua��o

typedef enum _TipoMensagemSER {
	INICIO_JOGO,
	JOGADA,
	PONTUA��O1O
} TipoMensagemSER;

typedef enum CicloDeVida {
	//Isto est� no enunciado
} CicloDeVida;

typedef enum TipoPowerUP {
	//ver lista do enunciado
} TipoPowerUP;

//Quase todos os elementos do jogo s�o rectangulos, por isso esta estrutura faz sentido
typedef struct Area {
	int x, y;
	int comprimento, altura;
} Area;

typedef struct Pontuacao {
	int pontos;
	TCHAR quem[SIZE];
	//falta aqui a cena da data que n�o me lembro
} Pontuacao;

//Array de Invaders, Defenders, Disparos? PowerUps? 
typedef struct Jogo {

}Jogo;

typedef struct Invader {
	Area area;
	int vidas;

} Invader;

typedef struct Defender {
	Area area;
	int id;
	Pontuacao pontos;
	int vidas;
} Defender;

//As bombas e os tiros t�m uma estrutura pr�ticamente igual, portanto ficam na mesma estrutura
typedef struct Disparos {
	int x, y; //s� ocupam um ponto por isso s�o n�o se usa area
	bool direcao; //As bombas descem e os tiros sobe 
	//bombas ficam com 0 tiros ficam com 1 !!!!!!!!!!!! N�o sei se n�o � TRUE or FALSE
	TCHAR dono[SIZE]; //para incrementar a pontua��o dos clientes
} Disparos;

typedef struct PowerUP {
	TipoPowerUP tipo;
	Area area;
	int duracao; //cada powerup tem efeito durante x tempo
} PowerUP;



//Estrutura para defenir dimens�es do "mapa"
//Se virmos que o mapa n�ovai ter mais nada para alem das dimensoes n�o vale a pena estar aqui
typedef struct Window { 

} Window;

//Mensagem que vai do Cliente para o Gateway
typedef struct MsgCLI {
	TipoMensagemCLI mensagem;
	int id;
	TCHAR quem[SIZE];
} MsgCLI;

//Mensagem que vai do Gateway para o cliente
typedef struct MsgSER {
	TipoMensagemSER mensagem;
} MsgSER;
