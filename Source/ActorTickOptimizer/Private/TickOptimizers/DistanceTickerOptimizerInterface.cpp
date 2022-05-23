// TODO: License GPL 3.0?

#include "TickOptimizers/DistanceTickerOptimizerInterface.h"
#include "Components/DistanceTickerOptimizerPOI.h"

UDistanceTickerOptimizerInterface::UDistanceTickerOptimizerInterface(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

const TArray<UDistanceTickerOptimizerPOI*>& IDistanceTickerOptimizerInterface::GetRegisteredPOIs() const
{
	static TArray<UDistanceTickerOptimizerPOI*> EmptyPOIs;
	return EmptyPOIs;
}

void IDistanceTickerOptimizerInterface::GetPOIsPositions(TArray<FVector>& PointsOfInterest) const
{
	for (const UDistanceTickerOptimizerPOI* POI : GetRegisteredPOIs())
	{
		PointsOfInterest.Add(POI->GetPOIPosition());
	}
}

bool IDistanceTickerOptimizerInterface::UpdateTickInterval(const FVector& Location, const TArray<FVector>& PointsOfInterest, FTickFunction& TickFunction) const
{
	const float SqrDistance = GetMinimumSqrDistance(Location, PointsOfInterest);
	const float CurveValue = GetTickingFalloffCurve()->GetFloatValue(SqrDistance);

	const float TickInterval = ShouldAddRandomValue() ? CurveValue + FMath::FRand() * CurveValue : CurveValue;
	const float OldTickInterval = TickFunction.TickInterval;
	TickFunction.TickInterval = TickInterval;

	return OldTickInterval >= TickInterval * GetDeltaToForceResetTicking();
}

float IDistanceTickerOptimizerInterface::GetMinimumSqrDistance(const FVector& PointA, const TArray<FVector>& Points)
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