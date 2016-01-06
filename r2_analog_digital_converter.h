// r2_analog_digital_converter.h
//

#ifndef R2_ANALOG_DIGITAL_CONVERTER_H
#define R2_ANALOG_DIGITAL_CONVERTER_H

#include <fcntl.h> // for open, O_RDWR
#include <stdio.h> // for fprintf, stderr

#include <sys/ioctl.h> // to get the number of bytes available on a file descriptor
#include <sys/select.h> // to select on the file descriptor


struct r2_adc {
    int fd;
    int bits;
    int vref;
    double scale;
    double offset;
};

/*
 *
 */
struct r2_adc * r2_adc_create( const char * device, const int bits,
        const double vref, const double scale, const double offset );
/*
 *
 */
void r2_adc_destroy( struct r2_adc * self );

long r2_adc_read( struct r2_adc * self );

double r2_adc_volts( struct r2_adc * self, const long count );

double r2_adc_convert( struct r2_adc * self, const long count );

#endif // R2_ANALOG_DIGITAL_CONVERTER_H


#ifndef R2_ANALOG_DIGITAL_CONVERTER_I
#define R2_ANALOG_DIGITAL_CONVERTER_I

struct r2_adc * r2_adc_create( const char * device, const int bits,
        const double vref, const double scale, const double offset )
{
    struct r2_adc * self = calloc( 1, sizeof( struct r2_adc ) );

    self->fd = open( device, O_RDWR );
    if( -1 == self->fd ) {
        perror( "open()" );
        fprintf( stderr, "could not open device: %s\n", device );
        return NULL;
#ifdef DEBUG
    } else {
        fprintf( stderr, "opened ADC device: %s\nat descriptor: %d\n",
            device, self->fd );
#endif
    }

    // send a newline to start the ADC
    if( 1 != write( self->fd, "\n", 1 ) ){
        fprintf( stderr, "wrote %zub instead of 1b to file descriptor\n" );
        return NULL;
    }

    self->bits = bits;
    self->vref = vref;
    self->scale = scale;
    self->offset = offset;

    return self;
}


void r2_adc_destroy( struct r2_adc * self )
{
    if( self ) {
        close( self->fd );
        free( self );
    }
}


long r2_adc_read( struct r2_adc * self )
{
    char buffer[] = "Waiting...";
    char * end;
    const size_t bufsz = strlen( buffer );
    while( 0 == strcmp( buffer, "Waiting..." ) ) {
        // sleep 100 ms to avoid next "Waiting..."
        nanosleep((const struct timespec[]){{0, 100000L}}, NULL);
        ssize_t bytes_read = read(self->fd, buffer, bufsz);
        if( -1 == bytes_read ) {
            perror( "r2_adc_read read()" );
            return 0;
        }
#ifdef DEBUG
        fprintf( stderr, "read string '%s' from fd %d\n", buffer, self->fd );
#endif // DEBUG
    }
    long count = strtol( buffer, &end, 10 );
#ifdef DEBUG
        fprintf( stderr, "read ADC value %ld from fd %d\n", count, self->fd );
#endif // DEBUG
    if ( errno == ERANGE ) {
        fprintf( stderr, "out of range: '%s'\n", buffer );
        errno = 0;
    } else if( end == buffer ) {
        fprintf( stderr, "could not convert: '%s'\n", buffer );
    }
    return count;
}


double r2_adc_volts( struct r2_adc * self, const long count )
{
    return (self->vref / self->bits) * (double)count;
}

double r2_adc_convert( struct r2_adc * self, const long count )
{
    return self->scale * (double)count + self->offset;
}

#endif // R2_ANALOG_DIGITAL_CONVERTER_I
