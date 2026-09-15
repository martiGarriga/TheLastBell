#pragma once

#include "Entity.h"
#include <string>

class Room;

class Exit : public Entity {
private:
	Direction direction;
	Room* origin;
	Room* destination;

	bool locked;
	std::string requiredKeyName;

	int timeCost;

public:
	Exit(Direction direction, Room* origin, Room* destination, bool locked = false, const std::string& requiredKeyName = "");

	Direction GetDirection() const;
	Room* GetOrigin() const;
	Room* GetDestination() const;

	bool IsLocked() const;
	void Unlock();
	const std::string& GetRequiredKeyName() const;

	int GetTimeCost() const;
	void SetTimeCost(int minutes);

	static std::string DirectionToString(Direction direction);
	static bool TryReadDirection(const std::string& text, Direction& outDirection);
};