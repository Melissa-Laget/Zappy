/*
** EPITECH PROJECT, 2023
** broadcast command
** File description:
** broadcast_command
*/

#include "server.h"

char *build_message(char **args)
{
    char *message = NULL;
    for (int i = 1; args[i] != NULL; i++) {
        if (message == NULL) {
            message = strdup(args[i]);
        } else {
            char *new_message = NULL;
            new_message = msprintf("%s %s", message, args[i]);
            free(message);
            message = new_message;
        }
    }
    return message;
}

void send_to_graphical_clients(client_t *clients, char *output)
{
    for (int i = 0; i < MAX_CLIENTS; i++)
        if (clients[i].is_graphical == 1 && clients[i].is_connected == 1)
            send_response(clients[i].socket, output);
}

int get_direction(client_t *emitter, client_t *receiver)
{
    int diff_x = receiver->x_position - emitter->x_position;
    int diff_y = receiver->y_position - emitter->y_position;
    if (diff_x == 0 && diff_y == 0)
        return 0;
    double angle = atan2(diff_y, diff_x) * 180.0 / M_PI;
    int orientations[4] = {0, 90, 180, 270};
    angle = fmod(angle - orientations[receiver->orientation], 360);
    if (angle < 0)
        angle += 360;
    for (int i = 0; i < 8; i++) {
        if (angles[i][0] <= angle && angle < angles[i][1])
            return i + 1;
        if (angles[i][0] > angles[i][1]
            && (angle >= angles[i][0] || angle < angles[i][1]))
            return i + 1;
    }
    return 0;
}

void send_broadcast_to_clients(client_t *clients, client_t *client,
    char *message, server_params_t *server_params)
{
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].is_connected == 0 || clients[i].is_graphical == 1
            || clients[i].is_dead == 1 || clients[i].id == client->id)
            continue;
        char *output = NULL;
        int tile_number = get_direction(client, &clients[i]);
        output = msprintf("message %d, %s\n", tile_number, message);
        dprintf(clients[i].socket, "%s", output);
        free(output);
    }
}

void handle_broadcast_command(client_t *clients,
    client_t *client, char **args, server_params_t *server_params)
{
    if (!args || !client)
        return;
    if (args[0] == NULL)
        return;
    if (strcmp(args[0], "Broadcast") == 0) {
        if (args[1] == NULL)
            return send_response(client->socket, "ko\n");
        char *message = build_message(args);
        if (!message)
            return;
        char *output = msprintf("pbc %d %s\n", client->id, message);
        if (!output)
            return;
        send_to_graphical_clients(clients, output);
        send_broadcast_to_clients(clients, client, message, server_params);
        send_response(client->socket, "ok\n");
        free(output);
        free(message);
    }
}
