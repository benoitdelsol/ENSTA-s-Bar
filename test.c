#include <gtk/gtk.h>
#include <iostream>
#include <string.h>
#include <socketio_client.cpp>

void quit_app(GtkWidget *widget, gpointer data)
{
    gboolean *done = data;
    *done = TRUE;

    g_main_context_wakeup(NULL);
}

int main(int argc, char const *argv[])
{
    gtk_init();
    gboolean done = FALSE;

    GtkWidget *window;
    GtkWidget *button;

    window = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(window), "ENSTA's Bar !");
    gtk_window_fullscreen(GTK_WINDOW(window));
    int width = gtk_widget_get_width(window);
    int height = gtk_widget_get_height(window);

    button = gtk_button_new_with_label("Quit");
    g_signal_connect(button, "clicked", G_CALLBACK(quit_app), &done);

    g_signal_connect(window, "destroy", G_CALLBACK(quit_app), NULL);

    return 0;
}
