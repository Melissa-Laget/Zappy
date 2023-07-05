/*
** EPITECH PROJECT, 2023
** send_message_to_graphical
** File description:
** send_message_to_graphical
*/

#include "server.h"

void send_message_to_graphical(client_t *clients, char *message)
{
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].is_connected == 1 && clients[i].is_graphical == 1)
            dprintf(clients[i].socket, "%s", message);
    }
}
