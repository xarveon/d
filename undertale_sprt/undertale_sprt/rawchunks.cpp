#include <cstdint>
#include "rawchunks.h"

// Convert chunk Name field into printable string
std::string chunk_head::name_str()
{
    std::string s;

    s.resize(4);
    *((uint32_t*)&s[0]) = Name;

    return s;
}
