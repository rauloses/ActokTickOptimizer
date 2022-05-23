// TODO: License GPL 3.0?

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DistanceTickerOptimizerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDistanceTickerOptimizerInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

/**
 * Data needed to modify the ticking interval in runtime
 */
class ACTORTICKOPTIMIZER_API IDistanceTickerOptimizerInterface : public IInterface
{
	GENERATED_IINTERFACE_BODY()

public:
	// Register a new POI to this optimizer	
	virtual void RegisterPOI(class UDistanceTickerOptimizerPOI* POI) {}

	// Unregister the POI from this optimizer
	virtual void UnregisterPOI(class UDistanceTickerOptimizerPOI* POI) {}

	// Clear the POI list
	virtual void ClearPOIs() {}

	// Gets the number of registered POIs
	virtual int32 GetNumRegisteredPOIs() const { return 0; }

	// Get a list with all registered POIs
	virtual const TArray<UDistanceTickerOptimizerPOI*>& GetRegisteredPOIs() const;

	// Gets the ticker falloff curve
	virtual const UCurveFloat* GetTickingFalloffCurve() const { return nullptr; }

	// Gets the internal value used to force a ticking interval's reset
	virtual float GetDeltaToForceResetTicking() const { return 0.0f; }

	// True if the optimizer should add a random value to the tick interval
	virtual bool ShouldAddRandomValue() const { return false; }

protected:
	// Gets all points of interest
	void GetPOIsPositions(TArray<FVector>& PointsOfInterest) const;

	/**
	 * Update the given tick interval
	 * @return True if the tick interval should be reset in the main thread
	 */
	bool UpdateTickInterval(const FVector& Location, const TArray<FVector>& PointsOfInterest, FTickFunction& TickFunction) const;

private:
	/** Gets the minimum distance from the given point to all other points */
	static float GetMinimumSqrDistance(const FVector& PointA, const TArray<FVector>& Points);
};
