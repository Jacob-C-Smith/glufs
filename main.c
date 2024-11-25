/** !
 * glufs example program
 * 
 * @file main.c
 * @author Jacob Smith
 */

// Standard library
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// glufs
#include <glufs/glufs.h>

// Entry point
int main ( int argc, const char *argv[] )
{

    // Initialized data
    glufs _glufs = { 0 };

    // Remove the old volume
    remove("resources/image.glufs");

    // Print metadata info
    glufs_metadata_print(&_glufs._metadata);

    // Success
    return EXIT_SUCCESS;

    // Error handlng
    {
        failed_to_initialize_metadata:

            // Print an error message to standard out
            printf("Error: Failed to initialize metadata!\n");

            // Error
            return EXIT_FAILURE;

        failed_to_open_file:

            // Print an error message to standard out
            printf("Error: Failed to open file!\n");

            // Error
            return EXIT_FAILURE;

        failed_to_write_metadata:

            // Print an error message to standard out
            printf("Error: Failed to write metadata!\n");

            // Error
            return EXIT_FAILURE;

        failed_to_read_metadata:

            // Print an error message to standard out
            printf("Error: Failed to read metadata!\n");

            // Error
            return EXIT_FAILURE;
    }
}
