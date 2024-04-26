/** !
 * TODO
 * 
 */

// Header file
#include <glufs/entry.h>

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
