#include "RegoleGame.h"

RegoleGame* RegoleGame::Istanza;
int RegoleGame::msTimeGame = 0;

RegoleGame* RegoleGame::Istance()
{
	if (Istanza == nullptr)
		Istanza = new RegoleGame();
	
	return Istanza;
}

RegoleGame::RegoleGame()
{
	Bonus_DaCreare = 0;
	TempoResiduoNuovoBonus = 0;

	/*
		Creazione Iniziale dei Protopiti di Bonus che verranno
		utilizzati successivamente come "Stampo" per la realizzazione di
		nuovi Bonus durante l'esecuzione della Partita.
	*/
	PrototipoBonus.push_back(new Bonus(pathOrange, 5));
	PrototipoBonus.push_back(new Bonus(pathBanana, 10));
	PrototipoBonus.push_back(new Bonus(pathApple, 15));
	PrototipoBonus.push_back(new Bonus(pathPeperoncino, 20));
	PrototipoBonus.at(3)->enableEatOtherGhosts();
}

void RegoleGame::InitTimeGame()
{
	msTimeGame = 0;

	glutTimerFunc(1000, RegoleGame::TimerGame, 1000);
}

void RegoleGame::ResumeTimeGame()
{
	if (!activeTimeGame)
	{
		activeTimeGame = true;
		glutTimerFunc(1000, RegoleGame::TimerGame, 1000);
	}
}

void RegoleGame::PauseTimeGame()
{
	activeTimeGame = false;
}

/*
	Il metodo TimerGame si occupa della gestione del Cronometro della Partita.
	Per ridurre al minimo l'overhead dell'intera applicazione, si è pensato 
	di associare degli eventi a questo cronometro, come ad esempio la creazione
	di nuovi Bonus quando l'utente nè mangia qualcuno durante la sua partita.
*/
void RegoleGame::TimerGame(int v)
{
	if (RegoleGame::Istance()->activeTimeGame)
	{
		msTimeGame += v;

		/*
			Viene verificato se ci sono Nuovi eventuali Bonus da creare
		*/
		if (RegoleGame::Istance()->Bonus_DaCreare > 0)
		{
			/*
				Si verifica se il Tempo necessario per la Creazione di Nuovi
				Bonus è scaduto
			*/
			RegoleGame::Istance()->TempoResiduoNuovoBonus -= v;

			if (RegoleGame::Istance()->TempoResiduoNuovoBonus <= 0)
				RegoleGame::Istance()->CreateNewBonus();
		}

		glutTimerFunc(v, RegoleGame::TimerGame, v);

		glutPostRedisplay();
	}
}

int RegoleGame::getSecondGame()
{
	return msTimeGame / 1000;
}

void RegoleGame::setActiveTimerGame(bool status)
{
	activeTimeGame = status;
}

/*
	Il Metodo InitRegoleGame si occupa della Inizializzazione di tutte le Regole
	del Gioco e di caricare un memoria una propria Struttura del Gioco Locale,
	in modo da ridurre al minimo problemi di concorrenza con la stessa
	Struttura del Gioco presente in altre Classi.
*/
void RegoleGame::InitRegoleGame()
{
	msTimeGame = 0;
	vector<string> StrutturaGioco = GestoreScena::Instance()->GetStruttuaGioco();

	CelleLibere.clear();
	TabellaHash_Coin.clear();
	TabellaHash_Bonus.clear();

	for (int rig = 0; rig < StrutturaGioco.size(); rig++)
	{
		for (int col = 0; col < StrutturaGioco[rig].size(); col++)
		{
			/*
				Dove vi sono Celle Libere, viene creato un Coin.
				Tutti i Coin sono contenuti in una Tabella Hash con l'obiettivo
				di ridurre al minimo Tempi di Ricerca ed Accesso nella struttura
				dati che contiene l'insieme di tutti i Coin presenti nel Gioco.
			*/
			if (StrutturaGioco[rig][col] == '0')
			{
				/*
					La Chiave di Hashing risulta essere composta da:
						Riga-Colonna
				*/
				string Chiave = to_string(rig);
				Chiave += "-" + to_string(col);

				CelleLibere_Iniziali.push_back(pair<int, int>(rig, col));
				Coin* coin = new Coin(rig, col);

				TabellaHash_Coin.insert(pair<string, Coin*>(Chiave, coin));
			}
			/*
				Se non risulta essere nè una Cella Libera e nè un Ostacolo, si
				verifica se risulta essere un Bonus da inserire inizialmente nel
				Gioco.
			*/
			else if(StrutturaGioco[rig][col] != '1')
				VerifyNewBonus(StrutturaGioco[rig][col], rig, col);
		}
	}
}

/*
	Il metodo VerifyNewBonus verifica se nella Cella attualmente analizzata
	del Gioco inizialmente, vi sia la presenza di uno Specifico Bonus.
	In caso positivo, il metodo gestisce la sua creazione e l'inserimento
	nella Tabella Hash contenente tutti gli altri Bonus presenti nel
	VideoGame.
*/
void RegoleGame::VerifyNewBonus(char Tipo_Bonus, int rig, int col)
{
	Bonus* newbonus = nullptr;
	
	switch (Tipo_Bonus)
	{
		case 'A':	newbonus = new Bonus(*PrototipoBonus.at(0));
					break;
		case 'B':	newbonus = new Bonus(*PrototipoBonus.at(1));
					break;
		case 'C':	newbonus = new Bonus(*PrototipoBonus.at(2));
					break;
		case 'D':	newbonus = new Bonus(*PrototipoBonus.at(3));
					break;
	}
		
	if (newbonus != nullptr)
	{
		newbonus->setPosition(rig, col);

		/*
			La Chiave di Hashing risulta essere composta da:
				Riga-Colonna
		*/
		string Chiave = to_string(rig);
		Chiave += "-" + to_string(col);

		TabellaHash_Bonus.insert(pair<string, Bonus*>(Chiave, newbonus));
	}
}

/*
	Il metodo CreateNewBonus gestisce la Creazione di un Nuovo Bonus durante
	l'esecuzione del Gioco.
	La Creazione di uno specifico Bonus è fatta secondo una Distribuzione di 
	Probabilità non Uniforme, opportunamente realizzata secondo le potenzialità
	di uno specifico Bonus.
*/
void RegoleGame::CreateNewBonus()
{
	Bonus* newBonus = nullptr;
	float Prob_Bonus = ((double)rand() / (RAND_MAX));

	if(Prob_Bonus < 0.5)
		newBonus = new Bonus(*PrototipoBonus.at(0));
	else if( Prob_Bonus > 0.5 && Prob_Bonus < 0.75)
		newBonus = new Bonus(*PrototipoBonus.at(1));
	else if (Prob_Bonus > 0.75 && Prob_Bonus < 0.90)
		newBonus = new Bonus(*PrototipoBonus.at(2));
	else
		newBonus = new Bonus(*PrototipoBonus.at(3));

	try
	{
		/*
			Il Nuovo Bonus Creato verrà inserito in una delle Celle Disponibili
			del VideoGame (si intede celle attualmente non occupate da nessun Coin
			e Bonus) seguendo una Distribuzione di Probabilità Uniforme nella sua
			selezione.
		*/
		int PosCella = RandomValueUniform::Istance()->getRandomValue(0, CelleLibere.size() - 1);

		newBonus->setPosition(CelleLibere.at(PosCella).first, CelleLibere.at(PosCella).second);
		CelleLibere.erase(CelleLibere.begin() + PosCella);

		string Chiave = to_string(CelleLibere.at(PosCella).first);
		Chiave += "-" + to_string(CelleLibere.at(PosCella).second);

		TabellaHash_Bonus.insert(pair<string, Bonus*>(Chiave, newBonus));
		Bonus::setBonusEffect(newBonus);
		Bonus::StartTimeEffect(100);

		Bonus_DaCreare -= 1;

		/*
			Nel caso in cui vi fossero ancora dei Bonus da Creare, viene
			resettato il Tempo Residuo per la loro Creazione.
		*/
		if (Bonus_DaCreare > 0)
			TempoResiduoNuovoBonus = 5000;
	}
	catch(...)
	{
		cout << "Ripetizione Creazione Bonus";
	}
}

void RegoleGame::Draw_Bonus_Coin()
{
	for (auto it = TabellaHash_Coin.begin(); it != TabellaHash_Coin.end(); ++it)
		it->second->Draw();
}

void RegoleGame::Draw_Bonus_Bonus()
{
	for (auto it = TabellaHash_Bonus.begin(); it != TabellaHash_Bonus.end(); ++it)
		it->second->Draw();
}


/*
	Il metodo Check_Collision verifica durante lo spostamento del PacMan nella
	Scena 3D, se la sua nuova Posizione occupata contiene un Bonus oppure un Coin.
	In caso positivo, quest'ultimo viene mangiato ed incrementato il Punteggio
	dell'utente.
*/
void RegoleGame::Check_Collision(int Riga_Utente, int Colonna_Utente)
{
	string Chiave;
	Chiave = to_string(Riga_Utente);
	Chiave += "-" + to_string(Colonna_Utente);

	unordered_map<string, Coin*>::iterator it_coin = TabellaHash_Coin.find(Chiave);

	/*
		Viene verificato se la Nuova cella del PacMan era occupata da un Coin
	*/
	if ( it_coin != TabellaHash_Coin.end())
	{
		/*
			Viene aumentato il suo Punteggio
		*/
		GestoreUtente::Instance()->IncreasePunteggio(1);

		/*
			Conversione della Chiave di hashing nei valori interi di 
			Riga e Colonna.
		*/
		string Chiave_cella = it_coin->first;

		replace(Chiave_cella.begin(), Chiave_cella.end(), '-', ' ');
		vector<int> array;
		stringstream ss(Chiave_cella);
		int temp;
		while (ss >> temp)
			array.push_back(temp);

		int rig = array.at(0);
		int col = array.at(1);

		/*
			Viene inserita una Nuova Cella Libera a disposizione del Gioco
		*/
		CelleLibere.push_back(pair<int, int>(rig, col));

		/*
			Eliminato il Coin nella Tabella Hash
		*/
		TabellaHash_Coin.erase(Chiave);

		/*
			Nel caso in cui fossero finiti tutti i Coin nella Scena,
			viene richiamato il metodo RefillCoin che si occupa del nuovo
			riempimento del Gioco di nuovi Coin
		*/
		if (TabellaHash_Coin.size() == 0)
			RefillCoin();
	}

	/*
		Viene verificato se la Nuova cella del PacMan era occupata da un Bonus
	*/
	else
	{
		unordered_map<string, Bonus*>::iterator it = TabellaHash_Bonus.find(Chiave);
		if (it != TabellaHash_Bonus.end())
		{
			/*
				Viene aumentato il suo Punteggio
			*/
			GestoreUtente::Instance()->IncreasePunteggio(it->second->getValore());

			/*
				Se il Bonus mangiato dal PacMan risulta essere il "Peperoncino",
				allora viene attivata la funzionalità di "SuperUser" all'Utente.
			*/
			if (it->second->checkEatOtherGhosts())
				GestoreUtente::Instance()->enableSuperUser();
			else
				GestoreAudio::Instance()->PlaySoundEatBonus();
		
			/*
				Conversione della Chiave di hashing nei valori interi di
				Riga e Colonna.
			*/
			string Chiave_cella = it->first;

			replace(Chiave_cella.begin(), Chiave_cella.end(), '-', ' ');
			vector<int> array;
			stringstream ss(Chiave_cella);
			int temp;
			while (ss >> temp)
				array.push_back(temp);

			int rig = array.at(0);
			int col = array.at(1);

			/*
				Viene inserita una Nuova Cella Libera a disposizione del Gioco
			*/
			CelleLibere.push_back(pair<int, int>(rig, col));

			/*
				Eliminato il Bonus nella Tabella Hash
			*/
			TabellaHash_Bonus.erase(Chiave);

			/*
				Se risulta essere il Primo Bonus che viene mangiato dall'Utente,
				viene attivato il Tempo Residuo per la Creazione di Nuovi Bonus 
				durante la Partita.
			*/
			if (Bonus_DaCreare == 0)
				TempoResiduoNuovoBonus = 5000;

			Bonus_DaCreare++;
		}
	}
}

void RegoleGame::RefillCoin()
{
	for (int i = 0; i < CelleLibere_Iniziali.size(); i++)
	{
		int Riga = CelleLibere_Iniziali.at(i).first;
		int Colonna = CelleLibere_Iniziali.at(i).second;
		
		string Chiave = to_string(Riga);
		Chiave += "-" + to_string(Colonna);

		unordered_map<string, Bonus*>::iterator it_coin = TabellaHash_Bonus.find(Chiave);

		if (it_coin == TabellaHash_Bonus.end())
		{
			Coin* coin = new Coin(Riga, Colonna);

			TabellaHash_Coin.insert(pair<string, Coin*>(Chiave, coin));
		}
	}
}
