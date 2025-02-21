#include <iostream>
#include <string.h>
#include <vector>
#include <stdbool.h>

class Channel{
    private:
        std::string name;
        std::string topic;
        bool op;
    public:
        Channel(){

        };
        ~Channel(){

        };

        std::string getName(){
            return (this->name);
        };
        std::string getTopic(){
            return (this->topic);
        };
        bool getOperator(){
            return (this->op);
        };
        void setName(std::string name){
            this->name = name;
        };
        void setTopic(std::string topic){
            this->topic = topic;
        };
        void setOperator(bool op){
            this->op = op;
        };

};

//Test Write TOPIC command in irc server
int main(int ac, char **av)
{
    Channel my_channel;

    my_channel.setName("CHIDO_channel");
    my_channel.setTopic("Welcome to the chido channel");
    my_channel.setOperator(1);
    if (ac == 2 || ac == 3)
    {
        if (ac == 2)
        {
            std::cout << "The topic is " << my_channel.getTopic() << std::endl;
        }
        else if (ac == 3)
        {
            std::string new_topic = av[2];
            my_channel.setTopic(new_topic);
            std::cout << "New topic is " << my_channel.getTopic() << std::endl;
        }
    }
    else
        std::cout << "Invalid Number Of Parameter " << std::endl;

}