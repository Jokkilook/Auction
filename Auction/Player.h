#pragma once

#include <vector>
#include "Item.h"
#include "Participant.h"

using namespace std;

class Player : public Participant
{
	string Name = "스꾸삐";
	vector<Item*> Inventory;

	float Goal;
	float InitMoney = 5000000.0f;
	int BuyItemCount = 0;
	int SellItemCount = 0;
	float TotalBuyValue = 0.0f;
	float TotalSellValue = 0.0f;

public:

	Player();
	~Player() override;

	//인벤토리 반환
	vector<Item*> GetInventory() const { return Inventory; };

	//인벤토리 개수 반환
	int GetInventoryCount() const;

	//인벤토리 아이템 반환(없으면 nullptr)
	Item* GetInventoryItem(int Index) const;

	//인벤토리 아이템 판매(실제 감정가로 판매 후 삭제)
	bool SellInventoryItem(int Index);

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
	
	//목표 금액 설정 함수
	void SetGoal(float NewGoal) { Goal = NewGoal; };

	//목표 금액 반환 함수
	float GetGoal() const { return Goal; };

	//초기 금액 설정 함수
	void SetInitMoney(float NewInit) { InitMoney = NewInit; }

	//초기 금액 반환 함수
	float GetInitMoney() const { return InitMoney; }

	//구매 아이템 개수 반환 함수
	int GetBuyItemCount() const { return BuyItemCount; }

	//총 구매 가격 반환 함수
	float GetTotalBuyValue() const { return TotalBuyValue; }

	//판매 아이템 개수 반환 함수
	int GetSellItemCount() const { return SellItemCount; }

	//총 판매 가격 반환 함수
	float GetTotalSellValue() const { return TotalSellValue; }
};