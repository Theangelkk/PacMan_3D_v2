#pragma once
#include "Libs.h"
#include "Modellazione_3D.h"
#include "GestoreScena.h"

/*	--------------------- DEFINIZIONE DELLA CLASSE ----------------------------------
	Classe Bonus relativa alla gestione e creazione dei Bonus presenti 
	all'interno del Gioco.
	Vi sono diverse tipologie di Bonus:
		1.	Arancia con valore 5
		2.	Banana con valore 10
		3.	Mela con valore 15
		4.	Peperoncino con valore 20 più la modalità per l'utente di SuperUser
	--------------------------------------------------------------------------------*/
class Bonus
{
	public:
		Bonus(const char* p, int val);
		Bonus(const Bonus &b);					

		static void setBonusEffect(Bonus* b);
		void setPosition(int rig, int col);
		void setDimensionScale(float d);

		int getValore();

		static void StartTimeEffect(int v);
		void enableEatOtherGhosts();
		bool checkEatOtherGhosts();

		void Draw();

	private:

		const struct aiScene* Bonus_Modello3D = nullptr;
		GLuint* texture = nullptr;
		aiVector3D scene_min;
		aiVector3D scene_max;
		aiVector3D scene_center;

		static Bonus* bonusEffect;
		static int TimeEffect;

		const char* Path_Modello3D;
		GLuint Bonus_Lista_Draw = 0;

		float Dim_Scale = 1.0;
		int Valore = 0;
		bool Mode_EatOtherGhosts = false;
		int Riga = 0;
		int Colonna = 0;
};
