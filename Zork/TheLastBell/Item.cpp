#include "Item.h"

Item::Item(const std::string& name, const std::string& description, bool canContainItems) : Entity(EntityType::Item, name, description)
, canContainItems(canContainItems)
//, timeBonus(0)

{}

bool Item::CanContainItems() const 
{
	return canContainItems;
}

int Item::GetTimeBonus() const
{
    return timeBonus;
}

void Item::SetTimeBonus(int minutes)
{
    timeBonus = minutes;
}