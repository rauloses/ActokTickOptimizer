// TODO: License GPL 3.0?

#pragma once

#include "CoreMinimal.h"
#include "TickOptimizers/ActorComponentTickerOptimizer.h"
#include "MockActorCompTickerOptimizer.generated.h"

/**
 * Mock actor component optimizer for testing
 */
UCLASS()
class ACTORTICKOPTIMIZERTESTS_API UMockActorCompTickerOptimizer : public UActorComponentTickerOptimizer
{
	GENERATED_BODY()

public:
	virtual void RegisterActorComponent(UActorComponent* Component) final;
	virtual void UnregisterActorComponent(UActorComponent* Component) final;
	virtual void ResetOptimizer() final;
	virtual void OptimizeTicks(float DeltaTime) final { }

	FORCEINLINE UActorComponent* GetMockRegisteredActorComponent() const { return MockRegisteredActorComponent; }

private:
	UPROPERTY(Transient)
	UActorComponent* MockRegisteredActorComponent = nullptr;
};
