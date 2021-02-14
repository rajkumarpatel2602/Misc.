/**
 * Example test
 *
 * Change POOL/CONT/SVCL defines below.
 * Pool should be created via 'dmg pool create'.
 * e.g.:
 * $ dmg pool create -s=10GB
 * Creating DAOS pool with 10 GB SCM and 0 B NVMe storage (100.00 % ratio)
 * Pool-create command SUCCEEDED: UUID: f8c9f6bd-f716-4955-8b20-fb901a7dee45, Service replicas: 0
 *
 * Returned UUID and SVCL should be populated below.
 * Container should be created via 'daos cont create' against the pool.
 * e.g.:
 * $ daos cont create --pool=f8c9f6bd-f716-4955-8b20-fb901a7dee45 --svc=0
 * Successfully created container 85579c95-f614-45b2-9cfe-6b0a10de71e9
 *
 * To compile the test:
 * $ gcc -o test -L${DAOS_PATH}/lib64 -I${DAOS_PATH}/include -ldaos -lgurt -ldaos_common -luuid ./test.c
 *
 * Then add ${DAOS_PATH}/lib64 to LD_LIBRARY_PATH to run the test case.
 */

#include <daos.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

/** local task information */
static char          node[ 128 ] = "unknown";
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

#define ENUM_DESC_BUF 512
#define ENUM_DESC_NR  5

enum
{
   OBJ_DKEY,
   OBJ_AKEY
};

//char key1[] = "key1";
//char val1[] = "val1";

/** What to write to the object */
#define NR_ENTRY 5

char *key[ NR_ENTRY ] = { "key1", "key2", "key3", "key4", "key5" };
char *val[ NR_ENTRY ] = { "val1", "val2", "val3", "val4", "val5" };

#define BUFLEN sizeof(val[0])

static void dts_buf_render( char *buf, unsigned int buf_len )
{
   int nr = 'z' - 'a' + 1;
   int i;

   for ( i = 0; i < buf_len - 1; i++ )
   {
      int randv = rand( ) % ( 2 * nr );

      if ( randv < nr )
         buf[ i ] = 'a' + randv;
      else
         buf[ i ] = 'A' + ( randv - nr );
   }

   buf[ i ] = '\0';
}

void kv_store_example( )
{
   daos_handle_t oh;
   char          rbuf[ BUFLEN ];
   daos_obj_id_t oid;
   int           i, rc;

   printf( "### KV STORE ###\n" );

   if ( rank == 0 )
      printf( "Example of DAOS High level KV type:\n" );

   /*
    * This is an example if the high level KV API which abstracts out the
    * 2-level keys and exposes a single Key and atomic single value to
    * represent a more traditional KV API. In this example we insert 10
    * keys each with value BUFLEN (note that the value under each key need
    * not to be of the same size.
    */
   oid.hi = 0;
   oid.lo = 4;

   /** the KV API requires the flat feature flag be set in the oid */
   daos_obj_generate_id( &oid, DAOS_OF_KV_FLAT, OC_SX, 0 );

   rc = daos_kv_open( coh, oid, DAOS_OO_RW, &oh, NULL );
   ASSERT( rc == 0, "KV open failed with %d", rc );

   printf( "\nPut %d Keys\n\n", NR_ENTRY );

   /** each rank puts NR_ENTRY keys */
   for ( i = 0; i < NR_ENTRY; i++ )
   {
      rc = daos_kv_put( oh, DAOS_TX_NONE, 0, key[ i ], BUFLEN, val[ i ], NULL );
      ASSERT( rc == 0, "KV put failed with %d", rc );
      printf( "Put key:%s with value:%s\n", key[ i ], val[ i ] );
   }

   printf( "\nGet %d Keys\n\n", NR_ENTRY );

   /** each rank gets 10 keys */
   for ( i = 0; i < NR_ENTRY; i++ )
   {
      daos_size_t size;

      /** first query the size */
      rc = daos_kv_get( oh, DAOS_TX_NONE, 0, key[ i ], &size, NULL, NULL );
      ASSERT( rc == 0, "KV get failed with %d", rc );
      ASSERT( size == BUFLEN, "Invalid read size" );

      rc = daos_kv_get( oh, DAOS_TX_NONE, 0, key[ i ], &size, rbuf, NULL );
      ASSERT( rc == 0, "KV get failed with %d", rc );
      ASSERT( size == BUFLEN, "Invalid read size" );

      if ( memcmp( val[ i ], rbuf, BUFLEN ) != 0 )
         ASSERT( 0, "Data verification" );

      printf( "Get key:%s with value:%s\n", key[ i ], rbuf );

      memset( rbuf, 0, BUFLEN );
   }

   int num_keys = 0;

   /** enumerate all keys */
   //list_keys( oh, &num_keys );
   //ASSERT( num_keys == KEYS * rankn, "KV enumerate failed" );

   /** each rank removes a key */

   /*rc = daos_kv_remove( oh, DAOS_TX_NONE, 0, key1, NULL );
      ASSERT( rc == 0, "KV remove failed with %d", rc );*/

   /** enumerate all keys */

/*    list_keys( oh, &num_keys );
   ASSERT( num_keys == ( KEYS - 1 ) * rankn,
           "KV enumerate after remove failed" ); */

   daos_kv_close( oh, NULL );

   if ( rank == 0 )
      printf( "SUCCESS\n" );
}

int main( int argc, char **argv )
{
   uuid_t pool_uuid, co_uuid;
   int    rc;

   if ( argc != 2 )
   {
      fprintf( stderr, "args: pool\n" );
      exit( 1 );
   }

   /** initialize DAOS by connecting to local agent */
   printf( "Initializing DAOS\t" );
   rc = daos_init( );
   ASSERT( rc == 0, "daos_init failed with %d", rc );

   rc = uuid_parse( argv[ 1 ], pool_uuid );
   ASSERT( rc == 0, "Failed to parse 'Pool uuid': %s", argv[ 1 ] );

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
   //handle_share( &coh, HANDLE_CO );

   /** Example of DAOS KV object */
   kv_store_example( );

   rc = daos_cont_close( coh, NULL );
   ASSERT( rc == 0, "cont close failed" );

   rc = daos_pool_disconnect( poh, NULL );
   ASSERT( rc == 0, "disconnect failed" );

   /** teardown the DAOS stack */
   printf( "Tearing down DAOS\t" );
   rc = daos_fini( );
   ASSERT( rc == 0, "daos_fini failed with %d", rc );

   return rc;
}
