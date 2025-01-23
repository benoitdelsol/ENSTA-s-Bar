#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <list>
#include "back2.hpp"

#define PORT 8080
#define MAX_CLIENTS 100
#define BUFFER_SIZE 1024

typedef struct
{
    int socket;
    char room[50];
} Client;

Client clients[MAX_CLIENTS];
int client_count = 0;

std::vector<room> join_room(int client_socket, const char *room_name, std::vector<room> rooms)
{
    // vérifier qu'il existe une room avec le code donné
    // si oui, ajouter le client à la room
    for (room r : rooms)
    {
        if (r.code == room_name)
        {
            // vérifier que la room ne soit pas pleine
            if (r.users.size() == 4)
            {
                const char *response = "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\nContent-Length: 9\r\n\r\nRoom is full";
                send(client_socket, response, strlen(response), 0);
                return rooms;
            }
            char response[1000];
            char response1[1000];
            sprintf(response1, "Succesfuly joined the game with code :%s", room_name);
            sprintf(response, "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: %ld\r\n\r\n%s", strlen(response1), response1);
            send(client_socket, response, strlen(response), 0);

            printf("Nombre de joueurs dans la room avant le join %ld", r.joueurs.size());

            r.join(client_socket);

            printf("Nombre de joueurs dans la room après le join %ld", r.joueurs.size());
            // print les joueurs dans la room
            for (int i = 0; i < r.users.size(); i++)
            {
                printf("user %d : %d\n", i, r.users[i]);
            }
            return rooms;
        }
    }
    // sinon, envoyer un message d'erreur
    const char *response = "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\nContent-Length: 9\r\n\r\nNot Found";
    send(client_socket, response, strlen(response), 0);
    return rooms;
}

void send_to_room(const char *room_name, const char *message)
{
    for (int i = 0; i < client_count; i++)
    {
        if (strcmp(clients[i].room, room_name) == 0)
        {
            send(clients[i].socket, message, strlen(message), 0);
        }
    }
}

std::vector<room> create_game(std::vector<room> rooms, int client_socket)
{
    // Créer une nouvelle room
    // Générer un code aléatoire
    // Ajouter le client à la room
    printf("Création d'une nouvelle room\n");
    char code[5];
    // générer un code de 4 lettres aléatoires
    for (int i = 0; i < 4; i++)
    {
        code[i] = 'A' + rand() % 26;
    }

    code[4] = '\0'; // Null-terminate the string
    printf("Code généré : %s\n", code);
    // vérifier si rooms n'est pas vide
    if (rooms.size() != 0)
    {
        printf("rooms n'est pas vide\n");
        for (room r : rooms)
        {
            if (r.code == code)
            {
                rooms = create_game(rooms, client_socket);
                return rooms;
            }
        }
    }
    // Créer la room
    room new_room(code, client_socket);
    rooms.push_back(new_room);

    printf("Nouvelle room créée avec le code %s\n", code);

    // Envoyer le code de la room au client
    char response[1000];
    char response1[1000];
    sprintf(response1, "Succesfuly created a game with code :%s", code);
    sprintf(response, "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: %ld\r\n\r\n%s", strlen(response1), response1);
    send(client_socket, response, strlen(response), 0);
    return rooms;
}

int main()
{
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    // Création du socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("Échec de la création du socket");
        exit(EXIT_FAILURE);
    }

    // Attachement du socket au port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("Échec du bind");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 10) < 0)
    {
        perror("Échec de listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Serveur démarré sur le port %d\n", PORT);

    // Liste des rooms
    std::vector<room> rooms;
    rooms.push_back(room("AAAA", 0));

    while (1)
    {
        buffer[0] = '\0';
        char response[100] = {0};
        char *code = NULL;

        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
        {
            perror("Échec de accept");
            continue;
        }

        // Lecture de la requête
        read(new_socket, buffer, BUFFER_SIZE);
        printf("Requête reçue :\n%s\n", buffer);

        // Vérification de la commande
        if (strstr(buffer, "GET /create_game"))
        {
            rooms = create_game(rooms, new_socket);
        }
        // si la requête est de type GET /join?code=AAAA
        else if (strstr(buffer, "GET /join?code="))
        {
            printf("Requête de join\n");
            code = strstr(buffer, "code=") + 5;
            char room_name[50] = {0};
            sscanf(code, "%s", room_name); // Récupérer le nom de la room
            printf(room_name);
            rooms = join_room(new_socket, room_name, rooms);
        }
        else if (strstr(buffer, "GET /players?code="))
        {
            // lister l'ensemble des joueurs dans la room
            printf("asking for players\n");
            code = strstr(buffer, "code=") + 5;
            char room_name[50] = {0};
            sscanf(code, "%s", room_name); // Récupérer le nom de la room
            printf("room_name : %s\n", room_name);
            std::string code_str = room_name;

            for (room r : rooms)
            {
                printf("%s\n", r.code.substr(0, 4).c_str());
                printf("%s\n", code_str.substr(0, 4).c_str());
                std::string r_code_part = r.code.substr(0, 4);
                std::string code_str_part = code_str.substr(0, 4);

                printf("r.code.substr(0,4): '%s' (size: %zu)\n", r_code_part.c_str(), r_code_part.size());
                printf("code_str.substr(0,4): '%s' (size: %zu)\n", code_str_part.c_str(), code_str_part.size());

                printf("Hex r.code.substr(0,4):");
                for (unsigned char c : r_code_part)
                    printf(" %02X", c);
                printf("\n");

                printf("Hex code_str.substr(0,4):");
                for (unsigned char c : code_str_part)
                    printf(" %02X", c);
                printf("\n");

                if (r.code.substr(0, 4) == code_str.substr(0, 4))
                {
                    printf("ouiiiiiiiiii");
                    // envoyer la liste des joueurs
                    std::string response = "Voici la liste des joueurs :";
                    for (std::string joueur : r.joueurs)
                    {
                        response += joueur + ":";
                    }
                    char message[100];
                    sprintf(message, "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: %ld\r\n\r\n%s", response.size(), response.c_str());
                    send(new_socket, message, strlen(message), 0);
                }
            }
        }
        else if (strstr(buffer, "GET /rename?name="))
        {
            // on a une requête de la forme GET /rename?name=joueur1&code=AAAA
            char *name = strstr(buffer, "name=") + 5;
            char *code = strstr(buffer, "code=") + 5;
            char room_name[50] = {0};
            char player_name[50] = {0};
            sscanf(code, "%s", room_name);   // Récupérer le nom de la room
            sscanf(name, "%s", player_name); // Récupérer le nom du joueur
            for (room r : rooms)
            {
                if (r.code == room_name)
                {
                    // renommer le joueur
                    r.rename(new_socket, player_name);
                    const char *response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 9\r\n\r\nSuccesfuly renamed";
                    send(new_socket, response, strlen(response), 0);
                    return 0;
                }
            }
            std::string error = "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\nContent-Length: 9\r\n\r\nNot Found";
            send(new_socket, error.c_str(), strlen(error.c_str()), 0);
        }
        else if (strstr(buffer, "GET /send?room="))
        {
            char *room = strstr(buffer, "room=") + 5;
            char room_name[50] = {0};
            sscanf(room, "%s", room_name);

            // Envoyer un message à tous les clients de la room
            send_to_room(room_name, "Message to the room\n");

            const char *response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 17\r\n\r\nMessage sent\n";
            send(new_socket, response, strlen(response), 0);
        }
        else
        {
            const char *not_found = "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\nContent-Length: 9\r\n\r\nNot Found";
            send(new_socket, not_found, strlen(not_found), 0);
        }
    }

    close(new_socket);

    return 0;
}
