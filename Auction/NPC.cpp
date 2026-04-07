#include "NPC.h"
#include "math.h"
#include <random>
#include "AuctionSystem.h"

NPC::NPC()
{

}

void NPC::SetMaxValue(Item* TargetItem)
{
	random_device rd;
	mt19937 gen(rd());
	uniform_real_distribution<float> dis(0.5f, 0.9f);
	float randomValue = dis(gen);

	MaxValue = TargetItem->GetRealValue() * randomValue;
}

void NPC::HandleItem(Item* TargetItem)
{
	if (State == DROP || State == READY) return;

	//블러핑 발동!
	random_device rd;
	mt19937 gen(rd());
	uniform_real_distribution<float> dis(0.0f, 1.0f);
	float randomValue = dis(gen);

	if (randomValue < 0.05f) {
		auto& Auction = AuctionSystem::GetInstance();
		CallValue = TargetItem->CallValue + Auction.Increament * 5.0f;
		TargetItem->CallValue = CallValue;
	}
	else {
		//지가 생각한 상한선 보다 비싸면 포기
		if (TargetItem->CallValue > MaxValue) {
			SetState(STATE::DROP);
		}
		//아니면 가격 올려 부르기
		else {
			auto& Auction = AuctionSystem::GetInstance();
			CallValue = TargetItem->CallValue + Auction.Increament;
			TargetItem->CallValue = CallValue;
		}
	}
}
