#include "Messaging.h"

#include "Events.h"

namespace PerkPointsPerLevel::Messaging
{
    void Register()
    {
        // Listen for Messages
        auto MessagingInterface = F4SE::GetMessagingInterface();
        MessagingInterface->RegisterListener(PerkPointsPerLevel::Messaging::F4SEMessageListener);
        REX::INFO("PerkPointsPerLevel: Started Listening for F4SE Message Callbacks.");
    }

    void F4SEMessageListener(F4SE::MessagingInterface::Message* a_msg)
    {
        switch (a_msg->type)
        {
        case F4SE::MessagingInterface::kGameDataReady:
        {
            REX::INFO("PerkPointsPerLevel: Received kGameDataReady");

            // Listen for an Increase in Perk Points
            RE::BSTGlobalEvent::EventSource<RE::PerkPointIncreaseEvent>* Source = RE::PerkPointIncreaseEvent::GetEventSource();
            Source->RegisterSink(&PerkPointsPerLevel::Events::PerkPointEventSink);

            break;
        }
        default:
            break;
        }
    }
}
