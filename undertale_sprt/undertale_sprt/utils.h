#pragma once
#include <cstdint>
#include <iostream>
#include <fstream>
#include <string>

size_t filesize(const char* filename);

std::string data_string(std::fstream& data, uint32_t pos);

void data_readblock(std::fstream& data, uint32_t pos, char* out, size_t size);

void data_writeblock(std::fstream& data, uint32_t pos, char* in, size_t size);

size_t chrtol(char c);
