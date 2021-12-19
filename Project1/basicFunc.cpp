#include<Windows.h>
#include<GL/glew.h>
#include<GL/freeglut.h>
#include<stdio.h>
#include<string.h>
#include"classDef.h"
#include"Imagx.h"
//開啟下面這行可以關掉console
//#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )

extern Player p1;
Building b1(5, 10, 5);
extern Timer myTimer;
extern ProgressBar myProgressBar;

//status flag
GLboolean isFullScreen=GL_FALSE;


//圖片素材
//external variable需要初始化一個實體
extern Imagx helpMenu=Imagx();

void init(){
	glClearColor(0.5,0.5,0.5,1.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	//texture
	//若某模型不須貼圖或透明，要關掉功能
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_RESCALE_NORMAL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	helpMenu=Imagx("assets/img/board/help.png",4);
}

void idle(){
	//圖片素材
	helpMenu.progress();

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
			//START跳轉頁面function
			//使用空白鍵可開始遊戲
			
// 			if(頁面為最後一頁)
			//用空白鍵開始遊戲
			p1.status = GAME;
		}
		else if (p1.status != GAME) {
			restartGame();
		}
		if (p1.status == TIMEUP) {
			//TIMEUP跳轉頁面function
			
		}
	}
	if (p1.status == GAME) {
		p1.kb(key,x,y);
	}
	if(key=='h'){
		helpMenu.scaleBig();
		helpMenu.scaleSmall();
	}
	if(key=='f'){
		if(isFullScreen){
			glutPositionWindow(500,200);
			glutReshapeWindow(700, 700);
		}
		else{
			glutFullScreen();
		}
		isFullScreen = ~isFullScreen;
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
