#include <easyx.h>
#include <conio.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#pragma comment(lib,"Winmm.lib")
#pragma comment(lib, "MSIMG32.LIB")

#define PeeMaxNum 1
#define EnemyMaxNum 2
#define AidMaxNum 2
#define STAGE_WIDTH 500
#define WINDOW_WIDTH 700
#define WINDOW_HEIGHT 600

struct Aqua 
{				//����
	POINT pos = { 0,0 };	//λ��
	IMAGE img;			//ͼƬ
	int score = 0;				//�÷�
	int peeNums = 0;			//�ӵ�����
	int aidNUM;		//���Ȱ���Ѫ����
	int hp = 3;
	int fullHp = 3;

};
struct Pee 
{
	POINT pos = { 0,0 };
	IMAGE img;
	int type = 0;
	int speed = 1;
	bool show = false;
};
struct Enemy 
{
	IMAGE img;
	POINT pos = { STAGE_WIDTH,STAGE_WIDTH / 2 };
	bool show = false;
	clock_t startClock = NULL, endClock = NULL;		//��������ʾ
	int hp = 1;
	int type1;
};
struct Aid 
{
	IMAGE img;
	POINT pos = { 0,0 };
	bool show = true;
	clock_t startaidClock = NULL, endClock = NULL;		//����
	int hp = 1;
	int type;
};
void putAlphaImage(IMAGE* dstimg, int x, int y, IMAGE* srcimg, UINT transparentcolor);
void Initial();
void Draw();
void drawText();
void drawAqua();
void drawPee();
void drawEnemy();
void Input();
void Logic();
void Run();
void addEnemy();
bool moveDelay(clock_t c1, clock_t c2, int delay);	//����
bool HitPee(Pee* pee, Enemy* enemy);	
bool HitEnemy(Enemy* enemy1, Enemy* enemy2);		
bool HitAqua(Enemy* enemy1, Aqua* enemy2);	//�����������
void addAid();
void drawAid();
void aidMoveDown();
void enemyMoveDown();
void getAid();					//�Ե����Ȱ�
void stayStage();
void drawTips();					//��ʾ