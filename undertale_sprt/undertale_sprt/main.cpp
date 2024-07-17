#include <iostream>
#include <fstream>
#include <map>
#include "utils.h"
#include "rawchunks.h"

#pragma pack(push,1)
struct SPRT_patch
{
    uint32_t Width, Height;

    SPRT_patch() : Width(1), Height(1) {}
    SPRT_patch(uint32_t len) : Width(1), Height(len) {}
};
#pragma pack(pop)

int error(const char* msg)
{
    std::cerr << msg << '\n';
    return -1;
}

int main(int argc, char **argv)
{
    if (argc != 2)
        return error("Input data file required as arguments!");

    // Open data file
    std::fstream data;
    data.open(argv[1], std::ios::in | std::ios::out | std::ios::binary);

    if (!data.is_open())
        return error("Could not open data file!");

    size_t data_size = filesize(argv[1]);

    if (data_size < sizeof(chunk_head) + sizeof(GEN8_head))
        return error("Data file too small!");

    // Intro
    std::cout << "Parsing data file (all values in hex)\nFile size:\t" << std::hex << data_size << '\n';

    // Read FORM chunk
    size_t pos = 0;

    chunk_head form;
    data.read((char*)&form, sizeof(form));

    std::cout << '[' << pos << "]\t == FORM == \nName:\t" << form.name_str() << "\nSize:\t" << form.Size << "\n\n";

    // Read GEN8 chunk
    GEN8_head gen;
    pos += sizeof(chunk_head);
    data.read((char*)&gen, sizeof(gen));

    std::cout << '[' << pos << "]\t== GEN8 ==\nName:\t\t" << gen.head.name_str() << "\nSize:\t\t" << gen.head.Size << '\n';
    std::cout << "DebugDisabled:\t" << chrtol(gen.DebugDisabled) << "\nBytecodeVersion:\t" << chrtol(gen.BytecodeVersion) << '\n';
    std::cout << "Unknown1:\t" << gen.Unknown1 << '\n';
    std::cout << "sFilename:\t[" << gen.sFilename << "] " << data_string(data, gen.sFilename) << '\n';
    std::cout << "sConfig:\t[" << gen.sConfig << "] " << data_string(data, gen.sConfig) << '\n';
    std::cout << "LastObj:\t" << gen.LastObj << "\nLastTile:\t" << gen.LastTile << "\nGameID:\t" << gen.GameID << '\n';
    std::cout << "Unknown2:\t" << gen.Unknown2[0] << ' ' << gen.Unknown2[1] << '\n';
    std::cout << "sName:\t[" << gen.sName << "] " << data_string(data, gen.sName) << '\n';
    std::cout << "Major:\t" << gen.Major << "\nMinor:\t" << gen.Minor << "\nRelease:\t" << gen.Release << "\nBuild:\t" << gen.Build << '\n';
    std::cout << "DefWindowWidth:\t" << gen.DefWindowWidth << "\nDefWindowHeight:\t" << gen.DefWindowHeight << '\n';
    std::cout << "InfoFlags:\t" << gen.InfoFlags << " (Studio version " << ((gen.InfoFlags & 0x0E00) >> 9) << ")\n";

    std::cout << "LicenseMD5: ";
    for (size_t i = 0; i < 16; i++)
        std::cout << chrtol(gen.LicenseMD5[i]) << ' ';
    std::cout << '\n';

    std::cout << "LicenseCRC32:\t" << gen.LicenseCRC32 << "\nTimeStamp:\t" << gen.Timestamp << '\n';
    std::cout << "sDisplayName:\t[" << gen.sDisplayName << "] " << data_string(data, gen.sDisplayName) << '\n';
    std::cout << "ActiveTargets:\t" << gen.ActiveTargets << '\n';
    std::cout << "Unknown3:\t" << gen.Unknown3[0] << ' ' << gen.Unknown3[1] << ' ' << gen.Unknown3[2] << ' ' << gen.Unknown3[3] << '\n';
    std::cout << "SteamAppId:\t" << gen.SteamAppId << '\n';
    std::cout << "NumberCount:\t" << gen.NumberCount << '\n';

    std::cout << "Numbers:\t";

    for (size_t i = 0; i < gen.NumberCount; i++)
    {
        uint32_t number;
        data.read((char*)&number, sizeof(number));

        std::cout << number << ' ';
    }

    std::cout << "\n\n";

    // Read OPTN Chunk
    pos += sizeof(chunk_head) + gen.head.Size;
    data.seekg(pos);

    OPTN_head optn;
    data.read((char*)&optn, sizeof(optn));

    std::cout << '[' << pos << "]\t == OPTN == \nName:\t" << optn.head.name_str() << "\nSize:\t" << optn.head.Size << '\n';

    std::cout << "Unknown1:\t" << optn.Unknown1[0] << ' ' << optn.Unknown1[1] << '\n';
    std::cout << "InfoFlags:\t" << optn.InfoFlags << '\n';

    std::cout << "Unknown2:\t";
    for (size_t i = 0; i < 12; i++)
        std::cout << optn.Unknown2[i] << ' ';

    std::cout << "\nConstantMap\n\tSize:\t" << optn.NumItems << '\n';
    std::cout << "\tElements:\n";

    for (size_t i = 0; i < optn.NumItems; i++)
    {
        OPTN_item item;
        data.read((char*)&item, sizeof(item));

        std::cout << "\t\tName:\t" << item.Name << "\t" << data_string(data, item.Name) << '\n';
        std::cout << "\t\tValue:\t" << item.Value << "\t" << data_string(data, item.Value) << "\n\n";
    }

    // Read LANG chunk
    pos += sizeof(chunk_head) + optn.head.Size;
    data.seekg(pos);

    LANG_head lang;
    data.read((char*)&lang, sizeof(lang));

    std::cout << '[' << pos << "]\t == LANG == \nName:\t" << lang.head.name_str() << "\nSize:\t" << lang.head.Size << '\n';
    std::cout << "Unknown:\t" << lang.Unknown[0] << ' ' << lang.Unknown[1] << ' ' << lang.Unknown[2] << "\n\n";

    // Read EXTN chunk
    pos += sizeof(chunk_head) + lang.head.Size;
    data.seekg(pos);

    chunk_head extn;
    data.read((char*)&extn, sizeof(extn));

    std::cout << '[' << pos << "]\t == EXTN == \nName:\t" << extn.name_str() << "\nSize:\t" << extn.Size << '\n';
    std::cout << "Format varies - skipping enumeration\n\n";

    // Read SOND chunk
    pos += sizeof(chunk_head) + extn.Size;
    data.seekg(pos);

    SOND_head sond;
    data.read((char*)&sond, sizeof(sond));

    std::cout << '[' << pos << "]\t == SOND == \nName:\t" << sond.head.name_str() << "\nSize:\t" << sond.head.Size << '\n';
    std::cout << "NumItems: " << sond.NumItems << '\n';

    for (size_t i = 0; i < sond.NumItems; i++)
    {
        uint32_t address;
        data.read((char*)&address, sizeof(address));

        SOND_item item;
        data_readblock(data, address, (char*)&item, sizeof(item));

        std::cout << "\t\tName:\t" << item.Name << "\t" << data_string(data, item.Name) << '\n';
        std::cout << "\t\tFlags:\t" << item.Flags << '\n';
        std::cout << "\t\tType:\t" << item.Type << "\t" << data_string(data, item.Type) << '\n';
        std::cout << "\t\tFile:\t" << item.File << "\t" << data_string(data, item.File) << '\n';
        std::cout << "\t\tUnknown:\t" << item.Uknown << '\n';
        std::cout << "\t\tVolume:\t" << item.Volume << '\n';
        std::cout << "\t\tPitch:\t" << item.Pitch << '\n';
        std::cout << "\t\tAudioId:\t" << item.AudioId << "\n\n";

        break;
    }

    std::cout << "\t\t...\n\n";

    // Read AGRP
    pos += sizeof(chunk_head) + sond.head.Size;
    data.seekg(pos);

    AGRP_head agrp;
    data.read((char*)&agrp, sizeof(agrp));

    std::cout << '[' << pos << "]\t == AGRP == \nName:\t" << agrp.head.name_str() << "\nSize:\t" << agrp.head.Size << '\n';
    std::cout << "NumItems: " << agrp.NumItems << '\n';

    for (size_t i = 0; i < agrp.NumItems; i++)
    {
        uint32_t address;
        data.read((char*)&address, sizeof(address));

        uint32_t agrp_name;
        data_readblock(data, address, (char*)&agrp_name, sizeof(agrp_name));

        std::cout << "\t\tName:\t" << agrp_name << "\t" << data_string(data, agrp_name) << '\n';
    }

    // Read SPRT
    pos += sizeof(chunk_head) + agrp.head.Size;
    data.seekg(pos);

    SPRT_head sprt;
    data.read((char*)&sprt, sizeof(sprt));

    std::cout << "\n[" << pos << "]\t == SPRT == \nName:\t" << sprt.head.name_str() << "\nSize:\t" << sprt.head.Size << '\n';
    std::cout << "NumItems: " << sprt.NumItems << "\n\n";

    uint32_t *item_list = new uint32_t[sprt.NumItems];

    for (size_t i = 0; i < sprt.NumItems; i++)
    {
        data.read((char*)&item_list[i], sizeof(uint32_t));
    }

    std::map<uint32_t, SPRT_patch> SPRT_patches;

    for (size_t i = 0; i < sprt.NumItems; i++)
    {
        uint32_t item_address = item_list[i];

        std::cout << '[' << std::hex << item_address << ']' << std::endl;

        SPRT_item item;
        data_readblock(data, item_address, (char*)&item, sizeof(item));

        std::cout << "\t\tName:\t" << item.Name << "\t" << data_string(data, item.Name) << "\n\n";

        std::cout << "Uncorrected (decimal values)\n" << std::dec;
        std::cout << "\t\tWidth:\t" << item.Width << '\n';
        std::cout << "\t\tHeight:\t" << item.Height << '\n';
        std::cout << "\t\tMarginTop:\t" << item.MarginTop << '\n';
        std::cout << "\t\tOriginX:\t" << item.OriginX << "\n\n";

        // Unswap sprite info
        uint32_t tmp = item.OriginX;
        item.OriginX = item.Height;
        item.Height = item.Width;
        item.Width = item.MarginTop;
        item.MarginTop = tmp;

        std::cout << "Corrected (decimal values)\n" << std::dec;
        std::cout << "\t\tWidth:\t" << item.Width << '\n';
        std::cout << "\t\tHeight:\t" << item.Height << '\n';
        std::cout << "\t\tMarginTop:\t" << item.MarginTop << '\n';
        std::cout << "\t\tOriginX:\t" << item.OriginX << '\n';

        data_writeblock(data, item_address, (char*)&item, sizeof(item));
    }

    data.close();

    return 0;
}

