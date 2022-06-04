#pragma once
#include <graphics.h>

struct Button {
	IMAGE imgNormal;
	IMAGE imgPress;
	int width, highth;
	int x, y;
	int flag; // 按钮的int类型标记
	bool pressed;
};

struct size {
	int width;
	int height;
};
//返回距离上一次调用间隔的时间（单位：ms)，第一次调用时返回0
int getDelay(); 

void putimagePNG(int  picture_x, int picture_y, IMAGE* picture);

// 适用于 y <0 以及y>0的任何情况
void putimagePNG2(int x, int y, IMAGE* picture);
void putimagePNG2(int x, int y, int winWidth, IMAGE* picture);

// 判断两个矩形是否相交
bool rectIntersect(int a1X, int a1Y, int a2X, int a2Y,
    int b1X, int b1Y, int b2X, int b2Y);

void  preLoadSound(const char* name);
void  playSound(const char* name);

void drawBloodBar(int x, int y, int width, int height, int lineWidth, int boardColor, int emptyColor, int fillColor, float percent);

void initButton(Button* btn,
	const char* normalFile, const char* pressFile,
	int width, int highth, int flag);

bool checkButtonSelect(Button* btn, MOUSEMSG* msg);

void drawPNG(int  picture_x, int picture_y, IMAGE* picture);//x为载入图片的X坐标，y为Y坐标

void drawButton(Button* btn);