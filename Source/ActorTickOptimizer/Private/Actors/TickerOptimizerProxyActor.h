// TODO: License GPL 3.0?

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TickerOptimizerProxyActor.generated.h"

/**
 * Proxy actor that holds all components that will run the optimization logic
 */
UCLASS(NotBlueprintable, NotPlaceable, Transient)
class ATickerOptimizerProxyActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Disable ticking and replication
	ATickerOptimizerProxyActor();
};
