#include <windows.h>
#include <tchar.h>
//Definir uma constante para facilitar a leitura do prot�tipo da fun��o
//Este .h deve ser inclu�do no projeto que o vai usar (modo impl�cito)
#define TAM 256
//Esta macro � definida pelo sistema caso estejamos na DLL (<DLL_IMP>_EXPORTS definida)
//ou na app (<DLL_IMP>_EXPORTS n�o definida) onde DLL_IMP � o nome deste projeto

//


#ifdef DlltpSO2_EXPORTS 
#define DlltpSO2 __declspec(dllexport)
#else
#define DlltpSO2 __declspec(dllimport)
#endif

#define MAX 50 //tamanho do buffer 
#define SIZE 20 //dimens�o da mensagem e do nome 
#define mPartilhadaMensagens TEXT("memPartilhadaMensagens")
#define TAMANHOBUFFER sizeof(BufferMensagens)

#define MaxClientes 10
#define MaxInvaders 60
#define MaxPowerUP 10
#define MaxDisparos 10



HANDLE SemaforoEscrever;
HANDLE SemaforoLer;
HANDLE MutexRead;
HANDLE MutexWrite;
HANDLE hMemoriaBuffer;
HANDLE hMemoriaJogo;



typedef enum _TipoMensagemCLI {
	INICIO,
	JOGANDO
} TipoMensagemCLI;

typedef enum _TipoMensagemSER {
	INICIO_JOGO,
	JOGADA
} TipoMensagemSER;

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

typedef struct _Input {
	int numInvaders;
	int	numDefenders;
	int numDisparos;
	int	numPowerUps;
	int numInvadersBase;
	int	numInvadersEsquivo;
	int	numInvadersOutros;
} Input, *PInput;



//Mensagem que vai do Cliente para o Gateway


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
	//estado do jogo � possivel
} Defender;

//As bombas e os tiros t�m uma estrutura pr�ticamente igual, portanto ficam na mesma estrutura
typedef struct Disparos {
	int x, y; //s� ocupam um ponto por isso s�o n�o se usa area
	BOOL direcao; //As bombas sobes e os tiros descem
				  //bombas ficam com 1 tiros ficam com 0 !!!!!!!!!!!! N�o sei se n�o � TRUE or FALSE
	TCHAR dono[SIZE]; //para incrementar a pontua��o dos clientes
	int velocidade;
} Disparos;



typedef struct _Jogo {

	int altura, comprimento;

	Invader Invaders[MaxInvaders];

	Defender Defenders[MaxClientes];

	PowerUP PowerUP[MaxPowerUP];

	Disparos Disparos[MaxDisparos];
	//Ver como s�o os power ups e os disparos !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	CicloDeVida CicloDeVida;

}Jogo, *PJogo;




TCHAR NomeSemaforoPodeLer[] = TEXT("Sem�foro Pode Ler");TCHAR NomeSemaforoPodeEscrever[] = TEXT("Sem�foro Pode Escrever");

#ifdef __cpluplus

extern "C" {
#endif
	//Vari�vel global da DLL
	extern DlltpSO2 int nDLL;	

	extern DlltpSO2 PBufferMensagens mensagens;

	extern DlltpSO2 PJogo jogo;
	//Fun��es a serem exportadas/importadas
	DlltpSO2 int UmaString(void);


	DlltpSO2 void IniciaSinc();

	

	DlltpSO2 void TrataMensagem();
	DlltpSO2 void EnviaMensagem();

#ifdef __cplusplus
}
#endif
