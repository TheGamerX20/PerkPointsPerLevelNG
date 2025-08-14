#include <pch.h>

// Patches
#include <PerkPointsPerLevel.h>

namespace Main
{
    // Config Options
    static REX::INI::Bool iPerkPointsPerLevelPatch{ "Patches"sv, "EnablePerkPointsPerLevelPatch"sv, true };

    F4SE_PLUGIN_PRELOAD(const F4SE::LoadInterface* a_f4se)
    {
        // Init
        F4SE::Init(a_f4se);
        REX::INFO("PerkPointsPerLevel NG Initializing...");

        // Get the Trampoline and Allocate
		REL::Trampoline& Trampoline = REL::GetTrampoline();
		Trampoline.create(128);

        // Load the Config
        REX::INI::SettingStore* Config = REX::INI::SettingStore::GetSingleton();
        Config->Init("Data/F4SE/Plugins/PerkPointsPerLevelNG.ini", "Data/F4SE/Plugins/PerkPointsPerLevelNGCustom.ini");
        Config->Load();

        // Install PerkPointsPerLevel
        if (iPerkPointsPerLevelPatch.GetValue() == true)
        {
            if (PerkPointsPerLevel::Install())
            {
                REX::INFO("PerkPointsPerLevel NG Initialized!");
            }
        }

        // Finished
        return true;
    }
}
