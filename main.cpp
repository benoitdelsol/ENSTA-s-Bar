#include <gtk/gtk.h>
#include "callbackData.hpp"
#include "client.hpp"
#include <future>
#include "accueilPage.hpp"
#include "joinPage.hpp"
#include "waitingPage.hpp"
#include "lobbyPage.hpp"
#include <vector>
#include <string>   

GtkWidget *window;

gboolean done = FALSE;

gboolean admin=TRUE;

void quit_app(GtkWidget *widget, gpointer data)
{
    done = TRUE;
    g_main_context_wakeup(NULL);
}

char code[5];

void updateCode(char *newCode)
{
    printf("Code : %s\n", newCode);
    strcpy(code, newCode);

}


int status = 1;


gboolean gameHasStarted = FALSE;

std::vector<std::string> players;

void updateStatus(int newStatus)
{
    status = newStatus;
    printf("Status : %d\n", newStatus);
    if(status==1){
        admin=TRUE;
        accueilPage(updateStatus, updateCode, quit_app, done, gtk_window_set_child, window);
    }
    else if(status==2){
        admin =FALSE;
        joinPage(updateStatus, updateCode, quit_app, done, gtk_window_set_child, GTK_WINDOW(window));
    }
    else if(status==-1){
        waitingPage(updateStatus, quit_app, done, gtk_window_set_child, GTK_WINDOW(window));
    }
    else if(status==3){
        printf("Entering lobby with code : %s\n", code);
        lobbyPage(updateStatus, updateCode, gtk_window_set_child, GTK_WINDOW(window), players, admin, code);
    }
}



int main(){
    gtk_init();

    window = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(window), "ENSTA's Bar");
    gtk_window_fullscreen(GTK_WINDOW(window));

    
    g_signal_connect(window, "destroy", G_CALLBACK(quit_app), NULL);

    accueilPage(updateStatus, updateCode, quit_app, done, gtk_window_set_child, window);

    gtk_widget_set_visible(window, TRUE);


  

    while(!done){
        g_main_context_iteration(NULL,TRUE);
    }
    return 0;
}