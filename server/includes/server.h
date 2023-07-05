/*
** EPITECH PROJECT, 2023
** Server
** File description:
** Server
*/

#ifndef SERVER_H_
    #define SERVER_H_

    #include <arpa/inet.h>
    #include <errno.h>
    #include <netinet/in.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <sys/select.h>
    #include <sys/socket.h>
    #include <unistd.h>
    #include <stdbool.h>
    #include <time.h>
    #include <math.h>
    #include <sys/time.h>
    #define _GNU_SOURCE

    #define MAX_CLIENTS 500
    #define BUFFER_SIZE 4096
    #define CLIENT_NAME "Anonymous"
    #define EPITECH_ERROR 84
    #define M_PI 3.14159265358979323846

    static double angles[8][2] = {
        { 337.5, 22.5},
        { 292.5, 337.5},
        { 247.5, 292.5},
        { 202.5, 247.5},
        { 157.5, 202.5},
        { 112.5, 157.5},
        { 67.5, 112.5},
        { 22.5,  67.5}
    };

    enum direction {
        FIRST,
        NORTH,
        EAST,
        SOUTH,
        WEST
    };

    typedef struct coord_params {
        int x1;
        int y1;
        int x2;
        int y2;
        int width;
        int height;
    } coord_params_t;

    typedef struct tile {
        int x;
        int y;
        int food;
        int linemate;
        int deraumere;
        int sibur;
        int mendiane;
        int phiras;
        int thystame;
        int *eggs;
    } tile_t;

    typedef struct command {
        char *name;
        char **args;
        long long execution_time;
        int executed;
        struct command *next;
    } command_t;

    typedef struct client {
        int id;
        int x_position;
        int y_position;
        int orientation;
        int level;
        int socket;
        char *team_name;
        int start_loggin;
        int is_graphical;
        int food;
        int linemate;
        int deraumere;
        int sibur;
        int mendiane;
        int phiras;
        int thystame;
        int is_connected;
        command_t *commands;
        int command_count;
        int is_dead;
        long long food_losing_timer;
        int team_max_clients;
        int is_elevating;
    } client_t;

    typedef struct server_params {
        int port;
        int width;
        int height;
        char **team_names;
        int clients_per_team;
        int frequency;
        tile_t **world;
        long long food_spawning_timer;
    } server_params_t;

    typedef struct DistributionParams {
        server_params_t *params;
        int total_resource;
        char resource;
    } DistributionParams;

    typedef struct {
        int x;
        int y;
    } coordinate_t;

    typedef struct command_info {
        const char *name;
        double execution_time_factor;
    } command_info_t;

// compare position and level
int compare_positions_and_level(client_t *cl1, client_t *cl2);

// init food losing timer
long long init_food_losing_timer(server_params_t *server_params);

// readfds
void setup_readfds(int server_socket, fd_set *readfds);

//void spawn_food(server_params_t *server_params)
void spawn_food(server_params_t *server_params);

// check events server
void check_events_server(client_t *client, client_t *clients,
    server_params_t *server_params, int i);

// check lose food
void check_lose_food(client_t *client, server_params_t *server_params);

// free command
void free_command(command_t *command);

// int calc_distance
int calc_distance(int x1, int y1, int x2, int y2);

// get tna
char *get_tna(server_params_t *server_params, client_t *client);

// get connect_nbr
int get_connect_nbr(client_t *clients, client_t *client);

// build_message_incantation
int build_message_incantation(client_t *clients,
    client_t *client, char *message);

// lib function
int generate_rand_position(int upper_limit);

// identify tile
int identify_tile(client_t *emitter,
    client_t *receiver, coord_params_t params);

int manhattan_distance_torus(coord_params_t params);

// remove stones
void remove_stones(tile_t *tile, int level);

// check stone
bool check_stone(tile_t *tile, int level);

// msprintf function
char *msprintf(const char *format, ...);

// free command
void free_command(command_t *command);

// free command args
void free_command_args(char **args);

// duplicate args
char **duplicate_args(char **args);

// Vérifie les erreurs de la ligne de commande
int check_errors(int argc, char **argv, server_params_t params);

// Initialise la liste des clients connectés
void init_clients_list(client_t *clients, server_params_t *server_params);

// Crée le socket et le lie à l'adresse du serveur
int create_and_bind_socket(server_params_t params,
    struct sockaddr_in *address);

// Attend les connexions entrantes et gère les clients connectés
void wait_for_connections(int server_socket, client_t *clients,
    struct sockaddr_in address, server_params_t *server_params);

// Ajoute un nouveau client à la liste des clients connectés
void update_client_struct(int new_socket, client_t *clients, server_params_t
    *server_params);

// Traite les erreurs du select
void handle_select_errors(int activity);

// Configure les descripteurs de fichiers des clients connectés pour le select
int set_clients_sockets(client_t *clients, fd_set *readfds, int server_socket);

// Accepte une nouvelle connexion entrante
int accept_new_connection(int server_socket, struct sockaddr_in address);

// Vérifie les descripteurs de fichiers des clients connectés une activité
void check_client_activity(client_t *clients,
    int server_socket, fd_set *readfds, server_params_t *server_params);

// Envoie une réponse au client
void send_response(int socket, char *message);

// Mets à jour la position des joueurs apres qu'ils se soient fait ejectés
void update_client_position(client_t *client, int orientation,
    server_params_t *server_params);

// Lis le message du client
ssize_t read_method(int socket, char *buffer);

// Gère les commandes du client
void handle_command(client_t *client, server_params_t *server_params,
    char **args);

// Gère les commandes du client avec un numéro de joueur
void handle_command_with_player_nbr(client_t *clients, client_t *client,
    server_params_t *server_params, char **args);

// handle_graphic_command_two
void handle_broadcast_command(client_t *clients,
    client_t *client, char **args, server_params_t *server_params);

// handle_graphic_command_three
void handle_eject_command(client_t *clients, client_t *client,
    server_params_t *server_params, char **args);

// Gère la deconnexion du client
void handle_disconnect(client_t *client, client_t *clients,
    server_params_t *server_params);

// Récupère les arguments du client
char **get_args_from_buffer(char *buffer);

// Libère un tableau de char **
void free_array(char **array);

// Parse les arguments passés en parametre au binaire
void parse_args(int argc, char **argv, server_params_t *params);

// handle_graphic_command
void handle_graphic_command(client_t *client, server_params_t *server_params);

// Print mct
void print_mct(server_params_t *server_params, client_t *client);

// Print tna
void print_tna(server_params_t *server_params, client_t *client);

// Print msz
void print_msz(server_params_t *server_params, client_t *client);

// Print sgt
void print_sgt(server_params_t *server_params, client_t *client);

// Print bct
void print_bct(server_params_t *server_params, client_t *client, char **args);

// Print ppo
void print_ppo(client_t *client, client_t *player);

// concat strings
char *concat_strings(char *output, char *temp);

// Initalise la structure par défaut
server_params_t init_default_server_params(void);

// check if the team name is valid
int check_team_name(char *team_name, char **valid_teams);

// send the team info to the client (from protocol of the start)
void send_info_loggin(int socket, client_t *client, server_params_t
    *server_params);

// define settings of the world
void define_settings_world(server_params_t *params);

// lib function to get random coordinate with a limit
int get_random_coordinate(int limit);

// concat strings
char *concat_strings(char *output, char *temp);

// send notification to the graphical client when a player loggin
void send_notification_player_loggin(client_t *clients,
    client_t *client_logged_in, server_params_t *server_params);

// distribute resources
void distribute_resources(server_params_t *params,
    int total_resource, char resource);
void distribute_food(server_params_t *params);
void distribute_linemate(server_params_t *params);
void distribute_deraumere(server_params_t *params);
void distribute_sibur(server_params_t *params);
void distribute_mendiane(server_params_t *params);
void distribute_phiras(server_params_t *params);
void distribute_thystame(server_params_t *params);

// send message to graphical
void send_message_to_graphical(client_t *clients, char *message);

// send message to graphical start incantation
void send_message_to_graphical_start_incantation(client_t *clients,
    client_t *client, server_params_t *server_params, char **args);

// client movement

void handle_forward_command(server_params_t *params, client_t *client,
                            char **args);
void handle_right_command(client_t *client, char **args);
void handle_left_command(client_t *client, char **args);

// client food
void print_inventory(server_params_t *server_params, client_t *client);
void take_command(server_params_t *server_params,
    client_t *client, char **argv);
void set_command(server_params_t *server_params,
    client_t *client, char **argv);

// client command
void handle_connect_nbr_command(client_t *clients, client_t *client,
    server_params_t *server_params, char **args);

// client incantation
void handle_incantation_command(client_t *clients,
    client_t *client, server_params_t *server_params, char **args);

// client fork
void handle_fork_command(client_t *client,
    client_t *clients, server_params_t *server_params, char **args);

// client look
void handle_look_command(client_t *clients, client_t *client,
    server_params_t *server_params, char **args);

coordinate_t get_relative_coords(client_t *client, int x, int y);

bool get_player_on_tile(client_t *clients, char *tiles_content, int *i,
                        coordinate_t coord);

bool get_relative_tile_items(coordinate_t coord, char *tiles_content, int *i,
                            server_params_t *server_params);

// check if client can do incantation
int can_do_incantation(client_t *clients, client_t *client,
    server_params_t *server_params, char **args);

// check if player is dead
void check_death_player(client_t *clients,
    client_t *client, server_params_t *server_params);

// handle client request
void handle_client_request(client_t *clients,
    char *buffer, client_t *client, server_params_t *server_params);

// create new command
command_t *create_new_command(char **args, server_params_t *server_params);

// int of array
int length_of_int(int *array);

// check if the user is valid
int is_valid_user(client_t *client);

// get the number of player on a tile
int get_nbr_on_tile(client_t *clients, client_t *client);

// find empty slot
int find_empty_slot(client_t *clients);

// win condition
void check_win_event(client_t client, client_t *clients,
                    server_params_t *server_params);

#endif /* !SERVER_H_ */
