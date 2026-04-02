#pragma once

#include <vector>
#include "Item.h"
#include "Participant.h"

using namespace std;

class Player : public Participant
{
	string Name = "빵빵이";
	vector<Item*> Inventory;

public:

	Player();

	//인벤토리에 아이템 넣는 함수
	bool PutItemToInventory(Item* NewItem);

	//아이템 구매 함수
	bool PurchaseItem(Item* NewItem);

	//아이템 처리 함수
	bool HandleItem(Item* TargetItem);

	//이름 반환 함수
	string GetName() const { return Name; };

	//이름 설정 함수
	void SetName(string NewName) { Name = NewName; };
};