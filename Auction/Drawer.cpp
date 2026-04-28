#include "Drawer.h"
#include "Data.h"

#include <Windows.h>
#include <conio.h>
#include <string>
#include <sstream>
#include <limits>
#include <iostream>
#include <random>

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
	"이 물건의 가치를 알아보실 수 있으실까요?",
	"오늘의 물건입니다!",
	"오늘도 재밌는 물건이 나왔군요!",
	"자~ 오늘의 물건을 보시죠!",
	"경매가 시작됩니다!",
	"여러분의 안목을 시험해보시죠!",
	"아....이건...."
};

void ClearBuffer() {
	// 키보드 입력 버퍼에 데이터가 남아있는 동안 계속 읽어서 버려버림
	while (_kbhit()) {
		_getch();
	}
}

string MoneyFormat(float value) {

	// 1. 음수 여부 확인 및 절대값 취하기
	bool isNegative = value < 0;
	long long num = std::abs((long long)value);

	// 2. 숫자를 문자열로 변환
	std::string s = std::to_string(num);

	// 3. 뒤에서부터 3자리마다 콤마 삽입
	// (int) 형변환을 통해 언더플로우 방지
	int insertPos = (int)s.length() - 3;
	while (insertPos > 0) {
		s.insert(insertPos, ",");
		insertPos -= 3;
	}

	// 4. 음수였다면 다시 '-' 붙여서 반환
	return isNegative ? "-" + s : s;
}

void TypeWrite(int X, int Y, const std::string& Text, int Delay = 50) {
	ClearBuffer();
	// 1. 시작 위치로 커서 이동
	// (중앙 정렬을 원하면 x좌표에서 문자열 길이의 절반을 빼주는 센스!)
	int StartX = X - (Text.length() / 4); // 한글은 2칸 차지하므로 대략 /4

	MoveCursor(Y, 2);
	printf("%100s","");
	MoveCursor(Y, StartX);

	// 2. 한 글자씩 출력
	for (int i = 0; i < Text.length(); ) {
		// 한글인지 영어인지 체크 (한글은 보통 2~3바이트씩 한꺼번에 찍어야 안 깨짐)
		int charStep = ((unsigned char)Text[i] < 0x80) ? 1 : 2; // CP949(한글 2바이트) 기준

		for (int j = 0; j < charStep; j++) {
			printf("%c", Text[i + j]);
		}

		i += charStep;

		// 3. 즉시 화면에 반영 (버퍼 비우기)
		fflush(stdout);

		// 4. 딜레이 (띠리리릭 소도가 결정됨)
		Sleep(Delay);
	}
}

void MoveCursor(int row, int col) {
	if (row < 1) row = 1;
	if (col < 1) col = 1;
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

void PrintAsciiAtLocation(int row, int col, const char* ascii)
{
	//row, col 위치에 ascii 출력
	printf("\033[%d;%dH%s", row, col, ascii);
}

void DrawAsciiArt(int X, int Y, const string& AsciiArt) {
	stringstream ss(AsciiArt);
	string Line;
	int CurrentY = Y;

	while (std::getline(ss, Line)) 
	{
		MoveCursor(CurrentY++, X);
		printf("%s", Line.c_str());
	}
}

void DrawMainMenu()
{
	static const char* Logo[13] = {
"			 \033[33m_____  _   _  _____      ___  ___  ___  ___   ______ _____  _   _  _____  ",
"			\033[33m|_   _|| | | ||  ___|    / _ \\ |  \\/  | / _ \\ |___  /|_   _|| \\ | ||  __ \\ ",
"			  \033[33m| |  | |_| || |__     / /_\\ \\| .  . |/ /_\\ \\   / /   | |  |  \\| || |  \\/ ",
"			  \033[33m| |  |  _  ||  __|    |  _  || |\\/| ||  _  |  / /    | |  | . ` || | __  ",
"			  \033[33m| |  | | | || |___    | | | || |  | || | | |./ /___ _| |_ | |\\  || |_\\ \\ ",
"			  \033[33m\\_/  \\_| |_/\\____/    \\_| |_/\\_|  |_/\\_| |_/\\_____/ \\___/ \\_| \\_/ \\____/ ",
"	",
"	  \033[31m______  _____  _____  _____  _____   ___   _          ___   _   _  _____  _____  _____  _____  _   _ ",
"	  \033[34m|  _  \\|_   _||  __ \\|_   _||_   _| / _ \\ | |        / _ \\ | | | |/  __ \\|_   _||_   _||  _  || \\ | |",
"	  \033[33m| | | |  | |  | |  \\/  | |    | |  / /_\\ \\| |       / /_\\ \\| | | || /  \\/  | |    | |  | | | ||  \\| |",
"	  \033[31m| | | |  | |  | | __   | |    | |  |  _  || |       |  _  || | | || |      | |    | |  | | | || . ` |",
"	  \033[34m| |/ /  _| |_ | |_\\ \\ _| |_   | |  | | | || |____   | | | || |_| || \\__/\\  | |   _| |_ \\ \\_/ /| |\\  |",
"	  \033[31m|___/   \\___/  \\____/ \\___/   \\_/  \\_| |_/\\_____/   \\_| |_/ \\___/  \\____/  \\_/   \\___/  \\___/ \\_| \\_/\033[0m",
	};

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

		int MenuRow = 30;
		int MenuCol = 50;

		for (int i = 0; i < sizeof(Logo) / sizeof(Logo[0]); ++i) {
			PrintAsciiAtLocation(8+i, 4, Logo[i]);
		}

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

			while (_kbhit()) _getch();
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
					DrawAuctionScreen();
					select = 0;
					break;
				case 1:
					DrawGameInfo();
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

void DrawGameInfo()
{
	system("cls");

	//전체 프레임
	DrawBox(1, 0, 120, 45);

	int Row = 3;
	int Col = 5;
	int Step = 2;
	int Count = 0;

	MoveCursor(Row++, 2);
	printf("===================================================== \033[01;34m게임 정보\033[0m ======================================================");
	Row += 2;
	MoveCursor(Row+(Step*Count++), Col);
	printf("똥 손같은 투자로 전재산을 홀라당 잃어버린 당신");
	MoveCursor(Row + (Step * Count++), Col);
	printf("어느 날 복권에 당첨되어 500만원을 얻게되는데...");
	MoveCursor(Row + (Step * Count++), Col);
	printf("레전드로 망한 투자대신 고른 것은 경매!");
	MoveCursor(Row + (Step * Count++), Col);
	printf("일주일 동안 경매로 물건을 사들이고, 판매하여 돈을 벌기로 한다.");
	MoveCursor(Row + (Step * Count++), Col);
	printf("이번엔 현명하게 수익이 목표치를 넘기지 못하면 그만 둔다는데... 과연 얼마나 이어갈 수 있을까?");

	Row += (Step * Count++)+1;
	Count = 0;

	MoveCursor(Row++, 2);
	printf("===================================================== \033[01;34m조작 방법\033[0m ======================================================");
	Row += 2;

	MoveCursor(Row + (Step * Count++), Col);
	printf("\033[1;32m←, → : 고르기 | Enter : 선택\033[0m");
	MoveCursor(Row + (Step * Count++), Col);
	printf("\033[1;32mESC : PAUSE 메뉴\033[0m");
	MoveCursor(Row + (Step * Count++), Col);
	printf("\033[1;32m시작가 입력 : 숫자 입력 후 Enter\033[0m");

	MoveCursor(43, 5);
	printf("아무 키나 누르면 메인 메뉴로 돌아갑니다...");
	while (_kbhit()) _getch();
	_getch();
}

bool DrawPauseScreen()
{
	const char* Menus[] = {
		"돌아가기",
		"나가기"
	};

	int Selected = 0;
	const int MenuCount = 2;

	while (true) {
		ClearLogSection();

		DrawBox(34, 35, 50, 11);
		MoveCursor(35, 56);
		printf("일시정지");

		for (int i = 0; i < MenuCount; i++) {
			MoveCursor(38 + (i * 2), 44);
			if (i == Selected) {
				printf("\033[01;33m> %s\033[0m", Menus[i]);
			}
			else {
				printf("  %s", Menus[i]);
			}
		}

		MoveCursor(43, 38);
		printf("ESC : 돌아가기");

		while (_kbhit()) _getch();
		int Key = _getch();

		if (Key == KEY_ESC) {
			ClearLogSection();
			return true;
		}

		if (Key == 0 || Key == 224) {
			Key = _getch();
			switch (Key) {
			case KEY_UP:
				Selected = (Selected - 1 + MenuCount) % MenuCount;
				break;
			case KEY_DOWN:
				Selected = (Selected + 1) % MenuCount;
				break;
			}
		}
		else if (Key == KEY_ENTER) {
			// 돌아가기
			if (Selected == 0) {
				ClearLogSection();
				return true;
			}

			// 나가기 확인
			int ConfirmSelected = 0; // 0 = 예, 1 = 아니오
			while (true) {
				ClearLogSection();

				DrawBox(34, 25, 70, 11);
				MoveCursor(36, 37);
				printf("정말 나가시겠습니까?");

				MoveCursor(39, 45);
				if (ConfirmSelected == 0) {
					printf("\033[01;33m[예]\033[0m   [아니오]");
				}
				else {
					printf("[예]   \033[01;33m[아니오]\033[0m");
				}

				MoveCursor(43, 33);
				printf("←→ 선택  Enter 결정  ESC 취소");

				while (_kbhit()) _getch();
				int ConfirmKey = _getch();
				if (ConfirmKey == KEY_ESC) {
					break;
				}

				if (ConfirmKey == 0 || ConfirmKey == 224) {
					ConfirmKey = _getch();
					switch (ConfirmKey) {
					case KEY_LEFT:
					case KEY_RIGHT:
						ConfirmSelected = 1 - ConfirmSelected;
						break;
					}
				}
				else if (ConfirmKey == KEY_ENTER) {
					if (ConfirmSelected == 0) {
						ClearLogSection();
						return false;
					}

					// 아니오 -> 퍼즈 메뉴로
					break;
				}
			}
		}
	}
}

void DrawPrologue()
{
	vector<string> Dialogue = {
		".........",
		".....",
		"내 돈이 어디 갔지...",
		"그 코인만 아니었어도.....",
		"........",
		".....",
		"A Few Moments Later",
		"끼얏호우-!!!",
		"내가 복권에 당첨되다니!!!",
		"그래... 1등은 아니지만 이 500만원으로 다시 시작해보는 거야!",
		"뭐가 좋을까...",
		"코인은 이제 손도 안댈거고... 주식도 무섭고...",
		"그래! 경매를 해보자...!",
		"값비싼 물건을 싸게 사서 비싸게 파는거야!",
		"일주일 동안 경매를 하고 마지막 날 암시장에 비싸게 팔자...",
		"일주일 간 수익률이 목표 금액을 못 넘으면 이것도 때려쳐버려야지!",
		"수익율은 20%로 가는거야!",
		"그럼 경매장으로 가볼까~"
	};

	system("cls");

	//전체 프레임
	DrawBox(1, 0, 120, 45);

	MoveCursor(2, 3);
	printf("\033[01;34m이름 입력 후 ESC를 누르면 건너뛸 수 있습니다.\033[0m");

	auto& Auction = AuctionSystem::GetInstance();

	TypeWrite(55, 20, "당신의 이름은? : ");
	string NewName;
	while (NewName.empty()) {
		MoveCursor(20, 68);
		getline(cin, NewName);
	}

	Auction.CurrentPlayer->SetName(NewName);

	for (string S : Dialogue) {
		TypeWrite(55, 20, S);
		while (_kbhit()) _getch();
		int Key = _getch();

		if (Key == KEY_ESC) return;
	}
}

const int SLOT_ROW = 7;
const int SLOTW_WIDTH = 30;
const int SLOT_HEIGHT = 6;

void DrawAuctionScreen()
{
	//WeekResultScreen(); return;

	DrawPrologue();

	auto& Auction = AuctionSystem::GetInstance();
	bool IsPlaying = true;

	while (IsPlaying) 
	{
		system("cls");

		//전체 프레임
		DrawBox(1, 0, 120, 45);

		//로그 섹션
		DrawBox(33, 0, 120, 13);

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
				printf("%s", Auction.CurrentPlayer->GetName().c_str());
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
			printf("%d 주차 %d 일째", Auction.Week, Auction.Day%7 ? Auction.Day%7 : 7);
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
				const bool IsCalculateDay = (Auction.Day%7==0);
				Item* PurchasedItem = Auction.AuctionItem;
				const bool Purchased = (PurchasedItem != nullptr) && Auction.CurrentPlayer->PurchaseItem(PurchasedItem);
				if (Purchased) {
					// 플레이어 인벤토리로 소유권 이전
					Auction.AuctionItem = nullptr;
					Auction.EndAuction();
					PurchaseSequence(PurchasedItem);
					if (IsCalculateDay) {
						if (!DrawCalculateDay()) return;
					}
					Auction.StartAuction();
					break;
				}

				// (돈 부족/인벤토리 가득 등) 구매 실패 시: 아이템은 폐기하고 다음 경매로
				Auction.EndAuction();
				ClearLogSection();
				MoveCursor(34, 3);
				printf("\033[1;31m낙찰되었지만 구매할 수 없었습니다...\033[0m");
				while (_kbhit()) _getch();
				_getch();
				if (IsCalculateDay) {
					if (!DrawCalculateDay()) return;
				}
				Auction.StartAuction();
				break;
			}

			//경매가 입력이 안됐으면 받기
			if (Auction.AuctionItem->CallValue == 0.0f) 
			{
				// 시작가 최저 금액조차 감당 못 하면 오늘은 건너뛰기
				if (Auction.AuctionItem->MinValue > Auction.CurrentPlayer->GetCurrentMoney())
				{
					const bool IsCalculateDay = (Auction.Day == 7);

					ClearLogSection();
					MoveCursor(34, 3);
					printf("\033[1;31m경매에 참여할 자금이 부족하네요.\033[0m");
					MoveCursor(34, 3);
					printf("\033[1;34mEnter를 누르면 다음날로 넘어갑니다.\033[0m");

					int Key = 0;
					while (Key != KEY_ENTER) {
						while (_kbhit()) _getch();
						Key = _getch();
						if (Key == KEY_ESC) {
							if (!DrawPauseScreen()) {
								Auction.ResetGame();
								return;
							}
							ClearLogSection();
							MoveCursor(34, 3);
							printf("\033[1;31m경매에 참여할 자금이 부족하네요.\033[0m");
							MoveCursor(34, 3);
							printf("\033[1;34mEnter를 누르면 다음날로 넘어갑니다.\033[0m");
							Key = 0;
							continue;
						}
						if (Key == 0 || Key == 224) _getch();
					}

					Auction.EndAuction();
					if (IsCalculateDay) {
						if (!DrawCalculateDay()) return;
					}
					Auction.StartAuction();
					break;
				}

				ClearLogSection();
				if (!CallValueSequence(Auction.AuctionItem)) {
					Auction.ResetGame();
					return;
				}
			}
			//경매가 입력이 완료 됐으면 플레이어 입력 받기 - 경매가 올리기 or 포기
			else 
			{
				ClearLogSection();
				//포기했으면!
				bool ExitToMenu = false;
				if (!PlayerSelectionSequence(ExitToMenu)) {
					if (ExitToMenu) {
						Auction.ResetGame();
						return;
					}
					const bool IsCalculateDay = (Auction.Day == 7);
					Auction.EndAuction();
					GiveupSequence();
					if (IsCalculateDay) {
						if (!DrawCalculateDay()) return;
					}
					Auction.StartAuction();
					if (IsCalculateDay) {
						break;
					}
				}
			}
		}
	}
	
}

bool DrawCalculateDay()
{
	auto& Auction = AuctionSystem::GetInstance();
	Player* CurrentPlayer = Auction.CurrentPlayer;
	Auction.SetNews();

	for (auto* Item : CurrentPlayer->GetInventory()) {
		Auction.ApplyNewsToItem(Item);
	}

	system("cls");

	//전체 프레임
	DrawBox(1, 0, 120, 45);

	//로그 섹션
	DrawBox(33, 0, 120, 13);

	//아이템 리스트 섹션
	DrawBox(1, 0, 89, 32);

	DrawBox(1, 0, 89, 3);
	MoveCursor(2, 3);
	printf("아무도 모르는 암시장 | 암시장 이용법 : ↑↓ 선택  ←→ 판매/보류  Enter 결정");

	//지갑 섹션(경매 화면이랑 같은 위치)
	DrawBox(1, 89, 32, 8);
	MoveCursor(2, 91);
	printf("당신의 지갑");

	string Goal = MoneyFormat(CurrentPlayer->GetGoal());
	MoveCursor(6, 91);
	printf("목표 금액 : %10s 원", Goal.c_str());


	MoveCursor(4, 3);
	switch (Auction.News) {
	case NONE: {
		printf("☞ 오늘의 뉴스!: 조용하네요.");
		break;
	}
	case ART: {
		printf("☞ 오늘의 뉴스!: 예술 시장에 %s가 들려옵니다!", Auction.NewsType ? "호재" : "악재");
		break;
	}
	case RARE: {
		printf("☞ 오늘의 뉴스!: 수집품 열풍에 %s가 들려옵니다!", Auction.NewsType ? "호재" : "악재");
		break;
	}
	case ACIENT: {
		printf("☞ 오늘의 뉴스!: 골동품 시장에 %s가 들려옵니다!", Auction.NewsType ? "호재" : "악재");
		break;
	}
	case MASTERPIECE: {
		printf("☞ 오늘의 뉴스!: 명작 시장에 %s가 들려옵니다!", Auction.NewsType ? "호재" : "악재");
		break;
	}
	case NAMED: {
		printf("☞ 오늘의 뉴스!: 명품 시장에 %s가 들려옵니다!", Auction.NewsType ? "호재" : "악재");
		break;
	}
	}

	MoveCursor(6, 3);
	printf("   이름                        시세            구매가");

	const int ListCol = 2;
	const int ListRow = 8;
	const int ListVisibleRows = 24;
	const int NextDayRow = 30;
	const int InnerWidth = 87;

	int SelectedIndex = 0; // 0..ItemCount (마지막 = 다음날로)
	int Action = 0; // 0 = 판매, 1 = 보류
	int ScrollTop = 0;
	string StatusMessage;

	while (true) {
		if (!CurrentPlayer) return false;

		UpdateWallet(CurrentPlayer);

		const int ItemCount = CurrentPlayer->GetInventoryCount();
		const int NextDayIndex = ItemCount; // 가상 버튼

		if (ItemCount == 0) {
			SelectedIndex = NextDayIndex;
		}
		else {
			if (SelectedIndex < 0) SelectedIndex = 0;
			if (SelectedIndex > NextDayIndex) SelectedIndex = NextDayIndex;
		}

		//스크롤 보정(아이템 선택 중일 때만)
		if (SelectedIndex < ItemCount) {
			if (SelectedIndex < ScrollTop) ScrollTop = SelectedIndex;
			if (SelectedIndex >= ScrollTop + ListVisibleRows) ScrollTop = SelectedIndex - ListVisibleRows + 1;

			if (ScrollTop < 0) ScrollTop = 0;
			const int MaxTop = (ItemCount > ListVisibleRows) ? (ItemCount - ListVisibleRows) : 0;
			if (ScrollTop > MaxTop) ScrollTop = MaxTop;
		}

		//리스트 그리기
		for (int i = 0; i < ListVisibleRows; i++) {
			const int Index = ScrollTop + i;

			MoveCursor(ListRow + i, ListCol);
			printf("%*s", InnerWidth, "");

			if (Index >= ItemCount) continue;

			Item* item = CurrentPlayer->GetInventoryItem(Index);
			if (!item) continue;

			MoveCursor(ListRow + i, ListCol);

			const string SellPrice = MoneyFormat(item->GetRealValue());
			const string BuyPrice = MoneyFormat(item->CallValue);

			if (item->Type == Auction.News) {
				if (Index == SelectedIndex) {
					printf("\033[01;33m> %-30s\033[0m", item->Name.c_str());
					MoveCursor(ListRow + i, ListCol + 30);
					printf("\033[01;33m[%10s 원] [%10s 원]\033[0m", SellPrice.c_str(), BuyPrice.c_str());
				}
				else {
					printf("  %-30s", item->Name.c_str());
					MoveCursor(ListRow + i, ListCol + 30);
					printf("[%10s 원] [%10s 원]", SellPrice.c_str(), BuyPrice.c_str());
				}

				MoveCursor(ListRow + i, ListCol + 30);
				if (Auction.NewsType) {
					printf("\033[01;31m[%10s 원]\033[0m", SellPrice.c_str());
				}
				else {
					printf("\033[01;34m[%10s 원]\033[0m", SellPrice.c_str());
				}

			}
			else {
				if (Index == SelectedIndex) {
					printf("\033[01;33m> %-30s\033[0m", item->Name.c_str());
					MoveCursor(ListRow + i, ListCol + 30);
					printf("\033[01;33m[%10s 원] [%10s 원]\033[0m", SellPrice.c_str(), BuyPrice.c_str());
				}
				else {
					printf("  %-30s", item->Name.c_str());
					MoveCursor(ListRow + i, ListCol + 30);
					printf("[%10s 원] [%10s 원]", SellPrice.c_str(), BuyPrice.c_str());
				}
			}
		}

		//다음날로 버튼
		MoveCursor(NextDayRow, ListCol);
		printf("%*s", InnerWidth, "");
		MoveCursor(NextDayRow, ListCol);
		if (SelectedIndex == NextDayIndex) {
			printf("\033[01;33m> [다음날로]\033[0m");
		}
		else {
			printf("  [다음날로]");
		}

		//로그/가이드
		ClearLogSection();

		if (ItemCount == 0) {
			MoveCursor(34, 3);
			printf("판매할 아이템이 없습니다!");
			MoveCursor(35, 3);
			printf("다음날로를 선택하고 Enter를 눌러주세요.");
		}
		else if (SelectedIndex == NextDayIndex) {
			MoveCursor(34, 3);
			printf("다음날로 넘어갑니다.");
			MoveCursor(35, 3);
			printf("Enter를 누르면 경매를 계속합니다.");
		}
		else {
			Item* SelectedItem = CurrentPlayer->GetInventoryItem(SelectedIndex);
			
			if (SelectedItem) {
				MoveCursor(34, 3);
				printf("선택한 아이템 : %s", SelectedItem->Name.c_str());
				MoveCursor(35, 3);
				printf("판매가 : %s 원", MoneyFormat(SelectedItem->GetRealValue()).c_str());
			}

			MoveCursor(37, 3);
			if (Action == 0) {
				printf("\033[01;33m[판매]\033[0m   [보류]");
			}
			else {
				printf("[판매]   \033[01;33m[보류]\033[0m");
			}
		}

		if (!StatusMessage.empty()) {
			MoveCursor(39, 3);
			printf("%*s", 110, "");
			MoveCursor(39, 3);
			printf("%s", StatusMessage.c_str());
		}

		//입력 받기
		while (_kbhit()) _getch();
		int Key = _getch();

		if (Key == 0 || Key == 224) {
			Key = _getch();
			switch (Key) {
			case KEY_UP:
				SelectedIndex--;
				if (SelectedIndex < 0) SelectedIndex = NextDayIndex;
				break;
			case KEY_DOWN:
				SelectedIndex++;
				if (SelectedIndex > NextDayIndex) SelectedIndex = 0;
				break;
			case KEY_LEFT:
			case KEY_RIGHT:
				if (SelectedIndex < ItemCount) {
					Action = 1 - Action;
				}
				break;
			}
		}
		else if (Key == KEY_ENTER) {
			StatusMessage.clear();

			if (SelectedIndex == NextDayIndex) {
				return WeekResultScreen();
			}

			//판매
			if (Action == 0) {
				Item* item = CurrentPlayer->GetInventoryItem(SelectedIndex);
				const string SoldName = item ? item->Name : string("???");
				const string SoldMoney = item ? MoneyFormat(item->GetRealValue()) : string("0");

				if (CurrentPlayer->SellInventoryItem(SelectedIndex)) {
					StatusMessage = SoldName + " 판매 완료! +" + SoldMoney + " 원";
				}
				else {
					StatusMessage = "판매에 실패했습니다...";
				}

				if (CurrentPlayer->GetInventoryCount() == 0) {
					return WeekResultScreen();
				}

				const int NewCount = CurrentPlayer->GetInventoryCount();
				if (SelectedIndex > NewCount) SelectedIndex = NewCount;
			}
			//보류
			else {
				StatusMessage = "보류했습니다.";
				SelectedIndex++;
				if (SelectedIndex > NextDayIndex) SelectedIndex = NextDayIndex;
			}
		}
	}

	return true;
}

void ClearLogSection()
{
	for (int i = 0; i < 11; i++) 
	{
		MoveCursor(34 + i, 2);
		printf("%*s", 118, "");
	}
}

int CustomGetInt(int x, int y) {
	string input = "";
	while (true) {
		MoveCursor(y, x);
		// 입력된 글자 뒤에 공백을 하나 줘서 지워질 때 잔상이 안 남게 합니다.
		printf("%s  ", input.c_str());
		// 커서를 다시 숫자 바로 뒤로 이동 (이게 핵심!)
		MoveCursor(y, x + (int)input.length());

		while (_kbhit()) _getch();
		int c = _getch(); // char 대신 int로 받는 게 확장키 대응에 좋습니다.

		if (c >= '0' && c <= '9') {
			if (input.length() < 10) input += (char)c;
		}
		else if (c == 8) { // 백스페이스
			if (!input.empty()) {
				input.pop_back();
				// 화면에서 한 칸 뒤로 가서 공백 찍고 다시 뒤로
				MoveCursor(y, x + (int)input.length());
				printf("  ");
			}
		}
		else if (c == 13) { // 엔터
			if (input.empty()) continue;

			// 엔터를 치는 순간 그 줄을 깔끔하게 유지하기 위해 루프 종료
			return stoi(input);
		}
		else if (c == KEY_ESC) {
			// 퍼즈 메뉴
			if (!DrawPauseScreen()) return -1; // 메인 메뉴로 나가기
			return -2; // 돌아가기(입력 다시 받기)
		}
	}
}

bool CallValueSequence(Item* NewItem)
{
	auto& Auction = AuctionSystem::GetInstance();

	int InputValue;

	while (true) 
    {
        MoveCursor(34, 3);
        printf("시작가를 입력하세요 : ");
        
        // CustomGetInt가 엔터를 칠 때까지 여기서 대기합니다.
        InputValue = CustomGetInt(25, 34);

		// ESC -> 퍼즈 메뉴 처리 결과
		if (InputValue == -1) {
			return false; // 메인 메뉴로
		}
		if (InputValue == -2) {
			ClearLogSection();
			continue; // 입력 다시 받기
		}

		ClearLogSection();

        // 조건 검사
        if (InputValue > Auction.CurrentPlayer->GetCurrentMoney())
        {
            MoveCursor(35, 3);
            printf("\033[1;31m지갑보다 비싼 시작가는 입력할 수 없어요!\033[0m");
        }
        else if (InputValue < NewItem->MinValue || InputValue > NewItem->MaxValue)
        {
            MoveCursor(35, 3);
            printf("\033[1;31m시작가 범위 내 가격으로 입력해주세요!\033[0m");
        }
         else 
         {
             NewItem->CallValue = (float)InputValue;
             Auction.CurrentPlayer->SetCallValue((float)InputValue);
             return true;
         }
     }

	return true;
}

bool PlayerSelectionSequence(bool& OutExitToMenu)
{
	OutExitToMenu = false;

	int Selected = 0;
	int OptionsSize = 2;
	int IsSelecting = 1;

	auto& Auction = AuctionSystem::GetInstance();

	while (IsSelecting) {

		//ClearLogSection();
		//선택지 출력
		MoveCursor(38, 3);
		printf("%*s", 110, "");
		MoveCursor(38, 3);
		if (Selected == 0) 
		{
			printf("\033[1;33m > 가즈아!!\033[0m");
		}
		else {
			printf("   가즈아!!");
		}

		printf("    ");
		if (Selected == 1) 
		{
			printf("\033[1;33m > 포기할래...\033[0m");
		}
		else
		{
			printf("   포기할래...");
		}

		//플레이어 입력 받기
		while (_kbhit()) _getch();
		int Key = _getch();

		//ESC 입력 시 퍼즈 메뉴
		if (Key == KEY_ESC) {
			if (!DrawPauseScreen()) {
				OutExitToMenu = true;
				return false;
			}
			ClearLogSection();
			continue;
		}

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

	return false;
}

void UpdateItem(Item* NewItem)
{
	if (NewItem) {
		//이름 출력
		MoveCursor(2, 3);
		printf("                           ");
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
		printf("                                                                                ");
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
	printf("%*s", 28, "");
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

const char* GetPurchaseSentence() {

	const char* List[10] = {
		"물건은 %s님이 가져갑니다!",
		"낙찰자는 %s님!",
		"오늘의 승자는 %s!",
		"물건의 주인공은 %s님!",
		"%s님, 대금은 저에게 주고 가시면 됩니다~",
		"치열한 경쟁이었습니다!!",
		"구매해주셔서 감사합니다.",
	};

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, 7);
	int Index = dis(gen);

	return List[Index];
}

const char* GetFailSentece() {
	const char* List[8] = {
		"네! 포기자는 저~~~~ 짝으로 빠져주시기 바랍니다!",
		"정말 아쉽군요!",
		"저런....",
		"돈이 없다니 정말 안타까울 따름입니다!",
		"엥 이걸?",
		"이 물건을 포기하시다니...!",
		"나가는 길은 저 쪽입니다.",
		"안녕히 가세요.^^77"
	};

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, 7);
	int Index = dis(gen);

	return List[Index];
}

void PurchaseSequence(const Item* PurchasedItem)
{
	auto& Auction = AuctionSystem::GetInstance();

	ClearLogSection();
	MoveCursor(34, 3);
	printf(GetPurchaseSentence(), Auction.CurrentPlayer->GetName().c_str());
	while (_kbhit()) _getch();
	_getch();
	MoveCursor(35, 3);
	if (PurchasedItem) {
		string Money = MoneyFormat(PurchasedItem->CallValue);
		printf("낙찰가 : %s 원", Money.c_str());
	}
	else {
		printf("낙찰 정보를 불러오지 못했습니다!");
	}
	MoveCursor(36, 3);
	printf("아무 키나 눌러 넘어가기.");
	while (_kbhit()) _getch();
	_getch();

}

void GiveupSequence()
{
	ClearLogSection();
	MoveCursor(34, 3);
	printf(GetFailSentece());
	while (_kbhit()) _getch();
	_getch();
	ClearLogSection();
}

bool WeekResultScreen()
{
	static const char* GameOverLogo[6] = {
"\033[31m _____   ___  ___  ___ _____   _____  _   _  _____ ______ ",
"|  __ \\ / _ \\ |  \\/  ||  ___| |  _  || | | ||  ___|| ___ \\",
"| |  \\// /_\\ \\| .  . || |__   | | | || | | || |__  | |_/ /",
"| | __ |  _  || |\\/| ||  __|  | | | || | | ||  __| |    / ",
"| |_\\ \\| | | || |  | || |___  \\ \\_/ /\\ \\_/ /| |___ | |\\ \\ ",
" \\____/\\_| |_/\\_|  |_/\\____/   \\___/  \\___/ \\____/ \\_| \\_|\033[0m"
	};

	vector<string> PassDialogue = {
		"끼얏호우~! 쏠쏠하게 벌었다!",
		"이번 주도 알차게 보냈군!",
		"좋은 물건이 많았어...",
		"목표 금액을 채웠다!",
		"빨리 다음 주가 왔으면 좋겠다~",
		"목표치는 채웠구만"
	};

	vector<string> FailDialogue = {
		"역시 난 안되나봐...",
		"왜 이런 물건밖에 안나오는거야!!",
		"내 돈.....",
		"이런 XX",
		"나 안할래."
	};

	vector<string> GoodFailDialogue = {
		"이 정도 벌었으면 됐다..ㅎ",
		"너무 욕심내지 말자",
		"그래 이거라도 어디야!",
		"운은 여기까진가 보다",
		"재미 좀 봤구만!"
	};

	system("cls");

	DrawBox(1, 0, 120, 50);

	for (int i = 0; i < sizeof(GameOverLogo) / sizeof(GameOverLogo[0]); ++i) {
		PrintAsciiAtLocation(4 + i, 31, GameOverLogo[i]);
	}

	AuctionSystem& Auction = AuctionSystem::GetInstance();

	//목표 금액을 넘기면 다음 날로
	if(Auction.CurrentPlayer->GetCurrentMoney() >= Auction.CurrentPlayer->GetGoal()){

		//새로운 목표 금액 설정
		float NewGoal = Auction.CurrentPlayer->GetCurrentMoney() * 1.2;
		Auction.CurrentPlayer->SetGoal(NewGoal);

		//메시지 출력
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(0, PassDialogue.size() - 1);
		int Index = dis(gen);

		TypeWrite(55, 20, PassDialogue[Index]);
		while (_kbhit()) _getch();
		_getch();
		return true;
	}
	//못 넘기면 게임 오버
	else {

		int SpendDay = Auction.Day;
		float FinalMoney = Auction.CurrentPlayer->GetCurrentMoney();
		float GainLoss = Auction.CurrentPlayer->GetCurrentMoney() - Auction.CurrentPlayer->GetInitMoney();
		float TotalBuyValue = Auction.CurrentPlayer->GetTotalBuyValue();
		float TotalSellValue = Auction.CurrentPlayer->GetTotalSellValue();
		int BuyItemCount = Auction.CurrentPlayer->GetBuyItemCount();
		int SellItemCount = Auction.CurrentPlayer->GetSellItemCount();
		float EarningRate = ((Auction.CurrentPlayer->GetCurrentMoney() - Auction.CurrentPlayer->GetInitMoney()) / Auction.CurrentPlayer->GetInitMoney()) * 100;

		string FinalMoneyString = MoneyFormat(FinalMoney);
		string GainLossString = MoneyFormat(GainLoss);
		string TotalBuyValueString = MoneyFormat(TotalBuyValue);
		string TotalSellValueString = MoneyFormat(TotalSellValue);

		DrawBox(14, 30, 60, 22);
		
		//메시지 출력
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(0, FailDialogue.size() - 1);
		int Index = dis(gen);
		TypeWrite(55, 12, EarningRate<0 ? FailDialogue[Index] : GoodFailDialogue[Index]);

		MoveCursor(14, 35);
		printf("%s : %d일", "진행한 날 수", SpendDay);

		MoveCursor(17, 35);
		printf("총 구매한 경매품 개수");
		MoveCursor(17, 72);
		printf("%10d 개", BuyItemCount);

		MoveCursor(19, 35);
		printf("총 판매한 경매품 개수");
		MoveCursor(19, 72);
		printf("%10d 개", SellItemCount);

		MoveCursor(21, 35);
		printf("총 구매 금액");
		MoveCursor(21, 62);
		printf("%20s 원", TotalBuyValueString.c_str());

		MoveCursor(23, 35);
		printf("총 판매 금액");
		MoveCursor(23, 62);
		printf("%20s 원", TotalSellValueString.c_str());

		MoveCursor(25, 35);
		printf("잔액");
		MoveCursor(25, 62);
		printf("%20s 원", FinalMoneyString.c_str());

		MoveCursor(27, 35);
		printf("손익");
		MoveCursor(27, 62);
		if (GainLoss < 0) {
			printf("\033[01;34m%20s\033[0m 원", GainLossString.c_str());
		}
		else {
			printf("\033[01;31m%20s\033[0m 원", GainLossString.c_str());
		}

		MoveCursor(29, 35);
		printf("수익률");
		MoveCursor(29, 72);
		if (EarningRate < 0) {
			printf("\033[01;34m%10.2f %%\033[0m", EarningRate);
		}
		else {
			printf("\033[01;31m%10.2f %%\033[0m", EarningRate);
		}

		MoveCursor(33, 46);
		printf("아무 키나 눌러 메인메뉴로...");

		while (_kbhit()) _getch();
		_getch();

		Auction.ResetGame();
		return false;
	}
}
