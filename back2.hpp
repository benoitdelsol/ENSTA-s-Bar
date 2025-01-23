#include <vector>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <algorithm>

struct room
{
    // users is a list of websocket of the users in the room
    std::vector<int> users;
    // code is the code of the room
    std::string code;
    std::vector<std::string> joueurs;

    // Constructor
    room(std::string code, int user)
    {
        this->code = code;
        this->users.push_back(user);
        this->joueurs.push_back("joueur1");
    }

    void join(int user)
    {
        if(this->users.size() == 1){
            this->joueurs.push_back("joueur2");
        }
        if(this->users.size() == 2){
            this->joueurs.push_back("joueur3");
        }
        if(this->users.size() == 3){
            this->joueurs.push_back("joueur4");
        }
        this->users.push_back(user);
    }

    void leave(int user)
    {
        this->users.erase(std::find(this->users.begin(), this->users.end(), user));
        if (this->users.size() == 0)
        {
            delete this;
        }
    }

    void sendMessage(std::string message)
    {
        for (int u : this->users)
        {
            send(u, message.c_str(), message.size(), 0);
        }
    }

    void rename(int user, std::string name)
    {
        // rename the user
        
        for(int i =0; i<this->users.size(); i++){
            if(this->users[i] == user){
                this->joueurs[i] = name;
            }
        }
    }
};
