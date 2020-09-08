#pragma once
#include "Libs.h"
#include "GestoreScena.h"
#include "GestoreUtente.h"
#include "Modellazione_3D.h"
#include "Coin.h"
#include "Bonus.h"
#include "RandomValueUniform.h"
#include "GestoreAudio.h"

/*
	la Classe RegoleGame si occupa dell'impostazione, controllo e settaggio
	di tutte le Politiche di Gioco presenti nel VideoGame.
	Nello specifico si occupa della Gestione del Cronometro della Partita, la 
	completa gestione (Creazione,Assegnazione Punteggio,Collisioni) dei Coin e 
	Bonus presenti.
*/
class RegoleGame
{
	public:
		static RegoleGame* Istance();

		static void InitTimeGame();
		static int getSecondGame();
		void setActiveTimerGame(bool status);

		void InitRegoleGame();
		void PauseTimeGame();
		void ResumeTimeGame();

		void Draw_Bonus_Coin();
		void Draw_Bonus_Bonus();

		void Check_Collision(int Riga_Utente, int Colonna_Utente);

	private:
		RegoleGame();					

		void CreateNewBonus();
		void VerifyNewBonus(char Tipo_Bonus, int rig, int col);
		void RefillCoin();

		static RegoleGame* Istanza;

		static int msTimeGame;
		static void TimerGame(int v);

		unordered_map<string, Coin*> TabellaHash_Coin;
		unordered_map<string, Bonus*> TabellaHash_Bonus;
		vector<pair<int, int>> CelleLibere;
		vector<pair<int, int>> CelleLibere_Iniziali;
		vector<Bonus*> PrototipoBonus;

		int Bonus_DaCreare = 0;
		int TempoResiduoNuovoBonus = 0;
		bool activeTimeGame = true;

		const char* pathOrange = "Modelli/Orange.obj";
		const char* pathBanana = "Modelli/Banana.obj";
		const char* pathApple = "Modelli/Apple.obj";
		const char* pathPeperoncino = "Modelli/Peperoncino.obj";
};
