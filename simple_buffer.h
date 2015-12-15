// simple_buffer.h
// A (very) simple buffer.

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

#ifndef SIMPLE_BUFFER_I
#define SIMPLE_BUFFER_I

struct simple_buffer * simple_buffer_create( size_t size )
{
    struct simple_buffer * b = calloc(1, sizeof(struct simple_buffer));
    b->size = size;
    b->position = 0;
    b->data = calloc(size + 1, 1);
    return b;
}

void simple_buffer_destroy( struct simple_buffer * b )
{
    if( b ) {
        free(b->data);
        free(b);
    }
}

size_t simple_buffer_available_data( const struct simple_buffer * b )
{
   return b->position;
}

size_t simple_buffer_available_space( const struct simple_buffer * b )
{
    return b->size - b->position;
}

size_t simple_buffer_fill( struct simple_buffer * b, int fd )
{
    size_t space = b->size - b->position;
    size_t incoming = 0; // bytes available on the serial port
    ioctl(fd, FIONREAD, &incoming); // TODO: check return value
    size_t bytes_to_read = space > incoming ? incoming : space;
    size_t bytes_read = read(fd, b->data + b->position, bytes_to_read);
    b->position += bytes_read;
    return bytes_read;
}

size_t simple_buffer_get_line( char * line, struct simple_buffer * b, size_t maxlen )
{
    size_t delimiter_position = 0;
    memset(b->data + b->position + 1, '\0', 1); // limit the strstr search, just in case
    char * found = strstr(b->data, "\r\n"); // TODO: Search for either, or, both in both orders
    if( NULL == found ) {
        found = strstr(b->data, "\n\r");
#ifdef DEBUG
        if( NULL != found ) {
            fprintf(stderr, "found LF CR instead of CR LF\n");
        }
    } else {
        fprintf(stderr, "found CR LF\n");
#endif
    }
    if( NULL != found ) {
        delimiter_position = found - b->data;
        size_t next_line_position = delimiter_position + 2;
        if( delimiter_position < maxlen ) {
            memset(b->data + delimiter_position, '\0', 2); // clear the delimiter so that strstr does not find it next time
            strcpy(line, b->data);
        } else {
            delimiter_position = 0;
        }
        b->position -= next_line_position; // decrement the buffer byte count
        memmove(b->data, b->data + next_line_position, b->position);
    }
    else if( b->size == b->position ) {
        b->position = 0;
        memset(b->data, '\0', b->size);
    }
    return delimiter_position;
}

void simple_buffer_print( const struct simple_buffer * b )
{
    size_t i;
    fprintf(stderr, "| ");
    for( i = 0; i < b->position; i++) fprintf(stderr, "%c", b->data[i]);
    fprintf(stderr, " |\t|");
    for( i = 0; i < b->position; i++) fprintf(stderr, " %02x", b->data[i]);
    fprintf(stderr, " |\n");
    fflush(stderr);
}

#endif // SIMPLE_BUFFER_I
