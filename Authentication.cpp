#include "Client.hpp"
#include "Server.hpp"

bool is_valid(std::string arg){
    int pos = arg.find(' ');
    return (pos == -1 && !arg.empty()) ? true : false;
}

bool checkCommand(std::string cmd){
    return (cmd == "PASS" || cmd == "NICK" || cmd == "USER")? true: false;
}

bool checkPass(std::string clientPass, std::string serverPass, int clientId){
    if((clientPass != serverPass) || !is_valid(clientPass)){
        std::string err = ERR_PASSWDMISMATCH(clientPass);
        send(clientId, err.c_str(), strlen(err.c_str()), 0);
        return (false);
    }
    return (true);
}

// bool checkNick(std::string nick, int clientId){
//     if (!is_valid(nick)){
//         std::string err = ERR_SPACENOTALLOWED(nick);
//         send(clientId, err.c_str(), strlen(err.c_str()), 0);
//         return (false);
//     }
//     return (true);
// }

// bool checkUser(std::string user, int clientId){
//     if (!is_valid(user)){
//         std::string err = ERR_SPACENOTALLOWED(user);
//         send(clientId, err.c_str(), strlen(err.c_str()), 0);
//         return (false);
//     }
//     return (true);
// }
bool Server::checkNickAvailability(const std::string& nick){
    std::vector<Client>::iterator iter;
    for (iter = this->clients.begin(); iter != this->clients.end(); iter++){
        if (iter->getNickname() == nick){
            return true;
        }
    }
    return false;
}

void Server::Authentication(std::string message, int clientId){
    std::string command, arg;

    int pos = 0;
    pos = message.find(' ');
    // if (pos == -1){
    //     std::string err = ERR_NEEDMOREPARAMS(command);
    //     send(this->polls[clientId].fd, err.c_str(), strlen(err.c_str()), 0);return;
    // }
    command = message.substr(0, pos);
    arg = message.substr(pos+1, '\n');
    arg.erase(arg.find_last_not_of("\r\n") + 1);
    command.erase(command.find_last_not_of("\r\n") + 1);
    std::cout << "CMD: |"<<command<<"|"<<std::endl;
    std::cout << "ARG: |"<<arg<<"|"<<std::endl;

    if (!checkCommand(command)){
        if (arg.empty()){
            std::string err = ERR_NEEDMOREPARAMS(command);
            send(this->polls[clientId].fd, err.c_str(), strlen(err.c_str()), 0);return;
        }
        std::string err = ERR_UNKNOWNCOMMAND(command);
        send(this->polls[clientId].fd, err.c_str(), strlen(err.c_str()), 0);
    }
    else if (command == "PASS"){
        if (checkPass(arg, this->serverpass, this->polls[clientId].fd)){
            this->newClient.setPassword(arg);
            this->newClient.has_pass = true;
        }
    }else if(command == "NICK"){
        if(!this->newClient.has_pass)
            send(this->polls[clientId].fd, ERR_NOTREGISTERED, strlen(ERR_NOTREGISTERED), 0);
        else if (checkNickAvailability(arg)){
            std::string err = ERR_NICKNAMEINUSE(arg);
            send(this->polls[clientId].fd, err.c_str(), strlen(err.c_str()), 0);
        }
        else{
            this->newClient.setNickname(arg);
            this->newClient.has_nick = true;
        }
    }else if(command == "USER" && this->newClient.has_pass){
        if(!this->newClient.has_pass)
            send(this->polls[clientId].fd, ERR_NOTREGISTERED, strlen(ERR_NOTREGISTERED), 0);
        else{
            this->newClient.setUsername(arg);
            this->newClient.has_user = true;
        }
    }
    if (this->newClient.Authontacated()){
        this->clients.push_back(this->newClient);
        this->newClient = Client();
        std::string welc = RPL_WELCOME(this->newClient.getNickname(), "welcome");
        send(this->polls[clientId].fd, welc.c_str(), strlen(welc.c_str()), 0);return;
    }
}