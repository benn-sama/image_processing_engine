--------------------------------------------------------------------------------------------------------

PNG_SIG (8 BYTE): 89 50 4E 47 0D 0A 1A 0A
EMPTY_BUFFER (4 BYTES): 00 00 00 0D

--------------------------------------------------------------------------------------------------------
// Chunk Layout Template

LENGTH       (4 BYTES) : 00 00 00 00
CHUNK_TYPE   (4 BYTES) : 00 00 00 00
CHUNK_DATA   (OPTIONAL): 00
CRC          (4 BYTES) : 00 00 00 00

--------------------------------------------------------------------------------------------------------
// IHDR

IDHR_LENGTH     (4 BYTES): 49 48 44 52
CHUNK_TYPE      (4 BYTES): 00 00 00 00
WIDTH           (4 BYTES): 00 00 00 00
HEIGHT          (4 BYTES): 00 00 00 00
BIT_DEPTH       (1 BYTE) : 01 || 02 || 04 || 08 || F0
COLOR           (1 BYTE) : 00=PLTE || 01=COLOR || 02=ALPHA
COMPRESS_METHOD (1 BYTE) : 00
FILTER          (1 BYTE) : 00
INTERLACE       (1 BYTE) : 00
CRC             (4 BYTES): 00 00 00 00


--------------------------------------------------------------------------------------------------------

// IDAT
// NOTE: X = IDAT_LENGTH
// NOTE: N = BETWEEN ZLIB && ADLER_CHECKSUM

IDAT_LENGTH (4 BYTES): 00 00 00 00 
IDAT_SIG    (4 BYTES): 49 44 41 54
CHUNK DATA  (N BYTES): X
    ZLIB           (2 BYTES): 00 00
    DATA           (N BYTES): N
    ADLER_CHECKSUM (4 BYTES): 00 00 00 00
CRC         (4 BYTES): 00 00 00 00


--------------------------------------------------------------------------------------------------------

// END OF PNG FILE/ END OF PNG DATASTREAM

IEND (4 BYTES): 49 45 4E 44