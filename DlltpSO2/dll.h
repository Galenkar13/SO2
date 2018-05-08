#include <windows.h>
#include <tchar.h>
//Definir uma constante para facilitar a leitura do prot�tipo da fun��o
//Este .h deve ser inclu�do no projeto que o vai usar (modo impl�cito)
#define TAM 256
//Esta macro � definida pelo sistema caso estejamos na DLL (<DLL_IMP>_EXPORTS definida)
//ou na app (<DLL_IMP>_EXPORTS n�o definida) onde DLL_IMP � o nome deste projeto

//


#ifdef DlltpSO2_EXPORTS 
#define DlltpSO2 __declspec(dllexport)
#else
#define DlltpSO2 __declspec(dllimport)
#endif

#ifdef __cpluplus

extern "C" {
#endif
	//Vari�vel global da DLL
	extern DlltpSO2 int nDLL;	
	//Fun��es a serem exportadas/importadas
	DlltpSO2 int UmaString(void);

#ifdef __cplusplus
}
#endif
