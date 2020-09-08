#pragma once
#include "Libs.h"
#include "Elem3D.h"
#include "Modellazione_3D.h"
#include "GestoreUtente.h"
#include "TrovaPercorso.h"
#include "GestoreScena.h"
#include "RandomValueUniform.h"

/*	--------------------- DEFINIZIONE DELLA CLASSE ----------------------------------
	Classe Nemico è responsabile di tutti i controlli e tutte le operazioni eseguite
	durante il ciclo di vita del nemico, ossia :
	- comportamenti relativi all'inseguimento del nemico sul pacman quando l'utente
	  non è super utente
	- comportamenti relativi alla fuga del nemico dal pacman quando l'utente
	  è super utente
	- comportamenti in seguito alla collisione tra il nemico ed il pacman
	- comportamenti relativi alla morte del nemico
--------------------------------------------------------------------------------------*/
class Nemico : public Elem3D
{
	public:
		Nemico() {};
		Nemico(double _riga, double _colonna, const char* p, const char* m);

		int getRiga() override;
		int getColonna() override;
		bool getSonoMorto();
		void getParametri(int &RigaNemico, int &ColonnaNemico, bool &_sonoMorto);

		void Draw() override;

		void setRiga(int _riga);
		void setColonna(int _col);
		void setRiga(double _riga);
		void setColonna(double _col);
		void setQuadrante(int riga1, int colonna1, int riga2, int colonna2);
		void setPuoMangiare(bool status);

		void GeneraCelleDisp();
		void impostaMorte();
		void Movimento();

	private:
		void checkPosizione(vector<string> strutturaGioco);
		double Riga;
		double Colonna;
		pair<int, int> vertice;

		vector<pair<int, int>> quadrante;
		vector<pair<int, int>> celle_disponibili;
		vector<pair<int, int>> listaVertici;
		vector<pair<int, int>> listaVerticiMorte;
		vector<pair<int, int>> listaVerticiMorteRitorno;
		int contaNumeroMov;
		int cornerDaRaggiungere;
		bool sonoMorto;
		bool puoMangiare = false;

		const struct aiScene* Nemico_Modello3D = nullptr;
		GLuint* texture = nullptr;
		GLuint Nemico_Lista_Draw = 0;

		const struct aiScene* Nemico_Modello3D_Morto = nullptr;
		GLuint* texture_morto = nullptr;
		GLuint Nemico_Morto_Lista_Draw = 0;

		aiVector3D scene_min;
		aiVector3D scene_max;
		aiVector3D scene_center;

		const char* Path_Modello3D;
		const char* Path_Modello3D_Morto;
};
