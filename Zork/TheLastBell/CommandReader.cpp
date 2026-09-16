#include "CommandReader.h"
#include <sstream>
#include <vector>
#include <algorithm>

namespace 
{
	std::string ToLower(const std::string& text) 
	{
		std::string result = text;
		std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c) {return static_cast<char>(std::tolower(c)); });
		return result;
	}

	std::vector<std::string> Tokenize(const std::string& text) 
	{ //Enters "Put key in bag" and returns {"put", "key", "in", "bag"}
		std::vector<std::string> tokens;
		std::istringstream stream(text); //treats a string as an input stream
		std::string token;
		while (stream >> token) 
		{
			tokens.push_back(token);
		}
		return tokens;
	}

	std::string Join(const std::vector<std::string>& tokens, size_t from, size_t to) 
	{
		std::string result;
		for (size_t i = from; i < to && i < tokens.size(); i++) 
		{
			if (!result.empty()) 
			{
				result += " ";
			}
			result += tokens[i];
		}
		return result;
	}
}

Command CommandReader::Reader(const std::string& input) 
{
	Command command;
	std::vector<std::string> tokens = Tokenize(ToLower(input));

	if (tokens.empty()) 
	{
		return command;
	}

	command.verb = tokens[0];

	if (command.verb == "put") 
	{
		size_t inIndex = std::string::npos;
		for (size_t i = 1; i < tokens.size(); ++i) 
		{
			if (tokens[1] == "in") 
			{
				inIndex = i;
				break;
			}
		}

		if (inIndex != std::string::npos) 
		{
			command.target = Join(tokens, 1, inIndex);
			command.target = Join(tokens, inIndex + 1, tokens.size());
			return command;
		}
	}

	command.target = Join(tokens, 1, tokens.size());
	return command;
}