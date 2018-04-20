#pragma once
#include <windows.h>
#include <tchar.h>
#include <io.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>  
#include <stdlib.h> 
#define _CRT_SECURE_NO_WARNINGS
#define MAX 50

/*
//Mecanismos de sincronização para o buffer
HANDLE SemLer, SemEscrever, MutexLer, MutexEscrever;

//Lado do Getaway
SemEscrever = CreateSemaphore(NULL, 0, MAX, TEXT("SemEscrever"));
MutexEscrever = CreateMutex(NULL, FALSE, TEXT("MutexEscrever"));

//Lado do Servidor
SemLer = CreateSemaphore(NULL, MAX, MAX, TEXT("SemLeR " ));
MutexLer = CreateSemaphore(NULL, FALSE, TEXT("MutexLer"));

*/

typedef struct Area {
	int x, y;
	int comprimento, altura;
} Area;


typedef struct Jogo {

}Jogo;

typedef struct Invader {

} Invader;

typedef struct Defender {

} Defender;

typedef struct Bomb {

} Bomb;

typedef struct Shot {

} Shot;

typedef struct PowerUP {

} PowerUP;

//Estrutura para defenir dimensões do "mapa"
typedef struct Window {

} Window;

//Mensagem que vai do Cliente para o Gateway
typedef struct MsgCLI {

} MsgCLI;

//Mensagem que vai do Gateway para o cliente
typedef struct MsgSER {

} MsgSER;
