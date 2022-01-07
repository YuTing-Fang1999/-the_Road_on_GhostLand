#pragma once
#include<Windows.h>
#include<GL/glew.h>
#include<GL/freeglut.h>
#include<stdio.h>
#include<string.h>
#define _USE_MATH_DEFINES
#include<math.h>

//glFunc
void init();
void idle();
void reshape(int w,int h);
void keyboardUp(unsigned char key, int x, int y);
void keyboard(unsigned char key,int x,int y);
void specialKb(int,int,int);

//draw
void drawGround(int w, int h);
void drawObstacles();
void drawBuildings();
void drawPlayer();
void drawProgressBar();
void drawTimer();
//void initGame();
void timer(int id);
void drawstr(GLfloat x, GLfloat y, char* format, ...);