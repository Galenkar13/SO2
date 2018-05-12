#include <windows.h>
#include <tchar.h>
//Definir uma constante para facilitar a leitura do protótipo da função
//Este .h deve ser incluído no projeto que o vai usar (modo implícito)
#define TAM 256
//Esta macro é definida pelo sistema caso estejamos na DLL (<DLL_IMP>_EXPORTS definida)
//ou na app (<DLL_IMP>_EXPORTS não definida) onde DLL_IMP é o nome deste projeto

//


#ifdef DlltpSO2_EXPORTS 
#define DlltpSO2 __declspec(dllexport)
#else
#define DlltpSO2 __declspec(dllimport)
#endif

#define MAX 50 //tamanho do buffer 
#define SIZE 20 //dimensão da mensagem e do nome 
#define mPartilhadaMensagens TEXT("memPartilhadaMensagens")
#define TAMANHOBUFFER sizeof(BufferMensagens)

//Estes defines tem de estar na dll porque vão ser utilizados para a memoria partilhada jogo
#define ComprimentoJanelaMAX 100
#define AlturaJanelaMAX 80
#define ComprimentoJanelaMIN 80
#define AlturaJanelaMIN 40



#define MaxClientes 10
#define MaxInvaders 60
#define MaxPowerUP 10
#define MaxDisparos 25



HANDLE SemaforoEscrever;
HANDLE SemaforoLer;
HANDLE MutexRead;
HANDLE MutexWrite;
HANDLE hMemoriaBuffer;
HANDLE hMemoriaJogo;
HANDLE hMutexJogo;
HANDLE hMutexJogo2;
HANDLE hEventActiva;
HANDLE hEventLida;



typedef enum _TipoMensagemCLI {
	INICIO,
	JOGANDO
} TipoMensagemCLI;

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
	EXTRA,
	MORTO
} TipoInvader;


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
	Pontuacao pontos;
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
//Apenas uma coisa de cada tipo de obecto do jogo porque vai ser feita a actualização 1 a 1

typedef struct _MsgPosicoesJogo{
	Invader Invader;
	Defender Defender;
	PowerUP PowerUP;
	Disparos Disparo;
	CicloDeVida CicloDeVida;
} MsgPosicoesJogo, *PMsgPosicoesJogo;

//Gateway manda para o cliente uma estrutura com uma cena de cada
//e actualiza a cada alteraçaao 

typedef struct _MsgSER {
	Pontuacao TOP[10];
} MsgSER, *PMsgSER;

typedef enum _Direcao {
	direita,
	baixo,
	cima,
	esquerda
} Direcao;

#define MOVEMENT_INCREMENT 2

TCHAR NomeSemaforoPodeLer[] = TEXT("Semáforo Pode Ler");TCHAR NomeSemaforoPodeEscrever[] = TEXT("Semáforo Pode Escrever");
TCHAR EventoManda[] = TEXT("EventoManda");TCHAR EventoRecebe[] = TEXT("EventoRecebe");

TCHAR Mutex1[] = TEXT("MutexEnvia");
TCHAR Mutex2[] = TEXT("MutexRecebe");

TCHAR Mutex3[] = TEXT("MutexEnviaBuffer");
TCHAR Mutex4[] = TEXT("MutexRecebeBuffer");



#ifdef __cpluplus
extern "C" {
#endif


	DlltpSO2 PBufferMensagens mensagens;

	 DlltpSO2 PJogo jogo;
	//Funções a serem exportadas/importadas


	DlltpSO2 int IniciaSinc();
	DlltpSO2 void AcabaSinc();
	DlltpSO2 void TrataMensagem();
	DlltpSO2 void EnviaMensagem(); 
	DlltpSO2 void MoveInvaderBase(int id, int x, int y, int num);
	DlltpSO2  void RecebeAtualizacao(int id);


#ifdef __cplusplus
}
#endif
