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

typedef size_t (*frame_finder)( const char * data, char * begin );

struct r2_buffer * r2_buffer_create( size_t size );

void r2_buffer_destroy( struct r2_buffer * self );

size_t r2_buffer_available_data( const struct r2_buffer * self );

size_t r2_buffer_available_space( const struct r2_buffer * self );

size_t r2_buffer_fill( struct r2_buffer * self, int fd );

/*  Get a generic data frame from the buffer.
 *
 *  Uses frame_finder function pointed to by argument f to find the frame.
 */
size_t r2_buffer_get_frame( struct r2_buffer * self, char * frame,
        frame_finder f );

/*  Get any line from the buffer.
 *
 *  Searches first for \r\n, then \n\r, then \n, then \r.
 *
 *  If compiled with DEBUG flag, prints the terminator to stderr.
 */
size_t r2_buffer_get_any_line( struct r2_buffer * self, char * line,
        size_t maxlen );

void r2_buffer_print( const struct r2_buffer * self );

#endif // R2_BUFFER_H

#ifndef R2_BUFFER_I
#define R2_BUFFER_I

struct r2_buffer * r2_buffer_create( size_t size )
{
    struct r2_buffer * self = calloc(1, sizeof(struct r2_buffer));
    self->size = size;
    self->position = 0;
    self->data = calloc(size + 1, 1);
    return self;
}

void r2_buffer_destroy( struct r2_buffer * self )
{
    if( self ) {
        free(self->data);
        free(self);
    }
}

size_t r2_buffer_available_data( const struct r2_buffer * self )
{
   return self->position;
}

size_t r2_buffer_available_space( const struct r2_buffer * self )
{
    return self->size - self->position;
}

size_t r2_buffer_fill( struct r2_buffer * self, int fd )
{
    size_t space = self->size - self->position;
    size_t incoming = 0; // bytes available on the serial port
    ioctl(fd, FIONREAD, &incoming); // TODO: check return value
    size_t bytes_to_read = space > incoming ? incoming : space;
    ssize_t bytes_read = read(fd, self->data + self->position, bytes_to_read);
    if( -1 == bytes_read ) {
        perror( "r2_buffer_fill read()" );
    } else {
        self->position += bytes_read;
    }
    return bytes_read;
}

size_t r2_buffer_get_frame( struct r2_buffer * self, char * frame,
        frame_finder f )
{
    fprintf(stderr, "r2_buffer_get_frame not yet implemented\n");
    return 0;
}


size_t r2_buffer_get_any_line( struct r2_buffer * self, char * line,
        size_t maxlen )
{
    size_t delimiter_position = 0;
    size_t delimiter_size = 0;
    memset(self->data + self->position + 1, '\0', 1); // limit the strstr search, just in case
    char * found = strstr(self->data, "\r\n"); // TODO: Search for either, or, both in both orders
    if( NULL != found ) {
        delimiter_size = 2;
#ifdef DEBUG
        fprintf(stderr, "found CR LF\n");
#endif
    } else {
        found = strstr(self->data, "\n\r");
        if( NULL != found ) {
            delimiter_size = 2;
#ifdef DEBUG
            fprintf(stderr, "found LF CR instead of CR LF\n");
#endif
        } else {
            found = strchr(self->data, '\n');
            if( NULL != found ) {
                delimiter_size = 1;
#ifdef DEBUG
                fprintf(stderr, "found LF only\n");
#endif
            } else {
                found = strchr(self->data, '\r');
                if( NULL != found ) {
                    delimiter_size = 1;
#ifdef DEBUG
                    fprintf(stderr, "found CR only\n");
#endif
                }
            }
        }
    }
    if( NULL != found ) {
        delimiter_position = found - self->data;
        size_t next_line_position = delimiter_position + delimiter_size;
        if( delimiter_position < maxlen ) {
            memset(self->data + delimiter_position, '\0', 1);
            strcpy(line, self->data);
        } else {
            delimiter_position = 0;
        }
        self->position -= next_line_position; // decrement the buffer byte count
        memmove(self->data, self->data + next_line_position, self->position);
    }
    else if( self->size == self->position ) {
        fprintf(stderr, "r2_buffer filled without any lines -- clearing\n");
        self->position = 0;
        memset(self->data, '\0', self->size);
    }
    return delimiter_position;
}

void r2_buffer_print( const struct r2_buffer * self )
{
    size_t i;
    fprintf( stderr, "%zub | ", self->position );
    for( i = 0; i < self->position; i++ )
        fprintf( stderr, "%c", self->data[i] );
    fprintf( stderr, " |\t|" );
    for( i = 0; i < self->position; i++ )
        fprintf( stderr, " %02x", self->data[i] );
    fprintf( stderr, " |\n" );
}

#endif // R2_BUFFER_I
