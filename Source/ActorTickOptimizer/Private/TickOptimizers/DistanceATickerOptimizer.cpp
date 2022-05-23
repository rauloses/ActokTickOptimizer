// TODO: License GPL 3.0?

#include "TickOptimizers/DistanceATickerOptimizer.h"

#include "ActorTickableOptimizerSubsystem.h"
#include "Components/DistanceTickerOptimizerPOI.h"


UDistanceATickerOptimizer::UDistanceATickerOptimizer()
	: Super()
	, bAddRandomValueToInterval(true)
{

}

void UDistanceATickerOptimizer::RegisterActor(AActor* Actor)
{
	ActorsToOptimize.Add(Actor);	
}

void UDistanceATickerOptimizer::UnregisterActor(AActor* Actor)
{
	ActorsToOptimize.RemoveSwap(Actor, false);
}

void UDistanceATickerOptimizer::ResetOptimizer()
{
	ActorsToOptimize.Empty();
}

void UDistanceATickerOptimizer::RegisterPOI(class UDistanceTickerOptimizerPOI* POI)
{
	POIs.Add(POI);
}

void UDistanceATickerOptimizer::UnregisterPOI(class UDistanceTickerOptimizerPOI* POI)
{
	POIs.RemoveSwap(POI);
}

void UDistanceATickerOptimizer::ClearPOIs()
{
	POIs.Empty();
}

void UDistanceATickerOptimizer::OptimizeTicks(float DeltaTime)
{
	if (!TickingFalloffCurve)
	{
		return;
	}

	TArray<FVector> PointsOfInterest;
	GetPOIsPositions(PointsOfInterest);
	if (PointsOfInterest.IsEmpty())
	{
		return;
	}

	SCOPED_NAMED_EVENT(UDistanceATickerOptimizer_OptimizeTicks, FColor::Cyan);

	for (AActor* Actor : ActorsToOptimize)
	{
		if (!Actor || Actor->IsActorBeingDestroyed())
		{
			continue;
		}

		const FVector ActorLocation = Actor->GetActorLocation();
		const bool bShouldBeReset = UpdateTickInterval(ActorLocation, PointsOfInterest, Actor->PrimaryActorTick);
		if (bShouldBeReset)
		{
			GetSubsystem()->RegisterActorForTickIntervalReset(Actor);
		}
	}
}