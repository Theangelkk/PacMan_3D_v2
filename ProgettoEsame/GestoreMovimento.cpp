#include "GestoreMovimento.h"

GestoreMovimento* GestoreMovimento::Istanza = nullptr;

GestoreMovimento* GestoreMovimento::Instance()
{
    if (Istanza == nullptr) 
        Istanza = new GestoreMovimento();

    return Istanza;
}

GestoreMovimento::GestoreMovimento()
{	}

/*
	Caricamento della Struttura del Gioco dal File di testo
	Selezionato.
	L'intera struttura del Gioco verrà memorizzata all'interno
	di un vector<string> dove, ogni riga rappresenta una riga effettiva
	del Gioco.
*/
void GestoreMovimento::CaricaStrutturaGioco(const char* pathGioco)
{
	StrutturaGioco.clear();
	ifstream input(pathGioco);

	/*
		Apertura del File che contiene la Struttura del Gioco
	*/
	if (!input.is_open()) 
		cout <<"Errore durante l'apertura della Struttura del Gioco" << endl;

	/*
		Inserimento dell'intera struttura del gioco all'interno
		del vector StrutturaGioco
	*/
	for (string line; getline(input, line);) 
		StrutturaGioco.push_back(line); 

	input.close();

	PosInizialeY = (double)StrutturaGioco.size();
}

/*
	Il metodo verifica se la nuova posizione dell'utente risula essere
	una posizione valida o meno.
	Viene restitito il valore:
		-	True  = se l'utente può spostarsi nella Nuova Posizione
		-	False = se ha incontrato un ostacolo
*/
bool GestoreMovimento::ControllaPosizioneUtente(int Riga, int Colonna, int& NuovaRiga, int& NuovaColonna)
{
	if (Riga < 0) 
	{ 
		NuovaRiga = StrutturaGioco.size() - 1;
		NuovaColonna = Colonna;
		return true;
	}

	else if (unsigned int(Riga) >= StrutturaGioco.size()) 
	{
		NuovaRiga = 0;
		NuovaColonna = Colonna;
		return true;
	}

	if (Colonna < 0) 
	{
		NuovaRiga = Riga;
		NuovaColonna = StrutturaGioco[0].size()-1;
		return true;
	}

	else if (unsigned int(Colonna) >= StrutturaGioco[0].size())
	{
		NuovaRiga = Riga;
		NuovaColonna = 0;
		return true;
	}

	if (unsigned int(Riga) < StrutturaGioco.size())
	{
		if (StrutturaGioco[Riga][Colonna] != '1' && StrutturaGioco[Riga][Colonna] != '*')
		{
			if (StrutturaGioco[Riga][Colonna] != '1') 
			{
				NuovaRiga = Riga;
				NuovaColonna = Colonna;
				return true;
			}
		}
	}
	
	return false;
}

void GestoreMovimento::SetNuovaRigaColonna(int Riga, int Colonna, char ID)
{
	StrutturaGioco.at(Riga).at(Colonna) = ID;
}

vector<string> GestoreMovimento::GetStruttuaGioco()
{
	return StrutturaGioco;
}

void GestoreMovimento::MovimentoPlayer()
{
	/*
		Nel caso in cui l'utente non abbia cambiato la sua posizione
	*/
	if (GestoreUtente::Instance()->StatusChangePosition() == false)
		GestoreUtente::Instance()->setMovment(false);
	
	/*
		Nel momento in cui l'utente invece ha cambiato la sua posizione, 
		viene attivata la sua animazione durante il movimento da una posizione
		all'altra
	*/
	else
	{
		if (GestoreUtente::Instance()->StatusMovment() == false)
		{
			GestoreUtente::Instance()->setMovment(true);

			int ChangePositionUser = GestoreUtente::Instance()->StatusChangePosition();

			/*
				Spostamento verso Sinistra
			*/
			if (ChangePositionUser == 1)
			{
				Colonna_Utente_offset = 1.0;
				Riga_Utente_offset = 0.0;
			}

			/*
				Spostamento verso Destra
			*/
			else if (ChangePositionUser == 2)
			{
				Colonna_Utente_offset = -1.0;
				Riga_Utente_offset = 0.0;
			}

			/*
				Spostamento verso l'Alto
			*/
			else if (ChangePositionUser == 3)
			{
				Colonna_Utente_offset = 0.0;
				Riga_Utente_offset = 1.0;
			}

			/*
				Spostamento verso il Basso
			*/
			else if (ChangePositionUser == 4)
			{
				Colonna_Utente_offset = 0.0;
				Riga_Utente_offset = -1.0;
			}

			MoveUser();
		}
		else
			MoveUser();
	}
	
	glutPostRedisplay();
}

GLfloat GestoreMovimento::GetZ()
{
	return z;
}

double GestoreMovimento::GetPosizioneY()
{
	return PosInizialeY;
}

/*
	Animazione del Movimento dell'utente nello spostarsi da una 
	Vecchia Posizione ad una Nuova.
	L'animazione durante la sua Traslazione da la Posizione Iniziale 
	e quella Finale viene calcolata attraverso una Interpolazione Lineare
	tra 4 Posizioni Intermedie.
*/
void GestoreMovimento::MoveUser()
{
	int ChangePositionUser = GestoreUtente::Instance()->StatusChangePosition();

	double Riga = (double)GestoreUtente::Instance()->getRiga();
	double Colonna = (double)GestoreUtente::Instance()->getColonna();

	/*
		Spostamento verso Sinistra
	*/
	if (ChangePositionUser == 1)
	{
		Colonna_Utente_offset -= 0.25;
		Riga_Utente_offset = 0.0;
	}

	/*
		Spostamento verso Destra
	*/
	else if (ChangePositionUser == 2)
	{
		Colonna_Utente_offset += 0.25;
		Riga_Utente_offset = 0.0;
	}
	
	/*
		Spostamento verso Alto
	*/
	if (ChangePositionUser == 3)
	{
		Colonna_Utente_offset = 0.0;
		Riga_Utente_offset -= 0.25;
	}
	
	/*
		Spostamento verso Basso
	*/
	else if (ChangePositionUser == 4)
	{
		Colonna_Utente_offset = 0.0;
		Riga_Utente_offset += 0.25;
	}

	/*
		Se l'animazione risulta essere Terminata
	*/
	if (abs(Colonna_Utente_offset) < 0.25 && abs(Riga_Utente_offset) < 0.25)
	{
		GestoreUtente::Instance()->setChangePosition(0);
		GestoreUtente::Instance()->setMovment(false);

		Riga_Utente_offset = 0.0;
		Colonna_Utente_offset = 0.0;
	}

	GestoreUtente::Instance()->setRiga(Riga);
	GestoreUtente::Instance()->setColonna(Colonna);

	glutPostRedisplay();
}