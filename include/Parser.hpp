#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <vector>

class Parser
{
          public:
                 static void parseCommand(const std::string &rawString, 
                    std::string &command, std::vector<std::string> &args);
          // private:
          //           Parser();
          

};

#endif