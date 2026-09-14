#pragma once

#include <string>

//Interprets various inputs to create commands. ex - verb="go" + target="north" / verb"put" + target="key" + target="bag" (put key in bag)
struct Command {
	std::string verb;
	std::string target;
	std::string targetTwo;
};

class CommandReader {
public:
	static Command Reader(const std::string& input); //Converts text in usable commands
};
