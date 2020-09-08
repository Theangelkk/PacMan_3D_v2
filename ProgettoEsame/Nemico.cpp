#include "Nemico.h"

/*
	Nel costruttore del nemico vengono inizializzate tutte le 
	variabili di inizio gioco del nemico e vengono caricati tutti i modelli del nemico.
*/
Nemico::Nemico(double _riga, double _colonna, const char* p, const char* m)
{
	Riga = _riga;
	Colonna = _colonna;

	contaNumeroMov = 0;
	cornerDaRaggiungere = 0;
	sonoMorto = false;
	puoMangiare = false;

	Path_Modello3D = p;
	Path_Modello3D_Morto = m;

	Nemico_Modello3D = Modellazione_3D::Caricamento_Modello3D(Path_Modello3D, &scene_min, &scene_center, &scene_max);
	texture = Modellazione_3D::LoadGLTextures(Nemico_Modello3D, Path_Modello3D);

	Nemico_Modello3D_Morto = Modellazione_3D::Caricamento_Modello3D(Path_Modello3D_Morto, &scene_min, &scene_center, &scene_max);
	texture_morto = Modellazione_3D::LoadGLTextures(Nemico_Modello3D_Morto, Path_Modello3D_Morto);

	/*
		Creazione lista per il draw del nemico
	*/
	Nemico_Lista_Draw = glGenLists(1);
	glNewList(Nemico_Lista_Draw, GL_COMPILE);
		Modellazione_3D::Recursive_Render(Nemico_Modello3D, Nemico_Modello3D->mRootNode, texture);
	glEndList();

	/*
		Creazione lista per il draw del nemico morto
	*/
	Nemico_Morto_Lista_Draw = glGenLists(1);
	glNewList(Nemico_Morto_Lista_Draw, GL_COMPILE);
		Modellazione_3D::Recursive_Render(Nemico_Modello3D_Morto, Nemico_Modello3D_Morto->mRootNode, texture_morto);
	glEndList();
}

int Nemico::getRiga()
{
	return (int)Riga;
}

int Nemico::getColonna()
{
	return (int)Colonna;
}

void Nemico::setRiga(int _riga)
{
	Riga = (double)_riga;
}
void Nemico::setColonna(int _col)
{
	Colonna = (double)_col;
}

void Nemico::setRiga(double _riga)
{
	Riga = _riga;
}

void Nemico::setColonna(double _col)
{
	Colonna = _col;
}

void Nemico::setQuadrante(int riga1, int colonna1, int riga2, int colonna2)
{
	quadrante.push_back(make_pair(riga1, colonna1));
	quadrante.push_back(make_pair(riga2, colonna2));
}

/*
	Questo metodo trova quali sono le celle del quadrante del nemico dove egli si può
	muovere. Ossia, le celle libere sono tutte quelle celle di StrutturaGioco
	il cui contenuto è uguale a 0.
*/
void Nemico::GeneraCelleDisp()
{
	for (int i = quadrante.at(0).first; i <= quadrante.at(1).first; i++)
		for (int j = quadrante.at(0).second; j <= quadrante.at(1).second; j++)
			if (GestoreScena::Instance()->GetStruttuaGioco().at(i).at(j) == '0') 
				celle_disponibili.push_back(make_pair(i, j));
			
}

bool Nemico::getSonoMorto()
{
	return sonoMorto;
}

void Nemico::setPuoMangiare(bool status)
{
	puoMangiare = status;
}

/*
	Se il nemico è morto, ossia è stato mangiato dal pacman super utente,
	la sua posizione sarà quella all'interno del rettangolo di gioco.
*/
void Nemico::impostaMorte()
{
	sonoMorto = true;
	Riga = 19;
	Colonna = 37;
	contaNumeroMov = listaVertici.size();
	glutPostRedisplay();
}

void Nemico::getParametri(int & RigaNemico, int & ColonnaNemico, bool & _sonoMorto)
{
	RigaNemico = Riga;
	ColonnaNemico = Colonna;
	_sonoMorto = sonoMorto;
}

void Nemico::Movimento()
{
	int RigaUtente;						//conterrà la riga della posizione attuale del PacMan
	int ColonnaUtente;					//conterrà la colonna della posizione attuale del PacMan
	int VecchiaRiga = int(Riga);		//conterrà la riga della posizione attuale del nemico
	int VecchiaColonna = int(Colonna);	//conterrà la colonna della posizione attuale del nemico

	/*
		Si prende la strutturaGioco attuale
	*/
	vector<string> strutturaGioco;
	strutturaGioco = GestoreScena::Instance()->GetStruttuaGioco();

	/*
		Determina la posizione attuale del pacman
	*/
	RigaUtente = GestoreUtente::Instance()->getRiga();
	ColonnaUtente = GestoreUtente::Instance()->getColonna();

	/*
		Se il nemico è morto, richiama il metodo checkPosizione(strutturaGioco)
	*/
	if (sonoMorto) 
	{
		checkPosizione(strutturaGioco);
		glutPostRedisplay();
		return;
	}

	/*
		Se il pacman si trova all'interno del quadrante del nemico
	*/
	if (RigaUtente >= quadrante.at(0).first && RigaUtente <= quadrante.at(1).first &&
		ColonnaUtente >= quadrante.at(0).second && ColonnaUtente <= quadrante.at(1).second)
	{
		contaNumeroMov = listaVertici.size();
		bool Esito;
		TrovaPercorso* t = new TrovaPercorso();

		//Se il pacman è superuser..
		if (GestoreUtente::Instance()->StatusUtente())
		{
			TrovaPercorso* t = new TrovaPercorso();

			/*
				Setta la posizione all'interno della copia di struttura gioco come 'N'.
				In sostanza, è come se adesso il nemico del nemico sia proprio il pacman.
				Adesso è il nemico che deve scappare dal pacman.
			*/
			strutturaGioco.at(RigaUtente).at(ColonnaUtente) = 'N';
			int rig, col;

			/*
				In maniera alternata, uno dei due corner del quadrante del nemico 
				diventa la meta che deve seguire il nemico per scappare dal pacman. 
				Una volta raggiunto un corner, scappa andando nell'altro corner. 
				Nello scappare, ovviamente non va verso il pacman ma calcola ogni 
				volta il prossimo movimento per raggiunger il corner tenendo conto
				della posizione del pacman.
			*/
			if (cornerDaRaggiungere == 0) 
			{
				rig = celle_disponibili.at(0).first;
				col = celle_disponibili.at(0).second;
			}
			else 
			{
				rig = celle_disponibili.at(celle_disponibili.size() - 1).first;
				col = celle_disponibili.at(celle_disponibili.size() - 1).second;
			}

			/*
				Calcola il prox movimento del nemico
			*/
			Esito = t->CalcolaProxMovimento(strutturaGioco, int(Riga), int(Colonna), rig, col, false, vertice);
			delete t;

			/*
				Se non può muoversi, ritorna 
			*/
			if (!Esito) 
				return;

			/*
				Assegna nuova posizione
			*/
			Riga = vertice.first;
			Colonna = vertice.second;

			/*
				Quando raggiunge il corner del quadrante imposta 
				l'altra quadrante come nuova meta
			*/
			if (Riga == celle_disponibili.at(0).first &&  Colonna == celle_disponibili.at(0).second)
				cornerDaRaggiungere++;
			else if (Riga == celle_disponibili.at(celle_disponibili.size() - 1).first &&  Colonna == celle_disponibili.at(celle_disponibili.size() - 1).second)
				cornerDaRaggiungere--;
		}
		/*
			Se il pacman non è superuser, è il nemico che deve inseguire il 
			pacman all'interno del suo quadrante
		*/
		else 
		{	
			/*
				Nel caso in cui c'è la collisione del nemico ed il pacman, 
				il nemico può mangiare l'utente e viene richiamato il metodo 
				per decrementare la vita dell'utente di una unità ed il metodo
				per assegnare la nuova posizione dell'utente all'interno della 
				struttura del gioco quando riprende a giocare dopo la sua morte. 
				Ossia, dopo la morte riprende a giocare nel corner più lontano 
				rispetto alla posizione attuale del nemico.
			*/
			if (puoMangiare)
			{
				puoMangiare = false;
				GestoreUtente::Instance()->decreaseLife();
				GestoreUtente::Instance()->AssegnaPosizioneDopoMorte(celle_disponibili.at(0), celle_disponibili.at(celle_disponibili.size() - 1));
			}

			/*
				Calcola Prox movimento del nemico che insegue il pacman
			*/
			Esito = t->CalcolaProxMovimento(strutturaGioco, int(Riga), int(Colonna), RigaUtente, ColonnaUtente, false, vertice);
			delete t;

			/*
				Se non può muoversi
			*/
			if (!Esito)
				return; 

			/*
				Aggiorna posizione del nemico
			*/
			Riga = vertice.first;
			Colonna = vertice.second;
		}
	}
	/*
		Se il pacman non si trova all'interno del quadrante del nemico, il nemico si muove casualmente all'interno
		del suo Quadrante
	*/
	else
		checkPosizione(strutturaGioco);
	
	glutPostRedisplay();
}

/*
	Sceglie un punto all'interno del suo quadrante, e salva il percorso all'interno di un vector.
	Fin tanto non si è raggiunta la destinazione, ovviamente non ricalcola la posizione.
*/
void Nemico::checkPosizione(vector<string> strutturaGioco)
{
	/*
		Se è la prima volta
	*/
	if (contaNumeroMov == listaVertici.size())
	{
		/*
			Trova una posizione random all'interno del suo quadrante
		*/
		int x = RandomValueUniform::Istance()->getRandomValue(0, celle_disponibili.size() - 1);
		TrovaPercorso* t = new TrovaPercorso();

		/*
			Calcola percorso e lo salva il listaVertici
		*/
		bool esito = t->CalcolaListaMovimenti(strutturaGioco, int(Riga), int(Colonna), celle_disponibili.at(x).first, celle_disponibili.at(x).second, false, listaVertici);
		delete t;

		if (esito)
			contaNumeroMov = 0;
		else
			contaNumeroMov = listaVertici.size();
	}
	/*
		Fin quando non è stato raggiunto l'ultimo movimento contenuto 
		all'interno del vector, assegna la nuova posizione
	*/
	if (contaNumeroMov != listaVertici.size())
	{
		Riga = listaVertici.at(contaNumeroMov).first;
		Colonna = listaVertici.at(contaNumeroMov).second;
		contaNumeroMov++;

		/*
			Quando ha raggiunto la cella di destinazione ed è morto in quel momento,
			non è più morto e ritorna vivo (e quindi può collidere con il pacman)
			aggiornando la variabile sonoMorto a false.
		*/
		if (sonoMorto && contaNumeroMov == listaVertici.size()) 
		{
			sonoMorto = false;
			contaNumeroMov = listaVertici.size();
		}
	}
}

void Nemico::Draw()
{
	float Tmp = 0.0;

	if (Nemico_Modello3D)
	{
		Tmp = scene_max.x - scene_min.x;
		Tmp = aisgl_max(scene_max.y - scene_min.y, Tmp);
		Tmp = aisgl_max(scene_max.z - scene_min.z, Tmp);

		Tmp = 1.4f / Tmp;

		glScalef(Tmp, Tmp, Tmp);

		if (sonoMorto == true)
			glCallList(Nemico_Morto_Lista_Draw);
		else
			glCallList(Nemico_Lista_Draw);
	}
}