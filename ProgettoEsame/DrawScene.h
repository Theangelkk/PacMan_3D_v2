#pragma once
#include "Libs.h"
#include "Elem3D.h"
#include "Modellazione_3D.h"

/*	--------------------- DEFINIZIONE DELLA CLASSE ----------------------------------
	Classe DrawScene relativa alla gestione di tutte le funzionalità per effettuare
	il Rendering di tutte le componenti grafiche presenti all'interno dell'applicativo.
	Per ottimizzare le prestazioni nella fase di rendering, si è fatto utilizzo delle
	funzionalità offerte dall'API OpenGL relative all'impiego della GPU per l'elaborazione
	delle primitive geometrica di ogni Oggetto Complesso presente nelle Scene 3D.
--------------------------------------------------------------------------------*/
class DrawScene
{
	public:
		static void DisegnaStrutturaGioco(GLuint StrutturaGioco, GLfloat z);
		static void DisegnaElemento3D(Elem3D* elem,GLfloat Colonna, GLfloat Riga, GLfloat PosInizialeY, GLfloat z);
		static void PunteggioDraw(GLfloat x, GLfloat y, GLfloat z, GLfloat PosInizialeY, float R, float G, float B, GLfloat ScaleX, GLfloat ScaleY, GLfloat ScaleZ, void* Font, const char *String, GLuint Dollar_Lista_Draw, float *angle);
		static void TextDraw(GLfloat x, GLfloat y, GLfloat z, GLfloat PosInizialeY, float R, float G, float B, GLfloat ScaleX, GLfloat ScaleY, GLfloat ScaleZ, void* Font, const char *String);
		static void BackGroundDraw(GLuint Back_Ground_Draw);
};

