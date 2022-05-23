// TODO: License GPL 3.0?

#pragma once

#include "CoreMinimal.h"
#include "TickOptimizers/ActorTickerOptimizer.h"
#include "MockActorTickerOptimizer.generated.h"

/**
 * Mock actor optimizer
 */
UCLASS()
class ACTORTICKOPTIMIZERTESTS_API UMockActorTickerOptimizer : public UActorTickerOptimizer
{
	GENERATED_BODY()

public:
	// Begin UActorTickOptimizer interface
	virtual void RegisterActor(AActor* Actor) final;
	virtual void UnregisterActor(AActor* Actor) final;
	virtual void ResetOptimizer() final;
	virtual void OptimizeTicks(float DeltaTime) final { }
	// End UActorTickOptimizer interface

	FORCEINLINE AActor* GetMockRegisteredActor() const { return MockRegisteredActor; }

private:
	UPROPERTY(Transient)
	AActor* MockRegisteredActor = nullptr;
};
