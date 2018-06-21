#pragma once
//Estes defines tem de estar na dll porque vão ser utilizados para a memoria partilhada jogo
#include <Windows.h>
#include "../DlltpSO2/dll.h"


#define CORTRANSPARENTE RGB(255,255,255)

HANDLE hThread;


//#define SOM_PERDEU_JOGO TEXT("play sound/explosion.wav")

#define MUSICA1 TEXT("251461__joshuaempyre__arcade-music-loop.wav")
#define SOM1 TEXT("shoot.wav")

LRESULT CALLBACK DialogConfigurar(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK DialogPipe(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


DWORD WINAPI arrancaComunicacaoCliente();
BOOL EnviaMensagemCLI(MsgCLI cli);
BOOL JOGANDO_CLI;
BOOL RecebeUpdates();

int Login(HWND hWnd);
int idJogador;
int Jogada(HWND hWnd, TECLA x);

MCIERROR ReproduzirSom(LPCTSTR som);
void ReproduzirMusica(LPCTSTR musica);

void Desenhar(MsgCliGat update);
void DesenhaElemento(HDC hdc, Area area, HBITMAP hBitmap);
void CarregaBitmaps();

TCHAR IP_PIPE[20];