#pragma once
#include "Libs.h"
#include "RichiestaGUI.h"
#include "MenuGame.h"

/*	--------------------- DEFINIZIONE DELLA CLASSE ----------------------------------
	Classe Callbback relativa alla Gestione dei metodi Callback per il
	Rendering grafico ad ogni Frame del Gioco.
	Questa classe controlla sia le funzionalità di Draw relative al Menù Iniziale
	e sia per il Game.
--------------------------------------------------------------------------------*/
class CallBack
{
	public:
		static void Display_Gioco();
		static void Resize_Win(int width, int height);
};

