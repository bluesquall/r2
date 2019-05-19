#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "r2_epoch.h"

int main( void ){
    printf( "%" PRId64 " microseconds since 1970-01-01 00:00:00\n",
            r2_epoch_usec_now() );
    printf( "%" PRId64 " milliseconds since 1970-01-01 00:00:00\n",
            r2_epoch_msec_now() );
    printf( "%" PRId64 " seconds since 1970-01-01 00:00:00\n",
            r2_epoch_sec_now() );
    exit( EXIT_SUCCESS );
}
