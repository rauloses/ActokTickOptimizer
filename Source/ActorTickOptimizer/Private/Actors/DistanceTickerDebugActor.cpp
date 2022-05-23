// TODO: License GPL 3.0?

#include "Actors/DistanceTickerDebugActor.h"
#include "ActorTickableOptimizerSubsystem.h"
#include "Components/DistanceTickerOptimizerPOI.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "TickOptimizers/DistanceATickerOptimizer.h"


ADistanceTickerDebugActor::ADistanceTickerDebugActor()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void ADistanceTickerDebugActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

#if !UE_BUILD_SHIPPING
	if (UActorTickableOptimizerSubsystem* TickOptimizerSubsystem = GetWorld()->GetSubsystem<UActorTickableOptimizerSubsystem>())
	{
		if (UDistanceATickerOptimizer* Optimizer = TickOptimizerSubsystem->GetOptimizer<UDistanceATickerOptimizer>(OptimizerClass))
		{
			TArray<FVector> PointsOfInterest;
			for (const UDistanceTickerOptimizerPOI* POI : Optimizer->GetRegisteredPOIs())
			{
				PointsOfInterest.Add(POI->GetPOIPosition());
			}

			if (!PointsOfInterest.IsEmpty())
			{
				DrawDebugFalloffCurve(GetWorld(), DebugData.bUseExternalCurve ? DebugData.ExternalCurve : Optimizer->GetTickingFalloffCurve(), DebugData, PointsOfInterest);
			}
		}
	}
#endif
}

#if !UE_BUILD_SHIPPING
void ADistanceTickerDebugActor::DrawDebugFalloffCurve(const UWorld* World, const UCurveFloat* Curve, const FDistanceTickerDebugData& DebugData, const TArray<FVector>& PointsOfInterest)
{
	if (!DebugData.bDrawDebugCurve)
	{
		return;
	}
	
	FVector Center = FVector::ZeroVector;
	for (const FVector& POI : PointsOfInterest)
	{
		Center += POI;
	}
	Center = Center / PointsOfInterest.Num();

	// TODO: Find a better why to display the area of influence, textures?
	// Ideally it would be much better to create a texture and display it somewhere, or find a way to
	// render this spheres using instancing instead of one by one.
	// If anything is possible, at least let them live for a couple of seconds before redrawing them. 
	
	for (float I=-DebugData.DebugAreaLength; I <= DebugData.DebugAreaLength; I += DebugData.DebugAreaDelta)
	{
		for (float J=-DebugData.DebugAreaLength; J <= DebugData.DebugAreaLength; J += DebugData.DebugAreaDelta)
		{
			const FVector Position = Center + FVector(I, J, DebugData.DebugZPosition);
			const float SqrDistance = GetMinimumSqrDistance(Position, PointsOfInterest);
			const float TickIntervalNormalized = FMath::Min(Curve->GetFloatValue(SqrDistance), DebugData.DebugMaxFarDistanceForColoring) / DebugData.DebugMaxFarDistanceForColoring;
			DrawDebugSphere(World, Position, DebugData.DebugAreaDelta * 0.25f, DebugData.DebugSegments, FLinearColor::LerpUsingHSV(DebugData.DebugCloseColor, DebugData.DebugFarColor, TickIntervalNormalized).ToFColor(true));
		}
	}
}

float ADistanceTickerDebugActor::GetMinimumSqrDistance(const FVector& PointA, const TArray<FVector>& Points)
{
	float MinDistance = FVector::DistSquared(Points[0], PointA);
	for (int32 I = 1; I < Points.Num(); I++)
	{
		const float NewDist = FVector::DistSquared(Points[I], PointA);
		if (NewDist < MinDistance)
		{
			MinDistance = NewDist;
		}
	}
	return MinDistance;
}
#endif
