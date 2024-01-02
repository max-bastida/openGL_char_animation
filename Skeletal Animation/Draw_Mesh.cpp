//  ========================================================================
//  COSC422: Advanced Computer Graphics;  University of Canterbury (2023)
//	Max Bastida
//
//  FILE NAME: Draw Mesh.cpp
//  ========================================================================
#include <GL/freeglut.h>
using namespace std;

#include <assimp/cimport.h>
#include <assimp/types.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

float base_color[3] = {0.7, 0.4, 0.2};
float shadow_color[3] = { 0.2, 0.15, 0.2 };

glm::vec3 Hips_offset1 = { 0,0,0 };
glm::vec3 LHipJoint_offset1 = { 0,0,0 };
glm::vec3 LeftUpLeg_offset1 = { 1.37959, -1.78713, 0.86582 };
glm::vec3 LeftLeg_offset1 = { 2.54611, -6.99539, 0.00000 };
glm::vec3 LeftFoot_offset1 = { 2.59951, -7.14211, 0.00000 };
glm::vec3 LeftToeBase_offset1 = { 0.18493, -0.50810, 1.96120 };
glm::vec3 LeftToeBase_EndSite1 = { 0.00000, 0.00000, 1.02054 };
glm::vec3 RHipJoint_offset1 = { 0,0,0 };
glm::vec3 RightUpLeg_offset1 = { -1.33564, -1.78713, 0.86582 };
glm::vec3 RightLeg_offset1 = { -2.52705, -6.94302, 0.00000 };
glm::vec3 RightFoot_offset1 = { -2.55964, -7.03257, 0.00000 };
glm::vec3 RightToeBase_offset1 = { -0.25804, - 0.70896, 2.12109 };
glm::vec3 RightToeBase_EndSite1 = { -0.00000, - 0.00000, 1.11821 };
glm::vec3 LowerBack_offset1 = { 0, 0, 0 };
glm::vec3 Spine_offset1 = { -0.02031, 2.24248, - 0.18900 };
glm::vec3 Spine1_offset1 = { 0.08342, 2.24689, - 0.03799 };
glm::vec3 Neck_offset1 = { 0,0,0 };
glm::vec3 Neck1_offset1 = { -0.11607, 1.55844, 0.24628 };
glm::vec3 Head_offset1 = { 0.06877, 1.60174, - 0.17013 };
glm::vec3 Head_EndSite1 = { 0.03358, 1.61266, - 0.08543 };
glm::vec3 LeftShoulder_offset1 = { 0,0,0 };
glm::vec3 LeftArm_offset1 = { 3.47537, 0.55866, - 0.72348 };
glm::vec3 LeftForeArm_offset1 = { 5.38154, - 0.00000, 0.00000 };
glm::vec3 LeftHand_offset1 = { 3.33312, - 0.00000, 0.00000 };
glm::vec3 LeftFingerBase_offset1 = { 0,0,0 };
glm::vec3 LeftHandIndex1_offset1 = { 0.71079, - 0.00000, 0.00000 };
glm::vec3 LeftHandIndex1_EndSite1 = { 0.57306, - 0.00000, 0.00000 };
glm::vec3 LThumb_offset1 = { 0,0,0 };
glm::vec3 LThumb_EndSite1 = { 0.58181, - 0.00000, 0.58181 };
glm::vec3 RightShoulder_offset1 = { 0,0,0 };
glm::vec3 RightArm_offset1 = { -3.27844, 0.90739, - 0.35708 };
glm::vec3 RightForeArm_offset1 = { -5.16954, - 0.00000, 0.00000 };
glm::vec3 RightHand_offset1 = { -3.38287, - 0.00000, 0.00000 };
glm::vec3 RightFingerBase_offset1 = { 0,0,0 };
glm::vec3 RightHandIndex1_offset1 = { -0.81797, - 0.00000, 0.00000 };
glm::vec3 RightHandIndex_EndSite1 = { -0.65947, - 0.00000, 0.00000 };
glm::vec3 RThumb_offset1 = { 0,0,0 };
glm::vec3 RThumb_EndSite1 = { -0.66953, - 0.00000, 0.66953 };

glm::vec3 Hips_offset2 = { 0,0,0 };
glm::vec3 LHipJoint_offset2 = { 0,0,0 };
glm::vec3 LeftUpLeg_offset2 = { 1.30186, - 1.90722, 0.65024 };
glm::vec3 LeftLeg_offset2 = { 2.56413, - 7.04489, 0.00000 };
glm::vec3 LeftFoot_offset2 = { 2.79321, - 7.67428, 0.00000 };
glm::vec3 LeftToeBase_offset2 = { 0.15723, - 0.43198, 2.08537 };
glm::vec3 LeftToeBase_EndSite2 = { 0.00000, - 0.00000, 1.07411 };
glm::vec3 RHipJoint_offset2 = { 0,0,0 };
glm::vec3 RightUpLeg_offset2 = { -1.28627, - 1.90723, 0.65024 };
glm::vec3 RightLeg_offset2 = { -2.56567, - 7.04913, 0.00000 };
glm::vec3 RightFoot_offset2 = { -2.75597, - 7.57198, 0.00000 };
glm::vec3 RightToeBase_offset2 = { -0.17500, - 0.48081, 2.22901 };
glm::vec3 RightToeBase_EndSite2 = { -0.00000, - 0.00000, 1.14762 };
glm::vec3 LowerBack_offset2 = { 0, 0, 0 };
glm::vec3 Spine_offset2 = { -0.04522, 2.07467, - 0.16566 };
glm::vec3 Spine1_offset2 = { -0.01178, 2.08825, - 0.05393 };
glm::vec3 Neck_offset2 = { 0,0,0 };
glm::vec3 Neck1_offset2 = { -0.05182, 1.80489, 0.16014 };
glm::vec3 Head_offset2 = { 0.14739, 1.78840, - 0.33342 };
glm::vec3 Head_EndSite2 = { 0.04364, 1.83899, - 0.13460 };
glm::vec3 LeftShoulder_offset2 = { 0,0,0 };
glm::vec3 LeftArm_offset2 = { 3.01264, 1.29090, - 0.06579 };
glm::vec3 LeftForeArm_offset2 = { 5.55680, - 0.00000, 0.00000 };
glm::vec3 LeftHand_offset2 = { 3.63979, - 0.00000, - 0.00000 };
glm::vec3 LeftFingerBase_offset2 = { 0,0,0 };
glm::vec3 LeftHandIndex1_offset2 = { 0.56942, - 0.00000, - 0.00000 };
glm::vec3 LeftHandIndex1_EndSite2 = { 0.45908, - 0.00000, - 0.00000 };
glm::vec3 LThumb_offset2 = { 0,0,0 };
glm::vec3 LThumb_EndSite2 = { 0.46609, - 0.00000, 0.46609 };
glm::vec3 RightShoulder_offset2 = { 0,0,0 };
glm::vec3 RightArm_offset2 = { -3.15796, 1.37283, - 0.36748 };
glm::vec3 RightForeArm_offset2 = { -5.34339, - 0.00000, 0.00000 };
glm::vec3 RightHand_offset2 = { -3.65101, - 0.00000, - 0.00000 };
glm::vec3 RightFingerBase_offset2 = { 0,0,0 };
glm::vec3 RightHandIndex1_offset2 = { -0.69425, - 0.00000, - 0.00000 };
glm::vec3 RightHandIndex_EndSite2 = { -0.55972, - 0.00000, - 0.00000 };
glm::vec3 RThumb_offset2 = { 0,0,0 };
glm::vec3 RThumb_EndSite2 = { -0.56827, - 0.00000, 0.56827 };

void drawJoint(float size) {
	glPushMatrix();
	glutSolidSphere(size, 8, 8);
	glPopMatrix();
}

void drawLimb(glm::vec3 offset, float size) {
	drawJoint(sqrt(2) * size/2);

	glm::vec3 norm_offset = glm::normalize(offset);
	float angle = glm::degrees(acos(dot({ 1,0,0 }, norm_offset)));
	glm::vec3 axis = cross({ 1,0,0 }, norm_offset);

	float length = sqrt(pow(offset[0], 2) + pow(offset[1],2) + pow(offset[2],2));
	glPushMatrix();
	glRotatef(angle,axis[0], axis[1], axis[2]);
	glScalef(length, size, size);
	glTranslatef(0.5, 0, 0);
	glutSolidCube(1);
	glPopMatrix();
}

void drawUpperLeg(glm::vec3 offset, float size) {
	drawJoint(sqrt(2) * size / 2);

	glm::vec3 norm_offset = glm::normalize(offset);
	float angle = glm::degrees(acos(dot({ 1,0,0 }, norm_offset)));
	glm::vec3 axis = cross({ 1,0,0 }, norm_offset);

	float length = sqrt(pow(offset[0], 2) + pow(offset[1], 2) + pow(offset[2], 2));
	glPushMatrix();
	glRotatef(angle, axis[0], axis[1], axis[2]);
	glTranslatef(0.5 * length, 0, 0);
	glScalef(length/2 + (sqrt(2) * size /2), size, size);
	glutSolidSphere(1, 11, 11);
	glPopMatrix();

}

void drawUpperArm(glm::vec3 offset, float size) {
	drawJoint(size * 1.1);

	glm::vec3 norm_offset = glm::normalize(offset);
	float angle = glm::degrees(acos(dot({ 1,0,0 }, norm_offset)));
	glm::vec3 axis = cross({ 1,0,0 }, norm_offset);

	float length = sqrt(pow(offset[0], 2) + pow(offset[1], 2) + pow(offset[2], 2));
	glPushMatrix();
	glRotatef(angle, axis[0], axis[1], axis[2]);
	glTranslatef(0.5 * length, 0, 0);
	glScalef(length / 2 + (sqrt(2) * size / 2), size, size);
	glutSolidSphere(1, 8, 8);
	glPopMatrix();
}

void drawLowerLeg(glm::vec3 offset, float size) {
	drawJoint(sqrt(2) * size / 2);

	glm::vec3 norm_offset = glm::normalize(offset);
	float angle = glm::degrees(acos(dot({ 1,0,0 }, norm_offset)));
	glm::vec3 axis = cross({ 1,0,0 }, norm_offset);

	float length = sqrt(pow(offset[0], 2) + pow(offset[1], 2) + pow(offset[2], 2));
	glPushMatrix();
	glRotatef(angle, axis[0], axis[1], axis[2]);
	glTranslatef(0.25 * length, 0, -0.3);
	glScalef(length / 4 + (sqrt(2) * size / 2), size, size);
	glutSolidSphere(1, 8, 8);
	glPopMatrix();

	glPushMatrix();
	glRotatef(angle, axis[0], axis[1], axis[2]);
	glTranslatef(0.5 * length, 0, 0);
	glScalef(length / 2 + (sqrt(2) * size / 2), size*0.75, size*0.75);
	glutSolidSphere(1, 8, 8);
	glPopMatrix();
}

void drawForeArm(glm::vec3 offset, float size) {
	drawJoint(sqrt(2) * size / 2);

	glm::vec3 norm_offset = glm::normalize(offset);
	float angle = glm::degrees(acos(dot({ 1,0,0 }, norm_offset)));
	glm::vec3 axis = cross({ 1,0,0 }, norm_offset);

	float length = sqrt(pow(offset[0], 2) + pow(offset[1], 2) + pow(offset[2], 2));
	glPushMatrix();
	glRotatef(angle, axis[0], axis[1], axis[2]);
	glTranslatef(0.3 * length, 0, 0);
	glScalef(length * 0.3 + (sqrt(2) * size / 2), size*0.9, size*0.9);
	glutSolidSphere(1, 8, 8);
	glPopMatrix();

	glPushMatrix();
	glRotatef(angle, axis[0], axis[1], axis[2]);
	glTranslatef(0.5 * length, 0, 0);
	glScalef(length / 2 + (sqrt(2) * size / 2), size * 0.75, size * 0.75);
	glutSolidSphere(1, 8, 8);
	glPopMatrix();
}

void drawHead(glm::vec3 offset) {
	//face plane
	glPushMatrix();
		glTranslatef(offset[0], offset[1]/2 + 0.1, 0.8);
		glRotatef(-15, 1, 0, 0);
		glScalef(1.4, offset[1], 0.8);
		glutSolidSphere(1, 8, 8);
	glPopMatrix();
	//cranium
	glPushMatrix();
		glTranslatef(offset[0], offset[1], offset[2]);
		glScalef(1, 1.1, 1);
		glutSolidSphere(1.6, 8, 8);
	glPopMatrix();
	//nose
	glPushMatrix();
		glTranslatef(offset[0], offset[1]-0.2, offset[2] + 1.2);
		glRotatef(-30, 1, 0, 0);
		glScalef(0.75, 2, 1);
		glutSolidCube(1);
	glPopMatrix();
}

void drawBlindfold(glm::vec3 offset, bool shadow) {
	glPushMatrix();
	if (!shadow) { glColor3f(0.2, 0.4, 0.6); }
		glTranslatef(offset[0], offset[1] + 0.3, offset[2]);
		glRotatef(68, 1, 0, 0);
		glScalef(1, 1.1, 1.1);
		glutSolidCylinder(1.75, 1.1, 10, 10);
		//knot
		glPushMatrix();
			glTranslatef(0,-1.8,0.2);
			glutSolidCube(1);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(0.4, -1.8, 1);
			glRotatef(30, 0, 1, 0);
			glScalef(0.8,0.25,2);
			glutSolidCube(1);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(-0.4, -1.8, 1);
			glRotatef(-30, 0, 1, 0);
			glScalef(0.8, 0.25, 2);
			glutSolidCube(1);
		glPopMatrix();
	glPopMatrix();
	if (!shadow) { glColor3f(base_color[0], base_color[1], base_color[2]); }
}

void drawHips(glm::vec3 offset) {
	float length = sqrt(pow(offset[0], 2) + pow(offset[1], 2) + pow(offset[2], 2));
	glPushMatrix();
	glRotatef(-15, 1, 0, 0);
	glScalef(0.75,0.9,0.5);
	glutSolidSphere(length, 8, 8);
	glPopMatrix();
}

void drawSkullJoint(float size) {
	glPushMatrix();
	glTranslatef(0, 1.2, 0);
	glScalef(3, 1.5, 1.5);
	glutSolidSphere(size, 8, 8);
	glPopMatrix();
}

void drawBack(float size) {
	//return;
	drawSkullJoint(0.75);
	glPushMatrix();
	glTranslatef(0, -0.5, 0);
	glScalef(0.85, 0.85, 0.6);
	glutSolidSphere(size, 8, 8);
	glPopMatrix();
}

void drawWaist(float size) {
	glPushMatrix();
	glScalef(1, 1.5, 0.5);
	glTranslatef(0, -size / 2, 0);
	glRotatef(-90, 1, 0, 0);
	glutSolidCylinder(size, size, 8, 8);
	glPopMatrix();
}

void drawAss(glm::vec3 offset, float size) {

	glm::vec3 norm_offset = glm::normalize(offset);
	float angle = glm::degrees(acos(dot({ 1,0,0 }, norm_offset)));
	glm::vec3 axis = cross({ 1,0,0 }, norm_offset);

	float length = sqrt(pow(offset[0], 2) + pow(offset[1], 2) + pow(offset[2], 2));
	glPushMatrix();
	glTranslatef(0, 0, -1);
	glRotatef(-20, 1, 0, 0);
	glRotatef(angle, axis[0], axis[1], axis[2]);
	glTranslatef(0.5 * length+0.2, 0, 0);
	glScalef(length / 2 + (sqrt(2) * size / 4), size, size);
	glutSolidSphere(1, 11, 11);
	glPopMatrix();
}

void drawNeck(glm::vec3 offset, float size) {
	drawJoint(size /2);

	glm::vec3 norm_offset = glm::normalize(offset);
	float angle = glm::degrees(acos(dot({ 1,0,0 }, norm_offset)));
	glm::vec3 axis = cross({ 1,0,0 }, norm_offset);

	float length = sqrt(pow(offset[0], 2) + pow(offset[1], 2) + pow(offset[2], 2));
	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	glRotatef(angle, axis[0], axis[1], axis[2]);
	glutSolidCylinder(size/2, length*2, 8, 8);
	glPopMatrix();
}

void drawShoulder(glm::vec3 offset, float size) {
	glm::vec3 norm_offset = glm::normalize(offset);
	float angle = glm::degrees(acos(dot({ 1,0,0 }, norm_offset)));
	glm::vec3 axis = cross({ 1,0,0 }, norm_offset);

	float length = sqrt(pow(offset[0], 2) + pow(offset[1], 2) + pow(offset[2], 2));

	glPushMatrix();
	//glTranslatef(0, 0, 0.2);
	glRotatef(angle, axis[0], axis[1], axis[2]);
	glTranslatef(length / 2 - (sqrt(2) * size / 4), 0, 0);
	glScalef(length / 2 + (sqrt(2) * size / 2), size * 0.75, size * 0.75);
	glutSolidSphere(1, 8, 8);
	glPopMatrix();
}

void draw_mesh1(const aiNode* node, bool shadow)
{
	if (shadow) { glColor3f(shadow_color[0], shadow_color[1], shadow_color[2]); }
	else { glColor3f(base_color[0], base_color[1], base_color[2]); }

	if (node->mNumMeshes > 0)
	{
		if (node->mName == aiString("Hips"))
		{
			//drawHips(LeftUpLeg_offset1);
		}
		else if (node->mName == aiString("LHipJoint"))
		{
			drawAss(LeftUpLeg_offset1, 1.3);
		}
		else if (node->mName == aiString("LeftUpLeg"))
		{
			drawUpperLeg(LeftLeg_offset1, 1.25);
		}
		else if (node->mName == aiString("LeftLeg"))
		{
			drawLowerLeg(LeftFoot_offset1, 1);
		}
		else if (node->mName == aiString("LeftFoot"))
		{
			drawLimb(LeftToeBase_offset1, 1);
		}
		else if (node->mName == aiString("LeftToeBase"))
		{
			drawLimb(LeftToeBase_EndSite1, 1);
		}
		else if (node->mName == aiString("RHipJoint"))
		{
			drawAss(RightUpLeg_offset1, 1.3);
		}
		else if (node->mName == aiString("RightUpLeg"))
		{
			drawUpperLeg(RightLeg_offset1, 1.25);
		}
		else if (node->mName == aiString("RightLeg"))
		{
			drawLowerLeg(RightFoot_offset1, 1);
		}
		else if (node->mName == aiString("RightFoot"))
		{
			drawLimb(RightToeBase_offset1, 1);
		}
		else if (node->mName == aiString("RightToeBase"))
		{
			drawLimb(RightToeBase_EndSite1, 1);
		}
		else if (node->mName == aiString("LowerBack"))
		{
			drawHips(LeftUpLeg_offset1);
		}
		else if (node->mName == aiString("Spine"))
		{
			drawWaist(1.5);
		}
		else if (node->mName == aiString("Spine1"))
		{
			drawLimb(Neck_offset1, 1);
		}
		else if (node->mName == aiString("Neck"))
		{
			drawBack(3);
		}
		else if (node->mName == aiString("Neck1"))
		{
			drawNeck(Head_offset1, 1.5);
		}
		else if (node->mName == aiString("Head"))
		{
			drawJoint(0.75);
			drawHead(Head_EndSite1);
			drawBlindfold(Head_EndSite1, shadow);
			drawLimb(Head_EndSite1, 1);
		}
		else if (node->mName == aiString("LeftShoulder"))
		{
			drawShoulder(LeftArm_offset1, 2);
		}
		else if (node->mName == aiString("LeftArm"))
		{
			drawUpperArm(LeftForeArm_offset1, 0.8);
		}
		else if (node->mName == aiString("LeftForeArm"))
		{
			drawForeArm(LeftHand_offset1, 0.7);
		}
		else if (node->mName == aiString("LeftHand"))
		{
			drawLimb(LeftFingerBase_offset1, 1);
		}
		else if (node->mName == aiString("LeftFingerBase"))
		{
			drawLimb(LeftHandIndex1_offset1, 1);
		}
		else if (node->mName == aiString("LeftHandIndex1"))
		{
			drawLimb(LeftHandIndex1_EndSite1, 1);
		}
		else if (node->mName == aiString("LThumb"))
		{
			drawLimb(LThumb_EndSite1, 1);
		}
		else if (node->mName == aiString("RightShoulder"))
		{
			drawShoulder(RightArm_offset1, 2);
		}
		else if (node->mName == aiString("RightArm"))
		{
			drawUpperArm(RightForeArm_offset1, 0.8);
		}
		else if (node->mName == aiString("RightForeArm"))
		{
			drawForeArm(RightHand_offset1, 0.7);
		}
		else if (node->mName == aiString("RightHand"))
		{
			drawLimb(RightFingerBase_offset1, 1);
		}
		else if (node->mName == aiString("RightFingerBase"))
		{
			drawLimb(RightHandIndex1_offset1, 1);
		}
		else if (node->mName == aiString("RightHandIndex1"))
		{
			drawLimb(RightHandIndex_EndSite1, 1);
		}
		else if (node->mName == aiString("RThumb"))
		{
			drawLimb(RThumb_EndSite1, 1);
		}
	}
}

void draw_mesh2(const aiNode* node, bool shadow)
{
	if (shadow) { glColor3f(shadow_color[0], shadow_color[1], shadow_color[2]); }
	else { glColor3f(base_color[0], base_color[1], base_color[2]); }
	if (node->mNumMeshes > 0)
	{
		if (node->mName == aiString("Hips"))
		{
			//drawHips(LeftUpLeg_offset2);
		}
		else if (node->mName == aiString("LHipJoint"))
		{
			drawAss(LeftUpLeg_offset2, 1.2);
		}
		else if (node->mName == aiString("LeftUpLeg"))
		{
			drawUpperLeg(LeftLeg_offset2, 1.2);
		}
		else if (node->mName == aiString("LeftLeg"))
		{
			drawLowerLeg(LeftFoot_offset2, 1);
		}
		else if (node->mName == aiString("LeftFoot"))
		{
			drawLimb(LeftToeBase_offset2, 1);
		}
		else if (node->mName == aiString("LeftToeBase"))
		{
			drawLimb(LeftToeBase_EndSite2, 1);
		}
		else if (node->mName == aiString("RHipJoint"))
		{
			drawAss(RightUpLeg_offset1, 1.2);
		}
		else if (node->mName == aiString("RightUpLeg"))
		{
			drawUpperLeg(RightLeg_offset2, 1.2);
		}
		else if (node->mName == aiString("RightLeg"))
		{
			drawLowerLeg(RightFoot_offset2, 1);
		}
		else if (node->mName == aiString("RightFoot"))
		{
			drawLimb(RightToeBase_offset2, 1);
		}
		else if (node->mName == aiString("RightToeBase"))
		{
			drawLimb(RightToeBase_EndSite2, 1);
		}
		else if (node->mName == aiString("LowerBack"))
		{
			drawHips(LeftUpLeg_offset1);
		}
		else if (node->mName == aiString("Spine"))
		{
			drawWaist(1.5);
		}
		else if (node->mName == aiString("Spine1"))
		{
			drawLimb(Neck_offset2, 1);
		}
		else if (node->mName == aiString("Neck"))
		{
			drawBack(3);
		}
		else if (node->mName == aiString("Neck1"))
		{
			drawNeck(Head_offset1, 1.5);
		}
		else if (node->mName == aiString("Head"))
		{
			drawJoint(0.75);
			drawHead(Head_EndSite2);
			drawLimb(Head_EndSite2, 1);
		}
		else if (node->mName == aiString("LeftShoulder"))
		{
			drawShoulder(LeftArm_offset2, 2);
		}
		else if (node->mName == aiString("LeftArm"))
		{
			drawUpperArm(LeftForeArm_offset2, 0.8);
		}
		else if (node->mName == aiString("LeftForeArm"))
		{
			drawForeArm(LeftHand_offset2, 0.7);
		}
		else if (node->mName == aiString("LeftHand"))
		{
			drawLimb(LeftFingerBase_offset2, 0.6);
		}
		else if (node->mName == aiString("LeftFingerBase"))
		{
			drawLimb(LeftHandIndex1_offset2, 0.4);
		}
		else if (node->mName == aiString("LeftHandIndex1"))
		{
			drawLimb(LeftHandIndex1_EndSite2, 0.3);
		}
		else if (node->mName == aiString("LThumb"))
		{
			drawLimb(LThumb_EndSite2, 0.4);
		}
		else if (node->mName == aiString("RightShoulder"))
		{
			drawShoulder(RightArm_offset2, 2);
		}
		else if (node->mName == aiString("RightArm"))
		{
			drawUpperArm(RightForeArm_offset2, 0.8);
		}
		else if (node->mName == aiString("RightForeArm"))
		{
			drawForeArm(RightHand_offset2, 0.7);
		}
		else if (node->mName == aiString("RightHand"))
		{
			drawLimb(RightFingerBase_offset2, 1);
		}
		else if (node->mName == aiString("RightFingerBase"))
		{
			drawLimb(RightHandIndex1_offset2, 1);
		}
		else if (node->mName == aiString("RightHandIndex1"))
		{
			drawLimb(RightHandIndex_EndSite2, 1);
		}
		else if (node->mName == aiString("RThumb"))
		{
			drawLimb(RThumb_EndSite2, 1);
		}
	}
}

void draw_mesh(const aiNode* node, int subject, bool shadow) {
	switch (subject) {
	case 1:
		draw_mesh1(node, shadow);
		break;
	case 2:
		draw_mesh2(node, shadow);
		break;
	}
}
