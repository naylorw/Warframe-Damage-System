#pragma once

class Faction
{
public:
	enum FactionEnum
	{
		NONE,
		TENNO,
		GRINEER,
		CORPUS,
		INFESTED,
		OROKIN,
		SENTIENT,
		STALKER,
		NARMER,
		MURMUR,
		SCALDRA,
		TECHROT,
		SYNDICATES,
		WILD
	};

	Faction() = default;
	constexpr Faction(FactionEnum aFaction) : _faction(aFaction) {}

	// Allow switch and comparisons.
	constexpr operator FactionEnum() const { return _faction; }

	// Prevent usage: if(faction)
	explicit operator bool() const = delete;

private:
	FactionEnum _faction;
};