#include "Exit.h"

Exit::Exit(Direction direction, Room* origin, Room* destination, bool locked, const std::string& requiredKeyName) : Entity(EntityType::Exit, DirectionToString(direction), "")
, direction(direction)
, origin(origin)
, destination(destination)
, locked(locked)
, requiredKeyName(requiredKeyName)
, timeCost(10)
{}

Direction Exit::GetDirection() const {
	return direction;
}

Room* Exit::GetOrigin() const {
	return origin;
}

Room* Exit::GetDestination()const {
	return destination;
}

bool Exit::IsLocked() const {
	return locked;
}

void Exit::Unlock() {
	locked = false;
}

const std::string& Exit::GetRequiredKeyName() const {
	return requiredKeyName;
}

int Exit::GetTimeCost() const
{
	return timeCost;
}

void Exit::SetTimeCost(int minutes)
{
	timeCost = minutes;
}

std::string Exit::DirectionToString(Direction direction) {
	switch (direction)
	{
	case Direction::North: return "north";
	case Direction::South: return "south";
	case Direction::East: return "east";
	case Direction::West: return "west";
	case Direction::Up: return "up";
	case Direction::Down: return "down";
	default: return "unknown";
	}
}

bool Exit::TryReadDirection(const std::string& text, Direction& outDirection) {
	if (text == "north" || text == "n") { outDirection = Direction::North; return true; }
	if (text == "south" || text == "s") { outDirection = Direction::South; return true; }
	if (text == "east" || text == "e") { outDirection = Direction::East;  return true; }
	if (text == "west" || text == "w") { outDirection = Direction::West;  return true; }
	if (text == "up") { outDirection = Direction::Up;    return true; }
	if (text == "down") { outDirection = Direction::Down;  return true; }
	return false;
}