#pragma once
#include "Libs.h"
#include "Elem3D.h"
#include "DrawScene.h"
#include "GestoreScena.h"
#include "RegoleGame.h"
#include "GestoreUtente.h"
#include "ElemTexture.h"

class RichiestaGUI 
{
	public:
		static void Init_ArrayElem3D();

		static void setStatusCountDown(bool status);
		static void setTempoIniziale(int t);
		static void DisegnaCircuito();
		static void DisegnaElementi3D();
		static void DisegnaMenu(unordered_map <string, ElemTexture*> TableHahTexture);
		static void DisegnaBestScore();

		static void PushElem3D(Elem3D* elem);
		static void PopElem3D(Elem3D* elem);

		static vector<Elem3D*> getArrayElem3D();

	private:
		static vector<Elem3D*> Elementi_Dinamici;
		static DrawScene* drawScene;
		static vector<GLfloat*> Colori_Struttura;
		static GLuint StrutturaGioco_Lista;

		static bool FirtTime;
		static double PosInizialeX;
		static double PosInizialeY;

		static const char* pathImageBackground[3];

		static void CostruisciLista_StrutturaGioco(GLfloat z);
		static void CreaListaBackGround(unsigned char* imagedata, int width, int height, int channels);
		static void ComponentiConnesse(unordered_map<string, int>* Tabella_Hash, int Riga, int Colonna, int i);
		static void ControllaAddiacenti(unordered_map<string, int>* Tabella_Hash, int Riga, int Colonna, int i);
		static void DisegnaMuro(int Riga, int Colonna, int i);
		static void DrawCountDown();

		static void DrawBackGround();
		static void DrawLife();
		static void DrawPunteggio();
		static void DrawTimer();
		static void DrawNomeUtente();
		static void DrawPausa();
		static void StringPunteggio(int Punteggio, string* String_Punt);
		static int TempoInzialeAttesa;
		static bool AttivoCountDown;
		 
		static const struct aiScene* Dollar_Modello3D;
		static GLuint* texture;
		static aiVector3D scene_min;
		static aiVector3D scene_max;
		static aiVector3D scene_center;

		static const char* Path_Modello3D;
		static GLuint Dollar_Lista_Draw;
		static GLuint Back_Ground_Draw;
		static GLuint Menu_Draw;

		static float angle;
};

