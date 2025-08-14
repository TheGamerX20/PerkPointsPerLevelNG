#pragma once

namespace PerkPointsPerLevel::Serialization
{
    constexpr std::uint32_t UniqueID = 'PPPL';
    constexpr int PluginVersionMajor = PLUGIN_VERSION_MAJOR;
    constexpr int PluginVersionMinor = PLUGIN_VERSION_MINOR;
    constexpr int PluginVersionPatch = PLUGIN_VERSION_PATCH;

    void Register();
    void F4SESaveCallback(const F4SE::SerializationInterface* a_intfc);
    void F4SELoadCallback(const F4SE::SerializationInterface* a_intfc);
    void F4SERevertCallback(const F4SE::SerializationInterface*);
}
