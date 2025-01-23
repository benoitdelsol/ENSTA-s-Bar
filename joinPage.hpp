#include <gtk/gtk.h>
#include "callbackData.hpp"
#include "client.hpp"
#include <future>
#include <string>


void retour(GtkWidget *widget, gpointer data){
    CallbackData *callbackData = (CallbackData *)data;
    printf("trying to go back to the main menu\n");
    callbackData->updateStatus(1);

    g_main_context_wakeup(NULL);
}

void tryJoin(GtkWidget *widget, gpointer data){
    CallbackData *callbackData = (CallbackData *)data;
    GtkEntryBuffer *buffer = callbackData->text;
    const char *text = gtk_entry_buffer_get_text(buffer);
    printf("Joining bar with code %s\n", text);
    std::string request = "http://127.0.0.1:8080/join?code=";
    request += text;
    //create response as a future variable
    std::future<std::string> response = std::async(std::launch::async, sendRequest, request);
    printf("Waiting for response\n");
    callbackData->updateStatus(-1);
    //wait for the response
    response.wait();
    if(response.valid()){
        printf("Response received\n");
        std::string response_str = response.get();
        char *response_c = new char[response_str.length() + 1];
        strcpy(response_c, response_str.c_str());
        //if the response is of the form "Succesfuly joined a game with code :"code" then get the code
        //else display an error message
        if(response_str.find("Succesfuly joined the game with code :") != std::string::npos){
            char *code = strtok(response_c, ":");
            code = strtok(NULL, ":");
            printf("Joined a game with code : %s\n", code);
            callbackData->updateCode(code);
            callbackData->updateStatus(3);
        }
    }
    g_main_context_wakeup(NULL);
}

void joinPage( void (updateStatus)(int),void (updateCode)(char *), void (quit_app)(GtkWidget *widget, gpointer data), gboolean done,void(*gtk_window_set_child)(GtkWindow *window, GtkWidget *child),GtkWindow *window){
    GtkWidget *label;
    GtkWidget *joinbutton;
    GtkWidget *homebutton;
    GtkWidget *grille;
    GtkWidget *textField;
    GtkEntryBuffer *buffer;


    label = gtk_label_new("Joining a Bar !");

    buffer = gtk_entry_buffer_new(NULL, -1);
    textField = gtk_entry_new();
    gtk_entry_buffer_set_text(buffer, "Joueur1", -1);
    gtk_entry_set_placeholder_text(GTK_ENTRY(textField), "Enter the bar code here");
    //every character entered will store in the buffer
    gtk_entry_set_buffer(GTK_ENTRY(textField), buffer);
    // the entry must have a max length of 4
    gtk_entry_set_max_length(GTK_ENTRY(textField), 4);
    //on enter, we must execute the join function

    joinbutton = gtk_button_new_with_label("Join");

    CallbackData *callbackData = g_new(CallbackData, 1);
    callbackData->text=buffer;
    callbackData->updateStatus = updateStatus;
    callbackData->updateCode = updateCode;


    g_signal_connect(textField, "activate", G_CALLBACK(tryJoin), callbackData);

    homebutton = gtk_button_new_with_label("Retour");
    g_signal_connect(homebutton, "clicked", G_CALLBACK(retour), callbackData);

    joinbutton = gtk_button_new_with_label("Join");
    g_signal_connect(joinbutton, "clicked", G_CALLBACK(tryJoin), &updateStatus);
    
    grille = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(grille), 10);
    gtk_grid_set_row_spacing(GTK_GRID(grille), 10);
    gtk_grid_attach(GTK_GRID(grille), label, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grille), textField, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grille), joinbutton, 2, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grille), homebutton, 0, 2, 1, 1);


    gtk_window_set_child(window, grille);

    return;

}