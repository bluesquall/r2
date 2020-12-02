// r2_timerfd.h
// Helper functions for timerfd

#ifndef R2_TIMERFD_H
#define R2_TIMERFD_H

#include <time.h>
#include <sys/timerfd.h>

int r2_timerfd_new( int clock, int flags );

void r2_timerfd_arm( const int fd, const time_t t, const time_t ti );

int r2_timerfd_armed( const int fd );

int64_t r2_timerfd_usec_remaining( const int fd );

#endif // R2_TIMERFD_H

#ifndef R2_TIMERFD_I
#define R2_TIMERFD_I

int r2_timerfd_new( int clock, int flags ) {
    int fd = timerfd_create( clock, flags );
    if( -1 == fd ) {
        perror( "timerfd_create" );
        fprintf( stderr, "failed to create timer\n" );
    }
    return fd;
}


void r2_timerfd_arm( const int fd, const time_t t, const time_t ti ) {
    struct itimerspec its = { { ti, 0 }, { t, 0 } };
    if( -1 == timerfd_settime( fd, 0, &its, NULL ) ) {
        perror( "timerfd_settime" );
        fprintf( stderr, "failed to set timer %d: ", fd );
        fprintf( stderr, "%ld s, interval %ld s\n", t, ti );
        exit( EXIT_FAILURE );
    }
}

int r2_timerfd_armed( const int fd ) {
    struct itimerspec remaining = { { 0, 0 }, { 0, 0 } };
    timerfd_gettime( fd, &remaining );
    if( ( remaining.it_value.tv_nsec  == 0 )
            && ( remaining.it_value.tv_sec == 0 ) ) {
        return 0;
    } else {
        return 1;
    }
}


int64_t r2_timerfd_usec_remaining( const int fd ) {
    struct itimerspec remaining = { { 0, 0 }, { 0, 0 } };
    timerfd_gettime( fd, &remaining );
    if( ( remaining.it_value.tv_sec == 0 )
            &&  ( remaining.it_value.tv_nsec < 1000 ) ) {
        return 0;
    } else {
        return (int64_t)remaining.it_value.tv_sec * 1000000
            + (int64_t)remaining.it_value.tv_nsec / 1000;
    }
}


#endif // R2_TIMERFD_I
