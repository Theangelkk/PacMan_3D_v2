#pragma once
#include "Libs.h"
#include "ElemTexture.h"
#include "CVector3D.h"
#include "GestoreImpostazioni.h"

/*	--------------------- DEFINIZIONE DELLA CLASSE ----------------------------------
	Classe MenuGame relativa alla gestione del Menu che compare prima di ogni partita.
	La prima funzionalità di questa classe è quella di caricare tutte le textures
	relative a tutte le schermate che compaiono nel Menù. Dopo questa fase di carimento,
	nel Menù sono disponibili diverse opzioni su cui scegliere :
	- Play Game : ->  inizia una nuova partita settando tutte le impostazioni necessarie
					  per l'esecuzione di essa.
	- Best Score : -> Mostra i 5 migliori risultati di sempre tra tutti i giocatori che
					  hanno partecipato al gioco.
	- Settings  :  -> Permette di scegliere il livello di difficoltà del gioco, la
					  possibilità di attivare/disattivare l'audio, scegliere la risoluzione
					  dello schermo durante la partita.
	- Quit      :  -> Esce dal gioco.
--------------------------------------------------------------------------------------*/
class MenuGame
{
	public:
		static MenuGame* Instance();

		bool getMenuAttivo();
		GestoreImpostazioni* getGestoreImp();

		void setMenuAttivo(bool status);

		void InitMenu();
		void DisableMenuInit();
		void Show();

	private:
		MenuGame();							
		static MenuGame* Istanza;

		void CaricaTexture();

		static void MouseHandler(GLint button, GLint state, GLint x, GLint y);
		static void MousePassiveHandler(GLint x, GLint y);

		bool VerificaIntersezione(ElemTexture *elem, GLfloat rigaMouse, GLfloat colonnaMouse);
		void VerificaSelezionePassiva(unordered_map < string, ElemTexture*> TableHash, GLfloat riga, GLfloat colonna);
		bool VerificaSelezioneFreccia(vector<ElemTexture *> contenitore, int & indice, int frecciaDir);
		void VerificaChangeFreccia(int change);

		/*
			Path Texture Main Menu
		*/
		const char *pathTextureBG =						"Texture/Menu/menu_background.png";
		const char *pathTexturePlayGameButton =			"Texture/Menu/play_game.png";
		const char *pathTextureSettingsButton =			"Texture/Menu/settings.png";
		const char *pathTextureBestScoreButton =		"Texture/Menu/BestScore.png";
		const char *pathTextureQuitGameButton =			"Texture/Menu/quit.png";
		const char *pathTexturePlayGameSelectedButton = "Texture/Menu/play_game_selected.png";
		const char *pathTextureQuitGameSelectedButton = "Texture/Menu/quit_selected.png";
		const char *pathTextureSettingsSelectedButton = "Texture/Menu/settings_selected.png";
		const char *pathTextureBestScoreSelectedButton = "Texture/Menu/BestScore_Selected.png";

		/*
			Path Texture Settings Menù
		*/

		//Background
		const char *pathTextureSettingsBG =			"Texture/Menu/Settings/Background_Setting.png";
		const char *pathTextureSettingsBackButton = "Texture/Menu/Settings/back.png";
		//Level
		const char *pathTextureSettingsLevelFacile =	"Texture/Menu/Settings/level_facile.png";
		const char *pathTextureSettingsLevelNormale =	"Texture/Menu/Settings/level_normale.png";
		const char *pathTextureSettingsLevelDifficile = "Texture/Menu/Settings/level_difficile.png";

		//Frecce sinistra, destra
		const char *pathTextureSettingsFrecciaDestraButton =	"Texture/Menu/Settings/freccia_destra.png";
		const char *pathTextureSettingsFrecciaSinistraButton =	"Texture/Menu/Settings/freccia_sinistra.png";

		//Audio
		const char *pathTextureSettingsAudioOn =	"Texture/Menu/Settings/audio_on.png";
		const char *pathTextureSettingsAudioOff =	"Texture/Menu/Settings/audio_off.png";

		//Risoluzione schermo
		const char *pathTextureSettingsSchermoFullHD =		"Texture/Menu/Settings/res_1920x1080.png";
		const char *pathTextureSettingsSchermoHD =			"Texture/Menu/Settings/res_1280x720.png";
		const char *pathTextureSettingsSchermo800 =			"Texture/Menu/Settings/res_800x600.png";
		const char *pathTextureSettingsSchermoFullScreen =	"Texture/Menu/Settings/res_full_screen.png";

		const char *pathTextureSettingsFrecciaDestraSelectedButton =	"Texture/Menu/Settings/freccia_destra_selected.png";
		const char *pathTextureSettingsFrecciaSinistraSelectedButton =	"Texture/Menu/Settings/freccia_sinistra_selected.png";

		/*
			Path Texture BestScore Menù
		*/
		const char *pathTextureBestScoreBG = "Texture/Menu/BestScore/bestscore_back_ground.png";

		bool menuAttivo = false;
		unordered_map <string, ElemTexture*> TableHash;
		unordered_map <string, ElemTexture*> TableHashSettings;
		unordered_map <string, ElemTexture*> TableHashBestScore;
		vector<ElemTexture*> level;
		vector<ElemTexture*> resSchermo;
		vector<ElemTexture*> audio;
		GestoreImpostazioni *gestoreImpostazioni;

		bool FirstTime = true;

		string lastSelected = "";
	
		int indiceLevel = 1;	//Livello normale è quello di default
		int indiceRes = 0;		//800x600 Risoluzione di Default
		int indiceAudio = 0;	//0--> ON 1--> OFF

		/*
			modalita = 0 --> Main Menù
			modalita = 1 --> Settings Menù
			modalita = 2 --> BestScore Menù
		*/
		int modalita = 0;
};



