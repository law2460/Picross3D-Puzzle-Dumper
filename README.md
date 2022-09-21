
# Picross3D-Puzzle-Dumper
Dumps puzzle files from a Picross3D game ROM.
Usage: `./puzzle_dumper [ROM file]`

Below is the file format of Picross3D puzzle files, all of which I've reverse-engineered.

    offset | size | description
    -------------------------
    0x000  |    1 | 0x20 for USA/EUR roms, 0x14 for the Japanese ROM
    0x001  |    1 | Collection ID
    0x002  |    1 | Number of strikes allowed
    0x003  |    1 | Music track ID
    0x004  |    1 | Green time limit (in minutes)
    0x005  |    1 | Yellow time limit (in minutes)
    0x006  |    2 | Puzzle size:
    				- width is first byte & 0x0F
    				- depth is (second byte & 0x3C) rsh 2
    				- height is (first byte & 0xE0) rsh 5
    0x008  |    4 | Magic number identifier, always [00 10 F0 4B]
    0x00C  |    1 | x rotation of finished puzzle, as a fraction of 255 * 360
    0x00D  |    1 | y rotation of finished puzzle, as a fraction of 255 * 360
    0x00E  |    1 | Set to 0x01 if in a different format (not yet documented)
    0x00F  |    1 | Slider flag, if this byte + 0x01 is divisible by 4, sliders are disabled
    0x010  |   32 | Internal name, either in ASCII or shift-JIS encoding
	    			- note: ASCII encodings are padded with an extra byte between characters
    0x030  |  104 | Color data (15 bit color, each color has 2 shades, using 4 bytes each):
    				- red is first byte & 0x1F
    				- green is ((second byte & 0x03) lsh 3) + ((first byte & 0xE0) rsh 5)
    				- blue is second byte rsh 2
	    			- Every color seems to be followed by a darker version of that color for shading purposes
    0x098  |   48 | English name, padded ASCII encoded
    0x0C8  |   48 | French name, padded ASCII encoded
    0x0F8  |   48 | German name, padded ASCII encoded
    0x128  |   48 | Italian name, padded ASCII encoded
    0x158  |   48 | Spanish name, padded ASCII encoded
    0x188  | 1000 | Puzzle data, one byte for each block starting from
    			the back bottom left corner going first in rows towards the screen,
    			rows stacking upward to complete a slice, slices going left to right.
    			Empty blocks are 0x00, blocks meant to be destroyed are 0x20, and blocks
    			part of the puzzle are >= 0x30. The color index for each block is given by
    			the block's byte - 0x30. The color index is the offset into the color data above
    			for the given block.
    0x570  |   60 | Clue mask:
    				Left side face goes in rows, bottom to top starting block in each row is back going forwards.
    				Top side face goes in columns, left to right starting block in each row is back going to forwards.
    				Front side face goes in columns, left to right starting block in each row is top going to bottom.
    				Each row/column takes up 2 bytes, the first byte masks the starting block until the 8th block
    				(one bit for each block, the least significant bit goes to the starting block), and
    				the second byte only masks the remaining 2 blocks. The a clue is masked if that block's corresponding
    				bit is a 1 and shown if it is 0.
    0x5AC  |   84 | padding, all 0x00  

Using this documentation and the puzzle dumper tool, I've been able to re-build puzzle solutions in the Unity game engine:

![Steam engine, with screenshot comparison](https://lh3.googleusercontent.com/pw/AL9nZEVQ2MpOrkRRhBjg71zjkjhud8HEBh22FxIsvwvNJz6FWn8EuWtHompFkFFtmhtLrx6JvO8EHH1kyPucXmpOV7iBws9zvj_BfcbNRPRAubEmA8YrdBsmYW_kdLVUCkE5p3Fqqdq5YYOXSfQ9H198Ps0=w1189-h840-no)

On the left is a screenshot from the game and on the right is a rebuild of the solution from the dumped puzzle data file only.
 

