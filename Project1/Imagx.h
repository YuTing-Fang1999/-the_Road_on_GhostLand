#pragma once
#include<Windows.h>
#include<GL/glew.h>
#include<GL/freeglut.h>
#include<stdio.h>
#define _USE_MATH_DEFINES
#include<math.h>
#include<vector>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC
#include"stbi_image.h"
#include"basicFunc.h"

class Imagx {
public:
	Imagx(){}
	/*
	imgfile is the name of the image.
	s is the maxScale of the image.
	x-------------------------------------
	isNeedDpIndex
	GL_TRUE -> make a image board
	GL_FALSE-> make only image texture
	*/
	Imagx(char* imgfile, GLfloat s,GLboolean isNeedDpIndex) {
		genIndex(isNeedDpIndex);
		max_scl = s;
		scl = max_scl;

		stbi_set_flip_vertically_on_load(true);//翻轉圖片
		unsigned char* pixels=stbi_load(imgfile,&width,&height,&nCh,0);
		if(pixels){
			printf("load %s\nw:%d h:%d ch:%d\n",imgfile,width,height,nCh);

			//set unitilize
			v[0] = (float)width / (float)width;
			v[1] = (float)height / (float)width;

			if(texIndex){
				glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
				glBindTexture(GL_TEXTURE_2D, texIndex);
				glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, texIndex);
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 1, GL_RGBA, width, height, GL_TRUE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

				if(nCh==3)		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height,0, GL_RGB, GL_UNSIGNED_BYTE,pixels);
				else if(nCh==4)	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height,0, GL_RGBA, GL_UNSIGNED_BYTE,pixels);
				glGenerateMipmap(GL_TEXTURE_2D);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

				if(isNeedDpIndex){
					setMaterial();
					compileDisplayList();
				}
				stbi_image_free(pixels);
			}
			else{
				printf("does not generate texture list\n");
			}
		}
		else{
			printf("does not load Img\n");
		}
		printf("\n");
	}
	~Imagx() {}

	/*.
	display the final image object
	*/
	void drawImg() {
		if(scl>0){
			glEnable(GL_TEXTURE_2D);glEnable(GL_BLEND);
			glPushMatrix();
			{
				//sin(x),x is rad , tick_ang is deg
				//isCircleRotateAnim doing here
				if (isCirleRotateAnim) {
					glRotatef(15 * sin(M_PI / 180 * tick_ang[1]), 0, 1, 0);
					glRotatef(-2.5 + 5 * sin(M_PI / 180 * (tick_ang[1] + 135)), 1, 0, 0);
				}

				glScalef(scl, scl, scl);
				if(dpIndex!=0 && texIndex!=0) glCallList(dpIndex);
				else{
					//CCW
					glBegin(GL_QUADS);
					{
						glNormal3f(0, 0, 1);
						glTexCoord2f(0, 0); glVertex3f(-v[0], -v[1], 0);
						glTexCoord2f(0, 1); glVertex3f(-v[0], v[1], 0);
						glTexCoord2f(1, 1); glVertex3f(v[0], v[1], 0);
						glTexCoord2f(1, 0); glVertex3f(v[0], -v[1], 0);
					}
					glEnd();
				}
			}
			glPopMatrix();
			glDisable(GL_TEXTURE_2D);glDisable(GL_BLEND);
		}
	}

	// animation settings
	void setScale(){
		scl = max_scl;
	}

	/*
	設定圖片最大縮放值
	*/
	void setMaxScale(float s) {
		max_scl = s;
	}
	/*
	設定圖片縮放過場動畫的播放時間
	播放時間以一個frame為單位
	*/
	void setEndTick(GLuint t) {
		end_tick = t;
	}
	/*
	讓圖片圓周晃動，看起來更活潑
	可以開啟(GL_TRUE)、關閉(GL_FALSE)
	*/
	void cirleRotateAnim(GLboolean b) {
		isCirleRotateAnim = b;
		if (isCirleRotateAnim) {
			//initailize the relative variable
			tick_ang[1] == 0;
		}
	}

	//image basic animation
	/*
	播放變大過場動畫
	播放期間為互斥狀態，只能有一種縮放動畫
	*/
	void scaleBig() {
		if (isScaling == 0 && scl == 0) {
			isScaling = 1;
		}
	}
	/*
	播放變小過場動畫
	播放期間為互斥狀態，只能有一種縮放動畫
	*/
	void scaleSmall() {
		if (isScaling == 0 && scl > 0) {
			isScaling = -1;
		}
	}
	/*
		彈出效果動畫
		duration要大於等於0
	*/
	void popUpAnim(GLfloat duration){
		if(duration>=0 && isPopupAnim==GL_FALSE){
			popUpDuration = duration;
			isPopupAnim = GL_TRUE;
		}
	}
	/*
	取得目前縮放值
	*/
	GLfloat getScale(){
		return scl;
	}
	/*
	取得最大縮放值
	*/
	GLfloat getMaxScale(){
		return max_scl;
	}
	/*
	得到Display index
	*/
	GLuint getDpIndex(){
		return dpIndex;
	}
	/*
	得到texture index
	*/
	GLuint getTexIndex(){
		return texIndex;
	}

	//place in idleFunc
	/*
	更新每個frame的狀態
	讓呼叫drawImg()有動畫效果
	*/
	void progress() {
		if (scl != 0 && isCirleRotateAnim) {
			tick_ang[1] += 1;
			if (tick_ang[1] >= 360) tick_ang[1] -= 360;
		}

		if (isScaling) {
			//!=0 doing scaling
			if (isScaling == 1) {
				//tanh(x);
				scl = max_scl * tanh((float)tick / 15);
			}
			else if (isScaling == -1) {
				//tanh(-x);
				scl = max_scl * tanh(-((float)tick - (float)end_tick) / 15);
			}

			tick++;
			if (tick == end_tick) {
				if (isScaling == 1) {
					scl = max_scl;
				}
				else if (isScaling == -1) {
					scl = 0;
				}
				tick = 0;
				isScaling = 0;
			}
		}

		if (isPopupAnim){
			if(scl==0)scaleBig();
			if(scl==max_scl){
				if(tick!=popUpDuration){
					tick++;
				}
				else{
					tick=0;
					isPopupAnim = GL_FALSE;
					scaleSmall();
				}
			}
		}

	}

private:
	//image info
	GLfloat v[2];
	GLint nCh;
	GLint height;
	GLint width;
	unsigned char* pixels;

	//display index
	GLuint texIndex;
	GLuint dpIndex;

	//anim parameters
	GLuint popUpDuration=0;
	GLuint tick = 0;
	GLuint end_tick = 60;
	GLfloat max_scl = 1;
	GLfloat scl = max_scl;
	GLfloat tick_ang[3] = { 0,0,0 };

	//anim flags
	int isScaling = 0;
	GLboolean isPopupAnim = GL_FALSE;
	GLboolean isCirleRotateAnim = GL_FALSE;

	//image materialss
	static GLfloat mat_amb[4];
	static GLfloat mat_dif[4];
	static GLfloat mat_nul[4];
	static GLfloat mat_shn[1];

	void setMaterial(){
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_amb);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_dif);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_dif);
		glMaterialfv(GL_FRONT, GL_SHININESS, mat_shn);
		glMaterialfv(GL_FRONT, GL_EMISSION, mat_dif);
	}

	void genIndex(GLboolean isNeedDpIndex){
		if(isNeedDpIndex){
			dpIndex=glGenLists(1);
			if(dpIndex){
				printf("gen list succ\n");
			}
			else{
				printf("gen list FAIL\n");
			}
		}
		else{
			printf("does not need to gen display list\n");
		}

		glGenTextures(1,&texIndex);
		if(texIndex){
			printf("gen tex succ\n");
		}
		else{
			printf("gen tex fail\n");
		}
		//printf("imagx index:%d %d\n\n",dpIndex,texIndex);
	}

	void compileDisplayList() {
		if(dpIndex){
			glNewList(dpIndex, GL_COMPILE);
			{
				glBindTexture(GL_TEXTURE_2D, texIndex);
				//CCW
				glBegin(GL_QUADS);
				{
					glNormal3f(0, 0, 1);
					glTexCoord2f(0, 0); glVertex3f(-v[0], -v[1], 0);
					glTexCoord2f(0, 1); glVertex3f(-v[0], v[1], 0);
					glTexCoord2f(1, 1); glVertex3f(v[0], v[1], 0);
					glTexCoord2f(1, 0); glVertex3f(v[0], -v[1], 0);
				}
				glEnd();
			}
			glEndList();
		}
		else{
			printf("does not generate display list\n");
		}
	}
};

class ImagxList{
public:
	std::vector<unsigned int> list;
	int i=0;
	ImagxList(){};
	~ImagxList(){};

	void push(unsigned id){
		list.push_back(id);
	}
	void draw(){
		glColor3ub(255, 255, 255);
		drawstr(-0.5, -1.5, "[IMG]DpIndex:%d", list[i]);
		

		glEnable(GL_TEXTURE_2D);glEnable(GL_BLEND);
		glCallList(list[i]);
		glDisable(GL_TEXTURE_2D);glDisable(GL_BLEND);
	}
	void specialKb(int key,int x,int y){
		if(key==100){
			//left arrow(dec index)
			i = (i+list.size()-1)%list.size();
		}
		if(key==102){
			//right arrow(inc index)
			i = (i+1)%list.size();
		}
	}
};