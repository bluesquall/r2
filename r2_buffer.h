// r2_buffer.h
// A (very) simple buffer.

#ifndef R2_BUFFER_H
#define R2_BUFFER_H

#include <stdio.h> // for fprintf, stderr
#include <stdlib.h> // for free, calloc
#include <string.h> // for memset, strstr
#include <unistd.h> // for read
#include <sys/ioctl.h> // to get the number of bytes available on a fd

struct r2_buffer {
    char *data;
    size_t position;
    size_t size;
};

struct r2_buffer * r2_buffer_create( size_t size );

void r2_buffer_destroy( struct r2_buffer * b );

size_t r2_buffer_available_data( const struct r2_buffer * b );

size_t r2_buffer_available_space( const struct r2_buffer * b );

size_t r2_buffer_fill( struct r2_buffer * b, int fd );

size_t r2_buffer_get_line( char * line, struct r2_buffer * b, size_t maxlen );

void r2_buffer_print( const struct r2_buffer * b );

#endif // R2_BUFFER_H

#ifndef R2_BUFFER_I
#define R2_BUFFER_I

struct r2_buffer * r2_buffer_create( size_t size )
{
    struct r2_buffer * b = calloc(1, sizeof(struct r2_buffer));
    b->size = size;
    b->position = 0;
    b->data = calloc(size + 1, 1);
    return b;
}

void r2_buffer_destroy( struct r2_buffer * b )
{
    if( b ) {
        free(b->data);
        free(b);
    }
}

size_t r2_buffer_available_data( const struct r2_buffer * b )
{
   return b->position;
}

size_t r2_buffer_available_space( const struct r2_buffer * b )
{
    return b->size - b->position;
}

size_t r2_buffer_fill( struct r2_buffer * b, int fd )
{
    size_t space = b->size - b->position;
    size_t incoming = 0; // bytes available on the serial port
    ioctl(fd, FIONREAD, &incoming); // TODO: check return value
    size_t bytes_to_read = space > incoming ? incoming : space;
    size_t bytes_read = read(fd, b->data + b->position, bytes_to_read);
    b->position += bytes_read;
    return bytes_read;
}

size_t r2_buffer_get_line( char * line, struct r2_buffer * b, size_t maxlen )
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

void r2_buffer_print( const struct r2_buffer * b )
{
    size_t i;
    fprintf(stderr, "| ");
    for( i = 0; i < b->position; i++) fprintf(stderr, "%c", b->data[i]);
    fprintf(stderr, " |\t|");
    for( i = 0; i < b->position; i++) fprintf(stderr, " %02x", b->data[i]);
    fprintf(stderr, " |\n");
    fflush(stderr);
}

#endif // R2_BUFFER_I
