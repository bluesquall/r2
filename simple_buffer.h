#ifndef SIMPLE_BUFFER_H
#define SIMPLE_BUFFER_H

#include <stdio.h> // for fprintf, stderr
#include <stdlib.h> // for free, calloc
#include <string.h> // for memset, strstr
#include <unistd.h> // for read
#include <sys/ioctl.h> // to get the number of bytes available on a fd

struct simple_buffer {
    char *data;
    size_t position;
    size_t size;
};

struct simple_buffer * simple_buffer_create( size_t size );

void simple_buffer_destroy( struct simple_buffer * b );

size_t simple_buffer_available_data( const struct simple_buffer * b );

size_t simple_buffer_available_space( const struct simple_buffer * b );

size_t simple_buffer_fill( struct simple_buffer * b, int fd );

size_t simple_buffer_get_line( char * line, struct simple_buffer * b, size_t maxlen );

void simple_buffer_print( const struct simple_buffer * b );

#endif // SIMPLE_BUFFER_H
