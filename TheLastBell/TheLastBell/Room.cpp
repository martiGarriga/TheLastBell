#include "Room.h"
#include "Exit.h"
#include <sstream>

Room::Room(const std::string& name, const std::string& description) : Entity(EntityType::Room, name, description) {}

void Room::AddExit(Exit* exit) {
	if (exit != nullptr) {
		exits.push_back(exit);
	}
}

Exit* Room::GetExit(Direction direction) const {
	for (Exit* exit : exits) {
		if (exit->GetDirection() == direction) {
			return exit;
		}
	}
	return nullptr;
}

const std::vector<Exit*>& Room::GetExits() const {
	return exits;
}

std::string Room::GetFullDescription() const {
	std::ostringstream out;
	out << GetName() << "\n" << GetDescription() << "\n";

	if (!IsEmpty()) {
		out << "\nYou see in the room:\n";
		for (const std::unique_ptr<Entity>& entity : GetContents()) {
			out << "  - " << entity->GetName() << "\n";
		}
	}

	if (!exits.empty()) {
		out << "\nExits available:\n";
		for (const Exit* exit : exits) {
			out << "  - " << Exit::DirectionToString(exit->GetDirection());
			if (exit->IsLocked()) {
				out << " closed";
			}
			out << "\n";
		}
	}
	return out.str();
}

