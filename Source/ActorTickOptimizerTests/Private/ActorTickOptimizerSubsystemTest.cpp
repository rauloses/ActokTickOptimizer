// TODO: License GPL 3.0?

#include "ActorTickOptimizerTestsCommon.h"
#include "MockActorCompTickerOptimizer.h"
#include "MockActorTickerOptimizer.h"

#define LOCTEXT_NAMESPACE "ActorTickOptimizerSubsystemTest"

PRAGMA_DISABLE_OPTIMIZATION

template<class T>
struct FActorTickOptimizerSubsystemTest_Fixture : public FActorTickOptimizerTestBase
{
	TSubclassOf<T> MockOptimizerClass = T::StaticClass();
	T* MockOptimizer = nullptr;
	AActor* TestedActor = nullptr;

	virtual bool SetUp() override
	{
		FActorTickOptimizerTestBase::SetUp();

		MockOptimizer = GetOptimizer<T>(TickOptimizerSubsystem, MockOptimizerClass);
		AITEST_NOT_NULL("MockOptimizer", MockOptimizer);

		TestedActor = SpawnAutoDestroyActor();
		AITEST_NOT_NULL("TestedActor", TestedActor);

		return true;
	}
};

struct FActorTickOptimizerSubsystemTest_ActorFixture 
	: public FActorTickOptimizerSubsystemTest_Fixture<UMockActorTickerOptimizer>
{
};

IMPLEMENT_INSTANT_TEST_WITH_FIXTURE(FActorTickOptimizerSubsystemTest_ActorFixture, "Plugins.ActorTickerOptimizerSubsystem.Common", RegisterActor)
{
	TickOptimizerSubsystem->RegisterActor(TestedActor, MockOptimizerClass);
	AITEST_NOT_NULL("After register, GetMockRegisteredActor()", MockOptimizer->GetMockRegisteredActor());
	AITEST_EQUAL("GetMockRegisteredActor()", MockOptimizer->GetMockRegisteredActor(), TestedActor);

	return true;
}

IMPLEMENT_INSTANT_TEST_WITH_FIXTURE(FActorTickOptimizerSubsystemTest_ActorFixture, "Plugins.ActorTickerOptimizerSubsystem.Common", UnregisterActor)
{
	MockOptimizer->RegisterActor(TestedActor);
	AITEST_EQUAL("After register, GetMockRegisteredActor()", MockOptimizer->GetMockRegisteredActor(), TestedActor);

	TickOptimizerSubsystem->UnregisterActor(TestedActor, MockOptimizerClass);
	AITEST_NULL("After unregister, GetMockRegisteredActor()", MockOptimizer->GetMockRegisteredActor());

	return true;
}

IMPLEMENT_INSTANT_TEST_WITH_FIXTURE(FActorTickOptimizerSubsystemTest_ActorFixture, "Plugins.ActorTickerOptimizerSubsystem.Common", Actor_TickIntervalReset)
{
	TickOptimizerSubsystem->RegisterActorForTickIntervalReset(TestedActor);
	AITEST_EQUAL("After register, GetNumOfRegisteredActorsForTickIntervalReset()", TickOptimizerSubsystem->GetNumOfRegisteredActorsForTickIntervalReset(), 1);

	TickOptimizerSubsystem->Tick(FAITestHelpers::TickInterval);
	AITEST_EQUAL("After tick, GetNumOfRegisteredActorsForTickIntervalReset()", TickOptimizerSubsystem->GetNumOfRegisteredActorsForTickIntervalReset(), 0);

	return true;
}

struct FActorTickOptimizerSubsystemTest_ActorComponentFixture
	: public FActorTickOptimizerSubsystemTest_Fixture<UMockActorCompTickerOptimizer>
{
	typedef FActorTickOptimizerSubsystemTest_Fixture<UMockActorCompTickerOptimizer> Super;

	UActorComponent* TestedActorComponent = nullptr;

	virtual bool SetUp() override
	{
		const bool result = Super::SetUp();
		if (result)
		{
			TestedActorComponent = NewObject<USceneComponent>(TestedActor, TEXT("ComponentToOptimize"), RF_Transient);
			AITEST_NOT_NULL("TestedActorComponent", TestedActorComponent);
			TestedActorComponent->RegisterComponent();
		}

		return TestedActorComponent != nullptr;
	}
};

IMPLEMENT_INSTANT_TEST_WITH_FIXTURE(FActorTickOptimizerSubsystemTest_ActorComponentFixture, "Plugins.ActorTickerOptimizerSubsystem.Common", RegisterActorComponent)
{
	TickOptimizerSubsystem->RegisterActorComponent(TestedActorComponent, MockOptimizerClass);
	AITEST_NOT_NULL("After register, GetMockRegisteredActorComponent", MockOptimizer->GetMockRegisteredActorComponent());
	AITEST_EQUAL("GetMockRegisteredActorComponent", MockOptimizer->GetMockRegisteredActorComponent(), TestedActorComponent);

	return true;
}

IMPLEMENT_INSTANT_TEST_WITH_FIXTURE(FActorTickOptimizerSubsystemTest_ActorComponentFixture, "Plugins.ActorTickerOptimizerSubsystem.Common", UnregisterActorComponent)
{
	MockOptimizer->RegisterActorComponent(TestedActorComponent);
	AITEST_EQUAL("After register, GetMockRegisteredActorComponent", MockOptimizer->GetMockRegisteredActorComponent(), TestedActorComponent);

	TickOptimizerSubsystem->UnregisterActorComponent(TestedActorComponent, MockOptimizerClass);
	AITEST_NULL("After unregister, GetMockRegisteredActorComponent", MockOptimizer->GetMockRegisteredActorComponent());

	return true;
}

IMPLEMENT_INSTANT_TEST_WITH_FIXTURE(FActorTickOptimizerSubsystemTest_ActorComponentFixture, "Plugins.ActorTickerOptimizerSubsystem.Common", ActorComponent_TickIntervalReset)
{
	TickOptimizerSubsystem->RegisterActorComponentForTickIntervalReset(TestedActorComponent);
	AITEST_EQUAL("After register, GetNumOfRegisteredActorComponentsForTickIntervalReset", TickOptimizerSubsystem->GetNumOfRegisteredActorComponentsForTickIntervalReset(), 1);

	TickOptimizerSubsystem->Tick(FAITestHelpers::TickInterval);
	AITEST_EQUAL("After tick, GetNumOfRegisteredActorComponentsForTickIntervalReset", TickOptimizerSubsystem->GetNumOfRegisteredActorComponentsForTickIntervalReset(), 0);

	return true;
}

PRAGMA_ENABLE_OPTIMIZATION

#undef LOCTEXT_NAMESPACE