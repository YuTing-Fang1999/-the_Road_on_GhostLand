#pragma once
#include<Windows.h>
#include<GL/glew.h>
#include<GL/freeglut.h>
#include<stdio.h>
#include"Imagx.h"
#include"glm.h"
class ObjectLoader{
public:
	ObjectLoader(){}
	/*
	只能用一張貼圖
	objpath : 物件路徑名稱
	texpath : 貼圖路徑名稱，不需貼圖填NULL
	unit_flag : 是否開啟glmUnitize()功能
	mode : GLM_...的render參數選項
	textrue,smooth,material...
	*/
	ObjectLoader(char* objpath,char* texpath,GLboolean unit_flag,GLuint mode){
		genIndex(1);
		//GL_FALSE menas load image as only a textrue
		if(texpath!=NULL){
			Imagx texImg = Imagx(texpath,1,GL_FALSE);
			texIndex = texImg.getTexIndex();
		}
		model = myReadOBJ(objpath,unit_flag);
		compileDisplayList(dpIndex,mode);
		glmDelete(model);
	}
	~ObjectLoader(){}

	void drawObj(GLuint index){
		if(model){
			glCallList(index);
		}
		else{
			printf("model is null\n");
		}
	}

	void progress(){
	
	}

	GLuint getDpIndex(){return dpIndex;}
	GLuint getTexIndex(){return texIndex;}
	/*GLuint getDpNum(){return dp_num;}*/

private:
	GLMmodel* model;
	GLuint dpIndex=0;
	GLuint texIndex=0;
	/*GLuint dp_num=0;*/

	/*
	gen texture and display indeics;
	*/
	void genIndex(GLsizei di){
		dpIndex = glGenLists(di);
	}

	/*
	compile one loaded object
	*/
	void compileDisplayList(GLuint index,GLuint mode){
		if(index){
			glNewList(index,GL_COMPILE);
			{
				glEnable(GL_TEXTURE_2D);glEnable(GL_BLEND);
				glPushMatrix();
				{
					if(GLM_TEXTURE)glBindTexture(GL_TEXTURE_2D, texIndex);
					glmDraw(model,mode);
				}
				glPopMatrix();
				glDisable(GL_TEXTURE_2D);glDisable(GL_BLEND);
			}
			glEndList();
		}
		else{
			printf("objloader.compileDisplayList: dpIndex is 0\n");
		}
	}
};

class ObjList{
public:
	std::vector<unsigned int> list;
	int index=0;
	ObjList(){};
	~ObjList(){};

	void push(unsigned id){
		list.push_back(id);
	}
	void draw(){
		glEnable(GL_TEXTURE_2D);glEnable(GL_BLEND);
		glCallList(list[index]);
		glDisable(GL_TEXTURE_2D);glDisable(GL_BLEND);
	}
	void specialKb(int key,int x,int y){
		if(key==100){
			//left arrow
			index = (index+1)%list.size();
		}
		if(key==102){
			//right arrow
			index = (index+list.size()-1)%list.size();
		}
	}
};