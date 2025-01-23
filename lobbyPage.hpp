#include <gtk/gtk.h>
#include "callbackData.hpp"
#include "client.hpp"
#include <future>
#include <string>
#include <vector>



void changeName(GtkWidget *widget, gpointer data)
{
    CallbackData *callbackData = (CallbackData *)data;
    GtkEntryBuffer *buffer = callbackData->text;
    char * code = callbackData->code;
    const char *text = gtk_entry_buffer_get_text(buffer);
    std::string request = "http://127.0.0.1:8080/rename?name=";
    char request2[100];
    sprintf(request2, "%s%s", request.c_str(), text,"&code=", code);
    printf("Request : %s\n", request2);
    std::future<std::string> response = std::async(std::launch::async, sendRequest, request2);
    printf("Waiting for response\n");
    response.wait();
    if(response.valid()){
        printf("Response received\n");
        std::string response_str = response.get();
        if(response_str.find("Succesfuly renamed") != std::string::npos){
            printf("Name changed to %s\n", text);
        }
    }else{
        printf("Error while changing the name\n");
    }
}


void lobbyPage(void (updateStatus)(int),void (updateCode)(char *),void (*gtk_window_set_child)(GtkWindow *window, GtkWidget *child), GtkWindow *window,std::vector<std::string> players, gboolean admin, char * code)
{
    GtkWidget *label1;
    GtkWidget *label2;
    GtkWidget *label3;
    GtkWidget *label4;
    GtkWidget *nameplayer1;
    GtkWidget *nameplayer2;
    GtkWidget *nameplayer3;
    GtkWidget *nameplayer4;
    GtkWidget *homebutton;
    GtkWidget *startbutton;
    GtkWidget *grille;
    GtkWidget *grillename;
    GtkWidget *textField;
    GtkEntryBuffer *buffer;
    GtkWidget *listejoueurs;


    buffer = gtk_entry_buffer_new(NULL, -1);



    grille = gtk_grid_new();
    gtk_grid_set_row_homogeneous(GTK_GRID(grille), TRUE);
    gtk_grid_set_column_homogeneous(GTK_GRID(grille), TRUE);

    label1 = gtk_label_new("Lobby");

    gtk_grid_attach(GTK_GRID(grille), label1, 1, 0, 1, 1);

    printf("Lobby page\n");

    label2 = gtk_label_new("Votre nom :");
    textField = gtk_entry_new();
    gtk_entry_set_buffer(GTK_ENTRY(textField), buffer);
    gtk_entry_buffer_set_text(GTK_ENTRY_BUFFER(buffer), "Joueur1", -1);
    label3 = gtk_label_new("wait for your admin to start the game");
    startbutton = gtk_button_new_with_label("Join a game");
    homebutton = gtk_button_new_with_label("Home");

    grillename = gtk_grid_new();
    gtk_grid_attach(GTK_GRID(grillename), label2, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grillename), textField, 1, 0, 1, 1);

    CallbackData *callbackData = g_new(CallbackData, 1);
    callbackData->updateStatus = updateStatus;
    callbackData->updateCode = updateCode;
    callbackData->text = buffer;
    callbackData->code = code;



    g_signal_connect(textField , "activate", G_CALLBACK(changeName), callbackData);

    g_signal_connect(homebutton, "clicked", G_CALLBACK(retour), callbackData);

    grillename=gtk_grid_new();




    //ask for the name of the player
    std::string request = "http://127.0.0.1:8080/players?code=";
    char request1[100];
    sprintf(request1, "%s%s", request.c_str(), code);
    printf("Request : %s\n", request1);
    std::future<std::string> response3 = std::async(std::launch::async, sendRequest, request1);
    printf("Waiting for response\n");
    response3.wait();
    if(response3.valid()){
        //la réponse se présente sous la forme Voici la liste des joueurs :joueur1:joueur2:joueur3:joueur4
        //on doit extraire les noms des joueurs
        printf("Response received\n");
        std::string response_str = response3.get();
        char *response_c = new char[response_str.length() + 1];
        strcpy(response_c, response_str.c_str());
        printf("Response : %s\n", response_c);
        char *player = strtok(response_c, ":");
        while(player != NULL){
            players.push_back(player);
            player = strtok(NULL, ":");
        }
        //enlever le premeier élément de players
        players.erase(players.begin());
        std::vector<GtkWidget *> nameplayers = {nameplayer1,nameplayer2,nameplayer3,nameplayer4 };



        listejoueurs = gtk_grid_new();
        label4 = gtk_label_new("Players :");
        gtk_grid_attach(GTK_GRID(listejoueurs), label4, 0, 0, 1, 1);

        //print players size
        printf("Players size : %ld\n", players.size());

        for(int i = 0; i<players.size(); i++){
            printf("Player %d : %s\n", i, players[i].c_str());
            nameplayers[i] = gtk_label_new(players[i].c_str());
            gtk_grid_attach(GTK_GRID(listejoueurs), nameplayers[i], 0, i+1, 1, 1);
        }
        gtk_grid_set_row_homogeneous(GTK_GRID(listejoueurs), TRUE);
        gtk_grid_set_column_homogeneous(GTK_GRID(listejoueurs), TRUE);
        gtk_grid_attach(GTK_GRID(grille), listejoueurs, 1, 1, 1, 1);



    }else{
        printf("Error while getting the players\n");

    }



    
    if(admin){
        gtk_grid_attach(GTK_GRID(grille), startbutton, 2, 2, 1, 1);
    }else{
        gtk_grid_attach(GTK_GRID(grille), label3, 2, 2, 1, 1);
    }
    gtk_grid_attach(GTK_GRID(grille), homebutton, 0, 2, 1, 1);


    gtk_window_set_child(window, grille);

}