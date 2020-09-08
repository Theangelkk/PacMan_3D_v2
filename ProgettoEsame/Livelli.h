#pragma once
#include "Libs.h"
#include "GestoreNemici.h"
#include "GestoreMovimento.h"
#include "RichiestaGUI.h"
#include "ControllerPlayer.h"
#include "CallBack.h"
#include "GestoreAudio.h"

/*	-------------------------- DEFINIZIONE DELLA CLASSE ----------------------------------
	Classe Livelli relativa alla gestione e settggio di tutte le impostazioni del Gioco.
	Risulta essere la prima Classe utilizzata per avviare l'intero Game, andando
	a definire il Livello di difficoltà, la classifica dei 5 migliori Punteggi, il Game
	Over dell'utente ed il CountDown iniziale.

	La classe è stata implementata attraverso l'utilzzo del Design Pattern Singleton
	dato che si richiedeva l'esistenza di una singola istanza per questa specifica classe.
-------------------------------------------------------------------------------------------*/
class Livelli
{
	public:
		struct ScoreUser
		{
			string User;
			int Punteggio;
			int Tempo_Sec;

			bool operator > (const ScoreUser& str) const
			{
				if (Punteggio > str.Punteggio)
					return true;
				else if (Punteggio == str.Punteggio)
					if (Tempo_Sec <= str.Tempo_Sec)
						return true;

				return false;
			}
		};

		static Livelli* Instance();

		static vector<ScoreUser> getListaBestScore();
		int getLivelloAttuale();

		void SelezionaLivello(int level);
		bool StatusPauseGame();

		void StartGame();
		void Resume_Pause_Game();
		
		void UtentePerdeVita();
		void GameOver();

	private:
		Livelli();

		void ResetGame();
		void PlayGame();
		void SetPositionPacMan();
		void SetPositionNemici();
		void SetVelocitaMovimenti();
		void ScritturaScoreOnFile();
		static void CaricamentoBestScore();
		void checkTop5BestScore(string NomeUtente, int Punteggio, int secondiGame, string Completa);
		static void CountDownIniziale(int v);

		static Livelli* Istanza;

		static const char* pathStrutturaGioco[3];
		static const char* pathNemico[5];
		static const char* pathScoreFile;
		static const char* pathBestScoreFile;

		static vector<Nemico*> ListaNemici;
		int Livello = 1;
		bool FirstTime = true;
		bool Pause = false;
		bool changeStatusPause = true;
		static int TempoInzialeAttesa;

		ofstream FileGameScore;
		static ifstream LetturaBestScore;
		static ofstream ScritturaBestScore;
		static vector<struct ScoreUser> ListaScore;

};
