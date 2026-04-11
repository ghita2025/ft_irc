#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <string>
#include <vector>

class Command
{
    public:
        std::string name;
        std::vector<std::string> args;
};

#endif