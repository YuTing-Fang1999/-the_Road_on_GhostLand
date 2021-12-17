#include<Windows.h>
#include<GL/glew.h>
#include<GL/freeglut.h>
#include<stdio.h>
#include<string.h>
#include"classDef.h"

extern Player p1;
Building b1(5, 10, 5);
extern Timer myTimer;
extern ProgressBar myProgressBar;

void init(){
	glClearColor(0.5,0.5,0.5,1.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_RESCALE_NORMAL);
}

void idle(){
	//player 自動移動
	p1.Progress();
	printf("pos.z = %f\n",p1.pos[2]);

	//如果遊戲結束，停止移動
	if(p1.status == END){
		p1.playerStop(myProgressBar.pathLen);
		printf("stop\n");
	}
	glutPostRedisplay();
}

void restartGame() {
	myTimer.nowTime = myTimer.time;
	memset(p1.pos, 0, sizeof(p1.pos));
}

void keyboard(unsigned char key,int x,int y){
	if(key==27){
		exit(0);
	}
	if (key == 32 ) {//space
		if (p1.status == START) {
			//跳轉轉場畫面
			//可以放春融訂外賣的畫面
			//使用空白鍵可開始遊戲

			//用空白鍵開始遊戲
			p1.status = GAME;
		}
		else if (p1.status != GAME) {
			restartGame();
		}
		if (p1.status == TIMEUP) {
			//可以放春融吃不到外賣，氣噗噗的畫面
			
		}
	}
	if (p1.status == GAME) {
		p1.kb(key,x,y);
	}
	
	glutPostRedisplay();
}

void timer(int id) {
	if (p1.status == GAME) {
		--myTimer.nowTime;
		if (myTimer.nowTime <= 0) {
			p1.status = TIMEUP;
			restartGame();
		}
	}
	glutTimerFunc(1000, timer, 0);
}

void reshape(int w,int h){
	glViewport(0,0,w,h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60,(GLfloat)w/(GLfloat)h,1,50);
	glMatrixMode(GL_MODELVIEW);
}

//====================================draw====================================
void drawGround(int w, int h) {
	glPushMatrix();
	{
		glDisable(GL_LIGHTING);
		{
			glColor3ub(80, 127, 80);
			glTranslatef(0, 0, -100);
			glRotatef(90, 1, 0, 0);
			glScalef(w, h, 0);
			glRectf(-1, 1, 1, -1);
		}
		glEnable(GL_LIGHTING);
	}
	glPopMatrix();
}

void drawObstacles() {
	//static method ,don't need instance
	Obstacles::drawObstacle(-2, 1, -5, 1.4, &p1);
	Obstacles::drawObstacle(3, 1, -10, 1.4, &p1);
	//Obstacles::drawObstacle(4, 1, -20);
	//Obstacles::drawObstacle(-4, 1, -50);
	//Obstacles::drawObstacle(4, 1, -70);
	//Obstacles::drawObstacle(-2.5, 1, -90);
	//Obstacles::drawObstacle(4, 1, -100);
	//Obstacles::drawObstacle(4, 1, -130);
	//Obstacles::drawObstacle(4, 1, -200);
}

void drawBuildings() {
	b1.setPos(10, 1, -70);
	b1.drawBuilding();
}

void drawPlayer() {
	p1.drawPlayer();
}

void drawProgressBar() {
	myProgressBar.draw(&p1);
}

void drawTimer() {
	myTimer.drawTimer(&p1);
}