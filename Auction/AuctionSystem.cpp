#include "AuctionSystem.h"
#include <random>

AuctionSystem::~AuctionSystem()
{
	if (AuctionItem) {
		delete AuctionItem;
		AuctionItem = nullptr;
	}

	for (NPC* npc : Npcs) {
		delete npc;
	}
	Npcs.clear();

	Participants.clear();

	if (CurrentPlayer) {
		delete CurrentPlayer;
		CurrentPlayer = nullptr;
	}
}

AuctionSystem::AuctionSystem()
{
	Player* MainPlayer = new Player();
	SetPlayer(MainPlayer);
	Participants.push_back(CurrentPlayer);

	StartAuction();
}

void AuctionSystem::JoinParticipants()
{
	//준비 상태의 참가자들을 참가 상태로 전환
	for (Participant* Part : Participants) 
	{
		if (Part->GetState() == READY) {
			Part->SetState(STATE::JOIN);
		}
	}
}

bool AuctionSystem::CheckAllNPCGiveup()
{
	for (NPC* Npc : Npcs) 
	{
		if (Npc->GetState() != DROP) return false;
	}

	return true;
}

void AuctionSystem::SetNewItem()
{
	if (AuctionItem) {
		delete AuctionItem;
		AuctionItem = nullptr;
	}

	AuctionItem = new Item();
	AuctionItem->SetEstimatedRange();
}

void AuctionSystem::StartAuction()
{
	SetNewItem();

	CurrentPlayer->SetCallValue(0);
	CurrentPlayer->SetState(READY);

	for (int i = 0; i < 3; i++) 
	{
		NPC* Npc = new NPC();
		Npc->SetMaxValue(AuctionItem);
		Npcs.push_back(Npc);
		Participants.push_back(Npc);
	}
}

void AuctionSystem::EndAuction()
{
	if (AuctionItem) {
		delete AuctionItem;
		AuctionItem = nullptr;
	}

	for (NPC* npc : Npcs) {
		delete npc;
	}
	Npcs.clear();

	Participants.clear();

	if (CurrentPlayer) {
		Participants.push_back(CurrentPlayer);
	}

	Day++;
	if (Day > 7) {
		Week++;
		Day = 1;
	}
}

void AuctionSystem::SetNews()
{
	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(0, ItemType::NAMED - 1);

	ItemType randomType = static_cast<ItemType>(dis(gen));
	News = randomType;

	std::uniform_int_distribution<int> diss(0, 1);
	NewsType = diss(gen);
}

void AuctionSystem::ApplyNewsToItem(Item* TargetItem)
{
	if (News == TargetItem->Type) {
		
		if (NewsType) {
			float EfftectedValue = TargetItem->GetRealValue() * GetRandom(1.05f, 1.3f);
			TargetItem->SetRealValue(EfftectedValue);
		}
		else {
			float EfftectedValue = TargetItem->GetRealValue() * GetRandom(0.7f, 0.95f);
			TargetItem->SetRealValue(EfftectedValue);
		}
	}
}

float AuctionSystem::GetRandom(float RangeA, float RangeB)
{
	random_device rd;
	mt19937 gen(rd());
	uniform_real_distribution<float> dis(RangeA, RangeB);
	//랜덤 인수
	float R = dis(gen);

	return R;
}

int AuctionSystem::GetRandom(int RangeA, int RangeB)
{
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<int> dis(RangeA, RangeB);
	//랜덤 인수
	float R = dis(gen);

	return R;
}
