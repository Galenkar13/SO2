#include <windows.h> 
#include <stdio.h>
#include <conio.h>
#include <tchar.h>
#include <stdlib.h>  
#include <string.h>  
#include <WinUser.h>
#include <Windows.h>
#include <conio.h>
#include <CommCtrl.h>

#include "cliente.h"
#include "resource2.h"

HBITMAP Bitmaps[14];
HWND hWnd;
HDC hDC;
RECT rect;

/*
int _tmain() {

	hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)arrancaComunicacaoCliente, NULL, 0, NULL);

	WaitForSingleObject(hThread, INFINITE);

	return 0;

}*/



// GT_HelloWorldWin32.cpp  
// compile with: /D_UNICODE /DUNICODE /DWIN32 /D_WINDOWS /c  

//#include "resource.h"

// Global variables  

// The main window class name.  
static TCHAR szWindowClass[] = _T("win32app");

// The string that appears in the application's title bar.  
static TCHAR szTitle[] = _T("Cliente - Space Invaders");

// Forward declarations of functions included in this code module:  
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

HINSTANCE hInst;


//cenas cenas123;


int CALLBACK WinMain(
	_In_ HINSTANCE hInstance,
	_In_ HINSTANCE hPrevInstance,
	_In_ LPSTR     lpCmdLine,
	_In_ int       nCmdShow
)
{
	hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)arrancaComunicacaoCliente, NULL, 0, NULL);
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	//wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hIcon = LoadIcon(NULL, IDI_WARNING);
	wcex.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0));

	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	JOGANDO_CLI = FALSE;
	
	CarregaBitmaps();
	//arrancaComunicacaoCliente();

	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL,
			_T("Call to RegisterClassEx failed!"),
			_T("Cliente - Space Invaders"),
			NULL);

		return 1;
	}

	hInst = hInstance; // Store instance handle in our global variable  

					   // The parameters to CreateWindow explained:  
					   // szWindowClass: the name of the application  
					   // szTitle: the text that appears in the title bar  
					   // WS_OVERLAPPEDWINDOW: the type of window to create  
					   // CW_USEDEFAULT, CW_USEDEFAULT: initial position (x, y)  
					   // 500, 100: initial size (width, length)  
					   // NULL: the parent of this window  
					   // NULL: this application does not have a menu bar  
					   // hInstance: the first parameter from WinMain  
					   // NULL: not used in this application  
	hWnd = CreateWindow(
		szWindowClass,
		szTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		500, 500,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	if (!hWnd)
	{
		MessageBox(NULL,
			_T("Call to CreateWindow failed!"),
			_T("Janela Servidor"),
			NULL);

		return 1;
	}

	// The parameters to ShowWindow explained:  
	// hWnd: the value returned from CreateWindow  
	// nCmdShow: the fourth parameter from WinMain  
	ShowWindow(hWnd,
		nCmdShow);
	UpdateWindow(hWnd);

	// Main message loop:  
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

//  
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)  
//  
//  PURPOSE:  Processes messages for the main window.  
//  
//  WM_PAINT    - Paint the main window  
//  WM_DESTROY  - post a quit message and return  
//  
//  

LRESULT CALLBACK DialogConfigurar(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{


	// DIALOGO DE CONFIGURAÇÃO DE PARAMETROS DE JOGO
	switch (message)
	{
	case WM_INITDIALOG:
	{

	}

	break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
		{
			if (!JOGANDO_CLI) {
				Login(hWnd);
				//DialogBox(hInst, MAKEINTRESOURCE(IDD_FORMVIEW_Cabecalho), hWnd, NULL);

				EnableWindow(GetParent(hWnd), TRUE);
				EndDialog(hWnd, 0);
				break;
			}
		}

		break;
		case IDCANCEL:

			EnableWindow(GetParent(hWnd), TRUE);
			EndDialog(hWnd, 0);
			break;
		}
		break;

	case WM_DESTROY:
		EndDialog(hWnd, 0);
		//PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

void DesenharObjeto(HDC hdc, Area area, HBITMAP hBitmap);

LRESULT CALLBACK WndProc(HWND hWndJanela, UINT message, WPARAM wParam, LPARAM lParam)
{

	//AQUI E QUE E PARA TRATAR AS TECLAS

	//HDC hDC, MemDCExercising;
	//HDC hDC1, MemDCExercising;
	HDC hdc2 = NULL;
	PAINTSTRUCT Ps;
	//HBITMAP bmpCenas;
	int res = 0;
	TECLA tecla;
	RECT rect;

	switch (message)
	{
	case WM_CREATE:
		hdc2 = GetDC(hWnd);
		hDC = CreateCompatibleDC(hdc2);
		HBITMAP bitmapComp = CreateCompatibleBitmap(hdc2, 500, 500);

		SelectObject(hDC, bitmapComp);

		ReleaseDC(hWnd, hdc2);
		break;
	case WM_PAINT:
	{

		Area area;
		area.x = 0;
		area.y = 0;
		area.altura = 250;
		area.comprimento = 150;

		hdc2 = BeginPaint(hWnd, &Ps);
	//	DesenharObjeto(hDC, area, bmpCenas);

		BitBlt(hdc2, 0, 0, 500, 500, hDC, 0, 0, SRCCOPY);
		EndPaint(hWnd, &Ps);
	}
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ID_INCIARJOGO:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1_Jogar), hWndJanela, DialogConfigurar);


			break;
		case ID_SAIR:
			res = MessageBox(hWndJanela, TEXT("Pretende Sair?"), TEXT("CONFIRMACAO...."), MB_YESNO);
			if (res == IDYES)
			{
				DestroyWindow(hWndJanela);
			}
			break;
		}

		break;
	case WM_KEYDOWN:
		if (JOGANDO_CLI) {
			switch (wParam)
			{
			case VK_LEFT:
				tecla = ESQUERDA;
				Jogada(hWndJanela, tecla);

				break;

			case VK_RIGHT:

				tecla = DIREITA;
				Jogada(hWndJanela, tecla);

				break;

			case VK_UP:

				tecla = CIMA;
				Jogada(hWndJanela, tecla);

				break;

			case VK_DOWN:

				tecla = BAIXO;
				Jogada(hWndJanela, tecla);

				break;
			case VK_SPACE:

				tecla = ESPAÇO;
				Jogada(hWndJanela, tecla);

				break;

				// Process other non-character keystrokes. 

			default:
				break;
			}
		}
		break;
	/*case WM_CHAR:
		switch (wParam)
		{

		case 0x1B:

			// Process an escape. 
			res = MessageBox(hWndJanela, TEXT("Pretende Sair?"), TEXT("CONFIRMACAO...."), MB_YESNO);
			if (res == IDYES)
			{
				DestroyWindow(hWndJanela);
			}

			break;


		default:

			// Process displayable characters. 

			break;
		}*/
	case WM_CLOSE:     //FECHAR JANELA (X) //ex2_ficha6 

		res = MessageBox(hWndJanela, TEXT("Pretende Sair?"), TEXT("CONFIRMACAO...."), MB_YESNO);
		if (res == IDYES)
		{

			DestroyWindow(hWndJanela);
		}

		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWndJanela, message, wParam, lParam);
		break;
	}

	return 0;
}

void CarregaBitmaps() {
	Bitmaps[0] = LoadImage(hInst, TEXT("SPBichob.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	Bitmaps[1] = LoadImage(hInst, TEXT("SPBichocLaranja.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	Bitmaps[2] = LoadImage(hInst, TEXT("SINaveAzul.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	Bitmaps[3] = LoadImage(hInst, TEXT("SINaveVermelho.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	Bitmaps[4] = LoadImage(hInst, TEXT("SITiroAzul.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	Bitmaps[5] = LoadImage(hInst, TEXT("SITiroVermelho.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	Bitmaps[6] = LoadImage(hInst, TEXT("SIVida.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	Bitmaps[7] = LoadImage(hInst, TEXT("SIGelo.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	Bitmaps[8] = LoadImage(hInst, TEXT("SIMais.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	Bitmaps[9] = LoadImage(hInst, TEXT("SPBattery.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	Bitmaps[10] = LoadImage(hInst, TEXT("SPBeer.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	Bitmaps[11] = LoadImage(hInst, TEXT("SIBomba.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	Bitmaps[12] = LoadImage(hInst, TEXT("SIEscudo.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

}


int Login(HWND hWnd) {
	TCHAR buff[254];
	MsgCLI mensagem_cli;
	mensagem_cli.id = -1;
	mensagem_cli.tecla = OUTRA_TECLA;
	mensagem_cli.tipo_mensagem = INICIO;
	GetWindowText(GetDlgItem(hWnd, IDC_EDIT_INPUT), mensagem_cli.nome, sizeof(buff));
	EnviaMensagemCLI(mensagem_cli);
	JOGANDO_CLI = TRUE;

	return 0;
}

int Jogada(HWND hWnd, TECLA x) {
	TCHAR buff[254];
	MsgCLI mensagem_cli;
	mensagem_cli.id = idJogador;
	mensagem_cli.tecla = x;
	_tcscpy(mensagem_cli.nome, TEXT("oi"));
	mensagem_cli.tipo_mensagem = JOGANDO;
	EnviaMensagemCLI(mensagem_cli);
	return 0;
}



void DesenharObjeto(HDC hdc, Area area, HBITMAP hBitmap)
{
	RECT rect;
	BITMAP bitmap;
	LONG largura = area.comprimento;
	LONG altura = area.altura;

	if (!hdc || !hBitmap)
		return;

	HDC auxDC = CreateCompatibleDC(hdc);

	if (auxDC == NULL)
	{
		TCHAR buf[256];

		_stprintf_s(buf, 256, TEXT("[CLIENTE] CreateCompatibleDC. GLE=%d\n"), GetLastError());
		OutputDebugString(buf);
		return;
	}

	GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&bitmap);
	SelectObject(auxDC, hBitmap);

	//Redimensionar + Remover fundo
	TransparentBlt(hdc, area.x, area.y, largura, altura,
		auxDC, 0, 0, bitmap.bmWidth, bitmap.bmHeight, CORTRANSPARENTE);

	//StretchBlt(hdc, 0, 0, 500, 500,
	//	auxDC, 0, 0, bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);

	int err = GetLastError();

	DeleteDC(auxDC);
}

void VaisDesenharCRL(MsgCliGat update) {
	int i, vidas, pontos;
	TCHAR text[10];
	TCHAR text1[10];
	TCHAR text2[10];
	TCHAR text3[] = TEXT("Id Jogador:");
	TCHAR text4[] = TEXT("vidas:");
	TCHAR text5[] = TEXT("Pontuacao:");


	vidas = update.JogoCopia.Defenders[idJogador].vidas;
	pontos = update.JogoCopia.Defenders[idJogador].pontos;
	_itot_s(vidas, text, 10, 10);
	_itot_s(pontos, text1, 10, 10);
	_itot_s(idJogador, text2, 10, 10);

	rect.bottom = 500;
	rect.top = 0;
	rect.left = 0;
	rect.right = 500;
	FillRect(hDC,&rect, GetStockObject(BLACK_BRUSH));
	SetTextColor(hDC, RGB(255, 155, 130));
	SetBkColor(hDC, RGB(0, 0, 0));

	TextOut(hDC, 5, 5, text3, 11);
	TextOut(hDC, 80, 5, text, 2);
	TextOut(hDC, 120, 5, text5, 10);
	TextOut(hDC, 200, 5, text1, 2);
	TextOut(hDC, 240, 5, text4, 6);
	TextOut(hDC, 290, 5, text2, 2);
	for (i = 0; i < update.JogoCopia.Dados.nDefenders; i++) {
		if (update.JogoCopia.Defenders[i].id_defender != -1) {
			if (update.JogoCopia.Defenders[i].id_defender == idJogador) {
				DesenharObjeto(hDC, update.JogoCopia.Defenders[i].area, Bitmaps[3]);
			//	ReproduzirSom(MUSICA2);
			}
			else
				DesenharObjeto(hDC, update.JogoCopia.Defenders[i].area, Bitmaps[2]);
		}
	}

	for (i = 0; i < update.JogoCopia.Dados.nInvaders; i++) {
		if (update.JogoCopia.Invaders[i].id_invader != -1) {
			if (i % 2 == 0) {
				DesenharObjeto(hDC, update.JogoCopia.Invaders[i].area, Bitmaps[0]);
			}
			else
				DesenharObjeto(hDC, update.JogoCopia.Invaders[i].area, Bitmaps[1]);
		}
	}

	for (i = 0; i < update.JogoCopia.Dados.nBombas; i++) {
		if (update.JogoCopia.Bombas[i].id_bombas != -1) {
			DesenharObjeto(hDC, update.JogoCopia.Bombas[i].area, Bitmaps[11]);
		}
	}

	for (i = 0; i < update.JogoCopia.Dados.nTiros; i++) {
		if (update.JogoCopia.Tiros[i].id_tiros != -1) {
			if (update.JogoCopia.Tiros[i].id_dono == idJogador) {
				DesenharObjeto(hDC, update.JogoCopia.Tiros[i].area, Bitmaps[5]);
			}
			else
				DesenharObjeto(hDC, update.JogoCopia.Tiros[i].area, Bitmaps[4]);
		}
	}

	for (i = 0; i < update.JogoCopia.Dados.nPowerUPs; i++) {
		if (update.JogoCopia.PowerUP[i].id_powerUP != -1) {
			switch (update.JogoCopia.PowerUP[i].tipo)
			{
			case GELO:
				DesenharObjeto(hDC, update.JogoCopia.PowerUP[i].area, Bitmaps[7]);
				break;
			case BATERIA:
				DesenharObjeto(hDC, update.JogoCopia.PowerUP[i].area, Bitmaps[9]);
				break;
			case ESCUDO:
				DesenharObjeto(hDC, update.JogoCopia.PowerUP[i].area, Bitmaps[12]);
				break;
			case MAIS:
				DesenharObjeto(hDC, update.JogoCopia.PowerUP[i].area, Bitmaps[8]);
				break;
			case VIDA:
				DesenharObjeto(hDC, update.JogoCopia.PowerUP[i].area, Bitmaps[6]);
				break;
			case ALCOOL:
				DesenharObjeto(hDC, update.JogoCopia.PowerUP[i].area, Bitmaps[10]);
				break;
			default:
				break;
			}

		}
	}

	InvalidateRect(hWnd, NULL, TRUE);
}