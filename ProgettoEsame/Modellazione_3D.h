#pragma once
#include "Libs.h"

#define aisgl_min(x,y) (x<y?x:y)
#define aisgl_max(x,y) (y>x?y:x)

class Modellazione_3D
{
	public:
		static const aiScene* Caricamento_Modello3D(const char* path, aiVector3D* scene_min, aiVector3D* scene_center, aiVector3D* scene_max);
		static void Recursive_Render(const struct aiScene *sc, const struct aiNode* nd, GLuint* textureIds);
		static GLuint* LoadGLTextures(const aiScene* scene, const string& pModelPath);
		static void RotationObect(float* angle);
	private:
		static void Get_BoundingBox_Children(const struct aiScene* modello, const struct aiNode* nd, aiVector3D* min, aiVector3D* max, aiMatrix4x4* trafo);
		static void Get_Bounding_Box(const struct aiScene* modello, aiVector3D* min, aiVector3D* max);

		static void color4_to_float4(const aiColor4D*c, float f[4]);
		static void set_float4(float f[4], float a, float b, float c, float d);

		static void Applicazione_Material(const struct aiMaterial *mtl);		
};

