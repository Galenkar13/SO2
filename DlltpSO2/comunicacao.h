#pragma once
#include <windows.h>
#include <tchar.h>

#define MAX 50 //tamanho do buffer 
#define SIZE 20 //dimensão da mensagem e do nome 
#define TAM 254
//Estes defines tem de estar na dll porque vão ser utilizados para a memoria partilhada jogo
#define ComprimentoJanelaMAX 100
#define AlturaJanelaMAX 80
#define ComprimentoJanelaMIN 80
#define AlturaJanelaMIN 40



#define MaxClientes 10
#define MaxInvaders 60
#define MaxPowerUP 10
#define MaxDisparos 25

typedef enum _TipoMensagemCLI {
	INICIO,
	JOGANDO
} TipoMensagemCLI;

typedef struct _MsgCLI {
	TipoMensagemCLI tipo_mensagem;
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
	TCHAR proximaTecla[SIZE]; //para guardar utilixção por ciclo jogo
							  //estado do jogo é possivel
} Defender;

//As bombas e os tiros têm uma estrutura práticamente igual, portanto ficam na mesma estrutura
typedef struct Disparos {
	int x, y; //só ocupam um ponto por isso são não se usa area
	BOOL direcao; //As bombas sobes e os tiros descem
				  //bombas ficam com 1 tiros ficam com 0 !!!!!!!!!!!! Não sei se não é TRUE or FALSE
	TCHAR dono[SIZE]; //para incrementar a pontuação dos clientes
	int velocidade;
	int id_disparos;
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
		MsgSER cenas1;
		Pontuacao cenas2;
		Jogo cenas3;
	};
} MsgCliGat, *PMsgCliGat;
