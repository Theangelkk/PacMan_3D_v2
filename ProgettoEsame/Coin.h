#pragma once
#include "Libs.h"
#include "Modellazione_3D.h"
#include "GestoreScena.h"

/*	--------------------- DEFINIZIONE DELLA CLASSE ----------------------------------
	Classe Coin relativa alla gestione e creazione delle Monete presenti
	all'interno del Gioco.
	Ogni Moneta possiede il valore 1 ed una sua specifica posizione all'interno
	della Struttura del Gioco.
--------------------------------------------------------------------------------*/
class Coin
{
	public:
		Coin(int rig, int col);
		void Draw();
	
	private:
		GLuint Coin_Lista_Draw = 0;

		int Riga = 0;
		int Colonna = 0;
};
