/*
** EPITECH PROJECT, 2023
** lib_client
** File description:
** lib
*/

#include "server.h"
#include <stdarg.h>

void send_response(int socket, char *message)
{
    write(socket, message, strlen(message));
    fflush(stdout);
}

ssize_t read_method(int socket, char *buffer)
{
    memset(buffer, 0, BUFFER_SIZE);
    ssize_t bytes_read = read(socket, buffer, BUFFER_SIZE - 1);
    if (bytes_read < 0) {
        perror("read");
        return -1;
    }
    buffer[bytes_read] = '\0';
    printf("Message reçu:%s.\n", buffer);
    return bytes_read;
}

char *msprintf(const char *format, ...)
{
    char *str = NULL;
    va_list args;
    int len = 0;

    va_start(args, format);
    len = vsnprintf(NULL, 0, format, args);
    va_end(args);
    if (len >= 0) {
        str = calloc(len + 1, sizeof(char));
        if (str) {
            va_start(args, format);
            vsnprintf(str, len + 1, format, args);
            va_end(args);
        }
    }
    return str;
}

char *concat_strings(char *output, char *temp)
{
    char *new_output;

    if (output == NULL) {
        new_output = strdup(temp);
    } else {
        char *old_output = output;
        new_output = msprintf("%s%s", old_output, temp);
        free(old_output);
    }
    return new_output;
}
