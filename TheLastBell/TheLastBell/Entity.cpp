#include "Entity.h"

Entity::Entity(EntityType type, const std::string& name, const std::string& description)
	: type(type)
	, name(name)
	, description(description)
{}

const std::string& Entity::GetName() const {
	return name;
}

const std::string& Entity::GetDescription() const {
	return description;
}

EntityType Entity::GetType() const {
	return type;
}

void Entity::AddEntity(std::unique_ptr<Entity> entity) {
	if (entity != nullptr) {
		content.push_back(std::move(entity));
	}
}

std::unique_ptr<Entity> Entity::RemoveEntity(const std::string& name) {
	for (auto it = content.begin(); it != content.end(); ++it) {
		if ((*it)->GetName() == name) {
			std::unique_ptr<Entity> found = std::move(*it);
			content.erase(it);
			return found;
		}
	}
	return nullptr;
}

Entity* Entity::FindEntity(const std::string& name) const {
	for (const std::unique_ptr<Entity>& entity : content) {
		if (entity->GetName() == name) {
			return entity.get();
		}
	}
	return nullptr;
}

const std::list<std::unique_ptr<Entity>>& Entity::GetContents() const {
	return content;
}

bool Entity::IsEmpty() const {
	return content.empty();
}