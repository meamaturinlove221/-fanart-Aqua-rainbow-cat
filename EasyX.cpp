#include "EasyX.h"
clock_t startClock, processClock;
clock_t startaidClock, stopClock;
int directLvl = 1;
Aqua aqua;
Pee pee[PeeMaxNum];
Enemy enemy[EnemyMaxNum];
Aid aid[AidMaxNum];
IMAGE bg;
bool gameQuit = false;
int peeNum, enemyNum;
int aidSpeed;
double enemySpeed;
int  enemyMoveDealy;
bool canChangeLevel = true;
bool gameStart = true;
double addEnemySpd;
double upAidSpd;

void Initial() 
{
	aidSpeed = 1;
	enemySpeed = 1;
	enemyMoveDealy = 5;
	addEnemySpd = 1.5;
	upAidSpd = 0.5;
	for (int i = 0; i < PeeMaxNum; i++)
	{
		loadimage(&pee[i].img, _T("data/long pee.png"));
	}
	for (int i = 0; i < EnemyMaxNum; i++)
	{
		Resize(&enemy[i].img, 128, 128);
		int type1 = rand() % 4;
		enemy[i].type1 = type1;
		switch (type1) 
		{
		case 0:
			loadimage(&enemy[i].img, _T("data/enemy1.png"));
			break;
		case 1:
			loadimage(&enemy[i].img, _T("data/enemy2.png"));
			break;
		case 2:
			loadimage(&enemy[i].img, _T("data/enemy3.png"));
			break;
		case 3:
			loadimage(&enemy[i].img, _T("data/enemy4.png"));
			break;
		}
		enemy[i].type1 = type1;
		mciSendString("open data/background.wma alias bkmusic", NULL, 0, NULL);
		mciSendString("play bkmusic repeat", NULL, 0, NULL);  // 循环播放背景音乐
	}
	for (int i = 0; i < AidMaxNum; i++)
	{
		Resize(&aid[i].img, 128,128);
		int type = rand() % 4;
		aid[i].type = type;
		switch (type)
		{
		case 0:
			loadimage(&aid[i].img, _T("data/aid1.png"));
			break;
		case 1:
			loadimage(&aid[i].img, _T("data/aid2.png"));
			break;
		case 2:
			loadimage(&aid[i].img, _T("data/aid3.png"));
			break;
		case 3:
			loadimage(&aid[i].img, _T("data/aid4.png"));
			break;
		}
		aid[i].type = type;
	}
	for (int i = 0; i < EnemyMaxNum; i++) 
	{
		enemy[i].show = false;
	}
	for (int i = 0; i < AidMaxNum; i++) 
	{
		aid[i].show = false;
	}
	Resize(&aqua.img, 128, 128);
	peeNum = 0;
	enemyNum = 0;
	gameQuit = false;
	aqua.pos.x = 250;
	aqua.pos.y = 350;
	aqua.score = 0;
	aqua.peeNums = 0;
	aqua.fullHp = 3;
	aqua.hp = aqua.fullHp;

	loadimage(&aqua.img, _T("data/aqua.png"));
	loadimage(&bg, _T("data/background.png"));

	startClock = clock();
	startClock = clock();
}

void Run() 
{
	Initial();
	initgraph(WINDOW_WIDTH, WINDOW_HEIGHT);
	while (!gameQuit)
	{
		Draw();
		Input();
		if (gameStart) 
		{
			Logic();
		}
	}
	closegraph();
}

void Draw() 
{
	BeginBatchDraw();
	putimage(0, 0, &bg);
	drawText();
	drawPee();
	drawEnemy();
	drawAqua();
	drawAid();
	if (!gameStart)
	{
		drawTips();
	}
	EndBatchDraw();
}

void drawTips()
{
	IMAGE tips;
	loadimage(&tips, _T("data/hint.png"));
	putimage(0, WINDOW_HEIGHT / 2 - tips.getheight() / 2, &tips);
}

void drawText()
{
	clearrectangle(STAGE_WIDTH, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	TCHAR t1[] = _T("鼠标控制移动");
	TCHAR t2[] = _T("左键发射子弹");
	TCHAR t3[] = _T("吃到急救包+1");
	TCHAR t4[] = _T("击中敌人+1");
	TCHAR t5[] = _T("子弹打到急救包+1");
	TCHAR t6[] = _T("碰到敌人护甲-1");

	TCHAR sScore[20];
	_stprintf_s(sScore, 20, _T("当前得分：%d"), aqua.score);
	TCHAR sHp[20];
	_stprintf_s(sHp, 20, _T("当前护甲: %d"), aqua.hp);
	settextcolor(RGB(255, 255, 255));
	outtextxy(500, 20, t1);
	outtextxy(500, 40, t2);
	outtextxy(500, 60, t3);
	outtextxy(500, 100, t4);
	outtextxy(500, 120, t5);
	outtextxy(500, 140, t6);
	outtextxy(500, 300, sScore);
	outtextxy(500, 360, sHp);
}

void drawAqua() 
{
	putAlphaImage(NULL, aqua.pos.x, aqua.pos.y, &aqua.img, 0x000000);
}

void drawPee()
{
	for (int i = 0; i < PeeMaxNum; i++) 
	{
		if (pee[i].show) 
		{
			putAlphaImage(NULL, pee[i].pos.x, pee[i].pos.y, &pee[i].img, 0x000000);
		}
	}
}

void drawEnemy() 
{
	for (int i = 0; i < EnemyMaxNum; i++) 
	{
		if (enemy[i].show) 
		{
			putAlphaImage(NULL, enemy[i].pos.x, enemy[i].pos.y, &enemy[i].img, 0x000000);
		}
	}
}
void Input() 
{
	MOUSEMSG mouse;
	while (MouseHit()) 
	{
		mouse = GetMouseMsg();
		switch (mouse.uMsg) 
		{
		case WM_MOUSEMOVE:
			aqua.pos.x = mouse.x - aqua.img.getwidth() / 2;
			aqua.pos.y = mouse.y - aqua.img.getheight() / 2;
			break;
		case WM_LBUTTONUP:
			for (int i = 0; i < PeeMaxNum; i++) 
			{
				if (aqua.peeNums > 0) 
				{
					if (pee[i].show == false) 
					{
						pee[i].show = true;
						pee[i].pos.x = aqua.pos.x - aqua.img.getwidth() / 2 - pee[i].img.getwidth() / 2;
						pee[i].pos.y = aqua.pos.y + aqua.img.getheight() / 2 - pee[i].img.getheight() / 2;	
						break;
					}
				}
			}
			if (!gameStart) 
			{
				gameStart = true;
				Initial();
			}
			break;
		case WM_RBUTTONUP:
			for (int i = 0; i < PeeMaxNum; i++)
			{
				if (aqua.peeNums > 0) 
				{
					if (pee[i].show == false) 
					{
						pee[i].show = true;
						if (i % 2 == 0) 
						{
							pee[i].pos.y = aqua.pos.y + aqua.img.getheight() / 2 - pee[i].img.getheight() / 2 + 10 * i;
						}
						else 
						{
							pee[i].pos.y = aqua.pos.x + aqua.img.getwidth() / 2 - pee[i].img.getheight() / 2 - 10 * i;
						}
						pee[i].pos.y = aqua.pos.y - aqua.img.getheight() / 2 - pee[i].img.getheight() / 2;
						aqua.peeNums--;				
					}
				}
			}
			if (!gameStart) 
			{
				gameQuit = true;
			}
			break;
		}
	}
}

void Logic() 
{
	for (int i = 0; i < PeeMaxNum; i++) 
	{
		if (pee[i].show) 
		{
			pee[i].pos.x -= aidSpeed;
			if (pee[i].pos.x <= 0)
			{
				pee[i].pos = { 0,0 };
				pee[i].show = false;
			}
		}
	}
	for (int j = 0; j < EnemyMaxNum; j++) 
	{
		if (enemy[j].show) 
		{
			if (HitAqua(&enemy[j], &aqua)) 
			{
				aqua.hp -= 1;
			}
			for (int i = 0; i < PeeMaxNum; i++)
			{
				if (pee[i].show) {
					if (HitPee(&pee[i], &enemy[j]))
					{
						aqua.score++;
						break;
					}
				}
			}
			for (int k = 0; k < EnemyMaxNum; k++) 
			{
				if (k == j) 
				{
					break;
				}
				if (enemy[k].show) 
				{
					if (HitEnemy(&enemy[j], &enemy[k]))
					{
						aqua.score += 1;
						break;
					}
				}
			}
		}
	}
	addEnemy();
	addAid();
	enemyMoveDown();
	aidMoveDown();
	getAid();
	stayStage();
	if (aqua.hp <= 0) 
	{
		gameStart = false;
	}
}

void putAlphaImage(IMAGE* dstimg, int x, int y, IMAGE* srcimg, UINT transparentcolor) 
{			//为了展示png文件，直接调用windows GDI
	HDC dstDC = GetImageHDC(dstimg);
	HDC srcDC = GetImageHDC(srcimg);
	int w = srcimg->getwidth();
	int h = srcimg->getheight();
	// 使用 Windows GDI 函数实现透明位图
	TransparentBlt(dstDC, x, y, w, h, srcDC, 0, 0, w, h, transparentcolor);
}

void addEnemy() 
{
	for (int i = 0; i < EnemyMaxNum; i++)
	{
		processClock = clock();						//结束增加敌机计时
		if (!enemy[i].show) 
		{
			if ((stopClock - startClock) >= int(1000 * upAidSpd)) 
			{		//0.5秒增加一个血包
				int y = rand() % 500;
				POINT pos = { 0,y };
				enemy[i].pos = pos;
				enemy[i].show = true;
				startClock = clock();				//增加敌机计时

				enemy[i].startClock = clock();		//敌人计时
				enemy[i].endClock = clock();
			}
		}
	}
}

bool moveDelay(clock_t c1, clock_t c2, int delay) 
{
	if (c2 - c1 >= delay)
	{
		return true;
	}
	return false;
}

bool HitPee(Pee* bullet, Enemy* enemy) 
{
	if (bullet->pos.y < enemy->pos.y + enemy->img.getheight() &&
		enemy->pos.y < bullet->pos.y + bullet->img.getheight() &&
		enemy->pos.x < bullet->pos.x + bullet->img.getwidth() &&
		bullet->pos.x < enemy->pos.x + enemy->img.getwidth()
		) {
		bullet->show = false;
		bullet->pos = { -100,-100 };
		enemy->show = false;
		enemy->pos = { -100,-100 };
		return true;
	}
	return false;
}

bool HitEnemy(Enemy* enemy1, Enemy* enemy2)
{
	if (enemy1->pos.y < enemy2->pos.y + enemy2->img.getheight() &&
		enemy2->pos.y < enemy1->pos.y + enemy1->img.getheight() &&
		enemy2->pos.x < enemy1->pos.x + enemy1->img.getwidth() &&
		enemy1->pos.x < enemy2->pos.x + enemy2->img.getwidth())
	{
		enemy1->show = false;
		enemy1->pos = { -100,-100 };
		enemy2->show = false;
		enemy2->pos = { -100,-100 };
		return true;
	}
	return false;
}

bool HitAqua(Enemy* enemy1, Aqua* enemy2) 
{
	if (enemy1->pos.y < enemy2->pos.y + enemy2->img.getheight() &&
		enemy2->pos.y < enemy1->pos.y + enemy1->img.getheight() &&
		enemy2->pos.x < enemy1->pos.x + enemy1->img.getwidth() &&
		enemy1->pos.x < enemy2->pos.x + enemy2->img.getwidth()) 
	{
		enemy1->show = false;
		enemy1->pos = { 0,0 };
		return true;
	}
	return false;
}


	

void addAid() 
{
	for (int i = 0; i < AidMaxNum; i++) 
	{
		stopClock = clock();						//结束增加急救包计时
		if (!aid[i].show) 
		{
			if ((stopClock - startaidClock) >= int(1000 * upAidSpd)) 
			{		//0.5秒增加一个血包
				int y = rand() % 500;
				POINT pos = { 0,y};
				aid[i].pos = pos;
				aid[i].show = true;
				startaidClock = clock();				//增加急救包计时

				aid[i].startaidClock = clock();		//敌人计时
				aid[i].endClock = clock();
			}
		}
	}
}

void drawAid() 
{
	for (int i = 0; i < AidMaxNum; i++) 
	{
		if (aid[i].show) 
		{
			putAlphaImage(NULL, aid[i].pos.x, aid[i].pos.y, &aid[i].img, 0x000000);
		}
	}
}

void aidMoveDown() 
{
	for (int i = 0; i < AidMaxNum; i++)
	{
		if (aid[i].show) 
		{
			aid[i].endClock = clock();
			if (moveDelay(aid[i].startaidClock, aid[i].endClock, enemyMoveDealy)) 
			{
				aid[i].startaidClock = clock();
				aid[i].pos.x += aidSpeed;
				if (aid[i].pos.x > 700) 
				{
					aid[i].pos = { 0,0 };
					aid[i].show = false;
				}
			}
		}
	}
}

void enemyMoveDown() 
{
	for (int i = 0; i < EnemyMaxNum; i++)
	{
		if (enemy[i].show)
		{
			enemy[i].endClock = clock();
			if (moveDelay(enemy[i].startClock, enemy[i].endClock, enemyMoveDealy))
			{
				enemy[i].startClock = clock();
				enemy[i].pos.x += enemySpeed;
				if (enemy[i].pos.x > 700) 
				{
					enemy[i].pos = { 0,0 };
					enemy[i].show = false;
				}
			}
		}
	}
}

void getAid() 
{
	for (int i = 0; i < AidMaxNum; i++)
	{
		if (aid[i].show)
		{
			if (aid[i].pos.y < aqua.pos.y + aqua.img.getheight() &&
				aqua.pos.y < aid[i].pos.y + aid[i].img.getheight() &&
				aqua.pos.x < aid[i].pos.x + aid[i].img.getwidth() &&
				aid[i].pos.x < aqua.pos.x + aqua.img.getwidth()) 
			{
					if (aid[i].type == 3) 
					{
						if (aqua.hp < aqua.fullHp) 
						{
							aqua.hp += 1;
						}
					}
					aqua.peeNums++;
					aqua.score++;
					
				aid[i].pos = { 0,0 };
				aid[i].show = false;
				
			}
			for (int j = 0; j < EnemyMaxNum; j++)
			{
				if (enemy[j].show) 
				{
					if (aid[i].pos.y < enemy[j].pos.y + enemy[j].img.getheight() &&
						enemy[j].pos.y < aid[i].pos.y + aid[i].img.getheight() &&
						enemy[j].pos.x < aid[i].pos.x + aid[i].img.getwidth() &&
						aid[i].pos.x < enemy[j].pos.x + enemy[j].img.getwidth()) 
					{
						aid[i].pos = { 0,0 };
						aid[i].show = false;
						aqua.score -= 1;
					}
				}
			}
			for (int k = 0; k < PeeMaxNum; k++) 
			{
				if (pee[k].show)
				{
					if (aid[i].pos.y < pee[k].pos.y + pee[k].img.getheight() &&
						pee[k].pos.y < aid[i].pos.y + aid[i].img.getheight() &&
						pee[k].pos.x < aid[i].pos.x + aid[i].img.getwidth() &&
						aid[i].pos.x < pee[k].pos.x + pee[k].img.getwidth())
					{
						pee[k].show = false;
						pee[k].pos = { 0,0 };

						
						
						if (aid[i].hp <= 0)
						{
							aid[i].pos = { 0,0 };
							aid[i].show = false;
						}
						aqua.score += 1;
					}
				}
			}
		}
	}
}

void stayStage() 
{//保持在舞台上
	if (aqua.pos.x > STAGE_WIDTH - aqua.img.getwidth()) 
	{
		aqua.pos.x = STAGE_WIDTH - aqua.img.getwidth();
	}
	if (aqua.pos.x < 0)
	{
		aqua.pos.x = 0;
	}
	if (aqua.pos.y > STAGE_WIDTH - aqua.img.getheight())
	{
		aqua.pos.y = STAGE_WIDTH - aqua.img.getheight();
	}
	if (aqua.pos.y < 0) 
	{
		aqua.pos.y = 0;
	}
	for (int i = 0; i < AidMaxNum; i++) 
	{
		if (aid[i].pos.y > STAGE_WIDTH - aid[i].img.getheight()) 
		{
			aid[i].pos.y = STAGE_WIDTH - aid[i].img.getheight();
		}
		if (aid[i].pos.y < 0) 
		{
			aid[i].pos.y = 0;
		}
		if (aid[i].pos.x > STAGE_WIDTH - aid[i].img.getwidth()) 
		{
			aid[i].pos.x = STAGE_WIDTH - aid[i].img.getwidth();
		}
		if (aid[i].pos.x < 0)
		{
			aid[i].pos.x = 0;
		}
	}
	for (int i = 0; i < EnemyMaxNum; i++) 
	{
		if (enemy[i].pos.y > STAGE_WIDTH - enemy[i].img.getheight())
		{
			enemy[i].pos.y = STAGE_WIDTH - enemy[i].img.getheight();
		}
		if (enemy[i].pos.y < 0) 
		{
			enemy[i].pos.y = 0;
		}
		if (enemy[i].pos.x > STAGE_WIDTH - enemy[i].img.getwidth()) 
		{
			enemy[i].pos.x = STAGE_WIDTH - enemy[i].img.getwidth();
		}
		if (enemy[i].pos.x < 0)
		{
			enemy[i].pos.x = 0;
		}
	}
}