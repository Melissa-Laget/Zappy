/*
** EPITECH PROJECT, 2023
** zappy main$
** File description:
** main
*/

#include "server.h"

void print_params(server_params_t params)
{
    printf("\nSettings recupéres depuis les paramètres:\n\n");
    printf("Port:\t%d\n", params.port);
    printf("Width:\t%d\n", params.width);
    printf("Height:\t%d\n", params.height);
    printf("Clients per Team:\t%d\n", params.clients_per_team);
    printf("Frequency:\t%d\n\n", params.frequency);
    if (!params.team_names)
        return;
    printf("Team Names:\n");
    for (int i = 0; params.team_names[i]; i++)
        printf("- %s\n", params.team_names[i]);
    printf("\n\n");
}

int main(int argc, char **argv)
{
    server_params_t params = init_default_server_params();
    parse_args(argc, argv, &params);
    int errorChecked = check_errors(argc, argv, params);
    if (errorChecked == 84)
        return EPITECH_ERROR;
    if (errorChecked == 1)
        return 0;
    print_params(params);
    client_t *clients = calloc(MAX_CLIENTS, sizeof(client_t));
    struct sockaddr_in address;
    init_clients_list(clients, &params);
    int server_socket = create_and_bind_socket(params, &address);
    if (server_socket == -1)
        return EPITECH_ERROR;
    if (listen(server_socket, 3) < 0)
        return EPITECH_ERROR;
    wait_for_connections(server_socket, clients, address, &params);
    free(clients);
    return 0;
}
