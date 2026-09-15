#pragma once

#include "Creature.h"
#include <vector>
#include <string>

class NPC : public Creature {
private:
	std::vector<std::string> dialogueScript;
	size_t nextLine;

public:
	NPC(const std::string& name, const std::string& description, Room* location);

	void AddDialogueLine(const std::string& line);

	std::string Talk();

};
