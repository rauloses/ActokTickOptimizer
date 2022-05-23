// TODO: License GPL 3.0?

#include "ActorTickableOptimizerSubsystem.h"
#include "Actors/TickerOptimizerProxyActor.h"
#include "Kismet/GameplayStatics.h"
#include "Stats/Stats2.h"
#include "TickOptimizers/ActorComponentTickerOptimizer.h"
#include "TickOptimizers/ActorTickerOptimizer.h"
#include "TickOptimizers/BaseTickOptimizer.h"


void UActorTickableOptimizerSubsystem::RegisterActor(AActor* Actor, const TSubclassOf<class UActorTickerOptimizer>& OptimizerClass)
{
#if !UE_BUILD_SHIPPING
	if (bDisableOptimizer)
		return;
#endif

	UActorTickerOptimizer* Optimizer = GetOrCreateOptimizer<UActorTickerOptimizer>(OptimizerClass);
	check(Optimizer);
	
	Optimizer->RegisterActor(Actor);
}

void UActorTickableOptimizerSubsystem::UnregisterActor(AActor* Actor, const TSubclassOf<class UActorTickerOptimizer>& OptimizerClass)
{
#if !UE_BUILD_SHIPPING
	if (bDisableOptimizer)
		return;
#endif

	if (UActorTickerOptimizer* Optimizer = GetOptimizer<UActorTickerOptimizer>(OptimizerClass))
	{
		Optimizer->UnregisterActor(Actor);
	}
}

void UActorTickableOptimizerSubsystem::RegisterActorComponent(UActorComponent* ActorComponent, const TSubclassOf<class UActorComponentTickerOptimizer>& OptimizerClass)
{
#if !UE_BUILD_SHIPPING
	if (bDisableOptimizer)
		return;
#endif

	UActorComponentTickerOptimizer* Optimizer = GetOrCreateOptimizer<UActorComponentTickerOptimizer>(OptimizerClass);
	check(Optimizer);

	Optimizer->RegisterActorComponent(ActorComponent);
}

void UActorTickableOptimizerSubsystem::UnregisterActorComponent(UActorComponent* ActorComponent, const TSubclassOf<class UActorComponentTickerOptimizer>& OptimizerClass)
{
#if !UE_BUILD_SHIPPING
	if (bDisableOptimizer)
		return;
#endif

	if (UActorComponentTickerOptimizer* Optimizer = GetOptimizer<UActorComponentTickerOptimizer>(OptimizerClass))
	{
		Optimizer->UnregisterActorComponent(ActorComponent);
	}
}

template<class T>
T* UActorTickableOptimizerSubsystem::GetOptimizer(const TSubclassOf<class UBaseTickOptimizer>& OptimizerClass) const
{
	if (IsInitialized() && OptimizerClassToObjectMap.Contains(OptimizerClass))
	{
		return CastChecked<T>(OptimizerClassToObjectMap[OptimizerClass]);
	}
	return nullptr;
}

template<>
class UBaseTickOptimizer* UActorTickableOptimizerSubsystem::GetOptimizer(const TSubclassOf<class UBaseTickOptimizer>& OptimizerClass) const
{
	if (IsInitialized() && OptimizerClassToObjectMap.Contains(OptimizerClass))
	{
		return OptimizerClassToObjectMap[OptimizerClass];
	}
	return nullptr;
}

template<class T>
T* UActorTickableOptimizerSubsystem::GetOrCreateOptimizer(const TSubclassOf<class UBaseTickOptimizer>& OptimizerClass)
{
	return CastChecked<T>(GetOrCreateOptimizerInternal(OptimizerClass));
}

template<>
UBaseTickOptimizer* UActorTickableOptimizerSubsystem::GetOrCreateOptimizer(const TSubclassOf<class UBaseTickOptimizer>& OptimizerClass)
{
	return GetOrCreateOptimizerInternal(OptimizerClass);
}

class UBaseTickOptimizer* UActorTickableOptimizerSubsystem::GetOrCreateOptimizerInternal(const TSubclassOf<class UBaseTickOptimizer>& OptimizerClass)
{
	if (!IsInitialized())
	{
		return nullptr;
	}

	if (OptimizerClassToObjectMap.Contains(OptimizerClass))
	{
		return OptimizerClassToObjectMap[OptimizerClass];
	}

	if (!TickerProxy)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		TickerProxy = GetWorld()->SpawnActor(ATickerOptimizerProxyActor::StaticClass(), &FTransform::Identity, SpawnParams);
		check(TickerProxy);
	}

	UBaseTickOptimizer* Optimizer = Cast<UBaseTickOptimizer>(TickerProxy->AddComponentByClass(OptimizerClass, false, FTransform::Identity, false));
	Optimizer->InitializeOptimizer(this);

	Optimizers.Add(Optimizer);
	OptimizerClassToObjectMap.Add(OptimizerClass, Optimizer);

	return Optimizer;
}

void UActorTickableOptimizerSubsystem::RegisterActorForTickIntervalReset(AActor* Actor)
{
	FScopeLock Lock(&ActorsCriticalSection);
	ActorsToReset.Add(Actor);
}

void UActorTickableOptimizerSubsystem::RegisterActorComponentForTickIntervalReset(UActorComponent* ActorComponent)
{
	FScopeLock Lock(&ActorComponentsCriticalSection);
	ActorComponentsToReset.Add(ActorComponent);
}

void UActorTickableOptimizerSubsystem::Tick(float DeltaTime)
{
#if !UE_BUILD_SHIPPING
	if (bDisableOptimizer)
		return;
#endif

	SCOPED_NAMED_EVENT(UActorTickableOptimizerSubsystem_Tick, FColor::Cyan);

	TArray<AActor*> TempActors;
	{
		FScopeLock Lock(&ActorsCriticalSection);
		TempActors.Append(ActorsToReset);
		ActorsToReset.Empty();
	}

	for (AActor* Actor : TempActors)
	{
		if (!Actor || Actor->IsActorBeingDestroyed() || !Actor->IsActorTickEnabled())
		{
			continue;
		}

		Actor->PrimaryActorTick.UpdateTickIntervalAndCoolDown(Actor->GetActorTickInterval());
	}

	TArray<UActorComponent*> TempActorComponents;
	{
		FScopeLock Lock(&ActorComponentsCriticalSection);
		TempActorComponents.Append(ActorComponentsToReset);
		ActorComponentsToReset.Empty();
	}

	for (UActorComponent* Component : TempActorComponents)
	{
		if (!Component || Component->IsBeingDestroyed() || !Component->IsComponentTickEnabled())
		{
			continue;
		}

		Component->PrimaryComponentTick.UpdateTickIntervalAndCoolDown(Component->GetComponentTickInterval());
	}
}

TStatId UActorTickableOptimizerSubsystem::GetStatId() const
{
	return TStatId();
}

void UActorTickableOptimizerSubsystem::ResetRegisteredActors()
{
	{
		FScopeLock Lock(&ActorsCriticalSection);
		ActorsToReset.Empty();
	}

	{
		FScopeLock Lock(&ActorComponentsCriticalSection);
		ActorComponentsToReset.Empty();
	}

	for (UBaseTickOptimizer* Optimizer : Optimizers)
	{
		Optimizer->ResetOptimizer();
	}
}