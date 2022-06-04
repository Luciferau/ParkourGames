#pragma once
#include <graphics.h>

struct Button {
	IMAGE imgNormal;
	IMAGE imgPress;
	int width, highth;
	int x, y;
	int flag; // ��ť��int���ͱ��
	bool pressed;
};

struct size {
	int width;
	int height;
};
//���ؾ�����һ�ε��ü����ʱ�䣨��λ��ms)����һ�ε���ʱ����0
int getDelay(); 

void putimagePNG(int  picture_x, int picture_y, IMAGE* picture);

// ������ y <0 �Լ�y>0���κ����
void putimagePNG2(int x, int y, IMAGE* picture);
void putimagePNG2(int x, int y, int winWidth, IMAGE* picture);

// �ж����������Ƿ��ཻ
bool rectIntersect(int a1X, int a1Y, int a2X, int a2Y,
    int b1X, int b1Y, int b2X, int b2Y);

void  preLoadSound(const char* name);
void  playSound(const char* name);

void drawBloodBar(int x, int y, int width, int height, int lineWidth, int boardColor, int emptyColor, int fillColor, float percent);

void initButton(Button* btn,
	const char* normalFile, const char* pressFile,
	int width, int highth, int flag);

bool checkButtonSelect(Button* btn, MOUSEMSG* msg);

void drawPNG(int  picture_x, int picture_y, IMAGE* picture);//xΪ����ͼƬ��X���꣬yΪY����

void drawButton(Button* btn);