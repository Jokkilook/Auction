#pragma once
#include <stdio.h>
#include <string.h>

class Item
{
	char* Name[10];
	char* Description[100];

public:

	float RealValue;
	float MinValue;
	float MaxValue;
	float CallValue;

	//감정가 범위 세팅 함수
	void SetEstimatedRange();
};