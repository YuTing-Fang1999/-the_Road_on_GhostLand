#pragma once
#include<Windows.h>
#include<GL/glew.h>
#include<GL/freeglut.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define _USE_MATH_DEFINES
#include<math.h>
#include"basicFunc.h"
#include <vector>
#include <set>
using namespace std;

enum Status { START, GAME, DEAD, TIMEUP, END, MAIN_MENU, DEBUG };

class Player{
public:
	GLfloat pos[3];	//Position
	GLfloat LR_MOVE = 1; //玩家每次移動左右的距離
	GLfloat v = 0.01; //玩家移動速度
	GLfloat maxV = 0.1; //玩家移動速度
	GLfloat minV = 0.01; //玩家移動速度
	GLfloat shift = 0; //玩家與相機的位移
	Status status = MAIN_MENU; //遊戲目前的狀態

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
			glTranslatef(pos[0], pos[1], pos[2] - this->shift);
			glScalef(1,2,1);
			glutSolidCube(1);
		}
		glPopMatrix();
	}

	//player鍵盤功能
	void kb(unsigned char key, int x, int y) {
		if (key == 'w') {
			//速度
			if (v + 0.005 < maxV) v += 0.005;
			else v = maxV;
			//相機位移
			if (this->shift + 0.3 < 3) this->shift += 0.3;
			else this->shift = 3;
		}
		else if (key == 's') {
			if (v - 0.005 > minV) v -= 0.005;
			else v = minV;

			if (v > 0) {
				if (this->shift - 0.1 > 0) this->shift -= 0.1;
				else this->shift = 0;
			}
			
		}
		else if (key == 'a') {
			pos[0] -= LR_MOVE;
		}
		else if (key == 'd') {
			pos[0] += LR_MOVE;
		}
	}

	//自動前進
	void Progress() {
		pos[2] -= v;

		if (this->shift - 0.03 > 0) this->shift -= 0.03;
		else this->shift = 0;
		
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
	int nowTime;
	int time;
	Timer(int t) {
		this->time = t;
		this->nowTime = t;
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
				drawstr(-4, 7.5, timer);
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

	static void drawObstacle(GLfloat x,GLfloat y,GLfloat z, GLfloat r, Player *p, GLuint displayId){
		GLfloat mat_dif_yellow[4] = { 0.8,0.7,0,1 };
		GLfloat mat_dif_white[4] = { 1,1,1,1 };
		GLfloat mat_dif_red[4] = { 1,0,0,1 };
		
		CollisionBall clision(x, y, z);
		glPushMatrix();
		{
			//glMaterialfv(GL_FRONT,GL_DIFFUSE, mat_dif_yellow);
			if (clision.isColision(r, p->pos)) {
				glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_dif_red);
			}
			glTranslatef(x, y, z);
			glEnable(GL_TEXTURE_2D); glEnable(GL_BLEND);
			{
				glCallList(displayId);
			}
			glDisable(GL_TEXTURE_2D); glDisable(GL_BLEND);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_dif_white);
		}
		glPopMatrix();
	}
};

typedef enum { ELDER, OTHER } TYPE;

struct Pos {
	float x, y, z;
	TYPE type;
	Pos(float a, float b, float c, TYPE type) {
		x = a, y = b, z = c;
		this->type = type;
	}
};

class RandomGenObStacles {
public:
	int minX = -6, maxX = 6;/* 指定X範圍 */
	int intialPosZ = -15; //障礙物的初始z座標
	int nowZ = -15;
	int genNum = 4; //每次生成幾個障礙物
	int posZ_Shift = 10; //每次生成完後Z位移的範圍
	int headIdx = 0; //從一個障礙物開始畫
	bool gen = true;
	vector<Pos> ObStaclesPos;

	RandomGenObStacles(int minX, int maxX,
				int genNum,
				int intialPosZ,
				int posZ_Shift)
	{
		this->minX = minX;
		this->maxX = maxX;
		this->genNum = genNum;
		this->intialPosZ = intialPosZ;
		this->nowZ = intialPosZ;
		this->posZ_Shift = posZ_Shift;
	}

	void init() {
		headIdx = 0;
		nowZ = intialPosZ;
		ObStaclesPos.clear();
		gen = true;
	}
	void genObstaclePos() {
		nowZ -= (rand() % 10);

		set<int> X;
		int num = rand() % genNum;
		if (num == 1) {
			int x = rand() % (maxX - minX + 1) + minX;
			Pos pos((float)x, 1, (float)nowZ,ELDER);
			ObStaclesPos.push_back(pos);
		}
		else {
			for (int i = 0; i < num; ++i) {
				/* 產生亂數 */
				int x = rand() % (maxX - minX + 1) + minX;
				while (X.find(x) != X.end()) {
					x = rand() % (maxX - minX + 1) + minX;
				}
				//printf("%d\n", x);
				X.insert(x);
				Pos pos((float)x, 1, (float)nowZ, OTHER);
				ObStaclesPos.push_back(pos);
			}
		}
		
	}

	void drawObstacle(Player *p, int pathLen) {
		if(gen) genObstaclePos();
		/*printf("size = %d\n", ObStaclesPos.size());*/
		//for (int i = headIdx; i < ObStaclesPos.size(); ++i) {
		for (int i = ObStaclesPos.size()-1; i >= headIdx; --i) {
			Obstacles::drawObstacle(ObStaclesPos[i].x, 1, ObStaclesPos[i].z, 1.4, p,10);
			if (ObStaclesPos[i].z - p->pos[2] > 5) headIdx = i;
			if (ObStaclesPos[i].z < pathLen) gen = false;
		}
	}
};