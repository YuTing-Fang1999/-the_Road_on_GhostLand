#pragma once
#include<Windows.h>
#include<GL/glew.h>
#include<GL/freeglut.h>
#include<stdio.h>
#include<string.h>
#define _USE_MATH_DEFINES
#include<math.h>

void init();
void idle();
void reshape(int w,int h);
void keyboard(unsigned char key,int x,int y);

//draw
void drawGround(int w, int h);
void drawObstacles();
void drawBuildings();
void drawPlayer();
void drawProgressBar();
void drawTimer();
//void restartGame();
void timer(int id);