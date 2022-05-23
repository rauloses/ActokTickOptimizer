// TODO: License GPL 3.0?

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseTickOptimizer.generated.h"

/**
 * Define the base structure for any optimizer class.
 * 
 * Child classes should have the list of all objects to optimize and when
 * ticked it should increase or decrease the TickInterval value. By default
 * child optimizer will run in a separate thread.
 *
 * Use OptimizeTicks to update the tick interval at will.
 */
UCLASS(Abstract, NotBlueprintable)
class ACTORTICKOPTIMIZER_API UBaseTickOptimizer : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBaseTickOptimizer();

	// Initialize the optimizer
	virtual void InitializeOptimizer(class UActorTickableOptimizerSubsystem* Subsystem);

	// Reset the optimizer cleaning up all registered objects
	virtual void ResetOptimizer() PURE_VIRTUAL(UBaseTickOptimizer::ResetOptimizer());

	// Being UActorComponent interface
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) final;
	// End UActorComponent interface

protected:
	// TODO: Inherit from UObject instead
	// I should check if is possible to prevent this class from inherit from UActorComponent
	// and have direct control of the ticking, UActorComponent has many thing that aren't needed
	// Check UTickableWorldSystem to see if that's possible

	// Optimizer the TickInterval for all registered objects
	virtual void OptimizeTicks(float DeltaTime) PURE_VIRTUAL(UBaseTickOptimizer::OptimizeTicks);

	// Gets the reference of the subsystem
	FORCEINLINE class UActorTickableOptimizerSubsystem* GetSubsystem() const { return OptimizerSubsystem; }

private:
	// Reference to the optimizer subsystem
	UPROPERTY(Transient)
	class UActorTickableOptimizerSubsystem* OptimizerSubsystem;
};
