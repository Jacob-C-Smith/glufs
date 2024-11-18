/** !
 * TODO
 * 
 */

// Header file
#include <glufs/glufs.h>

int glufs_open ( glufs *p_glufs, const char *name, const char *path )
{

    // TODO: Error check
    //

    // Initialized data
    glufs_entry *p_glufs_identifier = (void *) 0;

    // Open the file
    p_glufs->p_file = fopen(path, "w+");

    // Initialize the metadata
    glufs_metadata_init(&p_glufs->_metadata, 4096);

    //p_glufs->p_entries = malloc(8 * sizeof(glufs_entry));

    // Store an identifier entry
    // p_glufs->p_entries[0] = (glufs_entry)
    // {
    //     .type = GLUFS_ENTRY_IDENTIFIER,
    //     .identifier = 
    //     {
    //         .ts = timer_high_precision(),
    //         ._name = { 0 }
    //     }
    // };

    // Copy the name
    // strncpy(p_glufs->p_entries[0].identifier._name, name, 55);

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {

        }
    }
}

int glufs_file_add ( glufs *p_glufs, const char *path )
{

    // Argument check
    if ( p_glufs == (void *) 0 ) goto no_glufs;
    if ( path    == (void *) 0 ) goto no_path;

    // Initialized data
    //
    
    // Success
    return 1;

    // Error handling
    {
        no_glufs:
        no_path:

            // Error
            return 0;
    }
}

int glufs_write ( glufs *p_glufs )
{

    // Argument check
    //

    // Initialized data
    //

    //glufs_metadata_write(&p_glufs->_metadata, p_glufs->p_file);

    //glufs_entry_serialize(&p_glufs->p_entries[0], p_glufs->p_file);

    // Success
    return 1;
}
