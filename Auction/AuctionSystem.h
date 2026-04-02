#pragma once
#include "Player.h"
#include "NPC.h"
#include <vector>

using namespace std;

//싱글톤 클래스
class AuctionSystem
{
	AuctionSystem();

public:

	int Day = 1;
	int Week = 1;

	float Increament = 100000;

	Player* CurrentPlayer;

	vector<NPC*> Npcs;

	vector<Participant*> Participants;

	Item* AuctionItem;

	static AuctionSystem& GetInstance() {
		static AuctionSystem Instance;
		return Instance;
	}

	void SetPlayer(Player* NewPlayer) { CurrentPlayer = NewPlayer; };

	void SetIncreament(float NewIncreament) { Increament = NewIncreament; };

	//참가자들 참가 상태로
	void JoinParticipants();

	//NPC 포기 상태 확인;
	bool CheckAllNPCGiveup();

	//새로운 아이템으로 갈아끼우기
	void SetNewItem();

	//경매 시작 함수
	void StartAuction();

	//경매 종료 함수
	void EndAuction();	
};
