// TODO: License GPL 3.0?

#pragma once

#include "AITestsCommon.h"
#include "ActorTickableOptimizerSubsystem.h"
#include "Components/SceneComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "TickOptimizers/BaseTickOptimizer.h"


struct ACTORTICKOPTIMIZERTESTS_API FActorTickOptimizerTestBase : public FAITestBase
{
public:
	UActorTickableOptimizerSubsystem* TickOptimizerSubsystem = nullptr;

private:
	TArray<AActor*> SpawnedActors;
	uint32 bActorTearedDown : 1;

public:
	FActorTickOptimizerTestBase() : bActorTearedDown(false) {}
	virtual ~FActorTickOptimizerTestBase();

	virtual bool SetUp() override;
	virtual void TearDown() override;

	/**
	 * Creates an Actor and keeps its reference for its destruction after the test
	 * @param bAddRootComponent Adds an SceneComponent as the RootComponent to the Actor to allow transforms
	 */
	AActor* SpawnAutoDestroyActor(bool bAddRootComponent = false);

	/**
	 * Returns the T optimizer (if an object doesn't exist it will create one)
	 */
	template<class T>
	T* GetOptimizer(UActorTickableOptimizerSubsystem* TickOptimizerSubsystem, const TSubclassOf<UBaseTickOptimizer>& OptimizerClass) const;
};
