//  ========================================================================
//  COSC422: Advanced Computer Graphics;  University of Canterbury (2023)
//	Max Bastida
//
//  FILE NAME: Draw Mesh.h
//  ========================================================================
#include <iostream>
#include <fstream>
#include <GL/freeglut.h>
using namespace std;

#include <assimp/cimport.h>
#include <assimp/types.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

void draw_mesh(const aiNode* node, int subject, bool shadow);