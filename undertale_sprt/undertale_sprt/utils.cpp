#include <cstdint>
#include <string>
#include <sys/stat.h>
#include "utils.h"

// Get file size
size_t filesize(const char* filename)
{
    struct stat statbuf;
    return (stat(filename, &statbuf) == -1) ? 0 : statbuf.st_size;
}

// Extract string from data file by position
std::string data_string(std::fstream& data, uint32_t pos)
{
    // Save current seek position
    auto saved_pos = data.tellg();
    data.seekg(pos);

    // Read null-terminated string at offset (UTF-8)
    std::string s;
    std::getline(data, s, '\0');

    // Restore original seek position and return read string
    data.seekg(saved_pos);
    return s;
}

void data_readblock(std::fstream& data, uint32_t pos, char* out, size_t size)
{
    // Save current seek position
    auto saved_pos = data.tellg();
    data.seekg(pos);

    // Read into out buffer
    data.read((char*)out, size);

    // Restore original seek position
    data.seekg(saved_pos);
}

void data_writeblock(std::fstream& data, uint32_t pos, char* in, size_t size)
{
    // Save current seek position
    auto saved_pos = data.tellg();
    data.seekg(pos, std::ios::beg);
    //data.seekp(pos, std::ios::beg);

    // Read into out buffer
    data.write((char*)in, size);

    // Restore original seek position
    data.seekg(saved_pos);
}

size_t chrtol(char c)
{
    return (((size_t)c) & 0xff);
}
