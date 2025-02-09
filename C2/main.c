#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "listener.h"
#include "server.h"
#include "server_ui.h"

int main(void) {
    pthread_t listener_thread;
    pthread_t ui_thread;

    //start the listener in a separate thread
    if (pthread_create(&listener_thread, NULL, start_listener, NULL) != 0) {
        perror("Failed to create listener thread");
        return 1;
    }

    //start the UI thread
    if (pthread_create(&ui_thread, NULL, start_server_ui, NULL) != 0) {
        perror("Failed to create UI thread");
        return 1;
    }

    //wait for threads to finish (in practice, they might run indefinitely ^^)
    pthread_join(listener_thread, NULL);
    pthread_join(ui_thread, NULL);

    return 0;
}