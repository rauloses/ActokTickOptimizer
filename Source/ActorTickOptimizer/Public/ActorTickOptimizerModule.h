// TODO: License GPL 3.0?

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FActorTickOptimizerModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
