#pragma once

namespace PerkPointsPerLevel::Events
{
    class PerkPointIncreaseEventSink : public RE::BSTEventSink<RE::PerkPointIncreaseEvent>
    {
    public:
        virtual RE::BSEventNotifyControl ProcessEvent(const RE::PerkPointIncreaseEvent& a_event, RE::BSTEventSource<RE::PerkPointIncreaseEvent>* a_source) override;
    };

    extern PerkPointIncreaseEventSink PerkPointEventSink;
}
