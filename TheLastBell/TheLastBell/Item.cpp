#include "Item.h"

Item::Item(const std::string& name, const std::string& description, bool canContainItems) : Entity(EntityType::Item, name, description)
, canContainItems(canContainItems)
/*, timeBonus(0) por si pongo tiempo limite al final*/

{}

bool Item::CanContainItems() const {
	return canContainItems;
}
