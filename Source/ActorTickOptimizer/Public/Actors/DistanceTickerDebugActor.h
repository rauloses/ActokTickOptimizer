// TODO: License GPL 3.0?

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Curves/CurveFloat.h"
#include "DistanceTickerDebugActor.generated.h"

// Debug Data used to draw the area of influence for the distance ticker optimizer
USTRUCT()
struct FDistanceTickerDebugData
{
	GENERATED_BODY()

public:
	// True to draw the falloff curve
	UPROPERTY(EditAnywhere, Category="Debug Optimizer")
	uint8 bDrawDebugCurve : 1;

	// Increments used to draw the spheres
	UPROPERTY(EditAnywhere, Category="Debug Optimizer")
	float DebugAreaDelta = 10.0f;

	// Length used to generate the drawn area 
	UPROPERTY(EditAnywhere, Category="Debug Optimizer")
	float DebugAreaLength = 1000.0f;

	// Z value of position where the debug geometry will be placed
	UPROPERTY(EditDefaultsOnly, Category="Debug Optimizer")
	float DebugZPosition = 50.0f;

	// Amount of segments used for each debug draw
	UPROPERTY(EditDefaultsOnly, Category="Debug Optimizer")
	int32 DebugSegments = 2;
	
	// Color used to draw the tick interval when is closer to a POI
	UPROPERTY(EditDefaultsOnly, Category="Debug Optimizer")
	FLinearColor DebugCloseColor = FColor::Red;

	// Color used to draw the tick interval when is far away from a POI
	UPROPERTY(EditDefaultsOnly, Category="Debug Optimizer")
	FLinearColor DebugFarColor = FColor::Green;

	// Max allowed distance to blend between red and green
	UPROPERTY(EditAnywhere, Category="Debug Optimizer")
	float DebugMaxFarDistanceForColoring = 120.0f;

	// Allow to override the distance curve
	UPROPERTY(EditAnywhere, Category="Debug Optimizer")
	uint8 bUseExternalCurve : 1;

	// External curve to debug
	UPROPERTY(EditAnywhere, Category="Debug Optimizer")
	UCurveFloat* ExternalCurve;
};

/**
 * Draw distance ticker optimizer data
 */
UCLASS(BlueprintType, Blueprintable)
class ACTORTICKOPTIMIZER_API ADistanceTickerDebugActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Constructor
	ADistanceTickerDebugActor();

protected:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

#if !UE_BUILD_SHIPPING
	// Draw the curve around point of interests
	static void DrawDebugFalloffCurve(const UWorld* World, const UCurveFloat* Curve, const FDistanceTickerDebugData& DebugData, const TArray<FVector>& PointsOfInterest);

	// Gets the minimum distance from point A to one of the points in the given list
	static float GetMinimumSqrDistance(const FVector& PointA, const TArray<FVector>& Points);
#endif

	// Distancer class to draw
	UPROPERTY(EditAnywhere, Category="Debug")
	TSubclassOf<class UDistanceATickerOptimizer> OptimizerClass;
	
	// Data used to draw this ticker optimizer influence area
	UPROPERTY(EditAnywhere, Category="Debug")
	FDistanceTickerDebugData DebugData;
};
