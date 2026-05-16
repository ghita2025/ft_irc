#include "Parser.hpp"
#include <cctype>

void Parser::parseCommand(const std::string &rawString,
    std::string &command,
    std::vector<std::string> &args)
{
    command.clear();
    args.clear();

    if (rawString.empty())
        return;

    std::string raw = rawString;

    if (!raw.empty() && raw[raw.size() - 1] == '\r')
        raw.erase(raw.size() - 1);

    if (raw.size() > 510)
        return;

    while (!raw.empty() && raw[0] == ' ')
        raw.erase(0, 1);

    if (raw.empty())
        return;

 
    if (raw[0] == ':')
    {
        size_t prefixEnd = raw.find(' ');

        if (prefixEnd == std::string::npos)
            return;

        raw.erase(0, prefixEnd + 1);
    }

    size_t pos = raw.find(' ');

    if (pos == std::string::npos)
    {
        command = raw;

        for (size_t i = 0; i < command.size(); ++i)
            command[i] = std::toupper(command[i]);

        return;
    }

    command = raw.substr(0, pos);

    for (size_t i = 0; i < command.size(); ++i)
        command[i] = std::toupper(command[i]);

    std::string rest = raw.substr(pos + 1);

    while (!rest.empty())
    {
        while (!rest.empty() && rest[0] == ' ')
            rest.erase(0, 1);

        if (rest.empty())
            break;

        if (rest[0] == ':')
        {
            args.push_back(rest.substr(1));
            break;
        }

        size_t spacePos = rest.find(' ');

        if (spacePos == std::string::npos)
        {
            args.push_back(rest);
            break;
        }

        std::string word = rest.substr(0, spacePos);

        if (!word.empty())
            args.push_back(word);

        rest.erase(0, spacePos + 1);
    }
}