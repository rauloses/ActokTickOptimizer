// TODO: License GPL 3.0?

#include "ActorTickOptimizerTestsCommon.h"
#include "Components/DistanceTickerOptimizerPOI.h"
#include "TickOptimizers/DistanceACTickerOptimizer.h"
#include "TickOptimizers/DistanceCMTickerOptimizer.h"
#include "TickOptimizers/DistanceATickerOptimizer.h"
#include "TickOptimizers/DistanceTickerOptimizerInterface.h"

#define LOCTEXT_NAMESPACE "DistanceTickerOptimizerTest"

PRAGMA_DISABLE_OPTIMIZATION

struct FDistanceTickerOptimizerTest_Fixture : public FActorTickOptimizerTestBase
{
	AActor* TestedActor = nullptr;
	AActor* PointOfInterestA = nullptr;
	AActor* PointOfInterestB = nullptr;
	UDistanceATickerOptimizer* AOptimizer = nullptr;

	virtual bool SetUp() override
	{
		FActorTickOptimizerTestBase::SetUp();

		AOptimizer = GetOptimizer<UDistanceATickerOptimizer>(TickOptimizerSubsystem, UDistanceATickerOptimizer::StaticClass());
		AITEST_NOT_NULL("AOptimizer", AOptimizer);
		AITEST_EQUAL("AOptimizer POIs Num", AOptimizer->GetNumRegisteredPOIs(), 0);

		AOptimizer->TickingFalloffCurve = CreateCurveFloat();
		AOptimizer->DeltaToForceResetTicking = 1.0f;
		AOptimizer->bAddRandomValueToInterval = false;
		AITEST_NOT_NULL("AOptimizer->TickingFalloffCurve", AOptimizer->TickingFalloffCurve);

		PointOfInterestA = CreateAndRegisterPOI(AOptimizer, FVector(10.0f, 5.0f, 0.0f));
		PointOfInterestB = CreateAndRegisterPOI(AOptimizer, FVector(-10.0f, 5.0f, 0.0f));
		AITEST_NOT_NULL("PointOfInterestA", PointOfInterestA);
		AITEST_NOT_NULL("PointOfInterestB", PointOfInterestB);

		TestedActor = SpawnAutoDestroyActor(true);
		AITEST_NOT_NULL("TestedActor", TestedActor);

		AOptimizer->RegisterActor(TestedActor);

		return true;
	}

	virtual void TearDown() override
	{
		AOptimizer->ClearPOIs();

		FActorTickOptimizerTestBase::TearDown();
	}

	UCurveFloat* CreateCurveFloat() 
	{
		if (UCurveFloat* NewCurve = NewAutoDestroyObject<UCurveFloat>())
		{
			TArray<FRichCurveKey> CurveKeys;
			CurveKeys.Add(GetCurveKey(0.0f, 0.0f));
			CurveKeys.Add(GetCurveKey(25.0f, 0.5f));
			CurveKeys.Add(GetCurveKey(100.0f, 1.0f));
			NewCurve->FloatCurve.SetKeys(CurveKeys);

			AITEST_EQUAL("Curve at   0.0", NewCurve->GetFloatValue(0.0f), 0.0f);
			AITEST_EQUAL("Curve at  25.0", NewCurve->GetFloatValue(25.0f), 0.5f);
			AITEST_EQUAL("Curve at 100.0", NewCurve->GetFloatValue(100.0f), 1.0f);

			return NewCurve;
		}
		return nullptr;
	}

	FRichCurveKey GetCurveKey(float InTime, float InValue)
	{
		FRichCurveKey CurveKey;
		CurveKey.TangentMode = RCTM_Break;
		CurveKey.TangentWeightMode = RCTWM_WeightedNone;
		CurveKey.InterpMode = RCIM_Linear;
		CurveKey.Time = InTime;
		CurveKey.Value = InValue;
		return CurveKey;
	}

	AActor* CreateAndRegisterPOI(IDistanceTickerOptimizerInterface* Optimizer, const FVector& Location)
	{
		AActor* Actor = SpawnAutoDestroyActor(true);
		Actor->SetActorLocation(Location);

		UDistanceTickerOptimizerPOI* POI = NewObject<UDistanceTickerOptimizerPOI>(Actor, TEXT("ComponentToOptimize"), RF_Transient);
		POI->RegisterComponent();
		Optimizer->RegisterPOI(POI);

		return Actor;
	}
};

IMPLEMENT_INSTANT_TEST_WITH_FIXTURE(FDistanceTickerOptimizerTest_Fixture, "Plugins.ActorTickerOptimizerSubsystem.Optimizers.DistanceTicker", TickInterval_00)
{
	const FVector TestPosition(0.0f, 5.0f, 0.0f);
	const float ExpectedSqDistance = 100.0f;
	const float ExpectedValue = 1.0f;
	float SqDistance = 0.0f;

	TestedActor->SetActorLocation(TestPosition);
	AITEST_EQUAL("Location", TestedActor->GetActorLocation(), TestPosition);

	SqDistance = FVector::DistSquared(TestedActor->GetActorLocation(), PointOfInterestA->GetActorLocation());
	AITEST_EQUAL("SqDistance with A", SqDistance, ExpectedSqDistance);
	AITEST_EQUAL("Curve Value", AOptimizer->GetTickingFalloffCurve()->GetFloatValue(ExpectedSqDistance), ExpectedValue);

	AOptimizer->TickComponent(FAITestHelpers::TickInterval, LEVELTICK_All, nullptr);
	AITEST_EQUAL("Actor TickInterval", TestedActor->PrimaryActorTick.TickInterval, ExpectedValue);

	return true;
}

IMPLEMENT_INSTANT_TEST_WITH_FIXTURE(FDistanceTickerOptimizerTest_Fixture, "Plugins.ActorTickerOptimizerSubsystem.Optimizers.DistanceTicker", TickInterval_05)
{
	const FVector TestPosition(5.0f, 5.0f, 0.0f);
	const float ExpectedSqDistance = 25.0f;
	const float ExpectedValue = 0.5f;
	float SqDistance = 0.0f;

	TestedActor->SetActorLocation(TestPosition);
	AITEST_EQUAL("Location", TestedActor->GetActorLocation(), TestPosition);

	SqDistance = FVector::DistSquared(TestedActor->GetActorLocation(), PointOfInterestA->GetActorLocation());
	AITEST_EQUAL("SqDistance with A", SqDistance, ExpectedSqDistance);
	AITEST_EQUAL("Curve Value", AOptimizer->GetTickingFalloffCurve()->GetFloatValue(ExpectedSqDistance), ExpectedValue);

	AOptimizer->TickComponent(FAITestHelpers::TickInterval, LEVELTICK_All, nullptr);
	AITEST_EQUAL("Actor TickInterval", TestedActor->PrimaryActorTick.TickInterval, ExpectedValue);

	return true;
}

IMPLEMENT_INSTANT_TEST_WITH_FIXTURE(FDistanceTickerOptimizerTest_Fixture, "Plugins.ActorTickerOptimizerSubsystem.Optimizers.DistanceTicker", TickInterval_10)
{
	const FVector TestPosition(10.0f, 5.0f, 0.0f);
	const float ExpectedSqDistance = 0.0f;
	const float ExpectedValue = 0.0f;
	float SqDistance = 0.0f;

	TestedActor->SetActorLocation(TestPosition);
	AITEST_EQUAL("Location", TestedActor->GetActorLocation(), TestPosition);

	SqDistance = FVector::DistSquared(TestedActor->GetActorLocation(), PointOfInterestA->GetActorLocation());
	AITEST_EQUAL("SqDistance with A", SqDistance, ExpectedSqDistance);
	AITEST_EQUAL("Curve Value", AOptimizer->GetTickingFalloffCurve()->GetFloatValue(ExpectedSqDistance), ExpectedValue);

	AOptimizer->TickComponent(FAITestHelpers::TickInterval, LEVELTICK_All, nullptr);
	AITEST_EQUAL("Actor TickInterval", TestedActor->PrimaryActorTick.TickInterval, ExpectedValue);

	return true;
}

IMPLEMENT_INSTANT_TEST_WITH_FIXTURE(FDistanceTickerOptimizerTest_Fixture, "Plugins.ActorTickerOptimizerSubsystem.Optimizers.DistanceTicker", TickInterval_30)
{
	const FVector TestPosition(30.0f, 5.0f, 0.0f);
	const float ExpectedSqDistance = 400.0f;
	const float ExpectedValue = 1.0f;
	float SqDistance = 0.0f;

	TestedActor->SetActorLocation(TestPosition);
	AITEST_EQUAL("Location", TestedActor->GetActorLocation(), TestPosition);

	SqDistance = FVector::DistSquared(TestedActor->GetActorLocation(), PointOfInterestA->GetActorLocation());
	AITEST_EQUAL("SqDistance with A", SqDistance, ExpectedSqDistance);
	AITEST_EQUAL("Curve Value", AOptimizer->GetTickingFalloffCurve()->GetFloatValue(ExpectedSqDistance), ExpectedValue);

	AOptimizer->TickComponent(FAITestHelpers::TickInterval, LEVELTICK_All, nullptr);
	AITEST_EQUAL("Actor TickInterval", TestedActor->PrimaryActorTick.TickInterval, ExpectedValue);

	return true;
}

PRAGMA_ENABLE_OPTIMIZATION

#undef LOCTEXT_NAMESPACE