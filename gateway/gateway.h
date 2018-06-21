#pragma once

#include <windows.h>
#include <tchar.h>
#include <io.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>  
#include <stdlib.h> 
#include "../DlltpSO2/dll.h"



#define _CRT_SECURE_NO_WARNINGS

HANDLE hThreadEscritor;

DWORD WINAPI arrancaComunicacaoGateway();
DWORD WINAPI  EnviaUpdateCliente();
DWORD WINAPI RecebeMensagensClientes(LPVOID param);

MsgCliGat EnviaUpdate(int estado);