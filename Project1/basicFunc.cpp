#include<Windows.h>
#include<GL/glew.h>
#include<GL/freeglut.h>
#include<stdio.h>
#include<string.h>
#include"classDef.h"
#include"Imagx.h"

#include "FreeImage.h"
#include "glm.h"
#include "Imagx.h"
#include "ObjectLoader.h"

extern Player p1;
Building b1(5, 10, 5);
extern Timer myTimer;
extern ProgressBar myProgressBar;

//status flag
GLboolean isFullScreen=GL_FALSE;


//圖片素材
//external variable需要初始化一個實體
extern Imagx helpMenu=Imagx();
extern Imagx mainMenu=Imagx();
extern Imagx aboutMenu=Imagx();

//3D素材
extern ObjectLoader stev=ObjectLoader();

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

	//載入圖片素材
	helpMenu=Imagx("assets/img/board/menu_help.png",0,GL_TRUE);helpMenu.setMaxScale(4);
	aboutMenu=Imagx("assets/img/board/menu_about.png",0,GL_TRUE);aboutMenu.setMaxScale(4);
	mainMenu=Imagx("assets/img/board/menu_main.png",2.5,GL_TRUE);

	//載入3D素材
	stev=ObjectLoader(	"assets/img/stev/stev.obj",
						"assets/img/stev/stev.jpg",
						GL_FALSE,
						GLM_SMOOTH|GLM_MATERIAL|GLM_TEXTURE);
}

void idle(){
	//圖片素材
	helpMenu.progress();
	aboutMenu.progress();

	//player 自動移動
	p1.Progress();
	printf("\rpos.z = %f ,STATUS:%d \t",p1.pos[2],p1.status);

	//如果遊戲結束，停止移動
	if(p1.status == END){
		p1.playerStop(myProgressBar.pathLen);
		printf("\r[p1]stop ");
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
			//skip跳過故事
			p1.status = GAME;
		}
		else if (p1.status != GAME) {
			restartGame();
		}
		if (p1.status == TIMEUP ||
			p1.status == DEAD   ||
			p1.status == END) {
			//TIMEUP跳轉頁面function
			p1.status = GAME;
		}
		if (p1.status == MAIN_MENU){
			p1.status = START;
		}
	}
	if (p1.status == GAME) {
		p1.kb(key,x,y);
	}
	if (key=='r'){
		if (p1.status == TIMEUP ||
			p1.status == DEAD   ||
			p1.status == END    ){
			//回到主選單
			restartGame();
			p1.status = MAIN_MENU;
		}
	}
	if(key=='h'){
		//幫助/遊戲說明
		if(p1.status==MAIN_MENU){
			//動畫播放完之後，才能進行下一個動畫
			if(helpMenu.getScale() == 0)helpMenu.scaleBig();
			else if(helpMenu.getScale() == helpMenu.getMaxScale())helpMenu.scaleSmall();
		}
	}
	if(key=='v'){
		//關於遊戲資訊
		if(p1.status==MAIN_MENU){
			//動畫播放完之後，才能進行下一個動畫
			if(aboutMenu.getScale() == 0)aboutMenu.scaleBig();
			else if(aboutMenu.getScale() == aboutMenu.getMaxScale())aboutMenu.scaleSmall();
		}
	}
	if(key=='f'){
		//全螢幕
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
