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

#include "glm.h"
#include "Imagx.h"
#include "ObjectLoader.h"

//遊戲時長(秒)
extern Timer myTimer(30); 
//地板(minX, maxX, 路徑長度)
extern Ground myGround(-2, 2, 600); 
//進度條
extern ProgressBar myProgressBar(myGround.pathLen);
//隨機產生障礙物(minX, maxX, genNum, intialPosZ, , posZ_Shift)
extern RandomGenObStacles myRandGenObstacles(myGround.minX, myGround.maxX, 2, -10, 3);
//玩家
extern Player p1=Player(myGround.minX, myGround.maxX);

//flag
extern GLboolean listExchange;
extern GLboolean hasShownArchiv;
Imagx* archiv_tmp;

//圖片素材
extern Imagx coverRGL;//local z = -1
extern Imagx helpMenu;//local z = 0
extern Imagx mainMenu;//local z = -0.1
extern Imagx exitMenu;//local z = 0.2
extern Imagx aboutMenu;//local z = 0.1
extern Imagx restartMenu;
extern ImagxList imlist;
//人物
extern Imagx callingMan;
extern Imagx callingWoman;
extern Imagx callingOldMan;
extern Imagx callingOldWoman;
extern Imagx text_succ;
extern Imagx text_dead;
extern Imagx text_begin;
extern Imagx text_timeup;
extern Imagx text_succ_teacher;
extern Imagx text_dead_teacher;
extern Imagx text_begin_teacher;
extern Imagx text_timeup_teacher;

//道路
extern Imagx zebraStripe;
extern Imagx laneStripe;
extern Imagx traffic_light;
//道路障礙
extern Imagx event_fire;
extern Imagx event_hole;
extern Imagx event_xross_to_R;
extern Imagx event_xross_to_L;

//背景
extern Imagx bg_color;
extern Imagx bg_building;

//成就
extern Imagx archiv_ad_board;
extern Imagx archiv_fire;
extern Imagx archiv_intersection_car;
extern Imagx archiv_xross_road;
extern Imagx archiv_reverse_car;
extern Imagx archiv_road_hole;

//3D素材
extern ObjectLoader stev;
extern ObjectLoader building_test;
extern ObjectLoader board_small_cup;
extern ObjectLoader board_pawnshop;
extern ObjectLoader playerObj;
extern ObjectLoader car;
extern ObjList objlist;

extern Building b1;

//遊戲除錯空間
void drawDebugView(){
	//stev測試用OBJ
	/*glPushMatrix();
	{
		glTranslatef(0,1,-5);
		glRotatef(45,1,0,0);
		glScalef(2,2,2);
		stev.drawObj(stev.getDpIndex());
		glTranslatef(-3,-3,0);
		building_test.drawObj(building_test.getDpIndex());	
	}
	glPopMatrix();*/

	//預覽
	glPushMatrix();
	{
		glTranslatef(0,3,0);
		
		if(listExchange){
			glScalef(3,3,3);
			imlist.draw();
		}
		else{
			glRotatef(60,0,1,0);
			objlist.draw();
		}
	}
	glPopMatrix();
}

//遊戲選單畫面
void drawMainMenu(){
	//helpMenu
	glPushMatrix();
	{
		glTranslatef(0,4,2);
		glRotatef(-10,1,0,0);

		//cover
		glPushMatrix();
		{
			glTranslatef(0,0,-1);
			coverRGL.drawImg();
		}
		glPopMatrix();

		//main
		glPushMatrix();
		{
			glTranslatef(0,-3.5,-0.1);
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

void drawDialog(Status s, GLfloat pos[],int id){
	Imagx* im=NULL;
	Imagx* dialog=NULL;
	if(id==0)		im=&callingMan;
	else if(id==1)	im=&callingWoman;
	else if(id==2)	im=&callingOldMan;
	else if(id==3)	im=&callingOldWoman;

	switch(s){
		case START:
		{
			if(id==4)	dialog = &text_begin_teacher;
			else		dialog = &text_begin;
		}	
		break;

		case END:
		{
			if(id==4)	dialog = &text_succ_teacher;
			else		dialog = &text_succ;
		}
		break;

		case DEAD:
		{
			if(id==4)	dialog = &text_dead_teacher;
			else		dialog = &text_dead;
		}
		break;

		case TIMEUP:
		{
			if(id==4)	dialog = &text_timeup_teacher;
			else		dialog = &text_timeup;
		}
		break;

		default:
		break;
	}

	if(dialog!=NULL){
		glPushAttrib(GL_LIGHTING_BIT);
		glPushMatrix();
		{
			glTranslatef(pos[0],pos[1],pos[2]);
			glTranslatef(0,2,2);
			glRotatef(-10,1,0,0);

			glDisable(GL_DEPTH_TEST);
			if(im!=NULL){
				//draw Text
				dialog->drawImg();

				//draw calling person
				glTranslatef(-2.4,-2,0.1);
				glScalef(0.7,0.7,0.7);
				im->drawImg();
			}
			else{
				dialog->drawImg();
			}
			glEnable(GL_DEPTH_TEST);
		}
		glPopAttrib();
		glPopMatrix();
	}
}


//遊戲開始畫面
void drawStart() {
	//可以放春融訂外賣的畫面，使用空白鍵可開始遊戲
	//這裡放故事情節，讓玩家選擇是否跳過(SKIP)
	drawPlayer();
	glPushMatrix();
	{
		glPushAttrib(GL_LIGHTING_BIT);
		glTranslatef(0,-5,0);
		building_test.drawObj(building_test.getDpIndex());//為了亮度
		glPopAttrib();
	}
	glPopMatrix();
}

//遊戲運行畫面
void drawGame() {

	//倒數計時器
	drawTimer();
	
	//地板(寬,高)
	drawGround(6, -myProgressBar.pathLen);

	//建築
	drawBuildings();

	//player
	drawPlayer();

	//障礙物
	drawObstacles();

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
}

void drawExitMenu(GLfloat pos[]){
	glPushMatrix();
	{
		glDisable(GL_DEPTH_TEST);
		glTranslatef(pos[0],pos[1],pos[2]);
		glTranslatef(0,4,2);
		glRotatef(-10,1,0,0);
		exitMenu.drawImg();
		glEnable(GL_DEPTH_TEST);
	}
	glPopMatrix();
}

void drawRestartMenu(Status s,GLfloat pos[]){
	switch (p1.status)
	{
		case END:
		case DEAD:
		case TIMEUP:
		{
			glPushMatrix();
			{
				glDisable(GL_DEPTH_TEST);
				glTranslatef(pos[0],pos[1],pos[2]);
				glTranslatef(10,-2.8,0);
				glRotatef(-10,1,0,0);
				restartMenu.drawImg();
				glEnable(GL_DEPTH_TEST);
			}
			glPopMatrix();
			
		}
		break;
		default:
			break;
	}
}

void drawEvent(Player* p) {
	switch(p->status)
	{
		case END:
		case DEAD:
		case TIMEUP:
		{
			glPushMatrix();
			{
				glTranslated(p->pos[0],p->pos[1],p->pos[2]);
				glTranslatef(0,3.3,0);
				glRotatef(-10,1,0,0);
				glDisable(GL_DEPTH_TEST);
				if(hasShownArchiv){
					//has shown
					if(archiv_tmp!=NULL)	archiv_tmp->drawImg();
				}
				else{
					//does not show
					hasShownArchiv = GL_TRUE;
					switch (p->event){
						case CAR:
							archiv_tmp=&archiv_reverse_car;
							break;
						case FIRE:
							archiv_tmp=&archiv_fire;
							break;
						case HOLE:
							archiv_tmp=&archiv_road_hole;
							break;
						case ELDER_L:
						case ELDER_R:
							archiv_tmp=&archiv_xross_road;
							break;
						default:
							printf("event:%d\n",p->event);
							break;
					}

					if(archiv_tmp!=NULL)	archiv_tmp->popUpAnim(180);
				}
				glEnable(GL_DEPTH_TEST);
			}
			glPopMatrix();
		}
		break;
		default:
			break;
	}
}

void drawBackground(GLfloat pos[]){
	glPushAttrib(GL_LIGHTING_BIT);
	glPushMatrix();
	{
		glTranslatef(pos[0],pos[1],pos[2]);
		glTranslatef(0,-5,-80);
		glRotatef(-10.5,1,0,0);
		static float ss=105;

		glPushMatrix();
		{
			glScalef(ss,ss,1);
			bg_color.drawImg();
		}
		glPopMatrix();

		glPushMatrix();
		{
			glTranslatef(0,10,0.001);
			glScalef(ss-10,ss-10,1);
			bg_building.drawImg();
		}
		glPopMatrix();

	}
	glPopMatrix();
	glPopAttrib();
}

void display(){
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	p1.lookAt();
	
	//====================================
	drawBackground(p1.pos);

	switch (p1.status)
	{
		case START:
			drawStart();
			break;

		case GAME:
			drawGame();
			break;

		case DEAD:
			drawGame();
			//drawEvent(p1.status,&p1);
			break;

		case END:
			drawGame();
			//drawEnd();
			break;

		case TIMEUP:
			drawGame();
			//drawTimeUp();
			break;

		case MAIN_MENU:
			drawMainMenu();
			break;

		case DEBUG:
			drawDebugView();
			break;
		default:
			break;
	}

	drawDialog(p1.status,p1.pos,p1.character);
	drawRestartMenu(p1.status,p1.pos);
	drawEvent(&p1);
	drawExitMenu(p1.pos);
	//====================================

	glutSwapBuffers();
}



int main(int argc,char **argv){
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH |GLUT_MULTISAMPLE);
	glutInitWindowPosition(0,0);
	glutInitWindowSize(700,700);
	glutCreateWindow("TITLE");
	GLenum err = glewInit();
	if( err != GLEW_OK){
		printf("GLEW_ERROR\n");
	}

	init();

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyboardUp);
	glutSpecialFunc(specialKb);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);
	glutTimerFunc(1000,timer,0);
	glutTimerFunc(20, timer, 1);
	glutTimerFunc(5, timer, 2);
	glutMainLoop();
	return 0;
}
