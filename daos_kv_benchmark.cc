/**
 * Example kv store
 */
#include <cstdio>
#include <benchmark/benchmark.h>

#include <daos.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define BM_1K_BUF   1024
#define BM_4K_BUF   (1024 * 4)
#define BM_8K_BUF   (1024 * 8)
#define BM_16K_BUF  (1024 * 16)
#define BM_32K_BUF  (1024 * 32)

char val_1k[BM_1K_BUF]={0};
char val_4k[BM_4K_BUF]={0};
char val_8k[BM_8K_BUF]={0};
char val_16k[BM_16K_BUF]={0};
char val_32k[BM_32K_BUF]={0};

static char          node[ 128 ] = "new_node";
static daos_handle_t poh;
static daos_handle_t coh;
static int           rank, rankn;
#define FAIL( fmt, ... )                                          \
    do {                                                            \
        fprintf(stderr, "Process (%s): " fmt " aborting\n",     \
                node, ## __VA_ARGS__);                          \
        exit(1);                                                \
    } while (0)

#define ASSERT( cond, ... )                                       \
    do {                                                            \
        if (!(cond))                                            \
        FAIL(__VA_ARGS__);                              \
    } while (0)

enum
{
    OBJ_DKEY,
    OBJ_AKEY
};

#define ENUM_DESC_BUF 512
#define ENUM_DESC_NR  3


/** What to write to the object */
#define NR_ENTRY 7

char *key[ NR_ENTRY ] = { "key1111", "key2", "key3", "key4", "key5", "key6", "key7"};
char *val[ NR_ENTRY ] = { "val1", "val2", "val3", "val4", "val5", "val6", "val7"};

#define BUFLEN 100

daos_handle_t oh;
char          rbuf[ BUFLEN ];
daos_obj_id_t oid;
int           i, rc;

uuid_t pool_uuid, co_uuid;

int setup_main( )
{

    /** initialize DAOS by connecting to local agent */
    rc = daos_init( );
    ASSERT( rc == 0, "daos_init failed with %d", rc );

    rc = uuid_parse("6ecfe2d0-541b-4c9d-81f2-d97a3da6089c", pool_uuid );

    /** Call connect on rank 0 only and broadcast handle to others */
    if ( rank == 0 ) //TODO define rank
    {
        rc = daos_pool_connect( pool_uuid, NULL, DAOS_PC_RW, &poh,
                NULL, NULL );
        ASSERT( rc == 0, "pool connect failed with %d", rc );
    }

    if ( rank == 0 )
    {
        /** generate uuid for container */
        uuid_generate( co_uuid );

        /** create container */
        rc = daos_cont_create( poh, co_uuid, NULL /* properties */,
                NULL /* event */ );
        ASSERT( rc == 0, "container create failed with %d", rc );

        /** open container */
        rc = daos_cont_open( poh, co_uuid, DAOS_COO_RW, &coh, NULL,
                NULL );
        ASSERT( rc == 0, "container open failed with %d", rc );
    }

    /** share container handle with peer tasks */
    //printf( "### KV STORE ###\n" );

    if ( rank == 0 )
        //printf( "Example of DAOS High level KV type:\n" );

    oid.hi = 0;
    oid.lo = 4;

    /** the KV API requires the flat feature flag be set in the oid */
    daos_obj_generate_id( &oid, DAOS_OF_KV_FLAT, OC_SX, 0 );

    rc = daos_kv_open( coh, oid, DAOS_OO_RW, &oh, NULL );
    ASSERT( rc == 0, "KV open failed with %d", rc );


}
void tear_down(){

    rc = daos_cont_close( coh, NULL );
    ASSERT( rc == 0, "cont close failed" );

    rc = daos_pool_disconnect( poh, NULL );
    ASSERT( rc == 0, "disconnect failed" );

    /** teardown the DAOS stack */
    rc = daos_fini( );
    ASSERT( rc == 0, "daos_fini failed with %d", rc );

}

// bnechmark function is getting called here.
static void BM_SomeFunction(benchmark::State& state) {
    setup_main();
    char *key = (char*) malloc(sizeof(state.range(0)));//key generation here
    memset(key,'x', state.range(0));
    key[state.range(0)-1]=0;
    printf("key size is %d and iterations are : %d and key : %s\n", state.range(0), state.range(1), key);
    for (auto _ : state) {

        for (int i=0;i < state.range(1); i++){

            state.PauseTiming();
            
            state.ResumeTiming();
            daos_kv_put( oh, DAOS_TX_NONE, 0, key, sizeof("test_val"), "test_val", NULL );
        }
    }
    free(key);
    //daos_size_t size = 0;
    //daos_kv_get( oh, DAOS_TX_NONE, 0, "test_key", &size, NULL, NULL );
    //daos_kv_get( oh, DAOS_TX_NONE, 0, "test_key", &size, rbuf, NULL );
    //printf("=== get value is : %s ====", rbuf);
    tear_down();
}

// Register the function as a benchmark
BENCHMARK(BM_SomeFunction)
    ->Args({64,100})
    ->Args({64,1000})
    ->Unit(benchmark::kMillisecond);
// // Run the benchmark
BENCHMARK_MAIN();


