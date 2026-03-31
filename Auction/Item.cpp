#include "Item.h"
#include <random>

void Item::SetEstimatedRange()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis(0.0f, 1.0f);
	//랜덤 인수
	float R = dis(gen);

	//오차범위
	float W = RealValue * 0.8;
	
	//감정 최소값
	MinValue = RealValue - (W * R);
	//감정 최대값
	MaxValue = RealValue + (W * (1.0f - R));
}
