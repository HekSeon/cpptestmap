// Designed by KINEMATION, 2025. All rights reserved.

#include "RecoilAnimation.h"
#include "PluginSettings.h"
#include "Developer/Settings/Public/ISettingsModule.h"

#define LOCTEXT_NAMESPACE "FRecoilAnimationModule"

void FRecoilAnimationModule::StartupModule()
{
	if(ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->RegisterSettings("Project", "Plugins", "PRAS_Settings",
		                                 LOCTEXT("RuntimeSettingsName", "Recoil Animation"),
		                                 LOCTEXT("RuntimeSettingsDescription", "Recoil Animation Settings"),
		                                 GetMutableDefault<UPluginSettings>());
	}
}

void FRecoilAnimationModule::ShutdownModule()
{
	if(ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings("Project", "Plugins", "PRAS_Settings");
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FRecoilAnimationModule, RecoilAnimation)