// TODO: License GPL 3.0?

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "ActorTickableOptimizerSubsystem.generated.h"

/**
 * Allow to register components and actors to have an automatic TickInterval management
 */
UCLASS()
class ACTORTICKOPTIMIZER_API UActorTickableOptimizerSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:
	// Register the given actor to the optimizer class's instanced object
	void RegisterActor(AActor* Actor, const TSubclassOf<class UActorTickerOptimizer>& OptimizerClass);

	// Unregister the given actor from the optimizer class's instanced object
	void UnregisterActor(AActor* Actor, const TSubclassOf<class UActorTickerOptimizer>& OptimizerClass);

	// Register the given actor component to the optimizer class's instanced object
	void RegisterActorComponent(UActorComponent* ActorComponent, const TSubclassOf<class UActorComponentTickerOptimizer>& OptimizerClass);

	// Unregister the given actor component from the optimizer class's instanced object
	void UnregisterActorComponent(UActorComponent* ActorComponent, const TSubclassOf<class UActorComponentTickerOptimizer>& OptimizerClass);

	/** Gets the instanced optimizer object from the given class */
	template<class T>
	T* GetOptimizer(const TSubclassOf<class UBaseTickOptimizer>& OptimizerClass) const;

	/** Gets the instanced optimizer object from the given class */
	template<>
	class UBaseTickOptimizer* GetOptimizer(const TSubclassOf<class UBaseTickOptimizer>& OptimizerClass) const;

	// Gets the instanced object of the given class (if that class isn't instanced it will instance one)
	template<class T>
	T* GetOrCreateOptimizer(const TSubclassOf<class UBaseTickOptimizer>& OptimizerClass);
	
	// Gets the instanced object of the given class (if that class isn't instanced it will instance one)
	template<>
	class UBaseTickOptimizer* GetOrCreateOptimizer(const TSubclassOf<class UBaseTickOptimizer>& OptimizerClass);

	// Register the given actor for reset its ticking function
	// This can only be done on the main thread
	void RegisterActorForTickIntervalReset(AActor* Actor);

	// Register the given actor component for reset its ticking function
	// This can only be done on the main thread
	void RegisterActorComponentForTickIntervalReset(UActorComponent* ActorComponent);

#if !UE_BUILD_SHIPPING
	// Disable the optimizer subsystem
	uint8 bDisableOptimizer : 1;
#endif

	// Gets the num of all actors registered for a tick interval reset
	FORCEINLINE int32 GetNumOfRegisteredActorsForTickIntervalReset() const { return ActorsToReset.Num(); }

	// Gets the num of all actor components registered for a tick interval reset
	FORCEINLINE int32 GetNumOfRegisteredActorComponentsForTickIntervalReset() const { return ActorComponentsToReset.Num(); }

	// Reset all actors and actor components that were registered in this subsystem
	void ResetRegisteredActors();

	// UTickableWorldSubsystem implementation Begin
	virtual ETickableTickType GetTickableTickType() const override { return ETickableTickType::Conditional; }
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;
	// UTickableWorldSubsystem implementation End

private:

	// Gets the instanced object of the given class
	class UBaseTickOptimizer* GetOrCreateOptimizerInternal(const TSubclassOf<class UBaseTickOptimizer>& OptimizerClass);

	// Map that links a class with the current instanced object
	UPROPERTY(Transient)
	TMap<const UClass*, class UBaseTickOptimizer*> OptimizerClassToObjectMap;

	// List with all optimizers
	UPROPERTY(Transient)
	TArray<class UBaseTickOptimizer*> Optimizers;

	// Actor that owns all optimizers
	UPROPERTY(Transient)
	AActor* TickerProxy;

	// List with all actors to reset their tick interval
	UPROPERTY(Transient)
	TArray<AActor*> ActorsToReset;

	// List with all actor components to reset their tick interval
	UPROPERTY(Transient)
	TArray<UActorComponent*> ActorComponentsToReset;

	// Prevent locks and dangling pointers when resetting actor tick interval
	FCriticalSection ActorsCriticalSection;

	// Prevent locks and dangling pointers when resetting actor component tick interval
	FCriticalSection ActorComponentsCriticalSection;
};
