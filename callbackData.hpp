#ifndef CALLBACK_H
#define CALLBACK_H
struct CallbackData {
  GtkEntryBuffer *text;
  char * code;
  void (*updateStatus)(int);
  void (*updateCode)(char *);
};
#endif