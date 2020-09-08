#include "RichiestaGUI.h"
#include "Livelli.h"

vector<Elem3D*> RichiestaGUI::Elementi_Dinamici;
DrawScene* RichiestaGUI::drawScene = new DrawScene();
GLuint RichiestaGUI::StrutturaGioco_Lista;
vector<GLfloat*> RichiestaGUI::Colori_Struttura;
bool RichiestaGUI::FirtTime = true;
double RichiestaGUI::PosInizialeX = 0.0;
double RichiestaGUI::PosInizialeY = 0.0;
int RichiestaGUI::TempoInzialeAttesa = 0;

const struct aiScene* RichiestaGUI::Dollar_Modello3D = nullptr;
GLuint* RichiestaGUI::texture = nullptr;
aiVector3D RichiestaGUI::scene_min;
aiVector3D RichiestaGUI::scene_max;
aiVector3D RichiestaGUI::scene_center;
const char* RichiestaGUI::Path_Modello3D = "Modelli/Dollar.obj";
GLuint RichiestaGUI::Dollar_Lista_Draw = 0;
GLuint RichiestaGUI::Back_Ground_Draw = 0;
float RichiestaGUI::angle = 0.0f;
bool RichiestaGUI::AttivoCountDown = false;
const char* RichiestaGUI::pathImageBackground[3] = { "Image/Level1_Background.png","Image/Level2_Background.png","Image/Level3_Background.png" };

void RichiestaGUI::Init_ArrayElem3D()
{
	Elementi_Dinamici.clear();
	Colori_Struttura.clear();

	//Inserimento Colori Struttura
	GLfloat* Rosso = new GLfloat[3];
	Rosso[0] = 1.0f;
	Rosso[1] = 0.0f;
	Rosso[2] = 0.0f;
	Colori_Struttura.push_back(Rosso);

	GLfloat* Verde = new GLfloat[3];
	Verde[0] =  0.0f;
	Verde[1] =  1.0f;
	Verde[2] =  0.0f;
	Colori_Struttura.push_back(Verde);

	GLfloat* Blu = new GLfloat[3];
	Blu[0] = 0.0f;
	Blu[1] = 0.0f;
	Blu[2] = 1.0f;
	Colori_Struttura.push_back(Blu);
}

void RichiestaGUI::PushElem3D(Elem3D* elem)
{
	bool Esiste = false;

	for (unsigned int i = 0; i < unsigned int (Elementi_Dinamici.size()); i++)
	{
		// Elemento Dinamico già Presente
		if (Elementi_Dinamici.at(i) == elem)
			return;
	}

	Elementi_Dinamici.push_back(elem);
}

void RichiestaGUI::PopElem3D(Elem3D* elem)
{
	bool Esiste = false;

	for (unsigned int i = 0; i < Elementi_Dinamici.size(); i++)
	{
		// Se Esiste l'Elemento Dinamico
		if (Elementi_Dinamici.at(i) == elem)
		{
			Elementi_Dinamici.erase(Elementi_Dinamici.begin() + i);
			return;
		}
	}
}

vector<Elem3D*> RichiestaGUI::getArrayElem3D()
{
	return Elementi_Dinamici;
}

void RichiestaGUI::DisegnaCircuito()
{
	GLfloat z = GestoreScena::Instance()->GetZ();
	
	if (FirtTime)
	{
		CostruisciLista_StrutturaGioco(z);
		RegoleGame::Istance();
		FirtTime = false;

		// Caricamento Modello 3D Dollar
		Dollar_Modello3D = Modellazione_3D::Caricamento_Modello3D(Path_Modello3D, &scene_min, &scene_center, &scene_max);
		texture = Modellazione_3D::LoadGLTextures(Dollar_Modello3D, Path_Modello3D);

		Dollar_Lista_Draw = glGenLists(1);
		glNewList(Dollar_Lista_Draw, GL_COMPILE);
			Modellazione_3D::Recursive_Render(Dollar_Modello3D, Dollar_Modello3D->mRootNode, texture);
		glEndList();

		int width = 0;
		int height = 0;
		int channels = 0;
		unsigned char* imagedata = nullptr;
		int Level = Livelli::Instance()->getLivelloAttuale();

		GestoreTexture::CaricaTextureBackgroundGame(pathImageBackground[Level - 1], imagedata, &width, &height, &channels);

		Back_Ground_Draw = glGenLists(1);
		glNewList(Back_Ground_Draw, GL_COMPILE);
			glEnable(GL_TEXTURE_2D);
			CreaListaBackGround(imagedata, width, height, channels);
			glDisable(GL_TEXTURE_2D);
		glEndList();
	}
	
	//DrawBackGround();
	DrawLife();
	DrawTimer();
	DrawCountDown();
	DrawNomeUtente();
	DrawPunteggio();

	if (Livelli::Instance()->StatusPauseGame())
		DrawPausa();

	drawScene->DisegnaStrutturaGioco(StrutturaGioco_Lista, z);
	RegoleGame::Istance()->Draw_Bonus_Coin();
	RegoleGame::Istance()->Draw_Bonus_Bonus();
}

void RichiestaGUI::DrawBackGround()
{
	drawScene->BackGroundDraw(Back_Ground_Draw);
}

void RichiestaGUI::DrawLife()
{
	GLfloat z = GestoreScena::Instance()->GetZ();
	const int font = (int)GLUT_STROKE_ROMAN;

	string String_Compl = "Life: " + to_string(GestoreUtente::Instance()->getLife());

	drawScene->TextDraw(40.0, 5.0, z, PosInizialeY, 1.0, 1.0, 1.0, 0.03, 0.01, 1.0, (void*)font, String_Compl.c_str());
}

void RichiestaGUI::DrawPunteggio()
{
	GLfloat z = GestoreScena::Instance()->GetZ();
	const int font = (int)GLUT_STROKE_ROMAN;
	string String_Punt = "00000000";

	StringPunteggio(GestoreUtente::Instance()->getPunteggio(), &String_Punt);

	string String_Compl = "Score: " + String_Punt;

	drawScene->PunteggioDraw(-1.5, 50.0, z, PosInizialeY, 1.0, 1.0, 1.0, 0.03, 0.01, 1.0, (void*)font, String_Compl.c_str(), Dollar_Lista_Draw, &angle);
}

void RichiestaGUI::DrawTimer()
{
	GLfloat z = GestoreScena::Instance()->GetZ();
	const int font = (int)GLUT_STROKE_ROMAN;

	int SecontiTot = RegoleGame::getSecondGame();

	int Secondi = SecontiTot%60;
	int Minuti = (SecontiTot%3600) / 60;
	int Ore = SecontiTot/3600;

	string Sec_String;
	string Min_String;
	string Ore_String;

	if (Secondi < 10)
		Sec_String = "0" + to_string(Secondi);
	else
		Sec_String = to_string(Secondi);

	if (Minuti < 10)
		Min_String = "0" + to_string(Minuti);
	else
		Min_String = to_string(Minuti);

	if (Ore < 10)
		Ore_String = "0" + to_string(Ore);
	else
		Ore_String = to_string(Ore);

	string Timer = "Time: " + Ore_String + ":" + Min_String + ":" + Sec_String;

	drawScene->TextDraw(-1.5, 5.0, z, PosInizialeY, 1.0, 1.0, 1.0, 0.03, 0.01, 1.0, (void*)font, Timer.c_str());
}

void RichiestaGUI::DisegnaMenu(unordered_map < string, ElemTexture*> TableHahTexture)
{
	for (auto elem = TableHahTexture.begin(); elem != TableHahTexture.end(); elem++)
	{
		if (!elem->second->getActive())
			continue;

		GLfloat riga = elem->second->OrigRiga();
		GLfloat colonna = elem->second->OrigColonna();
		GLfloat z = elem->second->OrigZ();
		GLfloat h = elem->second->AltezzaTexture();
		GLfloat w = elem->second->LarghezzaTexture();
		unsigned int textureID = elem->second->TextureID();
		unsigned int textureIDselected = elem->second->TextureIDselected();

		glPushMatrix();
		if (!elem->second->getIsSelected())
			glBindTexture(GL_TEXTURE_2D, textureID);
		else glBindTexture(GL_TEXTURE_2D, textureIDselected);

		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 0.0);
			glVertex3f(riga + h, colonna, z);

			glTexCoord2f(1.0, 0.0);
			glVertex3f(riga, colonna, z);

			glTexCoord2f(1.0, 1.0);
			glVertex3f(riga, colonna + w, z);

			glTexCoord2f(0.0, 1.0);
			glVertex3f(riga + h, colonna + w, z);
		glEnd();
		glPopMatrix();
	}
}

void RichiestaGUI::DisegnaBestScore()
{
	vector<Livelli::ScoreUser> BestScore = Livelli::getListaBestScore();
	const int font = (int)GLUT_STROKE_ROMAN;

	float offset_y = 12.0f;

	float R = 1.0f;
	float G = 1.0f;
	float B = 1.0f;

	for (int i=0; i < BestScore.size(); i++)
	{
		string Nome_Utente = BestScore.at(i).User;
		string Punteggio = to_string(BestScore.at(i).Punteggio);

		int Secondi = BestScore.at(i).Tempo_Sec % 60;
		int Minuti = (BestScore.at(i).Tempo_Sec % 3600) / 60;
		int Ore = BestScore.at(i).Tempo_Sec / 3600;

		string Sec_String;
		string Min_String;
		string Ore_String;

		if (Secondi < 10)
			Sec_String = "0" + to_string(Secondi);
		else
			Sec_String = to_string(Secondi);

		if (Minuti < 10)
			Min_String = "0" + to_string(Minuti);
		else
			Min_String = to_string(Minuti);

		if (Ore < 10)
			Ore_String = "0" + to_string(Ore);
		else
			Ore_String = to_string(Ore);

		string Tempo = "" + Ore_String + ":" + Min_String + ":" + Sec_String;

		/*
			Se risulta essere il miglior Punteggio
		*/
		if (i == 0)
		{
			R = 252.0 / 255.0;
			G = 140.0 / 255.0;
			B = 3.0 / 255.0;
		}

		/*
			Se risulta essere il Secondo miglior Punteggio
		*/
		else if(i == 1)
		{
			R = 128.0 / 255.0;
			G = 128.0 / 255.0;
			B = 128.0 / 255.0;
		}

		/*
			Se risulta essere il Terzo miglior Punteggio
		*/
		else if (i == 2)
		{
			R = 205.0 / 255.0;
			G = 127.0 / 255.0;
			B = 50.0 / 255.0;
		}

		drawScene->TextDraw(-73.0+((float)i*offset_y), 22.0, 0.0, 0.0, R, G, B, 0.025, 0.025, 1.0, (void*)font, Nome_Utente.c_str());
		drawScene->TextDraw(-73.0+((float)i*offset_y), 58.0, 0.0, 0.0, R, G, B, 0.025, 0.025, 1.0, (void*)font, Punteggio.c_str());
		drawScene->TextDraw(-73.0+((float)i*offset_y), 80.0, 0.0, 0.0, R, G, B, 0.025, 0.025, 1.0, (void*)font, Tempo.c_str());
	}

	glutPostRedisplay();
}

void RichiestaGUI::CostruisciLista_StrutturaGioco(GLfloat z)
{
	vector<string> StrutturaGioco = GestoreScena::Instance()->GetStruttuaGioco();
	PosInizialeY = (double)StrutturaGioco.size();
	PosInizialeX = (double)StrutturaGioco[0].size() / 2.0;

	int Num_Color = Colori_Struttura.size();
	int Colore = 0;
	unordered_map<string, int>* Tabella_Hash = new unordered_map<string, int>();

	StrutturaGioco_Lista = glGenLists(1);
	glNewList(StrutturaGioco_Lista, GL_COMPILE);
	glEnable(GL_COLOR_MATERIAL);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// Disegno del Gioco
	for (int rig = 0; rig < StrutturaGioco.size(); rig++) //scorriamo ogni elemento come una matrice
	{
		for (int col = 0; col < StrutturaGioco[rig].size(); col++)
		{
			// Inserisco l'ostacolo nella Tabella Hash
			if (StrutturaGioco[rig][col] == '1')
			{
				string Chiave = to_string(rig);
				Chiave += "-" + to_string(col);
				Tabella_Hash->insert(pair<string, int>(Chiave, 0));
			}
		}
	}

	while (Tabella_Hash->size() > 0)
	{
		Colore += 1;
				
		GLfloat* Colore_Ostacolo = Colori_Struttura.at(Colore%Colori_Struttura.size());
			
		string Chiave = Tabella_Hash->begin()->first;

		replace(Chiave.begin(), Chiave.end(), '-', ' ');
		vector<int> array;
		stringstream ss(Chiave);
		int temp;
		while(ss >> temp)
			array.push_back(temp);
				
		int Riga = array.at(0);
		int Colonna = array.at(1);

		// Disegno Muro
		DisegnaMuro(Riga, Colonna, Colore);

		Tabella_Hash->erase(Tabella_Hash->begin());

		ControllaAddiacenti(Tabella_Hash, Riga, Colonna, Colore);
	}

	delete Tabella_Hash;
	glDisable(GL_COLOR_MATERIAL);
	glEndList();
}

void RichiestaGUI::ComponentiConnesse(unordered_map<string, int>* Tabella_Hash, int Riga,int Colonna,int Colore)
{
	string chiave_find = to_string(Riga);
	chiave_find += "-" + to_string(Colonna);

	unordered_map<std::string, int>::iterator elemento = Tabella_Hash->find(chiave_find);
	
	// Verifico Attualmente se non sono stato già disegnato
	if (elemento != Tabella_Hash->end())
	{
		// Disegna Muro
		DisegnaMuro(Riga, Colonna, Colore);

		Tabella_Hash->erase(elemento);

		// Controllo 8 Posizioni Connesse
		ControllaAddiacenti(Tabella_Hash, Riga, Colonna, Colore);
	}
}

void RichiestaGUI::ControllaAddiacenti(unordered_map<string, int>* Tabella_Hash, int Riga, int Colonna, int Colore)
{
	// Diagonale Superiore
	string chiave_find = to_string(Riga - 1);
	chiave_find += "-" + to_string(Colonna - 1);

	if (Tabella_Hash->find(chiave_find) != Tabella_Hash->end())
		ComponentiConnesse(Tabella_Hash, Riga - 1, Colonna - 1, Colore);

	// Elemento in Alto
	chiave_find.clear();
	chiave_find = "" + Riga - 1;
	chiave_find += "-" + Colonna;

	if (Tabella_Hash->find(chiave_find) != Tabella_Hash->end())
		ComponentiConnesse(Tabella_Hash, Riga - 1, Colonna, Colore);

	// Elemento Diagonale Superiore
	chiave_find.clear();
	chiave_find = to_string(Riga - 1);
	chiave_find += "-" + to_string(Colonna + 1);

	if (Tabella_Hash->find(chiave_find) != Tabella_Hash->end())
		ComponentiConnesse(Tabella_Hash, Riga - 1, Colonna + 1, Colore);

	// Elemento alla Sinistra 
	chiave_find.clear();
	chiave_find = to_string(Riga);
	chiave_find += "-" + to_string(Colonna - 1);

	if (Tabella_Hash->find(chiave_find) != Tabella_Hash->end())
		ComponentiConnesse(Tabella_Hash, Riga, Colonna - 1, Colore);

	// Elemento alla Destra
	chiave_find.clear();
	chiave_find = to_string(Riga);
	chiave_find += "-" + to_string(Colonna + 1);

	if (Tabella_Hash->find(chiave_find) != Tabella_Hash->end())
		ComponentiConnesse(Tabella_Hash, Riga, Colonna + 1, Colore);

	// Elemento in Basso a Sinistra
	chiave_find.clear();
	chiave_find = to_string(Riga + 1);
	chiave_find += "-" + to_string(Colonna - 1);

	if (Tabella_Hash->find(chiave_find) != Tabella_Hash->end())
		ComponentiConnesse(Tabella_Hash, Riga + 1, Colonna - 1, Colore);

	// Elemento in Basso
	chiave_find.clear();
	chiave_find = to_string(Riga + 1);
	chiave_find += "-" + to_string(Colonna);

	if (Tabella_Hash->find(chiave_find) != Tabella_Hash->end())
		ComponentiConnesse(Tabella_Hash, Riga + 1, Colonna, Colore);

	// Elemento in Basso a Destra
	chiave_find.clear();
	chiave_find = to_string(Riga + 1);
	chiave_find += "-" + to_string(Colonna + 1);

	if (Tabella_Hash->find(chiave_find) != Tabella_Hash->end())
		ComponentiConnesse(Tabella_Hash, Riga + 1, Colonna + 1, Colore);
}

void RichiestaGUI::DisegnaMuro(int Riga,int Colonna, int Colore)
{
	GLfloat z = GestoreScena::Instance()->GetZ();
	GLfloat* Colore_Ostacolo = Colori_Struttura.at(Colore % Colori_Struttura.size());

	glPushMatrix();
		glColor3fv(Colore_Ostacolo);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, Colore_Ostacolo);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, Colore_Ostacolo);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 128.0);
		glTranslatef(GLfloat(0.0 + Colonna), GLfloat(PosInizialeY - Riga), z);
		glutSolidCube(0.9);
	glPopMatrix();
}

void RichiestaGUI::DisegnaElementi3D()
{
	GLfloat Colonna=0.0, Riga=0.0, PosInizialeY=0.0, z=0.0;

	for (unsigned i = 0; i < Elementi_Dinamici.size(); i++) 
	{
		Colonna = (GLfloat)Elementi_Dinamici.at(i)->getColonna();
		Riga = (GLfloat)Elementi_Dinamici.at(i)->getRiga();
		PosInizialeY = (GLfloat)GestoreScena::Instance()->GetStruttuaGioco().size();
		z = GestoreScena::Instance()->GetZ();

		drawScene->DisegnaElemento3D(Elementi_Dinamici.at(i),Colonna, Riga, PosInizialeY, z);
	}
}

void RichiestaGUI::setTempoIniziale(int t)
{
	TempoInzialeAttesa = t;
}

void RichiestaGUI::DrawCountDown()
{
	GLfloat z = GestoreScena::Instance()->GetZ();
	
	float R = 1.0;
	float G = 1.0;
	float B = 1.0;

	if (AttivoCountDown)
	{
		const int font = (int)GLUT_STROKE_MONO_ROMAN;
		string countDown = to_string(TempoInzialeAttesa / 1000);

		if (TempoInzialeAttesa <= 3000)
		{
			G = 0.0;
			B = 0.0;
		}

		// Disegna Tempo Rimanente
		drawScene->TextDraw(30.0, 25.0, z + 1, PosInizialeY, R, G, B, 0.25, 0.20, 1.0, (void*)font, countDown.c_str());
	}
}

void RichiestaGUI::DrawNomeUtente()
{
	GLfloat z = GestoreScena::Instance()->GetZ();

	const int font = (int)GLUT_STROKE_ROMAN;
	string NomeUtente = "Player: " + GestoreUtente::Instance()->getNomeUtente();

	// Disegna Nome Utente
	drawScene->TextDraw(40.0, 50.0, z, PosInizialeY, 1.0, 1.0, 1.0, 0.02, 0.01, 1.0, (void*)font, NomeUtente.c_str());
}

void RichiestaGUI::DrawPausa()
{
	GLfloat z = GestoreScena::Instance()->GetZ();

	const int font = (int)GLUT_STROKE_ROMAN;
	string Pausa = "Pausa";

	drawScene->TextDraw(27.0, 10.0, z + 1, PosInizialeY, 1.0, 1.0, 1.0, 0.18, 0.18, 1.0, (void*)font, Pausa.c_str());
}

void RichiestaGUI::StringPunteggio(int Punteggio, string* String_Punt)
{
	for (int i = 0; i < 8; i++)
	{
		int Unit = (int)(Punteggio / pow(10.0, i)) % 10;

		String_Punt->at(8 - 1 - i) = 48 + Unit;
	}
}

void RichiestaGUI::setStatusCountDown(bool status)
{
	AttivoCountDown = status;
}

void RichiestaGUI::CreaListaBackGround(unsigned char* imagedata, int width, int height, int channels)
{
	glPixelStorei(GL_PACK_ALIGNMENT, 1);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imagedata);

	GLfloat z = GestoreScena::Instance()->GetZ();

	glPushMatrix();
		glBegin(GL_QUADS);

			glTexCoord2f(0.0, 0.0);
			glVertex3f(-5, -20, z);

			glTexCoord2f(1.0, 0.0);
			glVertex3f(100, -20, z);

			glTexCoord2f(1.0, 1.0);
			glVertex3f(100, 70, z);

			glTexCoord2f(0.0, 1.0);
			glVertex3f(-5, 70, z);

		glEnd();
	glPopMatrix();
}