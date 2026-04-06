#pragma once

#include <stdio.h>
#include "AuctionSystem.h"

//마우스 커서 위치 이동
void MoveCursor(int row, int col);

//네모 그리기 함수
void DrawBox(int row, int col, int width, int height);

//아스키 아트 그리기 함수


//메인 메뉴 그리기 함수
void DrawMainMenu();

//프롤로그 그리기 함수
void DrawPrologue();

//경매 그리기 함수
void DrawAuctionScreen();

//정산날 그리기 함수
void DrawCalculateDay();

//로그 섹션 클리어 함수
void ClearLogSection();

//경매가 입력 시퀀스
void CallValueSequence(Item* NewItem);

//플레이어 선택 시퀀스
bool PlayerSelectionSequence();

//아이템 데이터 업데이트 함수
void UpdateItem(Item* NewItem);

//참가자 슬롯 업데이트 함수
void UpdateSlot(Participant* Part, int Index);

//플레이어 지갑 업데이트 함수
void UpdateWallet(Player* Player);

//플레이어 구매 시퀀스
void PurchaseSequence(const Item* PurchasedItem);

//플레이어 포기 시퀀스
void GiveupSequence();
