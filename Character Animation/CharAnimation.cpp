//  ========================================================================
//  COSC422: Advanced Computer Graphics;  University of Canterbury (2023)
//
//  FILE NAME: CharAnimation.cpp
//
//  See Exer08_CharAnimation.pdf for details
//  ========================================================================

#include <iostream>
#include <fstream>
#include <GL/freeglut.h>
#include <IL/il.h>
using namespace std;

#include <assimp/cimport.h>
#include <assimp/types.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "assimp_extras.h"

//----------Globals----------------------------
const aiScene* meshScene = nullptr;
float camAngle = 0, camDist = 0, camNear = 0, camFov = 60.0;		//Camera's parameters
aiVector3D scene_min, scene_max;
int currTick = 0, tDuration = 0; 
float toRad = 3.14159265 / 180.0;   //Conversion from degrees to radians
float timeStep;
GLuint* textureIds = 0;

//----A basic mesh structure that stores initial values of-----
//----vertex coordinates and normal components-----------------
struct meshInit
{
	int mNumVertices;
	aiVector3D* mVertices;
	aiVector3D* mNormals;
};
meshInit* initData;

void floor()
{
	float white[4] = { 1., 1., 1., 1. };
	float black[4] = { 0 };
	glColor4f(0.665, 0.7, 0.5, 1.0);  //The floor is gray in colour
	glNormal3f(0.0, 1.0, 0.0);
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glDisable(GL_TEXTURE_2D);

	//The floor is made up of several tiny squares on a 400 x 400 grid. Each square has a unit size.
	glBegin(GL_QUADS);
	glVertex3f(-2000, 0, -2000);
	glVertex3f(-2000, 0, 2000);
	glVertex3f(2000, 0, 2000);
	glVertex3f(2000, 0, -2000);
	glEnd();


}

//----Loads the character model and stores mesh data------- 
bool loadModel(const char* fileName)
{
	int numVert;
	aiMesh* mesh;
	aiVector3D  scene_center;
	float scene_scale;

	meshScene = aiImportFile(fileName, aiProcessPreset_TargetRealtime_MaxQuality);
	if(meshScene == nullptr) exit(1);
	//printSceneInfo(scene);
	//printMeshInfo(scene);
	//printTreeInfo(scene->mRootNode);
	//printBoneInfo(scene);
	//for(int i = 0; i < scene->mNumAnimations; i++)
	//   printAnimInfo(scene, i);  //<<<WARNING!  This output could contain many many lines!

	//Data structure for storing vertices and normal vectors of the initial mesh.
	initData = new meshInit[meshScene->mNumMeshes];

	//Allocate space using number of vertices in each mesh object
	//and copy data
	for (int i = 0; i < meshScene->mNumMeshes; i++)
	{
		mesh = meshScene->mMeshes[i];
		numVert = mesh->mNumVertices;
		(initData + i)->mNumVertices = numVert;
		(initData + i)->mVertices = new aiVector3D[numVert];
		(initData + i)->mNormals = new aiVector3D[numVert];

		//Populate the arrays using mesh data
		for (int k = 0; k < numVert; k++) {
			(initData + i)->mVertices[k] = mesh->mVertices[k];
			(initData + i)->mNormals[k] = mesh->mNormals[k];
		}
	}

	//Compute camera parameters using model's dimensions
	get_bounding_box(meshScene, &scene_min, &scene_max);
	scene_center = (scene_min + scene_max) * 0.5f;
	aiVector3D scene_diag = scene_max - scene_center;
	scene_scale = scene_diag.Length();
	camNear = scene_scale / tan(camFov * toRad * 0.5);
	camDist = camNear * 1.5;
	return true;
}

//------Loads textures of the model ---------
void loadGLTextures(const aiScene* scene)
{
	ILuint imageId;

	if (scene == nullptr) exit(1);
	if (scene->HasTextures())
	{
		std::cout << "Support for meshes with embedded textures is not implemented" << endl;
		exit(1);
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
	glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
	glEnable(GL_TEXTURE_2D);
	ilInit();   //DevIL initialization

	textureIds = new GLuint[scene->mNumMaterials];
	glGenTextures(scene->mNumMaterials, textureIds);

	for (unsigned int m = 0; m < scene->mNumMaterials; ++m)
	{
		aiString path;  // file path, name

		if (scene->mMaterials[m]->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
		{
			ilGenImages(1, &imageId);
			ilBindImage(imageId); /* Binding of DevIL image name */
			ilEnable(IL_ORIGIN_SET);
			ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
			if (ilLoadImage((ILstring)path.data))   //if success
			{
				/* Convert image to RGBA */
				ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
				glBindTexture(GL_TEXTURE_2D, textureIds[m]);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ilGetInteger(IL_IMAGE_WIDTH),
					ilGetInteger(IL_IMAGE_HEIGHT), 0, GL_RGBA, GL_UNSIGNED_BYTE, ilGetData());
				glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
				cout << "  Texture:" << path.data << " successfully loaded." << endl;
			}
			else
			{
				cout << "Couldn't load Image: " << path.data << endl;
			}
		}

	}  //loop for material
}

void transformVertices() {
	//Construct the transformation matrices required to transform the vertex set represented by each bone
	//Transform corresponding vertices and normal vectors from initial mesh data and assign to char mesh

	aiNode* node;
	aiMesh* mesh;
	aiBone* bone;
	int vid;
	aiVector3D vert;
	aiVector3D norm;
	aiMatrix4x4 Mi;
	aiMatrix4x4 Ai;

	//for each mesh
	for (int m = 0; m < meshScene->mNumMeshes; m++)
	{
		mesh = meshScene->mMeshes[m];
		//for each bone of the mesh
		for (int i = 0; i < mesh->mNumBones; i++) {
			bone = mesh->mBones[i];
			//get offset matrix of the bone
			Mi = bone->mOffsetMatrix;
			node = meshScene->mRootNode->FindNode(bone->mName);
			aiMatrix4x4 Ai = Mi;
			while (node != nullptr)
			{
				Ai = node->mTransformation * Ai;
				node = node->mParent;
			}
			aiMatrix3x3 Ni = aiMatrix3x3(Ai);
			Ni.Transpose();
			Ni.Inverse();
			for (int k = 0; k < bone->mNumWeights; k++) {
				vid = (bone->mWeights[k]).mVertexId;
				vert = (initData + m)->mVertices[vid];
				norm = (initData + m)->mNormals[vid];
				mesh->mVertices[vid] = Ai * vert;

				mesh->mNormals[vid] = Ni * norm;
			}
		}
	}

}


//-----Draws all meshes of the character model by traversing the node tree-----
void render(const aiScene* scene, const aiNode* node)
{
	aiMatrix4x4 m = node->mTransformation;
	aiMesh* mesh;
	aiFace* face;
	aiColor4D diffuse;
	aiMaterial* mtl;
	int meshIndex, materialIndex;
	float materialCol[4] = { 0.8, 0.8, 0.8, 1 };

	m.Transpose();   //Convert to column-major order
	glPushMatrix();
	glMultMatrixf((float*)&m);   //Multiply by the transformation matrix for this node

	// Draw all meshes
	for (int n = 0; n < node->mNumMeshes; n++)
	{
		meshIndex = node->mMeshes[n];
		mesh = scene->mMeshes[meshIndex];

		materialIndex = mesh->mMaterialIndex;  //Get material index attached to the mesh
		mtl = scene->mMaterials[materialIndex];
		if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &diffuse))  //Get material colour from model
			glColor4f(diffuse.r, diffuse.g, diffuse.b, 1.0);
		else
			glColor4fv(materialCol);   //Default material colour

		if (mesh->HasTextureCoords(0))
		{
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, textureIds[materialIndex]);
		}
		else 
			glDisable(GL_TEXTURE_2D);

		
		//Get the polygons from each mesh and draw them
		for (int k = 0; k < mesh->mNumFaces; k++)
		{
			face = &mesh->mFaces[k];
			GLenum face_mode;

			switch(face->mNumIndices)
			{
				case 1: face_mode = GL_POINTS; break;
				case 2: face_mode = GL_LINES; break;
				case 3: face_mode = GL_TRIANGLES; break;
				default: face_mode = GL_POLYGON; break;
			}

			glBegin(face_mode);

			for(int i = 0; i < face->mNumIndices; i++) {
				int vertexIndex = face->mIndices[i]; 

				if(mesh->HasVertexColors(0))
					glColor4fv((GLfloat*)&mesh->mColors[0][vertexIndex]);

				if (mesh->HasTextureCoords(0))
					glTexCoord2f(mesh->mTextureCoords[0][vertexIndex].x, mesh->mTextureCoords[0][vertexIndex].y);

				if (mesh->HasNormals())
					glNormal3fv(&mesh->mNormals[vertexIndex].x);

				glVertex3fv(&mesh->mVertices[vertexIndex].x);
			}

			glEnd();
		}

	}

	// Draw all children of the current node
	for (int i = 0; i < node->mNumChildren; i++)
		render(scene, node->mChildren[i]);

	glPopMatrix();
}

//------Updates the nodes  with matrices computed using the ---------
//------position and rotation keys obtained from keyframes-----------
// Added:  Interpolation between keyframes
void updateNodeMatrices(int tick)
{
	aiAnimation* anim = meshScene->mAnimations[0];
	aiMatrix4x4 matPos, matRot, matProd;
	aiMatrix3x3 matRot3;
	aiNode* node;
	int nPoskeys, nRotkeys, index;
	aiVector3D posn1, posn2, posn;
	aiQuaternion rotn1, rotn2, rotn;
	float factor, time1, time2;

	for (int i = 0; i < anim->mNumChannels; i++)
	{
		matPos = aiMatrix4x4();   //Identity
		matRot = aiMatrix4x4();
		aiNodeAnim* channel = anim->mChannels[i];
		nPoskeys = channel->mNumPositionKeys;
		nRotkeys = channel->mNumRotationKeys;

        if (tick < (channel->mPositionKeys[nPoskeys - 1]).mTime)
		{
			index = 1;
			while (tick >= (channel->mPositionKeys[index]).mTime) index++;
			posn1 = (channel->mPositionKeys[index - 1]).mValue;
			posn2 = (channel->mPositionKeys[index]).mValue;
			time1 = (channel->mPositionKeys[index - 1]).mTime;
			time2 = (channel->mPositionKeys[index]).mTime;
			factor = (tick - time1) / (time2 - time1);
			posn = (1 - factor)*posn1 + factor * posn2;
		}
		else
			posn = (channel->mPositionKeys[nPoskeys - 1]).mValue;

		if (tick < (channel->mRotationKeys[nRotkeys - 1]).mTime)
		{
			index = 1;
			while (tick >= (channel->mRotationKeys[index]).mTime) index++;
			rotn1 = (channel->mRotationKeys[index - 1]).mValue;
			rotn2 = (channel->mRotationKeys[index]).mValue;
			time1 = (channel->mRotationKeys[index - 1]).mTime;
			time2 = (channel->mRotationKeys[index]).mTime;
			factor = (tick - time1) / (time2 - time1);
			rotn.Interpolate(rotn, rotn1, rotn2, factor);
		}
		else
			rotn = (channel->mRotationKeys[nRotkeys - 1]).mValue;

		matPos.Translation(posn, matPos);
		matRot3 = rotn.GetMatrix();
		matRot = aiMatrix4x4(matRot3);

		matProd = matPos * matRot;
		node = meshScene->mRootNode->FindNode(channel->mNodeName);
		node->mTransformation = matProd;
	}
}

void special(int key, int x, int y)
{
	if (key == GLUT_KEY_LEFT) camAngle -= 5;
	else if (key == GLUT_KEY_RIGHT) camAngle += 5;
	glutPostRedisplay();
}


//--------------------OpenGL initialization------------------------
void initialise()
{
	float ambient[4] = { 0.2, 0.2, 0.2, 1.0 };  //Ambient light
	float white[4] = { 1, 1, 1, 1 };		    //Light's colour
	float black[4] = { 0, 0, 0, 1 };
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_SPECULAR, black);   //Disable specular light
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 50);

	loadModel("dwarf.x");			//Specify input file name here
	loadGLTextures(meshScene);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(camFov, 1, camNear, 5000.0);

	tDuration = meshScene->mAnimations[0]->mDuration;
	float fps = meshScene->mAnimations[0]->mTicksPerSecond;
	if (fps < 10) fps = 30;
	timeStep = 1000.0 / fps;     //Animation time step in m.Sec
}

//------The main display function---------
void display()
{
	float lightPosn[4] = { -200, 200, 200, 1 }; 

	float ex = camDist * sin(camAngle * toRad);
	float ez = camDist * cos(camAngle * toRad);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(ex, camDist*0.5, ez, 0, 0, 0, 0, 1, 0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosn);


	floor();
	glPushMatrix();
	   glTranslatef(0, -scene_min.y, 0);   //move the character above floor plane
	   render(meshScene, meshScene->mRootNode);
	glPopMatrix();

	glutSwapBuffers();
}


void update(int value)
{
	if (currTick >= tDuration)
	{
		currTick = 0;
	}
		updateNodeMatrices(currTick);
		transformVertices();
		glutTimerFunc(timeStep, update, 0);
		currTick++;
	glutPostRedisplay();
}


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutCreateWindow("Character Animation");

	initialise();
	glutDisplayFunc(display);
	glutSpecialFunc(special);
	glutTimerFunc(timeStep, update, 0);
	glutMainLoop();
}

