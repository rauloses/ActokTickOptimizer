// TODO: License GPL 3.0?

#pragma once

#include "CoreMinimal.h"
#include "BaseTickOpComponent.h"
#include "ActorTickOpComponent.generated.h"

/**
 * Optimize owner actor's ticks
 */
UCLASS(BlueprintType, Blueprintable, meta=(BlueprintSpawnableComponent))
class ACTORTICKOPTIMIZER_API UActorTickOpComponent : public UBaseTickOpComponent
{
	GENERATED_BODY()

protected:
	// Begin UBaseTickOpComponent interface
	virtual void AddToOptimizer(class UActorTickableOptimizerSubsystem* OptimizerSubsystem) override;
	virtual void RemoveFromOptimizer(class UActorTickableOptimizerSubsystem* OptimizerSubsystem) override;
	// End UBaseTickOpComponent interface

	// Optimizer class to use
	UPROPERTY(EditDefaultsOnly, Category="Tick Optimizer")
	TSubclassOf<class UActorTickerOptimizer> OptimizerClass;
};
