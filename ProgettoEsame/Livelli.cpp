#include "Livelli.h"
#include "MenuGame.h"

Livelli* Livelli::Istanza = nullptr;
const char* Livelli::pathStrutturaGioco[3] = { "StrutturaGioco/Level1.txt","StrutturaGioco/Level2.txt" ,"StrutturaGioco/Level3.txt" };
const char* Livelli::pathNemico[5] = { "Modelli/Nemico_Verde.obj", "Modelli/Nemico_Rosso.obj", "Modelli/Nemico_Fucsia.obj","Modelli/Nemico_Blu.obj","Modelli/Nemico_Bianco.obj" };
const char* Livelli::pathScoreFile = "GameScore.csv";
const char* Livelli::pathBestScoreFile = "BestGameScore.csv";

vector<Nemico*> Livelli::ListaNemici;
int Livelli::TempoInzialeAttesa = 0;

vector<Livelli::ScoreUser> Livelli::ListaScore;
ifstream Livelli::LetturaBestScore;
ofstream Livelli::ScritturaBestScore;

Livelli* Livelli::Instance()
{
	if (Istanza == nullptr)
		Istanza = new Livelli;
	
	return Istanza;
}

Livelli::Livelli()
{
	ResetGame();
	ListaNemici.push_back(new Nemico(0, 0, pathNemico[0], pathNemico[4]));
	ListaNemici.push_back(new Nemico(0, 0, pathNemico[1], pathNemico[4]));
	ListaNemici.push_back(new Nemico(0, 0, pathNemico[2], pathNemico[4]));
	ListaNemici.push_back(new Nemico(0, 0, pathNemico[3], pathNemico[4]));
};

/*
	Il metodo ResetGame fornisce la possibilità di Resettare tutte le Impostazioni,
	Punteggi e Caratteristiche associate all'Utente prima dell'esecuzione di una 
	Nuova Partita.
*/
void Livelli::ResetGame()
{
	FirstTime = true;
	TempoInzialeAttesa = 8000;

	GestoreScena::Instance()->InitScena(pathStrutturaGioco[Livello]);

	RichiestaGUI::Init_ArrayElem3D();

	CaricamentoBestScore();

	GestoreUtente::Instance()->ResetPlayer();
	GestoreNemici::Instance()->ResetNemici();

	GestoreAudio::Instance()->StopAllSounds();
}

/*
	Il Metodo SelezionaLivello permette all'utente di selezionare uno
	dei tre livelli di diffoltà previsti nel VideoGioco.
	I 3 Livelli di difficoltà si differenziano per la Velocità d'inseguimento
	dei Nemici e per il Numero di Bonus presenti all'interno della Struttura
	del Gioco.
	Nello specifico:
		-	Livello Facile: Velocità Nemici = 140; Numero Bonus = 12
		-	Livello Normale: Velocità Nemici = 100; Numero Bonus = 10
		-	Livello Facile: Velocità Nemici = 70; Numero Bonus = 8
*/
void Livelli::SelezionaLivello(int level)
{
	if (level != 1 || level != 2 || level != 3)
		Livello = 1;
	else
		Livello = level;

	ResetGame();

	/*
		Settaggio Impostazioni e Caratteristiche Utente
	*/
	SetPositionPacMan();
	RichiestaGUI::PushElem3D(GestoreUtente::Instance()); 

	/*
		Settaggio Impostazioni e Caratteristiche Nemici
	*/
	SetPositionNemici();

	RichiestaGUI::PushElem3D(ListaNemici.at(0));
	RichiestaGUI::PushElem3D(ListaNemici.at(1));
	RichiestaGUI::PushElem3D(ListaNemici.at(2));
	RichiestaGUI::PushElem3D(ListaNemici.at(3));
	GestoreNemici::Instance()->PushNemico(ListaNemici.at(0));
	GestoreNemici::Instance()->PushNemico(ListaNemici.at(1));
	GestoreNemici::Instance()->PushNemico(ListaNemici.at(2));
	GestoreNemici::Instance()->PushNemico(ListaNemici.at(3));

	GestoreNemici::Instance()->AssegnaQuadrante();

	/*
		Settaggio Impostazioni, Regole e Grafica del Gioco
	*/
	RegoleGame::Istance()->InitRegoleGame();
	GestoreMovimento::Instance()->CaricaStrutturaGioco(pathStrutturaGioco[Livello]);
}

int Livelli::getLivelloAttuale()
{
	return Livello;
}

void Livelli::StartGame()
{
	RichiestaGUI::setStatusCountDown(true);
	GestoreAudio::Instance()->PlaySoundCountDown();
	CountDownIniziale(1000);
	glutDisplayFunc(CallBack::Display_Gioco);
}

/*
	Il Metodo PlayGame fornisce la possibilitàà di avviare la Partita.
	Viene verificato inizialmente se la partita non risulta essere già
	in corso, in caso positivo non è possibile eseguire le funzionalità 
	previste da questo metodo.
*/
void Livelli::PlayGame()
{
	if (FirstTime)
	{
		SetVelocitaMovimenti();
		FirstTime = false;
		GestoreAudio::Instance()->StartSoundGame();
	}
}

void Livelli::SetPositionPacMan()
{
	if(Livello == 1)
		GestoreUtente::Instance()->setPosition(29, 5);
	else if(Livello == 2)
		GestoreUtente::Instance()->setPosition(29, 5);
	else
		GestoreUtente::Instance()->setPosition(29, 5);
}

void Livelli::SetPositionNemici()
{
	if (Livello == 1 | Livello == 2 | Livello == 3)
	{
		ListaNemici.at(0)->setRiga(19);
		ListaNemici.at(0)->setColonna(37);

		ListaNemici.at(1)->setRiga(20);
		ListaNemici.at(1)->setColonna(37);

		ListaNemici.at(2)->setRiga(19);
		ListaNemici.at(2)->setColonna(39);

		ListaNemici.at(3)->setRiga(20);
		ListaNemici.at(3)->setColonna(39);
	}
}

/*
	Il metodo SetVelocitàMovimenti verifica in base al Livello di 
	difficoltà selezionato dall'operatore nel Menù Iniziale, impostando
	una differente velocità di spostamento dei Nemici e solo successivamente
	di avviare il Cronometro della Partita.
*/
void Livelli::SetVelocitaMovimenti()
{
	if (FirstTime)
	{
		/*
			Settaggio Velocità Spostamento Utente
		*/
		ControllerPlayer::Instance()->setTime(60);

		/*
			Settaggio Velocità Spostamento Nemici
		*/
		if (Livello == 1)
			GestoreNemici::Instance()->setTime(140);
		else if (Livello == 2)
			GestoreNemici::Instance()->setTime(100);
		else if (Livello == 3)
			GestoreNemici::Instance()->setTime(80);

		/*
			Avvio Movimento Utente e Nemici.
			Avvio Cronometro Partita
		*/
		ControllerPlayer::Init_Controller(0);
		GestoreNemici::Instance()->Init_MovimentoNemico(0);
		RegoleGame::InitTimeGame();
	}
}

/*
	Il metodo Resume_Pause_Game fornisce la possibilità di mettere il
	Pausa il Gioco durante una Partita.
	Nel caso in cui la partita fosse già nello Stato di Pausa, allora richiama 
	una serie metodi relativi alla gestione del CountDown per garantire 
	la ripresa del Gioco all'utente. 
*/
void Livelli::Resume_Pause_Game()
{
	/*
		Se non è ancora ancora attivo il CountDown per la ripresa 
		del Gioco, non è possibile riattivare lo stato di Pausa.
	*/
	if (!changeStatusPause)
		return;

	/*
		Quando viene richiesto di impostare il Gioco nello stato di Pausa.
	*/
	if (!Pause)
	{
		/*
			Viene disabilitato il Cronometro della Partita e tutti i
			movimenti degli Oggetti Dinamici 3D presenti nel Videogioco.
		*/
		RegoleGame::Istance()->PauseTimeGame();
		ControllerPlayer::Instance()->enablePause();
		GestoreNemici::Instance()->enablePause();

		Pause = true;

		GestoreAudio::Instance()->StopAllSounds();
		GestoreAudio::Instance()->PlaySoundPause();
	}

	/*
		Quando si deve riprendere una Partita che era attualmente nello stato 
		di Pausa.
	*/
	else
	{
		changeStatusPause = false;
		Pause = false;
		
		/*
			Attivazione del CountDown
		*/
		TempoInzialeAttesa = 8000;
		RichiestaGUI::setStatusCountDown(true);
		GestoreAudio::Instance()->PlaySoundCountDown();
		CountDownIniziale(1000);
	}
}

bool Livelli::StatusPauseGame()
{
	return Pause;
}

/*
	Il metodo UtentePerdeVita permette all'utente di continuare a giocare 
	se possiede ancora qualche Vita all'interno della sua Partita.
	Per fornisce una ripresa graduale del Gioco, viene attivato anche in
	questo caso un CountDown inziale.
*/
void Livelli::UtentePerdeVita()
{
	RegoleGame::Istance()->PauseTimeGame();
	GestoreAudio::Instance()->StopAllSounds();
	GestoreAudio::Instance()->PlaySoundLooseLife();

	TempoInzialeAttesa = 8000;
	RichiestaGUI::setStatusCountDown(true);

	GestoreAudio::Instance()->PlaySoundCountDown();
	CountDownIniziale(1000);
}

/*
	Il metodo GameOver viene richiamato nel momento in cui l'utente
	perde tutte le vite concesse all'interno di una partita.
	Prima di ritornare al menù principale, viene salvato su un File di Log
	il Nome,il Punteggio e lo Score ottenuto e verificato eventualmente se 
	risulta essere uno dei 5 migliori Punteggi ottenuti per questo videogioco.
*/
void Livelli::GameOver()
{
	GestoreAudio::Instance()->StopAllSounds();
	GestoreAudio::Instance()->PlaySoundGameOver();

	/*
		Salvataggio Partita e verifica se risulta essere uno dei 5 miglior
		Punteggi.
	*/
	ScritturaScoreOnFile();
	RegoleGame::Istance()->setActiveTimerGame(false);
	MenuGame::Instance()->setMenuAttivo(true);
}

/*
	Il Metodo ScritturaScoreOnFile fornisce la funzionalità di scrivere in 
	modalità Append sul File di Log del videogioco, la Partita appena effettuata
	dall'Utente.
*/
void Livelli::ScritturaScoreOnFile()
{
	FileGameScore.open(pathScoreFile, std::ofstream::out | std::ofstream::app);

	string RecordScoreUser = "";

	RecordScoreUser += GestoreUtente::Instance()->getNomeUtente() + ";";
	RecordScoreUser += to_string(GestoreUtente::Instance()->getPunteggio()) + ";";
	RecordScoreUser += to_string(RegoleGame::Istance()->getSecondGame()) + "\n";

	FileGameScore << RecordScoreUser;
	
	/*
		Verifica se questa partita risulta essere una delle 5 migliori effettuate
		per questo videogioco.
	*/
	checkTop5BestScore(GestoreUtente::Instance()->getNomeUtente(), GestoreUtente::Instance()->getPunteggio(), RegoleGame::Istance()->getSecondGame(), RecordScoreUser);

	FileGameScore.close();
}

/*
	Il metodo checkTop5BestScore controlla se questa partita appena eseguita da
	un Utente, risulta essere migliore di una delle 5 Partite Migliori 
	precedentemente ottenute.
	Le 5 Partite Migliori sono salvate in un differente File rispetto a quello di 
	Log in quanto si sono ottimizzate le operazioni di Ordimento e Lettura sul File
	di tutte le Partite eseguite.
*/
void Livelli::checkTop5BestScore(string NomeUtente, int Punteggio, int secondiGame, string Completa)
{
	/*
		Si verifica prima se eventualmente il File non risulta essere 
		già stato creato.
	*/
	LetturaBestScore.open(pathBestScoreFile, std::ofstream::in);

	/*
		Se non risulta essere già Create, allora viene inserita direttamente
		questa partita come la migliore dato che è la Prima partita effettuata.
	*/
	if (!LetturaBestScore.is_open())
	{
		ScritturaBestScore.open(pathBestScoreFile, std::ofstream::out | std::ofstream::app);
		ScritturaBestScore << Completa;
		LetturaBestScore.close();
		ScritturaBestScore.close();

		CaricamentoBestScore();
	}

	/*
		Nel caso di esistenza invece del File
	*/
	else
	{
		/*
			Vengono Caricate tutte le Migliori Partite Precedenti
		*/
		CaricamentoBestScore();

		/*
			Inserita nella ListaScore anche l'ultima partita appena giocata
			e successivamente ordinto questo Vettore
		*/
		struct ScoreUser DaInserire;
		DaInserire.User = NomeUtente;
		DaInserire.Punteggio = Punteggio;
		DaInserire.Tempo_Sec = secondiGame;

		ListaScore.push_back(DaInserire);

		sort(ListaScore.begin(), ListaScore.end(), greater<ScoreUser>());

		/*
			Sul File dei migliori Punteggi, si vogliono memorizzare solo
			le prime 5 migliori Partite
		*/
		int size = 5;
		if (ListaScore.size() < 5)
			size = ListaScore.size();
		
		/*
			Si effettua la Cancellazione del File dell 5 migliori Partite
		*/
		if (remove(pathBestScoreFile) != 0)
			cout << "Errore Cancellazione File Best Score" << endl;

		/*
			Si effettua la sua nuova Scrittura con le 5 migliori partire aggiornate.
		*/
		ScritturaBestScore.open(pathBestScoreFile, std::ofstream::out | std::ofstream::app);
		LetturaBestScore.open(pathBestScoreFile, std::ofstream::in);

		for (int i = 0; i < size; i++)
		{
			string RecordScoreUser = "";

			RecordScoreUser += ListaScore.at(i).User + ";";
			RecordScoreUser += to_string(ListaScore.at(i).Punteggio) + ";";
			RecordScoreUser += to_string(ListaScore.at(i).Tempo_Sec) + "\n";

			ScritturaBestScore << RecordScoreUser;
		}

		ScritturaBestScore.close();
	}
}

/*
	Il metodo CaricamentoBestScore si occupa del caricamento delle
	5 migliori partite attualmente memorizzate sul File "BestGameScore.csv"
*/
void Livelli::CaricamentoBestScore()
{
	ListaScore.clear();

	LetturaBestScore.close();
	LetturaBestScore.open(pathBestScoreFile, std::ofstream::in);
	/*
		Il File Best Score contiene al più la Top 5 dei migliori punteggi
	*/
	if (LetturaBestScore.is_open())
	{
		string RecordAttuale;

		while (getline(LetturaBestScore, RecordAttuale))
		{
			int Pos = RecordAttuale.find(";");
			struct ScoreUser Attuale;

			Attuale.User = RecordAttuale.substr(0, Pos);

			string Tmp = RecordAttuale.substr(Pos + 1);
			Pos = Tmp.find(";");

			Attuale.Punteggio = stoi(Tmp.substr(0, Pos));
			Attuale.Tempo_Sec = stoi(Tmp.substr(Pos + 1));

			ListaScore.push_back(Attuale);
		}

		LetturaBestScore.close();
	}
}

vector<Livelli::ScoreUser> Livelli::getListaBestScore()
{
	if (ListaScore.size() == 0)
		CaricamentoBestScore();

	return ListaScore;
}

void Livelli::CountDownIniziale(int v = 1000)
{
	/*
		Nel momento in cui il CountDown non risulta essere Scaduto
	*/
	if (TempoInzialeAttesa > 0)
	{
		TempoInzialeAttesa -= v;
		RichiestaGUI::setTempoIniziale(TempoInzialeAttesa);

		glutTimerFunc(v, Livelli::CountDownIniziale, v);
	}

	/*
		Se il CountDown risulta essere Scaduto
	*/
	else
	{
		RichiestaGUI::setStatusCountDown(false);

		/*
			Nel caso in cui dobbiamo avviare il Gioco per la Prima volta.
		*/
		if(Livelli::Instance()->FirstTime)
			Livelli::Instance()->PlayGame();

		/*
			Nel caso in cui il gioco era nello Stato di Pausa
		*/
		if (!Livelli::Instance()->changeStatusPause)
		{
			RegoleGame::Istance()->ResumeTimeGame();
			ControllerPlayer::Instance()->disablePause();
			GestoreNemici::Instance()->disablePause();

			Livelli::Instance()->changeStatusPause = true;
			GestoreAudio::Instance()->StartSoundGame();
		}

		/*
			Nel caso in cui l'Utente era Morto ed avevo perso una Vita
		*/
		if (GestoreUtente::Instance()->getUtenteMorto())
		{
			RegoleGame::Istance()->ResumeTimeGame();
			GestoreUtente::Instance()->setUtenteMorto(false);

			ControllerPlayer::Init_Controller(0);
			GestoreNemici::Instance()->Init_MovimentoNemico(0);
			GestoreAudio::Instance()->StartSoundGame();
		}
	}
}
