#include "GestoreScena.h"
#include "MenuGame.h"

GestoreScena* GestoreScena::Istanza = nullptr;

GestoreScena* GestoreScena::Instance()
{
    if (Istanza == nullptr)
        Istanza = new GestoreScena;
    
    return Istanza;
}

GestoreScena::GestoreScena()
{	}

/*
	Inizializzazione della Scena del Game
*/
void GestoreScena::InitScena(const char* pathGioco)
{
	LeggiMatriceDaFile(pathGioco);
	Setup();
}

/*
	Lettura e costruzione della Struttura del Gioco attraverso
	il Path del File passato in Input.
	L'intera struttura verrà contenuta in un vector<string> dove, ogni
	Riga corrisponde ad una riga effettiva della struttura del Game.
*/
void GestoreScena::LeggiMatriceDaFile(const char* pathGioco)
{
	StrutturaGioco.clear();
	ifstream input(pathGioco);

	/*
		Apertura del File della Struttura del Gioco
	*/
	if (!input.is_open())
		cout << "Errore Apertura File della Struttura del Gioco" << endl;

	/*
		Caricamento dell'intera struttura del gioco nel vector StrutturaGioco
	*/
	for (string line; getline(input, line); )
		StrutturaGioco.push_back(line); 

	input.close();
}

void GestoreScena::GetAltezzaLarghezza(int & h, int & l)
{
	h = StrutturaGioco.size();
	l = StrutturaGioco.at(0).size();
}

vector<string> GestoreScena::GetStruttuaGioco()
{
	return StrutturaGioco;
}

GLfloat GestoreScena::GetZ()
{
	return z;
}

double GestoreScena::GetPosY()
{
	return PosInizialeY;
}

/*
	Setup Iniziale della Scena
*/
void GestoreScena::Setup()
{
	/*
		Vengono disabilitate tutte le funzionalità 
		precedentemente abailitate nelle altre Scene del Gioco in
		modo da garantire una corretta visualizzazione e funzionamento
	*/
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHT0);
	glDisable(GL_NORMALIZE);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_MULTISAMPLE);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	/*
		Impostazioni relative alla Luce Principale del Game
	*/
	GLfloat lightPos0[] = { 0.0f,0.0f, -z+10, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
	glEnable(GL_LIGHT0);

	glEnable(GL_NORMALIZE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_MULTISAMPLE);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	PosInizialeY = (double)StrutturaGioco.size();
	double Righe = (double)StrutturaGioco.size() + 4.0;
	double Colonne = (double)StrutturaGioco[0].size() + 4.0;

	/*
		Dimensione della Struttura del Gioco 42 x 77:
			Z = -91 
			Near = 92 
			Far = 101
	*/
	glFrustum(-4.0, Colonne, -4.0, Righe, -(z+1), -(z-10));
}
