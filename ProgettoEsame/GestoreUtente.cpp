#include "GestoreUtente.h"
#include "Livelli.h"

GestoreUtente* GestoreUtente::Istanza = nullptr;
int GestoreUtente::CountDown = 10000;

GestoreUtente* GestoreUtente::Instance()
{
    if (Istanza == nullptr)
		Istanza = new GestoreUtente;
    
    return Istanza;
}

/*
	Nel costruttore dell'utente vengono inizializzate tutte le impostazioni
	dell'utente prima della partita e vengono caricate e create le liste
	dei modelli del pacman relative alla modalità user/super user.
*/
GestoreUtente::GestoreUtente()
{
	ResetPlayer();

	Utente_Modello3D = Modellazione_3D::Caricamento_Modello3D(Path_Modello3D, &scene_min, &scene_center, &scene_max);
	UtenteSuper_Modello3D = Modellazione_3D::Caricamento_Modello3D(Path_Modello3D_Super, &scene_min, &scene_center, &scene_max);
	texture = Modellazione_3D::LoadGLTextures(Utente_Modello3D, Path_Modello3D);
	textureSuper = Modellazione_3D::LoadGLTextures(UtenteSuper_Modello3D, Path_Modello3D_Super);

	Utente_Lista_Draw = glGenLists(1);
	glNewList(Utente_Lista_Draw, GL_COMPILE);
		Modellazione_3D::Recursive_Render(Utente_Modello3D, Utente_Modello3D->mRootNode,texture);
	glEndList();

	UtenteSuper_Lista_Draw = glGenLists(1);
	glNewList(UtenteSuper_Lista_Draw, GL_COMPILE);
		Modellazione_3D::Recursive_Render(UtenteSuper_Modello3D, UtenteSuper_Modello3D->mRootNode, textureSuper);
	glEndList();
}

void GestoreUtente::ResetPlayer()
{
	Movment = false;

	Riga = 0;
	Colonna = 0;

	ChangePosition = 0;
	Punteggio = 0;
	Life = 3;
	SuperUtente = false;
	CountSuperUtente = 0;
	UtenteMorto = false;

	GestoreUtente::ResetCountDown();
}

int GestoreUtente::getRiga()
{
	return (int) Riga;
}

int GestoreUtente::getColonna()
{
	return (int) Colonna;
}

void GestoreUtente::setRiga(int _riga)
{
	Riga = (double) _riga;
}

void GestoreUtente::setColonna(int _col)
{
	Colonna = (double) _col;
}

void GestoreUtente::setRiga(double _riga)
{
	Riga = _riga;
}

void GestoreUtente::setColonna(double _col)
{
	Colonna = _col;
}

void GestoreUtente::setPosition(int _riga, int _col)
{
	Riga = (double) _riga;
	Colonna = (double) _col;
}

string GestoreUtente::getNomeUtente()
{
	return NomeUtente;
}

void GestoreUtente::setNomeUtente(string n)
{
	NomeUtente = n;
}

void GestoreUtente::setChangePosition(int change)
{
	ChangePosition = change;

	/*
		Spostamento dell'Utente verso Sinistra
	*/
	if (ChangePosition == 1)
	{
		Rotation = 0.0;
	}

	/*
		Spostamento dell'Utente verso Destra
	*/
	else if (ChangePosition == 2)
	{
		Rotation = 180.0;
		AsseY = 1.0;
		AsseZ = 0.0;
	}
	
	/*
		Spostamento dell'Utente verso l'Alto
	*/
	else if (ChangePosition == 3)
	{
		Rotation = -90.0;
		AsseY = 1.0;
		AsseZ = 1.0;
	}

	/*
		Spostamento dell'Utente verso il Basso
	*/
	else if(ChangePosition == 4)
	{
		Rotation = 90.0;
		AsseY = 0.0;
		AsseZ = 1.0;
	}
}

void GestoreUtente::setMovment(bool move)
{
	Movment = move;
}

bool GestoreUtente::StatusMovment()
{
	return Movment;
}

int GestoreUtente::StatusChangePosition()
{
	return ChangePosition;
}

void GestoreUtente::IncreasePunteggio(int Valore)
{
	Punteggio += Valore;
}

void GestoreUtente::decreaseLife()
{
	UtenteMorto = true;

	Life -= 1;

	/*
		Se l'utente ha terminato le sue vite, si ritorna
		al Menù Principale
	*/
	if (Life == 0)
		Livelli::Instance()->GameOver();
	else
		Livelli::Instance()->UtentePerdeVita();
}

int GestoreUtente::getLife()
{
	return Life;
}

int GestoreUtente::getPunteggio()
{
	return Punteggio;
}

/*
	Modalità SuperUser dove in questo caso il PacMan ha la
	possibilità di mangiare i Nemici presenti all'interno del
	Game
*/
void GestoreUtente::enableSuperUser()
{
	CountSuperUtente += 1;
	
	/*
		Se non è ancora attiva la Modalità SuperUser
	*/
	if (CountSuperUtente == 1)
	{
		SuperUtente = true;

		/*
			Attivazione del CountDown per restare il questa
			Modalità
		*/
		GestoreUtente::ResetCountDown();
		GestoreUtente::CountDown_SuperUtente(1000);

		GestoreAudio::Instance()->StopAllSounds();
		GestoreAudio::Instance()->PlaySoundSuperPacman();
	}
}

void GestoreUtente::disableSuperUser()
{
	SuperUtente = false;
}

void GestoreUtente::ResetCountDown()
{
	CountDown = 10000;
}

void GestoreUtente::CountDown_SuperUtente(int v)
{
	CountDown -= 1000;

	if (CountDown <= 0)
	{
		GestoreUtente::Instance()->CountSuperUtente -= 1;

		/*
			Se il PacMan ha mangiato ancora altri Bonus che lo
			permettono di usufruire della Modalità SuperUser, 
			viene resettato il CountDown e si resta in questa
			specifica Modalità
		*/
		if (GestoreUtente::Instance()->CountSuperUtente > 0)
		{
			GestoreUtente::Instance()->ResetCountDown();
			glutTimerFunc(v, GestoreUtente::CountDown_SuperUtente, v);

			GestoreAudio::Instance()->StopAllSounds();
			GestoreAudio::Instance()->PlaySoundSuperPacman();
		}
		else
		{
			GestoreUtente::Instance()->disableSuperUser();
			GestoreAudio::Instance()->StartSoundGame();
		}
	}
	else
		glutTimerFunc(v, GestoreUtente::CountDown_SuperUtente, v);
}

bool GestoreUtente::StatusUtente()
{
	return SuperUtente;
}

bool GestoreUtente::getUtenteMorto()
{
	return UtenteMorto;
}

void GestoreUtente::setUtenteMorto(bool _utenteMorto)
{
	UtenteMorto = _utenteMorto;
}

/*
	Metodo richiamato quando il PacMan perde una vita.
	Per riprendere a giocare, il PacMan viene posizionato nella 
	Posizione di maggior distanza rispetto al quadrante (utilizzando
	per il calcolo la Posizione del Vertice in Alto a Sinistra e quello
	in Basso a Destra) in cui esso è effettivamente contenuto.
*/
void GestoreUtente::AssegnaPosizioneDopoMorte(pair<int, int> AngoloAltoSx, pair<int, int> AngoloBassoDx)
{
	int dist1 = abs(Riga - AngoloAltoSx.first) + abs(Colonna - AngoloAltoSx.second);
	int dist2 = abs(Riga - AngoloBassoDx.first) + abs(Colonna - AngoloBassoDx.second);

	if (dist1 > dist2)
	{
		Riga = AngoloAltoSx.first;
		Colonna = AngoloAltoSx.second;
	}
	else
	{
		Riga = AngoloBassoDx.first;
		Colonna = AngoloBassoDx.second;
	}
}

/*
	Il Metodo ModalitàUtente verifica effettivamente lo Status dell'utente,
	quindi se risulta essere nella modalità Normale oppure SuperUser, e di 
	conseguenza verifica se nel suo spostamento ha intercettato qualche Nemico
	presenti nel Videogioco.
	Si posso verificare in sostanza due Scenari possibili:
		-	Il Primo è quello in cui l'Utente risulta essere nella Modalità Normale
			e quindi verrà mangiato dal Nemico perdendo una Vita.
		-	Il Seconodo è quello in cui l'Utente risulta essere nella Modalità SuperUser
			e quindi sarà lui questa volta invece a mangiare il Nemico, incrementando 
			il suo Punteggio.
*/
void GestoreUtente::ModalitaUtente(int rigaUtente, int colonnaUtente, int pos)
{
	for (const auto& nemico : GestoreNemici::Instance()->GetNemici())
	{
		int rigaNemico, colonnaNemico;
		bool sonoMorto;

		nemico->getParametri(rigaNemico, colonnaNemico, sonoMorto);

		/*
			Secondo Scenario Possibile: PacmMan mangia il Nemico nella Modalità
			SuperUser.
		*/
		if (rigaNemico == rigaUtente && colonnaNemico == colonnaUtente
			&& !sonoMorto && GestoreUtente::Instance()->StatusUtente())
		{
			nemico->impostaMorte();
			IncreasePunteggio(100);
			GestoreAudio::Instance()->PlaySoundEatNemici();
			return;
		}
		/*
			Primo Scenario Possibile: Nemico mangia il PacMan.
		*/
		else if (!GestoreUtente::Instance()->StatusUtente())
		{
			/*
				Spostamento verso Sinistra
			*/
			if (pos == 1)
				colonnaNemico--;
			/*
				Spostamento verso Destra
			*/
			else if (pos == 2)
				colonnaNemico++;
			/*
				Spostamento verso Alto
			*/
			else if (pos == 3)
				rigaNemico--;
			/*
				Spostamento verso Basso
			*/
			else if (pos == 4)
				rigaNemico++;
			
			/*
				Viene verificato se nel momento dell'intersezione tra il PacMan nella
				Modalità Normale ed invece il Nemico, quest'ultimo non si ancora nella
				fase di rigenerazione dopo la sua Morte.
			*/
			if (rigaNemico == rigaUtente && colonnaNemico == colonnaUtente
				&& !sonoMorto)
			{
				nemico->setPuoMangiare(true);
				return;
			}
		}
	}
}

void GestoreUtente::Draw()
{
	float Tmp = 0.0;

	if (Utente_Modello3D)
	{	
		Tmp = scene_max.x - scene_min.x;
		Tmp = aisgl_max(scene_max.y - scene_min.y, Tmp);
		Tmp = aisgl_max(scene_max.z - scene_min.z, Tmp);

		Tmp = 1.5f / Tmp;

		glScalef(Tmp, Tmp, Tmp);

		glRotatef(Rotation, 0.0f, AsseY, AsseZ);

		if(SuperUtente)
			glCallList(UtenteSuper_Lista_Draw);
		else
			glCallList(Utente_Lista_Draw);
	}
}
