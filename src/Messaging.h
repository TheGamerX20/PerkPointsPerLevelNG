#pragma once

namespace PerkPointsPerLevel::Messaging
{
    void Register();
    void F4SEMessageListener(F4SE::MessagingInterface::Message* a_msg);
}
