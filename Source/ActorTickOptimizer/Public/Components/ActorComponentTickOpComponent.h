// TODO: License GPL 3.0?

#pragma once

#include "CoreMinimal.h"
#include "BaseTickOpComponent.h"
#include "ActorComponentTickOpComponent.generated.h"

USTRUCT()
struct  FActorTickOptimizerData
{
	GENERATED_BODY()

public:
	// Class used to search for all components to optimize
	UPROPERTY(EditDefaultsOnly, Category="OptimizerData")
	TSubclassOf<UActorComponent> ComponentClassToOptimize;

	// Optimizer class to use
	UPROPERTY(EditDefaultsOnly, Category="OptimizerData")
	TSubclassOf<class UActorComponentTickerOptimizer> OptimizerClass;
};

/**
 * Optimize all owner's components subobjects
 */
UCLASS(BlueprintType, Blueprintable, meta=(BlueprintSpawnableComponent))
class ACTORTICKOPTIMIZER_API UActorComponentTickOpComponent : public UBaseTickOpComponent
{
	GENERATED_BODY()

protected:
	// Begin UBaseTickOpComponent interface
	virtual void AddToOptimizer(class UActorTickableOptimizerSubsystem* OptimizerSubsystem) override;
	virtual void RemoveFromOptimizer(class UActorTickableOptimizerSubsystem* OptimizerSubsystem) override;
	// End UBaseTickOpComponent interface
	
	// List of all components to optimize
	UPROPERTY(EditDefaultsOnly, Category="Tick Optimizer")
	TArray<FActorTickOptimizerData> ComponentsToOptimize;
};
