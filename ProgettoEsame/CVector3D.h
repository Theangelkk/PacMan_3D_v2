#pragma once
#include "Libs.h"

/*	--------------------- DEFINIZIONE DELLA CLASSE ----------------------------------
	Classe CVector3D relativa alla gestione e creazione di Coordinate 3D per posizionare
	un Oggetto all'interno della Scena.
--------------------------------------------------------------------------------*/
class CVector3D
{
	GLdouble x;
	GLdouble y;
	GLdouble z;

	public:
		CVector3D()	{	x = 0; y = 0; z = 0;	}
		
		static CVector3D GetOGLPos(int x, int y);
		
		CVector3D(GLdouble _x, GLdouble _y, GLdouble _z) {	x = _x; y = _y; z = _z;	}
		
		GLdouble getColonna()	{ return x; }
		GLdouble getRiga()		{ return y; }
		GLdouble getZ()			{ return z; }
};
