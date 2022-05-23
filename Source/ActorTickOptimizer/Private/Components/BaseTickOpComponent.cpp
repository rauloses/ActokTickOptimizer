// TODO: License GPL 3.0?

#include "Components/BaseTickOpComponent.h"
#include "ActorTickableOptimizerSubsystem.h"


UBaseTickOpComponent::UBaseTickOpComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UBaseTickOpComponent::BeginPlay()
{
	Super::BeginPlay();

	if (IsTemplate())
	{
		return;
	}

	if (UActorTickableOptimizerSubsystem* TickOptimizerSubsystem = GetWorld()->GetSubsystem<UActorTickableOptimizerSubsystem>())
	{
		AddToOptimizer(TickOptimizerSubsystem);	
	}
}

void UBaseTickOpComponent::BeginDestroy()
{
	Super::BeginDestroy();

	// TODO: Check why when recompiling code this code crashes the editor.
	if (IsTemplate())
	{
		return;
	}

	if (const UWorld* World = GetWorld())
	{
		if (UActorTickableOptimizerSubsystem* TickOptimizerSubsystem = World->GetSubsystem<UActorTickableOptimizerSubsystem>())
		{
			RemoveFromOptimizer(TickOptimizerSubsystem);
		}
	}
}