#pragma once
//Estes defines tem de estar na dll porque vão ser utilizados para a memoria partilhada jogo

#include "../DlltpSO2/dll.h"

typedef struct _DadosCli {
	int idCliente;
} DadosCli;

#define CORTRANSPARENTE RGB(255,255,255)

DWORD WINAPI arrancaComunicacaoCliente();
HANDLE hThread;

//#define SOM_PERDEU_JOGO TEXT("play sound/explosion.wav")

#define MUSICA1 TEXT("251461__joshuaempyre__arcade-music-loop.wav")
#define MUSICA2 TEXT("shoot.wav")

LRESULT CALLBACK DialogConfigurar(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

BOOL EnviaMensagemCLI(MsgCLI cli);
int Login(HWND hWnd);
int idJogador;
BOOL JOGANDO_CLI;
int Jogada(HWND hWnd, TECLA x);


MCIERROR ReproduzirSom(LPCTSTR som);
void ReproduzirMusica(LPCTSTR musica);


void VaisDesenharCRL(MsgCliGat update);
void CarregaBitmaps();