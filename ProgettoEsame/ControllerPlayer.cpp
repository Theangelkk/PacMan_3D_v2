#include "ControllerPlayer.h"
#include "GestoreMovimento.h"
#include "GestoreUtente.h"

ControllerPlayer* ControllerPlayer::Istanza = nullptr;

ControllerPlayer* ControllerPlayer::Instance()
{
	if (Istanza == nullptr) 
		Istanza = new ControllerPlayer();

	return Istanza;
}

ControllerPlayer::ControllerPlayer()
{
	Time = 100;
	Pause = false;
}

void ControllerPlayer::checkTime(int time)
{
	if (time > 0)
		Time = time;
	else
		Time = 100;
}

void ControllerPlayer::setTime(int time)
{
	if (time > 0)
		Time = time;
}

void ControllerPlayer::enablePause()
{
	if (!Pause)
	{
		Pause = true;
		Time += 1000;
	}
}

void ControllerPlayer::disablePause()
{
	if (Pause)
	{
		Pause = false;
		Time -= 1000;
	}
}

void ControllerPlayer::Init_Controller(int v)
{
	/*
		Abilitiamo il Controller del PacMan solo se l'utente risulta
		essere ancora in vita.
	*/
	if (!GestoreUtente::Instance()->getUtenteMorto())
	{
		Istanza->KeyboardInput();

		glutTimerFunc(Istanza->Time, ControllerPlayer::Init_Controller, v);
	}
}

void ControllerPlayer::KeyboardInput()
{
	/*
		Viene impostato il Gioco in Pausa oppure si ritorna a Giocare
	*/
	if (GetAsyncKeyState(VK_F1))
	{
		Livelli::Instance()->Resume_Pause_Game();
		return;
	}

	if (!Pause)
	{
		/*
			Viene verificato se l'utente attualmente non risulta essere
			in Movimento
		*/
		if (GestoreUtente::Instance()->StatusMovment() == false)
		{
			int Riga = GestoreUtente::Instance()->getRiga();
			int Colonna = GestoreUtente::Instance()->getColonna();
			int Position = 0;
			int NuovaRiga, NuovaColonna;

			/*
				Vero  = se l'utente può effettuare quella Mossa
				Falso = se incontra uno Ostacolo
			*/
			bool Esito;

			/*
				Muovi PacMan verso l'Alto
			*/
			if (GetAsyncKeyState(VK_UP))
			{
				Riga = Riga - 1;
				Position = 3;
			}

			/*
				Muovi PacMan verso il Basso
			*/
			else if (GetAsyncKeyState(VK_DOWN))
			{
				Riga = Riga + 1;
				Position = 4;
			}

			/*
				Muovi PacMan verso Sinistra
			*/
			else if (GetAsyncKeyState(VK_LEFT))
			{
				Colonna = Colonna - 1;
				Position = 1;
			}

			/* 
				Muovi PacMan verso Destra
			*/
			else if (GetAsyncKeyState(VK_RIGHT))
			{
				Colonna = Colonna + 1;
				Position = 2;
			}
			
			/*
				Viene verificata la validità della Mossa effettuata
			*/
			Esito = GestoreMovimento::Instance()->ControllaPosizioneUtente(Riga, Colonna, NuovaRiga, NuovaColonna);

			if (Esito == true)
			{
				GestoreUtente::Instance()->ModalitaUtente(NuovaRiga, NuovaColonna, Position);
				/*
					Viene aggiornata la posizione del PacMan all'interno del Gioco
				*/
				GestoreMovimento::Instance()->SetNuovaRigaColonna(GestoreUtente::Instance()->getRiga(), GestoreUtente::Instance()->getColonna(), '0');
				GestoreMovimento::Instance()->SetNuovaRigaColonna(NuovaRiga, NuovaColonna, 'P');

				/*
					Viene verificato se eventualmente nella sua Nuova Posizione non vi
					sia un Coin oppure un Bonus
				*/
				RegoleGame::Istance()->Check_Collision(NuovaRiga, NuovaColonna);

				/*
					Andiamo graficamente a modificare la sua Nuova Posizione
				*/
				GestoreUtente::Instance()->setPosition(NuovaRiga, NuovaColonna);
				GestoreUtente::Instance()->setChangePosition(Position);

				glutPostRedisplay();
			}
			else
				GestoreUtente::Instance()->ModalitaUtente(Riga, Colonna, Position);
		}
	}
}