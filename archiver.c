/** !
 * glufs image constructor
 * 
 * @file main.c
 * 
 * @author Jacob Smith
 */

// Standard library
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// POSIX
#include <dirent.h>

// tree
#include <tree/tree.h>
#include <tree/binary.h>

// type definitions
typedef struct
{
    char   _name[FILENAME_MAX];
    size_t sz;
    size_t osz;
    size_t off;
} glu_file;

glu_file *p_files [512] = { 0 };
int counter = 0;
size_t acc = 512;
char p_path[FILENAME_MAX] = { 0 };
char p_output[FILENAME_MAX] = { 0 };
FILE *p_f = 0;
bool preserve_data  = false,
     preserve_index = false;

// Forward declarations
/** !
 * Print a usage message to standard out
 * 
 * @param argv0 the name of the program
 * 
 * @return void
 */
void print_usage ( const char *argv0 );

/** !
 * Parse command line arguments
 * 
 * @param argc            the argc parameter of the entry point
 * @param argv            the argv parameter of the entry point
 * @param examples_to_run return
 * 
 * @return void on success, program abort on failure
 */
void parse_command_line_arguments ( int argc, const char *argv[] );

int filter ( const struct dirent *p_dir )
{

    // No '.' 
    if (p_dir->d_name[0]=='.' && p_dir->d_name[1]=='\0') return 0;

    // No '..' 
    if (p_dir->d_name[0]=='.' && p_dir->d_name[1]=='.' && p_dir->d_name[2]=='\0') return 0;

    // Okay
    return 1;
}

size_t load_file ( const char *path, void *buffer, bool binary_mode )
{

    // Argument check
    if ( path == 0 ) goto no_path;

    // Initialized data
    size_t  ret = 0;
    FILE   *f   = fopen(path, (binary_mode) ? "rb" : "r");
    
    // Check if file is valid
    if ( f == NULL ) goto invalid_file;

    // Find file size and prep for read
    fseek(f, 0, SEEK_END);
    ret = ftell(f);
    fseek(f, 0, SEEK_SET);
    
    // Read to data
    if ( buffer ) ret = fread(buffer, 1, ret, f);

    // The file is no longer needed
    fclose(f);
    
    // Success
    return ret;

    // Error handling
    {

        // Argument errors
        {
            no_path:
                #ifndef NDEBUG
                    log_error("Null pointer provided for parameter \"path\" in call to function \"%s\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // File errors
        {
            invalid_file:

                // Error
                return 0;
        }
    }
}

int print_directory_structure( char _path[FILENAME_MAX] )
{

    // Initialized data
    struct dirent **p_directories = 0;
    int count = scandir(_path, &p_directories, filter, 0);

    // Iterate through each directory entry
    for (size_t i = 0; i < count; i++)
    {
        
        // If this entry is a symlink
        if ( p_directories[i]->d_type == DT_LNK )
            continue;
            
        // If this entry is a directory ...
        else if ( p_directories[i]->d_type == DT_DIR )
        {
            
            // Initialized data
            char _path_prime[FILENAME_MAX] = { 0 };

            // Construct the path
            sprintf(_path_prime, "%s/%s", _path, p_directories[i]->d_name);

            // Recurse
            print_directory_structure(_path_prime);
        }

        // If this entry is an ordinary file
        else if ( p_directories[i]->d_type == DT_REG )
        {
            
            // Allocate memory for a file
            p_files[counter] = malloc(sizeof(glu_file));

            // Construct the path
            sprintf(p_files[counter]->_name, "%s/%s", _path, p_directories[i]->d_name);

            // Increment the counter
            counter++;
        }  
    }

    // Success
    return 1;
}

const void *const key_acc_node ( glu_file *p_value )
{
    
    // Success
    return &p_value->_name;
}

int print_node ( glu_file *p_glu_file )
{
    
    // Argument check
    if (p_glu_file == (void *) 0 ) return 1;
    
    // Initialized data
    FILE *inf = (void *) 0;

    // Store the offset 
    p_glu_file->off = acc;

    // Get the size of the file
    p_glu_file->osz = load_file(p_glu_file->_name, 0, true);
    
    // Store the size of the file, plus slack
    if (p_glu_file->osz % 512)
        p_glu_file->sz = 512 - (p_glu_file->osz % 512) + p_glu_file->osz;
    else
        p_glu_file->sz = p_glu_file->osz;
        
    // Update the offset
    acc += p_glu_file->sz;

    // Open the file for reading
    inf = fopen(p_glu_file->_name, "rb");
    
    // Append the file to the data block
    while (!feof(inf)) putc(getc(inf), p_f);
    
    // Pad the file upto 512 bytes
    for (size_t i = 512 - (p_glu_file->osz % 512); i != 1; i--) putc('\0', p_f);
    
    // Success
    return 1;
}

int comparator_node ( glu_file **pp_a, glu_file **pp_b )
{
    
    // Initialized data
    glu_file *p_a = *pp_a, *p_b = *pp_b;

    // Success
    return strcmp(p_a->_name, p_b->_name);
}

int serializer_node ( FILE *p_file, binary_tree_node *p_binary_tree_node )
{

    // Argument check
    if (((glu_file *)p_binary_tree_node->p_value) == (void *) 0 ) return 1;

    // Initialized data
    glu_file *p_glu_file = ((glu_file *)p_binary_tree_node->p_value);
    size_t    file_path_length = strlen(p_glu_file->_name);
    const char *name = (&p_glu_file->_name[1]);

    // Write the offset
    fwrite(&p_glu_file->off, sizeof(size_t), 1, p_file);

    // Write the size
    fwrite(&p_glu_file->sz, sizeof(size_t), 1, p_file);

    // Write the path
    fwrite(name, file_path_length, 1, p_file);
    
    // Pad the tree entry
    for (size_t i = (224 - file_path_length); i != 0; i--) putc('\0', p_file);
    
    // Success
    return 1;
}

// Entry point
int main ( int argc, const char *argv[] )
{

    // Initialized data
    binary_tree *p_binary_tree = (void *) 0;
    char _init_wd[FILENAME_MAX] = { 0 };
    char _system_command_buffer[FILENAME_MAX + 128] = { 0 };

    // Parse command line arguments
    parse_command_line_arguments(argc, argv);

    // Open the data block
    p_f = fopen("glufs.data", "w");
    
    // Push the current working directory
    getcwd(_init_wd, FILENAME_MAX);

    // Update the current working directory
    chdir(p_path);

    // Construct the directory structure
    print_directory_structure(".");

    // Sort the directory structure
    qsort(&p_files, counter, sizeof(void *), (int (*)(const void *, const void *))comparator_node);

    // Construct a binary tree from the directory structure
    binary_tree_construct_balanced
    (
        &p_binary_tree,
        (void **) &p_files,
        counter - 1,
        (fn_tree_equal*)comparator_node,
        (fn_tree_key_accessor *)key_acc_node,
        240
    );

    // Make space at the beginning of the file
    fseek(p_f, acc, SEEK_SET);
    
    // Build the index
    binary_tree_traverse_inorder(p_binary_tree, (fn_binary_tree_traverse *)print_node);

    // Update the current working directory
    chdir(_init_wd);

    // Write the index to a file
    binary_tree_serialize(p_binary_tree, "glufs.index", serializer_node);

    if ( p_output )
        
        // Build the cat command
        snprintf(_system_command_buffer, sizeof(_system_command_buffer), "cat glufs.data glufs.index > %s", p_output),

        // Concatenate the data block to the index; Store output
        system(_system_command_buffer);

    // Clean up
    if ( preserve_data  == false ) remove("glufs.data");
    if ( preserve_index == false ) remove("glufs.index");

    // Success
    return EXIT_SUCCESS;

    // Error handling
    {

    }
}

void print_usage ( const char *argv0 )
{

    // Argument check
    if ( argv0 == (void *) 0 ) exit(EXIT_FAILURE);

    // Print a usage message to standard out
    printf("Usage: %s [--input <path/to/dir>] [--output <output.glufs>] [--offset <N>] [--preserve-data] [--preserve-index]\n", argv0);
    printf("       --input  <path/to/dir>  : Path to the directory to archive\n");
    printf("       --output <output.glufs> : Path to the output file\n");
    printf("       --offset <N>            : Quantity of null bytes to prepend to image\n");
    printf("       --preserve-data         : Preserve the data block file\n");
    printf("       --preserve-index        : Preserve the index block file\n");

    // Done
    return;
}

void parse_command_line_arguments ( int argc, const char *argv[] )
{

    // If no command line arguments are supplied 
    //if ( argc < 2 ) goto invalid_arguments;

    // Iterate through each command line argument
    for (size_t i = 1; i < argc; i++)
    {
        
        // Preserve data
        if ( strcmp(argv[i], "--preserve-data") == 0 )
            
            // Set the preserve data flag
            preserve_data = true;

        // Preserve index
        else if ( strcmp(argv[i], "--preserve-index") == 0 )
            
            // Set the preserve index flag
            preserve_index = true;
        
        // Input
        else if ( strcmp(argv[i], "--input") == 0 )

            // Copy the input directory
            strncpy(p_path, argv[i + 1], FILENAME_MAX), i++;
        
        // Input
        else if ( strcmp(argv[i], "--output") == 0 )

            // Copy the input directory
            strncpy(p_output, argv[i + 1], FILENAME_MAX), i++;

        // Input
        else if ( strcmp(argv[i], "--offset") == 0 )
        {
            if ( 0 == sscanf(argv[i + 1], " %zd", &acc) ) 
                goto invalid_arguments;
        }

        // Default
        else goto invalid_arguments;
    }

    // Error check
    //if ( 0 == strlen(p_output) ) goto invalid_arguments;
    if ( 0 == strlen(p_path) ) goto invalid_arguments;
    
    // Success
    return;

    // Error handling
    {

        // Argument errors
        {
            invalid_arguments:
                
                // Print a usage message to standard out
                print_usage(argv[0]);

                // Abort
                exit(EXIT_FAILURE);
        }
    }
}