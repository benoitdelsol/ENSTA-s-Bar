#include <gtk/gtk.h>
#include "callbackData.hpp"
#include "client.hpp"
#include <future>

void join(GtkWidget *widget, gpointer data)
{
  CallbackData *callbackData = (CallbackData *)data;
  callbackData->updateStatus(2);

  g_main_context_wakeup(NULL);
}

void create(GtkWidget *widget, gpointer data)
{
  // envoyer une requête au serveur pour créer un bar
  // la response doit etre de type future
  // récupérer updateStatus
  CallbackData *callbackData = (CallbackData *)data;
  std::future<std::string> response = std::async(std::launch::async, sendRequest, "http://127.0.0.1:8080/create_game");
  printf("Waiting for response\n");
  callbackData->updateStatus(-1);
  // attendre la réponse
  response.wait();
  if (response.valid())
  {
    printf("Response received\n");
    std::string response_str = response.get();
    char *response_c = new char[response_str.length() + 1];
    strcpy(response_c, response_str.c_str());
    // faire une boucle if si la réponse est de la forme "Succesfuly created a game with code :"code" et récupérer le code
    // sinon afficher un message d'erreur
    if (response_str.find("Succesfuly created a game with code :") != std::string::npos)
    {
      char *code = strtok(response_c, ":");
      code = strtok(NULL, ":");
      printf("Created a game with code : %s\n", code);
      callbackData->updateCode(code);
      callbackData->updateStatus(3);
    }
  }
  else
  {
    printf("Error while creating a game\n");
    callbackData->updateStatus(0);
  }
}

void accueilPage(void (*updateStatus)(int),void (*updateCode)(char *) ,void (*quit_app)(GtkWidget *widget, gpointer data), gboolean done, void (*gtk_window_set_child2)(GtkWindow *window, GtkWidget *child), GtkWidget *window)
{
  GtkWidget *label;
  GtkWidget *joinbutton;
  GtkWidget *createbutton;
  GtkWidget *grille;
  GtkWidget *center;

  printf("Accueil page\n");

  label = gtk_label_new("Welcome to ENSTA's Bar !");

  joinbutton = gtk_button_new_with_label("Join a bar !");
  createbutton = gtk_button_new_with_label("Create a bar !");

  CallbackData *callbackData = g_new(CallbackData, 1);
  callbackData->text;
  callbackData->updateStatus = updateStatus;
  callbackData->updateCode = updateCode;

  g_signal_connect(joinbutton, "clicked", G_CALLBACK(join), callbackData);
  g_signal_connect(createbutton, "clicked", G_CALLBACK(create), callbackData);

  grille = gtk_grid_new();
  gtk_grid_attach(GTK_GRID(grille), label, 1, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(grille), joinbutton, 0, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(grille), createbutton, 2, 1, 1, 1);

  int height = 1080;
  int width = 1920;
  int centerheight = height / 2;
  int centerwidth = width / 2;

  center = gtk_fixed_new();
  gtk_fixed_put(GTK_FIXED(center), grille, centerwidth , centerheight);
  gtk_widget_set_halign(grille, GTK_ALIGN_CENTER);
  gtk_widget_set_valign(grille, GTK_ALIGN_CENTER);


  gtk_window_set_child2(GTK_WINDOW(window), center);
}