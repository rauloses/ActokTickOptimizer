// TODO: License GPL 3.0?

#include "Components/ActorTickOpComponent.h"
#include "ActorTickableOptimizerSubsystem.h"

void UActorTickOpComponent::AddToOptimizer(class UActorTickableOptimizerSubsystem* OptimizerSubsystem)
{
	if (OptimizerClass)
	{
		OptimizerSubsystem->RegisterActor(GetOwner(), OptimizerClass);
	}
}

void UActorTickOpComponent::RemoveFromOptimizer(class UActorTickableOptimizerSubsystem* OptimizerSubsystem)
{
	if (OptimizerClass)
	{
		OptimizerSubsystem->UnregisterActor(GetOwner(), OptimizerClass);
	}
}