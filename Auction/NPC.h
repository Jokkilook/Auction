#pragma once
#include "Item.h"

class NPC
{
	float CallValue;
	float MaxValue;

	//��Ű� ��� �θ��� �Լ�
	
	//NPC가 포기할 상한선 설정 함수
	void SetMaxValue(Item* TargetItem);
};