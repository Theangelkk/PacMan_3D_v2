#pragma once
#include "Libs.h"

#define ROW  100
#define COL  100

/*	--------------------- DEFINIZIONE DELLA CLASSE ----------------------------------
	Classe TrovaPercorso è responsabile del calcolo del percorso minimo tra
	il pacman e l'utente.
	In background, i metodi di tale classe per trovare il cammino minimo
	utilizzando l'algoritmo A*
--------------------------------------------------------------------------------------*/
class TrovaPercorso
{
	public:
		bool CalcolaProxMovimento(vector<string> grid, int srcX, int srcY, int destX, int destY, bool diagonale, pair<int, int>& prox);
		bool CalcolaListaMovimenti(vector<string> grid, int srcX, int srcY, int destX, int destY, bool diagonale, vector<pair<int, int>>& vertici);
	private:

		/*
			Creazione Tipo Coordinate
		*/
		typedef pair<int, int> Pair;

		typedef pair<double, pair<int, int>> pPair;

		/*
			Struttura che rappresenta una cella del percorso
		*/
		struct cell
		{
			int parent_i, parent_j;
			double f, g, h;
		};

		/*
			Funzione per verificare la Validità della Cella
		*/
		bool isValid(int row, int col);

		/*
			Funzione per verificare se una Cella è Bloccata
		*/
		bool isUnBlocked(vector<string> grid, int row, int col);

		/*
			Funzione per verificare se siamo giunti a Destinazione
		*/
		bool isDestination(int row, int col, Pair dest);

		double calculateHValue(int row, int col, Pair dest);

		/*
			Funzione che permette di calcolare il Percorso da seguire
		*/
		void tracePath(cell cellDetails[][COL], Pair dest);

		/*
			Funzione che permette di determianre il percorso più breve
		*/
		void aStarSearch(vector<string> grid, Pair src, Pair dest, bool diagonale);

		vector< pair<int, int>> listaVertici;
		pair<int, int> proxMovimento;
		bool proxMovimentoTrovato;
		int modalita;
};
