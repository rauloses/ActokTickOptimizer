// TODO: License GPL 3.0?

#pragma once

#include "CoreMinimal.h"
#include "BaseDistanceACTickerOptimizer.h"
#include "DistanceCMTickerOptimizer.generated.h"

/**
 * Reduce the Tick Interval using the minimum distance from the projected movement to the closest POI
 */
UCLASS(NotBlueprintType, Blueprintable)
class ACTORTICKOPTIMIZER_API UDistanceCMTickerOptimizer final : public UBaseDistanceACTickerOptimizer
{
	GENERATED_BODY()

public:
	// Begin UBaseDistanceACTickerOptimizer interface
	virtual void RegisterActorComponent(UActorComponent* Component) override;
	virtual void UnregisterActorComponent(UActorComponent* Component) override;
	virtual void ResetOptimizer() override;
	virtual void OptimizeTicks(float DeltaTime) override;
	// End UBaseDistanceACTickerOptimizer interface

protected:
	// True to draw the target location
	UPROPERTY(EditDefaultsOnly, Category="Tick Optimizer")
	uint8 bDrawDebugTargetLocation : 1;

private:
	// List with all character movement components to optimize
	UPROPERTY(Transient)
	TArray<class UCharacterMovementComponent*> ComponentsToOptimize;
};
