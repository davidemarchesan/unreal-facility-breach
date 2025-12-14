// Copyright Epic Games, Inc. All Rights Reserved.

#include "FacilityBreach.h"
#include "Modules/ModuleManager.h"
#include "UI/Slate/Styles/FacilityBreachStyle.h"

class FFacilityBreachGameModuleImpl : public FDefaultGameModuleImpl
{
public:
	virtual void StartupModule() override
	{
		FFacilityBreachStyle::Initialize();
	}
	virtual void ShutdownModule() override
	{
		FFacilityBreachStyle::Shutdown();
	}
};

IMPLEMENT_PRIMARY_GAME_MODULE( FFacilityBreachGameModuleImpl, FacilityBreach, "FacilityBreach" );
