#pragma once
#include "Libs.h"

/*	--------------------- DEFINIZIONE DELLA CLASSE ----------------------------------
	Classe ElemTexture relativa alla gestione e caricamento delle Texture da applicare
	successivamente ad Oggetti 3D presenti nelle diverse Sequeunze del videogame.
	Permette di definire se un oggetto 3D possiede la funzionalità di essere selezionato,
	quindi di conseguenza di modificare la sua Texture, e se riusulta essere attivo
	durante determinate fasi del ciclo di Vita dell'Applicativo.
-------------------------------------------------------------------------------------*/
class ElemTexture
{
	public:
		ElemTexture(GLfloat _x0, GLfloat _y0, GLfloat _z, GLfloat _h, GLfloat _w, unsigned int _textureID, unsigned int _textureIDselected);
		ElemTexture(GLfloat _x0, GLfloat _y0, GLfloat _z, GLfloat _h, GLfloat _w, unsigned int _textureID, bool _status);
	
		GLfloat OrigRiga()											{ return x0; }
		GLfloat OrigColonna()										{ return y0; }
		GLfloat OrigZ()												{ return z; }
		GLfloat AltezzaTexture()									{ return h; }
		GLfloat LarghezzaTexture()									{ return w; }

		unsigned int TextureID()									{ return textureID; }
		unsigned int TextureIDselected()							{ return textureIDselected; }

		void setTexture(unsigned int _textureID)					{ textureID = _textureID; }
		void setTextureIDselected(unsigned int _textureIDselected)	{ textureIDselected = _textureIDselected; }
		void setIsSelected(bool status)								{ isSelected = status; }
		void setActive(bool status)									{ active = status; }

		bool getNonSelezionabile()									{ return nonSelezionabile; }
		bool getIsSelected()										{ return isSelected; }
		bool getActive()											{ return active; }

		void ResetTexture();

	private:
		GLfloat x0;
		GLfloat y0;
		GLfloat h;
		GLfloat w;
		GLfloat z;

		unsigned int textureID = 0;
		unsigned int textureIDselected = 0;

		bool isSelected = false;
		bool nonSelezionabile = false;
		bool active = true;
};