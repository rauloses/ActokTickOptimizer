// TODO: License GPL 3.0?

#include "ActorTickOptimizerTestsModule.h"
#include "Modules/ModuleManager.h"

#define LOCTEXT_NAMESPACE "FActorTickOptimizerTests"

DEFINE_LOG_CATEGORY_STATIC(LogActorTickOptimizerTests, Log, All);

class FActorTickOptimizerTests : public IActorTickOptimizerTestsModule
{
};

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FActorTickOptimizerTests, ActorTickOptimizerTests)
