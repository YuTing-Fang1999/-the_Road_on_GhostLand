#pragma once
#include<Windows.h>
#include<GL/glew.h>
#include<GL/freeglut.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#define _USE_MATH_DEFINES

#define PLAYER_RATE 1 //玩家每次的移動距離


enum Status { START, GAME, DEAD, TIMEUP, END };

class Player{
public:
	GLfloat pos[3];	//Position
	Status status = START; //遊戲目前的狀態

	Player(){
		memset(this->pos, 0, 3);
	}
	~Player(){}

	//鏡頭
	void lookAt() {
		gluLookAt(
			pos[0], pos[1] + 5, pos[2] + 10,
			pos[0], pos[1], pos[2] - 17,
			0, 1, 0);
	}

	//畫出player
	void drawPlayer(){
		glPushMatrix();
		{
			glTranslatef(pos[0], pos[1], pos[2]);
			glScalef(1,2,1);
			glutSolidCube(1);
		}
		glPopMatrix();
	}

	//player鍵盤功能
	void kb(unsigned char key, int x, int y) {
		if (key == 'w') {
			pos[2] -= PLAYER_RATE;
		}
		else if (key == 's') {
			pos[2] += PLAYER_RATE;
		}
		else if (key == 'a') {
			pos[0] -= PLAYER_RATE;
		}
		else if (key == 'd') {
			pos[0] += PLAYER_RATE;
		}
	}

	//自動前進
	void Progress() {
		//pos[2] -= 0.1;
	}

	//停止
	void playerStop(int finalPos) {
		this->pos[2] = finalPos;
	}
};

class ProgressBar {
public:
	int pathLen; //終點位置

	ProgressBar(int pathLen) {
		this->pathLen = -pathLen;
	}
	void draw(Player *p) {
		float x = 0, y = 0; //進度條起點
		float width = 8, height = 0.2;
		
		//如果到達終點
		if (p->pos[2] <= pathLen) p->status = END;
		
		glDisable(GL_LIGHTING);
		{
			glPushMatrix();
			{
				//藍條
				glTranslatef(p->pos[0]-4, p->pos[1]+7.8, p->pos[2]);
				glColor3f(0, 1, 1);
				glBegin(GL_QUADS);
				glVertex2f(x, y);
				glVertex2f(x + (p->pos[2])/ pathLen * width, y);
				glVertex2f(x + (p->pos[2]) / pathLen * width, y + height);
				glVertex2f(x, y + height);
				glEnd();

				//白條
				glColor3f(1, 1, 1);
				glBegin(GL_QUADS);
				glVertex2f(x, y);
				glVertex2f(x + width, y);
				glVertex2f(x + width, y + height);
				glVertex2f(x, y + height);
				glEnd();
			}
			glPopMatrix();
			
		}
		glEnable(GL_LIGHTING);

		
	}
};


class Timer {
public:
	GLvoid* font_style = GLUT_BITMAP_HELVETICA_18;
	int nowTime;
	int time;
	Timer(int t) {
		this->time = t;
		this->nowTime = t;
	}

	void drawstr(GLfloat x, GLfloat y, char* format, ...)
	{
		va_list args;
		char buffer[255], * s;

		va_start(args, format);
		vsprintf(buffer, format, args);
		va_end(args);

		glRasterPos2f(x, y);
		for (s = buffer; *s; s++)
			glutBitmapCharacter(font_style, *s);
	}

	void drawTimer(Player *p) {
		glDisable(GL_LIGHTING);
		{
			glPushMatrix();
			{
				glTranslatef(p->pos[0], p->pos[1], p->pos[2]);
				glColor3f(1, 1, 1);
				char timer[15];
				sprintf(timer, "TIME: %3d", this->nowTime);
				drawstr(-4.5, 7, timer);
			}
			glPopMatrix();
		}
		glEnable(GL_LIGHTING);
	}
};
class Building{
public:
	GLfloat scl[3];
	GLfloat pos[3]={0,0,0};
	GLfloat mat_amb[4]={0.2,0.2,0.2,1};
	GLfloat mat_dif[4]={0.8,0.7,0.7,1};
	GLfloat mat_dif_w[4]={1,1,1,1};
	GLfloat mat_nul[4]={0,0,0,0};

	Building(GLfloat x,GLfloat y,GLfloat z){
		scl[0]=x;scl[1]=y;scl[2]=z;
	}
	~Building(){}

	void setPos(GLfloat x,GLfloat y,GLfloat z){
		pos[0]=x;pos[1]=y;pos[2]=z;
	}

	void drawBuilding(){
		glPushMatrix();
		{
			//glTranslatef(0,0,-70);
			glTranslatef(pos[0],pos[1],pos[2]);
			glScalef(scl[0],scl[1],scl[2]);
			//glMaterialfv(GL_FRONT,GL_AMBIENT,mat_amb);
			
			glutSolidCube(1);
			//glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_dif_w);
		}
		glPopMatrix();
	}
};

class CollisionBall {
public:
	GLfloat myPos[3];

	CollisionBall(GLfloat x, GLfloat y, GLfloat z) {
		myPos[0] = x;
		myPos[1] = y;
		myPos[2] = z;
	}
	~CollisionBall() {}

	GLfloat dist(GLfloat pos[3]) {
		return sqrt(
			pow(pos[0] - myPos[0], 2) +
			pow(pos[1] - myPos[1], 2) +
			pow(pos[2] - myPos[2], 2)
		);
	}
	bool isColision(GLfloat r, GLfloat pos[3]) {
		return dist(pos) < r;
	}
};
class Obstacles{
public:
	//draw a test object in scene
	Obstacles(){}
	~Obstacles(){}

	static void drawObstacle(GLfloat x,GLfloat y,GLfloat z, GLfloat r, Player *p/*, int displayId*/){
		GLfloat mat_dif_yellow[4] = { 0.8,0.7,0,1 };
		GLfloat mat_dif_white[4] = { 1,1,1,1 };
		GLfloat mat_dif_red[4] = { 1,0,0,1 };
		
		CollisionBall clision(x, y, z);
		glPushMatrix();
		{
			glMaterialfv(GL_FRONT,GL_DIFFUSE, mat_dif_yellow);
			if (clision.isColision(r, p->pos)) {
				glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_dif_red);
			}
			glTranslatef(x, y, z);
			glutSolidCube(1);
			//glCallList(displayID);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_dif_white);
		}
		glPopMatrix();
	}
};