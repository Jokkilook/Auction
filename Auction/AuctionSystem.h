#pragma once
#include "Player.h"
#include "NPC.h"
#include <vector>

using namespace std;

class AuctionSystem
{

public:

	int Day = 0;
	int Week = 1;

	Player* Player;

	vector<NPC*> Npcs;

	Item* AuctionItem;

	//경매 시작 함수
	void StartAuction();

	//경매 종료 함수
	void EndAuction();
};
