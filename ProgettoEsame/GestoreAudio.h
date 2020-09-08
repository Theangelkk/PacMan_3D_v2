#pragma once
#include "Libs.h"

using namespace irrklang;
#pragma comment(lib, "irrKlang.lib")

/*	--------------------- DEFINIZIONE DELLA CLASSE ----------------------------------
	Classe GestioneAudio relativa alla gestione e caricamento di tutti gli effetti
	Sonori presenti nell'Applicativo.
	Risulta essere un Wrapper della per Libreria irrKlang dove, sono stati ridefiniti 
	specifici metodi per alcune determinate casistiche del videogioco, come ad esempio
	l'esecuzione in background della colonna sonora del Menù.
--------------------------------------------------------------------------------------*/
class GestoreAudio
{
	public:
		static GestoreAudio* Instance();

		void EnableAudio();
		void DisableAudio();

		void StartSoundMenu();
		void StartSoundGame();

		void PlaySoundCountDown();
		void PlaySoundClick();
		void PlaySoundEatCoin();
		void PlaySoundEatBonus();
		void PlaySoundEatNemici();
		void PlaySoundSuperPacman();
		void PlaySoundRefillGame();
		void PlaySoundLooseLife();
		void PlaySoundGameOver();
		void PlaySoundPause();

		void StopAllSounds();

	private:
		GestoreAudio();							
		static GestoreAudio* Istanza;

		ISoundEngine* engine = 0;
		bool activeAudio = true;

		const char* audioBackgroundMenu =		"Audio/audioBackgroundMenu.mp3";
		const char* audioClick =				"Audio/audioClick.mp3";
		const char* audioBackgroundGame =		"Audio/audioBackgroundGame.mp3";
		const char* audioCountDown =			"Audio/audioCountDown.mp3";
		const char* audioEatCoin =				"Audio/audioEatCoin.mp3";
		const char* audioEatBonus =				"Audio/audioEatBonus.mp3";
		const char* audioEatNemici =			"Audio/audioEatNemico.mp3";
		const char* audioRefillGame =			"Audio/";
		const char* audioLooseLife =			"Audio/audioLooseLife.mp3";
		const char* audioGameOver =				"Audio/audioGameOver.mp3";
		const char* audioPausa =				"Audio/audioPausa.mp3";
		const char* audioSuperPacman =			"Audio/audioSuperPacman.mp3";
};

