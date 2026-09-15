#include "Creature.h"

Creature::Creature(EntityType type, const std::string& name, const std::string& description, Room* location) : Entity(type, name, description)
, location(location) {}

Room* Creature::GetLocation() const 
{
	return location;
}

void Creature::MoveTo(Room* room) 
{
	location = room;
}