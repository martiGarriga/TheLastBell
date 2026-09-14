#pragma once

#include "Entity.h"

//Since Item inherits from Entity, we can make an Entity contain an Entity using the "content" variable.
class Item : public Entity {
private:
	bool canContainItems;

	int timeBonus;

public:
	Item(const std::string& name, const std::string& description, bool canContainItems = false);

	bool CanContainItems() const;

};
