#pragma once

#include "Entity.h"
#include <vector>

class Exit;

class Room : public Entity 
{
private:
	std::vector<Exit*> exits;

public:
	Room(const std::string& name, const std::string& description);

	void AddExit(Exit* exit);
	Exit* GetExit(Direction direction) const;
	const std::vector<Exit*>& GetExits() const;

	std::string GetFullDescription() const;
};

