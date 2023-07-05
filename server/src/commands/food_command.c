/*
** EPITECH PROJECT, 2023
** server
** File description:
** food_command.c
*/

#include "server.h"

void print_inventory(server_params_t *server_params, client_t *client)
{
    dprintf(client->socket,
            "[ food %d, linemate %d, deraumere %d, sibur %d, mendiane %d, "
            "phiras %d, thystame %d ]\n",
            client->food,
            client->linemate,
            client->deraumere,
            client->sibur,
            client->mendiane,
            client->phiras,
            client->thystame);
}

static void take_object(const client_t *client, int *const *resources,
    int *const *inventory, int i)
{
    if (*resources[i] > 0) {
        (*resources[i])--;
        (*inventory[i])++;
        write(client->socket, "ok\n", 3);
    } else
        write(client->socket, "ko\n", 3);
}

void take_command(server_params_t *server_params,
    client_t *client, char **argv)
{
    tile_t *tile =
        &server_params->world[client->y_position][client->x_position];
    char **names = (char *[]){ "food", "linemate", "deraumere",
        "sibur", "mendiane", "phiras",
        "thystame", NULL };
    int **resources = (int *[]){ &tile->food, &tile->linemate,
        &tile->deraumere, &tile->sibur,
        &tile->mendiane, &tile->phiras,
        &tile->thystame, NULL };
    int **inventory = (int *[]){ &client->food, &client->linemate,
        &client->deraumere, &client->sibur,
        &client->mendiane, &client->phiras,
        &client->thystame, NULL };

    for (int i = 0; names[i] != NULL; i++) {
        if (strcmp(argv[1], names[i]) == 0) {
            take_object(client, resources, inventory, i);
            return;
        }
    }
}

static void set_object(const client_t *client, int *const *resources,
    int *const *inventory, int i)
{
    if (*inventory[i] > 0) {
        (*resources[i])++;
        (*inventory[i])--;
        write(client->socket, "ok\n", 3);
    } else
        write(client->socket, "ko\n", 3);
}

void set_command(server_params_t *server_params,
    client_t *client, char **argv)
{
    tile_t *tile =
        &server_params->world[client->y_position][client->x_position];
    char **names = (char *[]){ "food", "linemate", "deraumere",
        "sibur", "mendiane", "phiras", "thystame", NULL };
    int **resources = (int *[]){ &tile->food, &tile->linemate,
        &tile->deraumere, &tile->sibur,
        &tile->mendiane, &tile->phiras,
        &tile->thystame, NULL };
    int **inventory = (int *[]){ &client->food, &client->linemate,
        &client->deraumere, &client->sibur,
        &client->mendiane, &client->phiras,
        &client->thystame, NULL };
    for (int i = 0; names[i] != NULL; i++) {
        if (strcmp(argv[1], names[i]) == 0) {
            set_object(client, resources, inventory, i);
            return;
        }
    }
}
