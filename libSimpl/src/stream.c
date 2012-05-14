#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "simpl.h"

struct SimplInStream_t {
	uint8_t *buffer;
	uint8_t *buffer_end;
	uint8_t *buffer_ptr;
	FILE *fp;
	int copied;
	int is_stdin;
};

struct SimplOutStream_t {
	uint8_t *buffer;
};


SimplInStream simpl_istream_from_buffer(const uint8_t *data,
                                        const size_t length,
                                        int copy)
{
	struct SimplInStream_t *out = NULL;
	
	if (data && length) {
		out = (struct SimplInStream_t *)malloc(sizeof(struct SimplInStream_t));
		if (!out) return NULL;
		
		if (copy) {
			out->buffer = out->buffer_ptr = (uint8_t *)malloc(length * sizeof(uint8_t));
			if (!out->buffer) {
				free((void *)out);
				return NULL;
			}
			
			memcpy(out->buffer, data, length);
			
			out->buffer_end = out->buffer + length;
			out->fp = NULL;
			out->copied = TRUE;
			out->is_stdin = FALSE;
		} else {
			out->buffer = out->buffer_ptr = (uint8_t*)data;
			out->buffer_end = out->buffer + length;
			out->fp = NULL;
			out->copied = FALSE;
			out->is_stdin = FALSE;
		}
	}
	
	return out;
}


SimplInStream simpl_istream_from_file(const char *filename)
{
	struct SimplInStream_t *out = NULL;
	FILE *fp;
	
	if (filename && (fp = fopen(filename, "rb"))) {
		out = (struct SimplInStream_t *)malloc(sizeof(struct SimplInStream_t));
		if (!out) {
			fclose(fp);
			return NULL;
		}
		
		out->fp = fp;
		out->buffer = out->buffer_end = out->buffer_ptr = NULL;
		out->copied = TRUE;
		out->is_stdin = FALSE;
	}
	
	return out;
}


SimplInStream simpl_istream_from_stdin()
{
	struct SimplInStream_t *out = NULL;
	
	out = (struct SimplInStream_t *)malloc(sizeof(struct SimplInStream_t));
	if (out) {
		out->fp = stdin;
		out->buffer = out->buffer_end = out->buffer_ptr = NULL;
		out->copied = TRUE;
		out->is_stdin = TRUE;
	}
	
	return out;
}


void simpl_istream_free(SimplInStream *istream)
{
	if (*istream) {
		if ((*istream)->buffer && (*istream)->copied) {
			free((void *)(*istream)->buffer);
		}
		
		if ((*istream)->fp && !(*istream)->is_stdin) {
			fclose((*istream)->fp);
		}
		
		free((void *)(*istream));
		*istream = NULL;
	}
}


int simpl_istream_good(SimplInStream istream)
{
	if (istream->buffer && istream->buffer_ptr<istream->buffer_end) return TRUE;
	if (istream->fp && !feof(istream->fp)) return TRUE;

	return FALSE;
}


size_t simpl_istream_peek(SimplInStream istream,
                          uint8_t *data,
                          const size_t length)
{
	return 0;
}


void simpl_istream_skip(SimplInStream istream,
                        const size_t offset)
{
	
}


size_t simpl_istream_read(SimplInStream istream,
                          uint8_t *data,
                          const size_t length)
{
	size_t i=0, j;
	
	if (istream->buffer_ptr != istream->buffer_end) {
		for (; i<length && istream->buffer_ptr!=istream->buffer_end; ++i) {
			*data++ = *(istream->buffer_ptr)++;
		}
	}
	
	if (i!=length && istream->fp) {
		data += (j = fread(data, sizeof(uint8_t), length-i, istream->fp));
		i += j;
	}
	
	for (;i<length; ++i) *data++ = 0;
	
	return i;
}


uint8_t simpl_istream_read_byte(SimplInStream istream)
{
	uint8_t buffer;
	
	if (simpl_istream_read(istream, &buffer, 1)==1) {
		return buffer;
	}
	
	return 0;
}


uint16_t simpl_istream_read_le16(SimplInStream istream)
{
	uint8_t buffer[2];
	
	if (simpl_istream_read(istream, buffer, 2)!=0) {
		return ((buffer[1] << 8) | buffer[0]);
	}
	
	return 0;
}


uint16_t simpl_istream_read_be16(SimplInStream istream)
{
	uint8_t buffer[2];
	
	if (simpl_istream_read(istream, buffer, 2)!=0) {
		return ((buffer[0] << 8) | buffer[1]);
	}
	
	return 0;
}


uint32_t simpl_istream_read_le32(SimplInStream istream)
{
	uint8_t buffer[4];
	
	if (simpl_istream_read(istream, buffer, 4)!=0) {
		return (uint32_t)(buffer[3] << 24) | (uint32_t)(buffer[2] << 16) | (uint32_t)(buffer[1] << 8) | (uint32_t)(buffer[0]);
	}
	
	return 0;
}


uint32_t simpl_istream_read_be32(SimplInStream istream)
{
	uint8_t buffer[4];
	
	if (simpl_istream_read(istream, buffer, 4)!=0) {
		return (uint32_t)(buffer[0] << 24) | (uint32_t)(buffer[1] << 16) | (uint32_t)(buffer[2] << 8) | (uint32_t)(buffer[3]);
	}
	
	return 0;
}


#ifndef NDEBUG

#include <assert.h>

void test_streams(void)
{
	printf("\tTesting Streams.\n");
}

#endif
