#include "servidor.h"
#include"resource.h" 
#include <tchar.h>
#include <fcntl.h>
#include <io.h>
#include <stdio.h>
#include <string.h>  

// The main window class name.  
static TCHAR szWindowClass[] = _T("win32app");

// The string that appears in the application's title bar.  
static TCHAR szTitle[] = _T("Space Invaders the Game!!");

// Forward declarations of functions included in this code module:  
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

typedef struct _cenas {
	TCHAR yo[254];
} cenas;

cenas cenas123;

HINSTANCE hInst;

HWND janela;

int sentido = 0;

int CALLBACK WinMain(
	_In_ HINSTANCE hInstance,
	_In_ HINSTANCE hPrevInstance,
	_In_ LPSTR     lpCmdLine,
	_In_ int       nCmdShow
)
{
	DWORD threadId;


	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	//wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hIcon = LoadIcon(NULL, IDI_SHIELD);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1_Servidor);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0));

	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL,
			_T("Call to RegisterClassEx failed!"),
			_T("Win32 Guided Tour"),
			NULL);

		return 1;
	}


	jogo->CicloDeVida = CRIACAO; //este vai ser para o botao de configuraçao



	continua = 1;
	hMutexJogo = CreateMutex(NULL, FALSE, TEXT("GoMutex"));
	hEvento = CreateEvent(NULL, TRUE, FALSE, TEXT("GoEvent"));

	hThreadLeitor = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadConsumidor, NULL, 0, &threadId);
	if (hThreadLeitor != NULL)
		_tprintf(TEXT("Lancei uma thread com id %d\n"), threadId);
	else {
		_tprintf(TEXT("Erro ao criar Thread Escritor\n"));
		return -1;
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

BOOL RecebeConfiguracao(HWND hWnd) {
	TCHAR aux[254];

	GetWindowText(GetDlgItem(hWnd, IDC_EDIT1_nInvaders), aux, sizeof(aux)); //jogo->nInvaders
	jogo->Dados.nInvaders = _wtoi(aux);
	GetWindowText(GetDlgItem(hWnd, IDC_EDIT2_velocidadeInvader), aux, sizeof(aux)); // Sleep para o movimento dos Invaders 
	jogo->Dados.velocidadeInvaders = _wtoi(aux);
	GetWindowText(GetDlgItem(hWnd, IDC_EDIT3_ProbabilidadeInvader), aux, sizeof(aux)); // Probabilidade para verificar se Invaders Disparam ou nao
	jogo->Dados.probabilidadeInvaderDisparar = _wtoi(aux);
	GetWindowText(GetDlgItem(hWnd, IDC_EDIT4_VelocidadePowerUP), aux, sizeof(aux)); // Sleep para o movimento dos PowerUps 
	jogo->Dados.velocidadePowerUps = _wtoi(aux);
	GetWindowText(GetDlgItem(hWnd, IDC_EDIT5_DuracaoPowerUP), aux, sizeof(aux)); // Probabilidade verificacao dos PowerUps
	jogo->Dados.duracaoPowerUp = _wtoi(aux);
	GetWindowText(GetDlgItem(hWnd, IDC_EDIT6_VelocidadeTiroInvader), aux, sizeof(aux)); // Sleep de mover bombas
	jogo->Dados.velocidadeBomba = _wtoi(aux);
	GetWindowText(GetDlgItem(hWnd, IDC_EDIT7_NumeroDeVidas), aux, sizeof(aux));
	jogo->Dados.nVidas = _wtoi(aux);
	GetWindowText(GetDlgItem(hWnd, IDC_EDIT1_ProbDropPowerUP), aux, sizeof(aux));
	jogo->Dados.probabilidadePowerUp = _wtoi(aux);
	GetWindowText(GetDlgItem(hWnd, IDC_EDIT2_VelocidadeDefender), aux, sizeof(aux));
	jogo->Dados.velocidadeDefenders = _wtoi(aux);
	GetWindowText(GetDlgItem(hWnd, IDC_EDIT1_velocidadeTiroDefender), aux, sizeof(aux));
	jogo->Dados.velocidadeTiro = _wtoi(aux);

	if (jogo->Dados.nInvaders <= MAXNUMEROINVADERS &&
		jogo->Dados.velocidadeInvaders <= velocidadeInvaderBaseMax &&
		jogo->Dados.probabilidadeInvaderDisparar <= ProbabilidadeTiroInvaderMax &&
		jogo->Dados.velocidadePowerUps <= velocidadePowerUPMax &&
		jogo->Dados.duracaoPowerUp <= DuracaoPowerUPMax &&
		jogo->Dados.velocidadeTiro <= MAXVELOCIDADETIRODEFENDER &&
		jogo->Dados.nVidas <= MAXVIDASJOGADOR &&
		jogo->Dados.velocidadeBomba <= velocidadeDisparoInvaderMax &&
		jogo->Dados.probabilidadePowerUp <= MAXPROBABILIDADEPOWERUP &&
		jogo->Dados.velocidadeDefenders <= MAXVELOCIDADEDEFENDER &&
		jogo->Dados.nInvaders >= MINNUMEROINVADERS &&
		jogo->Dados.velocidadeInvaders >= velocidadeInvaderBaseMin &&
		jogo->Dados.probabilidadeInvaderDisparar >= ProbabilidadeTiroInvaderMin &&
		jogo->Dados.velocidadePowerUps >= velocidadePowerUPMin &&
		jogo->Dados.duracaoPowerUp >= DuracaoPowerUPMin &&
		jogo->Dados.velocidadeTiro >= MINVELOCIDADETIRODEFENDER &&
		jogo->Dados.nVidas >= MINVIDASJOGADOR &&
		jogo->Dados.velocidadeBomba >= velocidadeDisparoInvaderMin && 
		jogo->Dados.probabilidadePowerUp >= MINPROBABILIDADEPOWERUP &&
		jogo->Dados.velocidadeDefenders >= MINVELOCIDADEDEFENDER
		) {
		return TRUE;
	}



	return FALSE;
}

LRESULT CALLBACK DialogConfigurar(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	// DIALOGO DE CONFIGURAÇÃO DE PARAMETROS DE JOGO
	switch (message)
	{
	case WM_INITDIALOG:
	{
		TCHAR text[100];
		_itot_s(NUMEROINVADERS, text, 100, 10);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT1_nInvaders), WM_SETTEXT, 0, (LPARAM)text);
		_itot_s(velocidadeInvaderBase, text, 100, 10);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT2_velocidadeInvader), WM_SETTEXT, 0, (LPARAM)text);
		_itot_s(ProbabilidadeTiroInvader, text, 100, 10);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT3_ProbabilidadeInvader), WM_SETTEXT, 0, (LPARAM)text);
		_itot_s(velocidadePowerUP, text, 100, 10);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT4_VelocidadePowerUP), WM_SETTEXT, 0, (LPARAM)text);
		_itot_s(DuracaoPowerUP, text, 100, 10);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT5_DuracaoPowerUP), WM_SETTEXT, 0, (LPARAM)text);
		_itot_s(velocidadeDisparoInvader, text, 100, 10);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT6_VelocidadeTiroInvader), WM_SETTEXT, 0, (LPARAM)text);
		_itot_s(VIDASJOGADOR, text, 100, 10);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT7_NumeroDeVidas), WM_SETTEXT, 0, (LPARAM)text);
		_itot_s(PROBABILIDADEPOWERUP, text, 100, 10);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT1_ProbDropPowerUP), WM_SETTEXT, 0, (LPARAM)text);
		_itot_s(VELOCIDADEDEFENDER, text, 100, 10);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT2_VelocidadeDefender), WM_SETTEXT, 0, (LPARAM)text);
		_itot_s(VELOCIDADETIRODEFENDER, text, 100, 10);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT1_velocidadeTiroDefender), WM_SETTEXT, 0, (LPARAM)text);

		
	}

	break;
	case WM_COMMAND:
		if (HIWORD(wParam) == EN_CHANGE) {
			EnableWindow(GetDlgItem(hWnd, IDOK), RecebeConfiguracao(hWnd));
		}
		else
			switch (LOWORD(wParam))
			{
			case IDOK:
			{
				if (jogo->CicloDeVida == CRIACAO) {
					RecebeConfiguracao(hWnd);
					IniciaDefenders();
					IniciaInvaders();
					IniciaTiros();
					IniciaPowerUp();
					IniciaBombas();
					ColocaInvaders();
					jogo->CicloDeVida = ASSOCIACAO;
					EnableWindow(GetParent(hWnd), TRUE);
					EndDialog(hWnd, 0);
				}
				EnableWindow(GetParent(hWnd), TRUE);
				EndDialog(hWnd, 0);

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



LRESULT CALLBACK DialogIniciar(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	janela = hWnd;
	// DIALOGO DE CONFIGURAÇÃO DE PARAMETROS DE JOGO
	switch (message)
	{
	case WM_INITDIALOG:
	{
		TCHAR text[100];

		_itot_s(jogo->Dados.nDefenders, text, 100, 10);

		SendMessage(GetDlgItem(hWnd, IDC_STATIC_Valor), WM_SETTEXT, 0, (LPARAM)text);
	}

	break;
	case SinalUser:
	{

		TCHAR text[100];

		_itot_s(jogo->Dados.nDefenders, text, 100, 10);

		SendMessage(GetDlgItem(hWnd, IDC_STATIC_Valor), WM_SETTEXT, 0, (LPARAM)text);
	}
	break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
		{
			if (jogo->CicloDeVida == ASSOCIACAO) {
				jogo->CicloDeVida = DECORRER;
			//	ReleaseMutex(hMutexJogo);
				WaitForSingleObject(hMutexJogo,INFINITE);
				ColocaDefenders();
				//CreateThreadsElementosJogo();
				hThreadJogadores = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadJogadores, 0, 0, NULL);
				hThreadTiros = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadTiros, 0, 0, NULL);
				hThreadInvadersBase = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadInvadersBase, 0, 0, NULL);
				hThreadPowerups = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadPowerups, 0, 0, NULL);
				hThreadBombas = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadBombas, 0, 0, NULL);

				if (hThreadBombas == NULL || hThreadPowerups == NULL || hThreadTiros == NULL || hThreadJogadores == NULL || hThreadInvadersBase == NULL) {
					_tprintf(TEXT("[Erro]Criação de objectos do Windows(%d)\n"), GetLastError());
					return -1;
				}

				SetEvent(hEvento);
				ResetEvent(hEvento);
				ReleaseMutex(hMutexJogo);
				EnableWindow(GetParent(hWnd), TRUE);
				EndDialog(hWnd, 0);
			}
			else
				break;
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
	int res = 0;

	switch (message)
	{
	case WM_CREATE:
		break;
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &Ps);

		// Load the bitmap from the resource
		bmpCenas = LoadImage(hInst, TEXT("SIServer.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		// Create a memory device compatible with the above DC variable
		MemDCExercising = CreateCompatibleDC(hDC);
		// Select the new bitmap
		SelectObject(MemDCExercising, bmpCenas);

		// Copy the bits from the memory DC into the current dc
		BitBlt(hDC,85, 75, 300, 300, MemDCExercising, 0, 0, SRCCOPY);

		// Restore the old bitmap
		DeleteDC(MemDCExercising);
		DeleteObject(bmpCenas);
		EndPaint(hWnd, &Ps);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ID_NOVOJOGO_CONFIGURAR:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1_configurar), hWnd, DialogConfigurar);
			break;
		case ID_NOVOJOGO_INCIAR:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1_Iniciar), hWnd, DialogIniciar);
			break;
		case Sair:
			res = MessageBox(hWnd, TEXT("Pretende Sair?"), TEXT("CONFIRMACAO...."), MB_YESNO);
			if (res == IDYES)
			{
				DestroyWindow(hWnd);
			}
			break;
		}

		break;
	case WM_CHAR:
		switch (wParam)
		{

		case 0x1B:

			// Process an escape. 
			res = MessageBox(hWnd, TEXT("Pretende Sair?"), TEXT("CONFIRMACAO...."), MB_YESNO);
			if (res == IDYES)
			{
				DestroyWindow(hWnd);
			}

			break;


		default:

			// Process displayable characters. 

			break;
		}
	case WM_CLOSE:     //FECHAR JANELA (X) //ex2_ficha6 

		res = MessageBox(hWnd, TEXT("Pretende Sair?"), TEXT("CONFIRMACAO...."), MB_YESNO);
		if (res == IDYES)
		{
			DestroyWindow(hWnd);
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

int CreateThreadsInvaders() {

	hThreadInvadersBase = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadInvadersBase, 0, 0, NULL);
	//hThreadInvadersEsquivos = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadInvadersEsquivo, 0, 0, NULL);
	//hThreadsInvadersExtras = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadInvadersExtra, 0, 0, NULL);

/*	if (hThreadInvadersBase == NULL || hThreadInvadersEsquivos == NULL || hThreadsInvadersExtras == NULL) {
		_tprintf(TEXT("[Erro]Criação de objectos do Windows(%d)\n"), GetLastError());
		return -1;
	}

	return 0;*/

}

int CreateThreadsElementosJogo()
{

	CreateThreadsInvaders();
	hThreadBombas = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadBombas, 0, 0, NULL);
	hThreadPowerups = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadPowerups, 0, 0, NULL);
	hThreadTiros = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadTiros, 0, 0, NULL);
	hThreadJogadores = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadJogadores, 0, 0, NULL);



	if (hThreadBombas == NULL || hThreadPowerups == NULL || hThreadTiros == NULL || hThreadJogadores == NULL) {
		_tprintf(TEXT("[Erro]Criação de objectos do Windows(%d)\n"), GetLastError());
		return -1;
	}

	return 0;

}


DWORD WINAPI ThreadInvadersBase()
{
	int flag = 1;
	_tprintf(TEXT("Iniciei Thread Invaders \n"));
	WaitForSingleObject(hMutexJogo, INFINITE);
	while (jogo->CicloDeVida == DECORRER) {



		for (int i = 0; i < jogo->Dados.nInvaders; i++)
		{
			
			if (jogo->Invaders[i].id_invader != -1)
			{
			//	jogo->Invaders[i].area.x =	jogo->Invaders[i].area.x + 5;
				sentido = MoveInvaders(sentido);

				
			}

		}
	//	WaitForSingleObject(hMutexJogo,INFINITE);
		int x = 0;
		for (int i = 0; i < jogo->Dados.nInvaders; i++) {
			if (jogo->Invaders[i].id_invader == -1)
				x++;
		}

		if (x == jogo->Dados.nInvaders) {
			jogo->Dados.nInvaders = jogo->Dados.nInvaders + 5;
			IniciaInvaders();
			IniciaTiros();
			IniciaPowerUp();
			IniciaBombas();
			ColocaDefenders();
			ColocaInvaders();

			jogo->Dados.nivel++;
		}

		
		AtiraBomba();

		SetEvent(hEvento);
		ResetEvent(hEvento);
		ReleaseMutex(hMutexJogo);
		Sleep(jogo->Dados.velocidadeInvaders);
		//Sleep(2000);
	}
	return 0;
}

DWORD WINAPI ThreadPowerups()
{
	_tprintf(TEXT("Iniciei Thread PowerUp \n"));
	WaitForSingleObject(hMutexJogo, INFINITE);


	while (jogo->CicloDeVida == DECORRER) {

		for (int i = 0; i < jogo->Dados.nPowerUPs; i++)
		{
			if (jogo->PowerUP[i].id_powerUP != -1)
			{
				MovePowerUp(jogo->PowerUP[i].id_powerUP);
			}

		}

		SetEvent(hEvento);
		ResetEvent(hEvento);
		ReleaseMutex(hMutexJogo);

		//Sleep(2000);
		//Sleep(jogo->Dados.velocidadeBomba);

		Sleep(jogo->Dados.velocidadePowerUps);
		//Sleep(1000);
	}
	return 0;
}

DWORD WINAPI ThreadBombas() {  //adaptar com a estrutura bomba...
	_tprintf(TEXT("Iniciei Thread bomba \n"));

	WaitForSingleObject(hMutexJogo, INFINITE);

	while (jogo->CicloDeVida == DECORRER) {

		for (int i = 0; i < jogo->Dados.nBombas; i++)
		{
			if (jogo->Bombas[i].id_bombas != -1)
			{
				//funcao para mover as bombas
				MoveBomba(jogo->Bombas[i].id_bombas);

			}

		}

		SetEvent(hEvento);
		ResetEvent(hEvento);
		ReleaseMutex(hMutexJogo);

	//	Sleep(2000);
		Sleep(jogo->Dados.velocidadeBomba);
	}
	return 0;
}

DWORD WINAPI ThreadTiros()
{
	_tprintf(TEXT("Iniciei Thread Tiro \n"));

	while (jogo->CicloDeVida == DECORRER) {

		WaitForSingleObject(hMutexJogo, INFINITE);

		for (int i = 0; i < jogo->Dados.nTiros; i++)
		{
			if (jogo->Tiros[i].id_tiros != -1)
			{
				MoveTiro(jogo->Tiros[i].id_tiros);
			}

		}

		SetEvent(hEvento);
		ResetEvent(hEvento);
		ReleaseMutex(hMutexJogo);
		Sleep(jogo->Dados.velocidadeTiro);
	//	Sleep(100);
	}
	return 0;
}

DWORD WINAPI ThreadJogadores()
{
	while (jogo->CicloDeVida == DECORRER) {

		//funcao para colocarinvaders
		//WaitForSingleObject(hMutexJogo, INFINITE);

		WaitForSingleObject(hMutexJogo, INFINITE);

		for (int i = 0; i <jogo->Dados.nDefenders; i++)
		{
			if (jogo->Defenders[i].id_defender != -1)
			{
				MoveDefender(i);			

			}



		}
	
		SetEvent(hEvento);
		ResetEvent(hEvento);
		ReleaseMutex(hMutexJogo);
		//Sleep(1000);
		Sleep(jogo->Dados.velocidadeDefenders);
	}
	return 0;
}

void InicializaJogo() { //acabar isto
	int i;

	for (i = 0; i < MaxClientes; i++) {
		jogo->Defenders[i].area.x = 0;
		jogo->Defenders[i].area.y = 0;
		jogo->Defenders[i].area.altura = 0;
		jogo->Defenders[i].area.comprimento = 0;
		jogo->Defenders[i].pontos = 0;
		jogo->Defenders[i].velocidade = 0;
		jogo->Defenders[i].vidas = 0;
		jogo->Defenders[i].powerUP.area.x = 0;
	}

}

DWORD WINAPI ThreadConsumidor(LPVOID param) { //LADO DO SERVIDOR passar po servidor
	int x = 0;
	int i;
	MsgCLI dados;

	while (jogo->CicloDeVida != FINAL) { //Isto é para passar para a DLL

		dados = TrataMensagem();

		WaitForSingleObject(hMutexJogo, INFINITE);

		//WaitForSingleObject(hMutexJogo, INFINITE);
		if (jogo->CicloDeVida == ASSOCIACAO) {
			for (i = 0; i < MaxClientes; i++) {
				if (jogo->Defenders[i].id_defender == -1) {
					jogo->Defenders[i].id_defender = i;
					_tcscpy(jogo->Defenders[i].nome, dados.nome);
					jogo->Dados.nDefenders++;
					PostMessage(janela, SinalUser, 0, 0);
					break;
				}
			}
		}
		else
			if (jogo->CicloDeVida == DECORRER) {
				for (i = 0; i < jogo->Dados.nDefenders; i++) {
					if (jogo->Defenders[i].id_defender == dados.id) {
						jogo->Defenders[i].proxima_jogada = dados.tecla;
						break;
					}
				}
			}

		_tprintf(TEXT("[Event Generated] \n"));
		SetEvent(hEvento);
		ResetEvent(hEvento);
		ReleaseMutex(hMutexJogo);

	}

	return 0;
}

void IniciaDefenders()
{
	jogo->Dados.nDefenders = 0;
	int i = 0;
	for (i = 0; i < MaxClientes; i++)
	{

		jogo->Defenders[i].area.x = 0;
		jogo->Defenders[i].area.y = 0;
		jogo->Defenders[i].id_defender = -1;
		_tcscpy_s(jogo->Defenders[i].nome, sizeof(jogo->Defenders[i].nome), TEXT("nada"));
		jogo->Defenders[i].pontos = 0;
		jogo->Defenders[i].velocidade = 0;
		jogo->Defenders[i].vidas = jogo->Dados.nVidas;
		jogo->Defenders[i].proxima_jogada = OUTRA_TECLA;
	}

}

void IniciaInvaders() {

	int i = 0;
	for (i = 0; i < MaxInvaders; i++) {
			jogo->Invaders[i].area.x = 0;
			jogo->Invaders[i].area.y = 0;
			jogo->Invaders[i].area.altura = AlturaInvader;
			jogo->Invaders[i].area.comprimento = ComprimentoInvader;
			jogo->Invaders[i].tipo = BASICO;
			jogo->Invaders[i].velocidade = velocidadeInvaderBase;
			jogo->Invaders[i].vidas = 1;
			jogo->Invaders[i].id_invader = i;
	}
	//jogo->Dados.nInvaders = 1;
	jogo->Dados.nivel = 1;
}

void ColocaInvaders() { //Esta funcao nao esta automatizada porque ainda não temos o jogo totalmente incializado
	int i;
	int espaco = 50;
	int flag = 1;
	int fila = 1;
	int comp = ComprimentoJanelaMAX;
	for (i = 0; i < jogo->Dados.nInvaders; i++) {

		if (flag == 1) {
			jogo->Invaders[i].area.x = espaco;
			if (fila == 1)
				jogo->Invaders[i].area.y = (fila * espaco);
			else
				jogo->Invaders[i].area.y = (fila * espaco) + (jogo->Invaders[i].area.altura *(fila - 1));
			flag = 0;
		}
		else
		{
			//jogo->Invaders[i].area.x = jogo->Invaders[i - 1].area.x + jogo->Invaders[i].area.comprimento + espaco;
			jogo->Invaders[i].area.x = jogo->Invaders[i - 1].area.x + espaco;
			if (fila == 1)
				jogo->Invaders[i].area.y = (fila * espaco);
			else
				jogo->Invaders[i].area.y = (fila * espaco) + (jogo->Invaders[i].area.altura *(fila - 1));

			if ((jogo->Invaders[i].area.x + jogo->Invaders[i].area.comprimento) >= (comp - espaco)) {
				fila++;
				flag = 1;
				i--;
			}
		}
	}
}

void IniciaTiros()
{


	int i = 0;

	jogo->Dados.nTiros = 0;

	for (i = 0; i < MaxnumTiros; i++)

	{
		jogo->Tiros[i].area.x = 0;
		jogo->Tiros[i].area.y = 0;
		jogo->Tiros[i].velocidade = 0;
		jogo->Tiros[i].id_tiros = -1;
		jogo->Tiros[i].id_dono = 0;
		jogo->Tiros[i].area.comprimento = 5;
		jogo->Tiros[i].area.altura = 10;
	}

}


void IniciaBombas()
{
	int i = 0;

	jogo->Dados.nBombas = 0;

	for (i = 0; i < MaxnumBombas; i++)
	{
		jogo->Bombas[i].area.x = 0;
		jogo->Bombas[i].area.y = 0;
		jogo->Bombas[i].area.comprimento = 5;
		jogo->Bombas[i].area.altura = 5;
		jogo->Bombas[i].velocidade = -1;
		jogo->Bombas[i].id_bombas = -1;
	}

}

void IniciaDenfenders()
{

	int i = 0;

	jogo->Dados.nDefenders = 0;

	for (i = 0; i < MaxClientes; i++)
	{

		jogo->Defenders[i].area.x = 0;
		jogo->Defenders[i].area.y = 0;
		jogo->Defenders[i].id_defender = -1;
		_tcscpy_s(jogo->Defenders[i].nome, sizeof(jogo->Defenders[i].nome), TEXT("nada"));
		jogo->Defenders[i].pontos = 0;
		jogo->Defenders[i].velocidade = 0;
		jogo->Defenders[i].vidas = 0;

	}

}

void IniciaPowerUp()
{
	jogo->Dados.nPowerUPs = 0;

	for (int i = 0; i < MaxPowerUP; i++)
	{
		jogo->PowerUP[i].area.altura = 30;
		jogo->PowerUP[i].area.comprimento = 30;
		jogo->PowerUP[i].area.x = 0;
		jogo->PowerUP[i].area.y = 0;
		jogo->PowerUP[i].duracao = jogo->Dados.duracaoPowerUp;
		jogo->PowerUP[i].id_powerUP = -1;
		jogo->PowerUP[i].velocidade = 0;
		jogo->PowerUP[i].tipo = OUTRO;
	}

}

void IniciaJogo()
{
	jogo->Dados.altura = 600;
	jogo->Dados.comprimento = 400;
	jogo->CicloDeVida = CRIACAO;
	IniciaInvaders();
	IniciaTiros();
	IniciaBombas();
	IniciaDenfenders();
	IniciaPowerUp();

}

void ColocaDefenders()
{
	
	for (int i = 0; i < jogo->Dados.nDefenders; i++) {
		jogo->Defenders[i].area.x = (i +1 )* 100;
		jogo->Defenders[i].area.y = 400;
		jogo->Defenders[i].area.altura = 40;
		jogo->Defenders[i].area.comprimento = 40;
	}

}

void GeraPowerup(int x, int y)
{
	int res;

	for (int i = 0; i < jogo->Dados.nPowerUPs; i++)
	{
		jogo->PowerUP[i].area.x = x;
		jogo->PowerUP[i].area.y = y;
		srand(time(NULL));
		res = rand() % (6 - 0) + 0;

		switch (res) {

		case 0:
		{
			jogo->PowerUP[i].tipo = ESCUDO;

		}
		break;
		case 1:
		{
			jogo->PowerUP[i].tipo = GELO;
		}
		break;
		case 2:
		{
			jogo->PowerUP[i].tipo = BATERIA;
		}
		break;
		case 3:
		{
			jogo->PowerUP[i].tipo = MAIS;
		}
		break;

		case 4:
		{
			jogo->PowerUP[i].tipo = VIDA;
		}
		break;

		case 5:
		{
			jogo->PowerUP[i].tipo = ALCOOL;
		}
		break;

		case 6:
		{
			jogo->PowerUP[i].tipo = OUTRO;
		}
		break;
		}
	}





}

void AtiraBomba() {
	int y_aux = 0;
	int res;
	srand(time(NULL));
	for (int i = 0; i < jogo->Dados.nInvaders; i++) {
		if (jogo->Invaders[i].id_invader != -1) {
			if (jogo->Invaders[i].area.y >= y_aux)
				y_aux = jogo->Invaders[i].area.y;
		}
	}
	for (int i = 0; i < jogo->Dados.nInvaders; i++) {
	//	if (jogo->Invaders[i].id_invader != -1 && jogo->Invaders[i].area.y == y_aux) {
		if ( jogo->Invaders[i].area.y == y_aux) {
			//res = rand() % (6 - 0) + 0;
			if (rand() % 100 > jogo->Dados.probabilidadeInvaderDisparar) {

				for (int j = 0; j < MaxnumBombas; j++) {
					if (jogo->Bombas[j].id_bombas == -1) {

						jogo->Bombas[j].id_bombas = j;
						jogo->Bombas[j].area.x = jogo->Invaders[i].area.x;
						jogo->Bombas[j].area.y = jogo->Invaders[i].area.y + 30;
						jogo->Dados.nBombas++;
						//jogo->Bombas[j].velocidade ...
						break;
					}
				}
			}
		}
	}

}

int MoveInvaders(int verifica_sentido)
{
	typedef struct _guardaMax
	{
		int id;
		int max;
		int min;
	} guarda;

	int i;

	guarda guardaMax;
	guarda guardaMin;

	guardaMax.max = 0;
	guardaMax.min = 0;
	guardaMax.id = -1;

	guardaMin.max = 500;
	guardaMin.min = 500;
	guardaMin.id = -1;

	//DIREITA
	if (verifica_sentido == 0)
	{
		for (i = 0; i < jogo->Dados.nInvaders; i++)
		{
			if (jogo->Invaders[i].id_invader != -1)
			{

				if ((jogo->Invaders[i].area.x + jogo->Invaders[i].area.comprimento) > guardaMax.max)  //verifica se a posicao do invader esta mais a direita
				{
					guardaMax.max = (jogo->Invaders[i].area.x + jogo->Invaders[i].area.comprimento);  //guarda posicao mais a direita
					guardaMax.id = i;
				}
			}
		}
	}
	else{
		if(verifica_sentido == 1){
			
				for (i = 0; i < jogo->Dados.nInvaders; i++)
				{
					if (jogo->Invaders[i].id_invader != -1)
					{

						if ((jogo->Invaders[i].area.x - jogo->Invaders[i].area.comprimento) < guardaMin.min)  //verifica se a posicao do invader esta mais a esquerda
						{
							guardaMin.min = (jogo->Invaders[i].area.x - jogo->Invaders[i].area.comprimento);  //guarda posicao mais a esquerda
							guardaMin.id = i;
						}
					}
				}

			}
	}
	if (verifica_sentido == 0) {
		if (jogo->Invaders[guardaMax.id].area.x + jogo->Invaders[guardaMax.id].area.comprimento <= ComprimentoJanelaMAX - 30)
		{
			
				for (i = 0; i < jogo->Dados.nInvaders; i++)
				{
					if (jogo->Invaders[i].id_invader != -1)
					{
					jogo->Invaders[i].area.x++;
				}
			}
		}
		else
		{
			for (i = 0; i < jogo->Dados.nInvaders; i++)
				{
					if (jogo->Invaders[i].id_invader != -1)
					{
					jogo->Invaders[i].area.y = jogo->Invaders[i].area.y + 10;

				}
			}
			sentido = 1;
		}
		
	}
	else {
		if (verifica_sentido == 1) {
			if (jogo->Invaders[guardaMin.id].area.x - jogo->Invaders[10].area.comprimento >= ComprimentoJanelaMIN)
			{

				for (i = 0; i < jogo->Dados.nInvaders; i++)
				{
					if (jogo->Invaders[i].id_invader != -1)
					{
						jogo->Invaders[i].area.x --;
					}
				}
			}
			else
			{

				for (i = 0; i < jogo->Dados.nInvaders; i++)
				{
					if (jogo->Invaders[i].id_invader != -1)
					{
						jogo->Invaders[i].area.y + 10;
					}
				}
				sentido = 0;
			}
		}
	}

	return sentido;
}

void TiroAntigueInvader(int id)
{

	for (int i = 0; i < jogo->Dados.nTiros; i++)
	{

		if (jogo->Tiros[id].area.x >= jogo->Invaders[i].area.x && jogo->Tiros[id].area.x <= (jogo->Invaders[i].area.x + jogo->Invaders[i].area.comprimento) &&
			jogo->Tiros[id].area.y <= jogo->Invaders[i].area.y && jogo->Tiros[id].area.y >= (jogo->Invaders[i].area.y + jogo->Invaders[i].area.altura))
		{
			jogo->Invaders[i].vidas = jogo->Invaders[i].vidas - 1;
			//ou morre logo
			//DetroiInvader
			if (jogo->Invaders[id].vidas < 0)
			{
				jogo->Invaders[id].id_invader = -1;
			}


		}

	}

}

void BombaAntigueDefender(int x, int y)
{

	for (int i = 0; i < jogo->Dados.nDefenders; i++)
	{

		if (jogo->Bombas[i].area.x >= jogo->Defenders[i].area.x && jogo->Bombas[i].area.x <= (jogo->Defenders[i].area.x + jogo->Defenders[i].area.comprimento) &&
			jogo->Bombas[i].area.y <= jogo->Defenders[i].area.y && jogo->Bombas[i].area.y >= (jogo->Defenders[i].area.y + jogo->Defenders[i].area.altura))
		{

			if (jogo->Defenders[i].powerUP.tipo == ESCUDO)
			{

			}
			else
			{
				jogo->Defenders[i].vidas = jogo->Defenders[i].vidas - 1; //retira vidas 
			}


			if (jogo->Defenders[i].vidas == 0)
			{
				jogo->Defenders[i].id_defender = -1;  //muda id para -1 se for = -1 não imprime no mapa
			}
		}

	}
}

void MoveBomba(int id)
{
	//Sleep(2000);
	//verificar se existe alguma coisa 
	/*
	for(int j = 0; j < jogo->Dados.nBombas; j++)
	{
		if (jogo->Bombas[id].area.x >= jogo->Defenders[j].area.x && jogo->Bombas[id].area.x <= (jogo->Defenders[j].area.x + jogo->Defenders[j].area.comprimento) &&
			jogo->Bombas[id].area.y <= jogo->Defenders[j].area.y && jogo->Bombas[id].area.y >= (jogo->Defenders[j].area.y + jogo->Defenders[j].area.altura))
		{
			if (jogo->Defenders[j].powerUP.tipo == ESCUDO)
				break;
			else {
				if (jogo->Defenders[j].id_defender != -1) {
					jogo->Defenders[j].vidas = jogo->Defenders[j].vidas - 1; //retira vidas 
					jogo->Bombas[id].id_bombas = -1;
					if (jogo->Defenders[j].vidas == 0)
						jogo->Defenders[j].id_defender = 1;
				}
			}
			break;
		}
	}

	if (jogo->Bombas[id].area.y > AlturaJanelaMAX)
	{
		//desaparece do mapa
		jogo->Bombas[id].id_bombas = -1;
	}
	else
		jogo->Bombas[id].area.y = jogo->Bombas[id].area.y + 20;
		*/
	int x;
	int y;
	x = jogo->Bombas[id].area.y;
	jogo->Bombas[id].area.y = jogo->Bombas[id].area.y + 5;
	y = jogo->Bombas[id].area.y;
	x = 0;
}

 void CriaPowerUp(int x, int y) {
	int res;
	srand(time(NULL));
	for (int i = 0; i < jogo->Dados.nPowerUPs; i++) {
		if (jogo->PowerUP[i].id_powerUP == -1) {
			jogo->PowerUP[i].id_powerUP = i;
			jogo->PowerUP[i].area.x = x ;
			jogo->PowerUP[i].area.y = y + 5;
			res = rand() % (6 - 0) + 0;
			switch (res) {
			case 0:
			{
				jogo->PowerUP[i].tipo = ESCUDO;

			}
			break;
			case 1:
			{
				jogo->PowerUP[i].tipo = GELO;
			}
			break;
			case 2:
			{
				jogo->PowerUP[i].tipo = BATERIA;
			}
			break;
			case 3:
			{
				jogo->PowerUP[i].tipo = MAIS;
			}
			break;

			case 4:
			{
				jogo->PowerUP[i].tipo = ALCOOL;
			}
			break;

			case 5:
			{
				jogo->PowerUP[i].tipo = VIDA;
			}
			break;
			}
		}
	}

}

void MoveTiro(int id) {
	srand(time(NULL));
	int x, y, z, k, l, m, i;
	x = jogo->Tiros[id].area.x;
	l = jogo->Tiros[id].area.y;
	i = jogo->Dados.nTiros;
	for (int j = 0; j < jogo->Dados.nTiros; j++)
	{
		
	
		y = jogo->Tiros[id].area.x;
		k = jogo->Invaders[j].area.comprimento;
	
		m = jogo->Invaders[j].area.y;
	
		if (jogo->Tiros[id].area.x >= jogo->Invaders[j].area.x && jogo->Tiros[id].area.x <= (jogo->Invaders[j].area.x + jogo->Invaders[j].area.comprimento) &&
			jogo->Tiros[id].area.y >= jogo->Invaders[j].area.y && jogo->Tiros[id].area.y <= (jogo->Invaders[j].area.y + jogo->Invaders[j].area.altura))
		{
			if (jogo->Invaders[j].id_invader != -1) {
				jogo->Invaders[j].vidas = jogo->Invaders[j].vidas - 1; //retira vidas 
				jogo->Tiros[id].id_tiros = -1;

				if (jogo->Invaders[j].vidas == 0)
				{
					jogo->Invaders[j].id_invader = -1;
					jogo->Defenders[jogo->Tiros[id].id_dono].pontos ++;
					if (rand() % 100 > jogo->Dados.probabilidadePowerUp) {
						jogo->Dados.nPowerUPs++;
						CriaPowerUp(jogo->Invaders[j].area.x, jogo->Invaders[j].area.y);
					}
				}
			}
			break;
		}
	}

	if (jogo->Tiros[id].area.y < AlturaJanelaMIN + 30)
	{
		//desaparece do mapa
		jogo->Tiros[id].id_tiros = -1;
	}
	else 
		jogo->Tiros[id].area.y = jogo->Tiros[id].area.y - 5;
}

void MovePowerUp(int id)
{
	/*
	for (int j = 0; j < jogo->Dados.nDefenders; j++)
	{

		if (jogo->PowerUP[id].area.x >= jogo->Defenders[j].area.x && jogo->PowerUP[id].area.x <= (jogo->Defenders[j].area.x + jogo->Defenders[j].area.comprimento) &&
			jogo->PowerUP[id].area.y <= jogo->Defenders[j].area.y && jogo->PowerUP[id].area.y >= (jogo->Defenders[j].area.y + jogo->Defenders[j].area.altura))
		{

			jogo->Defenders[j].powerUP.tipo = jogo->PowerUP[id].tipo;
			jogo->Defenders[j].powerUP.duracao = jogo->PowerUP[id].duracao; //coloca propriedades do powerup no defender
			jogo->PowerUP[id].id_powerUP = -1;


		}

		if (jogo->PowerUP[id].area.y > AlturaJanelaMAX)
		{
			jogo->PowerUP[id].id_powerUP = -1;  //coloca o id a -1 para desparecer do mapa
		}

		else
		{
			jogo->PowerUP[id].area.y++;
		}
	}
	*/

	for (int j = 0; j < jogo->Dados.nDefenders; j++)
	{
		if (jogo->PowerUP[id].area.x >= jogo->Defenders[j].area.x && jogo->PowerUP[id].area.x <= (jogo->Defenders[j].area.x + jogo->Defenders[j].area.comprimento) &&
			jogo->PowerUP[id].area.y >= jogo->Defenders[j].area.y && jogo->PowerUP[id].area.y <= (jogo->Defenders[j].area.y + jogo->Defenders[j].area.altura))
		{
			if (jogo->Defenders[j].id_defender != -1) {
				
				jogo->PowerUP[id].id_powerUP= -1;
				jogo->Defenders[j].powerUP.tipo = jogo->PowerUP[id].tipo;
				jogo->Defenders[j].powerUP.duracao = 30;
				
			}
			break;
		}
	}

	if (jogo->PowerUP[id].area.y < AlturaJanelaMAX - 100)
	{
		//desaparece do mapa
	
		jogo->PowerUP[id].area.y = jogo->PowerUP[id].area.y + 5;
	}
	else
		jogo->PowerUP[id].id_powerUP = -1;
}

void MoveDefender(int id)
{
	int z = jogo->Defenders[id].proxima_jogada;

	int limY = (int)(500 * 0.8);

	switch (jogo->Defenders[id].proxima_jogada)
	{
	case ESQUERDA:
	{
		if (jogo->Defenders[id].area.x - movimento > ComprimentoJanelaMIN)
		{
			jogo->Defenders[id].area.x = jogo->Defenders[id].area.x - movimento;
		}

	}
	break;
	case DIREITA:
	{
		if (jogo->Defenders[id].area.x + jogo->Defenders[id].area.comprimento + movimento <= ComprimentoJanelaMAX - 30)
		{
			jogo->Defenders[id].area.x = jogo->Defenders[id].area.x + movimento;
		}

	}
	break;
	case CIMA:
	{
		if (jogo->Defenders[id].area.y + movimento >= (AlturaJanelaMAX*0.8))
		{
			jogo->Defenders[id].area.y = jogo->Defenders[id].area.y - movimento;
		}
	}

	break;
	case BAIXO:
	{

		if (jogo->Defenders[id].area.y + jogo->Defenders[id].area.altura - movimento <= (AlturaJanelaMAX*0.8))
		{
			jogo->Defenders[id].area.y = jogo->Defenders[id].area.y + movimento;
		}

	}
	break;

	case ESPAÇO:
	{
		for (int j = 0; j < MaxnumTiros; j++)
		{
			if (jogo->Tiros[j].id_tiros == -1)
			{
				jogo->Tiros[j].area.x = jogo->Defenders[id].area.x + 17;
				jogo->Tiros[j].area.y = jogo->Defenders[id].area.y - 20;
				jogo->Tiros[j].id_tiros = j;
				jogo->Tiros[j].id_dono = jogo->Defenders->id_defender;
				jogo->Dados.nTiros++;
				break;
			}

		}
	}
	break;
	default:
		jogo->Defenders[id].proxima_jogada = NULA;
		break;
	}

	jogo->Defenders[id].proxima_jogada = NULA;
}

void esperaThreads()
{

	WaitForSingleObject(hThreadBombas, INFINITE);
	WaitForSingleObject(hThreadPowerups, INFINITE);
	WaitForSingleObject(hThreadTiros, INFINITE);
	WaitForSingleObject(hThreadJogadores, INFINITE);

}

