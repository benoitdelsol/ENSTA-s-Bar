#include <gtk/gtk.h>

GtkWidget* roomPage( void (updateStatus)(int), void (quit_app)(GtkWidget *widget, gpointer data), gboolean done){
    GtkWidget *label;
    GtkWidget *column1;
    GtkWidget *joinbutton;
    GtkWidget *createbutton;
    GtkWidget *row1;
    GtkWidget *centerbox;

    centerbox=gtk_center_box_new();

    label = gtk_label_new("Room Page");

    joinbutton = gtk_button_new_with_label("Join a bar !");
    g_signal_connect(joinbutton, "clicked", G_CALLBACK(quit_app), &updateStatus);

    createbutton = gtk_button_new_with_label("Create a bar !");
    g_signal_connect(createbutton, "clicked", G_CALLBACK(quit_app), &done);
    
    row1=gtk_box_new(0,10);
    gtk_box_append(GTK_BOX(row1),joinbutton);
    gtk_box_append(GTK_BOX(row1),createbutton);

    column1=gtk_box_new(1,10);
    gtk_box_append(GTK_BOX(column1),label);
    gtk_box_append(GTK_BOX(column1),row1);

    gtk_center_box_set_center_widget(GTK_CENTER_BOX(centerbox),column1);

    return centerbox;

}