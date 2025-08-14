#include <Serialization.h>

#include "PerkPointsPerLevel.h"

namespace PerkPointsPerLevel::Serialization
{
    void Register()
    {
        // Listen for Serialization
        auto SerializationInterface = F4SE::GetSerializationInterface();
        SerializationInterface->SetUniqueID(UniqueID);
        SerializationInterface->SetSaveCallback(F4SESaveCallback);
        SerializationInterface->SetLoadCallback(F4SELoadCallback);
        SerializationInterface->SetRevertCallback(F4SERevertCallback);
        REX::INFO("PerkPointsPerLevel: Started Listening for F4SE Serialization Callbacks.");
    }

    void F4SESaveCallback(const F4SE::SerializationInterface* a_intfc)
    {
        REX::INFO("PerkPointsPerLevel: F4SESaveCallback Called");

        a_intfc->OpenRecord(UniqueID, PluginVersionMajor);
        a_intfc->WriteRecordData(PerkPointsPerLevel::LastKnownPlayerLevel);
        a_intfc->WriteRecordData(PerkPointsPerLevel::PointFractionLeft);
    }

    void F4SELoadCallback(const F4SE::SerializationInterface* a_intfc)
    {
        REX::INFO("PerkPointsPerLevel: F4SELoadCallback Called");

        uint32_t type, version, length;
        while (a_intfc->GetNextRecordInfo(type, version, length))
        {
            if (type == UniqueID)
            {
                REX::INFO("PerkPointsPerLevel: Reading Data (Version {})", version);

                a_intfc->ReadRecordData(&PerkPointsPerLevel::LastKnownPlayerLevel, sizeof(PerkPointsPerLevel::LastKnownPlayerLevel));
                a_intfc->ReadRecordData(&PerkPointsPerLevel::PointFractionLeft, sizeof(PerkPointsPerLevel::PointFractionLeft));
            }
        }
    }

    void F4SERevertCallback(const F4SE::SerializationInterface*)
    {
        REX::INFO("PerkPointsPerLevel: F4SERevertCallback Called");

        PerkPointsPerLevel::LastKnownPlayerLevel = 1;
        PerkPointsPerLevel::PointFractionLeft = 0.0f;
    }
}
