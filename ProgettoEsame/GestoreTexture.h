#pragma once
#include "Libs.h"

/*	--------------------- DEFINIZIONE DELLA CLASSE ----------------------------------
	Classe GestoreTexture relativa alla gestione delle textures.
	Ha il ruolo principale di caricare la textur in memeoria, settare il suo
	comportamento e ritornare un identificativo ad essa associata.
--------------------------------------------------------------------------------------*/
class GestoreTexture
{
	public:
		static unsigned int CaricaTexture(const char *path);
		static void CaricaTextureBackgroundGame(const char* path, unsigned char* imagedata, int* width, int* height, int* channels);
};
