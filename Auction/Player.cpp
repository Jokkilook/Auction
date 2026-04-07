#include "Player.h"
#include "Item.h"
#include "AuctionSystem.h"

Player::Player()
{
	float InitMoney = 5000000.0f;
	float Goal = InitMoney * 1.1f;

	AddMoney(InitMoney);
	SetGoal(Goal);

	for (int i = 0; i < 20; i++) {
		//PutItemToInventory(new Item());
	}
}

Player::~Player()
{
	for (Item* item : Inventory) {
		delete item;
	}
	Inventory.clear();
}

int Player::GetInventoryCount() const
{
	return static_cast<int>(Inventory.size());
}

Item* Player::GetInventoryItem(int Index) const
{
	if (Index < 0 || Index >= static_cast<int>(Inventory.size())) return nullptr;
	return Inventory[Index];
}

bool Player::SellInventoryItem(int Index)
{
	if (Index < 0 || Index >= static_cast<int>(Inventory.size())) return false;

	auto& Auction = AuctionSystem::GetInstance();

	Item* item = Inventory[Index];
	if (item) {
		//호재&악재 뜨면 반영된 가격으로 판매
		if (item->Type == Auction.News) {
			float SellValue = item->GetRealValue() * 
				(Auction.NewsType ? 1.3f : 0.7f);

			AddMoney(SellValue);
		}
		else {
			AddMoney(item->GetRealValue());
		}
		
		delete item;
	}

	Inventory.erase(Inventory.begin() + Index);
	return true;
}

bool Player::PutItemToInventory(Item* NewItem)
{
	if (Inventory.size() >= 20) return false;

	Inventory.push_back(NewItem);

	return true;
}

bool Player::PurchaseItem(Item* NewItem)
{
	//돈 없음
	if (NewItem->CallValue > CurrentMoney) return false;

	//인벤토리 넣기 시도
	if (PutItemToInventory(NewItem)) {
		//성공하면 돈 차감
		SpendMoney(NewItem->CallValue);
		return true;
	}

	//풀 인벤
	return false;
}

bool Player::HandleItem(Item* TargetItem) 
{
	auto& Auction = AuctionSystem::GetInstance();
	CallValue = TargetItem->CallValue + Auction.Increament;

	if (CallValue > CurrentMoney) 
	{
		return false;
	}

	TargetItem->CallValue = CallValue;
	return true;
}
