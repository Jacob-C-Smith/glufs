/** !
 * glufs metadata file
 * 
 * @file glufs/metadata.h
 * @author Jacob Smith
 */

// Standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// sync
#include <sync/sync.h>

// log
#include <log/log.h>

// Preprocessor definitions
#define glufs_metadata_size sizeof(timestamp) + sizeof(unsigned char) + sizeof(unsigned long long) + sizeof(unsigned long long) + sizeof(unsigned long long) + sizeof(unsigned long long)

// Structure declarations
struct glufs_metadata_s;

// Type definitions
typedef struct glufs_metadata_s glufs_metadata;

// Structure definitions
struct glufs_metadata_s
{
    timestamp ts;
    unsigned char checksum;
    struct
    {
        unsigned short block_size;
        unsigned long long data_blocks,
                           index_bytes,
                           total_blocks;
    } sizes;
};

// Function declarations

// Constructors
/** !
 * Initialize a metadata block
 * 
 * @param p_result   return
 * @param block_size The size of the block IF not zero ELSE 4096
 * 
 * @return 1 on success, 0 on error
 */
int glufs_metadata_init ( glufs_metadata *p_result, unsigned long long block_size );

// Read
/** !
 * Read a metadata block from a file
 * 
 * @param p_result return
 * @param p_file   the file to read from
 * 
 * @return 1 on success, 0 on error
 */
int glufs_metadata_read ( glufs_metadata *p_result, FILE *p_file );

// Write
/** !
 * Write a metadata block to a file
 * 
 * @param p_metadata the metadata
 * @param p_file     the file to write to
 * 
 * @return 1 on success, 0 on error
 */
int glufs_metadata_write ( glufs_metadata *p_metadata, FILE *p_file );

// Log
/** !
 * Print a textual representation of a metadata 
 * block to standard out
 * 
 * @param p_metadata the metadata
 * 
 * @return 1 on success, 0 on error
 */
int glufs_metadata_print ( glufs_metadata *p_metadata );
