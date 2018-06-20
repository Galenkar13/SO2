#include <Windows.h>
#include "cliente.h"


void ReproduzirSom(LPCTSTR som)
{
	if (waveOutGetNumDevs() > 0)
	{
		waveOutSetVolume(0, 0xff00ff00);
		PlaySound(som, NULL,  SND_ASYNC );
	}
}

void ReproduzirMusica(LPCTSTR musica)
{
	if (waveOutGetNumDevs() > 0)
	{
		waveOutSetVolume(0, 0xff00ff00);
		PlaySound(musica, NULL, SND_NOSTOP | SND_ASYNC | SND_LOOP);
	}
}