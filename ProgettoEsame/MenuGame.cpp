#include "MenuGame.h"
#include "GestoreTexture.h"
#include "RichiestaGUI.h"
#include "Livelli.h"
#include "GestoreAudio.h"

MenuGame* MenuGame::Istanza = nullptr;

MenuGame* MenuGame::Instance()
{
	if (Istanza == nullptr) 
		Istanza = new MenuGame;
	
	return Istanza;
}

/*
	Nel costruttore vengono inizializzate le modalità predefinite
	del sotto Menù impostazioni, ovvero :
	- livello di difficoltà NORMALE
	- Risoluzione 800x600
	- Audio abilitato
*/
MenuGame::MenuGame()
{	
	gestoreImpostazioni = new GestoreImpostazioni(make_pair(800, 600), true, 1);
}

bool MenuGame::getMenuAttivo()
{
	return menuAttivo;
}

GestoreImpostazioni* MenuGame::getGestoreImp()
{
	return gestoreImpostazioni;
}

void MenuGame::setMenuAttivo(bool status)
{
	menuAttivo = status;

	/*
		Se il Menù è attivo, inizializza tutte le impostazioni del menù,
		attiva l'audio del Menù ed attiva le callback glutMouseFunc
		e glutPassiveMotionFunc.
	*/
	if (menuAttivo) 
	{
		InitMenu();
		GestoreAudio::Instance()->StopAllSounds();
		GestoreAudio::Instance()->StartSoundMenu();
		glutMouseFunc(Instance()->MouseHandler);
		glutPassiveMotionFunc(Instance()->MousePassiveHandler);
	}

	/*
		Se il Menù viene disabilitato, vuol dire che sta per iniziare
		una nuova partita. Quindi, disabilita le callback glutMouseFunc
		e glutPassiveMotionFunc e setta tutte le impostazioni della partita
		(livello di difficolta, audio on/off, risoluzione schermo).
	*/
	else
	{
		MenuGame::Instance()->DisableMenuInit();
		GestoreAudio::Instance()->StopAllSounds();

		glutMouseFunc(nullptr);
		glutPassiveMotionFunc(nullptr);
		
		pair<int, int> res = Instance()->gestoreImpostazioni->getResSchermo();
		int difficolta = Instance()->gestoreImpostazioni->getDifficolta();

		/*
			Se abbiamo abilitato l'impostazione Full Screen
		*/
		if (Instance()->gestoreImpostazioni->getFullScreen()) 
			glutFullScreen();
		else 
			glutReshapeWindow(res.first, res.second);

		/*
			Impostazione della ViewPort secondo le impostazioni settate dall'Utente
		*/
		glViewport(0, 0, res.first, res.second);

		/*
			Viene effettuata la Richiesta del Nome Utente che vuole
			giocare alla partita
		*/
		string NomeUtente;
		cout << "Inserisci il tuo Nome e Cognome: ";
		getline(cin, NomeUtente);

		GestoreUtente::Instance()->setNomeUtente(NomeUtente);

		cout << endl << "Inserimento avvenuto con Successo... ATTENDERE PREGO!"<<endl;

		/*
			Settaggio della Difficoltà scelta dall'Utente
		*/
		Livelli::Instance()->SelezionaLivello(difficolta); 

		/*
			Inizio del Gioco
		*/
		Livelli::Instance()->StartGame();
		RegoleGame::Istance()->setActiveTimerGame(true);
	}
}

/*
	Metodo che prende in input le coordinate GLUT e ritorna le
	relative coordinate OpenGL.
*/
void MenuGame::CaricaTexture()
{
	/*
		Parametri del costruttore di ElemTexture :
		1) Colonna del punto in basso a destra del poligono
		2) Riga del punto in basso a destra del poligono
		3) Z del punto in basso a destra del poligono
		4) Offset Larghezza del poligono
		5) offset Altezza del poligono
		6) texture dell'elemento

		Il settimo parametro varia..
		-> Ci sono due costruttori, vediamo i due casi :
		1) Se l'oggetto è selezionabile e quindi prevede
		   una seconda texture quando viene selezionato,
		   inserire come settimo parametro texture_IDselected
		2) Se l'oggetto non è selezionabile (come ad esempio la texture di background
		   oppure una label text),richiama
		   secondo costruttore inserendo true come settimo
		   parametro.
		   Se l'oggetto non prevede un cambio texture quando si passa il mouse sopra
		   ma prevede di essere cliccato, passare false
		   (come ad esempio il tasto back nel menu impostazione)
	*/

	unsigned int texture_ID;			//texture principale dell'elemento
	unsigned int texture_IDselected;	//texture da appliccare quando l'elemento è selezionato

	//------------------------------------ Texture Menù Principale -----------------------------------
	// Texture Background
	texture_ID = GestoreTexture::CaricaTexture(pathTextureBG);

	if (FirstTime)
		TableHash.insert(make_pair("TextureBG", new ElemTexture(100, 0, 0, -100, 100, texture_ID, true)));
	else
		TableHash.find("TextureBG")->second->setTexture(texture_ID);

	// Texture PlayGame
	texture_ID = GestoreTexture::CaricaTexture(pathTexturePlayGameButton);
	texture_IDselected = GestoreTexture::CaricaTexture(pathTexturePlayGameSelectedButton);

	if(FirstTime)
		TableHash.insert(make_pair("TexturePlay", new ElemTexture(70, 40, 0, -40, 10, texture_ID, texture_IDselected)));
	else
	{
		TableHash.find("TexturePlay")->second->setTexture(texture_ID);
		TableHash.find("TexturePlay")->second->setTextureIDselected(texture_IDselected);
	}

	// Texture Settings
	texture_ID = GestoreTexture::CaricaTexture(pathTextureSettingsButton);
	texture_IDselected = GestoreTexture::CaricaTexture(pathTextureSettingsSelectedButton);

	if(FirstTime)
		TableHash.insert(make_pair("TextureSettings", new ElemTexture(50, 25, 0, -35, 10, texture_ID, texture_IDselected)));
	else
	{
		TableHash.find("TextureSettings")->second->setTexture(texture_ID);
		TableHash.find("TextureSettings")->second->setTextureIDselected(texture_IDselected);
	}

	// Texture Best Score
	texture_ID = GestoreTexture::CaricaTexture(pathTextureBestScoreButton);
	texture_IDselected = GestoreTexture::CaricaTexture(pathTextureBestScoreSelectedButton);

	if(FirstTime)
		TableHash.insert(make_pair("TextureBestScore", new ElemTexture(90, 25, 0, -35, 10, texture_ID, texture_IDselected)));
	else
	{
		TableHash.find("TextureBestScore")->second->setTexture(texture_ID);
		TableHash.find("TextureBestScore")->second->setTextureIDselected(texture_IDselected);
	}

	// Texture Quit
	texture_ID = GestoreTexture::CaricaTexture(pathTextureQuitGameButton);
	texture_IDselected = GestoreTexture::CaricaTexture(pathTextureQuitGameSelectedButton);

	if(FirstTime)
		TableHash.insert(make_pair("TextureQuitGame", new ElemTexture(70, 10, 0, -40, 10, texture_ID, texture_IDselected)));
	else
	{
		TableHash.find("TextureQuitGame")->second->setTexture(texture_ID);
		TableHash.find("TextureQuitGame")->second->setTextureIDselected(texture_IDselected);
	}

	//----------------------------------- Texture Menù Impostazioni ---------------------------------------
	// Texture Backgroung
	texture_ID = GestoreTexture::CaricaTexture(pathTextureSettingsBG);

	if(FirstTime)
		TableHashSettings.insert(make_pair("TextureSettingsBG", new ElemTexture(0, 0, 0, 100, 100, texture_ID, true)));

	// Texture Back
	texture_ID = GestoreTexture::CaricaTexture(pathTextureSettingsBackButton);

	if(FirstTime)
		TableHashSettings.insert(make_pair("TextureSettingsBack", new ElemTexture(90, 10, 0, -10, 10, texture_ID, texture_ID)));

	// Texture Livello Facile
	texture_ID = GestoreTexture::CaricaTexture(pathTextureSettingsLevelFacile);

	if(FirstTime)
		TableHashSettings.insert(make_pair("TextureSettingsLevelFacile", new ElemTexture(70, 70, 0, -20, 10, texture_ID, true)));

	// Texture Livello Normale
	texture_ID = GestoreTexture::CaricaTexture(pathTextureSettingsLevelNormale);
	
	if(FirstTime)
		TableHashSettings.insert(make_pair("TextureSettingsLevelNormale", new ElemTexture(70, 70, 0, -20, 10, texture_ID, true)));

	// Texture Livello Difficile
	texture_ID = GestoreTexture::CaricaTexture(pathTextureSettingsLevelDifficile);

	if(FirstTime)
		TableHashSettings.insert(make_pair("TextureSettingsLevelDifficile", new ElemTexture(70, 70, 0, -20, 10, texture_ID, true)));

	// Texture Frecce Sinsitra-Destra del Livello
	texture_ID = GestoreTexture::CaricaTexture(pathTextureSettingsFrecciaDestraButton);
	texture_IDselected = GestoreTexture::CaricaTexture(pathTextureSettingsFrecciaDestraSelectedButton);

	if(FirstTime)
		TableHashSettings.insert(make_pair("TextureSettingsFrecciaDestraLevel", new ElemTexture(80, 70, 0, -5, 10, texture_ID, texture_IDselected)));

	texture_ID = GestoreTexture::CaricaTexture(pathTextureSettingsFrecciaSinistraButton);
	texture_IDselected = GestoreTexture::CaricaTexture(pathTextureSettingsFrecciaSinistraSelectedButton);

	if(FirstTime)
		TableHashSettings.insert(make_pair("TextureSettingsFrecciaSinistraLevel", new ElemTexture(45, 70, 0, -5, 10, texture_ID, texture_IDselected)));

	// Texture Risoluzione 800x600
	texture_ID = GestoreTexture::CaricaTexture(pathTextureSettingsSchermo800);

	if(FirstTime)
		TableHashSettings.insert(make_pair("TextureSettingsMonitor800", new ElemTexture(70, 50, 0, -20, 10, texture_ID, true)));

	// Texture Risoluzione SchermoHD
	texture_ID = GestoreTexture::CaricaTexture(pathTextureSettingsSchermoHD);

	if(FirstTime)
		TableHashSettings.insert(make_pair("TextureSettingsMonitorHD", new ElemTexture(70, 50, 0, -20, 10, texture_ID, true)));

	// Texture Risoluzione Schermo FullHD
	texture_ID = GestoreTexture::CaricaTexture(pathTextureSettingsSchermoFullHD);

	if(FirstTime)
		TableHashSettings.insert(make_pair("TextureSettingsMonitorFullHD", new ElemTexture(70, 50, 0, -20, 10, texture_ID, true)));

	texture_ID = GestoreTexture::CaricaTexture(pathTextureSettingsSchermoFullScreen);
	TableHashSettings.insert(make_pair("TextureSettingsMonitorFullScreen", new ElemTexture(70, 50, 0, -20, 10, texture_ID, true)));

	//Texture Frecce Sinsitra-Destra del Livello della Risoluzione Schermo
	texture_ID = GestoreTexture::CaricaTexture(pathTextureSettingsFrecciaDestraButton);
	texture_IDselected = GestoreTexture::CaricaTexture(pathTextureSettingsFrecciaDestraSelectedButton);

	if(FirstTime)
		TableHashSettings.insert(make_pair("TextureSettingsFrecciaDestraRes", new ElemTexture(80, 50, 0, -5, 10, texture_ID, texture_IDselected)));

	texture_ID = GestoreTexture::CaricaTexture(pathTextureSettingsFrecciaSinistraButton);
	texture_IDselected = GestoreTexture::CaricaTexture(pathTextureSettingsFrecciaSinistraSelectedButton);

	if(FirstTime)
		TableHashSettings.insert(make_pair("TextureSettingsFrecciaSinistraRes", new ElemTexture(45, 50, 0, -5, 10, texture_ID, texture_IDselected)));

	// Texture Audio ON
	texture_ID = GestoreTexture::CaricaTexture(pathTextureSettingsAudioOn);

	if(FirstTime)
		TableHashSettings.insert(make_pair("TextureSettingsAudioOn", new ElemTexture(70, 30, 0, -20, 10, texture_ID, true)));

	// Texture Audio OFF
	texture_ID = GestoreTexture::CaricaTexture(pathTextureSettingsAudioOff);

	if(FirstTime)
		TableHashSettings.insert(make_pair("TextureSettingsAudioOff", new ElemTexture(70, 30, 0, -20, 10, texture_ID, true)));

	//Texture Frecce Sinsitra-Destra dell'Audio
	texture_ID = GestoreTexture::CaricaTexture(pathTextureSettingsFrecciaDestraButton);
	texture_IDselected = GestoreTexture::CaricaTexture(pathTextureSettingsFrecciaDestraSelectedButton);

	if(FirstTime)
		TableHashSettings.insert(make_pair("TextureSettingsFrecciaDestraAudio", new ElemTexture(80, 30, 0, -5, 10, texture_ID, texture_IDselected)));

	texture_ID = GestoreTexture::CaricaTexture(pathTextureSettingsFrecciaSinistraButton);
	texture_IDselected = GestoreTexture::CaricaTexture(pathTextureSettingsFrecciaSinistraSelectedButton);

	if(FirstTime)
		TableHashSettings.insert(make_pair("TextureSettingsFrecciaSinistraAudio", new ElemTexture(45, 30, 0, -5, 10, texture_ID, texture_IDselected)));

	//-------------------------------- Texture Menù BestScore -----------------------------------------
	// Texture Background
	texture_ID = GestoreTexture::CaricaTexture(pathTextureBestScoreBG);

	if(FirstTime)
		TableHashBestScore.insert(make_pair("TextureBestScoreBG", new ElemTexture(0, 0, 0, 100, 100, texture_ID, true)));
	
	// Texture Back
	texture_ID = GestoreTexture::CaricaTexture(pathTextureSettingsBackButton);

	if (FirstTime)
		TableHashBestScore.insert(make_pair("TextureBestScoreBack", new ElemTexture(90, 10, 0, -10, 10, texture_ID, false)));
		
	
	//---------------- Collegamento Lista Livello-Risoluzione-Audio ---------------------------------------
	/*
		N.B. Tutte le texture che in un determinato frame non devono essere visualizzate,
		vengono rese inattive. In tal modo solo la texture attiva viene disegnata.
		ESEMPIO : Se la texture attiva di livello è "Normale", le texture "Facile" e
		"Difficile" sono rese innative.
	*/
	if (FirstTime)
	{
		resSchermo.push_back(TableHashSettings["TextureSettingsMonitor800"]);

		TableHashSettings["TextureSettingsMonitorHD"]->setActive(false);
		resSchermo.push_back(TableHashSettings["TextureSettingsMonitorHD"]);
		TableHashSettings["TextureSettingsMonitorFullHD"]->setActive(false);
		resSchermo.push_back(TableHashSettings["TextureSettingsMonitorFullHD"]);
		TableHashSettings["TextureSettingsMonitorFullScreen"]->setActive(false);
		resSchermo.push_back(TableHashSettings["TextureSettingsMonitorFullScreen"]);

		TableHashSettings["TextureSettingsLevelFacile"]->setActive(false);
		level.push_back(TableHashSettings["TextureSettingsLevelFacile"]);
		level.push_back(TableHashSettings["TextureSettingsLevelNormale"]);
		TableHashSettings["TextureSettingsLevelDifficile"]->setActive(false);
		level.push_back(TableHashSettings["TextureSettingsLevelDifficile"]);

		audio.push_back(TableHashSettings["TextureSettingsAudioOn"]);
		TableHashSettings["TextureSettingsAudioOff"]->setActive(false);
		audio.push_back(TableHashSettings["TextureSettingsAudioOff"]);
		
		FirstTime = false;
	}
}

/*
	Metodo associato alla callback glutMouseFunc.
*/
void MenuGame::MouseHandler(GLint button, GLint state, GLint x, GLint y)
{
	int change = 0;

	/*
		Se è stato cliccato il tasto sinistro del mouse,
		si hanno diversi comportamenti in base alla canvas del menù
		in cui ci troviamo attualmente.
		Se modalita == 0, ci troviamo nel menù principale.
		Se modalita == 1, ci troviamo nel sotto-menù impostazioni.
		Se modalita == 2, ci troviamo nel sotto-menù best score.
	*/
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) 
	{
		CVector3D v = CVector3D::GetOGLPos(x, y);

		/*
			Se ci troviamo nel menù principale, scorre tutte le texture ad esso relative.
		*/
		if (Instance()->modalita == 0)
		{
			for (auto& elem : Instance()->TableHash)
			{	
				/*
					Se la texture cliccata è selezionabile
				*/
				if (Instance()->VerificaIntersezione(elem.second, v.getRiga(), v.getColonna()) && !elem.second->getNonSelezionabile())
				{	
					/*
						Cliccata texture Play
					*/
					if (elem.first == "TexturePlay")
					{
						Instance()->setMenuAttivo(false);
						GestoreAudio::Instance()->PlaySoundClick();
					}

					/*
						Cliccata texture settings e quindi cambia modalità
					*/
					else if (elem.first == "TextureSettings")
					{
						elem.second->setIsSelected(false);
						Instance()->lastSelected = "";
						Instance()->modalita = 1;
						GestoreAudio::Instance()->PlaySoundClick();
					}
					/*
						Cliccata texture best score e quindi cambia modalità
					*/
					else if (elem.first == "TextureBestScore")
					{
						elem.second->setIsSelected(false);
						Instance()->lastSelected = "";
						Instance()->modalita = 2;
						GestoreAudio::Instance()->PlaySoundClick();
					}
					/*
						Cliccata texture esci e termina il gioco
					*/
					else if (elem.first == "TextureQuitGame")
						exit(0);
				}
			}
		}
		/*
			Se ci troviamo nel menù sotto-menù impostazioni,
			scorre tutte le texture ad esso relative.
		*/
		else if (Instance()->modalita == 1)
		{
			for (auto& elem : Instance()->TableHashSettings)
			{
				/*
					Se la texture cliccata è selezionabile
				*/
				if (Instance()->VerificaIntersezione(elem.second, v.getRiga(), v.getColonna()) && !elem.second->getNonSelezionabile())
				{
					/*
						Cliccata texture back e torna nel menù principale
					*/
					if (elem.first == "TextureSettingsBack")
					{
						Instance()->lastSelected = "";
						Instance()->modalita = 0;
						GestoreAudio::Instance()->PlaySoundClick();
					}
					/*
						Controlla se sono state cliccate le frecce.
						Se è stata cliccata una freccia, richiama VerificaSelezioneFreccia
					*/
					else if (elem.first == "TextureSettingsFrecciaDestraLevel"
						&& Instance()->VerificaSelezioneFreccia(Instance()->level, Instance()->indiceLevel, 1))
						change = 1;
					else if (elem.first == "TextureSettingsFrecciaSinistraLevel"
						&& Instance()->VerificaSelezioneFreccia(Instance()->level, Instance()->indiceLevel, 0))
						change = 1;
					else if (elem.first == "TextureSettingsFrecciaDestraRes"
						&& Instance()->VerificaSelezioneFreccia(Instance()->resSchermo, Instance()->indiceRes, 1))
						change = 2;
					else if (elem.first == "TextureSettingsFrecciaSinistraRes"
						&& Instance()->VerificaSelezioneFreccia(Instance()->resSchermo, Instance()->indiceRes, 0))
						change = 2;
					else if (elem.first == "TextureSettingsFrecciaDestraAudio"
						&& Instance()->VerificaSelezioneFreccia(Instance()->audio, Instance()->indiceAudio, 1))
						change = 3;
					else if (elem.first == "TextureSettingsFrecciaSinistraAudio"
						&& Instance()->VerificaSelezioneFreccia(Instance()->audio, Instance()->indiceAudio, 0))
						change = 3;

					/*
						Verifica e nel caso applica il cambiamento all'impostazione.
					*/
					Instance()->VerificaChangeFreccia(change);
				}
			}
		}
		/*
			Se ci troviamo nel menù sotto-menù best score, 
			scorre tutte le texture ad esso relative.
		*/
		else if (Instance()->modalita == 2)
		{
			for (auto& elem : Instance()->TableHashBestScore)
			{
				if (Instance()->VerificaIntersezione(elem.second, v.getRiga(), v.getColonna()) && !elem.second->getNonSelezionabile())
				{
					/*
						Cliccata texture back e torna nel menù principale
					*/
					if (elem.first == "TextureBestScoreBack")
					{
						Instance()->lastSelected = "";
						Instance()->modalita = 0;
						GestoreAudio::Instance()->PlaySoundClick();
					}
				}
			}
		}
	}
}

/*
	Metodo associato alla callback glutPassiveMotionFunc.
	Questo metodo è utilizzato per cambiare texture quando
	esiste l'intersezione tra una texture del Menù e la posizione
	del cursore del mouse, il tutto in una modalità "passiva".
*/
void MenuGame::MousePassiveHandler(GLint x, GLint y)
{
	CVector3D v = CVector3D::GetOGLPos(x, y);
	
	/*	
		Verifica intersezione passiva tra cursore mouse e 
		texture con la funzione VerificaSelezionePassiva
	*/
	if (Instance()->modalita == 0)
		Instance()->VerificaSelezionePassiva(Instance()->TableHash, v.getRiga(), v.getColonna());
	else if(Instance()->modalita == 1)
		Instance()->VerificaSelezionePassiva(Instance()->TableHashSettings, v.getRiga(), v.getColonna());
}

/*
	Verifica l'intersezione tra il cuorse del mouse e la texture.
	Ritorna true se esiste tale intersezione, altrimenti false.
*/
bool MenuGame::VerificaIntersezione(ElemTexture * elem, GLfloat rigaMouse, GLfloat colonnaMouse)
{
	GLfloat riga = elem->OrigRiga();
	GLfloat colonna = elem->OrigColonna();
	GLfloat h = elem->AltezzaTexture();
	GLfloat w = elem->LarghezzaTexture();

	if (rigaMouse >= colonna && colonnaMouse <= riga && rigaMouse < colonna + w && colonnaMouse >= riga + h)
		return true;

	return false;
}

/*
	Questo metodo prende in input un vector di elementi
	(ovvero o texture di livelli o di risoluzione schermo o audio),
	l'indice del vector che corrisponde a quale texture attualmente è attiva, 
	ovvero qualche texture è mostrata a schermo,
	e frecciaDir che è 0 se è stato cliccato un botton di sinitra oppure 1 
	se è stato cliccato un bottone di destra.

	Ritorna true se c'è stato un cambiamento della specifica impostazione, altrimenti false.
*/
bool MenuGame::VerificaSelezioneFreccia(vector<ElemTexture *> contenitore, int & indice, int frecciaDir)
{
	/*
		Sinistra
	*/
	if (frecciaDir == 0) 
	{
		if (indice > 0)
		{
			contenitore.at(indice)->setActive(false);
			indice--;
			contenitore.at(indice)->setActive(true);
			GestoreAudio::Instance()->PlaySoundClick();
			return true;
		}
	}
	/*
		Destra
	*/
	else if (frecciaDir == 1) 
	{
		if (indice < contenitore.size() - 1)
		{
			contenitore.at(indice)->setActive(false);
			indice++;
			contenitore.at(indice)->setActive(true);
			GestoreAudio::Instance()->PlaySoundClick();
			return true;
		}
	}
	return false;
}

/*
	Se c'è stato un cambiamento ad una impostazione,
	applica il cambiamento alla specifica impostazione.
*/
void MenuGame::VerificaChangeFreccia(int change)
{
	if (change == 0)
		return;

	if (change == 1)
		Instance()->gestoreImpostazioni->setDifficolta(Instance()->indiceLevel);
	else if (change == 2)
	{
		if (Instance()->indiceRes == 0)
		{
			Instance()->gestoreImpostazioni->setResSchermo(make_pair(800, 600));
			Instance()->gestoreImpostazioni->setFullScreen(false);
		}
		else if (Instance()->indiceRes == 1)
		{
			Instance()->gestoreImpostazioni->setResSchermo(make_pair(1280, 720));
			Instance()->gestoreImpostazioni->setFullScreen(false);
		}
		else if (Instance()->indiceRes == 2)
		{
			Instance()->gestoreImpostazioni->setResSchermo(make_pair(1920, 1080));
			Instance()->gestoreImpostazioni->setFullScreen(false);
		}
		else if (Instance()->indiceRes == 3)
		{
			Instance()->gestoreImpostazioni->setResSchermo(make_pair(GetDeviceCaps(GetDC(NULL), HORZRES), GetDeviceCaps(GetDC(NULL), VERTRES)));
			Instance()->gestoreImpostazioni->setFullScreen(true);
		}
	}
	else if (change == 3)
	{
		if (Instance()->indiceAudio == 0)
		{
			Instance()->gestoreImpostazioni->setAudio(true);
			GestoreAudio::Instance()->EnableAudio();
			GestoreAudio::Instance()->StartSoundMenu();
		}
		else
		{
			Instance()->gestoreImpostazioni->setAudio(false);
			GestoreAudio::Instance()->StopAllSounds();
			GestoreAudio::Instance()->DisableAudio();
		}
	}
}

/*
	Se esiste l'intersezione tra la texture ed il cursore del mouse,
	attiva la texture di selezione specifica per quella texture.
*/
void MenuGame::VerificaSelezionePassiva(unordered_map <string, ElemTexture*> TableHash, GLfloat riga, GLfloat colonna)
{
	bool check = false;
	for (auto& elem : TableHash) 
	{
		if (Instance()->VerificaIntersezione(elem.second, riga, colonna) && !elem.second->getNonSelezionabile())
		{
			elem.second->setIsSelected(true);
			check = true;
			Instance()->lastSelected = elem.first;
			break;
		}
	}
	if (!check && Instance()->lastSelected != "")
	{
		TableHash[Instance()->lastSelected]->setIsSelected(false);
		Instance()->lastSelected = "";
	}
}

void MenuGame::InitMenu()
{
	glDisable(GL_LIGHT0);
	glDisable(GL_NORMALIZE);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_MULTISAMPLE);

	glViewport(0, 0, 800, 600);
	glutReshapeWindow(800, 600);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	// Se disabilito il Caricamento delle Texutre...
	CaricaTexture();

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glEnable(GL_TEXTURE_2D);
	glewExperimental = GL_TRUE;
	glewInit();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, 100.0, 0.0, 100.0, -1.0, 10.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void MenuGame::DisableMenuInit()
{
	for (auto& elem : TableHash)
		elem.second->ResetTexture();
}

/*
	Metodo utilizzato per richiamare i vari metodo di RichiestaGUI
	necessari per il draw delle texture in base a qual'è la modalità,
	ovvero qual è la canvas attualmente attiva.
*/
void MenuGame::Show()
{
	/*
		Se il menu è attivo
	*/
	if (menuAttivo)
		/*
			Ci troviamo nel menu principale
		*/
		if (Instance()->modalita == 0)
			/*
				Draw delle texture del menu principale
			*/
			RichiestaGUI::DisegnaMenu(TableHash);

		/*
			Ci troviamo nel sottomenu delle impostazioni
		*/
		else if (Instance()->modalita == 1)
			/*
				Draw delle texture del sottomenu
			*/
			RichiestaGUI::DisegnaMenu(TableHashSettings);

		/*
			Ci troviamo nel Sottomenu Best Score
		*/
		else if (Instance()->modalita == 2)
		{
			RichiestaGUI::DisegnaMenu(TableHashBestScore);
			RichiestaGUI::DisegnaBestScore();
		}
}

