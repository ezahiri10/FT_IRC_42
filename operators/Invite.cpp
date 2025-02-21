#include <iostream>
#include <string.h>
#include <vector>


//Test Write INVITE command in irc server
int main(int ac, char **av)
{
    if (ac == 3)
    {
        std::string channel = av[1];
        std::string user = av[2];
        std::vector<std::string> nicknames;
        nicknames.push_back("User1");
        nicknames.push_back("User2");
        nicknames.push_back("User3");
        nicknames.push_back(user);
        for (auto i = nicknames.begin(); i < nicknames.end(); i++){
            std::cout << *i << std::endl;    
        }
    }
    else
        std::cout << "Invalid Number Of Parameter " << std::endl;

}