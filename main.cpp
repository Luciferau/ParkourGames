#include <graphics.h>
#include <stdlib.h>
#include <easyx.h>
#include <conio.h>
#include <stdio.h>
#include "tools.h"
#include <vector>
#include <time.h>
 
#define OBSTACLE_COUNT 5
#define WIN_WINDTH 1012
#define WIN_HEIGHT 396

using namespace std;


typedef  enum {

    TORTOISE,//0
    LION,
    HOOK1,
    HOOK2,
    HOOK3,
    HOOK4,
    OBSTACLE_TYPE_COUNT

}obstacle_type;

//IMAGE obstacleImgs[3][12];
//二维

vector<vector<IMAGE>>  obstacleImgs;//存放所有障碍的图片

Button btnStart;
typedef struct obstacle//障碍物
{
    obstacle_type type;//障碍物的类型
    int x, y;	//障碍物的坐标
    int speed;
    int power;//受到的伤害 
    bool exist;
    int imgIndex;//显示当前图片的序号
    bool hited;//表示是否已经发生碰撞
    bool passed;//表示障碍物是否被通过
}obstacle_t;

obstacle_t obstacles[OBSTACLE_COUNT];//障碍物的个数

//global variable
IMAGE imgsBgs[3];

int bgX[3];

int updata; //表示是否要刷新画面

int bgSpeed[3] = { 1,2,4 };

int jumpHeightMax;

IMAGE imgHeros[12];

IMAGE imgBackground;

int heroBlood;

int heroIndex;

int heroX, heroY;

bool heroJump;

bool heroJump2;//二段跳

bool heroGun;

bool heroDown;

IMAGE imgHeroDown[2];

IMAGE imgHeroGun[12];

int heroJumpOff;

int lastObsIndex;

int score;

IMAGE imgSZ[10];


void daoJiShi() 
{
   
    IMAGE img[6];
    char name[64];
    for (int i = 0; i < 6; i++) {
        sprintf(name, "res/%d.png", i);
        loadimage(&img[i], name,300,250);
    }

    for (int i = 5; i >= 0; i--) {
        BeginBatchDraw();
        cleardevice();
        putimage(355,85, &img[i]);
        EndBatchDraw();
        mciSendString("play res/hit.mp3", 0, 0, 0);
        Sleep(1000);
    }
    cleardevice();
    mciSendString("stop res/hit.mp3", 0, 0, 0);
}

void createObstacle()
{  
    srand((unsigned)time(NULL));
    int i;
   for( i = 0; i < OBSTACLE_COUNT;i++)
    {
        if (obstacles[i].exist == false) {
            break;
        }
    }

    if (i >= OBSTACLE_COUNT) {
        return;
    }
    obstacles[i].hited = false;
    obstacles[i].exist = true;
    obstacles[i].imgIndex = 0;
    //obstacles[i].type = (obstacle_type)(rand() % OBSTACLE_TYPE_COUNT);
     obstacles[i].type = (obstacle_type)(rand() % 3);
    obstacles[i].y =  345 + 5 -  obstacleImgs[obstacles[i].type][0].getheight();
     obstacles[i].x = WIN_WINDTH; 
     obstacles[i].passed = false;

     if (   lastObsIndex >= 0 
         &&obstacles[lastObsIndex].type >= HOOK1 
         && obstacles[lastObsIndex ].type<= HOOK4
         &&obstacles[i].type == LION
         &&obstacles[lastObsIndex].x>(WIN_WINDTH-500)){
         obstacles[i].type == TORTOISE;
     }
     lastObsIndex = i;//更新
     if (obstacles[i].type == HOOK1)
     {
         obstacles[i].type = (obstacle_type)((int)(obstacles[i].type) + rand() % 4);


     }
   
    if (obstacles[i].type == TORTOISE)
    {
        obstacles[i].speed = 0;
        obstacles[i].power = 5;

    }
      else if (obstacles[i].type == LION)
    {
      
        obstacles[i].speed = 4;
        obstacles[i].power = 20;
    }
    //&&而且
      else if (obstacles[i].type >= HOOK1&&obstacles[i].type <= HOOK4)
    {
        obstacles[i].power = 20;
        obstacles[i].speed = 0;
        obstacles[i].y = 0;
    }


}

void updataEnemy()
{
    

    for (int i = 0; i <  OBSTACLE_COUNT; i++)
    {
        if (obstacles[i].exist) {
             putimagePNG2(obstacles[i].x, obstacles[i].y, WIN_WINDTH, &obstacleImgs[obstacles[i].type] [obstacles[i].imgIndex]);

        }
    }

}

void init()
{
    initgraph(WIN_WINDTH, WIN_HEIGHT, EW_SHOWCONSOLE);  

    char name[64];

    //载入背景图片
    for (int i = 0; i < 3; i++)

    {
        
        sprintf(name, "res/bg%03d.png", i + 1);//generate file name
        //loadimage(&imgBgs[i],"file name");
        loadimage(&imgsBgs[i], name);
        bgX[i] = 0;
    }


    //载入人物1
    for (int i = 0;i < 12;i++)
    {
        // res/hero1.png ~ hero12.png
        sprintf(name, "res/hero%d.png", i + 1);
        loadimage(&imgHeros[i], name);

    }
    


    heroX = WIN_WINDTH * 0.5 - imgHeros[0].getwidth() * 0.5;
    heroY = 345 - imgHeros[0].getheight();

    heroJump = false;

    heroGun = false;

    jumpHeightMax = 345 - imgHeros[0].getheight() - 150;

    updata = true;

    heroJumpOff = -4;

    //乌龟
    IMAGE imgTort;
    loadimage(&imgTort, "res/t1.png");
    vector<IMAGE>imgTortArray;
    imgTortArray.push_back(imgTort);

    obstacleImgs.push_back(imgTortArray);


    //狮子
   
     IMAGE imgLion;
     vector<IMAGE> imgLionArry;

    for (int i = 0; i < 6; i++)
    {   
 
        sprintf(name, "res/p%d.png", i + 1);
        loadimage(&imgLion, name);
        imgLionArry.push_back(imgLion);
       
    } 
    obstacleImgs.push_back(imgLionArry);

    



    //对障碍物池进行初始化
    for (int i = 0; i < OBSTACLE_COUNT; i++)
    {
        obstacles[i].exist = false;//都不允许出场
    }


    loadimage(&imgHeroDown[0], "res/d1.png");
    loadimage(&imgHeroDown[1], "res/d2.png");
   
   for (int i = 0; i < 12; i++)
    {
        // res/hero1.png ~ hero12.png
        sprintf(name, "res/g%02d.png", i + 1);
        loadimage(&imgHeroGun[i], name);

    }
  

    heroDown = false;

    IMAGE imgH;
   
    for (int i = 0; i < 4; i++)
    {
        vector<IMAGE> imgHookArray;
        sprintf_s(name, sizeof(name), "res/h%d.png", i + 1);
        loadimage(&imgH, name,63,260,true);
        imgHookArray.push_back(imgH);
        obstacleImgs.push_back(imgHookArray);
    }

    initButton(&btnStart, "res/btn-normal.jpg", "res/btn-press.jpg" ,131, 58, 0);
    btnStart.x = 436;
    btnStart.y = 259;

    loadimage(&imgBackground, "res/over.png");
    heroBlood = 100;

    //预加载音效
    preLoadSound("res/hit.mp3");

    //mciSendString(" play res/bg.mp3 repeat", 0, 0, 0);

    lastObsIndex = -1;

    score = 0; 

    for (int i = 0; i < 10; i++)
    {
        sprintf(name, "res/sz/%d.png", i);
        loadimage(&imgSZ[i], name);
    }
}

 

void checkHit()
{ 
    for (int i = 0; i < OBSTACLE_COUNT; i++)
     {
         if (obstacles[i].exist&& obstacles[i].hited == false)
        {

            int a1x, a1y, a2x, a2y;
            int off = 20;
            if (!heroDown)//非下蹲 奔跑 跳跃
            {
                a1x = heroX + off ;
                a1y = heroY + off;

                a2x = heroX + imgHeros[heroIndex].getwidth() -  off;
                a2y = heroY + imgHeros[heroIndex].getheight();


            }
            else { //下蹲状态
                a1x = heroX + off;
                a1y = 345  - imgHeroDown[heroIndex].getheight();

                a2x = heroX + imgHeroDown[heroIndex].getwidth() - off ;
                a2y =345;
            }
            
            IMAGE img = obstacleImgs[obstacles[i].type][obstacles[i].imgIndex];

            int b1x = obstacles[i].x + off;
            int b1y = obstacles[i].y + off;
            int b2x = obstacles[i].x + img.getwidth() - off;  
            int b2y = obstacles[i].y + img.getheight() - 10;

            if (rectIntersect(a1x, a1y, a2x, a2y, b1x, b1y, b2x, b2y))
            {
                //mciSendString("play res/hit.mp3", 0, 0, 0);
                heroBlood -= obstacles[i].power;
               printf("血量剩余 %d\n",heroBlood);
               playSound("res/hit.mp3");
               obstacles[i].hited = true;
            }
        }
    }

}




void fly()
{
    srand((unsigned)time(NULL));
    for (int i = 0; i < 3; i++)
    {
        // same speed
        bgX[i] -= 2;
        if (bgX[i] < -WIN_WINDTH)
        {
            bgX[i] = 0;
        }

    }

    //实现跳跃
    if (heroJump)
    {


        if (heroY < jumpHeightMax)
        {
            //上升
            heroJumpOff = 4;

        }

        heroY += heroJumpOff;

        if (heroY > 345 - imgHeros[0].getheight())
        {
            heroJump = false;
            heroJumpOff = -4;
        }
        

    }
    else if (heroDown)
    {
        static int count = 0;
        count++; 
        int delays[2] = { 8,30 };
        if (count >= delays[heroIndex])
        {
            count = 0;
            heroIndex++;

            if (heroIndex >= 2){
                heroIndex = 0;
                heroDown = false;
            }

        }

    }
    else if (heroGun)
    {
       
            heroIndex++;
            Sleep(5);

            if (heroIndex >= 12) {
               
                heroGun = false;
            }

    }
    else
    {//不跳跃的时候
        heroIndex = (heroIndex + 1) % 12;
    }

    //创建障碍物
    static int frameCount = 0;
    static int enemyFre = 50;
    frameCount++;
    if (frameCount > enemyFre) {
        frameCount = 0;
        enemyFre = 50 + rand() % 50;
        createObstacle();
    }

    //更新所有障碍物的坐标
    for (int i = 0; i < OBSTACLE_COUNT; i++)
    {
        if (obstacles[i].exist)
        {
            obstacles[i].x -= (obstacles[i].speed + bgSpeed[2]);


            if (obstacles[i].x < (-(obstacleImgs[obstacles[i].type][0].getwidth() * 2) ) )//跑出screen
            {
                obstacles[i].exist = false;

            }
            int len = obstacleImgs[obstacles[i].type].size();

            obstacles[i].imgIndex = (obstacles[i].imgIndex + 1) % len;

        }

        //碰撞检测
        checkHit();

    }      

}


void updataBg()
{
    putimagePNG2(bgX[0], 0, &imgsBgs[0]);
    putimagePNG2(bgX[1], 119, &imgsBgs[1]);
    putimagePNG2(bgX[2], 330, &imgsBgs[2]);
}


void down()
{
    updata = true;
    heroDown = true;
    heroIndex = 0;
}

void jump()
{

    heroJump = true;
    updata = true;

}

void gun()
{
    heroGun = true;
    updata = true;
}



void keyEvent()
{
    char ch;
    if (_kbhit())//如果有按键输入
    {

        ch = _getch();//_getch() 不需要按下回车 直接读取

        if (ch == 'w')
        {
            jump();
        }
        else if (ch == 's')
        {
            down();
        }
        else if (ch == 'd')
        {
            gun();//滚
        }
         
       
    }


}



void updataHero()
{
    if (!heroDown && !heroGun)
    {
        putimagePNG2(heroX, heroY,
            &imgHeros[heroIndex]);
    }else if(heroDown) {
        int y = 345 - (imgHeroDown[heroIndex].getheight());
            putimagePNG2(heroX, y, &imgHeroDown[heroIndex]);

    }
    else if (heroGun) {
        int y = 345 - (imgHeroGun[heroIndex].getheight());
        putimagePNG2(heroX, y, &imgHeroGun[heroIndex]);
    }
}

void welcome()
{
    mciSendString("play res/bg.mp3", 0, 0, 0);
    
    for (;;)
    {
        MOUSEMSG m = GetMouseMsg();
        FlushMouseMsgBuffer(); //不能少，后缀快速拖动顶部的标题按钮，讲导致鼠标消息太多
        switch (m.uMsg)
        {
        case WM_MOUSEMOVE:
            if (checkButtonSelect(&btnStart, &m)) {
                btnStart.pressed = true;
                drawButton(&btnStart);
            }
            else {
                btnStart.pressed = false;
                drawButton(&btnStart);
            }
            break;

        case WM_LBUTTONDOWN:
                    if (checkButtonSelect(&btnStart, &m))
                    {
                            btnStart.pressed = true;
                            drawButton(&btnStart);
                            break;                     
                    }
                        
                         
        case WM_LBUTTONUP:
            if ( checkButtonSelect(&btnStart, &m) )
            {

                if (btnStart.pressed)
                {
                    mciSendString("stop res/bg.mp3", 0, 0, 0);
                   // daoJiShi();
                    return;
                }
            }

            break;
        }

    }

}
void updataBloodBar()
{
    drawBloodBar(10, 10, 200, 10, 2, BLUE, DARKGRAY, RED,heroBlood/100.0);
}

void   checkOver()
{
    if (heroBlood <= 0)
    {
      
       // cleardevice();
        loadimage(0, "res/over.png");
        FlushBatchDraw();
        mciSendString("stop res/bg.mp3", 0, 0, 0);
        system("pause");
        
         heroBlood = 100;
         score = 0; 
        mciSendString("play res/bg.mp3 repeat", 0, 0, 0);
  
     }  
}

void checkScore()
{
    for (int i = 0; i < OBSTACLE_COUNT; i++)
    {
        if (obstacles[i].exist &&
                obstacles[i].passed == false && 
                obstacles[i].hited ==false && 
                obstacles[i].x + obstacleImgs[obstacles[i].type][0].getwidth() < heroX)  {
      

            score++;//加分
        obstacles[i].passed = true;
        printf("score :%d \n", score);
        }

    }
 }

void  updataScore()
{
    int x = 20, y = 25;
    char str[8];
    sprintf(str, "%d", score);
    for (int i = 0; str[i]; i++)
    {
        int sz = str[i] - '0';
        putimagePNG(x,y, &imgSZ[sz]);

        x += imgSZ[sz].getwidth() + 5;
    }
}


int main()
{
    // test();

    init();
   // putimage(0,0,&imgBackground);
  // drawButton(&btnStart);

   // welcome();


   
    int timer   = 0;//调用时间

    while (1)
    {


        keyEvent();

        timer += getDelay();

        if (timer > 30)
        {
            timer = 0;
            updata = true;

        }

        if (updata)
        {
            updata = false;//不能一直刷
            BeginBatchDraw();
            updataBg();
            updataHero();
            updataEnemy();
            updataBloodBar();

            updataScore();
            FlushBatchDraw();

            checkOver();

            checkScore();

            fly();

        }
       

    }
    return 0;
}




int test()
{
    IMAGE imgsBg[3];
    initgraph(WIN_WINDTH, WIN_HEIGHT);
    char name[64];

    //Load background resources
    for (int i = 0; i < 3; i++)

    {

        sprintf(name, "res/bg%03d.png", i + 1);//generate file name
        //loadimage(&imgBgs[i],"file name");
        loadimage(&imgsBgs[i], name);
        bgX[i] = 0;
    }
    putimagePNG2(0, 0, &imgsBgs[0]);
    putimagePNG2(0, 119, &imgsBgs[1]);
    putimagePNG2(0, 330, &imgsBgs[2]);
    Sleep(10000);

    return 0;
}