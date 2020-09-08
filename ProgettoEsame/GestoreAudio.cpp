#include "GestoreAudio.h"

GestoreAudio* GestoreAudio::Istanza = nullptr;

GestoreAudio* GestoreAudio::Instance()
{
	if (Istanza == nullptr) 
		Istanza = new GestoreAudio();
	
	return Istanza;
}

GestoreAudio::GestoreAudio()
{
	/*
		Creazione dell'Engine del Gestore dell'Audio
	*/
	engine = createIrrKlangDevice();

	if (!engine)
	{
		cout << "Errore Inizializzazione Sound"<<endl;
		engine = nullptr;
		activeAudio = false;
	}
}

/*
	Abilitazione del Suono durante il Gioco
*/
void GestoreAudio::EnableAudio()
{
	if(engine)
		activeAudio = true;
}

/*
	Disabilitazione del Suono durante il Gioco
*/
void GestoreAudio::DisableAudio()
{
	activeAudio = false;
}

void GestoreAudio::StartSoundMenu()
{
	if(engine && activeAudio)
		engine->play2D(audioBackgroundMenu, true);
}

void GestoreAudio::StartSoundGame()
{
	if (engine && activeAudio)
		engine->play2D(audioBackgroundGame, true);
}

void GestoreAudio::PlaySoundClick()
{
	if (engine && activeAudio)
		engine->play2D(audioClick, false);
}

void GestoreAudio::PlaySoundCountDown()
{
	if (engine && activeAudio)
		engine->play2D(audioCountDown, false);
}

void GestoreAudio::PlaySoundEatCoin()
{
	if (engine && activeAudio)
		engine->play2D(audioEatCoin, false);
}

void GestoreAudio::PlaySoundEatBonus()
{
	if (engine && activeAudio)
		engine->play2D(audioEatBonus, false);
}

void GestoreAudio::PlaySoundEatNemici()
{
	if (engine && activeAudio)
		engine->play2D(audioEatNemici, false);
}

void GestoreAudio::PlaySoundRefillGame()
{
	if (engine && activeAudio)
		engine->play2D(audioRefillGame, false);
}

void GestoreAudio::PlaySoundLooseLife()
{
	if (engine && activeAudio)
		engine->play2D(audioLooseLife, false);
}

void GestoreAudio::PlaySoundGameOver()
{
	if (engine && activeAudio)
		engine->play2D(audioGameOver, false);
}

void GestoreAudio::PlaySoundSuperPacman()
{
	if (engine && activeAudio)
		engine->play2D(audioSuperPacman, false);
}

void GestoreAudio::PlaySoundPause()
{
	if (engine && activeAudio)
		engine->play2D(audioPausa, false);
}

/*
	Terminazione di ogni Suono presente all'interno
	del Gioco attualmente.
*/
void GestoreAudio::StopAllSounds()
{
	if (engine && activeAudio)
		engine->stopAllSounds();
}