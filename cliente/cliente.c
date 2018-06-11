#include <windows.h> 
#include <stdio.h>
#include <conio.h>
#include <tchar.h>
#include <stdlib.h>  
#include <string.h>  
#include <WinUser.h>
#include <conio.h>
#include <CommCtrl.h>
#include "cliente.h"
#include "resource2.h"

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
static TCHAR szTitle[] = _T("Win32 Guided Tour Application");

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
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	JOGANDO_CLI = FALSE;
	//arrancaComunicacaoCliente();

	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL,
			_T("Call to RegisterClassEx failed!"),
			_T("Win32 Guided Tour"),
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
	HWND hWnd = CreateWindow(
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
			if(!JOGANDO_CLI)
				Login(hWnd);	
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

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hDC, MemDCExercising;
	PAINTSTRUCT Ps;
	HBITMAP bmpCenas;

	switch (message)
	{
	case WM_CREATE:
		break;
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &Ps);

		// Load the bitmap from the resource
		bmpCenas = LoadImage(hInst, TEXT("maxresdefault.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		// Create a memory device compatible with the above DC variable
		MemDCExercising = CreateCompatibleDC(hDC);
		// Select the new bitmap
		SelectObject(MemDCExercising, bmpCenas);

		// Copy the bits from the memory DC into the current dc
		BitBlt(hDC, 10, 10, 2000, 2000, MemDCExercising, 0, 0, SRCCOPY);

		// Restore the old bitmap
		DeleteDC(MemDCExercising);
		DeleteObject(bmpCenas);
		EndPaint(hWnd, &Ps);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ID_INCIARJOGO:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1_Jogar), hWnd, DialogConfigurar);
			break;
		case ID_SAIR:

			break;
		}

		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}

	return 0;
}


int Login(HWND hWnd) {
	TCHAR buff[254];
	MsgCLI mensagem_cli;
	mensagem_cli.id = -1;
	mensagem_cli.tecla = DIREITA;
	mensagem_cli.tipo_mensagem = INICIO;
	GetWindowText(GetDlgItem(hWnd, IDC_EDIT_INPUT), mensagem_cli.nome, sizeof(buff));
	EnviaMensagemCLI(mensagem_cli);
	JOGANDO_CLI = TRUE;

}