//
// Picross3D Puzzle Dumper
// Dumps Picross3D Puzzles from a game ROM into files
// 
// @author Luke Wyland
//
// // // // // // // // // // // // // // // // // // // //

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "p3d_io.h"

// Dumps the contents of a Picross3D game ROM into individual puzzle files.
// Each file will named PUZ_###.bin, with ### starting from 001 and counting upwards.
//
// @param argc  argument count
// @param argv  argument data
//
// @return      EXIT_FAILURE if no ROM file specified, EXIT_SUCESS when contents are dumped
int main(int argc, char** argv) {

    if(argc < 2) {
        // no ROM file specified in the arguments
        printf("Usage: puzzle_dump [ROM file]\n");
        return EXIT_FAILURE;
    }

    // get the ROM file name and open it
    char* rom_file_name = argv[1];
    FILE* rom_file = fopen(rom_file_name, "rb");

    if(rom_file == NULL) {
        // ROM file could not open
        printf("Could not open ROM file.\n");
        return EXIT_FAILURE;
    }

    // puzzle files found within ROM count
    int file_count = 0;

    // ROM bytes read offset counter, used to mark the location of files
    // within the ROM
    int rom_offset = 0;

    while(!feof(rom_file)) {

        // allocate memory for the puzzle header and data section of a puzzle file
        byte* header = malloc(HEADER_SIZE);
        byte* puzzle_data = malloc(DATA_SIZE);

        // copy 16 bytes into the header buffer and check if it contains the data
        // of a valid puzzle file header
        copy_file_bytes(header, rom_file, HEADER_SIZE, &rom_offset);
        bool puzzle_found = is_puzzle_header(header);

        // number of bytes copied into the puzzle data buffer, some puzzle files
        // are different lengths than others
        int bytes_copied;

        if(puzzle_found) {
            // a puzzle file has been found, header check passed

            bytes_copied = 0;
            while(bytes_copied < DATA_SIZE) {
                // copy data until the max data size has been reached 16 bytes at a time.
                // this is to check to make sure that if another puzzle file header was read in
                // while trying to read the puzzle data, it can terminate
                byte* copy_location = puzzle_data + bytes_copied;
                copy_file_bytes(copy_location, rom_file, HEADER_SIZE, &rom_offset);

                if(is_puzzle_header(copy_location)) {
                    // if the current 16 byte copy location contains a header,
                    // push that header back into the file stream and stop
                    // copying bytes into the puzzle data buffer
                    put_back(rom_file, copy_location, HEADER_SIZE, &rom_offset);
                    break;
                }

                // increment the number of bytes copied into the puzzle data buffer
                bytes_copied += HEADER_SIZE;
            }
        
            // calculate the offset that the magic number was found in the ROM file
            int magic_in_file_offset = rom_offset - bytes_copied - MAGIC_OFFSET;

            // calculate the size of the total number of bytes in the full puzzle file
            int total_file_length = HEADER_SIZE + bytes_copied;
           
            // increment the file count and create the puzzle file name
            file_count++;
            char file_out_name[15];
            sprintf(file_out_name, "PUZ_%03d.bin", file_count);

            // print the size and location of the found puzzle file
            printf("%d byte puzzle file found in ROM at 0x%08X. Dumping as %s\n", total_file_length, magic_in_file_offset, file_out_name);
            
            // write the header first and then the puzzle data to the output puzzle file
            FILE* out_file = fopen(file_out_name, "wb+");
            write_file(out_file, header, HEADER_SIZE);
            write_file(out_file, puzzle_data, bytes_copied);
            fclose(out_file);
        }

        free(puzzle_data);
        free(header);
        
    }
    fclose(rom_file);

    // print total file found count
    printf("%d files found.\n", file_count);

    return EXIT_SUCCESS;

}
