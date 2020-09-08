#include "GestoreNemici.h"

GestoreNemici* GestoreNemici::Istanza = nullptr;

GestoreNemici* GestoreNemici::Instance()
{
	if (Istanza == nullptr) {   // Only allow one instance of class to be generated.
		Istanza = new GestoreNemici();
	}
	return Istanza;
}

void GestoreNemici::setTime(int t)
{
	if (t < 0)
		Time = 100;
	else
		Time = t;
}

/*
	Questo metodo ha la responsabilità del permesso di esecuzione
	del movimento del nemico.
	Nello specifico, permette al nemico di eseguire tutte le
	funzioni relative al suo movimento solamente nel momento in cui
	l'utente non è morto e se il gioco è attivo (non è in uno stato di pausa).
*/
void GestoreNemici::Init_MovimentoNemico(int v)
{
	/*
		Se l'Utente non è Morto:
	*/
	if (!GestoreUtente::Instance()->getUtenteMorto())
	{
		/*
			Scorre il vector in cui sono contenuti i puntatori
			a tutte le instanze di tutti i nemici creati.
		*/
		for (unsigned int i = 0; i < Instance()->Nemici.size(); i++)
			Instance()->Nemici.at(i)->Movimento();

		/*
			Se il gioco non è in pausa, permetti
			ancora ai nemici di muoversi.
		*/
		if (!Istanza->Pause)
			glutTimerFunc(Istanza->Time, GestoreNemici::Init_MovimentoNemico, v);
	}
}

void GestoreNemici::enablePause()
{
	if (!Pause)
		Pause = true;
}

void GestoreNemici::disablePause()
{
	if (Pause)
	{
		Pause = false;
		glutTimerFunc(Istanza->Time, GestoreNemici::Init_MovimentoNemico, 0);
	}
}

/*
	Questo metodo calcola il quadrante in cui il nemico si muoverà
	nell'arco della durata del game.
	Dopo aver assegnato uno specifico quadrante per ogni nemico, calcola
	per ogni nemico l'insieme di tutte le possibili posizioni in cui il nemico
	può muoversi all'interno del suo quadrante.
	In tal modo, tale operazione viene eseguita una sola volta durante il caricamento
	iniziale del gioco risparmiando carico computazionale.
*/
void GestoreNemici::AssegnaQuadrante()
{
	int h, l;

	/*
		Recupera altezza e larghezza della struttura del gioco
	*/
	GestoreScena::Instance()->GetAltezzaLarghezza(h, l);

	/*
		Per ogni nemico calcola il suo quadrante e glielo assegna.
	*/
	Instance()->Nemici.at(0)->setQuadrante(0, 0, h / 2, l / 2);
	Instance()->Nemici.at(1)->setQuadrante(0, l / 2 + 1, h / 2, l - 1);
	Instance()->Nemici.at(2)->setQuadrante(h / 2 + 1, 0, h - 1, l / 2);
	Instance()->Nemici.at(3)->setQuadrante(h / 2 + 1, l / 2 + 1, h - 1, l - 1);

	/*
		Calcola le celle libere, in cui può muoversi il nemico
		all'interno del suo quadrante.
	*/
	Instance()->Nemici.at(0)->GeneraCelleDisp();
	Instance()->Nemici.at(1)->GeneraCelleDisp();
	Instance()->Nemici.at(2)->GeneraCelleDisp();
	Instance()->Nemici.at(3)->GeneraCelleDisp();
}

void GestoreNemici::PushNemico(Nemico* nemico)
{
	Nemici.push_back(nemico);
}

vector<Nemico*> GestoreNemici::GetNemici()
{
	return Nemici;
}

void GestoreNemici::ResetNemici()
{
	Nemici.clear();
}
