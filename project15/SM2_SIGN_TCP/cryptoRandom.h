#ifndef CRYPTORANDOM_H
#define CRYPTORANDOM_H

#include <stdint.h>
#include <Windows.h>
#include <bcrypt.h>

int random_bytes_default(uint8_t* buf, size_t len)
{
    if (!BCRYPT_SUCCESS(BCryptGenRandom(NULL, buf, (ULONG)len, BCRYPT_USE_SYSTEM_PREFERRED_RNG))) {
        return -1;
    }
    return 0;
}

#endif