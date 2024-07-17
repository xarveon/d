#pragma once
#include <cstdint>
#include <string>

#pragma pack(push,1)
struct chunk_head
{
    uint32_t Name, Size;
    std::string name_str();
};

struct GEN8_head
{
    chunk_head head;
    char DebugDisabled, BytecodeVersion;
    uint16_t Unknown1;
    uint32_t sFilename, sConfig, LastObj, LastTile, GameID,
        Unknown2[4], sName, Major, Minor, Release, Build,
        DefWindowWidth, DefWindowHeight, InfoFlags;
    char LicenseMD5[16];
    uint32_t LicenseCRC32;
    uint64_t Timestamp;
    uint32_t sDisplayName, ActiveTargets, Unknown3[4], SteamAppId, NumberCount;
};

struct OPTN_head
{
    chunk_head head;
    uint32_t Unknown1[2], InfoFlags, Unknown2[12];
    uint32_t NumItems;
};

struct OPTN_item
{
    uint32_t Name, Value;
};

struct LANG_head
{
    chunk_head head;
    uint32_t Unknown[3];
};

struct SOND_head
{
    chunk_head head;
    uint32_t NumItems;
};

struct SOND_item
{
    uint32_t Name, Flags, Type, File, Uknown,
        Volume, Pitch, GroupID, AudioId;
};

struct AGRP_head
{
    chunk_head head;
    uint32_t NumItems;
};

struct SPRT_head
{
    chunk_head head;
    uint32_t NumItems;
};

struct SPRT_item
{
    uint32_t Name, Width, Height, MarginLeft, MarginRight, MarginBottom,
        MarginTop, Unknown[3], BBoxMode, SepMasks, OriginX, OriginY, TextureCount;
};

struct SPRT_info
{
    std::string Name;
    uint32_t Width, Height, MaskCount, MasksStart, MasksEnd;
};

struct OBJT_head
{
    chunk_head head;
    uint32_t NumItems;
};

struct BGND_head
{
    chunk_head head;
    uint32_t NumItems;
};

struct BGND_item
{
    uint32_t Name, Unknown, TextureAddress;
};

struct PATH_head
{
    chunk_head head;
    uint32_t NumItems;
};

struct PATH_item
{
    uint32_t Name, IsSmooth, IsClosed, Precision;
};

struct TPAG_head
{
    chunk_head head;
    uint32_t NumItems;
};

struct TPAG_item
{
    uint16_t X, Y, Width, Height, RenderX, RenderY, BoundingX,
        BoundingY, BoundingWidth, BoundingHeight, SpritesheetId;
};

///////
/// Insert
///////

struct CODE_head
{
    chunk_head head;
    uint32_t NumItems;
};

struct CODE_item
{
    uint32_t sName, Length, Unknown1, ByteCodeOffset, Unknown2;
};

struct STRG_head
{
    chunk_head head;
    uint32_t NumElements;
};

struct TXTR_head
{
    chunk_head head;
    uint32_t NumItems;
};

struct TXTR_item
{
    uint32_t Unknown, Address;
};

#pragma pack(pop)