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
	hMutexJogo = CreateMutex(NULL, TRUE, TEXT("GoMutex"));
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
LRESULT CALLBACK DialogConfigurar(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	// DIALOGO DE CONFIGURAÇÃO DE PARAMETROS DE JOGO
	switch (message)
	{
	case WM_INITDIALOG:
	{
		TCHAR text[100];
		_itot_s(10, text, 100, 10);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT1_nInvaders), WM_SETTEXT, 0, (LPARAM)text);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT2_velocidadeInvader), WM_SETTEXT, 0, (LPARAM)text);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT3_ProbabilidadeInvader), WM_SETTEXT, 0, (LPARAM)text);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT4_VelocidadePowerUP), WM_SETTEXT, 0, (LPARAM)text);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT5_DuracaoPowerUP), WM_SETTEXT, 0, (LPARAM)text);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT6_VelocidadeTiroInvader), WM_SETTEXT, 0, (LPARAM)text);
		SendMessage(GetDlgItem(hWnd, IDC_EDIT7_NumeroDeVidas), WM_SETTEXT, 0, (LPARAM)text);

	}

	break;
	case WM_COMMAND:
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
				WaitForSingleObject(hMutexJogo,INFINITE);
				ColocaDefenders();
				//CreateThreadsElementosJogo();
				hThreadJogadores = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadJogadores, 0, 0, NULL);
				hThreadTiros = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadTiros, 0, 0, NULL);
				hThreadInvadersBase = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadInvadersBase, 0, 0, NULL);

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

void RecebeConfiguracao(HWND hWnd) {
	TCHAR aux[254];

	GetWindowText(GetDlgItem(hWnd, IDC_EDIT1_nInvaders), aux, sizeof(aux)); //jogo->nInvaders
	jogo->Dados.nInvaders = atoi(aux);
	GetWindowText(GetDlgItem(hWnd, IDC_EDIT2_velocidadeInvader), aux, sizeof(aux)); // Sleep para o movimento dos Invaders 
	jogo->Dados.velocidadeInvaders = atoi(aux);
	GetWindowText(GetDlgItem(hWnd, IDC_EDIT3_ProbabilidadeInvader), aux, sizeof(aux)); // Probabilidade para verificar se Invaders Disparam ou nao
	jogo->Dados.probabilidadeInvaderDisparar = atoi(aux);
	GetWindowText(GetDlgItem(hWnd, IDC_EDIT4_VelocidadePowerUP), aux, sizeof(aux)); // Sleep para o movimento dos PowerUps 
	jogo->Dados.velocidadePowerUps = atoi(aux);
	GetWindowText(GetDlgItem(hWnd, IDC_EDIT5_DuracaoPowerUP), aux, sizeof(aux)); // Probabilidade verificacao dos PowerUps
	jogo->Dados.probabilidadePowerUp = atoi(aux);
	GetWindowText(GetDlgItem(hWnd, IDC_EDIT6_VelocidadeTiroInvader), aux, sizeof(aux)); // Sleep de mover bombas
	jogo->Dados.velocidadeBomba = atoi(aux);
	GetWindowText(GetDlgItem(hWnd, IDC_EDIT7_NumeroDeVidas), aux, sizeof(aux));
	jogo->Dados.nVidas = atoi(aux);

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
	_tprintf(TEXT("Iniciei Thread Invaders \n"));

	while (jogo->CicloDeVida == DECORRER) {

		for (int i = 0; i < jogo->Dados.nInvaders; i++)
		{
			if (jogo->Invaders[i].id_invader != -1)
			{
			//	jogo->Invaders[i].area.x =	jogo->Invaders[i].area.x + 5;
				//sentido = MoveInvaders(sentido);
			}

		}

	//	SetEvent(hEvento);
	//	ResetEvent(hEvento);
	//	ReleaseMutex(hMutexJogo);
	//	Sleep(jogo->Dados.velocidadeInvaders);
		Sleep(4000);
	}
	return 0;
}

DWORD WINAPI ThreadPowerups()
{
	_tprintf(TEXT("Iniciei Thread PowerUp \n"));

	while (jogo->CicloDeVida == DECORRER) {

		for (int i = 0; i < jogo->Dados.nPowerUPs; i++)
		{
			if (jogo->PowerUP[i].id_powerUP != -1)
			{
				MovePowerUp(jogo->PowerUP[i].id_powerUP);
			}

		}

		Sleep(jogo->Dados.velocidadePowerUps);
	}
	return 0;
}

DWORD WINAPI ThreadBombas() {  //adaptar com a estrutura bomba...
	_tprintf(TEXT("Iniciei Thread bomba \n"));

	while (jogo->CicloDeVida == DECORRER) {

		for (int i = 0; i < MaxnumBombas; i++)
		{
			if (jogo->Bombas[i].id_bombas != -1)
			{
				//funcao para mover as bombas
				MoveBomba(jogo->Bombas[i].id_bombas);

			}

		}

		Sleep(jogo->Dados.velocidadeBomba);
	}
	return 0;
}

DWORD WINAPI ThreadTiros()
{
	_tprintf(TEXT("Iniciei Thread Tiro \n"));

	while (jogo->CicloDeVida == DECORRER) {


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
		//Sleep(jogo->Dados.velocidadeTiro);
		Sleep(100);
	}
	return 0;
}

DWORD WINAPI ThreadJogadores()
{
	while (jogo->CicloDeVida == DECORRER) {

		//funcao para colocarinvaders
		//WaitForSingleObject(hMutexJogo, INFINITE);

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
		Sleep(1000);
		//Sleep(jogo->Dados.velocidadeDefenders);
	}
	return 0;
}

Input RecebeInput() {
	Input aux;
	//Input dos valores para o Jogo
	_tprintf(TEXT("Introduza o número de Invaders: \n"));
	//scanf("%d", &numInvaders);
	_tscanf_s(TEXT("%d"), &aux.numInvaders);

	if (aux.numInvaders <= 20) {
		aux.numInvadersEsquivo = 2;
		aux.numInvadersOutros = 0;
		aux.numInvadersBase = aux.numInvaders - (aux.numInvadersEsquivo + aux.numInvadersOutros);
	}
	else
		if (aux.numInvaders <= 40) {
			aux.numInvadersEsquivo = 10;
			aux.numInvadersOutros = 0;
			aux.numInvadersBase = aux.numInvaders - (aux.numInvadersEsquivo + aux.numInvadersOutros);
		}
		else
		{
			aux.numInvadersEsquivo = 20;
			aux.numInvadersOutros = 0;
			aux.numInvadersBase = aux.numInvaders - (aux.numInvadersEsquivo + aux.numInvadersOutros);
		}

	return aux;
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
		jogo->Defenders[i].vidas = 0;
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
	jogo->Dados.nInvaders = 18;
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
		jogo->Bombas[i].x = 0;
		jogo->Bombas[i].y = 0;
		jogo->Bombas[i].velocidade = -1;
		jogo->Bombas[i].id_bombas = 0;
		jogo->Bombas[i].id_dono = 0;
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
		jogo->PowerUP[i].duracao = 0;
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
	int xmin, xmax, ymax;
	int difx = ComprimentoJanelaMAX / MaxClientes;  //tem de ser o numero de clientes
	int ymin = (int)(AlturaJanelaMAX * 0.8);
	int px, py;
	

	for (int i = 0; i < jogo->Dados.nDefenders; i++)     //MaxCliente --> nClientes
	{
		xmin = difx * i;
		xmax = (difx *i) + (difx - jogo->Defenders[i].area.comprimento);

		ymax = AlturaJanelaMAX - jogo->Defenders[i].area.altura;

		px = rand() % (xmax - xmin) + xmin;
		py = rand() % (ymax - ymin) + ymin;

		if (i == 0) {
			jogo->Defenders[i].area.x = 50;
			jogo->Defenders[i].area.y = 350;
			jogo->Defenders[i].area.altura = 40;
			jogo->Defenders[i].area.comprimento = 40;
		}
		if (i == 1) {
			jogo->Defenders[i].area.x = 250;
			jogo->Defenders[i].area.y = 350;
			jogo->Defenders[i].area.altura = 40;
			jogo->Defenders[i].area.comprimento = 40;
		}

	}

}

void GeraPowerup(int x, int y)
{
	int res;

	for (int i = 0; i < jogo->Dados.nPowerUPs; i++)
	{
		jogo->PowerUP[i].area.x = x;
		jogo->PowerUP[i].area.y = y;

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

int MoveInvaders(int verifica_sentido)
{
	struct guardaMax
	{
		int id;
		int max;
		int min;
	}guardaMax;

	int i;
	guardaMax.max = 0;
	guardaMax.min = 0;
	guardaMax.id = -1;

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
	else
	{
		for (i = 0; i < jogo->Dados.nInvaders; i++)
		{
			if (jogo->Invaders[i].id_invader != -1)
			{

				if ((jogo->Invaders[i].area.x - jogo->Invaders[i].area.comprimento) > guardaMax.min)  //verifica se a posicao do invader esta mais a esquerda
				{
					guardaMax.min = (jogo->Invaders[i].area.x - jogo->Invaders[i].area.comprimento);  //guarda posicao mais a esquerda
					guardaMax.id = i;
				}
			}
		}


	}
	if (verifica_sentido == 0) {
		if (jogo->Invaders[guardaMax.id].area.x + jogo->Invaders[guardaMax.id].area.comprimento < ComprimentoJanelaMAX)
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
					jogo->Invaders[i].area.y = jogo->Invaders[i].area.y + 5;

				}
			}
			verifica_sentido = 1;
		}
	}
	else
	{
		//if (jogo->Invaders[guardaMax.id].area.x + jogo->Invaders[guardaMax.id].area.comprimento < ComprimentoJanelaMIN)
		if (jogo->Invaders[guardaMax.id].area.x - jogo->Invaders[10].area.comprimento < ComprimentoJanelaMIN)
		{
			
				for (i = 0; i < jogo->Dados.nInvaders; i++)
				{
					if (jogo->Invaders[i].id_invader != -1)
					{
					jogo->Invaders[i].area.x--;
				}
			}
		}
		else
		{
			
				for (i = 0; i < jogo->Dados.nInvaders; i++)
				{
					if (jogo->Invaders[i].id_invader != -1)
					{
					jogo->Invaders[i].area.y++;
				}
			}
			verifica_sentido = 1;
		}
	}
	return verifica_sentido;
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

		if (jogo->Bombas[i].x >= jogo->Defenders[i].area.x && jogo->Bombas[i].x <= (jogo->Defenders[i].area.x + jogo->Defenders[i].area.comprimento) &&
			jogo->Bombas[i].y <= jogo->Defenders[i].area.y && jogo->Bombas[i].y >= (jogo->Defenders[i].area.y + jogo->Defenders[i].area.altura))
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
	//verificar se existe alguma coisa 

	for (int j = 0; j < jogo->Dados.nBombas; j++)
	{

		if (jogo->Bombas[id].x >= jogo->Defenders[j].area.x && jogo->Bombas[id].x <= (jogo->Defenders[j].area.x + jogo->Defenders[j].area.comprimento) &&
			jogo->Bombas[id].y <= jogo->Defenders[j].area.y && jogo->Bombas[id].y >= (jogo->Defenders[j].area.y + jogo->Defenders[j].area.altura))  //verifica se tem algum elemento na sua posicao
		{

			if (jogo->Defenders[j].powerUP.tipo == ESCUDO)
			{
				break;
			}
			else
			{
				jogo->Defenders[j].vidas = jogo->Defenders[j].vidas - 1; //retira vidas 
			}


			if (jogo->Defenders[j].vidas == 0)
			{

				jogo->Defenders[j].id_defender = -1;
			}

			if (jogo->Bombas[id].y > AlturaJanelaMAX)  //verifica se chegou ao fim do mapa
			{
				jogo->Bombas[id].id_bombas = -1;
			}


		}
		else
		{
			jogo->Bombas[id].y++;

		}

	}


}

 void CriaPowerUp(int x, int y) {
	int res;
	for (int i = 0; i < jogo->Dados.nPowerUPs; i++) {
		if (jogo->PowerUP[i].id_powerUP == -1) {
			jogo->PowerUP[i].id_powerUP = i;
			jogo->PowerUP[i].area.x = x ;
			jogo->PowerUP[i].area.y = y + 5;
			res = rand() % (5 - 0) + 0;
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
			}
		}
	}

}

void MoveTiro(int id) {

	for (int j = 0; j < jogo->Dados.nInvaders; j++)
	{
		int z = jogo->Tiros[id].area.x;
		int y = jogo->Invaders[j].area.x;
		int a = jogo->Invaders[j].area.x + jogo->Invaders[j].area.comprimento;
		int b = jogo->Tiros[id].area.y;
		int c = jogo->Invaders[j].area.y;
		int d = jogo->Invaders[j].area.y + jogo->Invaders[j].area.altura;

		if (jogo->Tiros[id].area.x >= jogo->Invaders[j].area.x && jogo->Tiros[id].area.x <= (jogo->Invaders[j].area.x + jogo->Invaders[j].area.comprimento) &&
			jogo->Tiros[id].area.y >= jogo->Invaders[j].area.y && jogo->Tiros[id].area.y <= (jogo->Invaders[j].area.y + jogo->Invaders[j].area.altura))
		{
			if (jogo->Invaders[j].id_invader != -1) {
				jogo->Invaders[j].vidas = jogo->Invaders[j].vidas - 1; //retira vidas 
				jogo->Tiros[id].id_tiros = -1;

				if (jogo->Invaders[j].vidas == 0)
				{
					//chamar funcao para destroir nave
					//destroiInvaders(&jogo->Invaders[j], jogo->Dados.nBombas);
					jogo->Invaders[j].id_invader = -1;
					jogo->Defenders[jogo->Tiros[id].id_dono].pontos ++;
					jogo->Dados.nPowerUPs++;
					CriaPowerUp(jogo->Invaders[j].area.x, jogo->Invaders[j].area.y);
				}
			}
			break;
		}
	}

	if (jogo->Tiros[id].area.y > AlturaJanelaMIN)
	{
		//desaparece do mapa
		jogo->Tiros[id].id_tiros = -1;
	}
	else 
		jogo->Tiros[id].area.y = jogo->Tiros[id].area.y - 5;
}

void MovePowerUp(int id)
{

	for (int j = 0; j < jogo->Dados.nDefenders; j++)
	{

		if (jogo->PowerUP[id].area.x >= jogo->Defenders[j].area.x && jogo->PowerUP[id].area.x <= (jogo->Defenders[j].area.x + jogo->Defenders[j].area.comprimento) &&
			jogo->PowerUP[id].area.y <= jogo->Defenders[j].area.y && jogo->PowerUP[id].area.y >= (jogo->Defenders[j].area.y + jogo->Defenders[j].area.altura))
		{

			jogo->Defenders[j].powerUP.tipo = jogo->PowerUP[id].tipo;
			jogo->Defenders[j].powerUP.duracao = jogo->PowerUP[id].duracao; //coloca propriedades do powerup no defender

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

}

void MoveDefender(int id)
{
	int z = jogo->Defenders[id].proxima_jogada;

	int limY = (int)(AlturaJanelaMAX *0.8);

	switch (jogo->Defenders[id].proxima_jogada)
	{
	case ESQUERDA:
	{
	
			jogo->Defenders[id].area.x = jogo->Defenders[id].area.x - 20;

	}
		break;
	case DIREITA:
	{
		if (jogo->Defenders[id].area.x > ComprimentoJanelaMAX)
		{
			jogo->Defenders[id].area.x = jogo->Defenders[id].area.x;
		}
		else
		{
			
			jogo->Defenders[id].area.x  = jogo->Defenders[id].area.x + 20;

		}
	}
		break;
	case CIMA:
	{
		if (jogo->Defenders[id].area.y > limY)
		{
			jogo->Defenders[id].area.y = jogo->Defenders[id].area.y;
		}
		else
		{
			jogo->Defenders[id].area.y = jogo->Defenders[id].area.y - 20;
		}
	}
		break;
	case BAIXO:
	{
			jogo->Defenders[id].area.y = jogo->Defenders[id].area.y + 20;

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

