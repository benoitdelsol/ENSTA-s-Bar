#include <gtk/gtk.h>
#include <vector>
#include "lobbyPage.hpp"



void gamePage( void (updateStatus)(int),void (updateCode)(char *), void (quit_app)(GtkWidget *widget, gpointer data), gboolean done,void(*gtk_window_set_child)(GtkWindow *window, GtkWidget *child),GtkWindow *window, char* code, gboolean admin){




lobbyPage(updateStatus,updateCode,gtk_window_set_child,window,players, admin, code);

while (1)
{
    /* code */
}




}