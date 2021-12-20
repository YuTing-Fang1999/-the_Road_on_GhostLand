#include<Windows.h>
#include<GL/glew.h>
#include<GL/freeglut.h>
#include<stdio.h>
#include<string.h>
#include<math.h>
#define _USE_MATH_DEFINES
#include"basicFunc.h"
#include"classDef.h"
//開啟下面這行可以關掉console
//#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )

#include "FreeImage.h"
#include "glm.h"
#include "Imagx.h"
#include "ObjectLoader.h"

extern Player p1=Player();
extern Timer myTimer(6); //遊戲時長(秒)
extern ProgressBar myProgressBar(100); //路徑長度

//圖片素材
extern Imagx helpMenu;
extern Imagx mainMenu;
extern Imagx aboutMenu;
extern ObjectLoader stev;

//遊戲選單畫面
void drawMainMenu(){
	//stev測試用OBJ
	glPushMatrix();
	{
		glTranslatef(0,3,-5);
		glRotatef(45,1,0,0);
		glScalef(2,2,2);
		stev.drawObj(stev.getDpIndex());
	}
	glPopMatrix();

	//helpMenu
	glPushMatrix();
	{
		glTranslatef(0,4,2);
		glRotatef(-10,1,0,0);

		//main
		glPushMatrix();
		{
			glTranslatef(0,-3.5,-0.2);
			mainMenu.drawImg();
		}
		glPopMatrix();
		
		//help
		helpMenu.drawImg();

		//about
		glPushMatrix();
		{
			glTranslatef(0,0,0.1);
			aboutMenu.drawImg();
		}
		glPopMatrix();
	}
	glPopMatrix();
}

//遊戲開始畫面
void drawStart() {
	//可以放春融訂外賣的畫面，使用空白鍵可開始遊戲
	//這裡放故事情節，讓玩家選擇是否跳過(SKIP)
	drawPlayer();
}

//遊戲運行畫面
void drawGame() {
	
	//地板(寬,高)
	drawGround(6, 600);

	//建築
	drawBuildings();

	//障礙物
	drawObstacles();

	//player
	drawPlayer();

	//上方的進度條
	drawProgressBar();

	//glTranslated(3, 2, 0);
	//glutSolidSphere(1, 100, 100);
}

//遊戲結束畫面
void drawEnd() {
	//可以放春融收到外賣的畫面
	drawProgressBar();
}

//時間到了
void drawTimeUp() {
	//可以放春融吃不到外賣，氣噗噗的畫面
	drawObstacles();
}

void display(){
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	p1.lookAt();
	//====================================

	switch (p1.status)
	{
		case START:
			drawStart();
			break;

		case GAME:
			drawGame();
			break;

		case END:
			drawEnd();
			break;

		case TIMEUP:
			drawTimeUp();
			break;
		case MAIN_MENU:
			drawMainMenu();
			break;
		default:
			break;
	}
	drawTimer();
	//====================================

	glutSwapBuffers();
}



int main(int argc,char **argv){
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH |GLUT_MULTISAMPLE);
	glutInitWindowPosition(500,200);
	glutInitWindowSize(700,700);
	glutCreateWindow("TITLE");

	init();

	GLenum err = glewInit();
	if( err != GLEW_OK){
		printf("GLEW_ERROR\n");
	}

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);
	glutTimerFunc(1000,timer,0);
	glutMainLoop();
	return 0;
}
