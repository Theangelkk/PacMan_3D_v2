#pragma once
#include "Libs.h"
#include "GestoreTexture.h"

/*	--------------------- DEFINIZIONE DELLA CLASSE ----------------------------------
	Classe GestoreScena relativa alla gestione della scena e della struttura del gioco.
	Ha il compito di caricare da file la struttura del circuito di game e di
	merorizzarla nella struttura dati 'StrutturaGioco'.
	Inoltre, ha il compito di attivare/disabilitare le componenti essenziali in OpenGL
	(come la luce, le normali, l'antialiasing, ecc) ad inizio/fine partita.
--------------------------------------------------------------------------------------*/
class GestoreScena 
{
	public:
		static GestoreScena* Instance();

		void InitScena(const char* pathGioco);
	
		void GetAltezzaLarghezza(int & h, int & l);
		vector<string> GetStruttuaGioco();
		GLfloat GetZ();
		double GetPosY();

	private:
		GestoreScena();							
		static GestoreScena* Istanza;

		void Setup();
		void LeggiMatriceDaFile(const char* pathGioco);

		vector<string> StrutturaGioco;
		GLfloat z = -91;
		double PosInizialeY = 0.0;
		bool FirstTime = true;
};
