// TODO: License GPL 3.0?

#pragma once

#include "CoreMinimal.h"
#include "BaseDistanceACTickerOptimizer.h"
#include "DistanceACTickerOptimizer.generated.h"

/**
 * Reduce the Tick Interval using the minimum distance from the owner's actor location to the closest POI
 */
UCLASS(NotBlueprintType, Blueprintable)
class ACTORTICKOPTIMIZER_API UDistanceACTickerOptimizer final : public UBaseDistanceACTickerOptimizer
{
	GENERATED_BODY()

public:
	// Begin UBaseDistanceACTickerOptimizer interface
	virtual void RegisterActorComponent(UActorComponent* Component) override;
	virtual void UnregisterActorComponent(UActorComponent* Component) override;
	virtual void ResetOptimizer() override;
	virtual void OptimizeTicks(float DeltaTime) override;
	// End UBaseDistanceACTickerOptimizer interface

private:
	// List with all actor components to optimize
	UPROPERTY(Transient)
	TArray<UActorComponent*> ActorComponentsToOptimize;
};
