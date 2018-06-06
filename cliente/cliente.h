#pragma once
//Estes defines tem de estar na dll porque vão ser utilizados para a memoria partilhada jogo

#include "../DlltpSO2/dll.h"
DWORD WINAPI arrancaComunicacaoCliente();
HANDLE hThread;


LRESULT CALLBACK DialogConfigurar(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

BOOL EnviaMensagemCLI(MsgCLI cli);
int Login(HWND hWnd);
int idJogador;
BOOL JOGANDO_CLI;