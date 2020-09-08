#include "Libs.h"
#include "MenuGame.h"
#include "CallBack.h"

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutInitWindowSize(800, 600);

	glutCreateWindow("PacMan 3D");

	MenuGame::Instance()->setMenuAttivo(true);
	glutDisplayFunc(CallBack::Display_Gioco);
	glutReshapeFunc(CallBack::Resize_Win);

	glutMainLoop();

	return 0;
}
