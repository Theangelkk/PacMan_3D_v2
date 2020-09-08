#pragma once
#include "Libs.h"

/*	--------------------- DEFINIZIONE DELLA CLASSE ----------------------------------
	Classe RandomValueUniform ha il compito di gestire il comportamento randomico
	dei generatori di numeri casuali.
	Funzione utile ogni quanl volta ci sia da calcolare un valore random all'interno
	di un intervallo (molto utile nella classe nemico).
--------------------------------------------------------------------------------------*/

class RandomValueUniform
{
	public:
		static RandomValueUniform* Istance();

		int getRandomValue(int start, int end);

	private:
		RandomValueUniform();					
		static RandomValueUniform* Istanza;

		random_device dev;
};

