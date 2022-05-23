// TODO: License GPL 3.0?

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DistanceTickerOptimizerPOI.generated.h"


/**
 * Establish the component's owner as point of interest for the DistanceATicker or DistanceACTicker 
 */
UCLASS(NotBlueprintable, ClassGroup=(TickOptimizer), meta=(BlueprintSpawnableComponent))
class ACTORTICKOPTIMIZER_API UDistanceTickerOptimizerPOI final : public UActorComponent
{
	GENERATED_BODY()

public:
	// Disable ticking for this component
	UDistanceTickerOptimizerPOI();

	// Get the POI position
	virtual FVector GetPOIPosition() const;
	
protected:
	// Begin UActorComponent interface
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;
	// End UActorComponent interface

	// List with Actor classes to register this POI
	UPROPERTY(EditDefaultsOnly, Category="Optimizer")
	TArray<TSubclassOf<class UDistanceATickerOptimizer>> ActorClassesToRegister;

	// List with ActorComponent classes to register this POI
	UPROPERTY(EditDefaultsOnly, Category="Optimizer")
	TArray<TSubclassOf<class UBaseDistanceACTickerOptimizer>> ActorComponentClassesToRegister;
};
