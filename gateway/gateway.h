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

DWORD WINAPI ThreadProdutor(LPVOID param); //LADO DO GATEWAY



