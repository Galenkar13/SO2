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
#define mPartilhadaMensagens TEXT("memPartilhadaMensagens")
#define TAMANHOBUFFER sizeof(BufferMensagens)
#define nVidas 5

#define velocidadeTiro  5
#define velocidadeDefensor 5
#define velocidadeInvaderBase 5 //A esquiva tem velocidadeInvaderBase + (velocidadeInvaderBase*0.10)
#define velocidadePowerUP 5
#define ProbabilidadeTiroInvader 5 //Esquiva ProbabilidadeTiroInvader - (ProbabilidadeTiroInvader * 0.40)
#define DuracaoPowerUP 5//igual para todos 

#define MaxClientes 10
#define MaxInvaders 20
#define MaxPowerUP 10
#define MaxDisparos 10
#define MinClientes 1
#define MinInvaders 10
#define MinPowerUP 2
#define MinDisparos 8

HANDLE SemaforoEscrever;
HANDLE SemaforoLer;
HANDLE MutexRead;
HANDLE MutexWrite;
HANDLE hMemoriaBuffer;
HANDLE hMemoriaJogo;
HANDLE hThreadLeitor;

int continua = 1;


typedef enum _TipoMensagemCLI {
	INICIO,
	JOGANDO
} TipoMensagemCLI;

typedef enum _TipoMensagemSER {
	INICIO_JOGO,
	JOGADA
} TipoMensagemSER;

typedef enum _CicloDeVida {
	CRIACAO,
	ASSOCIACAO,
	INICIO_CICLO_JOGO,
	DECORRER,
	FINAL
} CicloDeVida;

typedef enum TipoPowerUP {
	ESCUDO,
	GELO,
	BATERIA,
	MAIS,
	VIDA,
	ALCOOL,
	OUTRO
} TipoPowerUP;

typedef enum TipoInvader {
	BASICO,
	ESQUIVO,
	EXTRA
} TipoInvader;



//Mensagem que vai do Cliente para o Gateway
typedef struct _MsgCLI {
	TipoMensagemCLI mensagem;
	int id;
	TCHAR nome[SIZE];
	TCHAR tecla[SIZE];
} MsgCLI, *PMsgCLI;

typedef struct _BufferMensagens {
	MsgCLI buffer[MAX];
	int in;
	int out;
	int contadorMensagens;
} BufferMensagens, *PBufferMensagens;

typedef struct Area {
	int x, y;
	int comprimento, altura;
} Area;

typedef struct Pontuacao {
	int pontos;
	TCHAR quem[SIZE];
	SYSTEMTIME data;
} Pontuacao;

typedef struct Invader {
	TipoInvader tipo;
	Area area;
	int vidas;
	int velocidade;
} Invader;


typedef struct PowerUP {
	TipoPowerUP tipo;
	Area area;
	int duracao; //cada powerup tem efeito durante x tempo
	int velocidade;
} PowerUP;


typedef struct Defender {
	Area area;
	TCHAR nome[SIZE];
	Pontuacao pontos;
	int vidas;
	int velocidade;
	PowerUP powerUP;
	//estado do jogo é possivel
} Defender;

//As bombas e os tiros têm uma estrutura práticamente igual, portanto ficam na mesma estrutura
typedef struct Disparos {
	int x, y; //só ocupam um ponto por isso são não se usa area
	BOOL direcao; //As bombas sobes e os tiros descem
				  //bombas ficam com 1 tiros ficam com 0 !!!!!!!!!!!! Não sei se não é TRUE or FALSE
	TCHAR dono[SIZE]; //para incrementar a pontuação dos clientes
	int velocidade;
} Disparos;



typedef struct _Jogo {

	int altura, comprimento;

	Invader Invaders[MaxInvaders];

	Defender Defenders[MaxClientes];

	PowerUP PowerUP[MaxPowerUP];

	Disparos Disparos[MaxDisparos];
	//Ver como são os power ups e os disparos !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	CicloDeVida CicloDeVida;

}Jogo, *PJogo;

PBufferMensagens mensagens = NULL;
PJogo jogo = NULL;

DWORD WINAPI ThreadProdutor(LPVOID param); //LADO DO GATEWAY
DWORD WINAPI ThreadConsumidor(LPVOID param); //LADO DO GATEWAY
DWORD WINAPI ThreadInvadersBase(LPVOID param);
DWORD WINAPI ThreadInvadersEsquivo(LPVOID param);
DWORD WINAPI ThreadInvadersOutro(LPVOID param);

TCHAR NomeSemaforoPodeLer[] = TEXT("Semáforo Pode Ler");TCHAR NomeSemaforoPodeEscrever[] = TEXT("Semáforo Pode Escrever");