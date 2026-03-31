#pragma once

#include <vector>
#include "Item.h"

using namespace std;

class Player
{

	float CurrentMoney;

	vector<Item*> Inventory;

	//인벤토리에 아이템 넣는 함수
	bool PutItemToInventory(Item* NewItem);
};