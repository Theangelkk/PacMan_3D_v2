#pragma once
#include "Libs.h"
#include "GestoreScena.h"
#include "Nemico.h"

/*	--------------------- DEFINIZIONE DELLA CLASSE ----------------------------------
	Classe GestoreNemici relativa alla gestione del nemico.
	Prevede la possibilità di inizializzare la posizione del nemico ad inizio partita,
	calcolando il suo quadrante di appartenenza e generando l'insieme delle posibili celle
	di tale quadrante in cui il nemico può muoversi e le memorizza nell'instanza specifica
	di quel nemico.
	Inoltre, tale classe ha il compito della gestione del movimento del nemico; movimento
	che è permesso se il gioco non è in pausa.
	Tale gestione è relizzata in modalità Asincrona rispetto al FrameRate del Gioco.
--------------------------------------------------------------------------------------*/
class GestoreNemici
{
	public:
		static GestoreNemici* Instance();
		static void Init_MovimentoNemico(int v);
		static void AssegnaQuadrante();
		
		vector<Nemico*> GetNemici();
		void PushNemico(Nemico* nemico);
		void ResetNemici();
		
		void enablePause();
		void disablePause();
		void setTime(int t);

	private:
		GestoreNemici() {};					
		static GestoreNemici* Istanza;

		vector<Nemico*> Nemici;
		bool Pause = false;
		int Time = 100;
};
