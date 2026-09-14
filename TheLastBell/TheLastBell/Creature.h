#pragma once

#include "Entity.h"

class Room;

//Class to create specific location to the future heirs Player and NPC
class Creature : public Entity {
protected:
	Room* location; //dont posseses the room location

public:
	Creature(EntityType type, const std::string& name, const std::string& description, Room* location);

	Room* GetLocation() const;
	virtual void MoveTo(Room* room);
};