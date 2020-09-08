#include "ElemTexture.h"

ElemTexture::ElemTexture(GLfloat _x0, GLfloat _y0, GLfloat _z, GLfloat _h, GLfloat _w, unsigned int _textureID, unsigned int _textureIDselected)
{
	x0 = _x0;
	y0 = _y0;
	h = _h;
	w = _w;
	z = _z;
	textureID = _textureID;
	textureIDselected = _textureIDselected;
}

ElemTexture::ElemTexture(GLfloat _x0, GLfloat _y0, GLfloat _z, GLfloat _h, GLfloat _w, unsigned int _textureID, bool _status)
{
	x0 = _x0;
	y0 = _y0;
	h = _h;
	w = _w;
	z = _z;
	textureID = _textureID;
	nonSelezionabile = _status;
}

/*
	Metodo che Elimina la texture se attualmente risulta essere
	caricata in Memoria (necessario per ovviare ad un problema di
	OpenGL nella fase di Rendering di Scene successive nel Gioco)
*/
void ElemTexture::ResetTexture()
{
	if (textureID)
		glDeleteTextures(1, &textureID);

	if (textureIDselected)
		glDeleteTextures(1, &textureIDselected);

	textureID = 0;
	textureIDselected = 0;
}