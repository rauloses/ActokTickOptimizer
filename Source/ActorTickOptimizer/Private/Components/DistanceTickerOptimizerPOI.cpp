// TODO: License GPL 3.0?

#include "Components/DistanceTickerOptimizerPOI.h"
#include "ActorTickableOptimizerSubsystem.h"
#include "TickOptimizers/BaseDistanceACTickerOptimizer.h"
#include "TickOptimizers/DistanceATickerOptimizer.h"

UDistanceTickerOptimizerPOI::UDistanceTickerOptimizerPOI()
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UDistanceTickerOptimizerPOI::BeginPlay()
{
	Super::BeginPlay();

	if (IsTemplate())
	{
		return;
	}

	if (UActorTickableOptimizerSubsystem* TickOptimizerSubsystem = GetWorld()->GetSubsystem<UActorTickableOptimizerSubsystem>())
	{
		for (const TSubclassOf<class UDistanceATickerOptimizer>& OptimizerClass : ActorClassesToRegister)
		{
			UDistanceATickerOptimizer* ActorOptimizer = TickOptimizerSubsystem->GetOrCreateOptimizer<UDistanceATickerOptimizer>(OptimizerClass);
			check(ActorOptimizer);
			ActorOptimizer->RegisterPOI(this);
		}

		for (const TSubclassOf<class UBaseDistanceACTickerOptimizer>& OptimizerClass : ActorComponentClassesToRegister)
		{
			UBaseDistanceACTickerOptimizer* ActorComponentOptimizer = TickOptimizerSubsystem->GetOrCreateOptimizer<UBaseDistanceACTickerOptimizer>(OptimizerClass);
			check(ActorComponentOptimizer);
			ActorComponentOptimizer->RegisterPOI(this);
		}
	}
}

void UDistanceTickerOptimizerPOI::BeginDestroy()
{
	Super::BeginDestroy();

	if (IsTemplate())
	{
		return;
	}

	if (const UWorld* World = GetWorld())
	{
		if (const UActorTickableOptimizerSubsystem* TickOptimizerSubsystem = World->GetSubsystem<UActorTickableOptimizerSubsystem>())
		{
			for (const TSubclassOf<class UDistanceATickerOptimizer>& OptimizerClass : ActorClassesToRegister)
			{
				if (UDistanceATickerOptimizer* ActorOptimizer = TickOptimizerSubsystem->GetOptimizer<UDistanceATickerOptimizer>(OptimizerClass))
				{
					ActorOptimizer->UnregisterPOI(this);	
				}
			}

			for (const TSubclassOf<class UBaseDistanceACTickerOptimizer>& OptimizerClass : ActorComponentClassesToRegister)
			{
				if (UBaseDistanceACTickerOptimizer* ActorComponentOptimizer = TickOptimizerSubsystem->GetOptimizer<UBaseDistanceACTickerOptimizer>(OptimizerClass))
				{
					ActorComponentOptimizer->UnregisterPOI(this);
				}
			}
		}
	}
}

FVector UDistanceTickerOptimizerPOI::GetPOIPosition() const
{
	return GetOwner()->GetActorLocation();
}