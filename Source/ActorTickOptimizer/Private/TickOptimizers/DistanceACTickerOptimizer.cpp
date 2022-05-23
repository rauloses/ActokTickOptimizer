// TODO: License GPL 3.0?

#include "TickOptimizers/DistanceACTickerOptimizer.h"

#include "ActorTickableOptimizerSubsystem.h"
#include "TickOptimizers/DistanceATickerOptimizer.h"


void UDistanceACTickerOptimizer::RegisterActorComponent(UActorComponent* Component)
{
	ActorComponentsToOptimize.Add(Component);
}

void UDistanceACTickerOptimizer::UnregisterActorComponent(UActorComponent* Component)
{
	ActorComponentsToOptimize.RemoveSwap(Component, false);
}

void UDistanceACTickerOptimizer::ResetOptimizer()
{
	ActorComponentsToOptimize.Empty();
}

void UDistanceACTickerOptimizer::OptimizeTicks(float DeltaTime)
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

	SCOPED_NAMED_EVENT(UDistanceACTickerOptimizer_OptimizeTicks, FColor::Cyan);

	for (UActorComponent* Component : ActorComponentsToOptimize)
	{
		if (!Component || Component->IsBeingDestroyed())
		{
			continue;
		}

		const FVector ActorLocation = Component->GetOwner()->GetActorLocation();
		const bool bShouldBeReset = UpdateTickInterval(ActorLocation, PointsOfInterest, Component->PrimaryComponentTick);
		if (bShouldBeReset)
		{
			GetSubsystem()->RegisterActorComponentForTickIntervalReset(Component);
		}
	}
}