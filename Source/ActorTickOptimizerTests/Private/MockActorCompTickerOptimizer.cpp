// TODO: License GPL 3.0?

#include "MockActorCompTickerOptimizer.h"

void UMockActorCompTickerOptimizer::RegisterActorComponent(UActorComponent* Component)
{
	if (MockRegisteredActorComponent == nullptr)
	{
		MockRegisteredActorComponent = Component;
	}
}

void UMockActorCompTickerOptimizer::UnregisterActorComponent(UActorComponent* Component)
{
	if (MockRegisteredActorComponent == Component)
	{
		MockRegisteredActorComponent = nullptr;
	}
}

void UMockActorCompTickerOptimizer::ResetOptimizer()
{
	MockRegisteredActorComponent = nullptr;
}
