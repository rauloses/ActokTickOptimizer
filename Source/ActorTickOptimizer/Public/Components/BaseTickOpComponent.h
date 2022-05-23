// TODO: License GPL 3.0?

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseTickOpComponent.generated.h"


/**
 * Base class for components that will register other objects to the subsystem
 */
UCLASS(Abstract, NotBlueprintable)
class ACTORTICKOPTIMIZER_API UBaseTickOpComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBaseTickOpComponent();

protected:
	// Begin UActorComponent interface
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;
	// End UActorComponent interface

	/**
	 * Add actors or component to the optimizer subsystem
	 */
	virtual void AddToOptimizer(class UActorTickableOptimizerSubsystem* OptimizerSubsystem) PURE_VIRTUAL(UBaseTickOpComponent::AddToOptmizer);

	/**
	 * Remove actors or components from the optimizer subsystem 
	 */
	virtual void RemoveFromOptimizer(class UActorTickableOptimizerSubsystem* OptimizerSubsystem) PURE_VIRTUAL(UBaseTickOpComponent::RemoveFromOptimizer);
};
