#pragma once
#include<Windows.h>
#include<GL/glew.h>
#include<GL/freeglut.h>
#include<stdio.h>
#define _USE_MATH_DEFINES
#include<math.h>
#include "FreeImage.h"

class Imagx {
public:
	Imagx(){}
	/*
	imgfile is the name of the image.
	s is the maxScale of the image.
	x-------------------------------------
	isNeedDpIndex
	GL_TRUE -> make a image board
	GL_FALSE-> make only image textrue
	*/
	Imagx(char* imgfile, GLfloat s,GLboolean isNeedDpIndex) {
		genIndex(isNeedDpIndex);
		max_scl = s;
		scl = max_scl;

		image_format = FreeImage_GetFileType(imgfile, 0);				//檢查圖片格式
		bitmap = FreeImage_Load(image_format, imgfile, BMP_DEFAULT);	//載入圖片
		if (bitmap) {
			// bitmap successfully loaded!
			printf("load %s!\n\n", imgfile);

			getImgInfo();
			genTexture();
			if(isNeedDpIndex){
				setMaterial();
				compileDisplayList();
			}

			FreeImage_Unload(bitmap);
		}
		else {
			printf("no load image!\n");
		}
	}
	~Imagx() {}

	/*.
	show the infomation of the image has been loaded
	*/
	void printImgInfo() {
		printf("imageInfo:\n");
		printf("H:%u W:%u imageType:%d colorType:%d scale:%f\n\n", height, width, image_type, color_type,scl);
	}

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
	/*
	設定圖片最大縮放值
	*/
	void setMaxScale(GLuint s) {
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
	FIBITMAP* bitmap;
	FREE_IMAGE_TYPE image_type;
	FREE_IMAGE_FORMAT image_format;
	FREE_IMAGE_COLOR_TYPE color_type;
	GLfloat v[2];
	GLuint height;
	GLuint width;
	BYTE* bits;

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

	void getImgInfo() {
		//parameter is a referance
		image_type = FreeImage_GetImageType(bitmap);
		color_type = FreeImage_GetColorType(bitmap);
		height = FreeImage_GetHeight(bitmap);
		width = FreeImage_GetWidth(bitmap);
		bits = FreeImage_GetBits(bitmap);
		//符合圖片比例
		v[0] = (float)width / (float)width;
		v[1] = (float)height / (float)width;
	}

	void genTexture() {
		if(texIndex){
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glBindTexture(GL_TEXTURE_2D, texIndex);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

			GLenum data_color_format = GL_BGR;
			GLenum internal_color_format = GL_RGB;
			if (color_type == FIC_RGB){
				data_color_format = GL_BGR;
				internal_color_format = GL_RGB;
			}
			else if (color_type == FIC_RGBALPHA){
				data_color_format = GL_BGRA;
				internal_color_format = GL_RGBA;
			}
			else { printf("else format"); }
			glTexImage2D(GL_TEXTURE_2D, 0, internal_color_format, width, height,
				0, data_color_format, GL_UNSIGNED_BYTE,
				bits);
			//感覺是這邊造成圖片歪斜，特別是沒有透明度的檔案
		}
		else{
			printf("does not generate texture list\n");
		}
	}

	void setMaterial(){
		glMaterialfv(GL_FRONT, GL_AMBIENT, mat_amb);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_dif);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_dif);
		glMaterialfv(GL_FRONT, GL_SHININESS, mat_shn);
		//glMaterialfv(GL_FRONT, GL_EMISSION, mat_nul);
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