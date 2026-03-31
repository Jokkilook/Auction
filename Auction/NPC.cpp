#include "NPC.h"
#include "math.h"
#include <random>

void NPC::SetMaxValue(Item* TargetItem)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis(1.5f, 2.0f);
	float randomValue = dis(gen);

	MaxValue = TargetItem->RealValue * randomValue;
}
