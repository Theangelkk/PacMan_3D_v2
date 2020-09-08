#pragma once
#include "Libs.h"
#include "RegoleGame.h"
#include "Livelli.h"

/*	--------------------- DEFINIZIONE DELLA CLASSE ----------------------------------
	Classe ControllerPlayer relativa alla Gestione ed Interazione dell'utente durante
	tutta la Fase del Gioco.
	Prevede il controllo dell'Input da Tastiera per muovere il giocatore oppure attivare 
	diverse funzionalità offerte dall'applicativo.
	La gestione completa dell'input è relizzata in modalità Asincrona rispetto al 
	FrameRate del Gioco.
--------------------------------------------------------------------------------------*/

/*
	Definizione dei Tasti del Gioco
*/
#define VK_UP 0x26		// Freccia UP
#define VK_DOWN 0x28	// Freccia DOWN
#define VK_LEFT 0x25	// Freccia LEFT
#define VK_RIGHT 0x27	// Freccia RIGHT
#define VK_F1 0x70		// F1 per mettere il Gioco in Pausa oppure per Riprendere

class ControllerPlayer
{
	public:
		static ControllerPlayer* Instance();

		ControllerPlayer();

		static void Init_Controller(int v);

		void checkTime(int time);
		void setTime(int time);

		void enablePause();
		void disablePause();

		void KeyboardInput();

	private:
		static ControllerPlayer* Istanza;

		int Time = 0;
		bool Pause = false;
};

