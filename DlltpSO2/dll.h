#include <windows.h>
#include <tchar.h>
//Definir uma constante para facilitar a leitura do protótipo da função
//Este .h deve ser incluído no projeto que o vai usar (modo implícito)
#define TAM 256
//Esta macro é definida pelo sistema caso estejamos na DLL (<DLL_IMP>_EXPORTS definida)
//ou na app (<DLL_IMP>_EXPORTS não definida) onde DLL_IMP é o nome deste projeto

//


#ifdef DlltpSO2_EXPORTS 
#define DlltpSO2 __declspec(dllexport)
#else
#define DlltpSO2 __declspec(dllimport)
#endif

#ifdef __cpluplus

extern "C" {
#endif
	//Variável global da DLL
	extern DlltpSO2 int nDLL;	
	//Funções a serem exportadas/importadas
	DlltpSO2 int UmaString(void);

#ifdef __cplusplus
}
#endif
