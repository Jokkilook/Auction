#include "AuctionSystem.h"

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
	AuctionItem = nullptr;

	for (NPC* npc : Npcs) {
		delete npc;
	}
	Npcs.clear();

	Participants.clear();

	if (CurrentPlayer) {
		Participants.push_back(CurrentPlayer);
	}

	Day++;
	if (Day % 7 == 0) Week++;
}
