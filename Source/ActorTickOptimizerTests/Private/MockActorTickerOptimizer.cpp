// TODO: License GPL 3.0?

#include "MockActorTickerOptimizer.h"

void UMockActorTickerOptimizer::RegisterActor(AActor* Actor)
{
	if (MockRegisteredActor == nullptr)
	{
		MockRegisteredActor = Actor;
	}
}

void UMockActorTickerOptimizer::UnregisterActor(AActor* Actor)
{
	if (MockRegisteredActor == Actor)
	{
		MockRegisteredActor = nullptr;
	}
}

void UMockActorTickerOptimizer::ResetOptimizer()
{ 
	MockRegisteredActor = nullptr; 
}