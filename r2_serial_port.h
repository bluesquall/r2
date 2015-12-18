// r2_serial_port.h
//

#ifndef R2_SERIAL_PORT_H
#define R2_SERIAL_PORT_H

#include <fcntl.h> // for open, O_RDWR, O_NOCTTY, O_NDELAY, O_NONBLOCK
#include <stdio.h> // for fprintf, stderr
#include <termios.h> // for serial port

#include <sys/ioctl.h> // to get the number of bytes available on a serial port
#include <sys/select.h> // to select on the file descriptor for a serial port

#endif // R2_SERIAL_PORT_H

struct r2_serial_port {
    int fd;
    struct r2_buffer * buffer;
};

const struct termios R2_SERIAL_DEFAULT_OPTIONS = {
    .c_iflag = 0,
    .c_oflag = 0,
    .c_cflag = B9600 | CS8 | CLOCAL | CREAD,
    .c_lflag = 0,
    .c_cc[VMIN] = 0,
    options.c_cc[VTIME] = 0
};

struct r2_serial_port * r2_serial_port_create( const char * device,
        size_t buffer_size );

void r2_serial_port_destroy( struct r2_serial_port * self );

/*  Sets the termios options for the serial port.

    Any options not explicitly specified are set according to
    R2_SERIAL_DEFAULT_OPTIONS.
 */
int r2_serial_port_set_options( struct r2_serial_port * self,
        struct termios * options );

#ifndef R2_SERIAL_PORT_I
#define R2_SERIAL_PORT_I

struct r2_serial_port * r2_serial_port_create( const char * device,
        size_t buffer_size )
{
    struct r2_serial_port * self = calloc( 1, sizeof( struct r2_serial_port ) );

    self->fd = open( device, O_RDWR | O_NOCTTY | O_NDELAY | O_NONBLOCK );
    if( -1 == self->fd ) {
        perror( "open()" );
        fprintf( stderr, "could not open device: %s\n", device );
        return NULL;
    }

    r2_serial_port_set_options( self, NULL );

    self->buffer = r2_buffer_create( buffer_size );
    if( !self->buffer ) {
        fprintf( stderr, "could not create %zub buffer\n", buffer_size );
        return NULL;
    }

    return self;
}


void r2_serial_port_destroy( struct r2_serial_port_t * self )
{
    if( self ) {
        r2_buffer_destroy( self->buffer );
        free( self );
    }
}


int r2_serial_port_set_options( struct r2_serial_port_t * self,
         struct termios * options )
{
    int retval = -1;

    tcflush( self->fd, TCIFLUSH );

    if( NULL == options ) {
        retval = tcsetattr( self->fd, &R2_SERIAL_DEFAULT_OPTIONS );
    } else {
        fprintf(stderr, "i: %d, o:%d\n", options->c_iflag, options->c_oflag );
        if( NULL == options->c_iflag )
            options->c_iflag = R2_SERIAL_DEFAULT_OPTIONS.c_iflag;
        if( NULL == options->c_oflag )
            options->c_oflag = R2_SERIAL_DEFAULT_OPTIONS.c_oflag;
        retval = tcsetattr( self->fd, TCSANOW, options );
    }

    if( -1 == retval ) {
        perror( "tcsetattr()" );
        fprintf( stderr, "trouble setting termios attributes\n" );
        fprintf( stderr, "...attempting to continue\n" );
    }

    return retval;
}

#endif // R2_SERIAL_PORT_I
