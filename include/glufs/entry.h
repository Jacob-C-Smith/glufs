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

// Enumeration definitions
enum glufs_entry_type_e
{
    GLUFS_ENTRY_IDENTIFIER = 0,
    GLUFS_ENTRY_FILE       = 1,
    GLUFS_ENTRY_DIRECTORY  = 2,
    GLUFS_ENTRY_QUANTITY   = 3
};

// Forward declarations
struct glufs_entry_s;

// Type definitions
typedef struct glufs_entry_s glufs_entry;

typedef int (*fn_entry_serializer) ( glufs_entry *p_entry , FILE *p_file );
typedef int (*fn_entry_parser)     ( glufs_entry *p_result, FILE *p_file );

// Struct definitions
struct glufs_entry_allocator_s
{
    int x;  
};

struct glufs_entry_s
{
    enum glufs_entry_type_e type;

    union
    {

        struct
        {
            char _name[53+1];
            timestamp ts;
        } identifier;

        struct
        {
            bool deleted;
            timestamp ts;
            unsigned long long  length,
                               _range[2];
            char _name[16349+1];
        } file;

        struct
        {
            bool deleted;
            timestamp ts;
            char _name[16349+1];
        } directory;
    };
};

// Function declarations

// Allocator Factory
/** !
 * Construct a bump allocator for glufs entries.
 * 
 * @param p_
 * 
 * @return 1 on success, 0 on error
 */
int glufs_construct_allocator (  );

// Serializers
/** !
 * Write an entry to a file
 * 
 * @param p_entry the entry
 * @param p_file  the file to write to
 * 
 * @return 1 on success, 0 on error
 */
int glufs_entry_serialize ( glufs_entry *p_entry, FILE *p_file );

/** !
 * Write an identifier entry to a file
 * 
 * @param p_entry the identifier entry
 * @param p_file  the file to write to
 * 
 * @return 1 on success, 0 on error
 */
int glufs_entry_serialize_identifier ( glufs_entry *p_entry, FILE *p_file );

// int glufs_entry_serialize_file ( glufs_entry *p_entry, FILE *p_file );

// int glufs_entry_serialize_directory ( glufs_entry *p_entry, FILE *p_file );
