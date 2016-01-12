// r2_epoch.h
// Simple functions for Unix epoch (time since 1970-01-01 00:00:00.000)

#ifndef R2_EPOCH_H
#define R2_EPOCH_H

#include <inttypes.h> // for PRId64
#include <stdint.h> // for int64_t
#include <sys/time.h> // for gettimeofday

int64_t r2_epoch_usec_now( void );
int64_t r2_epoch_msec_now( void );
int64_t r2_epoch_sec_now( void );

// TODO: conversion functions to/from timeval

#endif // R2_EPOCH_H

#ifndef R2_EPOCH_I
#define R2_EPOCH_I
int64_t r2_epoch_usec_now( void )
{
    struct timeval utc;
    gettimeofday(&utc, NULL);
    return (int64_t)utc.tv_sec * 1000000 + (int64_t)utc.tv_usec;
}

int64_t r2_epoch_msec_now( void )
{
    struct timeval utc;
    gettimeofday(&utc, NULL);
    return (int64_t)utc.tv_sec * 1000 + (int64_t)utc.tv_usec % 1000;
}

int64_t r2_epoch_sec_now( void )
{
    struct timeval utc;
    gettimeofday(&utc, NULL);
    return (int64_t)utc.tv_sec;
}

#endif // R2_EPOCH_I
