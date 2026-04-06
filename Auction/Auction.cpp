#include <iostream>
#include  "Drawer.h"
#include <windows.h>

void EnableConsoleUtf8AndVT()
{
	// UTF-8 출력(한글/박스 문자 깨짐 방지)
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);

	// ANSI Escape Sequence(커서 이동/색상) 활성화
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hOut == INVALID_HANDLE_VALUE) return;

	DWORD mode = 0;
	if (!GetConsoleMode(hOut, &mode)) return;

	mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	SetConsoleMode(hOut, mode);
}

void FixConsoleSize() {
	// 1. 현재 콘솔 창의 핸들(ID)을 가져옵니다.
	HWND hwnd = GetConsoleWindow();

	// 2. 창의 현재 스타일 정보를 가져옵니다.
	long style = GetWindowLong(hwnd, GWL_STYLE);

	// 3. 스타일에서 '크기 조절(WS_THICKFRAME)'과 '최대화 버튼(WS_MAXIMIZEBOX)' 기능을 뺍니다.
	style &= ~WS_THICKFRAME;
	style &= ~WS_MAXIMIZEBOX;

	// 4. 수정한 스타일을 창에 다시 적용합니다.
	SetWindowLong(hwnd, GWL_STYLE, style);

	// 5. 변경사항을 즉시 반영하여 창을 갱신합니다.
	SetWindowPos(hwnd, NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
}

int main()
{
	//콘솔 설정
	SetConsoleTitle(L"레전드 경매 게임");
	EnableConsoleUtf8AndVT();
	system("mode con:cols=120 lines=45");
	FixConsoleSize();
	
	DrawMainMenu();
}

//게임 흐름
//1. 게임 시작 - 메인화면
//1-1. 게임 시작 클릭
//1-2. 프롤로그 출력
// - 플레이어가 경매에 참여하게 된 계기를 텍스트로 출력
//1-3. 플레이어 초기화
//2. 경매 시스템 시작
//2-1. 해당 일 아이템 초기화
//2-2. NPC 초기화
//2-3. 
