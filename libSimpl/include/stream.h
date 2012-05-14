/**
 * Header file for functions to abstract memory buffers, stdio FILEs, and
 * stdin/stdout as simple stream-like calls. Someday we might go crazy and
 * add support for networking, memory mapped IO and so on. Implementation of
 * these functions is in "stream.c".
 */
#ifndef SIMPL_STREAM_H_
#define SIMPL_STREAM_H_

#ifdef __cplusplus
extern "C" {
#endif


/**
 * Creates an input stream from a given buffer of data.
 *
 * @param data Pointer to a buffer.
 * @param length The length of the buffer, cannot be zero.
 * @param copy If not zero, the buffer is copied.
 *
 * @return A valid SimplInStream or NULL if failure.
 */
SimplInStream simpl_istream_from_buffer(const uint8_t *data,
                                        const size_t length,
                                        int copy);


/**
 * Creates an input stream from a given file path.
 *
 * @return A valid SimplInStream or NULL if file not found or not readable.
 */
SimplInStream simpl_istream_from_file(const char *filename);


/**
 * Creates an input stream from stdin.
 *
 * @return A valid SimplInStream or NULL if failure.
 */
SimplInStream simpl_istream_from_stdin();


/**
 * Deallocates and closes resources to an input stream setting the stream to
 * NULL.
 *
 * @param istream A double pointer to an input stream.
 */
void simpl_istream_free(SimplInStream *istream);


/**
 * Returns 1 if the input stream is in a readable state, else 0. The stream
 * is not readable if not a valid stream or reaches an End Of File state.
 *
 * @param istream A valid SimplInStream pointer.
 *
 * @return 1 if the input stream is in a readable state, else 0.
 */
int simpl_istream_good(SimplInStream istream);


/**
 * Copies a given number of bytes from the input stream and returns the
 * actual number of bytes copied. The stream point will not be advanced and
 * zeros will be copied where the pointer extends beyond the end of the data.
 *
 * @param istream A valid SimplInStream pointer.
 * @param data The buffer to copy to.
 * @param length The size of the buffer.
 *
 * @return The number of bytes copied. The buffer is in an EOF state when the
 *         returned value is not length.
 */
size_t simpl_istream_peek(SimplInStream istream,
                          uint8_t *data,
                          const size_t length);


/**
 * Moves the stream pointer forward by a given offset. The pointer will not
 * move passed the end of data.
 *
 * @param istream A valid SimplInStream pointer.
 * @param offset The offset to add to the stream pointer.
 */
void simpl_istream_skip(SimplInStream istream,
                        const size_t offset);


/**
 * Copies a given number of bytes from the input stream, advancing the stream
 * pointer, and returning the actual number of bytes copied. Zeros will be
 * copied where the pointer extends beyond the end of the data.
 *
 * @param istream A valid SimplInStream pointer.
 * @param data The buffer to copy to.
 * @param length The size of the buffer.
 *
 * @return The number of bytes copied. The buffer is in an EOF state when the
 *         returned value is not length.
 */
size_t simpl_istream_read(SimplInStream istream,
                          uint8_t *data,
                          const size_t length);


/**
 * Reads a byte value from an input stream.
 *
 * @param istream A valid SimplInStream pointer.
 *
 * @return The read byte value or zero in EOF conditions.
 */
uint8_t simpl_istream_read_byte(SimplInStream istream);


/**
 * Reads a little endian 16 bit value from an input stream.
 *
 * @param istream A valid SimplInStream pointer.
 *
 * @return The read 16-bit value. Zeros will be substituted for values
 *         outside buffer in EOF conditions.
 */
uint16_t simpl_istream_read_le16(SimplInStream istream);


/**
 * Reads a big endian 16 bit value from an input stream.
 *
 * @param istream A valid SimplInStream pointer.
 *
 * @return The read 16-bit value. Zeros will be substituted for values
 *         outside buffer in EOF conditions.
 */
uint16_t simpl_istream_read_be16(SimplInStream istream);


/**
 * Reads a little endian 32 bit value from an input stream.
 *
 * @param istream A valid SimplInStream pointer.
 *
 * @return The read 32-bit value. Zeros will be substituted for values
 *         outside buffer in EOF conditions.
 */
uint32_t simpl_istream_read_le32(SimplInStream istream);


/**
 * Reads a big endian 32 bit value from an input stream.
 *
 * @param istream A valid SimplInStream pointer.
 *
 * @return The read 32-bit value. Zeros will be substituted for values
 *         outside buffer in EOF conditions.
 */
uint32_t simpl_istream_read_be32(SimplInStream istream);


/**
 * Public function for unit testing.
 */
#ifndef NDEBUG
void test_streams(void);
#endif

#ifdef __cplusplus
}
#endif

#endif
