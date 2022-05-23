// TODO: License GPL 3.0?

#pragma once

#include "CoreMinimal.h"
#include "TickOptimizers/ActorComponentTickerOptimizer.h"
#include "Curves/CurveFloat.h"
#include "TickOptimizers/DistanceTickerOptimizerInterface.h"
#include "BaseDistanceACTickerOptimizer.generated.h"

/**
 * Base class for all optimizers that will use the distance for reduce or increase the tick interval of
 * actor's components.
 */
UCLASS(Abstract)
class ACTORTICKOPTIMIZER_API UBaseDistanceACTickerOptimizer : public UActorComponentTickerOptimizer, public IDistanceTickerOptimizerInterface
{
	GENERATED_BODY()

public:
	// Constructor
	UBaseDistanceACTickerOptimizer();

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
	virtual float GetDeltaToForceResetTicking() const override { return DeltaToForceResetTicking; }
	virtual bool ShouldAddRandomValue() const override { return bAddRandomValueToInterval; }
	// End IDistanceTickerOptimizerInterface interface

private:
	// List with all registered POI
	UPROPERTY(Transient)
	TArray<class UDistanceTickerOptimizerPOI*> POIs;
};
