#pragma once
#include "Libs.h"
#include "GestoreUtente.h"

/*	--------------------- DEFINIZIONE DELLA CLASSE ----------------------------------
	Classe GestoreMovimento relativa alla gestione del movimento del pacman, ossia del
	giocatore.
	In particolare, i metodi di questa classe hanno il compito di controllare
	se il pacman può effettuare un movimento, e quindi cambiare cella del gioco, oppure
	non può spostarsi dalla sua cella perché ha incontrato un ostacolo.
--------------------------------------------------------------------------------------*/
class GestoreMovimento 
{
    public:
        static GestoreMovimento* Instance();

		void CaricaStrutturaGioco(const char* pathGioco);
        bool ControllaPosizioneUtente(int Riga, int Colonna, int &NuovaRiga, int &NuovaColonna);
		void MovimentoPlayer();
		
		void SetNuovaRigaColonna(int Riga, int Colonna, char ID);
        
		vector<string> GetStruttuaGioco();
        GLfloat GetZ();
        double GetPosizioneY();

    private:
		GestoreMovimento();								
        static GestoreMovimento* Istanza;

        void MoveUser();

        vector<string> StrutturaGioco;
        double Riga_Utente_offset = 0.0; 
        double Colonna_Utente_offset = 0.0;

        GLfloat z = -91;
		double PosInizialeY=0.0;
};