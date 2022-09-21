//
// p3d_io.h
// Defines the necessary constants and function interfaces for IO
// operations used in the puzzle dumper application
//
// @author Luke Wyland
//
// // // // // // // // // // // // // // // // // // // // // // // //

#ifndef P3D_IO_H
#define P3D_IO_H

// full file header is 16 bytes
#define HEADER_SIZE 16

// magic number is 4 bytes
#define MAGIC_SIZE 4

// offset of the magic number in the header of the file
#define MAGIC_OFFSET 0x08

// max puzzle file length is 0x600 bytes
#define PUZZLE_SIZE 0x600

// maximum length of data section of puzzle
#define DATA_SIZE PUZZLE_SIZE - HEADER_SIZE

typedef unsigned char byte;

// Copies a number of bytes from the rom into a destination buffer. Each
// byte read updates a specified offset counter.
//
// @param dest          destination buffer to copy bytes to
// @param rom_file      rom file pointer to copy from
// @param length        number of bytes to read
// @param rom_offest    offset counter to increment
void copy_file_bytes(byte* dest, FILE* rom_file, int length, int* rom_offset);

// Returns true if a given 16 byte line is a valid puzzle file header
//
// @param header    16 byte line to check if it's a puzzle file header
//
// @return          true if the header contains the puzzle file magic number
bool is_puzzle_header(byte* header);

// Puts a number of bytes back into the file stream so that they can
// be read again in the same order they were taken out. Also decrements
// the given offset counter
//
// @param rom_file      file stream to put data back into
// @param data          data to push back into stream
// @param length        amount of bytes to push back into stream
// @param rom_offset    offset counter to decrement
void put_back(FILE* rom_file, byte* data, int length, int* rom_offset);

// Writes a number of bytes to a specified output file
//
// @param out_file  output file pointer to write to
// @param data      data to write
// @param length    number of bytes to write to file
void write_file(FILE* out_file, byte* data, int length);

#endif
