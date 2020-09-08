#include "DrawScene.h"

/*
	Disegno della Struttura del Gioco
*/
void DrawScene::DisegnaStrutturaGioco(GLuint StrutturaGioco, GLfloat z)
{
	glCallList(StrutturaGioco);
}

/*
	Disegno degli Elementi Dinamici 3D come ad esempio il PacMan
	oppure i Nemici
*/
void DrawScene::DisegnaElemento3D(Elem3D* elem, GLfloat Colonna, GLfloat Riga, GLfloat PosInizialeY, GLfloat z)
{
	glPushMatrix();
		glTranslatef(0.0f + Colonna, PosInizialeY - Riga, z);
		elem->Draw();
	glPopMatrix();
}

/*
	Disegno del Testo Vettoriale in Output
*/
void DrawScene::TextDraw(GLfloat x, GLfloat y, GLfloat z, GLfloat PosInizialeY, float R, float G, float B, GLfloat ScaleX, GLfloat ScaleY, GLfloat ScaleZ, void* Font, const char *String)
{
	const char *c;
	GLfloat Colore[3] = { R,G,B };

	glPushMatrix();
		glColor3fv(Colore);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, Colore);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, Colore);

		glTranslatef(y, PosInizialeY - x, z);
		glScalef(ScaleX, ScaleY, ScaleZ);

		for (c = String; *c != '\0'; c++)
			glutStrokeCharacter(Font, *c);
	glPopMatrix();
}

/*
	Disegno della Scritta del Punteggio + l'oggetto 3D del Dollaro
*/
void DrawScene::PunteggioDraw(GLfloat x, GLfloat y, GLfloat z, GLfloat PosInizialeY, float R, float G, float B, GLfloat ScaleX, GLfloat ScaleY, GLfloat ScaleZ, void* Font, const char *String, GLuint Dollar_Lista_Draw, float *angle)
{
	/*
		Disegno della testo "Score"
	*/
	TextDraw(x, y, z, PosInizialeY, R, G, B, ScaleX, ScaleY, ScaleZ, Font, String);
	
	/*
		Disegno dell'oggetto 3D del Dollaro, correlato della sua Animazione
	*/
	glPushMatrix();
		glTranslatef(y-3.1, PosInizialeY - x -0.2, z);
		glRotatef(*angle, 0.f, 1.f, 0.f);
		glCallList(Dollar_Lista_Draw);
		Modellazione_3D::RotationObect(angle);
	glPopMatrix();
}

/*
	Disegno dello Sfondo di Background di una Scena
*/
void DrawScene::BackGroundDraw(GLuint Back_Ground_Draw)
{
	glCallList(Back_Ground_Draw);
}
