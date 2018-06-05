#pragma once
//Estes defines tem de estar na dll porque vão ser utilizados para a memoria partilhada jogo

#include "../DlltpSO2/dll.h"
DWORD WINAPI arrancaComunicacaoCliente();
HANDLE hThread;