// TODO: License GPL 3.0?

#pragma once

#include "CoreMinimal.h"
#include "TickOptimizers/ActorTickerOptimizer.h"
#include "Curves/CurveFloat.h"
#include "TickOptimizers/DistanceTickerOptimizerInterface.h"
#include "DistanceATickerOptimizer.generated.h"

/**
 * Reduce the Tick Interval using the minimum distance from the owner's actor location
 */
UCLASS(NotBlueprintType, Blueprintable)
class ACTORTICKOPTIMIZER_API UDistanceATickerOptimizer final : public UActorTickerOptimizer, public IDistanceTickerOptimizerInterface
{
	GENERATED_BODY()

public:
	// Constructor
	UDistanceATickerOptimizer();

	// Curve used to reduce the ticking interval
	UPROPERTY(EditDefaultsOnly, Category = "Optimizer")
	UCurveFloat* TickingFalloffCurve;

	/**
		* Scaled value used to find when the change in the interval is big enough (when decreasing the value)
		* Once this threshold is passed the TickFunction is reschedule to prevent waiting for long periods
		*/
	UPROPERTY(EditDefaultsOnly, Category = "Optimizer", meta = (ClampMin = "1.01"))
	float DeltaToForceResetTicking = 2.0f;

	/**
	 * Add a random value to the interval
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Optimizer")
	uint8 bAddRandomValueToInterval : 1;

	// Being IDistanceTickerOptimizerInterface interface
	virtual void RegisterPOI(class UDistanceTickerOptimizerPOI* POI) override;
	virtual void UnregisterPOI(class UDistanceTickerOptimizerPOI* POI) override;
	virtual void ClearPOIs() override;
	virtual const TArray<class UDistanceTickerOptimizerPOI*>& GetRegisteredPOIs() const { return POIs; }
	virtual const UCurveFloat* GetTickingFalloffCurve() const override { return TickingFalloffCurve; }
	virtual float GetDeltaToForceResetTicking() const { return DeltaToForceResetTicking; }
	virtual bool ShouldAddRandomValue() const override { return bAddRandomValueToInterval; }
	// End IDistanceTickerOptimizerInterface interface

	// Begin UActorTickerOptimizer interface
	virtual void RegisterActor(AActor* Actor) override;
	virtual void UnregisterActor(AActor* Actor) override;
	virtual void ResetOptimizer() override;
	virtual void OptimizeTicks(float DeltaTime) override;
	// End UActorTickerOptimizer interface

protected:
	// List with all actors to optimize
	UPROPERTY(Transient)
	TArray<AActor*> ActorsToOptimize;

	// List with all registered POI
	UPROPERTY(Transient)
	TArray<class UDistanceTickerOptimizerPOI*> POIs;
};
