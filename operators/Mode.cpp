#include <iostream>
#include <string.h>
#include <sstream>
#include <vector>
#include <ctime>

class Channel {
private:
    std::string name;                 // Channel name
    std::string topic;                // Current topic of the channel
    std::string password;             // Channel password (if needed)
    int userLimit;                    // Maximum number of users allowed in the channel
    std::vector<std::string> operators;  // vector of operator nicknames
    std::vector<std::string> bannedUsers; // vector of banned users (could be nicknames, IPs, or hosts)
    std::vector<std::string> activeUsers; // vector of users currently in the channel
    std::vector<std::string> modes;   // Channel mode flags (e.g., +i, +m, +t, etc.)
    std::time_t creationTime;
};
//Test Write MODE command in irc server
int main()
{
    std::string cmd = "MODE NICK";
    std::vector<std::string> params;

    std::istringstream ss(cmd);  // Create a stringstream from the cmd string
    std::string tmp;

    while (ss >> tmp)
    {
        params.push_back(tmp);
    }

    auto i = params.begin();
    if (params.size() > 1)
    {

    }
    else 
        std::cout << "Not enough parameters" << std::endl;

    return 0;
}