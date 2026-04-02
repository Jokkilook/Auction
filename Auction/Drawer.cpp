#include "Drawer.h"
#include "Data.h"

#include <Windows.h>
#include <conio.h>
#include <string>
#include <sstream>
#include <limits>
#include <iostream>

using namespace std;

//플레이어 입력
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_ENTER 13
#define KEY_ESC 27
#define KEY_TAB 9

//색상코드
enum {
	BLACK,
	DARK_BLUE,
	DARK_GREEN,
	DARK_SKYBLUE,
	DARK_RED,
	DARK_VOILET,
	DAKR_YELLOW,
	GRAY,
	DARK_GRAY,
	BLUE,
	GREEN,
	SKYBLUE,
	RED,
	VIOLET,
	YELLOW,
	WHITE,
};

vector<string> messages = {
	"경매를 시작합니다!",
	"흥미로운 물건이 나왔군요.",
	"이건 아주 희귀한 물건입니다!",
	"이 물건의 가치를 알아보실 수 있으실까요?"
};

string MoneyFormat(float value) {

	// 소수점 버리고 정수로 변환
	string s = to_string((long long)value); 	
	int insertPos = s.length() - 3;

	while (insertPos > 0) {
		s.insert(insertPos, ",");
		insertPos -= 3;
	}
	return s;
}

void MoveCursor(int row, int col) {
	printf("\033[%d;%dH", row, col);
}

void DrawBox(int row, int col, int width, int height)
{
	//네모 위 그리기
	MoveCursor(row, col);
	printf("┌");
	for (int i = 0; i < width - 2; ++i) printf("─");
	printf("┐");

	//네모 중간 그리기
	for (int i = 1; i < height - 1; ++i) {
		MoveCursor(row + i, col);
		printf("│");
		for (int j = 0; j < width - 2; ++j) printf(" ");
		printf("│");
	}

	//네모 아래 그리기
	MoveCursor(row + height - 1, col);
	printf("└");
	for (int i = 0; i < width - 2; ++i) printf("─");
	printf("┘");
}

void DrawAsciiArt(int X, int Y, const string& AsciiArt) {
	stringstream ss(AsciiArt);
	string Line;
	int CurrentY = Y;

	while (std::getline(ss, Line)) 
	{
		MoveCursor(CurrentY++, X); // 한 줄 내려갈 때마다 X좌표는 고정, Y좌표만 +1
		printf("%s", Line.c_str());
	}
}

void DrawMainMenu()
{
	const char* menus[] = {
	"게임 시작",
	"게임 정보",
	"나 안할래"
	};

	int menu = 1;
	int select = 1;
	int menuCount = 3;
	int selected = 0;
	while (menu) {

		system("cls");

		DrawBox(1, 0, 120, 50);

		int MenuRow = 40;
		int MenuCol = 50;

		//메뉴 박스
		DrawBox(MenuRow, 50, 15, 7);

		select = 1;

		while (select) {
			//메뉴 출력
			for (int i = 0; i < 3; ++i) {
				MoveCursor(MenuRow+2 + i, 51);
				if (i == selected)
					printf("\033[01;33m > %s\033[0m\n", menus[i]);
				else if (i == menuCount - 1)
					printf("   %s", menus[i]);
				else
					printf("   %s\n", menus[i]);
			}

			MoveCursor(MenuRow+4, 62);

			int key = _getch();

			if (key == 0 || key == 224) {
				key = _getch();
				switch (key) {
				case KEY_UP:
					selected = (selected - 1 + menuCount) % menuCount;
					break;
				case KEY_DOWN:
					selected = (selected + 1) % menuCount;
					break;
				}
			}
			else if (key == KEY_ENTER) {
				switch (selected) {
				case 0:
					//drawGame();
					select = 0;
					break;
				case 1:
					//drawInfo();
					select = 0;
					break;
				case 2:
					exit(0);
					break;
				}
			}
		}
	}
}

const int SLOT_ROW = 7;
const int SLOTW_WIDTH = 30;
const int SLOT_HEIGHT = 6;

void DrawAuctionScreen()
{
	AuctionSystem Auction = AuctionSystem::GetInstance();
	bool IsPlaying = true;

	while (IsPlaying) 
	{
		system("cls");

		//전체 프레임
		DrawBox(1, 0, 120, 45);

		//로그 섹션
		DrawBox(33, 0, 120, 15);

		//물건 그리기
		//물건 이름 칸
		DrawBox(1, 0, 30, 3);
		//물건 설명 칸
		DrawBox(31, 0, 120, 3);

		//슬롯 섹션
		DrawBox(1, 89, 32, 31);

		//날짜 섹션
		DrawBox(31, 89, 32, 3);

		//참가자 슬롯
		for (int i = 0; i < 4; i++) 
		{
			DrawBox(SLOT_ROW + (i * SLOT_HEIGHT), 90, SLOTW_WIDTH, SLOT_HEIGHT);
			MoveCursor(SLOT_ROW + (i * SLOT_HEIGHT) + 1, 92);
			//플레이어 데이터 쓰기
			if (i == 0) 
			{
				printf("당신");
			}
			//NPC 데이터 쓰기
			else 
			{
				printf("NPC%d", i);
			}
		}

		//지갑 그리기
		//지갑 섹션
		DrawBox(1, 89, 32, 6);
		MoveCursor(2, 91);
		printf("당신의 지갑");
		

		bool IsProgressing = true;

		//경매 시퀀스 시작
		while (IsProgressing) 
		{
			//Auction.StartAuction();
			UpdateItem(Auction.AuctionItem);

			//플레이어 지갑 정보 업데이트
			UpdateWallet(Auction.CurrentPlayer);

			//날짜 업뎃
			MoveCursor(32, 92);
			printf("%d 주차 %d 일째", Auction.Week, Auction.Day);

			//NPC별 행동 - 경매가 올리기 or 포기
			int Index = 0;
			for (Participant* Part : Auction.Participants) 
			{
				if (Part)
				{
					if (NPC* Npc = dynamic_cast<NPC*>(Part))
					{
						if (Npc->GetState() == DROP)
						{
							Index++;
							continue;
						}
						Npc->HandleItem(Auction.AuctionItem);
						Sleep(500);
					}
					UpdateSlot(Part, Index);
				}
				Index++;
			}

			//모든 참가자 준비에서 참가 상태로
			Auction.JoinParticipants();

			//NPC가 모두 포기했으면 플레이어가 산다.
			if (Auction.CheckAllNPCGiveup()) {
				Auction.CurrentPlayer->PurchaseItem(Auction.AuctionItem);
				Auction.EndAuction();
				PurchaseSequence();
				Auction.StartAuction();
				break;
			}

			//경매가 입력이 안됐으면 받기
			if (Auction.AuctionItem->CallValue == 0.0f) 
			{
				ClearLogSection();
				CallValueSequence(Auction.AuctionItem);
			}
			//경매가 입력이 완료 됐으면 플레이어 입력 받기 - 경매가 올리기 or 포기
			else 
			{
				ClearLogSection();
				//포기했으면!
				if (!PlayerSelectionSequence()) {
					Auction.EndAuction();
					GiveupSequence();
					Auction.StartAuction();
				}
			}
		}
	}
	
}

void ClearLogSection()
{
	for (int i = 0; i < 11; i++) 
	{
		MoveCursor(34 + i, 2);
		printf("                                                                     ");
	}
}

void CallValueSequence(Item* NewItem)
{
	AuctionSystem Auction = AuctionSystem::GetInstance();

	int InputValue;

	while (true) 
	{
		MoveCursor(34, 3);
		printf("시작가를 입력하세요 : ");
		std::cin >> InputValue;

		// 숫자 외의 문자 입력 시
		if (std::cin.fail()) 
		{
			std::cin.clear();
			std::cin.ignore(256, '\n');
			ClearLogSection();
			MoveCursor(35, 3);
			printf("\033[1;31m숫자만 입력해 주세요!\n\033[0m");
		}
		// 숫자지만 최저 가격 미만 || 최고 가격 초과 시
		else {
			
			if (InputValue < NewItem->MinValue ||
				InputValue >NewItem->MaxValue)
			{
				std::cin.clear();
				std::cin.ignore(256, '\n');
				ClearLogSection();
				MoveCursor(35, 3);
				printf("\033[1;31m시작가 범위 내 가격으로 입력해주세요!\n\033[0m");

			}
			else 
			{
				NewItem->CallValue = InputValue;
				Auction.CurrentPlayer->SetCallValue(InputValue);
				return;
			}
		}
	}

}

bool PlayerSelectionSequence()
{
	int Selected = 0;
	int OptionsSize = 2;
	int IsSelecting = 1;

	AuctionSystem Auction = AuctionSystem::GetInstance();

	while (IsSelecting) {

		//ClearLogSection();
		//선택지 출력
		MoveCursor(38, 3);
		if (Selected == 0) 
		{
			printf("\033[1;33m > 가즈아!!\033[0m");
		}
		else {
			printf("   가즈아!!");
		}

		if (Selected == 1) 
		{
			printf("\033[1;33m > 안 사 ㅆㅂ...\033[0m");
		}
		else
		{
			printf("   안 사 ㅆㅂ...");
		}

		//플레이어 입력 받기
		int Key = _getch();

		//방향기 좌우 입력 시
		if (Key == 0 || Key == 224) 
		{
			Key = _getch();
			switch (Key) {
			case KEY_LEFT:
				Selected = (Selected - 1 + OptionsSize) % (OptionsSize);
				break;
			case KEY_RIGHT:
				Selected = (Selected + 1) % (OptionsSize);
				break;
			case KEY_UP:
			case KEY_DOWN:
				Selected = Selected > OptionsSize ? 0 : 100;
				break;
			}
		}
		//엔터 입력 시
		else if (Key == KEY_ENTER) {
			//선택지의 따라
			switch (Selected) 
			{
				//올리기
				case 0: {
					if (!Auction.CurrentPlayer->HandleItem(Auction.AuctionItem))
					{
						//돈 부족해서 못 올릴 시
						MoveCursor(35, 3);
						printf("\033[1;31m돈이 없셔....\n\033[0m");
					}
					else {
						return true;
					}
					break;
				}
				//포기
				case 1: {
					return false;
					break;
				}
			}
		}
	}	
}

void UpdateItem(Item* NewItem)
{
	if (NewItem) {
		//이름 출력
		MoveCursor(2, 3);
		printf("                         ");
		MoveCursor(2, 3);
		printf("%s", NewItem->Name.c_str());
		//시작가 범위 출력
		MoveCursor(2, 33);
		printf("                                                 ");
		MoveCursor(2, 33);
		string MinValue = MoneyFormat(NewItem->MinValue);
		string MaxValue = MoneyFormat(NewItem->MaxValue);
		printf("시작가 범위 : %s 원 ~ %s 원", MinValue.c_str(), MaxValue.c_str());

		//설명 출력
		MoveCursor(32, 3);
		printf("                                                            ");
		MoveCursor(32, 3);
		printf("%s", NewItem->Description.c_str());

		//아스키 아트 출럭
		DrawAsciiArt(30, 5, NewItem->AsciiArt);
	}
}

void UpdateSlot(Participant* Part, int Index)
{
	MoveCursor(SLOT_ROW + (Index * SLOT_HEIGHT) + 3, 92);
	printf("                          ");
	MoveCursor(SLOT_ROW + (Index * SLOT_HEIGHT) + 3, 92);
	if (Part->GetState() == STATE::READY) 
	{
		printf("대기");
	}
	else if (Part->GetState() == STATE::DROP) 
	{
		printf("포기");
	}
	else 
	{
		string Money = MoneyFormat(Part->GetCallValue());
		printf("입찰가 : %s 원", Money.c_str());
	}
}

void UpdateWallet(Player* Player)
{
	MoveCursor(4, 91);

	if (Player) 
	{
		string Money = MoneyFormat(Player->GetCurrentMoney());
		printf("%s 원", Money.c_str());
	}
	else 
	{
		printf("엥... 지갑이 없는디요...");
	}
}

void PurchaseSequence()
{
	AuctionSystem Auction = AuctionSystem::GetInstance();

	ClearLogSection();
	MoveCursor(34, 3);
	printf("오늘 물건의 낙찰자는 %s 님!!", Auction.CurrentPlayer->GetName().c_str());
	_getch();
	string Money = MoneyFormat(Auction.AuctionItem->CallValue);
	printf("%s 원에 낙찰되셨습니다!", Money.c_str());
	_getch();
	printf("다음에 또 만나요.");

}

void GiveupSequence()
{
	ClearLogSection();
	MoveCursor(34, 3);
	printf("네! 포기자는 저~~~~ 짝으로 빠져주시기 바랍니다!");
	_getch();
	ClearLogSection();
}