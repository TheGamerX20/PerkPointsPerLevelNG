#include "Events.h"

#include "PerkPointsPerLevel.h"

namespace PerkPointsPerLevel::Events
{
    PerkPointIncreaseEventSink PerkPointEventSink;

    RE::BSEventNotifyControl PerkPointIncreaseEventSink::ProcessEvent(const RE::PerkPointIncreaseEvent& a_event, RE::BSTEventSource<RE::PerkPointIncreaseEvent>* a_source)
    {
        REX::INFO("PerkPointsPerLevel: PerkPointIncreaseEventSink Called");
        PerkPointsPerLevel::OnPerkPointIncrease();
        return RE::BSEventNotifyControl::kContinue;
    }
}
