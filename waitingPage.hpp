#include <gtk/gtk.h>

void waitingPage(void (*updateStatus)(int), void (*quit_app)(GtkWidget *widget, gpointer data), gboolean done, void (*gtk_window_set_child)(GtkWindow *window, GtkWidget *child), GtkWindow *window) {
  GtkWidget *spinner;

    spinner = gtk_spinner_new();
    gtk_spinner_start(GTK_SPINNER(spinner));

  gtk_window_set_child(window, spinner);
}