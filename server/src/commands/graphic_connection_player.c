/*
** EPITECH PROJECT, 2023
** graphic_connection_player
** File description:
** graphic_connection_player
*/

#include "server.h"

void send_notif_to_graphic_on_loggin(client_t *client_logged_in,
    int graphic_socket)
{
    char *output = NULL;
    output = msprintf("pnw %d %d %d %d %d %s\n",
        client_logged_in->id,
        client_logged_in->x_position,
        client_logged_in->y_position,
        client_logged_in->orientation,
        client_logged_in->level,
        client_logged_in->team_name);
    send_response(graphic_socket, output);
    free(output);
}

void send_notification_player_loggin(client_t *clients,
    client_t *client_logged_in, server_params_t *server_params)
{
    for (int i = 0; i < MAX_CLIENTS; i++)
        if (clients[i].is_graphical == 1 &&
            client_logged_in->is_graphical == 0)
            send_notif_to_graphic_on_loggin(client_logged_in,
                clients[i].socket);
}
