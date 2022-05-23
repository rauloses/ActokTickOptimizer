// TODO: License GPL 3.0?

#include "TickerOptimizerProxyActor.h"

ATickerOptimizerProxyActor::ATickerOptimizerProxyActor()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	bReplicates = false;
}
