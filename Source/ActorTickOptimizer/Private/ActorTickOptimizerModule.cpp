// TODO: License GPL 3.0?

#include "ActorTickOptimizerModule.h"

#define LOCTEXT_NAMESPACE "FActorTickOptimizer"

void FActorTickOptimizerModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FActorTickOptimizerModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FActorTickOptimizerModule, ActorTickOptimizer)