#pragma once
//Estes defines tem de estar na dll porque v�o ser utilizados para a memoria partilhada jogo

#include "../DlltpSO2/dll.h"
DWORD WINAPI arrancaComunicacaoCliente();
HANDLE hThread;