#pragma once

#include "Messaging.h"
#include "Serialization.h"
#include "SimpleINI.h"

namespace PerkPointsPerLevel
{
	// Config
	inline std::map<int, float> PointsPerLevelMap;

	// Save Data
	inline int16_t LastKnownPlayerLevel = 1;
	inline float_t PointFractionLeft = 0.0f;

	inline bool LoadConfig()
	{
		// SimpleINI Setup
		CSimpleIniA INI;
		INI.SetUnicode();

		// Load File and Check for Errors
		SI_Error rc = INI.LoadFile("Data/F4SE/Plugins/pppl.ini");
		if (rc < 0) { return false; };

		// Get all Keys in the Points Section
		const char* INISection = "points";
		CSimpleIniA::TNamesDepend INIKeys;
		INI.GetAllKeys(INISection, INIKeys);

		// Loop through our INI Keys
		for (const auto& INIKey : INIKeys)
		{
			float Value = static_cast<float>(std::stod(INI.GetValue(INISection, INIKey.pItem, "1.0")));
			int Key = std::stoi(INIKey.pItem);

			// Add them to our PointsPerLevel Map
			PointsPerLevelMap[Key] = Value;
		}

		// Is our Map filled?
		if (PointsPerLevelMap.empty())
		{
			return false;
		}

		return true;
	}

	inline bool Install()
	{
		if (!LoadConfig())
		{
			REX::WARN("PerkPointsPerLevel: Missing / Invalid Config, Exiting!");
			return false;
		}

		PerkPointsPerLevel::Messaging::Register();
		PerkPointsPerLevel::Serialization::Register();

		return true;
	}

	inline float GetCumulativePerkPoints(int FromLevel, int ToLevel)
	{
		float TotalPointsToGive = 0.0f;

		for (int Level = FromLevel + 1; Level <= ToLevel; ++Level)
		{
			std::map<int, float>::iterator Iterator = PointsPerLevelMap.lower_bound(Level);
			if (Iterator == PointsPerLevelMap.end() || Iterator->first > Level)
			{
				if (Iterator == PointsPerLevelMap.begin())
					TotalPointsToGive += 1.0f;
				else
					TotalPointsToGive += std::prev(Iterator)->second;
			}
			else
			{
				TotalPointsToGive += Iterator->second;
			}
		}

		return TotalPointsToGive;
	}

	inline void OnPerkPointIncrease()
	{
		// Get the Player Singleton
		RE::PlayerCharacter* Player = RE::PlayerCharacter::GetSingleton();

		// Player Level Difference
		int16_t LevelDifference = Player->GetLevel() - LastKnownPlayerLevel;

		// Have they Leveled Up?
		if (LevelDifference > 0)
		{
			REX::INFO("PerkPointsPerLevel: Player Leveled up by {} Levels", LevelDifference);

			// Remove the Perk Points the Player got
			uint8_t FinalAdjustment = -(uint8_t)LevelDifference;

			// Calculate ActualPointsToGive and Update PointFractionLeft
			float_t DesiredPointsToGive = GetCumulativePerkPoints(LastKnownPlayerLevel, Player->GetLevel()) + PointFractionLeft;
			int ActualPointsToGive = static_cast<int>(std::floor(DesiredPointsToGive));
			PointFractionLeft = DesiredPointsToGive - ActualPointsToGive;

			// Add the Points we need to give
			FinalAdjustment += (uint8_t)ActualPointsToGive;

			// Give the Perk Points
			Player->perkCount += (uint8_t)FinalAdjustment;

			// Log
			REX::INFO("PerkPointsPerLevel: Gave {} Perk Points with a Fraction remaining of {}, Perk Point Count is now {}", ActualPointsToGive, PointFractionLeft, (uint8_t)Player->perkCount);
			
			// Update LastKnownPlayerLevel
			LastKnownPlayerLevel = Player->GetLevel();

			// Update UI (Needs this Delay)
			F4SE::GetTaskInterface()->AddTask([=]() {
				RE::BSTGlobalEvent::EventSource<RE::PerkPointIncreaseEvent>* Source = RE::PerkPointIncreaseEvent::GetEventSource();
				Source->Notify(RE::PerkPointIncreaseEvent((uint8_t)Player->perkCount));
				});
		}
	}
}
