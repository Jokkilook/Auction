#include "Player.h"
#include "Item.h"
#include "AuctionSystem.h"

Player::Player()
{
	AddMoney(5000000);

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

	Item* item = Inventory[Index];
	if (item) {
		AddMoney(item->GetRealValue());
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
