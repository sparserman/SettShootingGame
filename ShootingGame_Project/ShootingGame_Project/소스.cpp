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

#define MasX 0			// 왼쪽 벽
#define MaxX 680			// 오른쪽 벽
#define MasY 0				// 위 벽
#define MaxY 870			// 아래 벽		(최대 이동 가능 범위입니다)

#define BULLETSPEED 20		// 총알 속도입니다
#define ENEMYBULLETSPEED 5	// 적 총알 속도입니다

#define COUNTERATTACKGAGE 1000		// 반격기 쿨타임

#define MAXGRIT 1200		// 세트 W 강화

MCI_PLAY_PARMS playBgm;
MCI_OPEN_PARMS openBgm;
int dwID;

int px = 300, py = 700;		// 플레이어 위치입니다
int php = 3;			// 플레이어의 체력입니다
int SPEED = 7;				// 플레이어 이동 속도 입니다

int bgy = 0;		// 배경 위치입니다

HANDLE hThrd1, hThrd2;

HWND hWndMain, hDlgMain;

HBITMAP hBit;

// 비트맵을 전부 선언했습니다
HBITMAP PlayerBit, BackgroundBit, BulletBit, PistolBit, RifleBit[6], MachineGunBit, BossBit[6], EnemyBulletBit, LifeBit, PlayerDamagedBit, BossLifeBit, BossBulletBit, HpPotionBit
		, StartBit, EndBit[4], InstructionBit;

HBITMAP HellMopBit[4], HellInstructionBit, HellBackgroundBit, HellBulletBit, HellEndBit[3], KingBit[2], KingCreateBit, KingAttackBit[4], KingBulletBit, KingCounterAttackBit
		, KingLifeBit, ItemBit[3], NewBulletBit[3], BoomBit, LevelBit[3], WarningBit, MopDieBit[7], SBulletBit[8], BoomBit2, QueenPlayerBit[2], QueenPlayerBulletBit, QueenBoomBit
		, SettBit, SettBulletBit, SettWBit, SettWEfectBit, SettLifeBit, SettEBit, SettMax[3], RevengeBit[5];

TCHAR str[128];			// 스태틱 글자 바꾸기를 위함


int Level = 0;				// 게임의 난이도입니다

bool AllStop = true;		// 전체적인 정지 상태입니다
bool stop = true;			// 플레이어만 정지인 상태입니다

int player_frm = 0;			// 플레이어 애니메이션을 관리할 변수입니다
int pistol_frm = 0;			// 적의 대부분의 애니메이션을 관리할 변수입니다

int boss_frm = 0;			// 퀸 애니메이션을 관리할 변수입니다

int king_frm = 0;			// 킹 애니메이션을 관리할 변수입니다
int king_bullet_frm = 0;	// 킹 총알 애니메이션
int king_attack_frm = 0;	// 킹 반격기 애니메이션
int boom_frm = 0;			// 폭탄 애니메이션

int bulletcheck = 0;		// 총알을 반복해서 사용하기위해 만들었습니다
int enemybulletcheck = 0;	// 적 총알을 반복해서 사용하기위해 만들었습니다
int riflebulletcheck = 0;	// 소총병 총알을 반복해서 사용하기위해 만들었습니다
int bossbulletcheck = 0;	// 보스 총알 반복용입니다

int hellbulletcheck = 0;	// 보스 총알 반복용입니다

int pistolcheck = 0;		// 권총병을 반복해서 사용하기위해 만들었습니다
int riflecheck = 0;			// 소총
int machineguncheck = 0;	// 기관총
int itemcheck = 0;			// 아이템


int efectcheck = 0;	// 이펙트 체크

int mopcheck = 0;	// 몹

int stagecheck = 0;			// 스테이지에서 나오는 적을 컨트롤하려고 만들었습니다
int hellstagecheck = 0;		// 헬 난이도에서 나오는 적 컨트롤용

bool delay = true;				// 사격 버튼제어입니다	버튼을 꾹 누르고있을 때 총알이 나가게 하지않으려고 만들었습니다

bool damage = false;		// 연속 피격 방지

int page = 0;		// 시작화면이나 엔딩을 관리 할 변수입니다

int music = 0;		// 음악 상태
bool musicOn = true;	// 음악 켜짐 꺼짐

bool showqueen = true;			// 퀸 보일지 말지

int vibe = 0;		// 진동
int vibes = 10;		// 세트w 진동

bool kingcreate = true;		// 킹 한번만 소환용

enum PlayerStyle { DEFAULT = 0, SGUN, HHGUN, HGUN};
DWORD style = DEFAULT;			// 총 스타일

int bulletindex = 0;		// 남은 총알

int boomindex = 1;			// 남은 폭탄
bool boomcheck = false;			// 폭탄 사용상태
bool boomcheck2 = false;			// 폭탄 사용상태
int boom2 = 0;			// 진동용

bool showlevel = false;		// 레벨 표시

bool showwarning = false;		// warning 표시

int point = 0;		// 점수

bool QueenPlayer = false;		// 플레이어가 퀸인지
bool QueenBoom = false;		// 퀸 폭탄 상태인지

bool gritsound = true;		// 투지 소리 내는게 가능한지


bool realhell = false;		// 이상한 모드


struct Bullet			// 총알
{
	float x = 0, y = 0;					// 위치
	bool create = false;		// 생성 확인
	float random = 2;
};

struct NewBullet			// 새롭게 개편 된 불릿 구조체
{
	float x = 0, y = 0;					// 위치
	bool create = false;		// 생성 확인
	float sx = 0, sy = 0;		// 이동 방향
	HBITMAP bit = NULL;			// 총알생김새
};

struct Pistol			// 권총 든 적
{
	int hp = 30;					// 체력
	int x = 0, y = 0;					// 위치
	bool create = false;		// 생성 확인
	int dir = 0;				// 이동 방향
	int gage = 0;				// 총 쏘는 딜레이 게이지를 채우면 총알을 쏩니다
};


struct Rifle			// 소총 든 적
{
	int hp = 20;					// 체력
	int x = 0, y = 0;					// 위치
	bool create = false;		// 생성 확인
	bool hide = true;			// 숨은 상태인지 확인   숨은 상태는 총알에 맞지않게했습니다
	int gage = 0;				// 총 쏘는 딜레이 게이지를 채우면 총알을 쏩니다
};

struct MachineGun		// 기관총 든 적
{
	int hp = 80;					// 체력
	int x = 0, y = 0;					// 위치
	bool create = false;		// 생성 확인
	int gage = 0;				// 총 쏘는 딜레이 게이지를 채우면 총알을 쏩니다
	int dir = 0;				// 이동 방향
};

struct Boss				// 첫번째 보스
{
	int hp = 2000;				// 체력
	int x = 0, y = 0;					// 위치
	int state = 0;				// 상태
	int stance = 0;				// 총알 움직임을 결정 할 변수
	bool create = false;		// 생성 확인
	int dir = 0;				// 이동 방향
	int gage = 0;			// 패턴1
	int gage2 = 0;			// 패턴2
	int gage3 = 0;			// 패턴3
};


struct Mop			// 새롭게 개편 된 적 병사 구조체
{
	int hp = 0;					// 체력
	int x = 0, y = 0;			// 위치
	bool create = false;		// 생성 확인
	int dir = 0;				// 이동 방향
	int gage = 0;				// 총 쏘는 딜레이 게이지를 채우면 총알을 쏩니다
	int maxgage = 0;			// 게이지가 이 만큼 되면 발사
	int type = 0;				// 타입
};

struct King				// Hell 난이도 최종 보스
{
	int hp = 3000;				// 체력
	int x = 0, y = 0;					// 위치
	int state = 0;				// 상태
	bool create = false;		// 생성 확인
	int gage = 0;			// 패턴1
	int counter = 0;		// 반격게이지
};

struct Efect				// 이펙트
{
	int x = 0, y = 0;					// 위치
	bool create = false;		// 생성 확인
	HBITMAP bit = NULL;		// 비트맵 저장
	int frm = 0;		// 프레임
	DWORD maxfrm = 123;		// 최대 프레임
};

struct Sett				// 세트 보스 (특별판)
{
	int hp = 3000;				// 체력
	int x = 0, y = 0;					// 위치
	int grit = 0;			// 세트 투지
	bool create = false;		// 생성 확인
	int gage = 0;			// 패턴1
	bool show = true;		// 세트 보이게 할지말지
};

enum MOP {
	M_P = 0, M_C, M_S, M_T, KING1, KING2, KING3, KING4, KING5, QUEEN1, QUEEN2, QUEEN3, QUEEN4, QUEEN5
	, SETT1, SETT2, SETT3, SETT4, SETT5, SETT6, SETT7
};			// 적 종류와 패턴

enum MUSIC { LOBBY = 0, INGAME, CLEAR, HELL };			// 음악 종류


NewBullet bullet[100];				// 총알 선언
Bullet enemybullet[100];			// 적 총알 선언
Bullet riflebullet[5];			// 소총병 총알 선언
Bullet bossbullet[30];			// 보스 총알 선언

NewBullet hellbullet[100];		// 새롭게 개편 된 총알 (보스랑 구별 없음)

Bullet HpPotion;			// 회복약 선언		총알과 비슷한 방식이라 class를 그대로 사용했습니다

Pistol pistol[30];			// 권총병
Rifle rifle[10];			// 소총병
MachineGun machinegun[10];	// 기관총병
Boss boss;					// 보스

King king;					// 킹

Sett sett;					// 세트

Mop	mop[30];				// 개편 된 적

Efect efect[10];			// 이펙트

NewBullet item[5];			// 플레이어가 쓸 아이템

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


// 함수 선언
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


#pragma region Hell 이전 함수들

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

// 세트 W 사용
void UseSettW()
{
	HellBulletCreate(SETT3, 0);
	SetTimer(hWndMain, 10, 960, NULL);
}

// 세트 소환
void SettCreate()
{
	sett.create = true;		// 소환하며 초기화 작업입니다
	sett.gage = 0;
	sett.grit = 0;
	sett.hp = 3000;
	sett.x = 325;
	sett.y = 120;
}

// 세트 컨트롤
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

		if (sett.hp <= 0)			// 체력이 0 이하가 될 경우 제거
		{
			sett.create = false;
			OnBGM(CLEAR);
			AllStop = true;
			page = 3;
		}
		BossLifeBlt(hMemDC, 100, 55, SettLifeBit, RGB(255, 255, 255));		// 보스 체력 비트맵
	}
}

// 효과음
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

// S총알 생김새
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

// 모든 적과 총알 삭제 (폭탄 사용 시)
void MopClear(int m)
{
	// 적 총알
	for (int i = 0; i < 100; i++)
	{
		enemybullet[i].create = false;
	}
	// 소총병 총알
	for (int i = 0; i < 5; i++)
	{
		riflebullet[i].create = false;
	}
	// 보스1 총알
	for (int i = 0; i < 30; i++)
	{
		bossbullet[i].create = false;
	}
	// 헬 총알
	for (int i = 0; i < 100; i++)
	{
		hellbullet[i].create = false;
	}


	// 권총병
	for (int i = 0; i < 30; i++)
	{
		pistol[i].hp -= m;
	}
	// 소총병
	for (int i = 0; i < 10; i++)
	{
		rifle[i].hp -= m;
	}
	// 기관총병
	for (int i = 0; i < 10; i++)
	{
		machinegun[i].hp -= m;
	}
	for (int i = 0; i < 30; i++)
	{
		mop[i].hp -= m;
	}
}

// 폭탄 사용
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

// 남은 폭탄 그리기
void DrawBoomIndex(HDC hMemDC)
{
	HFONT font, oldfont;
	RECT rt;
	TCHAR str[30];

	SetRect(&rt, 650, 920, 800, 1000);

	wsprintf(str, "Boom : %d", boomindex);

	SetBkMode(hMemDC, TRANSPARENT);
	font = CreateFont(30, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 3, 2, 1,
		VARIABLE_PITCH | FF_ROMAN, "맑은 고딕");
	oldfont = (HFONT)SelectObject(hMemDC, font);

	SetTextColor(hMemDC, RGB(255, 255, 255));		// 글자 색
	DrawText(hMemDC, str, -1, &rt, DT_WORDBREAK);
	DeleteObject(SelectObject(hMemDC, oldfont));
}

// 점수 그리기
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
		VARIABLE_PITCH | FF_ROMAN, "맑은 고딕");
	oldfont = (HFONT)SelectObject(hMemDC, font);

	SetTextColor(hMemDC, RGB(255, 255, 255));		// 글자 색
	DrawText(hMemDC, str, -1, &rt, DT_WORDBREAK);
	DeleteObject(SelectObject(hMemDC, oldfont));
}

// 남은 총알 그리기
void DrawBulletIndex(HDC hMemDC)
{
	HFONT font, oldfont;
	RECT rt;
	TCHAR str[30];

	SetRect(&rt, 650, 900, 800, 1000);

	wsprintf(str, "Bullet : %d", bulletindex);

	SetBkMode(hMemDC, TRANSPARENT);
	font = CreateFont(30, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 3, 2, 1,
		VARIABLE_PITCH | FF_ROMAN, "맑은 고딕");
	oldfont = (HFONT)SelectObject(hMemDC, font);

	SetTextColor(hMemDC, RGB(255, 255, 255));		// 글자 색
	DrawText(hMemDC, str, -1, &rt, DT_WORDBREAK);
	DeleteObject(SelectObject(hMemDC, oldfont));
}

// 총 별 사격
void Sshot()
{
	if (QueenPlayer)
	{
		if (QueenBoom)
		{
			for (int i = 0; i < 3; i++)
			{
				int rnd = rand() % 20 - 10;
				bullet[bulletcheck].create = true;				// 총알을 생성하게 했습니다
				bullet[bulletcheck].x = px + 38;			// 처음에 발사 되는 순간에는 플레이어 위치에 맞췄습니다
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
				bullet[bulletcheck].create = true;				// 총알을 생성하게 했습니다
				bullet[bulletcheck].x = px + 38;			// 처음에 발사 되는 순간에는 플레이어 위치에 맞췄습니다
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
				bullet[bulletcheck].create = true;				// 총알을 생성하게 했습니다
				bullet[bulletcheck].x = px + 38;			// 처음에 발사 되는 순간에는 플레이어 위치에 맞췄습니다
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
				bullet[bulletcheck].create = true;				// 총알을 생성하게 했습니다
				bullet[bulletcheck].x = px + 38;			// 처음에 발사 되는 순간에는 플레이어 위치에 맞췄습니다
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
				bullet[bulletcheck].create = true;				// 총알을 생성하게 했습니다
				bullet[bulletcheck].x = px + 38 + (i * 20);			// 처음에 발사 되는 순간에는 플레이어 위치에 맞췄습니다
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
		bullet[bulletcheck].create = true;				// 총알을 생성하게 했습니다
		bullet[bulletcheck].x = px + 72;			// 처음에 발사 되는 순간에는 플레이어 위치에 맞췄습니다
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

// 아이템 생성
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

// 킹의 퀸 보호패턴	사용안함
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

// 퀸 컨트롤
void QueenControl(HDC hMemDC)
{
	if (boss.create)
	{
		/*
		if (king.create)
		{
			boss.gage++;
			if (boss.gage % 50 == 0)		// 패턴 1
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
			if (boss.gage == 100 || boss.gage == 300 || boss.gage == 650 || boss.gage == 900)		// 패턴 1
			{
				EfectCreate(boss.x, boss.y, BossBit[1], 4);
				HellBulletCreate(QUEEN1, 0);
			}
			if (boss.gage == 250 || boss.gage == 600)				// 패턴 2
			{
				EfectCreate(boss.x, boss.y, BossBit[1], 4);
				HellBulletCreate(QUEEN2, 0);
			}

			if (boss.gage % 400 == 0 && !realhell)
			{
				HellBulletCreate(QUEEN5, 0);
			}

			if (boss.hp >= 1200)		// 체력이 적을 경우 패턴 변경
			{
				if (boss.gage == 400 || boss.gage == 700)
				{
					boss.state = 3;
				}
				if ((boss.gage >= 400 && boss.gage <= 550) || (boss.gage >= 700 && boss.gage <= 750))				// 패턴 3
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
				if ((boss.gage >= 400 && boss.gage <= 550) || (boss.gage >= 700 && boss.gage <= 750))				// 패턴 3
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

		if (boss.hp <= 0)			// 체력이 0 이하가 될 경우 제거
		{
			boss.create = false;
			AllStop = true;
			page = 3;
			OnBGM(CLEAR);
		}
		BossLifeBlt(hMemDC, 100, 30, BossLifeBit, RGB(255, 255, 255));		// 보스 체력 비트맵
	}
	if (!boss.create)
	{
		boss.hp = 2000;
	}
}

// 킹 이동
void KingMove(int x, int y)
{
	king.x = x;
	king.y = y;
	king.counter = COUNTERATTACKGAGE;
	EfectCreate(king.x, king.y, KingCreateBit, 4);
}

// 이펙트 컨트롤
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

// 이펙트 생성
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

// 킹 반격기 이펙트
void CounterAttack(HDC hMemDC)
{
	if (!realhell)
	{
		py = 850;
	}
	MoveTransBlt(hMemDC, 0, 0, KingCounterAttackBit, RGB(255, 255, 255), king_attack_frm);
	/*for (int i = 0; i < 100; i++)		// 플레이어 총알 없애기
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

// 킹 컨트롤
void KingControl(HDC hMemDC)
{
	if (king.create)
	{
		if (king.hp >= 1500)
		{
			king.gage++;
			if (king.gage == 300 || king.gage == 720 || king.gage == 780)			// 패턴 2
			{
				HellBulletCreate(KING2, 0);
				EfectCreate(king.x, king.y, KingAttackBit[0], 5);
			}
			if (king.gage == 320 || king.gage == 750)			// 패턴 1
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

			if (king.gage == 780)		// 패턴 반복
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

			if (king.gage == 1000)		// 패턴 반복
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

			if (king.gage == 1000)		// 패턴 반복
			{
				king.gage = 0;
			}
		}

		//QueenSafe(hMemDC);		// 퀸 지키는 패턴

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
		case 1:		// 반격자세
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

		if (king.hp <= 0)			// 체력이 0 이하가 될 경우 제거
		{
			king.create = false;
			Level++;
		}
		BossLifeBlt(hMemDC, 100, 40, KingLifeBit, RGB(255, 255, 255));		// 보스 체력 비트맵
	}
}

// 킹 소환
void KingCreate()
{
	king.create = true;		// 소환하며 초기화 작업입니다
	king.hp = 3000;
	king.gage = 0;
	king.x = 325;
	king.y = 120;
	king.counter = COUNTERATTACKGAGE;
	EfectCreate(king.x, king.y, KingCreateBit, 4);
}

// 초기화 작업
void Reset()
{
	for (int i = 0; i < 30; i++)
	{
		pistol[i].create = false;		// 적도 없애고
	}
	for (int i = 0; i < 10; i++)	// 게임을 처음부터 하기위한 초기화 작업입니다
	{
		rifle[i].create = false;
		machinegun[i].create = false;
	}
	for (int i = 0; i < 100; i++)		// 총알도 없애고
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
	HpPotion.create = false;			// 회복약도 없애고
	boss.create = false;			// 보스도 없애고
	king.create = false;			// 보스도 없애고
	sett.create = false;			
	stagecheck = 0;					// stage도 처음부터 시작하고
	hellstagecheck = 0;					// stage도 처음부터 시작하고
	px = 300;			// 플레이어 위치도 초기화
	py = 700;
	php = 3;				// 플레이어 체력도 3으로 채워줬습니다
	boomindex = 1;			// 폭탄 1개로 초기화
	bulletindex = 0;		// 총알 초기화
}

// BGM
void OnBGM(int num)
{
	if (!musicOn)
	{
		return;
	}

	mciSendCommand(dwID, MCI_CLOSE, MCI_NOTIFY, (DWORD)(LPVOID)&openBgm); // 일시정지 PAUSE, 완전 제거 CLOSE

	openBgm.lpstrDeviceType = "mpegvideo";// wave , "mpegvideo"; //mp3 형식
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

// 헬 불릿 check 컨트롤
void HellBulletCheckControl()
{
	if (hellbulletcheck != 99)							// 최대 총알 개수에 도달하면 0으로 되돌렸습니다
	{
		hellbulletcheck++;
	}
	else
	{
		hellbulletcheck = 0;
	}
}

// 헬 총알 생성
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
			hellbullet[hellbulletcheck].x = mop[_i].x + 10;			// 처음에 발사는 적의 위치에 맞춤
			hellbullet[hellbulletcheck].y = mop[_i].y + 60;
			hellbullet[hellbulletcheck].bit = HellBulletBit;

			hellbullet[hellbulletcheck].sx = -4 + (i * 2);
			hellbullet[hellbulletcheck].sy = ENEMYBULLETSPEED;
			HellBulletCheckControl();
		}
		break;
	case M_P:
		hellbullet[hellbulletcheck].create = true;
		hellbullet[hellbulletcheck].x = mop[_i].x + 10;			// 처음에 발사는 적의 위치에 맞춤
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
			hellbullet[hellbulletcheck].x = mop[_i].x + 20;		// 처음에 발사는 적의 위치에 맞춤
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
	case KING1:				// 일자 공격
		hellbullet[hellbulletcheck].sy = 10;
		hellbullet[hellbulletcheck].create = true;
		hellbullet[hellbulletcheck].x = king.x + 25;
		hellbullet[hellbulletcheck].y = king.y + 140;
		hellbullet[hellbulletcheck].bit = KingBulletBit;

		hellbullet[hellbulletcheck].sx = 0;
		HellBulletCheckControl();
		break;
	case KING2:					// 양갈래 공격
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
	case KING3:				// 반격 후 공격
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
	case KING4:				// 흩뿌리기1
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
	case KING5:				// 흩뿌리기2
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
	case QUEEN1:				// 양갈래 사격
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
	case QUEEN2:				// 3갈래 사격
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
	case QUEEN3:				// 랜덤 스핀
		random = (float)(rand() % 10) - 5;

		hellbullet[hellbulletcheck].sy = 15;
		hellbullet[hellbulletcheck].create = true;
		hellbullet[hellbulletcheck].x = boss.x + 40;
		hellbullet[hellbulletcheck].y = boss.y + 120;
		hellbullet[hellbulletcheck].bit = BossBulletBit;
		hellbullet[hellbulletcheck].sx = random;
		HellBulletCheckControl();
		break;
	case QUEEN4:				// 게틀링
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

// 몹 사격
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

// 몹 컨트롤
void HellMopControl(HDC hMemDC)
{
	for (int i = 0; i < 30; i++)			// 적을 30명까지 확인
	{
		if (mop[i].create)				// 생성되어있다면
		{
			mop[i].y++;					// 그 적을 배경과 같은 속도로 내려가게 했습니다

			if (mop[i].x < 0)			// 벽에 부딪힐 경우 반대방향으로 가게 했습니다
			{
				mop[i].dir = 1;
			}
			else if (mop[i].x > 700)
			{
				mop[i].dir = 0;
			}

			if (mop[i].dir == 0)		// dir에 따라 이동방향이 바뀝니다
			{
				mop[i].x--;
			}
			else if (mop[i].dir == 1)
			{
				mop[i].x++;
			}
			MoveTransBlt(hMemDC, mop[i].x, mop[i].y, HellMopBit[mop[i].type], RGB(76, 76, 76), pistol_frm);		// 타입에 맞게 그리기

			if (mop[i].y >= 1000)			// 만약 하단 밖으로 나가면
			{
				mop[i].create = false;		// 사라지게 했습니다
			}
			if (mop[i].hp <= 0)				// hp가 0 이하가 되면
			{
				mop[i].create = false;		// 사라지게 했습니다
				EfectCreate(mop[i].x, mop[i].y, MopDieBit[mop[i].type + 3], 4);
				point += (mop[i].type * 100) + 150;
			}
		}
	}
}

// 몹 생성
void HellMopCreate(int x, int type, int dir)
{
	mop[mopcheck].y = -100;		// 윈도우 밖에서부터 생성이 되게 했습니다
	mop[mopcheck].x = x;			// x는 원하는 위치에 생성 할 수 있도록 해놨습니다
	mop[mopcheck].create = true;	// 생성상태로 바꾸었습니다
	mop[mopcheck].dir = dir;		// 방향 설정가능
	mop[mopcheck].type = type;
	switch (type)
	{
	case M_P:
		mop[mopcheck].hp = 40;					// 생성하며 hp 초기화
		mop[mopcheck].maxgage = 75;			// 사격 딜레이 초기화
		break;
	case M_C:
		mop[mopcheck].hp = 90;					// 생성하며 hp 초기화
		mop[mopcheck].maxgage = 250;			// 사격 딜레이 초기화
		break;
	case M_S:
		mop[mopcheck].hp = 120;					// 생성하며 hp 초기화
		mop[mopcheck].maxgage = 120;			// 사격 딜레이 초기화
		break;
	case M_T:
		mop[mopcheck].hp = 300;					// 생성하며 hp 초기화
		mop[mopcheck].maxgage = 300;			// 사격 딜레이 초기화
		break;
	}

	if (mopcheck != 29)				// 29번 생성 이후로는 다시 0부터
	{
		mopcheck++;
	}
	else
	{
		mopcheck = 0;					// 0으로 되돌려서 다시 배열의 처음부터 생성하도록 했습니다
	}
}

// 헬 총알 컨트롤
void HellBulletControl(HDC hMemDC)
{
	for (int i = 0; i < 100; i++)
	{
		if (hellbullet[i].create)
		{
			hellbullet[i].y += hellbullet[i].sy;			// 이동 방향
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
		if (hellbullet[i].y >= 1000)		// 밖으로 빠지면 삭제
		{
			hellbullet[i].create = false;
		}
	}

	for (int i = 0; i < 100; i++)
	{
		if (bullet[i].create)
		{
			bullet[i].y -= bullet[i].sy;			// 이동 방향
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
		if (hellbullet[i].y <= -20)		// 밖으로 빠지면 삭제
		{
			hellbullet[i].create = false;
		}
	}
}

// 회복약 생성
void HpPotionCreate(int x)
{
	HpPotion.create = true;		// 생성하고
	HpPotion.x = x;				// HpPotion.x를 받은 x값으로 변경했습니다
	HpPotion.y = -100;			// 안보이는 곳에서 생성
}

// 회복약 및 아이템 컨트롤
void HpPotionControl(HDC hMemDC)
{
	if (HpPotion.create)			// 생성 되어있을 시
	{
		HpPotion.y++;				// 배경속도와 같게 내려갑니다
		TransBlt(hMemDC, HpPotion.x, HpPotion.y, HpPotionBit, RGB(255, 255, 255));
	}
	
	if (HpPotion.y >= 1000)				// 아래로 넘어가면 사라집니다
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

// 보스 총알 컨트롤 3
void BossBulletControl3(HDC hMemDC)
{
	for (int i = 0; i < 30; i++)
	{
		if (bossbullet[i].create)
		{
			bossbullet[i].y += 16;				// 내려가고
			bossbullet[i].x += bossbullet[i].random / 3;	// 대각석이동을 위해 랜덤으로 좌우 이동을 했습니다

			TransBlt(hMemDC, bossbullet[i].x, bossbullet[i].y, BossBulletBit, RGB(255, 255, 255));
		}
		if (bossbullet[i].y >= 1000)		// 밖으로 빠지면 삭제
		{
			bossbullet[i].create = false;
		}
	}
}

// 보스 총알 컨트롤 2
void BossBulletControl2(HDC hMemDC)
{
	for (int i = 0; i < 30; i++)
	{
		if (bossbullet[i].create)
		{
			bossbullet[i].y += 2;		// 내려가고
			bossbullet[i].x += bossbullet[i].random / 20;	// 대각석이동을 위해 랜덤으로 좌우 이동을 했습니다
			
			TransBlt(hMemDC, bossbullet[i].x, bossbullet[i].y, BossBulletBit, RGB(255, 255, 255));
		}
		if (bossbullet[i].y >= 1000)	// 밖으로 빠지면 삭제
		{
			bossbullet[i].create = false;
		}
	}
}

// 보스 총알 컨트롤
void BossBulletControl(HDC hMemDC)
{
	for (int i = 0; i < 30; i++)
	{
		if (bossbullet[i].create)
		{
			bossbullet[i].y += 12;			// 내려가고 총알의 번호에 따라서 좌우로 나아가게했습니다
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
		if (bossbullet[i].y >= 1000)		// 배경밖으로 나가면 삭제
		{
			bossbullet[i].create = false;
		}
	}
}

// 보스 사격 (패턴1)
void BossShot()
{
	boss.stance = 0;
	boss_frm = 0;		// 보스 프레임 초기화
	for (int i = 0; i < 3; i++)
	{
		bossbullet[bossbulletcheck].create = true;
		if (bossbulletcheck != 29)							// 최대 총알 개수에 도달하면 0으로 되돌렸습니다
		{
			bossbullet[bossbulletcheck].x = boss.x + 40;		// 총알이 나오는 위치입니다
			bossbullet[bossbulletcheck].y = boss.y + 120;
			bossbulletcheck++;
		}
		else
		{
			bossbullet[bossbulletcheck].x = boss.x + 40;
			bossbullet[bossbulletcheck].y = boss.y + 120;
			bossbulletcheck = 0;			// 아닐경우 0부터 다시 생성
		}
	}
	//PlaySound(MAKEINTRESOURCE(IDR_WAVE1), g_hInst, SND_RESOURCE | SND_ASYNC);
}

void BossShoting()
{
	for (int i = 0; i < 5; i++)
	{
		hellbullet[hellbulletcheck].create = true;
		hellbullet[hellbulletcheck].x = boss.x - 65 + i * 70;		// 총알이 나오는 위치입니다
		hellbullet[hellbulletcheck].y = boss.y + 120;
		hellbullet[hellbulletcheck].bit = BossBulletBit;
		hellbullet[hellbulletcheck].sy = 12;
		if (hellbulletcheck != 99)							// 최대 총알 개수에 도달하면 0으로 되돌렸습니다
		{
			hellbulletcheck++;
		}
		else
		{
			hellbulletcheck = 0;			// 아닐경우 0부터 다시 생성
		}
	}
	//PlaySound(MAKEINTRESOURCE(IDR_WAVE1), g_hInst, SND_RESOURCE | SND_ASYNC);
}

// 보스 스핀 사격2
void BossShot2()
{
	bossbullet[bossbulletcheck].random = rand() % 100 - 50;		// 총알이 좌우로 얼마나 이동 할 지 랜덤으로 부여했습니다

	bossbullet[bossbulletcheck].create = true;
	if (bossbulletcheck != 29)
	{
		bossbullet[bossbulletcheck].x = boss.x + 40;			// 나머진 위와 비슷합니다
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

// 보스 사격 3
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

// 보스 컨트롤 함수
void BossControl(HDC hMemDC)
{
	if (boss.create)
	{
		boss.gage++;
		if (boss.gage >= 200 && boss.state == 0)		// 게이지가 전부 차고, 상태가 기본상태라면 행동합니다
		{
			BossShot();
			boss.state = 1;
			boss.gage = 0;
		}

		if (boss.gage2 % 300 == 0)
		{
			BossShoting();
		}
		
		if (boss.gage2 < 1100 && boss.state != 2)		// 게이지가 1100 보다 커질 수 없게했습니다
		{
			boss.gage2++;
		}
		else if (boss.gage > 0)
		{
			boss.gage2--;
		}
		if (boss.gage2 >= 1100 || boss.state == 2)				// 게이지2가 전부 찼는데
		{
			if (boss.state == 0 || boss.state == 2)			// 다른 동작 중엔 안하게했습니다
			{
				if (boss.hp >= 1600)			// 보스 체력이 1600 이상이라면
				{
					BossShot();				// 패턴 1을 했습니다
					boss.state = 1;
				}
				else						// 아니라면
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
						BossShot3();			// 패턴 3을 했습니다
					}
				}
			}
		}

		if (boss.hp >= 1000)
		{
			if (boss.gage3 < 500 && boss.state != 3)		// 게이지를 다 채우면 정지하고
			{
				boss.gage3++;
			}
			else if (boss.gage >= 0)				// 깎기 시작
			{
				boss.gage3 -= 10;
			}

			if (boss.gage3 == 500 || boss.state == 3)			// 게이지가 차면
			{
				if (boss.state == 0 || boss.state == 3)		// 다른 동작 중엔 안하게했습니다
				{
					if (boss.state == 0)		// 상태가 기본일 때만
					{
						if (boss.state != 3)		// 상태 회전으로 변경
						{
							boss.stance = 1;	// 총알 컨트롤 변경
							boss_frm = 0;		// 보스 프레임 초기화
							SetTimer(hWndMain, 4, 3000, NULL);			// 3초후 회전 중지와 상태 기본으로 변경
						}
					}
					if (boss.gage3 % 50 == 0 && boss.gage3 >= 0)	// 게이지가 깎기면서 50의 배수일때마다 총알 발사
					{
						BossShot2();
					}
					boss.state = 3;
				}
			}
		}
		else
		{
			if (boss.gage3 < 500 && boss.state != 4)		// 게이지를 다 채우면 정지하고
			{
				boss.gage3++;
			}
			else if (boss.gage >= 0)				// 깎기 시작
			{
				boss.gage3 -= 10;
			}

			if (boss.gage3 == 500 || boss.state == 4)			// 게이지가 차면
			{
				if (boss.state == 0 || boss.state == 4)
				{
					if (boss.state == 0)		// 상태가 기본일 때만
					{
						if (boss.state != 4)		// 상태 회전으로 변경
						{
							boss.stance = 0;	// 총알 컨트롤 변경
							boss_frm = 0;		// 보스 프레임 초기화
							SetTimer(hWndMain, 4, 3000, NULL);			// 3초후 회전 중지와 상태 기본으로 변경
						}
					}
					if (boss.gage3 % 50 == 0 && boss.gage3 >= -500)	// 게이지가 깎기면서 50의 배수일때마다 총알 발사
					{
						BossShot2();
					}
					boss.state = 4;
				}
			}
		}


		switch (boss.state)			// state에 따라 비트맵을 다르게 그렸습니다
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
			if (boss.dir == 0)				// dir에 맞게 이동했습니다
			{
				boss.x += 10;
			}
			else
			{
				boss.x -= 10;
			}
			break;
		}



		if (boss.hp <= 0)			// 체력이 0 이하가 될 경우 제거
		{
			boss.create = false;
			Level++;
		}
		BossLifeBlt(hMemDC, 100, 30, BossLifeBit, RGB(255, 255, 255));		// 보스 체력 비트맵
	}
	if (!boss.create)
	{
		boss.hp = 2000;
	}
}

// 보스 소환
void BossCreate()
{
	boss.create = true;		// 소환하며 초기화 작업입니다
	boss.gage = 0;
	boss.gage2 = 0;
	boss.gage3 = 0;
	boss.state = 0;
	boss.hp = 100;
	boss.x = 325;
	boss.y = 80;
}

// 보스의 체력을 그리는 함수입니다
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

// 생명을 그리는 함수입니다
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

// 적의 발사
void EnemyShot()
{
	for (int i = 0; i < 30; i++)
	{
		if (pistol[i].create)
		{
			pistol[i].gage++;
			if (pistol[i].gage == 100)
			{
				enemybullet[enemybulletcheck].create = true;				// 총알을 생성하게 했습니다
				if (enemybulletcheck != 99)							// 최대 총알 개수에 도달하면 0으로 되돌렸습니다
				{
					enemybullet[enemybulletcheck].x = pistol[i].x + 20;			// 처음에 발사 되는 순간에는 권총병 위치에 맞췄습니다
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
		//소총
		if (rifle[i].create)							// 전부 위와 같은 방식으로 남은 적을 처리했습니다
		{
			rifle[i].gage++;
			if (rifle[i].gage >= 220 && rifle[i].y <= py + 30 && rifle[i].y >= py - 10)			// 플레이어와 Y값이 비슷해졌고, 게이지가 전부 찼다면
			{
				rifle[i].hide = false;								// 숨은 상태가 풀리고
				riflebullet[riflebulletcheck].create = true;		// 발사
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
		//기관총
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

// 적 피격
DWORD CALLBACK EnemyDamaged(LPVOID n)
{
	while (1)
	{
		if (!AllStop)
		{
			for (int i = 0; i < 100; i++)
			{
				if (bullet[i].create)			// 생성된 총알만
				{
					for (int j = 0; j < 30; j++)
					{
						if (pistol[j].create)		// 생성된 적만
						{
							if (CheckPtInRect(BulletBit, bullet[i].x, bullet[i].y, PistolBit, pistol[j].x, pistol[j].y))		// 충돌 확인 후
							{
								bullet[i].create = false;			// 총알은 없애고
								pistol[j].hp -= 10;				// 체력은 -10
								if (damage)
								{
									pistol[j].hp -= 10;			// 플레이어 무적 땐 2배 피해
								}
							}
						}
					}

					for (int j = 0; j < 10; j++)
					{

						if (rifle[j].create)		// 생성된 적만
						{
							if (rifle[j].hide == false)		// 숨은 상태면 피격무효
							{
								if (CheckPtInRect(BulletBit, bullet[i].x, bullet[i].y, RifleBit[0], rifle[j].x, rifle[j].y))		// 충돌 확인 후
								{
									bullet[i].create = false;			// 총알은 없애고
									rifle[j].hp -= 10;				// 체력은 -10
									if (damage)
									{
										rifle[j].hp -= 10;			// 플레이어 무적 땐 2배 피해
									}
								}
							}
						}


						if (machinegun[j].create)		// 생성된 적만
						{
							if (CheckPtInRect(BulletBit, bullet[i].x, bullet[i].y, MachineGunBit, machinegun[j].x, machinegun[j].y))		// 충돌 확인 후
							{
								bullet[i].create = false;			// 총알은 없애고
								machinegun[j].hp -= 10;				// 체력은 -10
								if (damage)
								{
									machinegun[j].hp -= 10;			// 플레이어 무적 땐 2배 피해
								}
							}
						}
					}

					if (boss.create)		// 보스가 생성 되있을 시
					{
						if (CheckPtInRect(BulletBit, bullet[i].x, bullet[i].y, BossBit[0], boss.x, boss.y))		// 충돌 확인 후
						{
							bullet[i].create = false;			// 총알은 없애고
							boss.hp -= 5;				// 체력은 -5
							if (damage)
							{
								boss.hp -= 5;			// 플레이어 무적 땐 2배 피해
							}
						}
					}

					if (king.create)		// 킹이 생성 되있을 시
					{
						if (CheckPtInRect(BulletBit, bullet[i].x, bullet[i].y, BossBit[0], king.x, king.y))		// 충돌 확인 후
						{
							bullet[i].create = false;			// 총알은 없애고

							king.hp -= 5;				// 체력은 -5
							if (damage)
							{
								king.hp -= 5;			// 플레이어 무적 땐 2배 피해
							}

						}
					}

					if (sett.create)		// 세트가 생성 되있을 시
					{
						if (CheckPtInRect(BulletBit, bullet[i].x, bullet[i].y, SettBit, sett.x, sett.y))		// 충돌 확인 후
						{
							bullet[i].create = false;			// 총알은 없애고

							sett.hp -= 5;				// 체력은 -5
							if (sett.show)
							{
								sett.grit += 5;
							}

						}
					}

					for (int j = 0; j < 30; j++)
					{
						if (mop[j].create)		// 생성된 적만
						{
							if (CheckPtInRect(BulletBit, bullet[i].x, bullet[i].y, HellMopBit[mop[j].type], mop[j].x, mop[j].y))		// 충돌 확인 후
							{
								bullet[i].create = false;			// 총알은 없애고
								mop[j].hp -= 10;				// 체력은 -10
								if (damage)
								{
									mop[j].hp -= 10;			// 플레이어 무적 땐 2배 피해
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

// 플레이어 피격
DWORD CALLBACK Damaged(LPVOID n)
{
	while (1)
	{
		if (!AllStop)
		{
			for (int i = 0; i < 100; i++)
			{
				if (enemybullet[i].create)			// 생성된 총알만
				{
					if (CheckPtInRect(EnemyBulletBit, enemybullet[i].x, enemybullet[i].y, PlayerBit, px, py))		// 충돌 확인 후
					{
						if (!damage)
						{
							enemybullet[i].create = false;			// 총알은 없애고
							php--;			// 플레이어의 체력은 -1
							boomindex++;	// 폭탄을 얻기
							damage = true;
							SetTimer(hWndMain, 3, 2000, NULL);
							Music(3);
						}
					}
				}
			}

			for (int i = 0; i < 5; i++)
			{
				if (riflebullet[i].create)			// 생성된 총알만
				{
					if (CheckPtInRect(EnemyBulletBit, riflebullet[i].x, riflebullet[i].y, PlayerBit, px, py))		// 충돌 확인 후
					{
						if (!damage)
						{
							riflebullet[i].create = false;			// 총알은 없애고
							php--;			// 플레이어의 체력은 -1
							boomindex++;	// 폭탄을 얻기
							damage = true;
							SetTimer(hWndMain, 3, 2000, NULL);
							Music(3);
						}
					}
				}
			}

			for (int i = 0; i < 50; i++)		// 보스 총알
			{
				if (bossbullet[i].create)			// 생성된 총알만
				{
					if (CheckPtInRect(BossBulletBit, bossbullet[i].x, bossbullet[i].y, PlayerBit, px, py))		// 충돌 확인 후
					{
						if (!damage)
						{
							bossbullet[i].create = false;			// 총알은 없애고
							php--;			// 플레이어의 체력은 -1
							boomindex++;	// 폭탄을 얻기
							damage = true;
							SetTimer(hWndMain, 3, 2000, NULL);
							Music(3);
						}
					}
				}
			}

			for (int i = 0; i < 30; i++)			// 권총병과 충돌
			{
				if (pistol[i].create)
				{
					if (CheckPtInRect(PistolBit, pistol[i].x, pistol[i].y, PlayerBit, px, py))
					{
						if (!damage)
						{
							php--;
							boomindex++;	// 폭탄을 얻기
							damage = true;
							SetTimer(hWndMain, 3, 2000, NULL);
							Music(3);
						}
					}
				}
			}

			for (int i = 0; i < 10; i++)			// 기관총병과 충돌
			{
				if (machinegun[i].create)
				{
					if (CheckPtInRect(MachineGunBit, machinegun[i].x, machinegun[i].y, PlayerBit, px, py))
					{
						if (!damage)
						{
							php--;
							boomindex++;	// 폭탄을 얻기
							damage = true;
							SetTimer(hWndMain, 3, 2000, NULL);
							Music(3);
						}
					}
				}
			}

			if (boss.create)		// 보스와 충돌
			{
				if (CheckPtInRect(BossBit[0], boss.x, boss.y, PlayerBit, px, py))
				{
					if (!damage)
					{
						php--;
						boomindex++;	// 폭탄을 얻기
						damage = true;
						SetTimer(hWndMain, 3, 2000, NULL);
						Music(3);
					}
				}
			}

			if (king.create)		// 보스와 충돌
			{
				if (CheckPtInRect(KingBit[0], king.x, king.y, PlayerBit, px, py))
				{
					if (!damage)
					{
						php--;
						boomindex++;	// 폭탄을 얻기
						damage = true;
						SetTimer(hWndMain, 3, 2000, NULL);
						Music(3);
					}
				}
			}

			if (sett.create)		// 보스와 충돌
			{
				if (CheckPtInRect(SettBit, sett.x, sett.y, PlayerBit, px, py))
				{
					if (!damage)
					{
						php--;
						boomindex++;	// 폭탄을 얻기
						damage = true;
						SetTimer(hWndMain, 3, 2000, NULL);
						Music(3);
					}
				}
			}

			if (HpPotion.create)		// 회복약과 충돌
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

			for (int i = 0; i < 100; i++)		// 헬 총알
			{
				if (hellbullet[i].create)			// 생성된 헬 총알만
				{
					if (CheckPtInRect(hellbullet[i].bit, hellbullet[i].x, hellbullet[i].y, PlayerBit, px, py))		// 충돌 확인 후
					{
						if (!damage)
						{
							hellbullet[i].create = false;			// 총알은 없애고
							php--;			// 플레이어의 체력은 -1
							boomindex++;	// 폭탄을 얻기
							damage = true;
							SetTimer(hWndMain, 3, 2000, NULL);
							Music(3);
						}
					}
				}
			}

			for (int i = 0; i < 5; i++)			// 아이템과 충돌
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

// 사각형 충돌
BOOL CheckPtInRect(HBITMAP hBit, int Obitx, int Obity, HBITMAP Pbit, int Pbitx, int Pbity) //저장할 좌표, 위치, 비트맵, 비트맵 위치
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

	if (Pbit == PistolBit || Pbit == MachineGunBit || Pbit == HellMopBit[0] || Pbit == HellMopBit[1] || Pbit == HellMopBit[2] || Pbit == HellMopBit[3])				// 비트맵 별 충돌범위를 다르게 하기 위해 if문을 썼습니다
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

// 권총병 소환
void PistolCreate(int x, int dir)
{
	if (pistolcheck != 29)				// 9번 이상 생성했을 때는 class 배열이 10개이므로 다시 0부터 사용하게 했습니다
	{
		pistol[pistolcheck].y = -100;		// 윈도우 밖에서부터 생성이 되게 했습니다
		pistol[pistolcheck].x = x;			// x는 원하는 위치에 생성 할 수 있도록 해놨습니다
		pistol[pistolcheck].create = true;	// 생성상태로 바꾸었습니다
		pistol[pistolcheck].dir = dir;
		pistol[pistolcheck].hp = 30;					// 생성하며 hp 초기화
		pistolcheck++;		// 다음번엔 다음 배열에 넣기위해 +1을 해주었습니다
	}
	else
	{
		pistol[pistolcheck].y = -100;
		pistol[pistolcheck].x = x;
		pistol[pistolcheck].create = true;
		pistol[pistolcheck].hp = 30;
		pistol[pistolcheck].dir = dir;
		pistolcheck = 0;					// 0으로 되돌려서 다시 배열의 처음부터 생성하도록 했습니다
	}
}

// 소총병 소환
void RifleCreate(int x)					// 위와 같습니다
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

// 기관총병 소환
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

// 스테이지 1
void Stage1()										// 스테이지를 관리하는 함수입니다
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

// 스테이지 2
void Stage2()													// 스테이지를 관리하는 함수입니다
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

// 스테이지 Hell
void HellStage()													// 스테이지를 관리하는 함수입니다
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

// 적의 총알을 컨트롤 해주는 함수
void EnemyBulletControl(HDC hMemDC)
{
	for (int i = 0; i < 100; i++)			// Bullet을 100까지 확인해서
	{
		if (enemybullet[i].create)				// 생성되어있는 총알이라면
		{
			enemybullet[i].y += ENEMYBULLETSPEED;				// 그 총알을 아래로 나아가게 했습니다
			TransBlt(hMemDC, enemybullet[i].x, enemybullet[i].y, EnemyBulletBit, RGB(255, 255, 255));		// 총알을 그려주었고요
		}
		if (enemybullet[i].y >= 1000)				// 만약 상단 밖으로 나가면
		{
			enemybullet[i].create = false;		// 사라지게 했습니다
		}
	}
}

// 소총병 총알을 컨트롤 해주는 함수
void RifleBulletControl(HDC hMemDC)		// 소총병 전용 총알 방향과 속도
{
	for (int i = 0; i < 5; i++)			// Bullet을 5까지 확인해서
	{
		if (riflebullet[i].create)				// 생성되어있는 총알이라면
		{
			riflebullet[i].x += 20;				// 그 총알을 오른쪽으로 나아가게 했습니다
			TransBlt(hMemDC, riflebullet[i].x, riflebullet[i].y, EnemyBulletBit, RGB(255, 255, 255));		// 총알을 그려주었고요
		}
		if (riflebullet[i].x >= 800)				// 만약 우측 밖으로 나가면
		{
			riflebullet[i].create = false;		// 사라지게 했습니다
		}
	}
}

// 권총병을 컨트롤 해주는 함수
void PistolControl(HDC hMemDC)
{
	for (int i = 0; i < 30; i++)			// 적을 30명까지 확인
	{
		if (pistol[i].create)				// 생성되어있는 권총병이라면
		{
			pistol[i].y++;					// 그 적을 배경과 같은 속도로 내려가게 했습니다

			if (pistol[i].x < 0)			// 벽에 부딪힐 경우 반대방향으로 가게 했습니다
			{
				pistol[i].dir = 1;
			}
			else if (pistol[i].x > 700)
			{
				pistol[i].dir = 0;
			}
			
			if (pistol[i].dir == 0)		//dir에 따라 이동방향이 바뀝니다
			{
				pistol[i].x--;
			}
			else if(pistol[i].dir == 1)
			{
				pistol[i].x++;
			}
			MoveTransBlt(hMemDC, pistol[i].x, pistol[i].y, PistolBit, RGB(76, 76, 76), pistol_frm);		// 적을 그려주었고요

			if (pistol[i].y >= 1000)			// 만약 하단 밖으로 나가면
			{
				pistol[i].create = false;		// 사라지게 했습니다
			}
			if (pistol[i].hp <= 0)				// hp가 0 이하가 되면
			{
				pistol[i].create = false;		// 사라지게 했습니다
				EfectCreate(pistol[i].x, pistol[i].y, MopDieBit[0], 4);
				point += 100;
			}
		}
	}
}

// 소총병을 컨트롤 해주는 함수
void RifleControl(HDC hMemDC)
{

	for (int i = 0; i < 10; i++)			// 적을 10명까지 확인
	{
		if (rifle[i].create)				// 생성되어있는 적이라면
		{
			if (rifle[i].y <= py + 20)			// 플레이어를 y축으로 따라다니게 했습니다
			{
				rifle[i].y++;
			}
			else
			{
				rifle[i].y--;
			}

			if (rifle[i].hide)				// 상태에 따라 바뀌는 비트맵
			{
				MoveTransBlt(hMemDC, rifle[i].x, rifle[i].y, RifleBit[1], RGB(76, 76, 76), pistol_frm);		// 적을 그려주었고요
			}
			else
			{
				MoveTransBlt(hMemDC, rifle[i].x, rifle[i].y, RifleBit[0], RGB(76, 76, 76), pistol_frm);
			}

			if (rifle[i].hp <= 0)				// hp가 0 이하가 되면
			{
				rifle[i].create = false;		// 사라지게 했습니다
				EfectCreate(rifle[i].x, rifle[i].y, MopDieBit[1], 2);
				point += 150;
			}
		}
	}
}

// 기관총병을 컨트롤 해주는 함수
void MachineGunControl(HDC hMemDC)
{
	for (int i = 0; i < 10; i++)			// 적을 10명까지 확인
	{
		if (machinegun[i].create)				// 생성되어있는 기관총병이라면
		{

			machinegun[i].y++;					// 그 적을 배경과 같은 속도로 내려가게 했습니다

			if (machinegun[i].x < 0)			// 벽에 부딪힐 경우 반대방향으로 가게 했습니다
			{
				machinegun[i].dir = 1;
			}
			else if (machinegun[i].x > 700)
			{
				machinegun[i].dir = 0;
			}

			if (machinegun[i].dir == 0)		//dir에 따라 이동방향이 바뀝니다
			{
				machinegun[i].x -= 5;
			}
			else if (machinegun[i].dir == 1)
			{
				machinegun[i].x += 5;
			}
			MoveTransBlt(hMemDC, machinegun[i].x, machinegun[i].y, MachineGunBit, RGB(76, 76, 76), pistol_frm);		// 적을 그려주었고요

			if (machinegun[i].y >= 1000)			// 만약 하단 밖으로 나가면
			{
				machinegun[i].create = false;		// 사라지게 했습니다
			}
			if (machinegun[i].hp <= 0)				// hp가 0 이하가 되면
			{
				machinegun[i].create = false;		// 사라지게 했습니다
				EfectCreate(machinegun[i].x, machinegun[i].y, MopDieBit[2], 4);
				point += 300;
			}
		}
	}
}

// 배경을 그리는 함수입니다
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

	BitBlt(hdc, x, y + bgy - 500, bx, by, MemDC, 0, 0, SRCCOPY);				// 배경을 그렸습니다
	if (bgy >= 500)
	{
		BitBlt(hdc, x, y + bgy - 500 - 1499, bx, by, MemDC, 0, 0, SRCCOPY);		// 배경이 더 없으면 위에 배경을 한번더 그려줬습니다
	}

	if (bgy >= 1499)					// 그리고 다시 배경 좌표를 0으로 바꾸면서 자연스럽게 보이도록 했습니다
	{
		bgy = 0;
	}

	SelectObject(MemDC, OldBitmap);
	DeleteDC(MemDC);
}

// 움직이는 보스를 그리는 함수입니다
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

// 움직이는 그림을 그리는 함수입니다
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

// 정해진 색을 지워서 그려주는 함수입니다
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

// 그림을 그려주는 함수입니다
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

// 더블버퍼링의 핵심입니다
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
		//배경
		if (Level != 3)
		{
			BackgroundBlt(hMemDC, 0, 0, BackgroundBit);							// 배경을 그렸습니다
		}
		else
		{
			BackgroundBlt(hMemDC, 0, 0, HellBackgroundBit);
		}

		EfectControl(hMemDC);		// 이펙트 컨트롤함수

		//플레이어 비트맵

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
					MoveTransBlt(hMemDC, px, py, QueenBoomBit, RGB(255, 255, 255), player_frm);		// 퀸 플레이어를 그렸습니다
				}
				else
				{
					MoveTransBlt(hMemDC, px, py, QueenPlayerBit[0], RGB(255, 255, 255), player_frm);		// 퀸 플레이어를 그렸습니다
				}
			}
			else
			{
				MoveTransBlt(hMemDC, px, py, PlayerBit, RGB(255, 255, 255), player_frm);		// 플레이어를 그렸습니다
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
					MoveTransBlt(hMemDC, px, py, QueenBoomBit, RGB(255, 255, 255), player_frm);		// 퀸 플레이어를 그렸습니다
				}
				else
				{
					MoveTransBlt(hMemDC, px, py, QueenPlayerBit[1], RGB(255, 255, 255), player_frm);		// 퀸 플레이어를 그렸습니다
				}
			}
			else
			{
				MoveTransBlt(hMemDC, px, py, PlayerDamagedBit, RGB(255, 255, 255), player_frm);		// 플레이어를 그렸습니다
			}
		}

		if (php <= 0)		// 플레이어의 체력이 0이하가 되면
		{
			AllStop = true;		// 게임을 정지시키고
			KillTimer(hWndMain, 3);		// 무적 없애기
			if (boss.create)		// 보스전에서 죽었을 때와 아닐 때를 구분해서 엔딩을 지어줬습니다
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

		bgy++;				// 배경 움직임

		EnemyBulletControl(hMemDC);	// 적 총알 컨트롤 함수

		PistolControl(hMemDC);			// 권총병 컨트롤 함수

		EnemyShot();					// 적 발사

		RifleControl(hMemDC);			// 소총병 컨트롤 함수
		RifleBulletControl(hMemDC);	// 소총병 총알 컨트롤 함수

		MachineGunControl(hMemDC);			// 기관총병 컨트롤 함수


		HellMopControl(hMemDC);		// hell 몹 컨트롤
		MopShot();						// hell 적 사격
		HellBulletControl(hMemDC);		// hell 적 총알 컨트롤

		/*if (boss.hp < 500)
		{*/
			QueenControl(hMemDC);		// 퀸 컨트롤 함수
		/*}
		else
		{
			BossControl(hMemDC);		// 보스컨트롤 함수
		}*/

		HpPotionControl(hMemDC);			// 포션 컨트롤 함수

		KingControl(hMemDC);			// 킹 컨트롤함수

		SettControl(hMemDC);			// 세트 컨트롤함수

		/*if (boss.hp >= 500)
		{
			if (boss.create)
			{
				switch (boss.stance)			// 총알 상태
				{
				case 0:
					BossBulletControl(hMemDC);		// 보스총알 컨트롤 함수  패턴 1
					break;
				case 1:
					BossBulletControl2(hMemDC);	// 패턴 2
					break;
				case 2:
					BossBulletControl3(hMemDC);	// 패턴 3
					break;
				}
			}
		}*/

		if (boss.x > 650)				// 보스의 방향 전환을 위해 만들었습니다
		{
			boss.dir = 1;
		}
		if (boss.x < 0)
		{
			boss.dir = 0;
		}

		LifeBlt(hMemDC, 20, 880, LifeBit, RGB(255, 255, 255), php);		// 플레이어 체력입니다

		if (boomcheck)		// 폭탄 이펙트
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

		DrawBulletIndex(hMemDC);					// 남은 총알

		DrawBoomIndex(hMemDC);						// 남은 폭탄

		DrawPoint(hMemDC);							// 점수

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
			DrawBitmap(hMemDC, 0, 0, RevengeBit[0]);		// 퀸 로비 화면
		}
		else
		{
			DrawBitmap(hMemDC, 0, 0, StartBit);		// 로비 화면
		}
		break;
	case 1:
		if (QueenPlayer)
		{
			DrawBitmap(hMemDC, 0, 0, RevengeBit[4]);		// 퀸 그냥 패배
		}
		else
		{
			DrawBitmap(hMemDC, 0, 0, EndBit[0]);	// 그냥 패배 엔딩
		}
		break;
	case 2:
		DrawBitmap(hMemDC, 0, 0, EndBit[1]);	// 퀸에서 패배 엔딩
		break;
	case 3:
		if (QueenPlayer)
		{
			DrawBitmap(hMemDC, 0, 0, RevengeBit[1]);		// 퀸 클리어 화면
		}
		else
		{
			DrawBitmap(hMemDC, 0, 0, EndBit[2]);	// 승리 엔딩
		}
		DrawPoint(hMemDC);							// 점수
		break;
	case 4:
		DrawBitmap(hMemDC, 0, 0, InstructionBit);	// 설명서
		break;
	case 5:
		if (QueenPlayer)
		{
			DrawBitmap(hMemDC, 0, 0, RevengeBit[3]);	// 렛츠 고~
		}
		else
		{
			DrawBitmap(hMemDC, 0, 0, HellInstructionBit);	// 설명서2
		}
		break;
	case 6:
		DrawBitmap(hMemDC, 0, 0, EndBit[3]);	// 킹에서 패배 엔딩
		break;
	case 7:
		DrawBitmap(hMemDC, 0, 0, RevengeBit[2]);	// 퀸 세트에게 패배
		break;
	}

	SelectObject(hMemDC, OldBit);
	DeleteDC(hMemDC);
	ReleaseDC(hWndMain, hdc);
	InvalidateRect(hWndMain, NULL, false);
}

// 시작 시
void Create()				// 모든 비트맵을 여기서 삽입했습니다
{
	#pragma region Hell 이전 비트맵들

	PlayerBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1));			// 플레이어
	BackgroundBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP2));		// 배경
	BulletBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP3));			// 총알
	PistolBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP4));			// 권총병

	RifleBit[0] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP5));			// 소총병
	RifleBit[1] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP6));			// 소총병 숨은 자세
	RifleBit[2] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP7));			// 소총병 사격 자세

	EnemyBulletBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP8));			// 적 총알

	LifeBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP10));			// 플레이어 생명
	PlayerDamagedBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP11));			// 플레이어 피격

	MachineGunBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP12));			// 기관총병

	// 보스 비트맵들 (퀸)
	BossBit[0] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP13));			// 기본
	BossBit[1] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP14));			// 사격
	BossBit[2] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP15));			// 스킬
	BossBit[3] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP16));			// 회전1
	BossBit[4] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP17));			// 회전2

	BossLifeBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP18));			// 보스 체력
	BossBulletBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP19));			// 보스 총알
	//

	HpPotionBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP20));			// 회복약

	StartBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP21));				// 시작 화면
	EndBit[0] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP22));			// 패배 엔딩 1
	EndBit[1] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP23));			// 패배 엔딩 2
	EndBit[2] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP24));			// 성공 엔딩 1

	InstructionBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP25));			// 설명서

	#pragma endregion

	HellInstructionBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP26));			// HELL 설명서

	HellBackgroundBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP27));			// 헬 배경

	HellBulletBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP28));			// 헬 총알

	HellMopBit[0] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP29));			// 강화된 권총병
	HellMopBit[1] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP30));			// 크레모아병
	HellMopBit[2] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP31));			// 샷건병
	HellMopBit[3] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP32));			// 탱크


	RifleBit[3] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP34));			// Hell 소총병 숨음
	RifleBit[4] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP33));			// 기본
	RifleBit[5] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP35));			// 사격

	// 킹
	KingBit[0] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP37));			// 기본
	KingBit[1] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP38));			// 반격기

	KingCreateBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP39));			// 소환 이펙트
	KingAttackBit[0] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP40));			// 공격1
	KingAttackBit[1] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP43));			// 공격2
	KingAttackBit[2] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP44));			// 공격3
	KingAttackBit[3] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP45));			// 공격4
	KingBulletBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP41));			// 총알
	KingCounterAttackBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP42));	// 반격이펙트
	KingLifeBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP46));	// 체력바
	//

	ItemBit[0] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP47));	// 아이템1
	ItemBit[1] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP48));	// 아이템2
	ItemBit[2] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP49));	// 아이템3

	NewBulletBit[0] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP50));	// S총알
	NewBulletBit[1] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP51));	// HH총알
	NewBulletBit[2] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP52));	// H총알

	BoomBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP53));	// 폭탄

	LevelBit[0] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP54));	// 레벨1
	LevelBit[1] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP55));	// 레벨2
	LevelBit[2] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP56));	// 레벨3

	WarningBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP57));	// Warning

	MopDieBit[0] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP58));	// 권총병 사망
	MopDieBit[1] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP63));	// 소총병 사망
	MopDieBit[2] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP59));	// 기관총병 사망
	MopDieBit[3] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP60));	// 권총병2 사망
	MopDieBit[4] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP61));	// 크레모아병 사망
	MopDieBit[5] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP62));	// 샷건병 사망
	MopDieBit[6] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP64));	// 탱크 사망


	SBulletBit[0] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP65));	// S총알
	SBulletBit[1] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP66));	// S총알
	SBulletBit[2] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP67));	// S총알
	SBulletBit[3] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP68));	// S총알
	SBulletBit[4] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP69));	// S총알
	SBulletBit[5] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP70));	// S총알
	SBulletBit[6] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP71));	// S총알

	EndBit[3] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP72));	// 킹 엔딩

	BoomBit2 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP73));	// 폭탄2

	QueenPlayerBit[0] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP74));	// 퀸
	QueenPlayerBit[1] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP75));	// 퀸 피해받음
	QueenPlayerBulletBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP76));	// 퀸 피해받음

	QueenBoomBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP77));	// 퀸 폭탄

	SettBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP78));	// 세트
	SettWBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP79));	// 세트 W 모션
	SettWEfectBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP80));	// 세트 W 이펙트
	SettBulletBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP81));	// 세트 총알
	SettLifeBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP82));	// 세트 체력바
	SettEBit = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP83));	// 세트 E 모션

	SettMax[2] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP84));	// 세트 E
	SettMax[0] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP85));	// 세트 기본
	SettMax[1] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP86));	// 세트 W

	RevengeBit[0] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP87));	// 퀸 로비
	RevengeBit[1] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP88));	// 퀸 클리어
	RevengeBit[2] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP89));	// 퀸 세트에게 패배
	RevengeBit[3] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP90));	// 퀸 설명서?
	RevengeBit[4] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP91));	// 퀸 패배



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

		Create();		// 비트맵 삽입용 함수입니다
		
		SetTimer(hWnd, 0, 10, NULL);		// OnTimer, GetKeyState
		SetTimer(hWnd, 1, 80, NULL);		// OnTimer는 너무 빨라서 따로 player_frm 값 등을 컨트롤 할 타이머입니다

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
					if (GetKeyState(VK_LEFT) & 0x8000)			//가상 키코드 (현재 상태에 키보드 상태를 물어봄) 현재 상태에서 키가 눌렷는지 확인해봄(값이 0x 8000)
					{												// 왼쪽
						px -= SPEED;
					}
				}

				if (px < MaxX)
				{
					if (GetKeyState(VK_RIGHT) & 0x8000)            // 오른쪽
					{
						px += SPEED;
					}
				}

				if (py > MasY)
				{
					if (GetKeyState(VK_UP) & 0x8000)            // 위쪽
					{
						py -= SPEED;
					}
				}

				if (py < MaxY)
				{
					if (GetKeyState(VK_DOWN) & 0x8000)            // 아래쪽
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
				if (delay)					// delay가 true일 때만 발사가 가능하게 했습니다
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
			player_frm++;					// 플레이어 이동 프레임을 바꾸는 것
			player_frm %= 8;				// 다시 초기화 시켰습니다

			pistol_frm++;				// 권총병		//위와 같은 방식입니다
			pistol_frm %= 4;

			if (boss_frm < 7 && boss.state != 5)			// 7 이상 안올라가게 했습니다
			{
				boss_frm++;					// 보스
			}
			else if (boss.state == 5)		// 스킬 끝나면
			{
				if (boss_frm != 0)			// 0일 때까지 감소
				{
					boss_frm--;
				}
			}
			switch (boss.state)
			{
			case 0:			// 기본상태
				boss_frm %= 7;
				break;
			case 1:			// 공격
				boss_frm %= 5;
				if (boss_frm == 0)
				{
					boss.state = 0;
				}
				break;
			case 2:			// 게틀링		// 빈 칸 입니다
			case 5:

				break;
			case 3:			// 회전
			case 4:
				boss_frm %= 6;
				break;
			}


			if (king.state != 1)
			{
				king_frm++;			// 킹 프레임관리
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

			// 폭탄 프레임
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

			// 보스는 시간이 지날수록 점수가 깎여서 빨리 깨도록 만들기
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
		case 2:						// stage 재생용
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
		case 3:						// 피해 입은 후 무적 해제 타이머
			damage = false;
			KillTimer(hWnd, 3);
			break;
		case 4:						// 패턴 2 정지
			boss.state = 0;
			boss.gage3 = 0;
			KillTimer(hWnd, 4);
			break;
		case 5:						// 패턴 3 정지1
			boss.gage2 = 0;
			boss.state = 5;
			SetTimer(hWnd, 6, 500, NULL);
			KillTimer(hWnd, 5);
			break;
		case 6:						// 패턴 3 정지2
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
		case 'Z':				// 공격 버튼
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
			else if (page == 4)			// 설명창에서 Z버튼을 누르면 진행 할 수 있게하려고 넣었습니다
			{
				page = 5;			// 다음 설명서
			}
			else if (page == 5)
			{
				page = -1;			//case에 존재하지않는 아무 번호로 바꿨습니다
				AllStop = false;		// 정지해제
				SetTimer(hWndMain, 2, 1000, NULL);		// 스테이지1 실행 타이머입니다
				if (!QueenPlayer)
				{
					OnBGM(INGAME);
				}
			}
			else if (page == 3)		// 클리어 시
			{
				page = 0;		// 로비화면
				if (QueenPlayer)		// 퀸 화
				{
					QueenPlayer = false;
				}
				else
				{
					QueenPlayer = true;
				}
				Reset();		// 초기화
				OnBGM(LOBBY);
				point = 0;
				Level = 0;
			}
			else if (!AllStop || !stop)
			{
				if (delay && style != DEFAULT)					// delay가 true일 때만 발사가 가능하게 했습니다
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
					delay = false;					// Z 버튼을 꾹 누르고 있을 때 발사되지않게 하기위해 delay를 false로 만들었습니다
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
				page = -1;			//case에 존재하지않는 아무 번호로 바꿨습니다
				AllStop = false;		// 정지해제
				SetTimer(hWndMain, 2, 1000, NULL);
				OnBGM(INGAME);
			}
			break;
		case 'R':
			if (page == 1)			// 이어하기 버튼
			{
				point -= 1000;
				php = 3;
				boomindex = 1;
				bulletindex = 0;
				page = -1;			//case에 존재하지않는 아무 번호로 바꿨습니다
				AllStop = false;		// 정지해제
				damage = true;
				style = DEFAULT;
				SetTimer(hWndMain, 3, 1000, NULL);		// 1초 무적
				SetTimer(hWndMain, 2, 1000, NULL);		// 스테이지 실행 타이머입니다
			}
			else if(page == 2)
			{
				point -= 1000;
				php = 3;
				page = -1;			//case에 존재하지않는 아무 번호로 바꿨습니다
				AllStop = false;		// 정지해제
				style = DEFAULT;
				SetTimer(hWndMain, 3, 1000, NULL);		// 1초 무적
				SetTimer(hWndMain, 2, 1000, NULL);		// 스테이지 실행 타이머입니다

				Reset();
				hellstagecheck = 57;
			}
			else if (page == 6)
			{
				point -= 1000;
				php = 3;
				page = -1;			//case에 존재하지않는 아무 번호로 바꿨습니다
				AllStop = false;		// 정지해제
				style = DEFAULT;
				SetTimer(hWndMain, 3, 1000, NULL);		// 1초 무적
				SetTimer(hWndMain, 2, 1000, NULL);		// 스테이지 실행 타이머입니다

				Reset();
				stagecheck = 62;
			}
			else if (page == 7)
			{
				point -= 1000;
				php = 3;
				page = -1;			//case에 존재하지않는 아무 번호로 바꿨습니다
				AllStop = false;		// 정지해제
				style = DEFAULT;
				SetTimer(hWndMain, 3, 1000, NULL);		// 1초 무적
				SetTimer(hWndMain, 2, 1000, NULL);		// 스테이지 실행 타이머입니다

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
				if (!delay)						// Z 버튼을 뗐을 때
				{
					delay = true;				// delay를 true로 만들어서 다시 발사가 가능하게 만들었습니다
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
		hdc = BeginPaint(hWnd, &ps);	// 도구를 빌리고

		DrawBitmap(hdc, 0, 0, hBit);		// 메모리에 미리 그려둔 그림들 한번에 그리기

		EndPaint(hWnd, &ps);		// 반납했습니다
		return 0;
	case WM_DESTROY:
		for (int i = 0; i < 12; i++)
		{
			KillTimer(hWnd, i);				// 모든 타이머 정지
		}
		PostQuitMessage(0);
		return 0;
	}

	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}