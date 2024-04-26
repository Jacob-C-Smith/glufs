/** !
 * TODO
 * 
 * 
 */

// Header file
#include <glufs/metadata.h>

int glufs_metadata_init ( glufs_metadata *p_result, unsigned long long block_size )
{

    // Argument check
    if ( p_result == (void *) 0 ) goto no_result;

    // Store the result
    *p_result = (glufs_metadata)
    {
        .ts       = 0, // TODO
        .checksum = 0, // TODO
        .sizes =
        {
            .block_size   = block_size,
            .data_blocks  = 0,
            .index_bytes  = 0,
            .total_blocks = 1
        }
    };

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_result:
                #ifndef NDEBUG
                    log_error("[glufs] Null pointer provided for \"p_result\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int glufs_metadata_read ( glufs_metadata *p_result, FILE *p_file )
{

    // Argument check
    if ( p_result == (void *) 0 ) goto no_result;
    if ( p_file   == (void *) 0 ) goto no_file;

    // Initialized data
    unsigned char _metadata[glufs_metadata_size] = { 0 };

    // Read the metadata
    fread(&_metadata, glufs_metadata_size, 1, p_file);

    // Store the result
    *p_result = (glufs_metadata)
    {
        .ts       = *((timestamp *)&_metadata[0]),
        .checksum = *((unsigned char *)&_metadata[8]), 
        .sizes =
        {
            .block_size   = *((unsigned short *)&_metadata[9]),
            .data_blocks  = *((unsigned long long *)&_metadata[17]),
            .index_bytes  = *((unsigned long long *)&_metadata[25]),
            .total_blocks = *((unsigned long long *)&_metadata[33])
        }
    };

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_result:
                #ifndef NDEBUG
                    log_error("[glufs] Null pointer provided for \"p_result\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
            
            no_file:
                #ifndef NDEBUG
                    log_error("[glufs] Null pointer provided for \"p_file\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int glufs_metadata_write ( glufs_metadata *p_metadata, FILE *p_file )
{

    // Argument check
    if ( p_metadata == (void *) 0 ) goto no_metadata;
    if ( p_file     == (void *) 0 ) goto no_file;

    // Initialized data
    unsigned char _metadata[glufs_metadata_size] = { 0 };

    // Store the timestamp
    *((timestamp *)&_metadata[0]) = p_metadata->ts,

    // Store the checksum
    *((unsigned char *)&_metadata[8]) = p_metadata->checksum,

    // Store the block size
    *((unsigned short *)&_metadata[9]) = p_metadata->sizes.block_size,

    // Store the quantity of data blocks
    *((unsigned long long *)&_metadata[17]) = p_metadata->sizes.data_blocks,

    // Store the size of the index
    *((unsigned long long *)&_metadata[25]) = p_metadata->sizes.index_bytes,

    // Store the total quantity of blocks
    *((unsigned long long *)&_metadata[33]) = p_metadata->sizes.total_blocks;

    // Write the metadata to the file
    fwrite(&_metadata, sizeof(glufs_metadata), 1, p_file);

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_metadata:
                #ifndef NDEBUG
                    log_error("[glufs] Null pointer provided for \"p_metadata\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
            
            no_file:
                #ifndef NDEBUG
                    log_error("[glufs] Null pointer provided for \"p_file\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int glufs_metadata_print ( glufs_metadata *p_metadata )
{

    // TODO: Argument check
    //

    // Print the metadata
    printf(
        " --- Metadata ---\n"\
        " timestamp   : %llu\n"\
        " checksum    : %hhu\n"\
        " block size  : %hu\n"\
        " data blocks : %llu\n"\
        " index bytes : %llu\n"\
        " total blocks: %llu\n",
        p_metadata->ts,
        p_metadata->checksum,
        p_metadata->sizes.block_size,
        p_metadata->sizes.data_blocks,
        p_metadata->sizes.index_bytes,
        p_metadata->sizes.total_blocks
    );

    // Success
    return 1;
}
