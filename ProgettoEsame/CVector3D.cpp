#include "CVector3D.h"

CVector3D CVector3D::GetOGLPos(int x, int y)
{
	GLint viewport[4];
	GLdouble modelview[16], projection[16];
	GLfloat winX, winY, winZ;
	GLdouble posX, posY, posZ;

	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	glGetIntegerv(GL_VIEWPORT, viewport);
	winX = (GLfloat)x;
	winY = (GLfloat)viewport[3] - (GLfloat)y;
	glReadPixels(x, GLint(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
	gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

	return CVector3D(posX, posY, posZ);
}