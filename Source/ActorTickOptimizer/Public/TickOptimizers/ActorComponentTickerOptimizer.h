// TODO: License GPL 3.0?

#pragma once

#include "CoreMinimal.h"
#include "TickOptimizers/BaseTickOptimizer.h"
#include "ActorComponentTickerOptimizer.generated.h"


/**
 * Interface for optimize ActorComponent's ticks
 */
UCLASS(Abstract)
class ACTORTICKOPTIMIZER_API UActorComponentTickerOptimizer : public UBaseTickOptimizer
{
	GENERATED_BODY()

public:
	// Register the given actor component to this optimizer
	virtual void RegisterActorComponent(UActorComponent* Component) PURE_VIRTUAL(UActorComponentTickerOptimizer::RegisterActorComponent);

	// Unregister the given actor component from this optimizer 
	virtual void UnregisterActorComponent(UActorComponent* Component) PURE_VIRTUAL(UActorComponentTickerOptimizer::UnregisterActorComponent);
};
