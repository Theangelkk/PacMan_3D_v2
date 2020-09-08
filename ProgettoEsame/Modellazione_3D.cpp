#include "Modellazione_3D.h"

// Caricamento del Nostro Oggetto 3D
const aiScene* Modellazione_3D::Caricamento_Modello3D(const char* path, aiVector3D* scene_min, aiVector3D* scene_center, aiVector3D* scene_max)
{
	// Caricamento che avviene passando il Path del nostro Oggetto 3D
	const aiScene* Modello3D = aiImportFile(path, aiProcessPreset_TargetRealtime_Quality);
	
	/*
		Se l'Oggetto 3D Esiste andiamo ad effettuare una 
		Opportuna Scalatura in base alla Dimensione e Finestra 3D
	*/
	if (Modello3D)
	{
		Get_Bounding_Box(Modello3D,scene_min, scene_max);
		scene_center->x = (scene_min->x + scene_max->x) / 2.0f;
		scene_center->y = (scene_min->y + scene_max->y) / 2.0f;
		scene_center->z = (scene_min->z + scene_max->z) / 2.0f;

		return Modello3D;
	}

	return nullptr;
}

// Funzione che carica la Mesh partendo dalla Root
void Modellazione_3D::Get_Bounding_Box(const struct aiScene* modello, aiVector3D* min, aiVector3D* max)
{
	aiMatrix4x4 trafo;

	aiIdentityMatrix4(&trafo);

	min->x = min->y = min->z = 1e10f;
	max->x = max->y = max->z = -1e10f;

	Get_BoundingBox_Children(modello, modello->mRootNode, min, max, &trafo);
}

// Funzione relativa al caricamento della Mesh ==> si stanno disegnando tutte le Msh Children partendo dalla Root (quindi hanno una Posizione Locale 
// all'oggetto Root)
void Modellazione_3D::Get_BoundingBox_Children(const struct aiScene* modello, const struct aiNode* nd, aiVector3D* min, aiVector3D* max, aiMatrix4x4* trafo)
{
	aiMatrix4x4 prev;
	unsigned int n = 0, t;

	prev = *trafo;
	aiMultiplyMatrix4(trafo, &nd->mTransformation);

	for (; n < nd->mNumMeshes; ++n) {
		const struct aiMesh* mesh = modello->mMeshes[nd->mMeshes[n]];
		for (t = 0; t < mesh->mNumVertices; ++t)
		{
			aiVector3t<float> tmp = mesh->mVertices[t];
			aiTransformVecByMatrix4(&tmp, trafo);

			min->x = aisgl_min(min->x, tmp.x);
			min->y = aisgl_min(min->y, tmp.y);
			min->z = aisgl_min(min->z, tmp.z);

			max->x = aisgl_max(max->x, tmp.x);
			max->y = aisgl_max(max->y, tmp.y);
			max->z = aisgl_max(max->z, tmp.z);
		}
	}

	for (n = 0; n < nd->mNumChildren; ++n)
		Get_BoundingBox_Children(modello,nd->mChildren[n], min, max, trafo);
	
	*trafo = prev;
}

/* ---------------------------------------------------------------------------- */

// Trasformazione del Colore in RGB in valori Float
void Modellazione_3D::color4_to_float4(const aiColor4D* c, float f[4])
{
	f[0] = c->r;
	f[1] = c->g;
	f[2] = c->b;
	f[3] = c->a;
}

void Modellazione_3D::set_float4(float f[4], float a, float b, float c, float d)
{
	f[0] = a;
	f[1] = b;
	f[2] = c;
	f[3] = d;
}

// Applicazione del Material al nostro oggetto 3D
void Modellazione_3D::Applicazione_Material(const struct aiMaterial *mtl)
{
	float c[4];
	GLenum fill_mode;
	int ret1, ret2;
	aiColor4D diffuse;
	aiColor4D specular;
	aiColor4D ambient;
	aiColor4D emission;

	float shininess, strength;
	int two_sided;
	int wireframe;
	unsigned int max;

	set_float4(c, 0.8f, 0.8f, 0.8f, 1.0f);

	if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &diffuse))
		color4_to_float4(&diffuse, c);

	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, c);

	set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);

	if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_SPECULAR, &specular))
		color4_to_float4(&specular, c);

	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);
	

	set_float4(c, 0.2f, 0.2f, 0.2f, 1.0f);

	if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_AMBIENT, &ambient))
		color4_to_float4(&ambient, c);

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, c);

	set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);

	if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_EMISSIVE, &emission))
		color4_to_float4(&emission, c);

	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, c);

	max = 1;
	ret1 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS, &shininess, &max);

	if (ret1 == AI_SUCCESS) 
	{
		max = 1;
		ret2 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS_STRENGTH, &strength, &max);

		if (ret2 == AI_SUCCESS)
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess * strength);

		else
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

	}
	else 
	{
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0f);
		set_float4(c, 0.0f, 0.0f, 0.0f, 0.0f);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);
	}

	max = 1;

	if (AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_ENABLE_WIREFRAME, &wireframe, &max))
		fill_mode = wireframe ? GL_LINE : GL_FILL;
	else
		fill_mode = GL_FILL;

	glPolygonMode(GL_FRONT_AND_BACK, fill_mode);

	max = 1;

	if ((AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_TWOSIDED, &two_sided, &max)) && two_sided)
		glDisable(GL_CULL_FACE);
	else
		glEnable(GL_CULL_FACE);
}

// Funzione per il Disegno del nostro Oggetto 3D
void Modellazione_3D::Recursive_Render(const struct aiScene *sc, const struct aiNode* nd, GLuint* textureIds)
{
	unsigned int i;
	unsigned int n = 0, t;

	aiMatrix4x4 m = nd->mTransformation;

	GLuint prev_tex_id_idx = 0;

	/* Aggiornamento della Posizione */

	aiTransposeMatrix4(&m);

	glPushMatrix();
	glMultMatrixf((float*)&m);

	/* Disegno di tutte le Mesh Children della Root dell'Oggetto 3D */
	for (; n < nd->mNumMeshes; ++n)
	{
		// Caricamento delle Mesh Children
		const struct aiMesh* mesh = sc->mMeshes[nd->mMeshes[n]];

		if (mesh->HasTextureCoords(0) && textureIds != nullptr)
		{
			// enable first texture as default
			prev_tex_id_idx = 0;
			glBindTexture(GL_TEXTURE_2D, textureIds[prev_tex_id_idx]);
		}

		// Caricamento ed Applicazione dei Material
		Applicazione_Material(sc->mMaterials[mesh->mMaterialIndex]);

		// Nel momento in cui non abbiamo a disposizione Material, abdiamo a disattivare le Luci
		if (mesh->mNormals == NULL)
			glDisable(GL_LIGHTING);
		else
			glEnable(GL_LIGHTING);
		

		// Andiamo a verificare per ogni Mesh Children caricata, che tipologia di Facce del Poligono abbiamo
		for (t = 0; t < mesh->mNumFaces; ++t)
		{
			// Caricamento delle Facce dell'Oggetto 3D
			const struct aiFace* face = &mesh->mFaces[t];
			GLenum face_mode;

			// Se della Facciamo abbiamo N Vertici, allora possiamo dedurre la sua Rappresentazione Geometrica
			switch (face->mNumIndices)
			{
				case 1: face_mode = GL_POINTS; break;

				case 2: face_mode = GL_LINES; break;

				case 3: face_mode = GL_TRIANGLES; break;

				default: face_mode = GL_POLYGON; break;
			}

			if (mesh->HasTextureCoords(0) && textureIds != nullptr)
			{
				// get current texture ID and check if need to enable new texture
				if (mesh->mTextureCoords[1][t].x != prev_tex_id_idx)
				{
					prev_tex_id_idx = mesh->mTextureCoords[1][t].x;
					glBindTexture(GL_TEXTURE_2D, textureIds[prev_tex_id_idx]);
				}
			}

			// Andiamo a Disegnare la Faccia del nostro Oggetto 3D attualmente considerata
			glBegin(face_mode);

			// Per ogni suo Vertice...
			for (i = 0; i < face->mNumIndices; i++)
			{
				int index = face->mIndices[i];

				// Se Possiede un Colore Specifico...
				if (mesh->mColors[0] != NULL)
					glColor4fv((GLfloat*)&mesh->mColors[0][index]);

				// Se sono state calcoalre le Normali per quel Punto...
				if (mesh->mNormals != NULL)
					glNormal3fv(&mesh->mNormals[index].x);

				glVertex3fv(&mesh->mVertices[index].x);
			}
			glEnd();
		}
	}

	/*
		Questa risulta essere una funzione Ricorsiva, partendo dalla Root --> andiamo a disegnare tutti i suoi Children del nostro
		oggetto 3D.
	*/

	for (n = 0; n < nd->mNumChildren; ++n)
		Recursive_Render(sc, nd->mChildren[n],textureIds);

	glPopMatrix();
}

GLuint* Modellazione_3D::LoadGLTextures(const aiScene* scene, const string& pModelPath)
{
	GLuint*	textureIds = nullptr;

	// Check if scene has textures.
	if (scene->HasTextures())
	{
		// Unsignet Int della Texture da passare successivamente in Output
		textureIds = new GLuint[scene->mNumTextures];
		
		glGenTextures(scene->mNumTextures, textureIds);// generate GL-textures ID's
		// upload textures
		for (size_t ti = 0; ti < scene->mNumTextures; ti++)
		{
			glBindTexture(GL_TEXTURE_2D, textureIds[ti]);// Binding of texture name
			//
			//redefine standard texture values
			//
			// We will use linear interpolation for magnification filter
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			// tiling mode
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (scene->mTextures[ti]->achFormatHint[0] & 0x01) ? GL_REPEAT : GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (scene->mTextures[ti]->achFormatHint[0] & 0x01) ? GL_REPEAT : GL_CLAMP);
			// Texture specification
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, scene->mTextures[ti]->mWidth, scene->mTextures[ti]->mHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE,
				scene->mTextures[ti]->pcData);
		}
	}

	return textureIds;
}

void Modellazione_3D::RotationObect(float* angle)
{
	static GLint prev_time = 0;
	static GLint prev_fps_time = 0;
	static int frames = 0;

	int time = glutGet(GLUT_ELAPSED_TIME);

	*angle += (time - prev_time)*0.1;
	prev_time = time;

	frames += 1;

	if ((time - prev_fps_time) > 10) /* update every seconds */
	{
		int current_fps = frames * 10 / (time - prev_fps_time);
		frames = 0;
		prev_fps_time = time;
	}

	glutPostRedisplay();
}