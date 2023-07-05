/*
** EPITECH PROJECT, 2023
** protocol_start
** File description:
** protocol_start
*/

#include "server.h"

int check_team_name(char *team_name, char **valid_teams)
{
    for (int i = 0; valid_teams[i]; i++) {
        if (strcmp(team_name, valid_teams[i]) == 0)
            return 1;
    }
    return 0;
}

void send_info_loggin(int socket, client_t *client, server_params_t
    *server_params)
{
    if (client->is_graphical) {
        handle_graphic_command(client, server_params);
        return;
    }
    char *slots_str = NULL;
    char *world_dimensions_str = NULL;
    char *output = NULL;

    if (check_team_name(client->team_name, server_params->team_names)) {
        slots_str = msprintf("%d\n", server_params->clients_per_team);
        world_dimensions_str = msprintf("%d %d\n", server_params->width,
            server_params->height);
        output = msprintf("%s%s", slots_str, world_dimensions_str);
        send_response(socket, output);
    } else
        send_response(socket, "ko\n");
    free(slots_str);
    free(world_dimensions_str);
    free(output);
}
