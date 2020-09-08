#include "Bonus.h"

Bonus* Bonus::bonusEffect = nullptr;
int Bonus::TimeEffect = 3000;

Bonus::Bonus(const char* p, int val)
{
	Path_Modello3D = p;
	Valore = val;

	Bonus_Modello3D = Modellazione_3D::Caricamento_Modello3D(Path_Modello3D, &scene_min, &scene_center, &scene_max);
	texture = Modellazione_3D::LoadGLTextures(Bonus_Modello3D, Path_Modello3D);

	Bonus_Lista_Draw = glGenLists(1);
	glNewList(Bonus_Lista_Draw, GL_COMPILE);
		Modellazione_3D::Recursive_Render(Bonus_Modello3D, Bonus_Modello3D->mRootNode,texture);
	glEndList();
}

/*
	Copy Constructor
*/
Bonus::Bonus(const Bonus &b)
{
	Path_Modello3D = b.Path_Modello3D;
	Valore = b.Valore;
	Bonus_Modello3D = b.Bonus_Modello3D;
	Bonus_Lista_Draw = b.Bonus_Lista_Draw;
	scene_min = b.scene_min;
	scene_center = b.scene_center;
	scene_max = b.scene_max;
	Mode_EatOtherGhosts = b.Mode_EatOtherGhosts;
}

/*
	Animazione Creazione Bonus
*/
void Bonus::setBonusEffect(Bonus* b)
{
	bonusEffect = b;

	b->setDimensionScale(0.0f);
}

void Bonus::StartTimeEffect(int v)
{
	if (bonusEffect != nullptr)
	{
		TimeEffect -= v;

		if (TimeEffect > 0)
		{
			bonusEffect->Dim_Scale += 0.033f;

			glutTimerFunc(v, Bonus::StartTimeEffect, v);

			glutPostRedisplay();
		}
		else
		{
			TimeEffect = 3000;
			bonusEffect = nullptr;
		}
	}
}

int Bonus::getValore()
{
	return Valore;
}

void Bonus::setPosition(int rig, int col)
{
	Riga = rig;
	Colonna = col;
}

void Bonus::setDimensionScale(float d)
{
	Dim_Scale = d;
}

void Bonus::enableEatOtherGhosts()
{
	Mode_EatOtherGhosts = true;
}

bool Bonus::checkEatOtherGhosts()
{
	return Mode_EatOtherGhosts;
}

/*
	Rendering del Bonus 
*/
void Bonus::Draw()
{
	float Tmp = 0.0;

	if (Bonus_Modello3D)
	{
		Tmp = scene_max.x - scene_min.x;
		Tmp = aisgl_max(scene_max.y - scene_min.y, Tmp);
		Tmp = aisgl_max(scene_max.z - scene_min.z, Tmp);

		Tmp = Dim_Scale;

		glPushMatrix();
			glScalef(Tmp, Tmp, Tmp);
			glTranslatef(0.0f + Colonna, GestoreScena::Instance()->GetPosY() - Riga, GestoreScena::Instance()->GetZ());
			glCallList(Bonus_Lista_Draw);
		glPopMatrix();
	}
}
