// TODO: License GPL 3.0?

#include "Components/ActorComponentTickOpComponent.h"
#include "ActorTickableOptimizerSubsystem.h"
#include "TickOptimizers/ActorComponentTickerOptimizer.h"

void UActorComponentTickOpComponent::AddToOptimizer(class UActorTickableOptimizerSubsystem* OptimizerSubsystem)
{
	for (const FActorTickOptimizerData& Data : ComponentsToOptimize)
	{
		if (Data.OptimizerClass)
		{
			TArray<UActorComponent*> Components;
			GetOwner()->GetComponents(Data.ComponentClassToOptimize, Components);
			for (UActorComponent* Component : Components)
			{
				OptimizerSubsystem->RegisterActorComponent(Component, Data.OptimizerClass);
			}
		}
	}
}

void UActorComponentTickOpComponent::RemoveFromOptimizer(class UActorTickableOptimizerSubsystem* OptimizerSubsystem)
{
	for (const FActorTickOptimizerData& Data : ComponentsToOptimize)
	{
		if (Data.OptimizerClass)
		{
			TArray<UActorComponent*> Components;
			GetOwner()->GetComponents(Data.ComponentClassToOptimize, Components);
			for (UActorComponent* Component : Components)
			{
				OptimizerSubsystem->UnregisterActorComponent(Component, Data.OptimizerClass);
			}
		}
	}
}