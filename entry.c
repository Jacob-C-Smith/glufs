/** !
 * TODO
 * 
 */

// Header file
#include <glufs/entry.h>

typedef int (fn_constructor) ( glufs_entry *p_entry, void *param );
/*
int entry_set_timestamp ( glufs_entry *p_entry, timestamp _timestamp )
{

    // Argument check
    if ( p_entry == (void *) 0 ) goto no_entry;

    // Strategy
    switch ( p_entry->type )
    {
        case GLUFS_ENTRY_IDENTIFIER:

            // Store the timestamp
            p_entry->identifier.ts = _timestamp;

            // Done
            break;
            
        case GLUFS_ENTRY_FILE:
            
            // Store the timestamp
            p_entry->file.ts = _timestamp;

            // Done
            break;

        case GLUFS_ENTRY_DIRECTORY:
            
            // Store the timestamp
            p_entry->directory.ts = _timestamp;

            // Done
            break;
        
        // Default 
        default:
            
            // Error
            return 0;
    }

    // Success
    return 1;

    // Error handling
    {

        // Argument error
        {
            no_entry:
                #ifndef NDEBUG
                    log_error("[glufs] [entry] Null pointer provided for parameter \"p_entry\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

struct
{
    glufs_entry    _entry;
    fn_constructor *_p_chain[32];
}
_entry_prototypes[GLUFS_ENTRY_QUANTITY] = {
    [GLUFS_ENTRY_IDENTIFIER] = 
    {
        ._entry = (glufs_entry)
        {
            .type = GLUFS_ENTRY_IDENTIFIER,
            .identifier = 
            {
                ._name = "GluFS",
                .ts    = 0
            }
        },
        ._p_chain = { entry_set_timestamp, 0 }
    },
    [GLUFS_ENTRY_FILE] = 
    {
        ._entry = (glufs_entry)
        {
            .type = GLUFS_ENTRY_FILE,
            .file = 
            {
                .deleted = false,
                .ts      = 0,
                .length  = 0,
                ._range  =
                {
                    [0] = -1,
                    [1] = -1
                },
                ._name = ""
            }
        },
        ._p_chain = { 0 }
    },
    [GLUFS_ENTRY_DIRECTORY] = 
    {
        ._entry = (glufs_entry)
        {
            .type = GLUFS_ENTRY_DIRECTORY,
            .directory = 
            {   
                .deleted = false,
                .ts      = 0,
                ._name = ""
            }
        },
        ._p_chain = { 0 }
    }
};

// Data
fn_entry_serializer _pfn_serializers[GLUFS_ENTRY_QUANTITY] = 
{
    glufs_entry_serialize_identifier, 0, 0
};

fn_entry_parser _pfn_parsers[GLUFS_ENTRY_QUANTITY] = 
{
    0, 0, 0
};

int glufs_entry_serialize ( glufs_entry *p_entry, FILE *p_file )
{

    // TODO: Argument check
    //

    // Initialized data
    fn_entry_serializer pfn_serializer = _pfn_serializers[p_entry->type];

    // Serialize
    pfn_serializer(p_entry, p_file);

    // Success
    return 1;

    // TODO: Error handling
    {

        // Argument check
        {
            
        }
    }
}

int glufs_entry_serialize_identifier ( glufs_entry *p_entry, FILE *p_file )
{

    // TODO: Argument check
    //

    // Initialized data
    unsigned char _data[64] = { 0 };

    // Store the entry type
    _data[0] = p_entry->type + 1;

    // Store the timestamp
    *((timestamp *)&_data[1]) = p_entry->identifier.ts;

    // Store the name
    strncpy(&_data[9], p_entry->identifier._name, 54);

    // Write the entry to the file 
    fwrite(&_data, 64, 1, p_file);

    // Success
    return 1;

    // TODO: Error handling
    {

        // Argument check
        {
            
        }
    }
}
*/