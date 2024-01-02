//  ========================================================================
//  COSC422: Advanced Computer Graphics;  University of Canterbury (2023)
//	Max Bastida
//
//  FILE NAME: Draw Environment.cpp
//  ========================================================================

#include <cmath>
#include <GL/freeglut.h>
using namespace std;

float floor_radius = 50;

void floor(GLuint* txId)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, txId[0]);
	glNormal3f(0.0, 1.0, 0.0);
	glColor3f(1, 1, 1);

	float grid_size = 25;
	float tex_coords = floor_radius * 2 / grid_size;
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(-floor_radius, -0.1, -floor_radius);
		glTexCoord2f(tex_coords, 0.0);
		glVertex3f(floor_radius, -0.1, -floor_radius);
		glTexCoord2f(tex_coords, tex_coords);
		glVertex3f(floor_radius, -0.1, floor_radius);
		glTexCoord2f(0.0, tex_coords);
		glVertex3f(-floor_radius, -0.1, floor_radius);
	glEnd();
	glDisable(GL_TEXTURE_2D);

}

void walls(GLuint* txId) {
	//skirting
	glColor3f(1, 1, 1);

	glPushMatrix();
	glTranslatef(0, 1.4, 50);
	glScalef(100, 3, 0.5);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 1.4, -50);
	glScalef(100, 3, 0.5);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(50, 1.4, 0);
	glScalef(0.5, 3, 100);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-50, 1.4, 0);
	glScalef(0.5, 3, 100);
	glutSolidCube(1);
	glPopMatrix();

	//walls
	float grid_size = 25;
	float wall_height = 30;
	float tex_coords = floor_radius * 2 / grid_size;

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, txId[1]);
	glNormal3f(0.0, 1.0, 0.0);
	glColor3f(1, 1, 1);

	glNormal3f(0.0, 0.0, -1.0);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-floor_radius, -0.1, floor_radius); // 0,0,1
	glTexCoord2f(tex_coords, 0.0);
	glVertex3f(floor_radius, -0.1, floor_radius); // 1,0,1
	glTexCoord2f(tex_coords, 1);
	glVertex3f(floor_radius, wall_height, floor_radius); // 1,1,1
	glTexCoord2f(0.0, 1);
	glVertex3f(-floor_radius, wall_height, floor_radius); //0,1,1
	glEnd();

	glNormal3f(0.0, 0.0, 1.0);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-floor_radius, -0.1, -floor_radius); // 0,0,0
	glTexCoord2f(tex_coords, 0.0);
	glVertex3f(floor_radius, -0.1, -floor_radius); // 1,0,0
	glTexCoord2f(tex_coords, 1);
	glVertex3f(floor_radius, wall_height, -floor_radius); // 1,1,0
	glTexCoord2f(0.0, 1);
	glVertex3f(-floor_radius, wall_height, -floor_radius); //0,1,0
	glEnd();

	glNormal3f(-1.0, 0.0, 0.0);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(floor_radius, -0.1, -floor_radius); // 1,0,0
	glTexCoord2f(tex_coords, 0.0);
	glVertex3f(floor_radius, -0.1, floor_radius); // 1,0,1
	glTexCoord2f(tex_coords, 1);
	glVertex3f(floor_radius, wall_height, floor_radius); // 1,1,1
	glTexCoord2f(0.0, 1);
	glVertex3f(floor_radius, wall_height, -floor_radius); //1,1,0
	glEnd();

	glNormal3f(1.0, 0.0, 0.0);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-floor_radius, -0.1, -floor_radius); // 0,0,0
	glTexCoord2f(tex_coords, 0.0);
	glVertex3f(-floor_radius, -0.1, floor_radius); // 0,0,1
	glTexCoord2f(tex_coords, 1);
	glVertex3f(-floor_radius, wall_height, floor_radius); // 0,1,1
	glTexCoord2f(0.0, 1);
	glVertex3f(-floor_radius, wall_height, -floor_radius); //0,1,0
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void drawEnvironment(GLuint* txId) {
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	floor(txId);
	walls(txId);
	//objects in scene - beds? bookshelf? wall
}