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
static TCHAR szTitle[] = _T("Win32 Guided Tour Application");

// Forward declarations of functions included in this code module:  
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

typedef struct _cenas {
	TCHAR yo[254];
} cenas;

cenas cenas123;

HINSTANCE hInst;

HWND janela;

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
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1_Servidor);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

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

	IniciaDefenders();


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
		1500, 800,
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
			if (jogo->CicloDeVida == CRIACAO) {
				RecebeConfiguracao(hWnd);
				jogo->CicloDeVida = ASSOCIACAO;
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

		//aux = jogo->nDefenders + 1;
		//tcscpy_s(aux, _countof(aux),jogo->nDefenders);
		//sprintf(aux, "%d", jogo->nDefenders + 1);
	//	_tcscpy_s(aux, _countof(aux), jogo->nDefenders + 1 );

		TCHAR text[100];

		_itot_s(jogo->nDefenders + 1, text, 100, 10);

		SendMessage(GetDlgItem(hWnd, IDC_STATIC_Valor), WM_SETTEXT, 0, (LPARAM)text);
	}
	break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
		{
			if (jogo->CicloDeVida == ASSOCIACAO) {
				jogo->CicloDeVida = INICIO_CICLO_JOGO;
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
		case ID_NOVOJOGO_CONFIGURAR:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1_configurar), hWnd, DialogConfigurar);
			break;
		case ID_NOVOJOGO_INCIAR:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1_Iniciar), hWnd, DialogIniciar);
			break;
		case ID_NOVOJOGO_SAIR:

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

void RecebeConfiguracao(HWND hWnd) {
	TCHAR aux[254];

	GetWindowText(GetDlgItem(hWnd, IDC_EDIT1_nInvaders), aux, sizeof(aux)); //jogo->nInvaders
	jogo->nInvaders = atoi(aux);
	GetWindowText(GetDlgItem(hWnd, IDC_EDIT2_velocidadeInvader), aux, sizeof(aux)); // Sleep para o movimento dos Invaders 
	jogo->velocidadeInvaders = atoi(aux);
	GetWindowText(GetDlgItem(hWnd, IDC_EDIT3_ProbabilidadeInvader), aux, sizeof(aux)); // Probabilidade para verificar se Invaders Disparam ou nao
	jogo->probabilidadeInvaderDisparar = atoi(aux);
	GetWindowText(GetDlgItem(hWnd, IDC_EDIT4_VelocidadePowerUP), aux, sizeof(aux)); // Sleep para o movimento dos PowerUps 
	jogo->velocidadePowerUps = atoi(aux);
	GetWindowText(GetDlgItem(hWnd, IDC_EDIT5_DuracaoPowerUP), aux, sizeof(aux)); // Probabilidade verificacao dos PowerUps
	jogo->probabilidadePowerUp = atoi(aux);
	GetWindowText(GetDlgItem(hWnd, IDC_EDIT6_VelocidadeTiroInvader), aux, sizeof(aux)); // Sleep de mover bombas
	jogo->velocidadeBomba = atoi(aux);
	GetWindowText(GetDlgItem(hWnd, IDC_EDIT7_NumeroDeVidas), aux, sizeof(aux));
	jogo->nVidas = atoi(aux);

}
/*

int _tmain(int argc, LPTSTR argv[]) {


	DWORD threadId;
	Input Input;

	TCHAR command[80];

#ifdef UNICODE
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);

#endif

	jogo->CicloDeVida = CRIACAO; //este vai ser para o botao de configuraçao
	jogo->CicloDeVida = ASSOCIACAO;

	continua = 1;
	hMutexJogo = CreateMutex(NULL, TRUE, TEXT("GoMutex"));
	hEvento = CreateEvent(NULL, TRUE, FALSE, TEXT("GoEvent"));

	IniciaDefenders();


	hThreadLeitor = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadConsumidor, NULL, 0, &threadId);
	if (hThreadLeitor != NULL)
		_tprintf(TEXT("Lancei uma thread com id %d\n"), threadId);
	else {
		_tprintf(TEXT("Erro ao criar Thread Escritor\n"));
		return -1;
	}

	_tprintf(TEXT("Command:"));
	//_tscanf_s(TEXT(" %s"), command, 80);
	_fgetts(command, 80, stdin);
	command[_tcslen(command) - 1] = '\0';



	//Isto futuramente vai estar dentro de um ciclo de jogo
	//Ter em anteção os niveis na criação desse mesmo ciclo

	Input = RecebeInput();


	_tprintf(TEXT("Numero Esquivos: %d \n"), Input.numInvadersEsquivo);

	_tprintf(TEXT("Numero Extras: %d \n"), Input.numInvadersOutros);

	_tprintf(TEXT("Numero Base: %d \n"), Input.numInvadersBase);

	IniciaInvaders(Input); //Inicializa valores dos invaders
	ColocaInvaders(Input);		//Coloca invaders nas posições correctas
/*
	WaitForSingleObject(hMutexJogo, INFINITE);

	jogo->altura = 2345;
	_tprintf(TEXT("[Event Generated] \n"));

	SetEvent(hEvento);
	ResetEvent(hEvento);
	//Sleep(3*1000);
	ReleaseMutex(hMutexJogo);


	for (int j = 0; j < Input.numInvaders; j++) {
		_tprintf(TEXT("Invader %d: x - %d    y - %d \n"), j, jogo->Invaders[j].area.x, jogo->Invaders[j].area.y);
	}

	CreateThreadsInvaders(); //Cria Thread por tipo de Invader


	WaitForSingleObject(hThreadLeitor, INFINITE);
	_tprintf(TEXT("[Thread Principal %d]Finalmente vou terminar..."), GetCurrentThreadId());


	//AcabaSinc();
	return 0;
}
*/
int CreateThreadsInvaders() {

	hThreadInvadersBase = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadInvadersBase, 0, 0, NULL);
	hThreadInvadersEsquivos = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadInvadersEsquivo, 0, 0, NULL);
	hThreadsInvadersExtras = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadInvadersExtra, 0, 0, NULL);

	if (hThreadInvadersBase == NULL || hThreadInvadersEsquivos == NULL || hThreadsInvadersExtras == NULL) {
		_tprintf(TEXT("[Erro]Criação de objectos do Windows(%d)\n"), GetLastError());
		return -1;
	}

	return 0;

}

DWORD WINAPI ThreadInvadersBase() {
	_tprintf(TEXT("Iniciei Thread Tipo Invaders Base \n"));

	while (continua == 1) {

		MoveInvaderBase(jogo->Invaders[10].id_invader, jogo->Invaders[10].area.x, jogo->Invaders[10].area.y, MaxInvaders);
		_tprintf(TEXT("%d %d %d \n"), jogo->Invaders[10].id_invader, jogo->Invaders[10].area.x, jogo->Invaders[10].area.y);

		Sleep(velocidadeInvaderBase);
	}
	return 0;
}

DWORD WINAPI ThreadInvadersEsquivo() {
	_tprintf(TEXT("Iniciei Thread Tipo Invaders Esquivo \n"));
	/*
	int i;
	do
	int contador=0
	for (i = 0; i < inp.numInvaders; i++) {
		if (jogo->Invaders[i].tipo == ESQUIVO) {
			//se tiver espaço
			//movimentoInvadersEsquivo();

			//calcular probabilidade para disparar
			//se sim CreatheThreadDisparo();


		}
		Sleep(jogo->Invaders[i]);
	}
	while (contador != inp.numInvadersEsquivos)

	*/
	return 0;
}

DWORD WINAPI ThreadInvadersExtra() {
	_tprintf(TEXT("Iniciei Thread Tipo Invaders Extra \n"));
	/*
	int i;
	do
	int contador=0
	for (i = 0; i < inp.numInvaders; i++) {
	if (jogo->Invaders[i].tipo == Extra) {
	//se tiver espaço
	//movimentoInvadersEsquivo();

	//calcular probabilidade para disparar
	//se sim CreatheThreadDisparo();


	}
	Sleep(jogo->Invaders[i]);
	}
	while (contador != inp.numInvadersExtra)

	*/
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
	while (continua == 1) { //Isto é para passar para a DLL

		TrataMensagem();
		//	WaitForSingleObject(hMutexJogo, INFINITE);
		if (jogo->CicloDeVida == ASSOCIACAO) {
			int i;
			for (i = 0; i < MaxClientes; i++) {
				if (jogo->Defenders[i].id_defender == -1) {
					jogo->Defenders[i].id_defender == i;
					jogo->nDefenders++;
					PostMessage(janela, SinalUser, 0, 0);
				}
				break;
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
	jogo->nDefenders = -1;
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

	}

}

void IniciaInvaders(Input inp) {
	_tprintf(TEXT("%d"), inp.numInvaders);
	int i = 0;
	for (i = 0; i < inp.numInvaders; i++) {
		if (i < inp.numInvadersBase) {
			jogo->Invaders[i].area.x = 0;
			jogo->Invaders[i].area.y = 0;
			jogo->Invaders[i].area.altura = AlturaInvader;
			jogo->Invaders[i].area.comprimento = ComprimentoInvader;
			jogo->Invaders[i].tipo = BASICO;
			jogo->Invaders[i].velocidade = velocidadeInvaderBase;
			jogo->Invaders[i].vidas = 1;
			jogo->Invaders[i].id_invader = i;
		}
		else
			if (i < inp.numInvadersBase + inp.numInvadersEsquivo) {
				jogo->Invaders[i].area.x = 0;
				jogo->Invaders[i].area.y = 0;
				jogo->Invaders[i].area.altura = AlturaInvader;
				jogo->Invaders[i].area.comprimento = ComprimentoInvader;
				jogo->Invaders[i].tipo = ESQUIVO;
				jogo->Invaders[i].velocidade = (int)(velocidadeInvaderBase * 0.40);
				jogo->Invaders[i].vidas = 3;
				jogo->Invaders[i].id_invader = i;
			}
			else
			{
				jogo->Invaders[i].area.x = 0;
				jogo->Invaders[i].area.y = 0;
				jogo->Invaders[i].area.altura = 0;
				jogo->Invaders[i].area.comprimento = 0;
				jogo->Invaders[i].tipo = EXTRA;
				jogo->Invaders[i].velocidade = 0;
				jogo->Invaders[i].vidas = 0;
				jogo->Invaders[i].id_invader = i;
			}
	}
}

void ColocaInvaders(Input inp) { //Esta funcao nao esta automatizada porque ainda não temos o jogo totalmente incializado
	int i;
	int espaco = 2;
	int flag = 1;
	int fila = 1;
	int comp = ComprimentoJanelaMAX;
	for (i = 0; i < inp.numInvaders; i++) {

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
			jogo->Invaders[i].area.x = jogo->Invaders[i - 1].area.x + jogo->Invaders[i].area.comprimento + espaco;
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

