#include "Player.h"
#include "Room.h"
#include "Item.h"
#include <iostream>

Player::Player(const std::string& name, Room* location) : Creature(EntityType::Player, name, "Player.", location) {}

bool Player::PickUp(const std::string& itemName) 
{
	if (location == nullptr) 
	{
		return false;
	}

	Entity* found = location->FindEntity(itemName);
	if (found == nullptr) {
		std::cout << "There is no \"" << itemName << "\" here. \n";
	}

	std::unique_ptr<Entity> item = location->RemoveEntity(itemName);
	if (item == nullptr) {
		return false;
	}

	AddEntity(std::move(item));
	std::cout << "You obtained: " << itemName << "\n";
	return true;
}

bool Player::Drop(const std::string& itemName) 
{
	Entity* found = FindEntity(itemName);
	if (found == nullptr) 
	{
		std::cout << "You dont have any \"" << itemName << "\" in your inventary. \n";
		return false;
	}

	std::unique_ptr<Entity> item = RemoveEntity(itemName);
	if (item == nullptr || location == nullptr) 
	{
		return false;
	}

	location->AddEntity(std::move(item));
	std::cout << "You dropped: " << itemName << "\n";
	return true;
}

bool Player::PutInside(const std::string& itemName, const std::string& containerName) {
	Entity* container = FindEntity(containerName);
	if (container == nullptr) 
	{
		std::cout << "You dont have any \"" << containerName << "\".\n";
		return false;
	}

	if (container->GetType() != EntityType::Item) 
	{
		std::cout << containerName << "it can't storage anything. \n";
	}

	Item* containerItem = static_cast<Item*>(container);
	if (!containerItem->CanContainItems()) 
	{
		std::cout << containerName << " it can't storage anything else. \n";
	}

	Entity* itemToMove = FindEntity(itemName);
	if (itemToMove == nullptr) 
	{
		std::cout << "You dont have any \"" << containerName << "\".\n";
	}

	if (itemToMove == container) 
	{
		std::cout << " You can't put items inside itself.\n";
	}

	std::unique_ptr<Entity> item = RemoveEntity(itemName);
	if (item == nullptr) 
	{
		return false;
	}

	containerItem->AddEntity(std::move(item));
	std::cout << "You put " << itemName << "inside a " << containerName << ".\n";
	return true;
}

void Player::ListInventory() const 
{
	if (IsEmpty()) {
		std::cout << "You aren't carrying anything. \n";
	}

	std::cout << "You carry: \n";
	for (const std::unique_ptr<Entity>& entity : GetContents()) 
	{
		std::cout << "  - " << entity->GetName() << "\n";
	}
}