/** !
 * glufs header file
 * 
 * @file glufs/entry.h
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

// glufs
#include <glufs/metadata.h>
#include <glufs/entry.h>

// Structure declarations
struct glufs_s;

// Type definitions
typedef struct glufs_s glufs;

// Structure definitions
struct glufs_s
{
    glufs_metadata  _metadata;
    FILE           *p_file;
};

// Function declarations

// Open
/** !
 * Open a glufs volume. If the path parameter 
 * is an existing file, the volume is opened. 
 * Otherwise, an empty volume is created.
 * 
 * @param p_glufs return
 * @param name    the name of the volume
 * @param path    the path to the file
 * 
 * @return 1 on success, 0 on error
 */
int glufs_open ( glufs *p_glufs, const char *name, const char *path );

// Read
//

// Write
/** !
 * Write a glufs volume to the file system
 * 
 * @param p_glufs the glufs volume
 * 
 * @return 1 on success, 0 on error
 */
int glufs_write ( glufs *p_glufs );

// Add
int glufs_file_add ( glufs *p_glufs, const char *path );

// Close
//
