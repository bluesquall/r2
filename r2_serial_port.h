// r2_serial_port.h
//

#ifndef R2_SERIAL_PORT_H
#define R2_SERIAL_PORT_H

#include <fcntl.h> // for open, O_RDWR, O_NOCTTY, O_NDELAY, O_NONBLOCK
#include <stdio.h> // for fprintf, stderr
#include <termios.h> // for serial port

#include <sys/ioctl.h> // to get the number of bytes available on a serial port
#include <sys/select.h> // to select on the file descriptor for a serial port

#include "r2_buffer.h"


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
    .c_cc[VTIME] = 0
};

/*
 *
 */
struct r2_serial_port * r2_serial_port_create( const char * device,
        const speed_t baud_rate, const size_t buffer_size );
/*
 *
 */
void r2_serial_port_destroy( struct r2_serial_port * self );

/*  Sets the termios options for the serial port.
 *
 *  Any options not explicitly specified are set according to
 *  R2_SERIAL_DEFAULT_OPTIONS.
 */
int r2_serial_port_set_options( struct r2_serial_port * self,
        const struct termios * options );

/*
 *
 */
int r2_serial_port_set_baud_rate( struct r2_serial_port * self,
        speed_t baud_rate );

#endif // R2_SERIAL_PORT_H


#ifndef R2_SERIAL_PORT_I
#define R2_SERIAL_PORT_I

struct r2_serial_port * r2_serial_port_create( const char * device,
        const speed_t baud_rate, const size_t buffer_size )
{
    struct r2_serial_port * self = calloc( 1, sizeof( struct r2_serial_port ) );

    self->fd = open( device, O_RDWR | O_NOCTTY | O_NDELAY | O_NONBLOCK );
    if( -1 == self->fd ) {
        perror( "open()" );
        fprintf( stderr, "could not open device: %s\n", device );
        return NULL;
#ifdef DEBUG
    } else {
        fprintf( stderr, "opened serial device: %s\nat descriptor: %d\n",
            device, self->fd );
#endif
    }

    if( -1 == r2_serial_port_set_options( self, NULL ) ) {
        fprintf(stderr, "could not set termios options\n");
        return NULL;
    }

    if( -1 != baud_rate ) {
        if( -1 == r2_serial_port_set_baud_rate( self, baud_rate ) ) {
            fprintf(stderr, "could not set baudrate\n");
            return NULL;
        }
    }

    self->buffer = r2_buffer_create( buffer_size );
    if( !self->buffer ) {
        fprintf( stderr, "could not create %zub buffer\n", buffer_size );
        return NULL;
#ifdef DEBUG
    } else {
        fprintf( stderr, "created %zub buffer at %p\n", buffer_size,
            self->buffer );
#endif
    }

    return self;
}


void r2_serial_port_destroy( struct r2_serial_port * self )
{
    if( self ) {
        close( self->fd );
        r2_buffer_destroy( self->buffer );
        free( self );
    }
}


int r2_serial_port_set_options( struct r2_serial_port * self,
         const struct termios * options )
{
    int retval = -1;

    if( NULL == options ) {
#ifdef DEBUG
        fprintf(stderr, "setting default serial port options\n" );
#endif
        retval = tcsetattr( self->fd, TCSAFLUSH, &R2_SERIAL_DEFAULT_OPTIONS );
    } else {
#ifdef DEBUG
        fprintf(stderr, "termios options: i=%d, o=%d\n", options->c_iflag,
            options->c_oflag );
#endif
// TODO: Optionally set defaults, then only overwrite specified options.
        retval = tcsetattr( self->fd, TCSAFLUSH, options );
    }

    if( -1 == retval ) {
        perror( "tcsetattr()" );
        fprintf( stderr, "trouble setting termios attributes\n" );
        fprintf( stderr, "...attempting to continue\n" );
    }

    return retval;
}


int r2_serial_port_set_baud_rate( struct r2_serial_port * self,
        speed_t baud_rate )
{
    struct termios options;

    if( -1 == tcgetattr( self->fd, &options ) ) {
        perror("tcgetattr()");
        return -1;
    }
    if( baud_rate != cfgetispeed( &options ) ) {
        if( -1 == cfsetispeed( &options, baud_rate ) ) {
            perror("cfsetispeed()");
            return -1;
        }
    }
    if( baud_rate != cfgetospeed( &options ) ) {
        if( -1 == cfsetospeed( &options, baud_rate ) ) {
            perror("cfsetospeed()");
            return -1;
        }
    }
    if( -1 == tcsetattr( self->fd, TCSAFLUSH, &options ) ) {
        perror("tcsetattr()");
        return -1;
    }
    return 0;
}

#endif // R2_SERIAL_PORT_I
