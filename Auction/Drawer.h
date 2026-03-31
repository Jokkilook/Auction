#pragma once

#include <stdio.h>
#include "AuctionSystem.h"

//마우스 커서 위치 이동
void MoveCursor(int row, int col);

//네모 그리기 함수
void DrawBox(int row, int col, int width, int height);

//메인 메뉴 그리기 함수
void DrawMainMenu();

//경매 틀 그리기 함수
void DrawAuctionScreen(AuctionSystem* CurrentAuction);