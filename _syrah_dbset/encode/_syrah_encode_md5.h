#ifndef _SYRAH_ENCODE_MD5_H
#define _SYRAH_ENCODE_MD5_H

//C++ Standard Library
#include <string>

/* Type define */
typedef unsigned char byte;
typedef unsigned int uint32;

using std::string;

/* MD5 declaration. */
class _syrah_encode_md5 {
public:
    _syrah_encode_md5();
    _syrah_encode_md5(const void* input, size_t length);
    _syrah_encode_md5(const string& str);
    void update(const void* input, size_t length);
    void update(const string& str);
    const byte* digest();
    string toString();
    void reset();

private:
    void update(const byte* input, size_t length);
    void final();
    void transform(const byte block[64]);
    void encode(const uint32* input, byte* output, size_t length);
    void decode(const byte* input, uint32* output, size_t length);
    string bytesToHexString(const byte* input, size_t length);

    /* class uncopyable */
    _syrah_encode_md5(const _syrah_encode_md5&);
    _syrah_encode_md5& operator=(const _syrah_encode_md5&);

private:
    uint32 _state[4];	/* state (ABCD) */
    uint32 _count[2];	/* number of bits, modulo 2^64 (low-order word first) */
    byte _buffer[64];	/* input buffer */
    byte _digest[16];	/* message digest */
    bool _finished;		/* calculate finished ? */

    static const byte PADDING[64];	/* padding for calculate */
    static const char HEX[16];
    enum { BUFFER_SIZE = 1024 };
};

#endif // _SYRAH_ENCODE_MD5_H
