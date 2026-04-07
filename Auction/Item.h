#pragma once
#include <stdio.h>
#include <string>
#include <vector>

using namespace std;

enum ItemType {
	NONE,
	ART,
	RARE,
	ACIENT,
	MASTERPIECE,
	NAMED
};

struct ItemData {
	ItemType Type;
	string Name;
	string Description;
	string AsciiArt;
	float Value;
};

class Item
{
	float RealValue = 0;

public:

	string Name;
	string Description;
	string AsciiArt;

	ItemType Type = NONE;

	float MinValue = 0;
	float MaxValue = 0;
	float CallValue = 0;

	//생성자
	Item();

	//감정가 범위 세팅 함수
	void SetEstimatedRange();

	//실제 감정가 수정 함수
	void SetRealValue(float NewValue) { RealValue = NewValue; };

	//실제 감정가 반환 함수
	float GetRealValue() const { return RealValue; };

	float GetRandom(float RangeA, float RangeB);

	int GetRandom(int RangeA, int RangeB);
};