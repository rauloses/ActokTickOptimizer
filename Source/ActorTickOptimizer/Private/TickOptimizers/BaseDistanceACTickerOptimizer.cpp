// TODO: License GPL 3.0?

#include "TickOptimizers/BaseDistanceACTickerOptimizer.h"
#include "Components/DistanceTickerOptimizerPOI.h"

UBaseDistanceACTickerOptimizer::UBaseDistanceACTickerOptimizer()
	: Super()
	, bAddRandomValueToInterval(true)
{

}

void UBaseDistanceACTickerOptimizer::RegisterPOI(class UDistanceTickerOptimizerPOI* POI)
{
	POIs.Add(POI);
}

void UBaseDistanceACTickerOptimizer::UnregisterPOI(class UDistanceTickerOptimizerPOI* POI)
{
	POIs.RemoveSwap(POI);
}

void UBaseDistanceACTickerOptimizer::ClearPOIs()
{
	POIs.Empty();
}