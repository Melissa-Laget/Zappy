/*
** EPITECH PROJECT, 2023
** handle_error
** File description:
** handle_error
*/

#include "server.h"
#include <ctype.h>

void handle_select_errors(int activity)
{
    if ((activity < 0) && (errno != EINTR))
        printf("select error");
}

int check_error_params(server_params_t params)
{
    if (params.port == 0 || params.port < 1) {
        printf("Port is not a valid number.\n");
        return EPITECH_ERROR;
    }
    if ((params.width == 0 || params.width < 10 || params.width > 30) ||
        (params.height == 0 || params.height < 10 || params.height > 30)) {
        printf("-x or -y option only accepts");
        printf(" integer values between 10 and 30\n");
        return EPITECH_ERROR;
    }
    if (params.frequency == 0 || params.frequency < 2 ||
        params.frequency > 10000) {
        printf("-f option only accepts integer values between 2 and 10000\n");
        return EPITECH_ERROR;
    }
    return 0;
}

int check_errors(int argc, char **argv, server_params_t params)
{
    if (!argv || !argv[1])
        return EPITECH_ERROR;
    if (!(argc == 1 || strcmp(argv[1], "-help") == 0))
        return 0;
    FILE *file = fopen("helper.txt", "r");
    if (file) {
        char buffer[256];
        while (fgets(buffer, sizeof(buffer), file))
            printf("%s", buffer);
        fclose(file);
        return 1;
    }
    if (check_error_params(params) == EPITECH_ERROR)
        return EPITECH_ERROR;
    return 0;
}
