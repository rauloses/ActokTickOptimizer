// TODO: License GPL 3.0?

#pragma once

#include "CoreMinimal.h"
#include "TickOptimizers/BaseTickOptimizer.h"
#include "ActorTickerOptimizer.generated.h"

/**
 * Interface to optimizer Actor's ticks
 */
UCLASS(Abstract)
class ACTORTICKOPTIMIZER_API UActorTickerOptimizer : public UBaseTickOptimizer
{
	GENERATED_BODY()

public:
	// Register the given actor to this optimizer
	virtual void RegisterActor(AActor* Actor) PURE_VIRTUAL(UActorTickerOptimizer::RegisterActor);

	// Unregister the given actor from this optimizer 
	virtual void UnregisterActor(AActor* Actor) PURE_VIRTUAL(UActorTickerOptimizer::UnregisterActor);
};
