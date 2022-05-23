// TODO: License GPL 3.0?

#include "ActorTickOptimizerTestsCommon.h"
#include "EngineGlobals.h"

FActorTickOptimizerTestBase::~FActorTickOptimizerTestBase()
{
	check(bActorTearedDown && "FActorTickOptimizerTestBase implementation of TearDown not called!");
}

bool FActorTickOptimizerTestBase::SetUp()
{
	UWorld& World = GetWorld();
	TickOptimizerSubsystem = World.GetSubsystem<UActorTickableOptimizerSubsystem>();
	AITEST_NOT_NULL("TickOptimizerSubsystem", TickOptimizerSubsystem);

	return true;
}

void FActorTickOptimizerTestBase::TearDown()
{
	TickOptimizerSubsystem->ResetRegisteredActors();

	for (auto AutoDestroyedActor : SpawnedActors)
	{
		AutoDestroyedActor->Destroy();
	}
	SpawnedActors.Reset();
	bActorTearedDown = true;

	FAITestBase::TearDown();
}

AActor* FActorTickOptimizerTestBase::SpawnAutoDestroyActor(bool bAddRootComponent)
{
	AActor* NewActor = GetWorld().SpawnActor<AActor>();
	if (bAddRootComponent)
	{
		USceneComponent* RootComponent = NewObject<USceneComponent>(NewActor, TEXT("RootComponent"), RF_Transient);
		RootComponent->RegisterComponent();
		NewActor->SetRootComponent(RootComponent);
	}
	SpawnedActors.Add(NewActor);
	return NewActor;
}

template<class T>
T* FActorTickOptimizerTestBase::GetOptimizer(UActorTickableOptimizerSubsystem* TickOptimizerSubsystem, const TSubclassOf<UBaseTickOptimizer>& OptimizerClass) const
{
	UBaseTickOptimizer* BaseOptimizer = TickOptimizerSubsystem->GetOrCreateOptimizer<UBaseTickOptimizer>(OptimizerClass);
	return CastChecked<T>(BaseOptimizer);
}