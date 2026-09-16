#include "NPC.h"

NPC::NPC(const std::string& name, const std::string& description, Room* location) : Creature(EntityType::NPC, name, description, location), nextLine(0) {}

void NPC::AddDialogueLine(const std::string& line) 
{
	dialogueScript.push_back(line);
}

std::string NPC::Talk() 
{
	if (dialogueScript.empty()) 
	{
		return GetName() + " He/She has nothing to say."; 
	}

	const std::string& line = dialogueScript[nextLine];
	nextLine = (nextLine + 1) % dialogueScript.size();
	return line;
}