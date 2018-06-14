#include <Windows.h>
#include "cliente.h"


MCIERROR ReproduzirSom(LPCTSTR som);
void ReproduzirMusica(LPCTSTR musica);

MCIERROR ReproduzirSom(LPCTSTR som)
{
	return mciSendString(som, NULL, 0, NULL);
}

void ReproduzirMusica(LPCTSTR musica)
{
	if (waveOutGetNumDevs() > 0)
	{
		waveOutSetVolume(0, 0xff00ff00);
		PlaySound(musica, NULL, SND_NOSTOP | SND_ASYNC | SND_LOOP);
	}
}