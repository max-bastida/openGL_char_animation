//  ========================================================================
//  COSC422: Advanced Computer Graphics;  University of Canterbury (2023)
//
//  FILE NAME: Skeleton Animation.cpp
//  See Exer04_SkeletalAnimation.pdf for details 
//  ========================================================================

#include <iostream>
#include <fstream>
#include <GL/freeglut.h>
using namespace std;

#include <assimp/cimport.h>
#include <assimp/types.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "assimp_extras.h"
#include "Draw_Mesh.h"
#include "DrawEnvironment.h"
#include "loadBMP.h"
#include <glm/trigonometric.hpp>

//----------Globals----------------------------
const aiScene* scene1 = NULL;
const aiScene* scene2 = NULL;
aiVector3D scene_min, scene_max, scene_center;
float scene_scale;
float timeStep = NULL;
double tDuration = NULL;
GLuint txId[2];
float cam_rotate_angle = 0;
float light[4] = { -5, 80, 60, 1 };
float shadowMat[16] = { light[1], 0, 0, 0, -light[0], 0, -light[2],
						-1, 0, 0, light[1], 0, 0, 0, 0, light[1] };
float bodyColor[3] = {0.7, 0.4, 0.2};


float cam_angle = 90;
float cam_pos[3] = { 0.0, 100.0, 80.0 };
float radius = 80.0;
int camera_type = 0;

void set_camera_position(float angle) {
	//define camera position; call this in display function
	gluLookAt(sin(glm::radians(angle)) * radius, 100, cos(glm::radians(angle)) * radius,
		0, 0,
		0, 0.0, 1.0, 0.0);
}
// ------A recursive function to traverse scene graph and render each mesh----------
// Simplified version for rendering a skeleton mesh
void render(const aiNode* node, int subject, bool shadow)
{
	aiMatrix4x4 m = node->mTransformation;

	m.Transpose();   //Convert to column-major order
	glPushMatrix();
	glMultMatrixf((float*)&m);   //Multiply by the transformation matrix for this node

	//The scene graph for a skeleton contains at most one mesh per node
	//Skeleton meshes are always triangle meshes
	draw_mesh(node, subject, shadow);

	// Recursively draw all children of the current node
	for (int i = 0; i < node->mNumChildren; i++)
		render(node->mChildren[i], subject, shadow);

	glPopMatrix();
}

//----- Function to update node matrices for each tick ------
// Complete this function
void updateNodeMatrices(int tick, const aiScene* scene)
{
	aiAnimation* anim;   //Animation object
	aiMatrix4x4 matPos, matRot, matProd;  //Position, rotation, product matrices
	aiMatrix3x3 matRot3;
	aiNode* node;

	anim = scene->mAnimations[0];
	for (int i = 0; i < anim->mNumChannels; i++)
	{
		matPos = aiMatrix4x4();   //Identity
		matRot = aiMatrix4x4();
		aiNodeAnim* chnl = anim->mChannels[i];      //Channel

		//See Lec04:Slides 31, 32  and complete this section
		aiVector3D posn;
		aiQuaternion rotn;

		if (chnl->mNumRotationKeys == 1)
			rotn = chnl->mRotationKeys[0].mValue;
		else
			rotn = chnl->mRotationKeys[tick].mValue;
		if (chnl->mNumPositionKeys == 1)
			posn = chnl->mPositionKeys[0].mValue;
		else
			posn = chnl->mPositionKeys[tick].mValue;

		matPos.Translation(posn, matPos);
		matRot3 = rotn.GetMatrix();
		matRot = aiMatrix4x4(matRot3);
		matProd = matPos * matRot;


		node = scene->mRootNode->FindNode(chnl->mNodeName);
		node->mTransformation = matProd;
	}
}

//-------------------------------------------------------------------
void loadTexture()
{
	glGenTextures(2, txId); 				// Create a Texture object
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, txId[0]);		//Blue car side with logo
	loadBMP("RedCarpet_S.bmp");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	//Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, txId[1]);		//Blue car side with logo
	loadBMP("wallpaper.bmp");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	//Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	//glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
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
	loadTexture();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40, 1, 1.0, 500.0);

	//---- Load the model ------
	scene1 = aiImportFile("20_13.bvh", aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_Debone);
	if (scene1 == NULL) exit(1);
	scene2 = aiImportFile("21_13.bvh", aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_Debone);
	if (scene2 == NULL) exit(1);
	//printTreeInfo(scene->mRootNode);
	//printAnimInfo(scene, 0);
	

	get_bounding_box(scene1, &scene_min, &scene_max);
	scene_center = (scene_min + scene_max) * 0.5f;
	aiVector3D scene_diag = scene_max - scene_center;
	scene_scale = 1.0 / scene_diag.Length();

	float fps = scene1->mAnimations[0]->mTicksPerSecond;
	timeStep = 500.0 / fps;
	tDuration = scene1->mAnimations[0]->mDuration;
}


//------The main display function---------
void display()
{
	aiMatrix4x4 m1 = scene1->mRootNode->mTransformation;
	float xpos1 = m1.a4;   //Root joint's position in world space
	float ypos1 = m1.b4;
	float zpos1 = m1.c4;
	aiMatrix4x4 m2 = scene2->mRootNode->mTransformation;
	float xpos2 = m2.a4;   //Root joint's position in world space
	float ypos2 = m2.b4;
	float zpos2 = m2.c4;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//gluLookAt(0, 0, 7, 0, 0, 0, 0, 1, 0); //set camera position


	glPushMatrix();
		glScalef(scene_scale, scene_scale, scene_scale);
		set_camera_position(cam_rotate_angle);
		glLightfv(GL_LIGHT0, GL_POSITION, light);
		drawEnvironment(txId);
		render(scene1->mRootNode, 1, false);
		render(scene2->mRootNode, 2, false);
		glDisable(GL_LIGHTING);
		glPushMatrix();
			glMultMatrixf(shadowMat);
			render(scene1->mRootNode, 1, true);
			render(scene2->mRootNode, 2, true);
		glPopMatrix();
		glEnable(GL_LIGHTING);
	glPopMatrix();

	glutSwapBuffers();
}

void update(int tick) {
	cam_rotate_angle += 0.1;
	if (tick > tDuration)
	{
		tick = 0;
	}
	updateNodeMatrices(tick, scene1);
	updateNodeMatrices(tick, scene2);
	tick++;
	glutTimerFunc(timeStep, update, tick);
	glutPostRedisplay();

}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(800, 800);
	glutCreateWindow("Skeleton Animation");

	initialise();
	glutDisplayFunc(display);
	glutTimerFunc(timeStep, update, 0);
	glutMainLoop();
}

