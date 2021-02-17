/**
 * Example kv store
 */
#include <daos.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

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

#define BUFLEN sizeof(val[0])

static void list_keys( daos_handle_t oh, int *num_keys )
{
   char            *buf;
   daos_key_desc_t kds[ ENUM_DESC_NR ];
   daos_anchor_t   anchor = { 0 };
   int             key_nr = 0;
   d_sg_list_t     sgl;
   d_iov_t         sg_iov;

   buf = malloc( ENUM_DESC_BUF );
   d_iov_set( &sg_iov, buf, ENUM_DESC_BUF );
   sgl.sg_nr     = 1;
   sgl.sg_nr_out = 0;
   sgl.sg_iovs   = &sg_iov;

   while ( !daos_anchor_is_eof( &anchor ) )
   {
      uint32_t nr = ENUM_DESC_NR;
      int      rc;

      memset( buf, 0, ENUM_DESC_BUF );
      rc = daos_kv_list( oh, DAOS_TX_NONE, &nr, kds, &sgl, &anchor,\
                         NULL );

      ASSERT( rc == 0, "KV list failed with %d", rc );
            
      printf("keys are : %s and buffere len is %d, iov len is %d\n", (sgl.sg_iovs)->iov_buf, (sgl.sg_iovs)->iov_buf_len, (sgl.sg_iovs)->iov_len);

        //printf("key is of lenght %d, and value type %d\n", \
         kds[0].kd_key_len, kds[0].kd_val_type);

         printf("key is : %s", key);
      // again get it.

      printf("iteration buffer %s\n", anchor.da_buf);
      if ( nr == 0 )
         continue;

      key_nr += nr;
   }

   *num_keys = key_nr;
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

   /** each rank gets NR_ENTRY keys */
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

   int nr_keys = 0;

   list_keys( oh, &nr_keys );
   printf("\nnumber of present keys : %d\n\n", nr_keys);

   printf("Removing key : %s\n\n", key[4]);

   rc = daos_kv_remove( oh, DAOS_TX_NONE, 0, key[4], NULL );
   ASSERT( rc == 0, "KV remove failed with %d", rc );

   list_keys( oh, &nr_keys );
   printf("Number of keys after removal : %d\n\n", nr_keys);

   for ( i = 0; i < NR_ENTRY; i++ )
   {
      daos_size_t size;

      rc = daos_kv_get( oh, DAOS_TX_NONE, 0, key[ i ], &size, NULL, NULL );
      if(size == 0){
         printf("KV get failed for %s\n", key[i]);
         continue;
      }
      rc = daos_kv_get( oh, DAOS_TX_NONE, 0, key[ i ], &size, rbuf, NULL );
      
      printf("Get key is %s : rbuf is : %s\n",key[i], rbuf);
      if(( memcmp( val[ i ], rbuf, BUFLEN ) != 0 )){
         ASSERT( 0, "Data verification" );
      }

      memset( rbuf, 0, BUFLEN );
   }

   daos_kv_close( oh, NULL );

   if ( rank == 0 )
      printf( "\n::SUCCESS::\n\n" );
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
