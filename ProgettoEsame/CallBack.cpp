#include "CallBack.h"

void CallBack::Display_Gioco()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/*
		Se il gioco è attualmente nella Scena del Menù, andiamo a richiamare
		le funzioni relative al Disegno del menù
	*/
	if (!MenuGame::Instance()->getMenuAttivo())
	{
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(0.0, -10.0, 5.0, 0.0, 0.0, -91.0, 0.0, 1.0, 0.0);

		RichiestaGUI::DisegnaCircuito();	
		RichiestaGUI::DisegnaElementi3D();	
	}
	/*
		Se invece il gioco è attualmente nella Scena del Game, andiamo a richiamare
		le funzioni relative al Disegno del Game
	*/
	else
	{
		glLoadIdentity();
		MenuGame::Instance()->Show();
	}

	glutSwapBuffers();
	glutPostRedisplay();
}

/*
	Il Metodo Resize_Win non permette di Ridimensionare
	la finestra del Gioco secondo le volontà dell'utente.
	Ogni qual volta si prova a modificare la Dimensione dell'applicativo,
	verrà automaticamente reimpostato le dimensione adatte della Scena
*/
void CallBack::Resize_Win(int width, int height)
{
	/*
		Se il Menù non è attivo, e quindi ci troviamo durante il gioco
	*/
	if (!MenuGame::Instance()->getMenuAttivo())
	{
		/*
			Se il gioco si trova in modalità full screen, non deve fare nulla
		*/
		if (MenuGame::Instance()->getGestoreImp()->getFullScreen())
			return;

		pair <int, int> res = MenuGame::Instance()->getGestoreImp()->getResSchermo();
		glutReshapeWindow(res.first, res.second);
	}
	/*
		Se prova a fare il resize quando il Menù è attivo, 
		la risoluzione è impostata sempre a 800x600.
	*/
	else
		glutReshapeWindow(800, 600);
}