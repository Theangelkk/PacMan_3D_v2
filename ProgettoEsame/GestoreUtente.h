#pragma once
#include "Libs.h"
#include "Elem3D.h"
#include "Modellazione_3D.h"

#define aisgl_min(x,y) (x<y?x:y)
#define aisgl_max(x,y) (y>x?y:x)

/*	--------------------- DEFINIZIONE DELLA CLASSE ----------------------------------
	Classe GestoreUtente relativa alla gestione dell'utente, di tutte le sue
	caratterisriche e possibili operazioni durante il ciclo di vita del pacman durante
	il gioco.
	Tale classe ha il compito di :
	- Notificare se l'utente ha cambiato posizione
	- Gestione dei punti vita dell'utente
	- Gestione collisione del pacman con il nemico
	- Gestione modalità utente / superutente
--------------------------------------------------------------------------------------*/
class GestoreUtente : public Elem3D
{
	public:
		static GestoreUtente* Instance();
		static void ResetCountDown();
		static void CountDown_SuperUtente(int v);

		void Draw()			override;

		int getRiga()		override;
		int getColonna()	override;
		int getPunteggio();
		int getLife();
		string getNomeUtente();
		bool getUtenteMorto();

		bool StatusMovment();
		int StatusChangePosition();
		bool StatusUtente();

		void setRiga(int _riga);
		void setColonna(int _col);
		void setRiga(double _riga);
		void setColonna(double _col);
		void setPosition(int _riga, int _col);
		void setMovment(bool move);
		void setChangePosition(int change);
		void setNomeUtente(string n);
		void setUtenteMorto(bool _utenteMorto);
		
		void IncreasePunteggio(int Valore);
		void decreaseLife();
		void enableSuperUser();
		void disableSuperUser();
		void AssegnaPosizioneDopoMorte(pair<int, int> AngoloAltoSx, pair<int, int> AngoloBassoDx);
		void ModalitaUtente(int rigaUtente, int colonnaUtente, int pos);

		void ResetPlayer();

	private:
		GestoreUtente();							
		
		static GestoreUtente* Istanza;
		static int CountDown;

		bool Movment = false;

		double Riga = 0.0;
		double Colonna = 0.0;

		/*
			Possibili Valori di ChangePosition
				0 = Nessun Cambiamento
				1 = Sinistra
				2 = Destra
				3 = Alto
				4 = Basso
		*/
		int ChangePosition = 0;

		const struct aiScene* Utente_Modello3D = nullptr;
		const struct aiScene* UtenteSuper_Modello3D = nullptr;
		GLuint* texture = nullptr;
		GLuint* textureSuper = nullptr;
		aiVector3D scene_min;
		aiVector3D scene_max;
		aiVector3D scene_center;

		const char* Path_Modello3D =		"Modelli/PacMan.obj";
		const char* Path_Modello3D_Super =	"Modelli/PacMan_Avversario.obj";

		GLuint Utente_Lista_Draw = 0;
		GLuint UtenteSuper_Lista_Draw = 0;
		GLfloat Rotation = 90.0;
		GLfloat AsseY = 0.0f;
		GLfloat AsseZ = 0.0f;

		int Punteggio = 0;
		int Life = 3;
		string NomeUtente;
		bool SuperUtente = false;
		int CountSuperUtente = 0;
		bool UtenteMorto = false;
};