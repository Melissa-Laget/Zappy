/*
** EPITECH PROJECT, 2023
** init_server
** File description:
** init_server
*/

#include "server.h"

server_params_t init_default_server_params(void)
{
    server_params_t params;

    params.port = 4242;
    params.width = 10;
    params.height = 10;
    params.team_names = NULL;
    params.clients_per_team = 3;
    params.frequency = 100;

    struct timeval tv;
    gettimeofday(&tv, NULL);
    long long ct_in_ms = tv.tv_sec * 1000LL + tv.tv_usec / 1000LL;
    params.food_spawning_timer = ct_in_ms + 126000LL / params.frequency;
    return params;
}

void parse_teams_names(int argc, char **argv, int i, server_params_t *params)
{
    int team_count = 0;
    for (int j = i + 1; j < argc; j++) {
        if (argv[j][0] == '-')
            break;
        team_count++;
    }
    params->team_names = malloc(sizeof(char *) * (team_count + 1));
    for (int k = 0; k < team_count; k++)
        params->team_names[k] = strdup(argv[i + k + 1]);
    params->team_names[team_count + 1] = NULL;
}

void parse_args(int argc, char **argv, server_params_t *params)
{
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-p") == 0)
            params->port = atoi(argv[++i]);
        if (strcmp(argv[i], "-x") == 0)
            params->width = atoi(argv[++i]);
        if (strcmp(argv[i], "-y") == 0)
            params->height = atoi(argv[++i]);
        if (strcmp(argv[i], "-n") == 0)
            parse_teams_names(argc, argv, i, params);
        if (strcmp(argv[i], "-c") == 0)
            params->clients_per_team = atoi(argv[++i]);
        if (strcmp(argv[i], "-f") == 0)
            params->frequency = atoi(argv[++i]);
    }
    define_settings_world(params);
}

int create_and_bind_socket(server_params_t params,
    struct sockaddr_in *address)
{
    if (params.port == 0 || !address)
        return -1;
    int server_socket;
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket failed");
        return -1;
    }
    address->sin_family = AF_INET;
    address->sin_addr.s_addr = INADDR_ANY;
    address->sin_port = htons(params.port);
    if (bind(server_socket,
        (struct sockaddr *)address, sizeof(*address)) < 0) {
        perror("bind failed");
        return -1;
    }
    printf("Port : %d\n", params.port);
    return server_socket;
}
