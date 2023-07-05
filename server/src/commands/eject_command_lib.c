/*
** EPITECH PROJECT, 2023
** broadcast command
** File description:
** broadcast_command
*/

#include "server.h"

void update_client_position_x(client_t *client, int orientation,
    server_params_t *server_params)
{
    if (orientation == EAST) {
        if (client->x_position < server_params->width - 1)
            client->x_position += 1;
        else
            client->x_position = 0;
    }
    if (orientation == WEST) {
        if (client->x_position > 0)
            client->x_position -= 1;
        else
            client->x_position = server_params->width - 1;
    }
}

void update_client_position_y(client_t *client, int orientation,
    server_params_t *server_params)
{
    if (orientation == NORTH) {
        if (client->y_position > 0)
            client->y_position -= 1;
        else
            client->y_position = server_params->height - 1;
    }
    if (orientation == SOUTH) {
        if (client->y_position < server_params->height - 1)
            client->y_position += 1;
        else
            client->y_position = 0;
    }
}

void update_client_position(client_t *client, int orientation,
    server_params_t *server_params)
{
    update_client_position_x(client, orientation, server_params);
    update_client_position_y(client, orientation, server_params);
}
