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

#include"Imagx.h"
using namespace std;

enum Status { START, GAME, DEAD, TIMEUP, END, MAIN_MENU, DEBUG };
typedef enum { ELDER_R = 7, ELDER_L = 8, CAR = 9, OTHER = 10 } TYPE;

class Player{
public:
	GLfloat pos[3];	//Position
	GLfloat LR_MOVE = 1; //玩家每次移動左右的距離
	GLfloat v = 0.01; //玩家移動速度
	GLfloat maxV = 1; //玩家移動速度
	GLfloat minV = 0.01; //玩家移動速度
	GLfloat shift = 0; //玩家與相機的位移
	Status status = MAIN_MENU; //遊戲目前的狀態
	TYPE event;
	bool cheat = false;
	bool bone = false;
	float minX, maxX;

	Player(float minX,float maxX){
		memset(this->pos, 0, 3);
		this->minX = minX-1.5;
		this->maxX = maxX+1.5;
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
			if (v + 0.002 < maxV) v += 0.002;
			else v = maxV;
			//相機位移
			if (this->shift + 0.3 < 3) this->shift += 0.3;
			else this->shift = 3;
		}
		else if (key == 's') {
			if (v - 0.07 > minV) v -= 0.07;
			else v = minV;

			if (v > 0) {
				if (this->shift - 0.1 > 0) this->shift -= 0.1;
				else this->shift = 0;
			}
			
		}
		else if (key == 'a') {
			if (this->pos[0] - LR_MOVE > this->minX)
				pos[0] -= LR_MOVE;
			else pos[0] = minX;
		}
		else if (key == 'd') {
			if (this->pos[0] + LR_MOVE < this->maxX)
				pos[0] += LR_MOVE;
			else pos[0] = maxX;
		}
	}

	//自動前進
	void Progress() {
		pos[2] -= v;
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
				char timer[50];
				int i = sprintf(timer, "TIME: %3d", this->nowTime);
				if (p->cheat) {
					sprintf(&timer[i], "\n\ncheat", NULL);
				}
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

	static void drawObstacle(GLfloat x,GLfloat y,GLfloat z, GLfloat r, Player *p, TYPE displayId){
		GLfloat mat_dif_yellow[4] = { 0.8,0.7,0,1 };
		GLfloat mat_dif_white[4] = { 1,1,1,1 };
		GLfloat mat_dif_red[4] = { 1,0,0,1 };
		
		CollisionBall clision(x, y, z);
		glPushMatrix();
		{
			if (p->bone) {
				glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_dif_yellow);
			}

			if (clision.isColision(r, p->pos)) {
				if(p->bone) glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_dif_red);
				if (!p->cheat && !p->bone) {
					p->event = displayId;
					p->status = DEAD;
					PlaySound(NULL, NULL, SND_ASYNC);
					PlaySound(TEXT("assets/music/湯姆貓慘叫聲.wav"), NULL, SND_ASYNC);
				}
				
			}
			glTranslatef(x, y, z);
			if (p->bone) {
				glutSolidCube(1);
			}
			else {
				glEnable(GL_TEXTURE_2D); glEnable(GL_BLEND);
				{
					glCallList((GLuint)displayId);
				}
				glDisable(GL_TEXTURE_2D); glDisable(GL_BLEND);
			}
			
			
			glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_dif_white);
		}
		glPopMatrix();
	}
};



struct Pos {
	float x, y, z;
	TYPE type;
	bool moveR = false;

	Pos(float a, float b, float c, TYPE type) {
		x = a, y = b, z = c;
		this->type = type;
	}
};

class RandomGenObStacles {
public:
	float minX, maxX;/* 指定X範圍 */
	int intialPosZ; //障礙物的初始z座標
	int nowZ; //目前的z座標
	int genNum = 4; //每次生成幾個障礙物
	int posZ_Shift = 10; //每次生成完後Z位移的範圍
	bool gen = true;
	int endIdx = 0; //畫到endIdx
	int roadNum = 3;
	float gap;
	vector<Pos> ObStaclesPos;

	RandomGenObStacles(float minX, float maxX,
				int genNum,
				int intialPosZ,
				int posZ_Shift)
	{
		float shift = 2.3;
		this->minX = minX - shift;
		this->maxX = maxX + shift;
		this->genNum = genNum;
		this->intialPosZ = intialPosZ;
		this->nowZ = intialPosZ;
		this->posZ_Shift = posZ_Shift;
		this->gap = (this->maxX - this->minX) / (this->roadNum * 2);
		printf("gap=%f\n", (this->maxX - this->minX) / (this->roadNum * 2));
		printf("minX=%f,maxX=%f\n", this->minX,this->maxX);
	}

	void init() {
		endIdx = 0;
		nowZ = intialPosZ;
		ObStaclesPos.clear();
		gen = true;
	}

	
	void genObstaclePos() {
		nowZ -= (rand() % posZ_Shift) + 6;

		set<int> X;
		int num = rand() % genNum;
		int older = rand() % 10; //控制老奶奶出現的機率
		int car = rand() % 10; //控制逆向車出現的機率

		if (num == 1 && older==0) { //老奶奶過馬路
			int R = rand() % 2;
			if (R) {
				Pos pos((float)maxX, 1, (float)nowZ, ELDER_R);
				pos.moveR = false;
				ObStaclesPos.push_back(pos);

			}
			else {
				Pos pos((float)minX, 1, (float)nowZ, ELDER_L);
				pos.moveR = true;
				ObStaclesPos.push_back(pos);

			}
			
			
		}
		else {
			for (int i = 0; i < num; ++i) {

				/* 產生亂數，隨機到某個道路*/
				int n = rand() % roadNum; 
				float x = minX + (2*n + 1) * gap; //gap=出現的間隔
				while (X.find(x) != X.end()) {
					n = rand() % roadNum;
					x = minX + (2 * n + 1) * gap;
				}
				//printf("x=%f\n", x);
				X.insert(x);

				if (i == 0 && car==0) { //逆向車
					Pos pos(x, 1, (float)nowZ, CAR);
					ObStaclesPos.push_back(pos);
				}
				else {
					Pos pos(x, 1, (float)nowZ, OTHER);
					ObStaclesPos.push_back(pos);
				}
				
			}
		}
		
	}

	void drawObstacle(Player *p, int pathLen) {
		if(gen) genObstaclePos();
		/*printf("size = %d\n", ObStaclesPos.size());*/
		//for (int i = ObStaclesPos.size()-1; i >= 0; --i) {
		for (int i = ObStaclesPos.size()-1; i >= endIdx; --i) {
			
			if ((ObStaclesPos[i].type == ELDER_R || ObStaclesPos[i].type == ELDER_L) && i-endIdx <= 30) { //老奶奶左右移動
				if (ObStaclesPos[i].x >= maxX) {
					ObStaclesPos[i].moveR = false;
					ObStaclesPos[i].type = ELDER_L;
				}
				if (ObStaclesPos[i].x <= minX) {
					ObStaclesPos[i].moveR = true;
					ObStaclesPos[i].type = ELDER_R;
				}

				if (ObStaclesPos[i].moveR) {
					ObStaclesPos[i].x += 0.01;
				}
				else {
					ObStaclesPos[i].x -= 0.01;
				}
			}

			if (ObStaclesPos[i].type == CAR && i - endIdx <= 30) { //逆向車
				ObStaclesPos[i].z += 0.05;
				if(ObStaclesPos[i].z - p->pos[2] > -40 && ObStaclesPos[i].z - p->pos[2] < -39)
					mciSendString(TEXT("play \"assets/music/逆向車.mp3\" "), NULL, 0, NULL);
			}
			else {
				//避免被逆向車的座標影響
				if (ObStaclesPos[i].z - p->pos[2] > 5) {
					endIdx = i;
					//printf("endIdx=%d\n", endIdx);
					break;
				}
			}
			Obstacles::drawObstacle(ObStaclesPos[i].x, 1, ObStaclesPos[i].z, 1.6, p, ObStaclesPos[i].type);

			if (ObStaclesPos[i].z < pathLen) gen = false;
		}
	}
};

class Ground {
public:
	float minX, maxX;
	int pathLen;
	Ground(float minX, float maxX, int pathLen) {
		this->minX = minX;
		this->maxX = maxX;
		this->pathLen = pathLen;
	}

	void draw(Imagx laneStripe) {
		int w = maxX - minX;
		int h;
		if (pathLen < 200) h = 200;
		else h = pathLen;

		glPushMatrix();
		{
			//glColor3ub(80, 127, 80);
			glColor3ub(90, 95, 90);
			glTranslatef(0, 0, -100);
			glRotatef(90, 1, 0, 0);
			glScalef(w, h, 0);

			glDisable(GL_LIGHTING);
			{
				glRectf(-1, 1, 1, -1);
			}
			glEnable(GL_LIGHTING);
		}
		glPopMatrix();

		//車道
		glPushMatrix();
		{
			glTranslatef(0, 0.001, -100);
			glRotatef(90, 1, 0, 0);
			glScalef(w, h, 0);

			glMatrixMode(GL_TEXTURE);
			glPushMatrix();
			{
				glLoadIdentity();
				glScalef(1, h / w, 0);
				laneStripe.drawImg();
			}
			glPopMatrix();
			glMatrixMode(GL_MODELVIEW);
		}
		glPopMatrix();
	}
};