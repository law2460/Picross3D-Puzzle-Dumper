//
// p3d_io.c
// Implements the functions in the interface of p3d_io.h
// used for file IO for a Picross3D game ROM
//
// @author Luke Wyland
//
// // // // // // // // // // // // // // // // // // // // // //

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "p3d_io.h"

// magic number used to identify a puzzle file in the game rom
const byte MAGIC_NUMBER[MAGIC_SIZE] = {0x00, 0x10, 0xF0, 0x4B};

// Copies a number of bytes from the rom into a destination buffer. Each
// byte read updates a specified offset counter.
void copy_file_bytes(byte* dest, FILE* rom_file, int length, int* rom_offset) {

    for(int i = 0; i < length; i++) {
        // copy bytes and increment the offset counter for each byte read
        dest[i] = getc(rom_file);
        *rom_offset += 1;
    }

}

// Returns true if a given 16 byte line is a valid puzzle file header
bool is_puzzle_header(byte* header) {

    for(int i = 0; i < MAGIC_SIZE; i++) {
        if(header[i + MAGIC_OFFSET] != MAGIC_NUMBER[i]) {
            // if the magic number is not found, then
            // the given bytes do not contain a puzzle
            // file header
            return false;
        }
    }

    // magic number test passed, this line does contain a puzzle header
    return true;

}

// Puts a number of bytes back into the file stream so that they can
// be read again in the same order they were taken out. Also decrements
// the given offset counter
void put_back(FILE* rom_file, byte* data, int length, int* rom_offset) {

    for(int i = length - 1; i >= 0; i--) {
        // push back data into file stream in reverse order
        // so that it can be read back in the same order it was
        // taken out from and decrement the offset counter
        ungetc(data[i], rom_file);
        *rom_offset -= 1;
    }
}

// Writes a number of bytes to a specified output file
void write_file(FILE* out_file, byte* data, int length) {

    for(int i = 0; i < length; i++) {
        // write the number of bytes to the file
        putc(data[i], out_file);
    }
}
