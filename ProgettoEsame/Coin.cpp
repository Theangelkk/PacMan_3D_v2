#include "Coin.h"

Coin::Coin(int rig, int col)
{
	Coin_Lista_Draw = 0;

	Riga = rig;
	Colonna = col;

	GLfloat colore[3] = { 255.0/255.0, 163.0/255.0, 0.0/255.0 };
	GLfloat Bianco[3] = { 1.0, 1.0, 1.0 };

	Coin_Lista_Draw = glGenLists(1);
	glNewList(Coin_Lista_Draw, GL_COMPILE);
		glPushMatrix();
			glColor3fv(colore);
			glColor3fv(colore);
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, colore);
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, colore);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Bianco);
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 128.0);
			glTranslatef(0.0f + Colonna, GestoreScena::Instance()->GetPosY() - Riga, GestoreScena::Instance()->GetZ());
			glutSolidSphere(0.3, 4, 4);
		glPopMatrix();
	glEndList();
}

void Coin::Draw()
{
	glCallList(Coin_Lista_Draw);
}