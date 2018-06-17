#pragma once
#include <windows.h>
#include <tchar.h>

#define MAX 50 //tamanho do buffer 
#define SIZE 254 //dimensão da mensagem e do nome 
#define TAM 254
//Estes defines tem de estar na dll porque vão ser utilizados para a memoria partilhada jogo
#define ComprimentoJanelaMAX 400
#define AlturaJanelaMAX 600
#define ComprimentoJanelaMIN 350
#define AlturaJanelaMIN 550



#define MaxClientes 20
#define MaxInvaders 60
#define MaxPowerUP 10
#define MaxnumBombas 25
#define MaxnumTiros 25

typedef enum _TECLA {
	DIREITA,
	BAIXO,
	CIMA,
	ESQUERDA,
	ESPAÇO,
	NULA,
	OUTRA_TECLA
} TECLA;

typedef enum _TipoMensagemCLI {
	INICIO,
	JOGANDO
} TipoMensagemCLI;

typedef struct _MsgCLI {
	TipoMensagemCLI tipo_mensagem;
	int id;
	TCHAR nome[SIZE];
	TECLA tecla;
} MsgCLI, *PMsgCLI;

typedef struct _BufferMensagens {
	MsgCLI buffer[MAX];
	int in;
	int out;
	int contadorMensagens;
} BufferMensagens, *PBufferMensagens;

typedef enum _CicloDeVida {
	CRIACAO,
	ASSOCIACAO,
	INICIO_CICLO_JOGO,
	DECORRER,
	FINAL
} CicloDeVida;

typedef enum _CicloDeVidaCliente {
	GAME,
	NO_GAME
} CicloDeVidaCliente;

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
	EXTRA,
	MORTO
} TipoInvader;


//Mensagem que vai do Cliente para o Gateway


typedef struct Area {
	int x, y;
	int comprimento, altura;
} Area;


typedef struct Invader {
	TipoInvader tipo;
	int id_invader; //inicializar isto na funcao
	Area area;
	int vidas;
	int velocidade;
} Invader;


typedef struct PowerUP {
	TipoPowerUP tipo;
	int id_powerUP;
	Area area;
	int duracao; //cada powerup tem efeito durante x tempo
	int velocidade;
} PowerUP;


typedef struct Defender {
	Area area;
	int id_defender;
	TCHAR nome[SIZE];
	int pontos;
	int vidas;
	int velocidade;
	PowerUP powerUP;
	TECLA proxima_jogada; //para guardar utilixção por ciclo jogo
						  //estado do jogo é possivel
} Defender;

//As bombas e os tiros têm uma estrutura práticamente igual, portanto ficam na mesma estrutura

typedef struct Bombas {
	int x, y;
	int velocidade;
	int id_bombas;
	int id_dono;
}Bombas;

typedef struct Tiros {
	Area area;
	int velocidade;
	int id_tiros;
	int id_dono;
}Tiros;


typedef struct _Dados {
	int altura, comprimento;
	int nInvaders;
	int nDefenders;
	int nPowerUPs;
	int nTiros;
	int nBombas;
	int nVidas;
	int duracaoPowerUp;
	int velocidadeBomba;
	int velocidadeTiro;
	int velocidadePowerUps;
	int velocidadeInvaders;
	int velocidadeDefenders;
	int probabilidadeInvaderDisparar;
	int probabilidadePowerUp;
} Dados;

typedef struct _Jogo {
	Invader Invaders[MaxInvaders];

	Defender Defenders[MaxClientes];

	PowerUP PowerUP[MaxPowerUP];

	Tiros Tiros[MaxnumTiros];

	Bombas Bombas[MaxnumBombas];

	CicloDeVida CicloDeVida;

	Dados Dados;

}Jogo, *PJogo;

typedef enum _TipoMensagemGAT {
	PRIMEIRA,
	PONTUACAO,
	ATUALIZACAO
} TipoMensagemGAT;

typedef struct Pontuacao {
	int pontos;
	TCHAR quem[SIZE];
	SYSTEMTIME data;
} Pontuacao;


typedef struct _MsgSER {
	Pontuacao TOP[10];
} MsgSER, *PMsgSER;

typedef struct _MsgCliGat {
	TipoMensagemGAT tipo;
	union data
	{
		MsgCLI mensagensCliente;
		MsgSER pontuacaoFinal;
		Jogo JogoCopia;
	};
} MsgCliGat, *PMsgCliGat;
