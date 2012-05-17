#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "simpl.h"

struct SimplStream_t {
    uint8_t *buffer;
    uint8_t *buffer_end;
    uint8_t *buffer_ptr;
    FILE *fp;
    int copied;
    int is_stdin;
};


SimplInStream simpl_istream_from_buffer(const uint8_t *data,
                                        const size_t length,
                                        int copy)
{
    struct SimplStream_t *out = NULL;
    
    if (data && length) {
        out = (struct SimplStream_t *)malloc(sizeof(struct SimplStream_t));
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
    struct SimplStream_t *out = NULL;
    FILE *fp;
    
    if (filename && (fp = fopen(filename, "rb"))) {
        out = (struct SimplStream_t *)malloc(sizeof(struct SimplStream_t));
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
    struct SimplStream_t *out = NULL;
    
    out = (struct SimplStream_t *)malloc(sizeof(struct SimplStream_t));
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
    size_t i, len=0;
    uint8_t *new_buffer;
    
    if (istream->fp) {
        if (istream->buffer) {
            len = (size_t)(istream->buffer_end - istream->buffer_ptr);
            if (len<length) {
                for (i=0; i<len; ++i) {
                    istream->buffer[i] = istream->buffer_ptr[i];
                }
                
                if ((size_t)(istream->buffer_end - istream->buffer) < length) {
                    new_buffer = (uint8_t *)realloc(istream->buffer, sizeof(uint8_t)*length);
                    if (new_buffer) {
                        istream->buffer = istream->buffer_ptr = new_buffer;
                        i = fread(&istream->buffer[len], sizeof(uint8_t), length-len, istream->fp);
                        istream->buffer_end = istream->buffer + (len + i);
                    } else {
                        istream->buffer_ptr = istream->buffer;
                        istream->buffer_end = istream->buffer + len;
                    }
                } else {
                    istream->buffer_ptr = istream->buffer;
                    i = fread(&istream->buffer[len], sizeof(uint8_t), length-len, istream->fp);
                    istream->buffer_end = istream->buffer + (len + i);
                }
            }
        } else {
            istream->buffer = istream->buffer_ptr = (uint8_t *)malloc(sizeof(uint8_t)*length);
            if (istream->buffer) {
                len = fread(istream->buffer, sizeof(uint8_t), length, istream->fp);
                istream->buffer_end = istream->buffer + len;
            } else {
                istream->buffer_ptr = istream->buffer_end = NULL;
            }
        }
    }
    
    len = (size_t)(istream->buffer_end - istream->buffer_ptr);
    for (i=0; i<len && i<length; ++i) {
        data[i] = istream->buffer_ptr[i];
    }
    
    for (;i<length; ++i) data[i] = 0;
    
    return len;
}


void simpl_istream_skip(SimplInStream istream,
                        const size_t offset)
{
    size_t i, len, total = offset;
    
    len = (size_t)(istream->buffer_end - istream->buffer_ptr);
    if (len<total) {
        istream->buffer_ptr += len;
        total -= len;
    } else {
        istream->buffer_ptr += offset;
        return;
    }
    
    if (istream->fp) {
        /* This is stupid, but portable with stdin */
        for (i=0; i<total; ++i) {
            if (fgetc(istream->fp)==EOF) break;
        }
    }
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


SimplOutStream simpl_ostream_to_buffer()
{
    struct SimplStream_t *out = NULL;
    
    out = (struct SimplStream_t *)malloc(sizeof(struct SimplStream_t));
    if (out) {
        out->buffer = out->buffer_end = out->buffer_ptr = NULL;
        out->fp = NULL;
        out->copied = TRUE;
        out->is_stdin = FALSE;
    }
    
    return out;
}


SimplOutStream simpl_ostream_to_file(const char *filename)
{
    struct SimplStream_t *out = NULL;
    FILE *fp;
    
    if (filename && (fp = fopen(filename, "wb"))) {
        out = (struct SimplStream_t *)malloc(sizeof(struct SimplStream_t));
        if (!out) {
            fclose(fp);
            return NULL;
        }
        
        out->buffer = out->buffer_end = out->buffer_ptr = NULL;
        out->fp = fp;
        out->copied = FALSE;
        out->is_stdin = FALSE;
    }
    
    return out;
}


SimplOutStream simpl_ostream_to_stdout()
{
    struct SimplStream_t *out = NULL;
    
    out = (struct SimplStream_t *)malloc(sizeof(struct SimplStream_t));
    if (out) {
        out->buffer = out->buffer_end = out->buffer_ptr = NULL;
        out->fp = stdout;
        out->copied = FALSE;
        out->is_stdin = TRUE;
    }
    
    return out;
}


void simpl_ostream_free(SimplOutStream *ostream)
{
    if (*ostream) {
        if ((*ostream)->buffer) {
            free((void *)(*ostream)->buffer);
        }
        
        if ((*ostream)->fp && !(*ostream)->is_stdin) {
            fclose((*ostream)->fp);
        }
        
        free((void *)(*ostream));
        *ostream = NULL;
    }
}


uint8_t *simpl_ostream_detach(SimplOutStream ostream,
                              size_t *length)
{
    uint8_t *out = NULL;
    
    *length = 0;
    if (ostream->buffer) {
        out = ostream->buffer;
        *length = (size_t)(ostream->buffer_ptr - ostream->buffer);
        ostream->buffer = ostream->buffer_end = ostream->buffer_ptr = NULL;
    }
    
    return out;
}


int simpl_ostream_write(SimplOutStream ostream,
                        const uint8_t *data,
                        const size_t length)
{
    size_t old_size, old_offset, new_size;
    uint8_t *new_buffer;
    
    if (ostream->fp) {
        if (fwrite(data, sizeof(uint8_t), length, ostream->fp)!=length) return FALSE;
    } else {
        if (ostream->buffer) {
            if ((size_t)(ostream->buffer_end - ostream->buffer_ptr) < length) {
                old_size = (size_t)(ostream->buffer_end - ostream->buffer);
                old_offset = (size_t)(ostream->buffer_ptr - ostream->buffer);
                new_size = length + old_size + old_size;
                
                new_buffer = (uint8_t *)realloc(ostream->buffer, sizeof(uint8_t)*new_size);
                if (new_buffer) {
                    ostream->buffer = new_buffer;
                    ostream->buffer_ptr = ostream->buffer + old_offset;
                    ostream->buffer_end = ostream->buffer + new_size;
                } else return FALSE;
            }
            
            memcpy(ostream->buffer_ptr, data, length);
            ostream->buffer_ptr += length;
        } else {
            new_size = length;
            if (new_size < 256) new_size = 256;
            
            ostream->buffer = (uint8_t *)malloc(sizeof(uint8_t) * new_size);
            if (!ostream->buffer) {
                ostream->buffer_ptr = ostream->buffer_end = NULL;
                return FALSE;
            }
            
            memcpy(ostream->buffer, data, length);
            ostream->buffer_ptr = ostream->buffer + length;
            ostream->buffer_end = ostream->buffer + new_size;
        }
    }
    
    return TRUE;
}


int simpl_ostream_write_byte(SimplOutStream ostream,
                             const uint8_t value)
{
    return simpl_ostream_write(ostream, &value, 1);
}


int simpl_ostream_write_le16(SimplOutStream ostream,
                             const uint16_t value)
{
    uint8_t data[2];
    
    data[0] = value & 0xff;
    data[1] = (value >> 8) & 0xff;
    
    return simpl_ostream_write(ostream, data, 2);
}


int simpl_ostream_write_be16(SimplOutStream ostream,
                             const uint16_t value)
{
    uint8_t data[2];
    
    data[0] = (value >> 8) & 0xff;
    data[1] = value & 0xff;
    
    return simpl_ostream_write(ostream, data, 2);
}


int simpl_ostream_write_le32(SimplOutStream ostream,
                             const uint32_t value)
{
    uint8_t data[4];
    
    data[0] = (uint8_t)(value & 0xffL);
    data[1] = (uint8_t)((value >> 8) & 0xffL);
    data[2] = (uint8_t)((value >> 16) & 0xffL);
    data[3] = (uint8_t)((value >> 24) & 0xffL);
    
    return simpl_ostream_write(ostream, data, 4);
}


int simpl_ostream_write_be32(SimplOutStream ostream,
                             const uint32_t value)
{
    uint8_t data[4];
    
    data[0] = (uint8_t)((value >> 24) & 0xffL);
    data[1] = (uint8_t)((value >> 16) & 0xffL);
    data[2] = (uint8_t)((value >> 8) & 0xffL);
    data[3] = (uint8_t)(value & 0xffL);
    
    return simpl_ostream_write(ostream, data, 4);
}


#ifndef NDEBUG

#include <assert.h>

void test_streams(void)
{
    SimplInStream istr;
    SimplOutStream ostr;
    uint8_t *buffer, header[8], header2[16], header3[16];
    size_t buffer_size, i;
    
    printf("\tTesting Streams.\n");
    
    /* Write a set of values to a buffer stream and check if it's invertible. */
    assert(ostr = simpl_ostream_to_buffer());
    
    assert(simpl_ostream_write_byte(ostr, 1));
    assert(simpl_ostream_write_byte(ostr, 2));
    assert(simpl_ostream_write_byte(ostr, 3));
    assert(simpl_ostream_write_le16(ostr, 0x1234));
    assert(simpl_ostream_write_be16(ostr, 0x5678));
    assert(simpl_ostream_write_le32(ostr, 0xdeadbeef));
    assert(simpl_ostream_write_be32(ostr, 0xcafebabe));
    
    assert(buffer = simpl_ostream_detach(ostr, &buffer_size));
    assert(buffer_size==15);
    
    simpl_ostream_free(&ostr);
    assert(ostr==NULL);
    
    assert(istr = simpl_istream_from_buffer(buffer, buffer_size, FALSE));
    assert(simpl_istream_good(istr));
    
    assert(simpl_istream_read_byte(istr)==1);
    assert(simpl_istream_read_byte(istr)==2);
    assert(simpl_istream_read_byte(istr)==3);
    assert(simpl_istream_read_le16(istr)==0x1234);
    assert(simpl_istream_read_be16(istr)==0x5678);
    assert(simpl_istream_read_le32(istr)==0xdeadbeef);
    assert(simpl_istream_read_be32(istr)==0xcafebabe);
    
    assert(!simpl_istream_good(istr));
    simpl_istream_free(&istr);
    assert(istr==NULL);
    
    free((void *)buffer);
    
    /* Now test the same process on a file. */
    assert(ostr = simpl_ostream_to_file("tmp/stream_test.bin"));
    
    assert(simpl_ostream_write_byte(ostr, 1));
    assert(simpl_ostream_write_byte(ostr, 2));
    assert(simpl_ostream_write_byte(ostr, 3));
    assert(simpl_ostream_write_le16(ostr, 0x1234));
    assert(simpl_ostream_write_be16(ostr, 0x5678));
    assert(simpl_ostream_write_le32(ostr, 0xdeadbeef));
    assert(simpl_ostream_write_be32(ostr, 0xcafebabe));
    
    buffer = simpl_ostream_detach(ostr, &buffer_size);
    assert(!buffer);
    assert(!buffer_size);
    
    simpl_ostream_free(&ostr);
    assert(ostr==NULL);
    
    assert(istr = simpl_istream_from_file("tmp/stream_test.bin"));
    assert(simpl_istream_good(istr));
    
    assert(simpl_istream_read_byte(istr)==1);
    assert(simpl_istream_read_byte(istr)==2);
    assert(simpl_istream_read_byte(istr)==3);
    assert(simpl_istream_read_le16(istr)==0x1234);
    assert(simpl_istream_read_be16(istr)==0x5678);
    assert(simpl_istream_read_le32(istr)==0xdeadbeef);
    assert(simpl_istream_read_be32(istr)==0xcafebabe);
    
    /* EOF requires one extra read to trigger in files. */
    assert(simpl_istream_read_byte(istr)==0);
    assert(!simpl_istream_good(istr));
    
    /* Make sure that subsequent reads return 0. */
    assert(simpl_istream_read_le16(istr)==0);
    assert(simpl_istream_read_be16(istr)==0);
    assert(simpl_istream_read_le32(istr)==0);
    assert(simpl_istream_read_be32(istr)==0);
    assert(!simpl_istream_good(istr));
    
    simpl_istream_free(&istr);
    assert(istr==NULL);
    
    /* Now try peeking into the Lena image without moving the pointer. */
    assert(istr = simpl_istream_from_file("images/lena.png"));
    assert(simpl_istream_peek(istr, header, 8)==8);
    assert(simpl_istream_peek(istr, header2, 16)==16);
    assert(simpl_istream_read(istr, header3, 16)==16);
    for (i=0; i<8; ++i) {
        assert(header[i] == header2[i]);
        assert(header[i] == header3[i]);
    }
    for (i=0; i<16; ++i) {
        assert(header2[i] == header3[i]);
    }
    
    /* Try skipping a couple bytes and peek / read. */
    simpl_istream_skip(istr, 2);
    assert(simpl_istream_peek(istr, header2, 16)==16);
    assert(simpl_istream_read(istr, header3, 16)==16);
    for (i=0; i<16; ++i) {
        assert(header2[i] == header3[i]);
    }
    simpl_istream_skip(istr, 4);
    
    /* The following is directly from lena.png. */
    assert(simpl_istream_read_byte(istr)==0x52);
    assert(simpl_istream_read_le16(istr)==0x4247);
    assert(simpl_istream_read_be16(istr)==0x00ae);
    assert(simpl_istream_read_le32(istr)==0x00e91cce);
    assert(simpl_istream_read_be32(istr)==0x073aa149);
    
    simpl_istream_free(&istr);
    assert(istr==NULL);
}

#endif
