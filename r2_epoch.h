// r2_epoch.h
// Simple functions for Unix epoch (time since 1970-01-01 00:00:00.000)

#ifndef R2_EPOCH_H
#define R2_EPOCH_H

#include <inttypes.h> // for int64_t, PRId64
#include <time.h> // for timespec, clock_gettime

struct timespec r2_epoch_timespec_now( void );
int64_t r2_epoch_usec_now( void );
int64_t r2_epoch_msec_now( void );
int64_t r2_epoch_sec_now( void );

// convenience wrapper
int64_t utime( void ) { return r2_epoch_usec_now(); }


// TODO: conversion functions to/from timespec

#endif // R2_EPOCH_H

#ifndef R2_EPOCH_I
#define R2_EPOCH_I
struct timespec r2_epoch_timespec_now( void ){
    struct timespec t;
    clock_gettime( CLOCK_REALTIME, &t );
    return t;
}

int64_t r2_epoch_usec_now( void )
{
    struct timespec t = r2_epoch_timespec_now();
    return (int64_t)( t.tv_sec * 1000000 ) + (int64_t)( t.tv_nsec / 1000 );
}

int64_t r2_epoch_msec_now( void )
{
    struct timespec t = r2_epoch_timespec_now();
    return (int64_t)( t.tv_sec * 1000 ) + (int64_t)( t.tv_nsec / 1000000 );
}

int64_t r2_epoch_sec_now( void )
{
    struct timespec t = r2_epoch_timespec_now();
    return (int64_t)( t.tv_sec );
}

#endif // R2_EPOCH_I
