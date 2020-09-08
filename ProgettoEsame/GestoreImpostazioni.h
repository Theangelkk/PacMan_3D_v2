#pragma once
#include "Libs.h"

/*	--------------------- DEFINIZIONE DELLA CLASSE ----------------------------------
	Classe GestoreImpostazioni relativa alla gestione ed al mantenimento di tutte le
	impostazioni e politiche di gioco.
	Nello specifico, le impostazioni gestite e quindi mantenute in questa classe sono:
	- Risoluzione dello schermo
	- Livello di difficoltà del gioco
	- Audio attivo/spento
	- Risoluzione a schermo intero attiva/disattiva
--------------------------------------------------------------------------------------*/
class GestoreImpostazioni
{
	public:
		GestoreImpostazioni(pair<int, int> _resSchermo, bool _audio, int _difficolta)
		{
			resSchermo = _resSchermo;
			audio = _audio;
			difficolta = _difficolta;
		}

		void setDifficolta(int _difficolta)				{ difficolta = _difficolta; }
		void setFullScreen(bool _fullScreen)			{ fullScreen = _fullScreen; }
		void setResSchermo(pair<int, int> _resSchermo)	{ resSchermo = _resSchermo; }
		void setAudio(bool _audio)						{ audio = _audio; }

		pair<int, int> getResSchermo()					{ return resSchermo; }
		bool getAudio()									{ return audio; }
		int getDifficolta()								{ return difficolta; }
		bool getFullScreen()							{ return fullScreen; }

	private:
		pair<int, int> resSchermo;
		bool audio;
		int difficolta;

		bool fullScreen = false;
};

