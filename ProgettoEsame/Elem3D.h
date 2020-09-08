#pragma once

/*	--------------------- DEFINIZIONE DELLA CLASSE -----------------------------------
	La Classe Astratta Elem3D ha lo scopo di definire le caratteristiche e funzionalità
	degli Oggetti Dinamici 3D (ad esempio Utente e Nemico) necessarie per la Fase di 
	Rendering.
--------------------------------------------------------------------------------------*/
class Elem3D
{
	public:
		virtual void Draw() = 0;
		virtual int getRiga() = 0;
		virtual int getColonna() = 0;
};