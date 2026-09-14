#pragma once

#include "Types.h"
#include <iostream>
#include <list>
#include <memory>
#include <string>

class Entity {
protected:
	EntityType type;
	std::string name;
	std::string description;
	std::list<std::unique_ptr<Entity>> content;

public:
	Entity(EntityType type, const std::string& name, const std::string& description);
	virtual ~Entity() = default; //~ to create the destructor, and it is marked `virtual` to free the player's memory in the event that the `Entity` is destroyed

	virtual void Update() {}

	const std::string& GetName() const;
	const std::string& GetDescription() const;
	EntityType GetType() const;

	void AddEntity(std::unique_ptr<Entity> entity);
	std::unique_ptr<Entity> RemoveEntity(const std::string& name);
	Entity* FindEntity(const std::string& name) const;

	const std::list<std::unique_ptr<Entity>>& GetContents() const;
	bool IsEmpty() const;
};
