#pragma once

#include "Creature.h"

class Player : public Creature 
{
public:
	Player(const std::string& name, Room* location);

	bool PickUp(const std::string& itemName);

	bool Drop(const std::string& itemName);

	bool PutInside(const std::string& itemName, const std::string& containerName);

	void ListInventory() const;
};
