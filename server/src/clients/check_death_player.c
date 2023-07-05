/*
** EPITECH PROJECT, 2023
** check_death_player
** File description:
** check_death_player
*/

#include "server.h"

void check_death_player(client_t *clients,
    client_t *client, server_params_t *server_params)
{
    char *output_to_graphical = NULL;
    if (client->is_dead == 1)
        return;
    if (client->food <= 0) {
        client->is_dead = 1;
        dprintf(client->socket, "dead\n");
        output_to_graphical = msprintf("pdi %d\n", client->id);
        send_message_to_graphical(clients, output_to_graphical);
    }
}
