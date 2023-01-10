#pragma comment(lib, "msimg32.lib")
#pragma comment (lib, "winmm.lib")

#include <windows.h>
#include <stdio.h>
#include<math.h>
#include <stdlib.h>
#include <time.h>
#include "resource.h"
#include <mmsystem.h>//PlaySound, mciSendCommand
#include <Digitalv.h>//mciSendCommand

#define MasX 0			// ���� ��
#define MaxX 680			// ������ ��
#define MasY 0				// �� ��
#define MaxY 870			// �Ʒ� ��		(�ִ� �̵� ���� �����Դϴ�)

#define BULLETSPEED 20		// �Ѿ� �ӵ��Դϴ�
#define ENEMYBULLETSPEED 5	// �� �Ѿ� �ӵ��Դϴ�

#define COUNTERATTACKGAGE 1000		// �ݰݱ� ��Ÿ��

#define MAXGRIT 1200		// ��Ʈ W ��ȭ

MCI_PLAY_PARMS playBgm;
MCI_OPEN_PARMS openBgm;
int dwID;

int px = 300, py = 700;		// �÷��̾� ��ġ�Դϴ�
int php = 3;			// �÷��̾��� ü���Դϴ�
int SPEED = 7;				// �÷��̾� �̵� �ӵ� �Դϴ�

int bgy = 0;		// ��� ��ġ�Դϴ�

HANDLE hThrd1, hThrd2;

HWND hWndMain, hDlgMain;

HBITMAP hBit;

// ��Ʈ���� ���� �����߽��ϴ�
HBITMAP PlayerBit, BackgroundBit, BulletBit, PistolBit, RifleBit[6], MachineGunBit, BossBit[6], EnemyBulletBit, LifeBit, PlayerDamagedBit, BossLifeBit, BossBulletBit, HpPotionBit
		, StartBit, EndBit[4], InstructionBit;

HBITMAP HellMopBit[4], HellInstructionBit, HellBackgroundBit, HellBulletBit, HellEndBit[3], KingBit[2], KingCreateBit, KingAttackBit[4], KingBulletBit, KingCounterAttackBit
		, KingLifeBit, ItemBit[3], NewBulletBit[3], BoomBit, LevelBit[3], WarningBit, MopDieBit[7], SBulletBit[8], BoomBit2, QueenPlayerBit[2], QueenPlayerBulletBit, QueenBoomBit
		, SettBit, SettBulletBit, SettWBit, SettWEfectBit, SettLifeBit, SettEBit, SettMax[3], RevengeBit[5];

TCHAR str[128];			// ����ƽ ���� �ٲٱ⸦ ����


int Level = 0;				// ������ ���̵��Դϴ�

bool AllStop = true;		// ��ü���� ���� �����Դϴ�
bool stop = true;			// �÷��̾ ������ �����Դϴ�

int player_frm = 0;			// �÷��̾� �ִϸ��̼��� ������ �����Դϴ�
int pistol_frm = 0;			// ���� ��κ��� �ִϸ��̼��� ������ �����Դϴ�

int boss_frm = 0;			// �� �ִϸ��̼��� ������ �����Դϴ�

int king_frm = 0;			// ŷ �ִϸ��̼��� ������ �����Դϴ�
int king_bullet_frm = 0;	// ŷ �Ѿ� �ִϸ��̼�
int king_attack_frm = 0;	// ŷ �ݰݱ� �ִϸ��̼�
int boom_frm = 0;			// ��ź �ִϸ��̼�

int bulletcheck = 0;		// �Ѿ��� �ݺ��ؼ� ����ϱ����� ��������ϴ�
int enemybulletcheck = 0;	// �� �Ѿ��� �ݺ��ؼ� ����ϱ����� ��������ϴ�
int riflebulletcheck = 0;	// ���Ѻ� �Ѿ��� �ݺ��ؼ� ����ϱ����� ��������ϴ�
int bossbulletcheck = 0;	// ���� �Ѿ� �ݺ����Դϴ�

int hellbulletcheck = 0;	// ���� �Ѿ� �ݺ����Դϴ�

int pistolcheck = 0;		// ���Ѻ��� �ݺ��ؼ� ����ϱ����� ��������ϴ�
int riflecheck = 0;			// ����
int machineguncheck = 0;	// �����
int itemcheck = 0;			// ������


int efectcheck = 0;	// ����Ʈ üũ

int mopcheck = 0;	// ��

int stagecheck = 0;			// ������������ ������ ���� ��Ʈ���Ϸ��� ��������ϴ�
int hellstagecheck = 0;		// �� ���̵����� ������ �� ��Ʈ�ѿ�

bool delay = true;				// ��� ��ư�����Դϴ�	��ư�� �� ���������� �� �Ѿ��� ������ ������������ ��������ϴ�

bool damage = false;		// ���� �ǰ� ����

int page = 0;		// ����ȭ���̳� ������ ���� �� �����Դϴ�

int music = 0;		// ���� ����
bool musicOn = true;	// ���� ���� ����

bool showqueen = true;			// �� ������ ����

int vibe = 0;		// ����
int vibes = 10;		// ��Ʈw ����

bool kingcreate = true;		// ŷ �ѹ��� ��ȯ��

enum PlayerStyle { DEFAULT = 0, SGUN, HHGUN, HGUN};
DWORD style = DEFAULT;			// �� ��Ÿ��

int bulletindex = 0;		// ���� �Ѿ�

int boomindex = 1;			// ���� ��ź
bool boomcheck = false;			// ��ź ������
bool boomcheck2 = false;			// ��ź ������
int boom2 = 0;			// ������

bool showlevel = false;		// ���� ǥ��

bool showwarning = false;		// warning ǥ��

int point = 0;		// ����

bool QueenPlayer = false;		// �÷��̾ ������
bool QueenBoom = false;		// �� ��ź ��������

bool gritsound = true;		// ���� �Ҹ� ���°� ��������


bool realhell = false;		// �̻��� ���


struct Bullet			// �Ѿ�
{
	float x = 0, y = 0;					// ��ġ
	bool create = false;		// ���� Ȯ��
	float random = 2;
};

struct NewBullet			// ���Ӱ� ���� �� �Ҹ� ����ü
{
	float x = 0, y = 0;					// ��ġ
	bool create = false;		// ���� Ȯ��
	float sx = 0, sy = 0;		// �̵� ����
	HBITMAP bit = NULL;			// �Ѿ˻����
};

struct Pistol			// ���� �� ��
{
	int hp = 30;					// ü��
	int x = 0, y = 0;					// ��ġ
	bool create = false;		// ���� Ȯ��
	int dir = 0;				// �̵� ����
	int gage = 0;				// �� ��� ������ �������� ä��� �Ѿ��� ���ϴ�
};


struct Rifle			// ���� �� ��
{
	int hp = 20;					// ü��
	int x = 0, y = 0;					// ��ġ
	bool create = false;		// ���� Ȯ��
	bool hide = true;			// ���� �������� Ȯ��   ���� ���´� �Ѿ˿� �����ʰ��߽��ϴ�
	int gage = 0;				// �� ��� ������ �������� ä��� �Ѿ��� ���ϴ�
};

struct MachineGun		// ����� �� ��
{
	int hp = 80;					// ü��
	int x = 0, y = 0;					// ��ġ
	bool create = false;		// ���� Ȯ��
	int gage = 0;				// �� ��� ������ �������� ä��� �Ѿ��� ���ϴ�
	int dir = 0;				// �̵� ����
};

struct Boss				// ù��° ����
{
	int hp = 2000;				// ü��
	int x = 0, y = 0;					// ��ġ
	int state = 0;				// ����
	int stance = 0;				// �Ѿ� �������� ���� �� ����
	bool create = false;		// ���� Ȯ��
	int dir = 0;				// �̵� ����
	int gage = 0;			// ����1
	int gage2 = 0;			// ����2
	int gage3 = 0;			// ����3
};


struct Mop			// ���Ӱ� ���� �� �� ���� ����ü
{
	int hp = 0;					// ü��
	int x = 0, y = 0;			// ��ġ
	bool create = false;		// ���� Ȯ��
	int dir = 0;				// �̵� ����
	int gage = 0;				// �� ��� ������ �������� ä��� �Ѿ��� ���ϴ�
	int maxgage = 0;			// �������� �� ��ŭ �Ǹ� �߻�
	int type = 0;				// Ÿ��
};

struct King				// Hell ���̵� ���� ����
{
	int hp = 3000;				// ü��
	int x = 0, y = 0;					// ��ġ
	int state = 0;				// ����
	bool create = false;		// ���� Ȯ��
	int gage = 0;			// ����1
	int counter = 0;		// �ݰݰ�����
};

struct Efect				// ����Ʈ
{
	int x = 0, y = 0;					// ��ġ
	bool create = false;		// ���� Ȯ��
	HBITMAP bit = NULL;		// ��Ʈ�� ����
	int frm = 0;		// ������
	DWORD maxfrm = 123;		// �ִ� ������
};

struct Sett				// ��Ʈ ���� (Ư����)
{
	int hp = 3000;				// ü��
	int x = 0, y = 0;					// ��ġ
	int grit = 0;			// ��Ʈ ����
	bool create = false;		// ���� Ȯ��
	int gage = 0;			// ����1
	bool show = true;		// ��Ʈ ���̰� ��������
};

enum MOP {
	M_P = 0, M_C, M_S, M_T, KING1, KING2, KING3, KING4, KING5, QUEEN1, QUEEN2, QUEEN3, QUEEN4, QUEEN5
	, SETT1, SETT2, SETT3, SETT4, SETT5, SETT6, SETT7
};			// �� ������ ����

enum MUSIC { LOBBY = 0, INGAME, CLEAR, HELL };			// ���� ����


NewBullet bullet[100];				// �Ѿ� ����
Bullet enemybullet[100];			// �� �Ѿ� ����
Bullet riflebullet[5];			// ���Ѻ� �Ѿ� ����
Bullet bossbullet[30];			// ���� �Ѿ� ����

NewBullet hellbullet[100];		// ���Ӱ� ���� �� �Ѿ� (������ ���� ����)

Bullet HpPotion;			// ȸ���� ����		�Ѿ˰� ����� ����̶� class�� �״�� ����߽��ϴ�

Pistol pistol[30];			// ���Ѻ�
Rifle rifle[10];			// ���Ѻ�
MachineGun machinegun[10];	// ����Ѻ�
Boss boss;					// ����

King king;					// ŷ

Sett sett;					// ��Ʈ

Mop	mop[30];				// ���� �� ��

Efect efect[10];			// ����Ʈ

NewBullet item[5];			// �÷��̾ �� ������

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("Shoting Game");

// WINDOWS API
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;	//MAKEINTRESOURCE(IDR_MENU1);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPED | WS_SYSMENU,
		//CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		550, 25, 800, 1000,
		NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	hWndMain = hWnd;

	while (GetMessage(&Message, NULL, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}


// �Լ� ����
void UseSettW();
void SettCreate();
void SettControl(HDC hMemDC);
void Music(int m);
void SBullet(int m);
void MopClear(int m);
void UseBoom();
void DrawBoomIndex(HDC hMemDC);
void DrawPoint(HDC hMemDC);
void DrawBulletIndex(HDC hMemDC);
void Sshot();
void HHshot();
void Hshot();
void DefaultShot();
void ItemCreate(int x, int y, HBITMAP bit);
void QueenSafe(HDC hMemDC);
void QueenControl(HDC hMemDC);
void KingMove(int x, int y);
void EfectControl(HDC hMemDC);
void EfectCreate(int x, int y, HBITMAP hbit, int maxfrm);
void CounterAttack(HDC hMemDC);
void KingControl(HDC hMemDC);
void KingCreate();
void Reset();
void OnBGM(int num);
void HellBulletCheckControl();
void HellBulletCreate(int type, int i);
void MopShot();
void HellMopControl(HDC hMemDC);
void HellMopCreate(int x, int type, int dir);
void HellBulletControl(HDC hMemDC);


#pragma region Hell ���� �Լ���

void HpPotionCreate(int x);
void HpPotionControl(HDC hMemDC);
void BossBulletControl3(HDC hMemDC);
void BossBulletControl2(HDC hMemDC);
void BossBulletControl(HDC hMemDC);
void BossShot();
void BossShoting();
void BossShot2();
void BossShot3();
void BossMoveTransBlt(HDC hdc, int x, int y, HBITMAP hbitmap, COLORREF clrMask, int frm);
void BossLifeBlt(HDC hdc, int x, int y, HBITMAP hbitmap, COLORREF clrMask);
void BossControl(HDC hMemDC);
void BossCreate();
void LifeBlt(HDC hdc, int x, int y, HBITMAP hbitmap, COLORREF clrMask, int hp);
void EnemyShot();
DWORD CALLBACK EnemyDamaged(LPVOID n);
DWORD CALLBACK Damaged(LPVOID n);
BOOL CheckPtInRect(HBITMAP hBit, int Obitx, int Obity, HBITMAP Pbit, int Pbitx, int Pbity);
void PistolCreate(int x, int dir = 0);
void RifleCreate(int x);
void MachineGunCreate(int x, int dir = 0);
void Stage1();
void Stage2();
void HellStage();
void EnemyBulletControl(HDC hMemDC);
void PistolControl(HDC hMemDC);
void RifleControl(HDC hMemDC);
void MachineGunControl(HDC hMemDC);
void BackgroundBlt(HDC hdc, int x, int y, HBITMAP hBit);
void TransBlt(HDC hdc, int x, int y, HBITMAP hbitmap, COLORREF clrMask);
void MoveTransBlt(HDC hdc, int x, int y, HBITMAP hbitmap, COLORREF clrMask, int frm);
void DrawBitmap(HDC hdc, int x, int y, HBITMAP hBit);
void OnTimer();
void Create();

#pragma endregion

// ��Ʈ W ���
void UseSettW()
{
	HellBulletCreate(SETT3, 0);
	SetTimer(hWndMain, 10, 960, NULL);
}

// ��Ʈ ��ȯ
void SettCreate()
{
	sett.create = true;		// ��ȯ�ϸ� �ʱ�ȭ �۾��Դϴ�
	sett.gage = 0;
	sett.grit = 0;
	sett.hp = 3000;
	sett.x = 325;
	sett.y = 120;
}

// ��Ʈ ��Ʈ��
void SettControl(HDC hMemDC)
{
	if (sett.create)
	{
		sett.gage++;
		
		if (sett.hp >= 2000)
		{
			switch (sett.gage)
			{
			case 100:
			case 150:

			case 600:
			case 700:
			case 800:

			case 900:
				HellBulletCreate(SETT1, 0);
				break;
			case 250:

			case 630:
				HellBulletCreate(SETT2, 0);
				break;
			case 300:
			case 320:
			case 340:
			case 360:
			case 380:

			case 660:
			case 870:

			case 950:
				HellBulletCreate(SETT6, 0);
				break;
			case 450:
			case 490:
				HellBulletCreate(SETT4, 0);
				break;
			case 470:
			case 510:

			case 850:
				HellBulletCreate(SETT5, 0);
				break;
			}
		}
		else if (sett.hp >= 1000)
		{
			switch (sett.gage)
			{
			case 100:
			case 950:
				HellBulletCreate(SETT2, 0);
				break;

			case 150:
			case 160:
			case 170:
			case 180:
			case 190:
			case 200:

			case 810:
			case 890:
				HellBulletCreate(SETT1, 0);
				break;
			case 155:
			case 165:
			case 175:
			case 185:
			case 195:

			case 250:
			case 300:
			case 350:
			case 400:
				HellBulletCreate(SETT6, 0);
				break;
			case 500:
			case 600:
			case 700:
			case 920:
				HellBulletCreate(SETT4, 0);
				break;
			case 550:
			case 650:
			case 750:
			case 800:
			case 921:
				HellBulletCreate(SETT5, 0);
				break;
			}
		}
		else if (sett.hp >= 0)
		{
			switch (sett.gage)
			{
			case 100:
			case 150:
			case 200:
			case 850:
			case 875:
			case 900:
			case 925:
				HellBulletCreate(SETT6, 0);
				break;
			case 300:
			case 650:
			case 950:
				HellBulletCreate(SETT2, 0);
				break;
			case 350:
			case 700:
				if (sett.grit > MAXGRIT)
				{
					HellBulletCreate(SETT7, 0);
				}
				UseSettW();
				break;
			case 400:
			case 450:
			case 500:
			case 550:
				HellBulletCreate(SETT4, 0);
				break;
			case 425:
			case 475:
			case 525:
			case 575:
				HellBulletCreate(SETT5, 0);
				break;
			}
		}
		
		if (sett.gage == 1000)
		{
			if (sett.grit > MAXGRIT)
			{
				HellBulletCreate(SETT7, 0);
			}
			UseSettW();
		}

		if (sett.show)
		{
			if (sett.grit > MAXGRIT)
			{
				TransBlt(hMemDC, sett.x, sett.y, SettMax[0], RGB(76, 76, 76));
			}
			else
			{
				TransBlt(hMemDC, sett.x, sett.y, SettBit, RGB(76, 76, 76));
			}
		}

		if (gritsound && sett.grit > MAXGRIT)
		{
			gritsound = false;
			Music(4);
		}

		if (sett.gage == 1020)
		{
			sett.gage = 0;
		}

		if (sett.hp <= 0)			// ü���� 0 ���ϰ� �� ��� ����
		{
			sett.create = false;
			OnBGM(CLEAR);
			AllStop = true;
			page = 3;
		}
		BossLifeBlt(hMemDC, 100, 55, SettLifeBit, RGB(255, 255, 255));		// ���� ü�� ��Ʈ��
	}
}

// ȿ����
void Music(int m)
{
	switch (m)
	{
	case 1:
		PlaySound("SettE.wav", NULL, SND_ASYNC);
		break;
	case 3:
		if (QueenPlayer)
		{
			PlaySound("QueenHit.wav", NULL, SND_ASYNC);
		}
		else
		{
			PlaySound("Hit.wav", NULL, SND_ASYNC);
		}
		break;
	case 4:
		PlaySound("Hit.wav", NULL, SND_ASYNC);
		break;
	}
}

// S�Ѿ� �����
void SBullet(int m)
{
	switch (m)
	{
	case -10:
	case -9:
	case -8:
	case -7:
	case -6:
	case -5:
		bullet[bulletcheck].bit = SBulletBit[6];
		break;
	case -4:
		bullet[bulletcheck].bit = SBulletBit[6];
		break;
	case -3:
		bullet[bulletcheck].bit = SBulletBit[5];
		break;
	case -2:
		bullet[bulletcheck].bit = SBulletBit[4];
		break;
	case -1:
		bullet[bulletcheck].bit = SBulletBit[4];
		break;
	case 0:
		bullet[bulletcheck].bit = SBulletBit[3];
		break;
	case 1:
		bullet[bulletcheck].bit = SBulletBit[2];
		break;
	case 2:
		bullet[bulletcheck].bit = SBulletBit[2];
		break;
	case 3:
		bullet[bulletcheck].bit = SBulletBit[1];
		break;
	case 4:
		bullet[bulletcheck].bit = SBulletBit[0];
		break;
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
	case 10:
		bullet[bulletcheck].bit = SBulletBit[0];
		break;
	}
}

// ��� ���� �Ѿ� ���� (��ź ��� ��)
void MopClear(int m)
{
	// �� �Ѿ�
	for (int i = 0; i < 100; i++)
	{
		enemybullet[i].create = false;
	}
	// ���Ѻ� �Ѿ�
	for (int i = 0; i < 5; i++)
	{
		riflebullet[i].create = false;
	}
	// ����1 �Ѿ�
	for (int i = 0; i < 30; i++)
	{
		bossbullet[i].create = false;
	}
	// �� �Ѿ�
	for (int i = 0; i < 100; i++)
	{
		hellbullet[i].create = false;
	}


	// ���Ѻ�
	for (int i = 0; i < 30; i++)
	{
		pistol[i].hp -= m;
	}
	// ���Ѻ�
	for (int i = 0; i < 10; i++)
	{
		rifle[i].hp -= m;
	}
	// ����Ѻ�
	for (int i = 0; i < 10; i++)
	{
		machinegun[i].hp -= m;
	}
	for (int i = 0; i < 30; i++)
	{
		mop[i].hp -= m;
	}
}

// ��ź ���
void UseBoom()
{
	if (QueenPlayer)
	{
		if (boomindex > 0)
		{
			boomindex--;
			QueenBoom = true;
			damage = true;
			KillTimer(hWndMain, 3);
			SetTimer(hWndMain, 7, 3000, NULL);
		}
	}
	else
	{
		if (boomindex >= 3)
		{
			boomindex -= 3;
			boom2 = 0;
			boomcheck2 = true;
			EfectCreate(-10, 0, BoomBit2, 16);
			damage = true;
			SetTimer(hWndMain, 3, 2000, NULL);
			PlaySound("Boom.wav", NULL, SND_ASYNC);
		}
		else if (boomindex > 0 && !boomcheck)
		{
			boomindex--;
			boomcheck = true;
		}
	}
}

// ���� ��ź �׸���
void DrawBoomIndex(HDC hMemDC)
{
	HFONT font, oldfont;
	RECT rt;
	TCHAR str[30];

	SetRect(&rt, 650, 920, 800, 1000);

	wsprintf(str, "Boom : %d", boomindex);

	SetBkMode(hMemDC, TRANSPARENT);
	font = CreateFont(30, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 3, 2, 1,
		VARIABLE_PITCH | FF_ROMAN, "���� ���");
	oldfont = (HFONT)SelectObject(hMemDC, font);

	SetTextColor(hMemDC, RGB(255, 255, 255));		// ���� ��
	DrawText(hMemDC, str, -1, &rt, DT_WORDBREAK);
	DeleteObject(SelectObject(hMemDC, oldfont));
}

// ���� �׸���
void DrawPoint(HDC hMemDC)
{
	HFONT font, oldfont;
	RECT rt;
	TCHAR str[30];
	int size = 30;

	if (page == 3)
	{
		SetRect(&rt, 220, 450, 700, 600);
		size = 72;
	}
	else
	{
		SetRect(&rt, 15, 15, 300, 100);
		size = 30;
	}
	wsprintf(str, "Point : %d", point);

	SetBkMode(hMemDC, TRANSPARENT);
	font = CreateFont(size, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 3, 2, 1,
		VARIABLE_PITCH | FF_ROMAN, "���� ���");
	oldfont = (HFONT)SelectObject(hMemDC, font);

	SetTextColor(hMemDC, RGB(255, 255, 255));		// ���� ��
	DrawText(hMemDC, str, -1, &rt, DT_WORDBREAK);
	DeleteObject(SelectObject(hMemDC, oldfont));
}

// ���� �Ѿ� �׸���
void DrawBulletIndex(HDC hMemDC)
{
	HFONT font, oldfont;
	RECT rt;
	TCHAR str[30];

	SetRect(&rt, 650, 900, 800, 1000);

	wsprintf(str, "Bullet : %d", bulletindex);

	SetBkMode(hMemDC, TRANSPARENT);
	font = CreateFont(30, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 3, 2, 1,
		VARIABLE_PITCH | FF_ROMAN, "���� ���");
	oldfont = (HFONT)SelectObject(hMemDC, font);

	SetTextColor(hMemDC, RGB(255, 255, 255));		// ���� ��
	DrawText(hMemDC, str, -1, &rt, DT_WORDBREAK);
	DeleteObject(SelectObject(hMemDC, oldfont));
}

// �� �� ���
void Sshot()
{
	if (QueenPlayer)
	{
		if (QueenBoom)
		{
			for (int i = 0; i < 3; i++)
			{
				int rnd = rand() % 20 - 10;
				bullet[bulletcheck].create = true;				// �Ѿ��� �����ϰ� �߽��ϴ�
				bullet[bulletcheck].x = px + 38;			// ó���� �߻� �Ǵ� �������� �÷��̾� ��ġ�� ������ϴ�
				bullet[bulletcheck].y = py - 5;
				bullet[bulletcheck].sx = rnd;
				bullet[bulletcheck].sy = BULLETSPEED;
				SBullet(bullet[bulletcheck].sx);
				if (bulletcheck != 99)
				{
					bulletcheck++;
				}
				else
				{
					bulletcheck = 0;
				}
			}
		}
		else
		{
			for (int i = 0; i < 20; i++)
			{
				bullet[bulletcheck].create = true;
				bullet[bulletcheck].x = px + 72;
				bullet[bulletcheck].y = py - 65;
				bullet[bulletcheck].sx = -10 + i;
				bullet[bulletcheck].sy = 20;
				SBullet(bullet[bulletcheck].sx);
				if (bulletcheck != 99)
				{
					bulletcheck++;
				}
				else
				{
					bulletcheck = 0;
				}
			}
			for (int i = 0; i < 10; i++)
			{
				bullet[bulletcheck].create = true;
				bullet[bulletcheck].x = px + 72;
				bullet[bulletcheck].y = py - 45;
				bullet[bulletcheck].sx = -5 + i;
				bullet[bulletcheck].sy = 20;
				SBullet(bullet[bulletcheck].sx);
				if (bulletcheck != 99)
				{
					bulletcheck++;
				}
				else
				{
					bulletcheck = 0;
				}
			}
			for (int i = 0; i < 8; i++)
			{
				bullet[bulletcheck].create = true;
				bullet[bulletcheck].x = px + 72;
				bullet[bulletcheck].y = py - 25;
				bullet[bulletcheck].sx = -4 + i;
				bullet[bulletcheck].sy = 20;
				SBullet(bullet[bulletcheck].sx);
				if (bulletcheck != 99)
				{
					bulletcheck++;
				}
				else
				{
					bulletcheck = 0;
				}
			}
			for (int i = 0; i < 6; i++)
			{
				bullet[bulletcheck].create = true;
				bullet[bulletcheck].x = px + 72;
				bullet[bulletcheck].y = py - 5;
				bullet[bulletcheck].sx = -3 + i;
				bullet[bulletcheck].sy = 20;
				SBullet(bullet[bulletcheck].sx);
				if (bulletcheck != 99)
				{
					bulletcheck++;
				}
				else
				{
					bulletcheck = 0;
				}
			}
		}
	}
	else
	{
		for (int i = 0; i < 10; i++)
		{
			bullet[bulletcheck].create = true;
			bullet[bulletcheck].x = px + 72;
			bullet[bulletcheck].y = py - 45;
			bullet[bulletcheck].sx = -5 + i;
			bullet[bulletcheck].sy = 20;
			SBullet(bullet[bulletcheck].sx);
			if (bulletcheck != 99)
			{
				bulletcheck++;
			}
			else
			{
				bulletcheck = 0;
			}
		}
		for (int i = 0; i < 8; i++)
		{
			bullet[bulletcheck].create = true;
			bullet[bulletcheck].x = px + 72;
			bullet[bulletcheck].y = py - 25;
			bullet[bulletcheck].sx = -4 + i;
			bullet[bulletcheck].sy = 20;
			SBullet(bullet[bulletcheck].sx);
			if (bulletcheck != 99)
			{
				bulletcheck++;
			}
			else
			{
				bulletcheck = 0;
			}
		}
		for (int i = 0; i < 6; i++)
		{
			bullet[bulletcheck].create = true;
			bullet[bulletcheck].x = px + 72;
			bullet[bulletcheck].y = py - 5;
			bullet[bulletcheck].sx = -3 + i;
			bullet[bulletcheck].sy = 20;
			SBullet(bullet[bulletcheck].sx);
			if (bulletcheck != 99)
			{
				bulletcheck++;
			}
			else
			{
				bulletcheck = 0;
			}
		}

		if (bulletcheck != 99)
		{
			bulletcheck++;
		}
		else
		{
			bulletcheck = 0;
		}
	}
}
void HHshot()
{
	if (QueenPlayer)
	{
		if (QueenBoom)
		{
			for (int i = 0; i < 3; i++)
			{
				int rnd = rand() % 20 - 10;
				bullet[bulletcheck].create = true;				// �Ѿ��� �����ϰ� �߽��ϴ�
				bullet[bulletcheck].x = px + 38;			// ó���� �߻� �Ǵ� �������� �÷��̾� ��ġ�� ������ϴ�
				bullet[bulletcheck].y = py - 5;
				bullet[bulletcheck].sx = rnd;
				bullet[bulletcheck].sy = BULLETSPEED;
				bullet[bulletcheck].bit = NewBulletBit[1];
				if (bulletcheck != 99)
				{
					bulletcheck++;
				}
				else
				{
					bulletcheck = 0;
				}
			}
		}
		else
		{
			for (int i = 0; i < 4; i++)
			{
				bullet[bulletcheck].create = true;
				bullet[bulletcheck].x = px + 72;
				bullet[bulletcheck].y = py - 35;
				bullet[bulletcheck].sx = -3 + i * 2;
				bullet[bulletcheck].sy = BULLETSPEED;
				bullet[bulletcheck].bit = NewBulletBit[1];
				if (bulletcheck != 99)
				{
					bulletcheck++;
				}
				else
				{
					bulletcheck = 0;
				}
			}
			for (int i = 0; i < 3; i++)
			{
				bullet[bulletcheck].create = true;
				bullet[bulletcheck].x = px + 72;
				bullet[bulletcheck].y = py - 5;
				bullet[bulletcheck].sx = -2 + i * 2;
				bullet[bulletcheck].sy = BULLETSPEED;
				bullet[bulletcheck].bit = NewBulletBit[1];
				if (bulletcheck != 99)
				{
					bulletcheck++;
				}
				else
				{
					bulletcheck = 0;
				}
			}
		}
	}
	else
	{
		for (int i = 0; i < 3; i++)
		{
			bullet[bulletcheck].create = true;
			bullet[bulletcheck].x = px + 72;
			bullet[bulletcheck].y = py - 5;
			bullet[bulletcheck].sx = -2 + i * 2;
			bullet[bulletcheck].sy = BULLETSPEED;
			bullet[bulletcheck].bit = NewBulletBit[1];
			if (bulletcheck != 99)
			{
				bulletcheck++;
			}
			else
			{
				bulletcheck = 0;
			}
		}
	}
}
void Hshot()
{
	if (QueenPlayer)
	{
		if (QueenBoom)
		{
			for (int i = 0; i < 3; i++)
			{
				int rnd = rand() % 20 - 10;
				bullet[bulletcheck].create = true;				// �Ѿ��� �����ϰ� �߽��ϴ�
				bullet[bulletcheck].x = px + 38;			// ó���� �߻� �Ǵ� �������� �÷��̾� ��ġ�� ������ϴ�
				bullet[bulletcheck].y = py - 5;
				bullet[bulletcheck].sx = rnd;
				bullet[bulletcheck].sy = BULLETSPEED;
				bullet[bulletcheck].bit = NewBulletBit[2];
				if (bulletcheck != 99)
				{
					bulletcheck++;
				}
				else
				{
					bulletcheck = 0;
				}
			}
		}
		else
		{
			for (int i = 0; i < 4; i++)
			{
				int random = rand() % 12 - 6;
				bullet[bulletcheck].create = true;
				bullet[bulletcheck].x = px + 72;
				bullet[bulletcheck].y = py - 5;
				bullet[bulletcheck].sx = random;
				bullet[bulletcheck].sy = BULLETSPEED;
				bullet[bulletcheck].bit = NewBulletBit[2];
				if (bulletcheck != 99)
				{
					bulletcheck++;
				}
				else
				{
					bulletcheck = 0;
				}
			}
		}
	}
	else
	{
		for (int i = 0; i < 2; i++)
		{
			float random = float(rand() % 10) - 5;
			bullet[bulletcheck].create = true;
			bullet[bulletcheck].x = px + 72;
			bullet[bulletcheck].y = py - 5;
			bullet[bulletcheck].sx = random;
			bullet[bulletcheck].sy = BULLETSPEED;
			bullet[bulletcheck].bit = NewBulletBit[2];
			if (bulletcheck != 99)
			{
				bulletcheck++;
			}
			else
			{
				bulletcheck = 0;
			}
		}
	}
}
void DefaultShot()
{
	if (QueenPlayer)
	{
		if (QueenBoom)
		{
			for (int i = 0; i < 3; i++)
			{
				int rnd = rand() % 20 - 10;
				bullet[bulletcheck].create = true;				// �Ѿ��� �����ϰ� �߽��ϴ�
				bullet[bulletcheck].x = px + 38;			// ó���� �߻� �Ǵ� �������� �÷��̾� ��ġ�� ������ϴ�
				bullet[bulletcheck].y = py - 5;
				bullet[bulletcheck].sx = rnd;
				bullet[bulletcheck].sy = BULLETSPEED;
				bullet[bulletcheck].bit = QueenPlayerBulletBit;
				if (bulletcheck != 99)
				{
					bulletcheck++;
				}
				else
				{
					bulletcheck = 0;
				}
			}
		}
		else
		{
			for (int i = 0; i < 2; i++)
			{
				bullet[bulletcheck].create = true;				// �Ѿ��� �����ϰ� �߽��ϴ�
				bullet[bulletcheck].x = px + 38 + (i * 20);			// ó���� �߻� �Ǵ� �������� �÷��̾� ��ġ�� ������ϴ�
				bullet[bulletcheck].y = py - 5;
				bullet[bulletcheck].sx = 0;
				bullet[bulletcheck].sy = BULLETSPEED;
				bullet[bulletcheck].bit = QueenPlayerBulletBit;
				if (bulletcheck != 99)
				{
					bulletcheck++;
				}
				else
				{
					bulletcheck = 0;
				}
			}
		}
	}
	else
	{
		bullet[bulletcheck].create = true;				// �Ѿ��� �����ϰ� �߽��ϴ�
		bullet[bulletcheck].x = px + 72;			// ó���� �߻� �Ǵ� �������� �÷��̾� ��ġ�� ������ϴ�
		bullet[bulletcheck].y = py - 5;
		bullet[bulletcheck].sx = 0;
		bullet[bulletcheck].sy = BULLETSPEED;
		bullet[bulletcheck].bit = BulletBit;
		if (bulletcheck != 99)
		{
			bulletcheck++;
		}
		else
		{
			bulletcheck = 0;
		}
	}
}

// ������ ����
void ItemCreate(int x, int y, HBITMAP bit)
{
	item[itemcheck].x = x;
	item[itemcheck].y = y;
	item[itemcheck].sy = 2;
	item[itemcheck].create = true;
	item[itemcheck].bit = bit;
	if (itemcheck != 4)
	{
		itemcheck++;
	}
	else
	{
		itemcheck = 0;
	}
}

// ŷ�� �� ��ȣ����	������
void QueenSafe(HDC hMemDC)
{
	if (boss.create)
	{
		for (int i = 0; i < 100; i++)
		{
			if (bullet[i].create)
			{
				if (bullet[i].x >= boss.x + 10 && bullet[i].x <= boss.x + 140)
				{
					if (bullet[i].y < 240 && bullet [i].y > 50)
					{
						for (int i = 0; i < 100; i++)
						{
							bullet[i].create = false;
						}
						KingMove(boss.x, king.y);
						king.counter = COUNTERATTACKGAGE + 100;
					}
				}
			}
		}
	}
}

// �� ��Ʈ��
void QueenControl(HDC hMemDC)
{
	if (boss.create)
	{
		/*
		if (king.create)
		{
			boss.gage++;
			if (boss.gage % 50 == 0)		// ���� 1
			{
				EfectCreate(boss.x, boss.y, BossBit[1], 4);
				HellBulletCreate(QUEEN3, 0);
			}
			if (boss.dir == 0 && showqueen)
			{
				boss.x += 3;
			}
			if (boss.dir == 1 && showqueen)
			{
				boss.x -= 3;
			}
		}
		*/
		if (boss.hp >= 500)
		{
			boss.gage++;
			if (boss.gage == 100 || boss.gage == 300 || boss.gage == 650 || boss.gage == 900)		// ���� 1
			{
				EfectCreate(boss.x, boss.y, BossBit[1], 4);
				HellBulletCreate(QUEEN1, 0);
			}
			if (boss.gage == 250 || boss.gage == 600)				// ���� 2
			{
				EfectCreate(boss.x, boss.y, BossBit[1], 4);
				HellBulletCreate(QUEEN2, 0);
			}

			if (boss.gage % 400 == 0 && !realhell)
			{
				HellBulletCreate(QUEEN5, 0);
			}

			if (boss.hp >= 1200)		// ü���� ���� ��� ���� ����
			{
				if (boss.gage == 400 || boss.gage == 700)
				{
					boss.state = 3;
				}
				if ((boss.gage >= 400 && boss.gage <= 550) || (boss.gage >= 700 && boss.gage <= 750))				// ���� 3
				{
					if (boss.gage % 5 == 0)
					{
						HellBulletCreate(QUEEN3, 0);
					}
				}
			}
			else
			{
				if (boss.gage == 400 || boss.gage == 700)
				{
					boss.state = 4;
				}
				if (boss.dir == 0 && showqueen)
				{
					boss.x += 10;
				}
				if (boss.dir == 1 && showqueen)
				{
					boss.x -= 10;
				}
				if ((boss.gage >= 400 && boss.gage <= 550) || (boss.gage >= 700 && boss.gage <= 750))				// ���� 3
				{
					if (boss.gage % 7 == 0)
					{
						HellBulletCreate(QUEEN3, 0);
					}
				}
			}

			if (boss.gage == 551 || boss.gage == 751)
			{
				boss.state = 0;
			}

			if (boss.gage == 1000)
			{
				boss.gage = 0;
			}
		}
		else
		{
			boss.gage++;

			if (boss.gage % 400 == 0)
			{
				HellBulletCreate(QUEEN5, 0);
			}

			if (boss.gage <= 2000)
			{
				boss.gage = 2000;
			}

			if (boss.gage == 2020 && boss.state != 2)
			{
				boss.state = 2;
			}

			if (boss.gage >= 2120)
			{
				if (boss.dir == 0)
				{
					boss.x += 5;
				}
				if (boss.dir == 1)
				{
					boss.x -= 5;
				}

				if (boss.gage % 5 == 0)
				{
					int ram = rand() % 10 - 5;
					int rax = rand() % 10 - 5;
					HellBulletCreate(QUEEN4, 0);
					SetWindowPos(hWndMain, NULL, 550 + ram, 25 + rax, 800, 1000, 0);
				}
			}
		}
		
		if (showqueen)
		{
			BossMoveTransBlt(hMemDC, boss.x, boss.y, BossBit[boss.state], RGB(255, 255, 255), boss_frm);
		}

		if (boss.hp <= 0)			// ü���� 0 ���ϰ� �� ��� ����
		{
			boss.create = false;
			AllStop = true;
			page = 3;
			OnBGM(CLEAR);
		}
		BossLifeBlt(hMemDC, 100, 30, BossLifeBit, RGB(255, 255, 255));		// ���� ü�� ��Ʈ��
	}
	if (!boss.create)
	{
		boss.hp = 2000;
	}
}

// ŷ �̵�
void KingMove(int x, int y)
{
	king.x = x;
	king.y = y;
	king.counter = COUNTERATTACKGAGE;
	EfectCreate(king.x, king.y, KingCreateBit, 4);
}

// ����Ʈ ��Ʈ��
void EfectControl(HDC hMemDC)
{
	for (int i = 0; i < 10; i++)
	{
		if (efect[i].create)
		{
			if (efect[i].frm == efect[i].maxfrm)
			{
				if (!showqueen && efect[i].bit == BossBit[1])
				{
					showqueen = true;
				}
				efect[i].create = false;
			}

			MoveTransBlt(hMemDC, efect[i].x, efect[i].y, efect[i].bit, RGB(76, 76, 76), efect[i].frm);
		}
	}
}

// ����Ʈ ����
void EfectCreate(int x, int y, HBITMAP hbit, int maxfrm)
{
	if (hbit == BossBit[1])
	{
		showqueen = false;
	}
	efect[efectcheck].x = x;
	efect[efectcheck].y = y;
	efect[efectcheck].bit = hbit;
	efect[efectcheck].create = true;
	efect[efectcheck].frm = 0;
	efect[efectcheck].maxfrm = maxfrm;
	if (efectcheck != 9)
	{
		efectcheck++;
	}
	else
	{
		efectcheck = 0;
	}
}

// ŷ �ݰݱ� ����Ʈ
void CounterAttack(HDC hMemDC)
{
	if (!realhell)
	{
		py = 850;
	}
	MoveTransBlt(hMemDC, 0, 0, KingCounterAttackBit, RGB(255, 255, 255), king_attack_frm);
	/*for (int i = 0; i < 100; i++)		// �÷��̾� �Ѿ� ���ֱ�
	{
		bullet[i].create = false;
	}*/
	switch (vibe)
	{
	case 0:
		SetWindowPos(hWndMain, NULL, 580, 5, 800, 1000, 0);
		break;
	case 1:
		SetWindowPos(hWndMain, NULL, 540, 35, 800, 1000, 0);
		break;
	case 2:
		if (king.hp >= 150)
		{
			HellBulletCreate(KING3, 0);
		}
		SetWindowPos(hWndMain, NULL, 550, 25, 800, 1000, 0);
		break;
	}
	vibe++;
	if (king_attack_frm == 4)
	{
		vibe = 0;
		king.state = 0;
	}
}

// ŷ ��Ʈ��
void KingControl(HDC hMemDC)
{
	if (king.create)
	{
		if (king.hp >= 1500)
		{
			king.gage++;
			if (king.gage == 300 || king.gage == 720 || king.gage == 780)			// ���� 2
			{
				HellBulletCreate(KING2, 0);
				EfectCreate(king.x, king.y, KingAttackBit[0], 5);
			}
			if (king.gage == 320 || king.gage == 750)			// ���� 1
			{
				HellBulletCreate(KING1, 0);
				EfectCreate(king.x, king.y, KingAttackBit[1], 5);
			}
			if (king.gage == 400 || king.gage == 500 || king.gage == 600)
			{
				HellBulletCreate(KING4, 0);
				EfectCreate(king.x, king.y, KingAttackBit[2], 5);
			}
			if (king.gage == 450 || king.gage == 550 || king.gage == 650)
			{
				HellBulletCreate(KING5, 0);
				EfectCreate(king.x, king.y, KingAttackBit[3], 5);
			}

			if (king.gage == 780)		// ���� �ݺ�
			{
				king.gage = 0;
			}
		}
		else if (king.hp >= 150)
		{
			int random = rand() % 400 + 150;
			king.gage++;
			if (king.gage % 200 == 0)
			{
				KingMove(random, king.y);
				EfectCreate(king.x, king.y, KingAttackBit[0], 5);
				HellBulletCreate(KING2, 0);
			}
			if (king.gage == 320 || king.gage == 700)
			{
				KingMove(random, king.y);
				HellBulletCreate(KING4, 0);
				EfectCreate(king.x, king.y, KingAttackBit[2], 5);
			}

			if (king.gage == 1000)		// ���� �ݺ�
			{
				king.gage = 0;
			}
		}
		else
		{
			int random = rand() % 400 + 150;
			king.gage++;
			if (king.gage % 50 == 0)
			{
				KingMove(random, king.y);
				HellBulletCreate(KING5, 0);
				EfectCreate(king.x, king.y, KingAttackBit[1], 5);
			}

			if (king.gage == 1000)		// ���� �ݺ�
			{
				king.gage = 0;
			}
		}

		//QueenSafe(hMemDC);		// �� ��Ű�� ����

		if (king.counter < COUNTERATTACKGAGE)
		{
			king.counter++;
		}
		if (king.counter == COUNTERATTACKGAGE)
		{
			king.counter = 0;
			king_frm = 0;
			king.state = 1;
		}

		switch (king.state)
		{
		case 0:
			MoveTransBlt(hMemDC, king.x, king.y, KingBit[0], RGB(255, 255, 255), king_frm);
			break;
		case 1:		// �ݰ��ڼ�
			MoveTransBlt(hMemDC, king.x, king.y, KingBit[1], RGB(255, 255, 255), king_frm);
			break;
		}

		if (king_frm >= 4 && king.state == 1)
		{
			if (king_attack_frm >= 5)
			{
				king_attack_frm = 0;
			}
			CounterAttack(hMemDC);
		}

		if (king.hp <= 0)			// ü���� 0 ���ϰ� �� ��� ����
		{
			king.create = false;
			Level++;
		}
		BossLifeBlt(hMemDC, 100, 40, KingLifeBit, RGB(255, 255, 255));		// ���� ü�� ��Ʈ��
	}
}

// ŷ ��ȯ
void KingCreate()
{
	king.create = true;		// ��ȯ�ϸ� �ʱ�ȭ �۾��Դϴ�
	king.hp = 3000;
	king.gage = 0;
	king.x = 325;
	king.y = 120;
	king.counter = COUNTERATTACKGAGE;
	EfectCreate(king.x, king.y, KingCreateBit, 4);
}

// �ʱ�ȭ �۾�
void Reset()
{
	for (int i = 0; i < 30; i++)
	{
		pistol[i].create = false;		// ���� ���ְ�
	}
	for (int i = 0; i < 10; i++)	// ������ ó������ �ϱ����� �ʱ�ȭ �۾��Դϴ�
	{
		rifle[i].create = false;
		machinegun[i].create = false;
	}
	for (int i = 0; i < 100; i++)		// �Ѿ˵� ���ְ�
	{
		enemybullet[i].create = false;
	}
	for (int i = 0; i < 30; i++)
	{
		bossbullet[i].create = false;
	}
	for (int i = 0; i < 5; i++)
	{
		riflebullet[i].create = false;
	}
	for (int i = 0; i < 30; i++)
	{
		mop[i].create = false;
	}
	for (int i = 0; i < 100; i++)
	{
		hellbullet[i].create = false;
	}
	HpPotion.create = false;			// ȸ���൵ ���ְ�
	boss.create = false;			// ������ ���ְ�
	king.create = false;			// ������ ���ְ�
	sett.create = false;			
	stagecheck = 0;					// stage�� ó������ �����ϰ�
	hellstagecheck = 0;					// stage�� ó������ �����ϰ�
	px = 300;			// �÷��̾� ��ġ�� �ʱ�ȭ
	py = 700;
	php = 3;				// �÷��̾� ü�µ� 3���� ä������ϴ�
	boomindex = 1;			// ��ź 1���� �ʱ�ȭ
	bulletindex = 0;		// �Ѿ� �ʱ�ȭ
}

// BGM
void OnBGM(int num)
{
	if (!musicOn)
	{
		return;
	}

	mciSendCommand(dwID, MCI_CLOSE, MCI_NOTIFY, (DWORD)(LPVOID)&openBgm); // �Ͻ����� PAUSE, ���� ���� CLOSE

	openBgm.lpstrDeviceType = "mpegvideo";// wave , "mpegvideo"; //mp3 ����
	switch(num)
	{
	case LOBBY:
		if (QueenPlayer)
		{
			openBgm.lpstrElementName = "BGM.mp3";
		}
		else
		{
			openBgm.lpstrElementName = "Lobby.mp3";
		}
		break;
	case INGAME:
		openBgm.lpstrElementName = "InGame.mp3";
		break;
	case CLEAR:
		openBgm.lpstrElementName = "Clear.mp3";
		break;
	}
	mciSendCommand(0, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (DWORD)(LPVOID)&openBgm);
	dwID = openBgm.wDeviceID;
	mciSendCommand(dwID, MCI_PLAY, MCI_DGV_PLAY_REPEAT, (DWORD)(LPVOID)&openBgm);
}

// �� �Ҹ� check ��Ʈ��
void HellBulletCheckControl()
{
	if (hellbulletcheck != 99)							// �ִ� �Ѿ� ������ �����ϸ� 0���� �ǵ��Ƚ��ϴ�
	{
		hellbulletcheck++;
	}
	else
	{
		hellbulletcheck = 0;
	}
}

// �� �Ѿ� ����
void HellBulletCreate(int type, int _i)
{
	int random;
	int rnd = rand() % 2;

	switch (type)
	{
	case M_S:
		for (int i = 0; i < 4; i++)
		{
			hellbullet[hellbulletcheck].create = true;
			hellbullet[hellbulletcheck].x = mop[_i].x + 10;			// ó���� �߻�� ���� ��ġ�� ����
			hellbullet[hellbulletcheck].y = mop[_i].y + 60;
			hellbullet[hellbulletcheck].bit = HellBulletBit;

			hellbullet[hellbulletcheck].sx = -4 + (i * 2);
			hellbullet[hellbulletcheck].sy = ENEMYBULLETSPEED;
			HellBulletCheckControl();
		}
		break;
	case M_P:
		hellbullet[hellbulletcheck].create = true;
		hellbullet[hellbulletcheck].x = mop[_i].x + 10;			// ó���� �߻�� ���� ��ġ�� ����
		hellbullet[hellbulletcheck].y = mop[_i].y + 60;
		hellbullet[hellbulletcheck].bit = HellBulletBit;

		hellbullet[hellbulletcheck].sx = 0;
		hellbullet[hellbulletcheck].sy = ENEMYBULLETSPEED;
		HellBulletCheckControl();
		break;
	case M_C:
		for (int i = 0; i < 10; i++)
		{
			hellbullet[hellbulletcheck].create = true;
			hellbullet[hellbulletcheck].x = mop[_i].x + 20;		// ó���� �߻�� ���� ��ġ�� ����
			hellbullet[hellbulletcheck].y = mop[_i].y + 60;
			hellbullet[hellbulletcheck].bit = HellBulletBit;

			hellbullet[hellbulletcheck].sx = (-9 + (i * 2)) / 2;
			hellbullet[hellbulletcheck].sy = 3;
			HellBulletCheckControl();
		}
		break;
	case M_T:
		for (int i = 0; i < 7; i++)
		{
			hellbullet[hellbulletcheck].sy = ENEMYBULLETSPEED;
			hellbullet[hellbulletcheck].create = true;
			hellbullet[hellbulletcheck].x = mop[_i].x + 40;
			hellbullet[hellbulletcheck].y = mop[_i].y + 80;
			hellbullet[hellbulletcheck].bit = HellBulletBit;

			hellbullet[hellbulletcheck].sx = -3 + i;
			HellBulletCheckControl();
		}
		break;
	case KING1:				// ���� ����
		hellbullet[hellbulletcheck].sy = 10;
		hellbullet[hellbulletcheck].create = true;
		hellbullet[hellbulletcheck].x = king.x + 25;
		hellbullet[hellbulletcheck].y = king.y + 140;
		hellbullet[hellbulletcheck].bit = KingBulletBit;

		hellbullet[hellbulletcheck].sx = 0;
		HellBulletCheckControl();
		break;
	case KING2:					// �簥�� ����
		for (int i = 0; i < 2; i++)
		{
			hellbullet[hellbulletcheck].sy = 10;
			hellbullet[hellbulletcheck].create = true;
			hellbullet[hellbulletcheck].x = king.x + 25;
			hellbullet[hellbulletcheck].y = king.y + 140;
			hellbullet[hellbulletcheck].bit = KingBulletBit;

			if (i == 0)
			{
				hellbullet[hellbulletcheck].sx = -3;
			}
			else
			{
				hellbullet[hellbulletcheck].sx = 3;
			}
			HellBulletCheckControl();
		}
		break;
	case KING3:				// �ݰ� �� ����
		hellbullet[hellbulletcheck].sy = 0;
		hellbullet[hellbulletcheck].create = true;
		hellbullet[hellbulletcheck].bit = KingBulletBit;

		switch (rnd)
		{
		case 0:
			hellbullet[hellbulletcheck].x = 800;
			hellbullet[hellbulletcheck].y = 880;
			hellbullet[hellbulletcheck].sx = -10;
			break;
		case 1:
			hellbullet[hellbulletcheck].x = -100;
			hellbullet[hellbulletcheck].y = 880;
			hellbullet[hellbulletcheck].sx = 10;
			break;
		}


		HellBulletCheckControl();
		break;
	case KING4:				// ��Ѹ���1
		for (int i = 0; i < 5; i++)
		{
			hellbullet[hellbulletcheck].sy = 6;
			hellbullet[hellbulletcheck].create = true;
			hellbullet[hellbulletcheck].x = king.x + 25;
			hellbullet[hellbulletcheck].y = king.y + 140;
			hellbullet[hellbulletcheck].bit = KingBulletBit;
			hellbullet[hellbulletcheck].sx = -10 + i * 4;
			HellBulletCheckControl();
		}
		break;
	case KING5:				// ��Ѹ���2
		for (int i = 0; i < 4; i++)
		{
			hellbullet[hellbulletcheck].sy = 6;
			hellbullet[hellbulletcheck].create = true;
			hellbullet[hellbulletcheck].x = king.x + 25;
			hellbullet[hellbulletcheck].y = king.y + 140;
			hellbullet[hellbulletcheck].bit = KingBulletBit;
			hellbullet[hellbulletcheck].sx = -8 + i * 4;
			HellBulletCheckControl();
		}
		break;
	case QUEEN1:				// �簥�� ���
		for (int i = 0; i < 4; i++)
		{
			hellbullet[hellbulletcheck].sy = 12;
			hellbullet[hellbulletcheck].create = true;
			hellbullet[hellbulletcheck].x = boss.x + 40;
			hellbullet[hellbulletcheck].y = boss.y + 120;
			hellbullet[hellbulletcheck].bit = BossBulletBit;
			hellbullet[hellbulletcheck].sx = -3 + i * 2;
			HellBulletCheckControl();
		}
		break;
	case QUEEN2:				// 3���� ���
		for (int i = 0; i < 3; i++)
		{
			hellbullet[hellbulletcheck].sy = 12;
			hellbullet[hellbulletcheck].create = true;
			hellbullet[hellbulletcheck].x = boss.x + 40;
			hellbullet[hellbulletcheck].y = boss.y + 120;
			hellbullet[hellbulletcheck].bit = BossBulletBit;
			hellbullet[hellbulletcheck].sx = -2 + i * 2;
			HellBulletCheckControl();
		}
		break;
	case QUEEN3:				// ���� ����
		random = (float)(rand() % 10) - 5;

		hellbullet[hellbulletcheck].sy = 15;
		hellbullet[hellbulletcheck].create = true;
		hellbullet[hellbulletcheck].x = boss.x + 40;
		hellbullet[hellbulletcheck].y = boss.y + 120;
		hellbullet[hellbulletcheck].bit = BossBulletBit;
		hellbullet[hellbulletcheck].sx = random;
		HellBulletCheckControl();
		break;
	case QUEEN4:				// ��Ʋ��
		random = (float)(rand() % 14) - 7;

		hellbullet[hellbulletcheck].sy = 7;
		hellbullet[hellbulletcheck].create = true;
		hellbullet[hellbulletcheck].x = boss.x + 40;
		hellbullet[hellbulletcheck].y = boss.y + 120;
		hellbullet[hellbulletcheck].bit = BossBulletBit;
		hellbullet[hellbulletcheck].sx = random;
		HellBulletCheckControl();
		break;
	case QUEEN5:
		for (int i = 0; i < 20; i++)
		{
			hellbullet[hellbulletcheck].sx = 10 + (i * -1);
			hellbullet[hellbulletcheck].create = true;
			hellbullet[hellbulletcheck].x = 350;
			hellbullet[hellbulletcheck].y = 0;
			hellbullet[hellbulletcheck].bit = BossBulletBit;
			hellbullet[hellbulletcheck].sy = 10;
			HellBulletCheckControl();
		}
		break;
	case SETT1:
		for (int i = 0; i < 3; i++)
		{
			hellbullet[hellbulletcheck].sx = 0;
			hellbullet[hellbulletcheck].create = true;
			hellbullet[hellbulletcheck].x = sett.x + 30 + (30 * i);
			hellbullet[hellbulletcheck].y = sett.y + 120;
			hellbullet[hellbulletcheck].bit = SettBulletBit;
			hellbullet[hellbulletcheck].sy = 12;
			HellBulletCheckControl();
		}
		break;
	case SETT2:
		if (sett.grit > MAXGRIT)
		{
			EfectCreate(sett.x, sett.y, SettMax[2], 8);
		}
		else
		{
			EfectCreate(sett.x, sett.y, SettEBit, 8);
		}
		Music(1);
		sett.show = false;
		SetTimer(hWndMain, 8, 640, NULL);
		break;
	case SETT3:
		EfectCreate(-25, 0, SettWEfectBit, 16);
		if (sett.grit > MAXGRIT)
		{
			EfectCreate(sett.x, sett.y, SettMax[1], 15);
		}
		else
		{
			EfectCreate(sett.x, sett.y, SettWBit, 15);
		}
		sett.show = false;
		SetTimer(hWndMain, 9, 1360, NULL);
		break;
	case SETT4:
		for (int i = 0; i < 5; i++)
		{
			hellbullet[hellbulletcheck].sx = i * -2;
			hellbullet[hellbulletcheck].create = true;
			hellbullet[hellbulletcheck].x = sett.x + 60;
			hellbullet[hellbulletcheck].y = sett.y + 120;
			hellbullet[hellbulletcheck].bit = SettBulletBit;
			hellbullet[hellbulletcheck].sy = 12;
			HellBulletCheckControl();
		}
		break;
	case SETT5:
		for (int i = 0; i < 5; i++)
		{
			hellbullet[hellbulletcheck].sx = i * 2;
			hellbullet[hellbulletcheck].create = true;
			hellbullet[hellbulletcheck].x = sett.x + 60;
			hellbullet[hellbulletcheck].y = sett.y + 120;
			hellbullet[hellbulletcheck].bit = SettBulletBit;
			hellbullet[hellbulletcheck].sy = 12;
			HellBulletCheckControl();
		}
		break;
	case SETT6:
		for (int i = 0; i < 4; i++)
		{
			random = rand() % 12 - 6;
			hellbullet[hellbulletcheck].sx = random;
			hellbullet[hellbulletcheck].create = true;
			hellbullet[hellbulletcheck].x = sett.x + 60;
			hellbullet[hellbulletcheck].y = sett.y + 120;
			hellbullet[hellbulletcheck].bit = SettBulletBit;
			hellbullet[hellbulletcheck].sy = 10;
			HellBulletCheckControl();
		}
		break;
	case SETT7:
		for (int i = 2; i < 5; i++)
		{
			hellbullet[hellbulletcheck].sx = i * -2;
			hellbullet[hellbulletcheck].create = true;
			hellbullet[hellbulletcheck].x = sett.x + 60;
			hellbullet[hellbulletcheck].y = sett.y + 120;
			hellbullet[hellbulletcheck].bit = SettBulletBit;
			hellbullet[hellbulletcheck].sy = 8;
			HellBulletCheckControl();
		}
		for (int i = 1; i < 5; i++)
		{
			hellbullet[hellbulletcheck].sx = i * 2;
			hellbullet[hellbulletcheck].create = true;
			hellbullet[hellbulletcheck].x = sett.x + 60;
			hellbullet[hellbulletcheck].y = sett.y + 120;
			hellbullet[hellbulletcheck].bit = SettBulletBit;
			hellbullet[hellbulletcheck].sy = 8;
			HellBulletCheckControl();
		}
		break;
	}
}

// �� ���
void MopShot()
{
	for (int i = 0; i < 30; i++)
	{
		if (mop[i].create)
		{
			mop[i].gage++;
			if (mop[i].gage == mop[i].maxgage)
			{
				HellBulletCreate(mop[i].type , i);
				mop[i].gage = 0;
				//PlaySound(MAKEINTRESOURCE(IDR_WAVE3), g_hInst, SND_RESOURCE | SND_ASYNC);
			}
		}
	}
}

// �� ��Ʈ��
void HellMopControl(HDC hMemDC)
{
	for (int i = 0; i < 30; i++)			// ���� 30����� Ȯ��
	{
		if (mop[i].create)				// �����Ǿ��ִٸ�
		{
			mop[i].y++;					// �� ���� ���� ���� �ӵ��� �������� �߽��ϴ�

			if (mop[i].x < 0)			// ���� �ε��� ��� �ݴ�������� ���� �߽��ϴ�
			{
				mop[i].dir = 1;
			}
			else if (mop[i].x > 700)
			{
				mop[i].dir = 0;
			}

			if (mop[i].dir == 0)		// dir�� ���� �̵������� �ٲ�ϴ�
			{
				mop[i].x--;
			}
			else if (mop[i].dir == 1)
			{
				mop[i].x++;
			}
			MoveTransBlt(hMemDC, mop[i].x, mop[i].y, HellMopBit[mop[i].type], RGB(76, 76, 76), pistol_frm);		// Ÿ�Կ� �°� �׸���

			if (mop[i].y >= 1000)			// ���� �ϴ� ������ ������
			{
				mop[i].create = false;		// ������� �߽��ϴ�
			}
			if (mop[i].hp <= 0)				// hp�� 0 ���ϰ� �Ǹ�
			{
				mop[i].create = false;		// ������� �߽��ϴ�
				EfectCreate(mop[i].x, mop[i].y, MopDieBit[mop[i].type + 3], 4);
				point += (mop[i].type * 100) + 150;
			}
		}
	}
}

// �� ����
void HellMopCreate(int x, int type, int dir)
{
	mop[mopcheck].y = -100;		// ������ �ۿ������� ������ �ǰ� �߽��ϴ�
	mop[mopcheck].x = x;			// x�� ���ϴ� ��ġ�� ���� �� �� �ֵ��� �س����ϴ�
	mop[mopcheck].create = true;	// �������·� �ٲپ����ϴ�
	mop[mopcheck].dir = dir;		// ���� ��������
	mop[mopcheck].type = type;
	switch (type)
	{
	case M_P:
		mop[mopcheck].hp = 40;					// �����ϸ� hp �ʱ�ȭ
		mop[mopcheck].maxgage = 75;			// ��� ������ �ʱ�ȭ
		break;
	case M_C:
		mop[mopcheck].hp = 90;					// �����ϸ� hp �ʱ�ȭ
		mop[mopcheck].maxgage = 250;			// ��� ������ �ʱ�ȭ
		break;
	case M_S:
		mop[mopcheck].hp = 120;					// �����ϸ� hp �ʱ�ȭ
		mop[mopcheck].maxgage = 120;			// ��� ������ �ʱ�ȭ
		break;
	case M_T:
		mop[mopcheck].hp = 300;					// �����ϸ� hp �ʱ�ȭ
		mop[mopcheck].maxgage = 300;			// ��� ������ �ʱ�ȭ
		break;
	}

	if (mopcheck != 29)				// 29�� ���� ���ķδ� �ٽ� 0����
	{
		mopcheck++;
	}
	else
	{
		mopcheck = 0;					// 0���� �ǵ����� �ٽ� �迭�� ó������ �����ϵ��� �߽��ϴ�
	}
}

// �� �Ѿ� ��Ʈ��
void HellBulletControl(HDC hMemDC)
{
	for (int i = 0; i < 100; i++)
	{
		if (hellbullet[i].create)
		{
			hellbullet[i].y += hellbullet[i].sy;			// �̵� ����
			hellbullet[i].x += hellbullet[i].sx;

			if (hellbullet[i].bit == KingBulletBit)
			{
				MoveTransBlt(hMemDC, hellbullet[i].x, hellbullet[i].y, hellbullet[i].bit, RGB(255, 255, 255), king_bullet_frm);
			}
			else
			{
				TransBlt(hMemDC, hellbullet[i].x, hellbullet[i].y, hellbullet[i].bit, RGB(255, 255, 255));
			}
		}
		if (hellbullet[i].y >= 1000)		// ������ ������ ����
		{
			hellbullet[i].create = false;
		}
	}

	for (int i = 0; i < 100; i++)
	{
		if (bullet[i].create)
		{
			bullet[i].y -= bullet[i].sy;			// �̵� ����
			bullet[i].x -= bullet[i].sx;

			if (bullet[i].bit == NewBulletBit[1])
			{
				MoveTransBlt(hMemDC, bullet[i].x, bullet[i].y, bullet[i].bit, RGB(76, 76, 76), pistol_frm);
			}
			else
			{
				TransBlt(hMemDC, bullet[i].x, bullet[i].y, bullet[i].bit, RGB(76, 76, 76));
			}
		}
		if (hellbullet[i].y <= -20)		// ������ ������ ����
		{
			hellbullet[i].create = false;
		}
	}
}

// ȸ���� ����
void HpPotionCreate(int x)
{
	HpPotion.create = true;		// �����ϰ�
	HpPotion.x = x;				// HpPotion.x�� ���� x������ �����߽��ϴ�
	HpPotion.y = -100;			// �Ⱥ��̴� ������ ����
}

// ȸ���� �� ������ ��Ʈ��
void HpPotionControl(HDC hMemDC)
{
	if (HpPotion.create)			// ���� �Ǿ����� ��
	{
		HpPotion.y++;				// ���ӵ��� ���� �������ϴ�
		TransBlt(hMemDC, HpPotion.x, HpPotion.y, HpPotionBit, RGB(255, 255, 255));
	}
	
	if (HpPotion.y >= 1000)				// �Ʒ��� �Ѿ�� ������ϴ�
	{
		HpPotion.create = false;
	}

	for (int i = 0; i < 5; i++)
	{
		if (item[i].create)
		{
			item[i].y += item[i].sy;
			TransBlt(hMemDC, item[i].x, item[i].y, item[i].bit, RGB(76, 76, 76));
		}
	}
}

// ���� �Ѿ� ��Ʈ�� 3
void BossBulletControl3(HDC hMemDC)
{
	for (int i = 0; i < 30; i++)
	{
		if (bossbullet[i].create)
		{
			bossbullet[i].y += 16;				// ��������
			bossbullet[i].x += bossbullet[i].random / 3;	// �밢���̵��� ���� �������� �¿� �̵��� �߽��ϴ�

			TransBlt(hMemDC, bossbullet[i].x, bossbullet[i].y, BossBulletBit, RGB(255, 255, 255));
		}
		if (bossbullet[i].y >= 1000)		// ������ ������ ����
		{
			bossbullet[i].create = false;
		}
	}
}

// ���� �Ѿ� ��Ʈ�� 2
void BossBulletControl2(HDC hMemDC)
{
	for (int i = 0; i < 30; i++)
	{
		if (bossbullet[i].create)
		{
			bossbullet[i].y += 2;		// ��������
			bossbullet[i].x += bossbullet[i].random / 20;	// �밢���̵��� ���� �������� �¿� �̵��� �߽��ϴ�
			
			TransBlt(hMemDC, bossbullet[i].x, bossbullet[i].y, BossBulletBit, RGB(255, 255, 255));
		}
		if (bossbullet[i].y >= 1000)	// ������ ������ ����
		{
			bossbullet[i].create = false;
		}
	}
}

// ���� �Ѿ� ��Ʈ��
void BossBulletControl(HDC hMemDC)
{
	for (int i = 0; i < 30; i++)
	{
		if (bossbullet[i].create)
		{
			bossbullet[i].y += 12;			// �������� �Ѿ��� ��ȣ�� ���� �¿�� ���ư����߽��ϴ�
			if (i == 0 || i == 3 || i == 6 || i == 9 || i == 12 || i == 15 || i == 18 || i == 21 || i == 24 || i == 27 || i == 30)
			{
				bossbullet[i].x -= 2;
			}
			else if (i == 1 || i == 4 || i == 7 || i == 10 || i == 13 || i == 16 || i == 19 || i == 22 || i == 25 || i == 26)
			{
				bossbullet[i].x += 2;
			}
			TransBlt(hMemDC, bossbullet[i].x, bossbullet[i].y, BossBulletBit, RGB(255, 255, 255));
		}
		if (bossbullet[i].y >= 1000)		// �������� ������ ����
		{
			bossbullet[i].create = false;
		}
	}
}

// ���� ��� (����1)
void BossShot()
{
	boss.stance = 0;
	boss_frm = 0;		// ���� ������ �ʱ�ȭ
	for (int i = 0; i < 3; i++)
	{
		bossbullet[bossbulletcheck].create = true;
		if (bossbulletcheck != 29)							// �ִ� �Ѿ� ������ �����ϸ� 0���� �ǵ��Ƚ��ϴ�
		{
			bossbullet[bossbulletcheck].x = boss.x + 40;		// �Ѿ��� ������ ��ġ�Դϴ�
			bossbullet[bossbulletcheck].y = boss.y + 120;
			bossbulletcheck++;
		}
		else
		{
			bossbullet[bossbulletcheck].x = boss.x + 40;
			bossbullet[bossbulletcheck].y = boss.y + 120;
			bossbulletcheck = 0;			// �ƴҰ�� 0���� �ٽ� ����
		}
	}
	//PlaySound(MAKEINTRESOURCE(IDR_WAVE1), g_hInst, SND_RESOURCE | SND_ASYNC);
}

void BossShoting()
{
	for (int i = 0; i < 5; i++)
	{
		hellbullet[hellbulletcheck].create = true;
		hellbullet[hellbulletcheck].x = boss.x - 65 + i * 70;		// �Ѿ��� ������ ��ġ�Դϴ�
		hellbullet[hellbulletcheck].y = boss.y + 120;
		hellbullet[hellbulletcheck].bit = BossBulletBit;
		hellbullet[hellbulletcheck].sy = 12;
		if (hellbulletcheck != 99)							// �ִ� �Ѿ� ������ �����ϸ� 0���� �ǵ��Ƚ��ϴ�
		{
			hellbulletcheck++;
		}
		else
		{
			hellbulletcheck = 0;			// �ƴҰ�� 0���� �ٽ� ����
		}
	}
	//PlaySound(MAKEINTRESOURCE(IDR_WAVE1), g_hInst, SND_RESOURCE | SND_ASYNC);
}

// ���� ���� ���2
void BossShot2()
{
	bossbullet[bossbulletcheck].random = rand() % 100 - 50;		// �Ѿ��� �¿�� �󸶳� �̵� �� �� �������� �ο��߽��ϴ�

	bossbullet[bossbulletcheck].create = true;
	if (bossbulletcheck != 29)
	{
		bossbullet[bossbulletcheck].x = boss.x + 40;			// ������ ���� ����մϴ�
		bossbullet[bossbulletcheck].y = boss.y + 120;
		bossbulletcheck++;
	}
	else
	{
		bossbullet[bossbulletcheck].x = boss.x + 40;
		bossbullet[bossbulletcheck].y = boss.y + 120;
		bossbulletcheck = 0;
	}
	//PlaySound(MAKEINTRESOURCE(IDR_WAVE1), g_hInst, SND_RESOURCE | SND_ASYNC);
}

// ���� ��� 3
void BossShot3()
{
	bossbullet[bossbulletcheck].random = rand() % 100 - 50;

	bossbullet[bossbulletcheck].create = true;
	if (bossbulletcheck != 29)
	{
		bossbullet[bossbulletcheck].x = boss.x + 80;
		bossbullet[bossbulletcheck].y = boss.y + 140;
		bossbulletcheck++;
	}
	else
	{
		bossbullet[bossbulletcheck].x = boss.x + 80;
		bossbullet[bossbulletcheck].y = boss.y + 140;
		bossbulletcheck = 0;
	}
	//PlaySound(MAKEINTRESOURCE(IDR_WAVE1), g_hInst, SND_RESOURCE | SND_ASYNC);
}

// ���� ��Ʈ�� �Լ�
void BossControl(HDC hMemDC)
{
	if (boss.create)
	{
		boss.gage++;
		if (boss.gage >= 200 && boss.state == 0)		// �������� ���� ����, ���°� �⺻���¶�� �ൿ�մϴ�
		{
			BossShot();
			boss.state = 1;
			boss.gage = 0;
		}

		if (boss.gage2 % 300 == 0)
		{
			BossShoting();
		}
		
		if (boss.gage2 < 1100 && boss.state != 2)		// �������� 1100 ���� Ŀ�� �� �����߽��ϴ�
		{
			boss.gage2++;
		}
		else if (boss.gage > 0)
		{
			boss.gage2--;
		}
		if (boss.gage2 >= 1100 || boss.state == 2)				// ������2�� ���� á�µ�
		{
			if (boss.state == 0 || boss.state == 2)			// �ٸ� ���� �߿� ���ϰ��߽��ϴ�
			{
				if (boss.hp >= 1600)			// ���� ü���� 1600 �̻��̶��
				{
					BossShot();				// ���� 1�� �߽��ϴ�
					boss.state = 1;
				}
				else						// �ƴ϶��
				{
					if (boss.state == 0)
					{
						boss_frm = 0;
						boss.state = 2;
						boss.stance = 2;
						SetTimer(hWndMain, 5, 7000, NULL);
					}
					else if (boss.gage2 <= 1000 && boss.gage2 % 10 == 0 && boss.gage2 >= 0)
					{
						BossShot3();			// ���� 3�� �߽��ϴ�
					}
				}
			}
		}

		if (boss.hp >= 1000)
		{
			if (boss.gage3 < 500 && boss.state != 3)		// �������� �� ä��� �����ϰ�
			{
				boss.gage3++;
			}
			else if (boss.gage >= 0)				// ��� ����
			{
				boss.gage3 -= 10;
			}

			if (boss.gage3 == 500 || boss.state == 3)			// �������� ����
			{
				if (boss.state == 0 || boss.state == 3)		// �ٸ� ���� �߿� ���ϰ��߽��ϴ�
				{
					if (boss.state == 0)		// ���°� �⺻�� ����
					{
						if (boss.state != 3)		// ���� ȸ������ ����
						{
							boss.stance = 1;	// �Ѿ� ��Ʈ�� ����
							boss_frm = 0;		// ���� ������ �ʱ�ȭ
							SetTimer(hWndMain, 4, 3000, NULL);			// 3���� ȸ�� ������ ���� �⺻���� ����
						}
					}
					if (boss.gage3 % 50 == 0 && boss.gage3 >= 0)	// �������� ���鼭 50�� ����϶����� �Ѿ� �߻�
					{
						BossShot2();
					}
					boss.state = 3;
				}
			}
		}
		else
		{
			if (boss.gage3 < 500 && boss.state != 4)		// �������� �� ä��� �����ϰ�
			{
				boss.gage3++;
			}
			else if (boss.gage >= 0)				// ��� ����
			{
				boss.gage3 -= 10;
			}

			if (boss.gage3 == 500 || boss.state == 4)			// �������� ����
			{
				if (boss.state == 0 || boss.state == 4)
				{
					if (boss.state == 0)		// ���°� �⺻�� ����
					{
						if (boss.state != 4)		// ���� ȸ������ ����
						{
							boss.stance = 0;	// �Ѿ� ��Ʈ�� ����
							boss_frm = 0;		// ���� ������ �ʱ�ȭ
							SetTimer(hWndMain, 4, 3000, NULL);			// 3���� ȸ�� ������ ���� �⺻���� ����
						}
					}
					if (boss.gage3 % 50 == 0 && boss.gage3 >= -500)	// �������� ���鼭 50�� ����϶����� �Ѿ� �߻�
					{
						BossShot2();
					}
					boss.state = 4;
				}
			}
		}


		switch (boss.state)			// state�� ���� ��Ʈ���� �ٸ��� �׷Ƚ��ϴ�
		{
		case 0:
			BossMoveTransBlt(hMemDC, boss.x, boss.y, BossBit[0], RGB(255, 255, 255), boss_frm);
			break;
		case 1:
			BossMoveTransBlt(hMemDC, boss.x, boss.y, BossBit[1], RGB(76, 76, 76), boss_frm);
			break;
		case 2:
		case 5:
			BossMoveTransBlt(hMemDC, boss.x, boss.y, BossBit[2], RGB(255, 255, 255), boss_frm);
			break;
		case 3:
			BossMoveTransBlt(hMemDC, boss.x, boss.y, BossBit[3], RGB(255, 255, 255), boss_frm);
			break;
		case 4:
			BossMoveTransBlt(hMemDC, boss.x, boss.y, BossBit[4], RGB(255, 255, 255), boss_frm);
			if (boss.dir == 0)				// dir�� �°� �̵��߽��ϴ�
			{
				boss.x += 10;
			}
			else
			{
				boss.x -= 10;
			}
			break;
		}



		if (boss.hp <= 0)			// ü���� 0 ���ϰ� �� ��� ����
		{
			boss.create = false;
			Level++;
		}
		BossLifeBlt(hMemDC, 100, 30, BossLifeBit, RGB(255, 255, 255));		// ���� ü�� ��Ʈ��
	}
	if (!boss.create)
	{
		boss.hp = 2000;
	}
}

// ���� ��ȯ
void BossCreate()
{
	boss.create = true;		// ��ȯ�ϸ� �ʱ�ȭ �۾��Դϴ�
	boss.gage = 0;
	boss.gage2 = 0;
	boss.gage3 = 0;
	boss.state = 0;
	boss.hp = 100;
	boss.x = 325;
	boss.y = 80;
}

// ������ ü���� �׸��� �Լ��Դϴ�
void BossLifeBlt(HDC hdc, int x, int y, HBITMAP hbitmap, COLORREF clrMask)
{
	BITMAP bm;
	HDC hMemDC;
	POINT ptSize;
	HBITMAP hOldBitmap;

	hMemDC = CreateCompatibleDC(hdc);
	hOldBitmap = (HBITMAP)SelectObject(hMemDC, hbitmap);
	GetObject(hbitmap, sizeof(BITMAP), (LPSTR)&bm);
	ptSize.x = bm.bmWidth;
	ptSize.y = bm.bmHeight;

	if (hbitmap == BossLifeBit)
	{
		TransparentBlt(hdc, x, y, boss.hp / 3.34, 50, hMemDC, 0, 0, ptSize.x, ptSize.y, clrMask);
	}
	else if (hbitmap == KingLifeBit)
	{
		TransparentBlt(hdc, x, y, king.hp / 5, 50, hMemDC, 0, 0, ptSize.x, ptSize.y, clrMask);
	}
	else if (hbitmap == SettLifeBit)
	{
		TransparentBlt(hdc, x, y, sett.hp / 5, 50, hMemDC, 0, 0, ptSize.x, ptSize.y, clrMask);
	}

	SelectObject(hMemDC, hOldBitmap);
	DeleteDC(hMemDC);
}

// ������ �׸��� �Լ��Դϴ�
void LifeBlt(HDC hdc, int x, int y, HBITMAP hbitmap, COLORREF clrMask, int hp)
{
	BITMAP bm;
	HDC hMemDC;
	POINT ptSize;
	HBITMAP hOldBitmap;

	hMemDC = CreateCompatibleDC(hdc);
	hOldBitmap = (HBITMAP)SelectObject(hMemDC, hbitmap);
	GetObject(hbitmap, sizeof(BITMAP), (LPSTR)&bm);
	ptSize.x = bm.bmWidth;
	ptSize.y = bm.bmHeight;

	TransparentBlt(hdc, x, y, php * 50, 50, hMemDC, 0, 0, hp * 50, ptSize.y, clrMask);

	SelectObject(hMemDC, hOldBitmap);
	DeleteDC(hMemDC);
}

// ���� �߻�
void EnemyShot()
{
	for (int i = 0; i < 30; i++)
	{
		if (pistol[i].create)
		{
			pistol[i].gage++;
			if (pistol[i].gage == 100)
			{
				enemybullet[enemybulletcheck].create = true;				// �Ѿ��� �����ϰ� �߽��ϴ�
				if (enemybulletcheck != 99)							// �ִ� �Ѿ� ������ �����ϸ� 0���� �ǵ��Ƚ��ϴ�
				{
					enemybullet[enemybulletcheck].x = pistol[i].x + 20;			// ó���� �߻� �Ǵ� �������� ���Ѻ� ��ġ�� ������ϴ�
					enemybullet[enemybulletcheck].y = pistol[i].y + 80;
					enemybulletcheck++;
				}
				else
				{
					enemybullet[enemybulletcheck].x = pistol[i].x + 20;
					enemybullet[enemybulletcheck].y = pistol[i].y + 80;
					enemybulletcheck = 0;
				}
				pistol[i].gage = 0;
				//PlaySound(MAKEINTRESOURCE(IDR_WAVE2), g_hInst, SND_RESOURCE | SND_ASYNC);
			}
		}
	}
	for (int i = 0; i < 10; i++)
	{
		//����
		if (rifle[i].create)							// ���� ���� ���� ������� ���� ���� ó���߽��ϴ�
		{
			rifle[i].gage++;
			if (rifle[i].gage >= 220 && rifle[i].y <= py + 30 && rifle[i].y >= py - 10)			// �÷��̾�� Y���� ���������, �������� ���� á�ٸ�
			{
				rifle[i].hide = false;								// ���� ���°� Ǯ����
				riflebullet[riflebulletcheck].create = true;		// �߻�
				if (riflebulletcheck != 4)
				{
					riflebullet[riflebulletcheck].x = rifle[i].x + 100;
					riflebullet[riflebulletcheck].y = rifle[i].y + 30;
					riflebulletcheck++;
				}
				else
				{
					riflebullet[riflebulletcheck].x = rifle[i].x + 100;
					riflebullet[riflebulletcheck].y = rifle[i].y + 30;
					riflebulletcheck = 0;
				}
				rifle[i].gage = 0;
				//PlaySound(MAKEINTRESOURCE(IDR_WAVE2), g_hInst, SND_RESOURCE | SND_ASYNC);
			}
		}
		//�����
		if (machinegun[i].create)
		{
			machinegun[i].gage++;
			if (machinegun[i].gage == 15)
			{
				enemybullet[enemybulletcheck].create = true;
				if (enemybulletcheck != 99)
				{
					enemybullet[enemybulletcheck].x = machinegun[i].x + 50;
					enemybullet[enemybulletcheck].y = machinegun[i].y + 100;
					enemybulletcheck++;
				}
				else
				{
					enemybullet[enemybulletcheck].x = machinegun[i].x + 50;
					enemybullet[enemybulletcheck].y = machinegun[i].y + 100;
					enemybulletcheck = 0;
				}
				machinegun[i].gage = 0;
				//PlaySound(MAKEINTRESOURCE(IDR_WAVE2), g_hInst, SND_RESOURCE | SND_ASYNC);
			}
		}
	}
}

// �� �ǰ�
DWORD CALLBACK EnemyDamaged(LPVOID n)
{
	while (1)
	{
		if (!AllStop)
		{
			for (int i = 0; i < 100; i++)
			{
				if (bullet[i].create)			// ������ �Ѿ˸�
				{
					for (int j = 0; j < 30; j++)
					{
						if (pistol[j].create)		// ������ ����
						{
							if (CheckPtInRect(BulletBit, bullet[i].x, bullet[i].y, PistolBit, pistol[j].x, pistol[j].y))		// �浹 Ȯ�� ��
							{
								bullet[i].create = false;			// �Ѿ��� ���ְ�
								pistol[j].hp -= 10;				// ü���� -10
								if (damage)
								{
									pistol[j].hp -= 10;			// �÷��̾� ���� �� 2�� ����
								}
							}
						}
					}

					for (int j = 0; j < 10; j++)
					{

						if (rifle[j].create)		// ������ ����
						{
							if (rifle[j].hide == false)		// ���� ���¸� �ǰݹ�ȿ
							{
								if (CheckPtInRect(BulletBit, bullet[i].x, bullet[i].y, RifleBit[0], rifle[j].x, rifle[j].y))		// �浹 Ȯ�� ��
								{
									bullet[i].create = false;			// �Ѿ��� ���ְ�
									rifle[j].hp -= 10;				// ü���� -10
									if (damage)
									{
										rifle[j].hp -= 10;			// �÷��̾� ���� �� 2�� ����
									}
								}
							}
						}


						if (machinegun[j].create)		// ������ ����
						{
							if (CheckPtInRect(BulletBit, bullet[i].x, bullet[i].y, MachineGunBit, machinegun[j].x, machinegun[j].y))		// �浹 Ȯ�� ��
							{
								bullet[i].create = false;			// �Ѿ��� ���ְ�
								machinegun[j].hp -= 10;				// ü���� -10
								if (damage)
								{
									machinegun[j].hp -= 10;			// �÷��̾� ���� �� 2�� ����
								}
							}
						}
					}

					if (boss.create)		// ������ ���� ������ ��
					{
						if (CheckPtInRect(BulletBit, bullet[i].x, bullet[i].y, BossBit[0], boss.x, boss.y))		// �浹 Ȯ�� ��
						{
							bullet[i].create = false;			// �Ѿ��� ���ְ�
							boss.hp -= 5;				// ü���� -5
							if (damage)
							{
								boss.hp -= 5;			// �÷��̾� ���� �� 2�� ����
							}
						}
					}

					if (king.create)		// ŷ�� ���� ������ ��
					{
						if (CheckPtInRect(BulletBit, bullet[i].x, bullet[i].y, BossBit[0], king.x, king.y))		// �浹 Ȯ�� ��
						{
							bullet[i].create = false;			// �Ѿ��� ���ְ�

							king.hp -= 5;				// ü���� -5
							if (damage)
							{
								king.hp -= 5;			// �÷��̾� ���� �� 2�� ����
							}

						}
					}

					if (sett.create)		// ��Ʈ�� ���� ������ ��
					{
						if (CheckPtInRect(BulletBit, bullet[i].x, bullet[i].y, SettBit, sett.x, sett.y))		// �浹 Ȯ�� ��
						{
							bullet[i].create = false;			// �Ѿ��� ���ְ�

							sett.hp -= 5;				// ü���� -5
							if (sett.show)
							{
								sett.grit += 5;
							}

						}
					}

					for (int j = 0; j < 30; j++)
					{
						if (mop[j].create)		// ������ ����
						{
							if (CheckPtInRect(BulletBit, bullet[i].x, bullet[i].y, HellMopBit[mop[j].type], mop[j].x, mop[j].y))		// �浹 Ȯ�� ��
							{
								bullet[i].create = false;			// �Ѿ��� ���ְ�
								mop[j].hp -= 10;				// ü���� -10
								if (damage)
								{
									mop[j].hp -= 10;			// �÷��̾� ���� �� 2�� ����
								}
							}
						}
					}
				}
			}
		}

	}

	return 0;
}

// �÷��̾� �ǰ�
DWORD CALLBACK Damaged(LPVOID n)
{
	while (1)
	{
		if (!AllStop)
		{
			for (int i = 0; i < 100; i++)
			{
				if (enemybullet[i].create)			// ������ �Ѿ˸�
				{
					if (CheckPtInRect(EnemyBulletBit, enemybullet[i].x, enemybullet[i].y, PlayerBit, px, py))		// �浹 Ȯ�� ��
					{
						if (!damage)
						{
							enemybullet[i].create = false;			// �Ѿ��� ���ְ�
							php--;			// �÷��̾��� ü���� -1
							boomindex++;	// ��ź�� ���
							damage = true;
							SetTimer(hWndMain, 3, 2000, NULL);
							Music(3);
						}
					}
				}
			}

			for (int i = 0; i < 5; i++)
			{
				if (riflebullet[i].create)			// ������ �Ѿ˸�
				{
					if (CheckPtInRect(EnemyBulletBit, riflebullet[i].x, riflebullet[i].y, PlayerBit, px, py))		// �浹 Ȯ�� ��
					{
						if (!damage)
						{
							riflebullet[i].create = false;			// �Ѿ��� ���ְ�
							php--;			// �÷��̾��� ü���� -1
							boomindex++;	// ��ź�� ���
							damage = true;
							SetTimer(hWndMain, 3, 2000, NULL);
							Music(3);
						}
					}
				}
			}

			for (int i = 0; i < 50; i++)		// ���� �Ѿ�
			{
				if (bossbullet[i].create)			// ������ �Ѿ˸�
				{
					if (CheckPtInRect(BossBulletBit, bossbullet[i].x, bossbullet[i].y, PlayerBit, px, py))		// �浹 Ȯ�� ��
					{
						if (!damage)
						{
							bossbullet[i].create = false;			// �Ѿ��� ���ְ�
							php--;			// �÷��̾��� ü���� -1
							boomindex++;	// ��ź�� ���
							damage = true;
							SetTimer(hWndMain, 3, 2000, NULL);
							Music(3);
						}
					}
				}
			}

			for (int i = 0; i < 30; i++)			// ���Ѻ��� �浹
			{
				if (pistol[i].create)
				{
					if (CheckPtInRect(PistolBit, pistol[i].x, pistol[i].y, PlayerBit, px, py))
					{
						if (!damage)
						{
							php--;
							boomindex++;	// ��ź�� ���
							damage = true;
							SetTimer(hWndMain, 3, 2000, NULL);
							Music(3);
						}
					}
				}
			}

			for (int i = 0; i < 10; i++)			// ����Ѻ��� �浹
			{
				if (machinegun[i].create)
				{
					if (CheckPtInRect(MachineGunBit, machinegun[i].x, machinegun[i].y, PlayerBit, px, py))
					{
						if (!damage)
						{
							php--;
							boomindex++;	// ��ź�� ���
							damage = true;
							SetTimer(hWndMain, 3, 2000, NULL);
							Music(3);
						}
					}
				}
			}

			if (boss.create)		// ������ �浹
			{
				if (CheckPtInRect(BossBit[0], boss.x, boss.y, PlayerBit, px, py))
				{
					if (!damage)
					{
						php--;
						boomindex++;	// ��ź�� ���
						damage = true;
						SetTimer(hWndMain, 3, 2000, NULL);
						Music(3);
					}
				}
			}

			if (king.create)		// ������ �浹
			{
				if (CheckPtInRect(KingBit[0], king.x, king.y, PlayerBit, px, py))
				{
					if (!damage)
					{
						php--;
						boomindex++;	// ��ź�� ���
						damage = true;
						SetTimer(hWndMain, 3, 2000, NULL);
						Music(3);
					}
				}
			}

			if (sett.create)		// ������ �浹
			{
				if (CheckPtInRect(SettBit, sett.x, sett.y, PlayerBit, px, py))
				{
					if (!damage)
					{
						php--;
						boomindex++;	// ��ź�� ���
						damage = true;
						SetTimer(hWndMain, 3, 2000, NULL);
						Music(3);
					}
				}
			}

			if (HpPotion.create)		// ȸ����� �浹
			{
				if (CheckPtInRect(HpPotionBit, HpPotion.x, HpPotion.y, PlayerBit, px, py))
				{
					if (php != 3)
					{
						HpPotion.create = false;
						php++;
						PlaySound("Item.wav", NULL, SND_ASYNC);
					}
					else if (!damage)
					{
						HpPotion.create = false;
						damage = true;
						boomindex++;
						if (QueenPlayer)
						{
							PlaySound("Item.wav", NULL, SND_ASYNC);
						}
						else
						{
							Music(3);
						}
						SetTimer(hWndMain, 3, 5000, NULL);
					}
				}
			}

			for (int i = 0; i < 100; i++)		// �� �Ѿ�
			{
				if (hellbullet[i].create)			// ������ �� �Ѿ˸�
				{
					if (CheckPtInRect(hellbullet[i].bit, hellbullet[i].x, hellbullet[i].y, PlayerBit, px, py))		// �浹 Ȯ�� ��
					{
						if (!damage)
						{
							hellbullet[i].create = false;			// �Ѿ��� ���ְ�
							php--;			// �÷��̾��� ü���� -1
							boomindex++;	// ��ź�� ���
							damage = true;
							SetTimer(hWndMain, 3, 2000, NULL);
							Music(3);
						}
					}
				}
			}

			for (int i = 0; i < 5; i++)			// �����۰� �浹
			{
				if (item[i].create)
				{
					if (CheckPtInRect(item[i].bit, item[i].x, item[i].y, PlayerBit, px, py))
					{
						if (item[i].bit == ItemBit[0])
						{
							style = SGUN;
							bulletindex = 8;
						}
						else if (item[i].bit == ItemBit[1])
						{
							style = HHGUN;
							bulletindex = 60;
						}
						else if (item[i].bit == ItemBit[2])
						{
							style = HGUN;
							bulletindex = 100;
						}
						item[i].create = false;
						PlaySound("Gun.wav", NULL, SND_ASYNC);
					}
				}
			}
		}
	}
	return 0;
}

// �簢�� �浹
BOOL CheckPtInRect(HBITMAP hBit, int Obitx, int Obity, HBITMAP Pbit, int Pbitx, int Pbity) //������ ��ǥ, ��ġ, ��Ʈ��, ��Ʈ�� ��ġ
{
	int ox, oy;
	int ppx, ppy;
	BITMAP Obit, Pl_bit;
	RECT Temp, O_bit, P_bit;


	GetObject(hBit, sizeof(BITMAP), &Obit);

	if (hBit == PistolBit || hBit == MachineGunBit || hBit == HellMopBit[0] || hBit == HellMopBit[1] || hBit == HellMopBit[2] || hBit == HellMopBit[3])
	{
		ox = 100;
		oy = 100;
	}
	else if (hBit == PlayerBit)
	{
		ox = 100;
		oy = 100;
	}
	else if (hBit == BossBit[0])
	{
		ox = 150;
		oy = 150;
	}
	else if (hBit == KingBulletBit)
	{
		ox = 100;
		oy = 100;
	}
	else if (hBit == NewBulletBit[1])
	{
		ox = 40;
		oy = 40;
	}
	else
	{
		ox = Obit.bmWidth;
		oy = Obit.bmHeight;
	}

	if (hBit == KingBulletBit)
	{
		O_bit.top = Obity + 10;
		O_bit.left = Obitx + 10;
		O_bit.right = Obitx + ox - 10;
		O_bit.bottom = Obity + oy - 10;
	}
	else
	{
		O_bit.top = Obity;
		O_bit.left = Obitx;
		O_bit.right = Obitx + ox;
		O_bit.bottom = Obity + oy;
	}

	GetObject(Pbit, sizeof(BITMAP), &Pl_bit);

	if (Pbit == PistolBit || Pbit == RifleBit[0] || Pbit == RifleBit[1] || Pbit == MachineGunBit
		|| Pbit == HellMopBit[0] || Pbit == HellMopBit[1] || Pbit == HellMopBit[2] || Pbit == HellMopBit[3])
	{
		ppx = 100;
		ppy = 100;
	}
	else if (Pbit == PlayerBit)
	{
		ppx = 100;
		ppy = 100;
	}
	else if (Pbit == BossBit[0])
	{
		ppx = 150;
		ppy = 150;
	}
	else
	{
		ppx = Pl_bit.bmWidth;
		ppy = Pl_bit.bmHeight;
	}

	if (Pbit == PistolBit || Pbit == MachineGunBit || Pbit == HellMopBit[0] || Pbit == HellMopBit[1] || Pbit == HellMopBit[2] || Pbit == HellMopBit[3])				// ��Ʈ�� �� �浹������ �ٸ��� �ϱ� ���� if���� ����ϴ�
	{
		P_bit.top = Pbity - 15;
		P_bit.left = Pbitx - 15;
		P_bit.right = Pbitx + 85;
		P_bit.bottom = Pbity + 85;
	}
	else if (Pbit == PlayerBit)
	{
		P_bit.top = Pbity + 50;
		P_bit.left = Pbitx + 50;
		P_bit.right = Pbitx + ppx - 35;
		P_bit.bottom = Pbity + ppy - 35;
	}
	else if (Pbit == BossBit[0] || Pbit == KingBit[0])
	{
		P_bit.top = Pbity;
		P_bit.left = Pbitx + 40;
		P_bit.right = Pbitx + ppx - 30;
		P_bit.bottom = Pbity + ppy;
	}
	else
	{
		P_bit.top = Pbity;
		P_bit.left = Pbitx;
		P_bit.right = Pbitx + ppx;
		P_bit.bottom = Pbity + ppy;
	}

	if (IntersectRect(&Temp, &O_bit, &P_bit))
	{
		return true;
	}
	else
	{
		return false;
	}

}

// ���Ѻ� ��ȯ
void PistolCreate(int x, int dir)
{
	if (pistolcheck != 29)				// 9�� �̻� �������� ���� class �迭�� 10���̹Ƿ� �ٽ� 0���� ����ϰ� �߽��ϴ�
	{
		pistol[pistolcheck].y = -100;		// ������ �ۿ������� ������ �ǰ� �߽��ϴ�
		pistol[pistolcheck].x = x;			// x�� ���ϴ� ��ġ�� ���� �� �� �ֵ��� �س����ϴ�
		pistol[pistolcheck].create = true;	// �������·� �ٲپ����ϴ�
		pistol[pistolcheck].dir = dir;
		pistol[pistolcheck].hp = 30;					// �����ϸ� hp �ʱ�ȭ
		pistolcheck++;		// �������� ���� �迭�� �ֱ����� +1�� ���־����ϴ�
	}
	else
	{
		pistol[pistolcheck].y = -100;
		pistol[pistolcheck].x = x;
		pistol[pistolcheck].create = true;
		pistol[pistolcheck].hp = 30;
		pistol[pistolcheck].dir = dir;
		pistolcheck = 0;					// 0���� �ǵ����� �ٽ� �迭�� ó������ �����ϵ��� �߽��ϴ�
	}
}

// ���Ѻ� ��ȯ
void RifleCreate(int x)					// ���� �����ϴ�
{
	if (riflecheck != 9)
	{
		rifle[riflecheck].y = -100;
		rifle[riflecheck].x = x;
		rifle[riflecheck].create = true;
		rifle[riflecheck].hide = true;
		rifle[riflecheck].hp = 20;
		riflecheck++;
	}
	else
	{
		rifle[riflecheck].y = -100;
		rifle[riflecheck].x = x;
		rifle[riflecheck].create = true;
		rifle[riflecheck].hide = true;
		rifle[riflecheck].hp = 20;
		riflecheck = 0;
	}
}

// ����Ѻ� ��ȯ
void MachineGunCreate(int x, int dir)
{
	if (machineguncheck != 9)
	{
		machinegun[machineguncheck].y = -100;
		machinegun[machineguncheck].x = x;
		machinegun[machineguncheck].create = true;
		machinegun[machineguncheck].hp = 80;
		machinegun[machineguncheck].dir = dir;
		machineguncheck++;
	}
	else
	{
		machinegun[machineguncheck].y = -100;
		machinegun[machineguncheck].x = x;
		machinegun[machineguncheck].create = true;
		machinegun[machineguncheck].hp = 80;
		machinegun[machineguncheck].dir = dir;
		machineguncheck = 0;
	}
}

// �������� 1
void Stage1()										// ���������� �����ϴ� �Լ��Դϴ�
{
	switch (stagecheck)
	{
	case 1:
		showlevel = true;
		break;
	case 3:
		HpPotionCreate(400);
		showlevel = false;
		break;
	case 5:
		PistolCreate(0, 1);
		PistolCreate(700);
		break;
	case 7:
		PistolCreate(100, 1);
		PistolCreate(300);
		break;
	case 9:
		PistolCreate(100, 1);
		PistolCreate(300, 1);
		PistolCreate(500, 1);
		PistolCreate(700, 1);
		break;
	case 12:
		MachineGunCreate(0);
		break;
	case 13:
		RifleCreate(0);
		break;
	case 15:
		PistolCreate(100, 0);
		PistolCreate(350, 1);
		PistolCreate(700, 1);
		break;
	case 20:
		MachineGunCreate(700, 1);
		break;
	case 22:
		RifleCreate(0);
		PistolCreate(0);
		PistolCreate(700, 1);
		break;
	case 25:
		PistolCreate(0);
		PistolCreate(100);
		PistolCreate(200);
		PistolCreate(700, 1);
		PistolCreate(600, 1);
		PistolCreate(500, 1);
		break;
	case 27:
		MachineGunCreate(0);
		MachineGunCreate(700, 1);
		MachineGunCreate(500, 1);
		break;
	case 30:
		MachineGunCreate(200);
		MachineGunCreate(600, 1);
		ItemCreate(100, 0, ItemBit[2]);
		break;
	case 32:
		PistolCreate(200);
		break;
	case 33:
		PistolCreate(200);
		break;
	case 34:
		PistolCreate(200);
		break;
	case 35:
		PistolCreate(200);
		break;
	case 36:
		PistolCreate(600, 1);
		break;
	case 37:
		PistolCreate(600, 1);
		break;
	case 38:
		PistolCreate(600, 1);
		break;
	case 39:
		PistolCreate(600, 1);
		break;
	case 40:
		RifleCreate(0);
		MachineGunCreate(700, 1);
		break;
	case 42:
		PistolCreate(0);
		PistolCreate(100);
		PistolCreate(200);
		break;
	case 44:
		PistolCreate(700, 1);
		PistolCreate(600, 1);
		PistolCreate(500, 1);
		break;
	case 45:
		MachineGunCreate(0);
		break;
	case 47:
		MachineGunCreate(300, 1);
		break;
	case 48:
		PistolCreate(200, 1);
		break;
	case 50:
		PistolCreate(400, 1);
		break;
	case 58:
		Level++;
		stagecheck = 0;
		break;
	}

	stagecheck++;
}

// �������� 2
void Stage2()													// ���������� �����ϴ� �Լ��Դϴ�
{
	switch (stagecheck)
	{
	case 1:
		showlevel = true;
		break;
	case 3:
		HpPotionCreate(400);
		showlevel = false;
		break;
	case 5:
		PistolCreate(700, 0);
		break;
	case 7:
		PistolCreate(100, 1);
		break;
	case 9:
		MachineGunCreate(500, 0);
		MachineGunCreate(300, 0);
		break;
	case 10:
		PistolCreate(100, 1);
		break;
	case 12:
		MachineGunCreate(0, 0);
		MachineGunCreate(700, 1);
		break;
	case 13:
		PistolCreate(400, 0);
		PistolCreate(400, 1);
		break;
	case 14:
		PistolCreate(400, 0);
		PistolCreate(400, 1);
		break;
	case 15:
		PistolCreate(400, 0);
		PistolCreate(400, 1);
		break;
	case 16:
		PistolCreate(400, 0);
		PistolCreate(400, 1);
		break;
	case 18:
		PistolCreate(200, 0);
		PistolCreate(500, 1);
		break;
	case 19:
		PistolCreate(200, 0);
		PistolCreate(500, 1);
		break;
	case 20:
		RifleCreate(0);
		PistolCreate(200, 0);
		PistolCreate(500, 1);
		break;
	case 21:
		PistolCreate(200, 0);
		PistolCreate(500, 1);
		break;
	case 22:
		PistolCreate(200, 0);
		PistolCreate(500, 1);
		ItemCreate(500, 0, ItemBit[0]);
		break;
	case 25:
		MachineGunCreate(0);
		break;
	case 27:
		MachineGunCreate(100);
		break;
	case 29:
		MachineGunCreate(200);
		break;
	case 31:
		MachineGunCreate(300);
		break;
	case 33:
		MachineGunCreate(400);
		break;
	case 36:
		MachineGunCreate(700, 1);
		break;
	case 38:
		MachineGunCreate(600, 1);
		break;
	case 40:
		MachineGunCreate(500, 1);
		break;
	case 45:
		PistolCreate(0);
		break;
	case 46:
		PistolCreate(150);
		break;
	case 47:
		PistolCreate(300);
		break;
	case 49:
		ItemCreate(600, 0, ItemBit[2]);
		PistolCreate(700);
		break;
	case 50:
		PistolCreate(550);
		break;
	case 51:
		PistolCreate(400);
		break;
	case 53:
		MachineGunCreate(100);
		break;
	case 55:
		RifleCreate(0);
		break;
	case 57:
		RifleCreate(0);
		break;
	case 58:
		PistolCreate(100, 1);
		PistolCreate(600, 0);
		break;
	case 62:
		PlaySound("Warning.wav", NULL, SND_ASYNC);
		showwarning = true;
		break;
	case 63:
		showwarning = false;
		break;
	case 64:
		showwarning = true;
		break;
	case 65:
		showwarning = false;
		break;
	case 68:
		KingCreate();
		break;
	}

	stagecheck++;
}

// �������� Hell
void HellStage()													// ���������� �����ϴ� �Լ��Դϴ�
{
	switch (hellstagecheck)
	{
	case 1:
		showlevel = true;
		break;
	case 3:
		HpPotionCreate(400);
		showlevel = false;
		break;
	case 5:
		HellMopCreate(100, M_P, 0);
		break;
	case 6:
		HellMopCreate(600, M_P, 1);
		break;
	case 7:
		HellMopCreate(100, M_P, 0);
		break;
	case 8:
		HellMopCreate(600, M_P, 1);
		break;
	case 10:
		HellMopCreate(400, M_C, 0);
		ItemCreate(100, 0, ItemBit[1]);
		break;
	case 12:
		HellMopCreate(400, M_P, 0);
		break;
	case 14:
		HellMopCreate(130, M_P, 0);
		break;
	case 15:
		HellMopCreate(570, M_P, 1);
		break;
	case 16:
		HellMopCreate(130, M_P, 0);
		break;
	case 17:
		HellMopCreate(570, M_P, 1);
		break;
	case 18:
		HellMopCreate(130, M_P, 0);
		break;
	case 19:
		HellMopCreate(570, M_P, 1);
		break;
	case 20:
		HellMopCreate(350, M_T, 0);
		break;
	case 22:
		HellMopCreate(250, M_C, 0);
		HellMopCreate(550, M_C, 1);
		break;
	case 25:
		HellMopCreate(250, M_S, 0);
		HellMopCreate(550, M_S, 1);
		break;
	case 32:
		HellMopCreate(250, M_P, 0);
		HellMopCreate(550, M_P, 1);
		break;
	case 35:
		HellMopCreate(450, M_T, 0);
		break;
	case 38:
		HellMopCreate(100, M_P, 1);
		HellMopCreate(300, M_P, 1);
		HellMopCreate(500, M_P, 1);
		break;
	case 40:
		ItemCreate(500, 0, ItemBit[2]);
		HellMopCreate(300, M_P, 0);
		HellMopCreate(500, M_P, 0);
		HellMopCreate(700, M_P, 0);
		break;
	case 42:
		HellMopCreate(100, M_P, 1);
		HellMopCreate(300, M_P, 1);
		HellMopCreate(500, M_P, 1);
		break;
	case 45:
		HellMopCreate(100, M_S, 1);
		break;
	case 48:
		HellMopCreate(700, M_S, 0);
		break;
	case 50:
		HellMopCreate(500, M_P, 1);
		break;
	case 51:
		HellMopCreate(700, M_P, 0);
		break;
	case 52:
		HellMopCreate(500, M_P, 1);
		break;
	case 53:
		HellMopCreate(700, M_P, 0);
		break;
	case 54:
		HellMopCreate(500, M_P, 1);
		break;
	case 58:
		PlaySound("Warning.wav", NULL, SND_ASYNC);
		showwarning = true;
		break;
	case 59:
		showwarning = false;
		break;
	case 60:
		showwarning = true;
		break;
	case 61:
		showwarning = false;
		break;
	case 64:
		if (realhell)
		{
			KingCreate();
			SettCreate();
			BossCreate();
		}
		else if (QueenPlayer)
		{
			SettCreate();
		}
		else
		{
			BossCreate();
		}
		break;
	}

	hellstagecheck++;
}

// ���� �Ѿ��� ��Ʈ�� ���ִ� �Լ�
void EnemyBulletControl(HDC hMemDC)
{
	for (int i = 0; i < 100; i++)			// Bullet�� 100���� Ȯ���ؼ�
	{
		if (enemybullet[i].create)				// �����Ǿ��ִ� �Ѿ��̶��
		{
			enemybullet[i].y += ENEMYBULLETSPEED;				// �� �Ѿ��� �Ʒ��� ���ư��� �߽��ϴ�
			TransBlt(hMemDC, enemybullet[i].x, enemybullet[i].y, EnemyBulletBit, RGB(255, 255, 255));		// �Ѿ��� �׷��־����
		}
		if (enemybullet[i].y >= 1000)				// ���� ��� ������ ������
		{
			enemybullet[i].create = false;		// ������� �߽��ϴ�
		}
	}
}

// ���Ѻ� �Ѿ��� ��Ʈ�� ���ִ� �Լ�
void RifleBulletControl(HDC hMemDC)		// ���Ѻ� ���� �Ѿ� ����� �ӵ�
{
	for (int i = 0; i < 5; i++)			// Bullet�� 5���� Ȯ���ؼ�
	{
		if (riflebullet[i].create)				// �����Ǿ��ִ� �Ѿ��̶��
		{
			riflebullet[i].x += 20;				// �� �Ѿ��� ���������� ���ư��� �߽��ϴ�
			TransBlt(hMemDC, riflebullet[i].x, riflebullet[i].y, EnemyBulletBit, RGB(255, 255, 255));		// �Ѿ��� �׷��־����
		}
		if (riflebullet[i].x >= 800)				// ���� ���� ������ ������
		{
			riflebullet[i].create = false;		// ������� �߽��ϴ�
		}
	}
}

// ���Ѻ��� ��Ʈ�� ���ִ� �Լ�
void PistolControl(HDC hMemDC)
{
	for (int i = 0; i < 30; i++)			// ���� 30����� Ȯ��
	{
		if (pistol[i].create)				// �����Ǿ��ִ� ���Ѻ��̶��
		{
			pistol[i].y++;					// �� ���� ���� ���� �ӵ��� �������� �߽��ϴ�

			if (pistol[i].x < 0)			// ���� �ε��� ��� �ݴ�������� ���� �߽��ϴ�
			{
				pistol[i].dir = 1;
			}
			else if (pistol[i].x > 700)
			{
				pistol[i].dir = 0;
			}
			
			if (pistol[i].dir == 0)		//dir�� ���� �̵������� �ٲ�ϴ�
			{
				pistol[i].x--;
			}
			else if(pistol[i].dir == 1)
			{
				pistol[i].x++;
			}
			MoveTransBlt(hMemDC, pistol[i].x, pistol[i].y, PistolBit, RGB(76, 76, 76), pistol_frm);		// ���� �׷��־����

			if (pistol[i].y >= 1000)			// ���� �ϴ� ������ ������
			{
				pistol[i].create = false;		// ������� �߽��ϴ�
			}
			if (pistol[i].hp <= 0)				// hp�� 0 ���ϰ� �Ǹ�
			{
				pistol[i].create = false;		// ������� �߽��ϴ�
				EfectCreate(pistol[i].x, pistol[i].y, MopDieBit[0], 4);
				point += 100;
			}
		}
	}
}

// ���Ѻ��� ��Ʈ�� ���ִ� �Լ�
void RifleControl(HDC hMemDC)
{

	for (int i = 0; i < 10; i++)			// ���� 10����� Ȯ��
	{
		if (rifle[i].create)				// �����Ǿ��ִ� ���̶��
		{
			if (rifle[i].y <= py + 20)			// �÷��̾ y������ ����ٴϰ� �߽��ϴ�
			{
				rifle[i].y++;
			}
			else
			{
				rifle[i].y--;
			}

			if (rifle[i].hide)				// ���¿� ���� �ٲ�� ��Ʈ��
			{
				MoveTransBlt(hMemDC, rifle[i].x, rifle[i].y, RifleBit[1], RGB(76, 76, 76), pistol_frm);		// ���� �׷��־����
			}
			else
			{
				MoveTransBlt(hMemDC, rifle[i].x, rifle[i].y, RifleBit[0], RGB(76, 76, 76), pistol_frm);
			}

			if (rifle[i].hp <= 0)				// hp�� 0 ���ϰ� �Ǹ�
			{
				rifle[i].create = false;		// ������� �߽��ϴ�
				EfectCreate(rifle[i].x, rifle[i].y, MopDieBit[1], 2);
				point += 150;
			}
		}
	}
}

// ����Ѻ��� ��Ʈ�� ���ִ� �Լ�
void MachineGunControl(HDC hMemDC)
{
	for (int i = 0; i < 10; i++)			// ���� 10����� Ȯ��
	{
		if (machinegun[i].create)				// �����Ǿ��ִ� ����Ѻ��̶��
		{

			machinegun[i].y++;					// �� ���� ���� ���� �ӵ��� �������� �߽��ϴ�

			if (machinegun[i].x < 0)			// ���� �ε��� ��� �ݴ�������� ���� �߽��ϴ�
			{
				machinegun[i].dir = 1;
			}
			else if (machinegun[i].x > 700)
			{
				machinegun[i].dir = 0;
			}

			if (machinegun[i].dir == 0)		//dir�� ���� �̵������� �ٲ�ϴ�
			{
				machinegun[i].x -= 5;
			}
			else if (machinegun[i].dir == 1)
			{
				machinegun[i].x += 5;
			}
			MoveTransBlt(hMemDC, machinegun[i].x, machinegun[i].y, MachineGunBit, RGB(76, 76, 76), pistol_frm);		// ���� �׷��־����

			if (machinegun[i].y >= 1000)			// ���� �ϴ� ������ ������
			{
				machinegun[i].create = false;		// ������� �߽��ϴ�
			}
			if (machinegun[i].hp <= 0)				// hp�� 0 ���ϰ� �Ǹ�
			{
				machinegun[i].create = false;		// ������� �߽��ϴ�
				EfectCreate(machinegun[i].x, machinegun[i].y, MopDieBit[2], 4);
				point += 300;
			}
		}
	}
}

// ����� �׸��� �Լ��Դϴ�
void BackgroundBlt(HDC hdc, int x, int y, HBITMAP hBit)
{
	HDC MemDC;
	HBITMAP OldBitmap;
	int bx, by;
	BITMAP bit;

	MemDC = CreateCompatibleDC(hdc);
	OldBitmap = (HBITMAP)SelectObject(MemDC, hBit);

	GetObject(hBit, sizeof(BITMAP), &bit);
	bx = bit.bmWidth;
	by = bit.bmHeight;

	BitBlt(hdc, x, y + bgy - 500, bx, by, MemDC, 0, 0, SRCCOPY);				// ����� �׷Ƚ��ϴ�
	if (bgy >= 500)
	{
		BitBlt(hdc, x, y + bgy - 500 - 1499, bx, by, MemDC, 0, 0, SRCCOPY);		// ����� �� ������ ���� ����� �ѹ��� �׷�����ϴ�
	}

	if (bgy >= 1499)					// �׸��� �ٽ� ��� ��ǥ�� 0���� �ٲٸ鼭 �ڿ������� ���̵��� �߽��ϴ�
	{
		bgy = 0;
	}

	SelectObject(MemDC, OldBitmap);
	DeleteDC(MemDC);
}

// �����̴� ������ �׸��� �Լ��Դϴ�
void BossMoveTransBlt(HDC hdc, int x, int y, HBITMAP hbitmap, COLORREF clrMask, int frm)
{
	BITMAP bm;
	HDC hMemDC;
	POINT ptSize;

	HBITMAP hOldBitmap;

	hMemDC = CreateCompatibleDC(hdc);
	hOldBitmap = (HBITMAP)SelectObject(hMemDC, hbitmap);
	GetObject(hbitmap, sizeof(BITMAP), (LPSTR)&bm);
	ptSize.x = bm.bmWidth;
	ptSize.y = bm.bmHeight;

	TransparentBlt(hdc, x, y, 150, 150, hMemDC, frm * 150, 0, 150, ptSize.y, clrMask);

	SelectObject(hMemDC, hOldBitmap);
	DeleteDC(hMemDC);
}

// �����̴� �׸��� �׸��� �Լ��Դϴ�
void MoveTransBlt(HDC hdc, int x, int y, HBITMAP hbitmap, COLORREF clrMask, int frm)
{
	BITMAP bm;
	HDC hMemDC;
	POINT ptSize;

	HBITMAP hOldBitmap;

	hMemDC = CreateCompatibleDC(hdc);
	hOldBitmap = (HBITMAP)SelectObject(hMemDC, hbitmap);
	GetObject(hbitmap, sizeof(BITMAP), (LPSTR)&bm);
	ptSize.x = bm.bmWidth;
	ptSize.y = bm.bmHeight;

	if (hbitmap == KingBit[0] || hbitmap == KingBit[1] || hbitmap == KingCreateBit
		|| hbitmap == KingAttackBit[0] || hbitmap == KingAttackBit[1]
		|| hbitmap == KingAttackBit[2] || hbitmap == KingAttackBit[3])
	{
		TransparentBlt(hdc, x, y, 150, 150, hMemDC, frm * 150, 0, 150, ptSize.y, clrMask);
	}
	else if (hbitmap == KingCounterAttackBit)
	{
		TransparentBlt(hdc, x, y, 800, 800, hMemDC, frm * 800, 0, 800, ptSize.y, clrMask);
	}
	else if (hbitmap == BossBit[1])
	{
		TransparentBlt(hdc, x, y, 150, 150, hMemDC, frm * 150, 0, 150, ptSize.y, clrMask);
	}
	else if (hbitmap == BoomBit)
	{
		TransparentBlt(hdc, x, y, 850, 850, hMemDC, frm * 850, 0, 850, ptSize.y, clrMask);
	}
	else if (hbitmap == NewBulletBit[1])
	{
		TransparentBlt(hdc, x, y, 40, 40, hMemDC, frm * 40, 0, 40, ptSize.y, clrMask);
	}
	else if (hbitmap == BoomBit2 || hbitmap == SettWEfectBit)
	{
		TransparentBlt(hdc, x, y, 800, 1000, hMemDC, frm * 800, 0, 800, ptSize.y, clrMask);
	}
	else if (hbitmap == SettWBit || hbitmap == SettEBit || hbitmap == SettMax[1] || hbitmap == SettMax[2])
	{
		TransparentBlt(hdc, x, y, 150, 150, hMemDC, frm * 150, 0, 150, ptSize.y, clrMask);
	}
	else
	{
		TransparentBlt(hdc, x, y, 100, 100, hMemDC, frm * 100, 0, 100, ptSize.y, clrMask);
	}

	SelectObject(hMemDC, hOldBitmap);
	DeleteDC(hMemDC);
}

// ������ ���� ������ �׷��ִ� �Լ��Դϴ�
void TransBlt(HDC hdc, int x, int y, HBITMAP hbitmap, COLORREF clrMask)
{

	BITMAP bm;
	HDC hMemDC;
	POINT ptSize;
	HBITMAP hOldBitmap;

	hMemDC = CreateCompatibleDC(hdc);
	hOldBitmap = (HBITMAP)SelectObject(hMemDC, hbitmap);
	GetObject(hbitmap, sizeof(BITMAP), (LPSTR)&bm);
	ptSize.x = bm.bmWidth;
	ptSize.y = bm.bmHeight;

	TransparentBlt(hdc, x, y, ptSize.x, ptSize.y, hMemDC, 0, 0, ptSize.x, ptSize.y, clrMask);

	SelectObject(hMemDC, hOldBitmap);
	DeleteDC(hMemDC);
}

// �׸��� �׷��ִ� �Լ��Դϴ�
void DrawBitmap(HDC hdc, int x, int y, HBITMAP hBit)
{
	HDC MemDC;
	HBITMAP OldBitmap;
	int bx, by;
	BITMAP bit;

	MemDC = CreateCompatibleDC(hdc);
	OldBitmap = (HBITMAP)SelectObject(MemDC, hBit);

	GetObject(hBit, sizeof(BITMAP), &bit);
	bx = bit.bmWidth;
	by = bit.bmHeight;

	BitBlt(hdc, x, y, bx, by, MemDC, 0, 0, SRCCOPY);

	SelectObject(MemDC, OldBitmap);
	DeleteDC(MemDC);
}

// ������۸��� �ٽ��Դϴ�
void OnTimer()
{
	RECT crt;
	HDC hdc, hMemDC;
	HBITMAP OldBit;

	GetClientRect(hWndMain, &crt);
	hdc = GetDC(hWndMain);

	if (!hBit)
	{
		hBit = CreateCompatibleBitmap(hdc, crt.right, crt.bottom);
	}

	hMemDC = CreateCompatibleDC(hdc);
	OldBit = (HBITMAP)SelectObject(hMemDC, hBit);

	if (!AllStop)
	{
		//���
		if (Level != 3)
		{
			BackgroundBlt(hMemDC, 0, 0, BackgroundBit);							// ����� �׷Ƚ��ϴ�
		}
		else
		{
			BackgroundBlt(hMemDC, 0, 0, HellBackgroundBit);
		}

		EfectControl(hMemDC);		// ����Ʈ ��Ʈ���Լ�

		//�÷��̾� ��Ʈ��

		if (!damage)
		{
			if (QueenPlayer)
			{
				if (QueenBoom)
				{
					if (player_frm == 6)
					{
						player_frm = 0;
					}
					MoveTransBlt(hMemDC, px, py, QueenBoomBit, RGB(255, 255, 255), player_frm);		// �� �÷��̾ �׷Ƚ��ϴ�
				}
				else
				{
					MoveTransBlt(hMemDC, px, py, QueenPlayerBit[0], RGB(255, 255, 255), player_frm);		// �� �÷��̾ �׷Ƚ��ϴ�
				}
			}
			else
			{
				MoveTransBlt(hMemDC, px, py, PlayerBit, RGB(255, 255, 255), player_frm);		// �÷��̾ �׷Ƚ��ϴ�
			}
		}
		else
		{
			if (QueenPlayer)
			{
				if (QueenBoom)
				{
					if (player_frm == 6)
					{
						player_frm = 0;
					}
					MoveTransBlt(hMemDC, px, py, QueenBoomBit, RGB(255, 255, 255), player_frm);		// �� �÷��̾ �׷Ƚ��ϴ�
				}
				else
				{
					MoveTransBlt(hMemDC, px, py, QueenPlayerBit[1], RGB(255, 255, 255), player_frm);		// �� �÷��̾ �׷Ƚ��ϴ�
				}
			}
			else
			{
				MoveTransBlt(hMemDC, px, py, PlayerDamagedBit, RGB(255, 255, 255), player_frm);		// �÷��̾ �׷Ƚ��ϴ�
			}
		}

		if (php <= 0)		// �÷��̾��� ü���� 0���ϰ� �Ǹ�
		{
			AllStop = true;		// ������ ������Ű��
			KillTimer(hWndMain, 3);		// ���� ���ֱ�
			if (boss.create)		// ���������� �׾��� ���� �ƴ� ���� �����ؼ� ������ ��������ϴ�
			{
				page = 2;
			}
			else if (king.create)
			{
				page = 6;
			}
			else if (sett.create)
			{
				page = 7;
			}
			else
			{
				page = 1;
			}
		}

		bgy++;				// ��� ������

		EnemyBulletControl(hMemDC);	// �� �Ѿ� ��Ʈ�� �Լ�

		PistolControl(hMemDC);			// ���Ѻ� ��Ʈ�� �Լ�

		EnemyShot();					// �� �߻�

		RifleControl(hMemDC);			// ���Ѻ� ��Ʈ�� �Լ�
		RifleBulletControl(hMemDC);	// ���Ѻ� �Ѿ� ��Ʈ�� �Լ�

		MachineGunControl(hMemDC);			// ����Ѻ� ��Ʈ�� �Լ�


		HellMopControl(hMemDC);		// hell �� ��Ʈ��
		MopShot();						// hell �� ���
		HellBulletControl(hMemDC);		// hell �� �Ѿ� ��Ʈ��

		/*if (boss.hp < 500)
		{*/
			QueenControl(hMemDC);		// �� ��Ʈ�� �Լ�
		/*}
		else
		{
			BossControl(hMemDC);		// ������Ʈ�� �Լ�
		}*/

		HpPotionControl(hMemDC);			// ���� ��Ʈ�� �Լ�

		KingControl(hMemDC);			// ŷ ��Ʈ���Լ�

		SettControl(hMemDC);			// ��Ʈ ��Ʈ���Լ�

		/*if (boss.hp >= 500)
		{
			if (boss.create)
			{
				switch (boss.stance)			// �Ѿ� ����
				{
				case 0:
					BossBulletControl(hMemDC);		// �����Ѿ� ��Ʈ�� �Լ�  ���� 1
					break;
				case 1:
					BossBulletControl2(hMemDC);	// ���� 2
					break;
				case 2:
					BossBulletControl3(hMemDC);	// ���� 3
					break;
				}
			}
		}*/

		if (boss.x > 650)				// ������ ���� ��ȯ�� ���� ��������ϴ�
		{
			boss.dir = 1;
		}
		if (boss.x < 0)
		{
			boss.dir = 0;
		}

		LifeBlt(hMemDC, 20, 880, LifeBit, RGB(255, 255, 255), php);		// �÷��̾� ü���Դϴ�

		if (boomcheck)		// ��ź ����Ʈ
		{
			MoveTransBlt(hMemDC, -25, 0, BoomBit, RGB(255, 0, 255), boom_frm);
			switch (boom_frm)
			{
			case 1:
			case 6:
			case 12:
			case 18:
			case 26:
				MopClear(50);
				if (boss.create)
				{
					boss.hp -= 3;
				}
				if (king.create)
				{
					king.hp -= 6;
				}
				if (sett.create)
				{
					sett.hp -= 6;
				}
				break;
			}
		}

		if (boomcheck2)
		{
			switch (boom2)
			{
			case 12:
				MopClear(300);
				boss.hp -= 70;
				king.hp -= 120;
				sett.hp -= 150;
				SetWindowPos(hWndMain, NULL, 555, 30, 800, 1000, 0);
				break;
			case 13:
				SetWindowPos(hWndMain, NULL, 545, 25, 800, 1000, 0);
				break;
			case 14:
				SetWindowPos(hWndMain, NULL, 550, 25, 800, 1000, 0);
				boomcheck2 = false;
				break;
			}
		}

		DrawBulletIndex(hMemDC);					// ���� �Ѿ�

		DrawBoomIndex(hMemDC);						// ���� ��ź

		DrawPoint(hMemDC);							// ����

		if (showlevel)
		{
			switch (Level)
			{
			case 1:
				DrawBitmap(hMemDC, 20, 400, LevelBit[0]);
				break;
			case 2:
				DrawBitmap(hMemDC, 20, 400, LevelBit[1]);
				break;
			case 3:
				DrawBitmap(hMemDC, 20, 400, LevelBit[2]);
				break;
			}
		}

		switch (vibes)
		{
		case 0:
			SetWindowPos(hWndMain, NULL, 560, 35, 800, 1000, 0);
			break;
		case 1:
			SetWindowPos(hWndMain, NULL, 540, 20, 800, 1000, 0);
			break;
		case 2:
			SetWindowPos(hWndMain, NULL, 550, 25, 800, 1000, 0);
			if (px + 75 > 257 && px + 25 < 512)
			{
				if (sett.grit > MAXGRIT)
				{
					php = 0;
				}
				else
				{
					if (!damage)
					{
						damage = true;
						SetTimer(hWndMain, 3, 2000, NULL);
						Music(3);
						php--;
					}
				}
			}
			if (sett.grit > MAXGRIT)
			{
				if (sett.hp >= 1000)
				{
					sett.grit = 0;
				}
				gritsound = true;
			}
			break;
		}

		if (showwarning)
		{
			TransBlt(hMemDC, 230, 300, WarningBit, RGB(255, 255, 255));
		}
	}
	


	switch (page)
	{
	case 0:
		if (QueenPlayer)
		{
			DrawBitmap(hMemDC, 0, 0, RevengeBit[0]);		// �� �κ� ȭ��
		}
		else
		{
			DrawBitmap(hMemDC, 0, 0, StartBit);		// �κ� ȭ��
		}
		break;
	case 1:
		if (QueenPlayer)
		{
			DrawBitmap(hMemDC, 0, 0, RevengeBit[4]);		// �� �׳� �й�
		}
		else
		{
			DrawBitmap(hMemDC, 0, 0, EndBit[0]);	// �׳� �й� ����
		}
		break;
	case 2:
		DrawBitmap(hMemDC, 0, 0, EndBit[1]);	// ������ �й� ����
		break;
	case 3:
		if (QueenPlayer)
		{
			DrawBitmap(hMemDC, 0, 0, RevengeBit[1]);		// �� Ŭ���� ȭ��
		}
		else
		{
			DrawBitmap(hMemDC, 0, 0, EndBit[2]);	// �¸� ����
		}
		DrawPoint(hMemDC);							// ����
		break;
	case 4:
		DrawBitmap(hMemDC, 0, 0, InstructionBit);	// ����
		break;
	case 5:
		if (QueenPlayer)
		{
			DrawBitmap(hMemDC, 0, 0, RevengeBit[3]);	// ���� ��~
		}
		else
		{
			DrawBitmap(hMemDC, 0, 0, HellInstructionBit);	// ����2
		}
		break;
	case 6:
		DrawBitmap(hMemDC, 0, 0, EndBit[3]);	// ŷ���� �й� ����
		break;
	case 7:
		DrawBitmap(hMemDC, 0, 0, RevengeBit[2]);	// �� ��Ʈ���� �й�
		break;
	}

	SelectObject(hMemDC, OldBit);
	DeleteDC(hMemDC);
	ReleaseDC(hWndMain, hdc);
	InvalidateRect(hWndMain, NULL, false);
}

// ���� ��
void Create()				// ��� ��Ʈ���� ���⼭ �����߽��ϴ�
{
	#pragma region Hell ���� ��Ʈ�ʵ�

	PlayerBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1));			// �÷��̾�
	BackgroundBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP2));		// ���
	BulletBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP3));			// �Ѿ�
	PistolBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP4));			// ���Ѻ�

	RifleBit[0] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP5));			// ���Ѻ�
	RifleBit[1] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP6));			// ���Ѻ� ���� �ڼ�
	RifleBit[2] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP7));			// ���Ѻ� ��� �ڼ�

	EnemyBulletBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP8));			// �� �Ѿ�

	LifeBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP10));			// �÷��̾� ����
	PlayerDamagedBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP11));			// �÷��̾� �ǰ�

	MachineGunBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP12));			// ����Ѻ�

	// ���� ��Ʈ�ʵ� (��)
	BossBit[0] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP13));			// �⺻
	BossBit[1] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP14));			// ���
	BossBit[2] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP15));			// ��ų
	BossBit[3] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP16));			// ȸ��1
	BossBit[4] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP17));			// ȸ��2

	BossLifeBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP18));			// ���� ü��
	BossBulletBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP19));			// ���� �Ѿ�
	//

	HpPotionBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP20));			// ȸ����

	StartBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP21));				// ���� ȭ��
	EndBit[0] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP22));			// �й� ���� 1
	EndBit[1] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP23));			// �й� ���� 2
	EndBit[2] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP24));			// ���� ���� 1

	InstructionBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP25));			// ����

	#pragma endregion

	HellInstructionBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP26));			// HELL ����

	HellBackgroundBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP27));			// �� ���

	HellBulletBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP28));			// �� �Ѿ�

	HellMopBit[0] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP29));			// ��ȭ�� ���Ѻ�
	HellMopBit[1] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP30));			// ũ����ƺ�
	HellMopBit[2] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP31));			// ���Ǻ�
	HellMopBit[3] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP32));			// ��ũ


	RifleBit[3] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP34));			// Hell ���Ѻ� ����
	RifleBit[4] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP33));			// �⺻
	RifleBit[5] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP35));			// ���

	// ŷ
	KingBit[0] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP37));			// �⺻
	KingBit[1] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP38));			// �ݰݱ�

	KingCreateBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP39));			// ��ȯ ����Ʈ
	KingAttackBit[0] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP40));			// ����1
	KingAttackBit[1] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP43));			// ����2
	KingAttackBit[2] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP44));			// ����3
	KingAttackBit[3] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP45));			// ����4
	KingBulletBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP41));			// �Ѿ�
	KingCounterAttackBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP42));	// �ݰ�����Ʈ
	KingLifeBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP46));	// ü�¹�
	//

	ItemBit[0] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP47));	// ������1
	ItemBit[1] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP48));	// ������2
	ItemBit[2] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP49));	// ������3

	NewBulletBit[0] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP50));	// S�Ѿ�
	NewBulletBit[1] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP51));	// HH�Ѿ�
	NewBulletBit[2] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP52));	// H�Ѿ�

	BoomBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP53));	// ��ź

	LevelBit[0] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP54));	// ����1
	LevelBit[1] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP55));	// ����2
	LevelBit[2] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP56));	// ����3

	WarningBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP57));	// Warning

	MopDieBit[0] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP58));	// ���Ѻ� ���
	MopDieBit[1] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP63));	// ���Ѻ� ���
	MopDieBit[2] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP59));	// ����Ѻ� ���
	MopDieBit[3] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP60));	// ���Ѻ�2 ���
	MopDieBit[4] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP61));	// ũ����ƺ� ���
	MopDieBit[5] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP62));	// ���Ǻ� ���
	MopDieBit[6] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP64));	// ��ũ ���


	SBulletBit[0] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP65));	// S�Ѿ�
	SBulletBit[1] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP66));	// S�Ѿ�
	SBulletBit[2] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP67));	// S�Ѿ�
	SBulletBit[3] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP68));	// S�Ѿ�
	SBulletBit[4] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP69));	// S�Ѿ�
	SBulletBit[5] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP70));	// S�Ѿ�
	SBulletBit[6] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP71));	// S�Ѿ�

	EndBit[3] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP72));	// ŷ ����

	BoomBit2 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP73));	// ��ź2

	QueenPlayerBit[0] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP74));	// ��
	QueenPlayerBit[1] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP75));	// �� ���ع���
	QueenPlayerBulletBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP76));	// �� ���ع���

	QueenBoomBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP77));	// �� ��ź

	SettBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP78));	// ��Ʈ
	SettWBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP79));	// ��Ʈ W ���
	SettWEfectBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP80));	// ��Ʈ W ����Ʈ
	SettBulletBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP81));	// ��Ʈ �Ѿ�
	SettLifeBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP82));	// ��Ʈ ü�¹�
	SettEBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP83));	// ��Ʈ E ���

	SettMax[2] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP84));	// ��Ʈ E
	SettMax[0] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP85));	// ��Ʈ �⺻
	SettMax[1] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP86));	// ��Ʈ W

	RevengeBit[0] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP87));	// �� �κ�
	RevengeBit[1] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP88));	// �� Ŭ����
	RevengeBit[2] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP89));	// �� ��Ʈ���� �й�
	RevengeBit[3] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP90));	// �� ����?
	RevengeBit[4] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP91));	// �� �й�



	hThrd1 = CreateThread(NULL, 0, Damaged, NULL, 0, NULL);
	CloseHandle(hThrd1);

	hThrd2 = CreateThread(NULL, 0, EnemyDamaged, NULL, 0, NULL);
	CloseHandle(hThrd2);

}


LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	
	switch (iMessage)
	{
	case WM_CREATE:
		srand(time(NULL));

		Create();		// ��Ʈ�� ���Կ� �Լ��Դϴ�
		
		SetTimer(hWnd, 0, 10, NULL);		// OnTimer, GetKeyState
		SetTimer(hWnd, 1, 80, NULL);		// OnTimer�� �ʹ� ���� ���� player_frm �� ���� ��Ʈ�� �� Ÿ�̸��Դϴ�

		OnBGM(LOBBY);

		return 0;
	case WM_TIMER:

		switch (wParam)
		{
		case 0:
			if (!AllStop)
			{
				if (px > MasX)
				{
					if (GetKeyState(VK_LEFT) & 0x8000)			//���� Ű�ڵ� (���� ���¿� Ű���� ���¸� ���) ���� ���¿��� Ű�� ���Ǵ��� Ȯ���غ�(���� 0x 8000)
					{												// ����
						px -= SPEED;
					}
				}

				if (px < MaxX)
				{
					if (GetKeyState(VK_RIGHT) & 0x8000)            // ������
					{
						px += SPEED;
					}
				}

				if (py > MasY)
				{
					if (GetKeyState(VK_UP) & 0x8000)            // ����
					{
						py -= SPEED;
					}
				}

				if (py < MaxY)
				{
					if (GetKeyState(VK_DOWN) & 0x8000)            // �Ʒ���
					{
						py += SPEED;
					}
				}
			}
			OnTimer();
			break;
		case 1:
			if (!AllStop)
			{
				if (delay)					// delay�� true�� ���� �߻簡 �����ϰ� �߽��ϴ�
				{
					switch (style)
					{
					case DEFAULT:
						DefaultShot();
						break;
					case SGUN:
						if (QueenBoom)
						{
							Sshot();
						}
						break;
					case HGUN:
						if (QueenBoom)
						{
							Hshot();
						}
						break;
					case HHGUN:
						if (QueenBoom)
						{
							HHshot();
						}
						break;
					}
				}
			}
			player_frm++;					// �÷��̾� �̵� �������� �ٲٴ� ��
			player_frm %= 8;				// �ٽ� �ʱ�ȭ ���׽��ϴ�

			pistol_frm++;				// ���Ѻ�		//���� ���� ����Դϴ�
			pistol_frm %= 4;

			if (boss_frm < 7 && boss.state != 5)			// 7 �̻� �ȿö󰡰� �߽��ϴ�
			{
				boss_frm++;					// ����
			}
			else if (boss.state == 5)		// ��ų ������
			{
				if (boss_frm != 0)			// 0�� ������ ����
				{
					boss_frm--;
				}
			}
			switch (boss.state)
			{
			case 0:			// �⺻����
				boss_frm %= 7;
				break;
			case 1:			// ����
				boss_frm %= 5;
				if (boss_frm == 0)
				{
					boss.state = 0;
				}
				break;
			case 2:			// ��Ʋ��		// �� ĭ �Դϴ�
			case 5:

				break;
			case 3:			// ȸ��
			case 4:
				boss_frm %= 6;
				break;
			}


			if (king.state != 1)
			{
				king_frm++;			// ŷ �����Ӱ���
			}
			else if(king_frm < 4)
			{
				king_frm++;
			}

			if (king.state == 0)
			{
				king_frm %= 6;
			}

			king_bullet_frm++;
			king_bullet_frm %= 5;

			king_attack_frm++;

			for (int i = 0; i < 10; i++)
			{
				if (efect[i].create)
				{
					efect[i].frm++;
				}
			}

			// ��ź ������
			if (boomcheck)
			{
				if (boom_frm != 29)
				{
					boom_frm++;
				}
				else
				{
					boom_frm = 0;
					boomcheck = false;
				}
			}
			if (boomcheck2)
			{
				boom2++;
			}

			// ������ �ð��� �������� ������ �𿩼� ���� ������ �����
			if (boss.create || king.create || sett.create)
			{
				point--;
			}
			
			if (sett.hp < 3000)
			{
				if (sett.hp >= 2000)
				{
					sett.hp += 1;
				}
				else if (sett.hp >= 1000)
				{
					sett.hp += 2;
				}
				else if (sett.hp >= 500)
				{
					sett.hp += 3;
				}
				else if (sett.hp >= 100)
				{
					sett.hp += 5;
				}
				else
				{
					sett.hp += 6;
				}
			}

			vibes++;
			break;
		case 2:						// stage �����
			switch (Level)
			{
			case 1:
				Stage1();
				break;
			case 2:
				Stage2();
				break;
			case 3:
				HellStage();
				break;
			}
			break;
		case 3:						// ���� ���� �� ���� ���� Ÿ�̸�
			damage = false;
			KillTimer(hWnd, 3);
			break;
		case 4:						// ���� 2 ����
			boss.state = 0;
			boss.gage3 = 0;
			KillTimer(hWnd, 4);
			break;
		case 5:						// ���� 3 ����1
			boss.gage2 = 0;
			boss.state = 5;
			SetTimer(hWnd, 6, 500, NULL);
			KillTimer(hWnd, 5);
			break;
		case 6:						// ���� 3 ����2
			boss.gage2 = 0;
			boss.stance = 0;
			boss.state = 0;
			KillTimer(hWnd, 6);
			break;
		case 7:
			QueenBoom = false;
			damage = false;
			KillTimer(hWnd, 7);
			break;
		case 8:
			sett.show = true;
			if (!realhell)
			{
				px = 350;
				SPEED = 3;
			}
			SetTimer(hWnd, 11, 1500, NULL);
			KillTimer(hWnd, 8);
			break;
		case 9:
			sett.show = true;
			KillTimer(hWnd, 9);
			break;
		case 10:
			vibes = 0;
			if (!realhell)
			{
				py = 850;
			}
			KillTimer(hWnd, 10);
			break;
		case 11:
			SPEED = 7;
			KillTimer(hWnd, 11);
			break;
		}
		return 0;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case 'Z':				// ���� ��ư
			if (page == 0)
			{
				if (QueenPlayer)
				{
					page = 5;
				}
				else
				{
					page = 4;
				}
				Level++;
			}
			else if (page == 4)			// ����â���� Z��ư�� ������ ���� �� �� �ְ��Ϸ��� �־����ϴ�
			{
				page = 5;			// ���� ����
			}
			else if (page == 5)
			{
				page = -1;			//case�� ���������ʴ� �ƹ� ��ȣ�� �ٲ���ϴ�
				AllStop = false;		// ��������
				SetTimer(hWndMain, 2, 1000, NULL);		// ��������1 ���� Ÿ�̸��Դϴ�
				if (!QueenPlayer)
				{
					OnBGM(INGAME);
				}
			}
			else if (page == 3)		// Ŭ���� ��
			{
				page = 0;		// �κ�ȭ��
				if (QueenPlayer)		// �� ȭ
				{
					QueenPlayer = false;
				}
				else
				{
					QueenPlayer = true;
				}
				Reset();		// �ʱ�ȭ
				OnBGM(LOBBY);
				point = 0;
				Level = 0;
			}
			else if (!AllStop || !stop)
			{
				if (delay && style != DEFAULT)					// delay�� true�� ���� �߻簡 �����ϰ� �߽��ϴ�
				{
					switch (style)
					{
					case DEFAULT:
						DefaultShot();
						break;
					case SGUN:
						Sshot();
						break;
					case HHGUN:
						HHshot();
						break;
					case HGUN:
						Hshot();
						break;
					}
					bulletindex--;
					if (bulletindex == 0)
					{
						style = DEFAULT;
					}
					delay = false;					// Z ��ư�� �� ������ ���� �� �߻�����ʰ� �ϱ����� delay�� false�� ��������ϴ�
				}
			}
			break;
		case 'X':
			UseBoom();
			break;
		case 'Q':
			if (page == 0)
			{
				if (QueenPlayer)
				{
					QueenPlayer = false;
					OnBGM(LOBBY);
				}
				else
				{
					QueenPlayer = true;
					OnBGM(LOBBY);
				}
			}
			break;
		case 'H':
			if (page == 0)
			{
				realhell = true;
				page = -1;
				Level = 3;
				hellstagecheck = 57;
				page = -1;			//case�� ���������ʴ� �ƹ� ��ȣ�� �ٲ���ϴ�
				AllStop = false;		// ��������
				SetTimer(hWndMain, 2, 1000, NULL);
				OnBGM(INGAME);
			}
			break;
		case 'R':
			if (page == 1)			// �̾��ϱ� ��ư
			{
				point -= 1000;
				php = 3;
				boomindex = 1;
				bulletindex = 0;
				page = -1;			//case�� ���������ʴ� �ƹ� ��ȣ�� �ٲ���ϴ�
				AllStop = false;		// ��������
				damage = true;
				style = DEFAULT;
				SetTimer(hWndMain, 3, 1000, NULL);		// 1�� ����
				SetTimer(hWndMain, 2, 1000, NULL);		// �������� ���� Ÿ�̸��Դϴ�
			}
			else if(page == 2)
			{
				point -= 1000;
				php = 3;
				page = -1;			//case�� ���������ʴ� �ƹ� ��ȣ�� �ٲ���ϴ�
				AllStop = false;		// ��������
				style = DEFAULT;
				SetTimer(hWndMain, 3, 1000, NULL);		// 1�� ����
				SetTimer(hWndMain, 2, 1000, NULL);		// �������� ���� Ÿ�̸��Դϴ�

				Reset();
				hellstagecheck = 57;
			}
			else if (page == 6)
			{
				point -= 1000;
				php = 3;
				page = -1;			//case�� ���������ʴ� �ƹ� ��ȣ�� �ٲ���ϴ�
				AllStop = false;		// ��������
				style = DEFAULT;
				SetTimer(hWndMain, 3, 1000, NULL);		// 1�� ����
				SetTimer(hWndMain, 2, 1000, NULL);		// �������� ���� Ÿ�̸��Դϴ�

				Reset();
				stagecheck = 62;
			}
			else if (page == 7)
			{
				point -= 1000;
				php = 3;
				page = -1;			//case�� ���������ʴ� �ƹ� ��ȣ�� �ٲ���ϴ�
				AllStop = false;		// ��������
				style = DEFAULT;
				SetTimer(hWndMain, 3, 1000, NULL);		// 1�� ����
				SetTimer(hWndMain, 2, 1000, NULL);		// �������� ���� Ÿ�̸��Դϴ�

				Reset();
				hellstagecheck = 57;
			}
			break;
		case VK_SHIFT:
			if (SPEED == 7)
			{
				SPEED = 4;
			}
			break;
		}
		break;
	case WM_KEYUP:
		switch (wParam)
		{
		case 'Z':
			if (!AllStop || !stop)
			{
				if (!delay)						// Z ��ư�� ���� ��
				{
					delay = true;				// delay�� true�� ���� �ٽ� �߻簡 �����ϰ� ��������ϴ�
				}
			}
			break;
		case VK_SHIFT:
			if (SPEED == 4)
			{
				SPEED = 7;
			}
			break;
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);	// ������ ������

		DrawBitmap(hdc, 0, 0, hBit);		// �޸𸮿� �̸� �׷��� �׸��� �ѹ��� �׸���

		EndPaint(hWnd, &ps);		// �ݳ��߽��ϴ�
		return 0;
	case WM_DESTROY:
		for (int i = 0; i < 12; i++)
		{
			KillTimer(hWnd, i);				// ��� Ÿ�̸� ����
		}
		PostQuitMessage(0);
		return 0;
	}

	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}