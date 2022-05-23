// TODO: License GPL 3.0?

#include "TickOptimizers/DistanceCMTickerOptimizer.h"

#include "ActorTickableOptimizerSubsystem.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TickOptimizers/DistanceATickerOptimizer.h"

void UDistanceCMTickerOptimizer::RegisterActorComponent(UActorComponent* Component)
{
	ComponentsToOptimize.Add(CastChecked<UCharacterMovementComponent>(Component));
}

void UDistanceCMTickerOptimizer::UnregisterActorComponent(UActorComponent* Component)
{
	ComponentsToOptimize.RemoveSwap(CastChecked<UCharacterMovementComponent>(Component), false);
}

void UDistanceCMTickerOptimizer::ResetOptimizer()
{
	ComponentsToOptimize.Empty();
}

void UDistanceCMTickerOptimizer::OptimizeTicks(float DeltaTime)
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

	SCOPED_NAMED_EVENT(UDistanceCMTickerOptimizer_OptimizeTicks, FColor::Cyan);

	for (UCharacterMovementComponent* Component : ComponentsToOptimize)
	{
		if (!Component || Component->IsBeingDestroyed())
		{
			continue;
		}

		AActor* CMOwner = Component->GetOwner();
		const FVector ActorLocation = CMOwner->GetActorLocation();
		const FVector LastFrameVel = Component->GetLastUpdateVelocity();
		const FVector ProjectedLocation = ActorLocation + LastFrameVel * Component->PrimaryComponentTick.TickInterval;

#if !UE_BUILD_SHIPPING
		if (bDrawDebugTargetLocation)
		{
			DrawDebugSphere(GetWorld(), ProjectedLocation, 24.f, 4, FColor::Cyan);
		}
#endif

		const bool bShouldBeReset = UpdateTickInterval(ProjectedLocation, PointsOfInterest, Component->PrimaryComponentTick);

		CMOwner->PrimaryActorTick.TickInterval = Component->PrimaryComponentTick.TickInterval;

		if (bShouldBeReset)
		{
			GetSubsystem()->RegisterActorComponentForTickIntervalReset(Component);
			GetSubsystem()->RegisterActorForTickIntervalReset(CMOwner);
		}
	}
}