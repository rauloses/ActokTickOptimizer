// TODO: License GPL 3.0?

#include "TickOptimizers/BaseTickOptimizer.h"
#include "ActorTickableOptimizerSubsystem.h"

UBaseTickOptimizer::UBaseTickOptimizer()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bRunOnAnyThread = true;
	PrimaryComponentTick.TickGroup = ETickingGroup::TG_LastDemotable;
}

void UBaseTickOptimizer::InitializeOptimizer(class UActorTickableOptimizerSubsystem* Subsystem)
{
	OptimizerSubsystem = Subsystem;
}

void UBaseTickOptimizer::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	OptimizeTicks(DeltaTime);
}