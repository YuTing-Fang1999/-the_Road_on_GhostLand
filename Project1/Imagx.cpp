#include"Imagx.h"

//defines the static member
//靜態變數要宣告在.cpp，如果再header裡面會重複宣告
GLfloat Imagx::mat_amb[4] = { 0.1,0.1,0.1,1 };
GLfloat Imagx::mat_dif[4] = { 1,1,1,1 };
GLfloat Imagx::mat_nul[4] = { 0,0,0,1 };
GLfloat Imagx::mat_shn[1] = { 500 };