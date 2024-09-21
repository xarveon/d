
**Extracting data.win from DFC**

# Unofficial GML format documentation

https://pcy.ulyssis.be/undertale/unpacking-corrected
https://gitlab.com/-/snippets/14944
https://github.com/puggsoy/GMExtract/blob/master/src/structure/ListChunk.hx
https://github.com/panzi/cook-serve-hoomans/blob/master/fileformat.md

# Dumping the data.win file

Open the DFC executable in a disassembler like IDA.

The "encrypted" data.win file will be stored somewhere in the .wdata segment, and the decryption loop is usually aroun 47B2BF and looks like
```
.text:0047B2BF                 mov     edi, 0A5D220CDh
.text:0047B2C4                 lea     edx, unk_9CE8558[eax]
.text:0047B2CA
.text:0047B2CA loc_47B2CA:
.text:0047B2CA                 mov     ecx, [eax]
.text:0047B2CC                 add     eax, 4
.text:0047B2CF                 imul    edi, 9CA97ED9h
.text:0047B2D5                 add     edi, 8EC01477h
.text:0047B2DB                 xor     ecx, edi
.text:0047B2DD                 mov     [eax-4], ecx    ; ecx contains decrypted DWORD
.text:0047B2E0                 cmp     edx, eax
.text:0047B2E2                 jnz     short loc_47B2CA
```

If we want to dump the entire file as it decrypts, we can instrument the instruction at 47B2DD and save each decrypted DWORD sequentially to a file, using a custom tool like ```dfc_dump```included here.

# Locating and fixing the chunk names

## Scanning for known chunk names

DFC replaces nearly all of the chunk names with either other real chunk names or a handful of names they appear to have made up but often use.

To identify the offsets to all of the chunks, we need to scan the extracted data.win file for these known names that could be used, then examine the surrounding characters, implied chunk sizes, and chunk formats both to rule out false positives and to spot when we may have missed one.

Running the included ```findchunks.py``` with data.win extracted from DFC 2.7.9c in the same folder gives us this output after a minute or so:
```
[0x8]    b'SCPT'    b'SCPT\x00j\x00\x00'
[0x520]    b'STRG'    b'STRGG\xd5\x00\x00'
[0x578]    b'FUNC'    b'FUNC\xd5O\x00\x00'
[0x58c]    b'GEN8'    b'GEN8F\x84\x00\x00'
[0x2294]    b'TPAG'    b'TPAGt\xb3\x00\x00'
[0x54a0]    b'VARI'    b'VARI\x1eV\x00\x00'
[0x54bc]    b'STAT'    b'STAT\xd5]\x00\x00'
[0x387c10]    b'OBJT'    b'OBJT\x96\x15\x00\x00'
[0x388b04]    b'ROOM'    b'ROOM\xbfX\x00\x00'
[0x38a8d4]    b'AUDO'    b'AUDO\x91\x92\x00\x00'
[0x38bfcc]    b'RASP'    b'RASP\xcac\x00\x00'
[0x38bfdc]    b'SHDR'    b'SHDR\x84p\x00\x00'
[0x38c0c8]    b'PATH'    b'PATH\x1c\xe5\x00\x00'
[0x3a6a9c]    b'GEN7'    b'GEN7Q:\x00\x00'
[0x3a6aa8]    b'TPAG'    b'TPAG\xfc\xea\x00\x00'
[0x4417ec]    b'OPTN'    b'OPTN\xc8\xe6\x00\x00'
[0xc0cc8c]    b'AGRP'    b'AGRP\xeb\xef\x00\x00'
[0xc0cc94]    b'AGRP'    b'AGRPD\x87\x00\x00'
[0xc39ef0]    b'STAT'    b'STAT\xe1<\x00\x00'
[0x103d094]    b'TMLN'    b'TMLNS\x0f\x00\x00'
[0x1074cec]    b'EMBI'    b'EMBI\xbe\x7f\x00\x00'
[0x1093ffc]    b'STRG'    b'STRGrZ\x00\x00'
[0x10b149e]    b'FUNC'    b'FUNCTION'
[0x10bb5ab]    b'STAT'    b'STATUS: '
[0x10bb667]    b'STAT'    b'STATUS F'
[0x10bb697]    b'STAT'    b'STATUS I'
[0x10bd56a]    b'ROOM'    b'ROOM DAN'
[0x10bdb42]    b'ROOM'    b'ROOM DAN'
[0x10d8286]    b'FEAT'    b'FEAT\\nYO'
[0x10e6114]    b'STAT'    b'STATUS!\x00'
[0x10e7e6a]    b'STAT'    b'STATUS:\x00'
[0x10ebf0e]    b'INST'    b'INSTRUCT'
[0x10ed15e]    b'STAT'    b'STAT\x00\x04\x00\x00'
[0x10ee923]    b'STAT'    b'STATUS\x00\x07'
[0x10f5773]    b'GLOB'    b'GLOBAL C'
[0x10f5862]    b'GLOB'    b'GLOBAL C'
[0x10f660d]    b'ROOM'    b'ROOM#/TO'
[0x10f7876]    b'ROOM'    b'ROOM POG'
[0x10f79df]    b'ROOM'    b'ROOM DAN'
[0x10f9ee3]    b'VARI'    b'VARIK:\x00\x0e'
[0x11063c5]    b'ROOM'    b'ROOM.\x00-\x00'
[0x1106f16]    b'STAT'    b'STATUS: '
[0x112c8ff]    b'GMEN'    b'GMENT SH'
[0x112ca43]    b'GMEN'    b'GMENT SH'
[0x112d3cd]    b'FORM'    b'FORMS\r\nu'
[0x112f4e5]    b'GMEN'    b'GMENT SH'
[0x112f629]    b'GMEN'    b'GMENT SH'
[0x112ffa1]    b'FORM'    b'FORMS\r\nu'
[0x1134b04]    b'GMEN'    b'GMENT SH'
[0x1134c48]    b'GMEN'    b'GMENT SH'
[0x1135580]    b'FORM'    b'FORMS\r\nu'
[0x113766a]    b'GMEN'    b'GMENT SH'
[0x11377ae]    b'GMEN'    b'GMENT SH'
[0x11380d4]    b'FORM'    b'FORMS\r\nu'
[0x117e800]    b'OPTN'    b'OPTN,h\x00\x00'
[0x1e3c058]    b'PSPS'    b'PSPS\x0bp\x00\x00'
[0x35df361]    b'ROOM'    b'ROOM8\x07\x0f\xba'
[0x634aa90]    b'CODE'    b'CODED BY'
[0x644251c]    b'FEAT'    b'FEAT\xac\x88\xaa\xc1'
[0x6536d1c]    b'CODE'    b'CODED BY'
[0x997df93]    b'PSPS'    b'PSPSTTPR'
Done.
```

The correct chunk names and positions for this data.win file are added to the end of each line here:
```
[0x0]                                               FORM
[0x8]    b'SCPT'    b'SCPT\x00j\x00\x00'            GEN8
[0x520]    b'STRG'    b'STRGG\xd5\x00\x00'          OPTN
[0x578]    b'FUNC'    b'FUNC\xd5O\x00\x00'          LANG
[0x58c]    b'GEN8'    b'GEN8F\x84\x00\x00'          EXTN
[0x2294]    b'TPAG'    b'TPAGt\xb3\x00\x00'         SOND
[0x54a0]    b'VARI'    b'VARI\x1eV\x00\x00'         AGRP
[0x54bc]    b'STAT'    b'STAT\xd5]\x00\x00'         SPRT
[0x387c10]    b'OBJT'    b'OBJT\x96\x15\x00\x00'    BGND
[0x388b04]    b'ROOM'    b'ROOM\xbfX\x00\x00'       PATH
[0x38a8d4]    b'AUDO'    b'AUDO\x91\x92\x00\x00'    SCPT
[0x38bfcc]    b'RASP'    b'RASP\xcac\x00\x00'       GLOB
[0x38bfdc]    b'SHDR'    b'SHDR\x84p\x00\x00'       SHDR
[0x38c0c8]    b'PATH'    b'PATH\x1c\xe5\x00\x00'    FONT
[0x3a6a9c]    b'GEN7'    b'GEN7Q:\x00\x00'          TMLN
[0x3a6aa8]    b'TPAG'    b'TPAG\xfc\xea\x00\x00'    OBJT
[0x4417ec]    b'OPTN'    b'OPTN\xc8\xe6\x00\x00'    ROOM
[0xc0cc8c]    b'AGRP'    b'AGRP\xeb\xef\x00\x00'    DAFL
[0xc0cc94]    b'AGRP'    b'AGRPD\x87\x00\x00'       TPAG
[0xc39ef0]    b'STAT'    b'STAT\xe1<\x00\x00'       CODE
[0x103d094]    b'TMLN'    b'TMLNS\x0f\x00\x00'      VARI
[0x1074cec]    b'EMBI'    b'EMBI\xbe\x7f\x00\x00'   FUNC
[0x1093ffc]    b'STRG'    b'STRGrZ\x00\x00'         STRG
[0x117e800]    b'OPTN'    b'OPTN,h\x00\x00'         TXTR
[0x1e3c058]    b'PSPS'    b'PSPS\x0bp\x00\x00'      AUDO
```

The first 4 characters of the file should always be corrected to "FORM".  There are usually random bytes where the name should be and it will not show up in the scan, but this correction must always be made along with the others.

## Missing chunks

When I first ran the script against data.win, I didn't have 'GEN8' in its database of names to look for, so the initial output looked like this:
```
[0x8]    b'SCPT'    b'SCPT\x00j\x00\x00'            GEN8
[0x520]    b'STRG'    b'STRGG\xd5\x00\x00'          OPTN
[0x578]    b'FUNC'    b'FUNC\xd5O\x00\x00'          LANG
[0x2294]    b'TPAG'    b'TPAGt\xb3\x00\x00'         EXTN???
```

It was possible to recognize that something was missing, because in past dumps the LANG chunk was never that large.  Searching in a hex editor for a few lines after the offset where we believe LANG starts (currently labeled FUNC in the file), we can find what must be our missing label:
```
Offset(h) 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F

00000570  2C BE 11 01 3D BE 11 01 46 55 4E 43 D5 4F 00 00  ,¾..=¾..FUNCÕO..
00000580  01 00 00 00 00 00 00 00 00 00 00 00 47 45 4E 38  ............GEN8
00000590  46 84 00 00 04 00 00 00 A8 05 00 00 F4 07 00 00  F„......¨...ô...
```

It's a good idea to add newly discovered chunk names used to the python script and run it again, because they sometimes get reused and it could save us from trying to find another missing chunk later in the file.

While the correct chunk names always occur in the same order, sometimes the GML format is updated, and a new version will start using new chunk types (FONT and GLOB are examples that didn't appear in some versions from last year but showed up later.)

## Verifying chunk identities

We watch out for missed chunks and other mistakes by occassionally examining a chunk after deciding what we believe is the correct label to make sure we can follow the format as outlined in some of the unofficial documentation and it making sure it makes sense.

We're generally trying to find an object name which will usually be descriptive and confirm that we're looking at what we think we are.

Say we believe this chunk is actually supposed to be SPRT
```
[0x54bc]    b'STAT'    b'STAT\xd5]\x00\x00'
```

The format of a SPRT chunk should be
```
char chunk_name[4]; // "SPRT" (originally labeled "STAT")
uint32_t chunk_size;
uint32_t array_size;
uint32_t pointer_array[array_size];
```

So the 3rd 4-byte value after SPRT should be a pointer to the first sprite object
```
Offset(h) 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F

000054B0  B8 54 00 00 D7 03 12 01 EE 03 12 01 53 54 41 54  ¸T..×...î...STAT
000054C0  D5 5D 00 00 AA 08 00 00 70 77 00 00 BC 80 00 00  Õ]..ª...pw..¼€..
000054D0  14 8A 00 00 84 8E 00 00 40 96 00 00 F4 9D 00 00  .Š..„Ž..@–..ô...
```

So if this is a SPRT chunk, then the pointer array should start at 54C8, and the first object should be at 7770 (the values are 32-bit little endian).

The first sanity check is that this is actually a pointer array.  We see 7770, 80bc, 8a14, etc.

This makes sense, as they look somewhat evenly spaced and all point to data that should be in this same chunk (after the start of the array and before where we believe the next chunk starts at 387c10.)

At 7770 should be a SPRT object, which we can check the docs to see has the format
```
uint32_t name_offset;
uint32_t width;
uint32_t height;
...
```

We want to take a look at the name stored at that offset.

Here is our presumed SPRT object:
```
Offset(h) 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F

00007770  04 04 12 01 42 00 00 00 88 00 00 00 00 00 00 00  ....B...ˆ.......
00007780  10 01 00 00 41 00 00 00 11 01 00 00 00 00 00 00  ....A...........
```

And let's see what's at offset 0x01120404 (which should be the name if this is really a sprite):
```
Offset(h) 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F

01120400  09 00 00 00 73 70 72 5F 74 69 74 6C 65 00 10 00  ....spr_title...
```

This is a valid string offset, and "spr_title" definitely sounds like a sprite name, so we have confirmed that we have likely correctly identified the SPRT chunk.

It isn't necessary to do this for every single chunk after some experience, but anytime something isn't making sense, it's a good way to start verifying your assumptions and confirm a few of them to make it easier to spot where you may have missed identifying a chunk or the format may have changed slightly to add or remove newer chunks.

## False positives

If we look back at the python scan, we clearly have found way too many things, and by looking at the last column which also contains some of the characters that immediately follow, we can see that some of these are likely from the string table or exif data from the TXTR or AUDO files:
```
...
[0x10b149e]    b'FUNC'    b'FUNCTION'
[0x10bb5ab]    b'STAT'    b'STATUS: '
[0x10bb667]    b'STAT'    b'STATUS F'
[0x10bb697]    b'STAT'    b'STATUS I'
[0x10bd56a]    b'ROOM'    b'ROOM DAN'
[0x10bdb42]    b'ROOM'    b'ROOM DAN'
[0x10d8286]    b'FEAT'    b'FEAT\\nYO'
[0x10e6114]    b'STAT'    b'STATUS!\x00'
...
```

If it's clearly part of a sentence, we can be relatively certain these are just random words found in the string table and not the actual start of any chunks.

We can delete or ignore those during our analysis, while keeping in mind that the STRG chunk needs to begin somewhere before these offsets, which serves as a sanity check when we're working out the real chunk names.

Also of note are some of the entries found near the end
```
[0x35df361]    b'ROOM'    b'ROOM8\x07\x0f\xba'
[0x634aa90]    b'CODE'    b'CODED BY'
[0x644251c]    b'FEAT'    b'FEAT\xac\x88\xaa\xc1'
[0x6536d1c]    b'CODE'    b'CODED BY'
[0x997df93]    b'PSPS'    b'PSPSTTPR
```

These are all from inside of AUDO sound file exif data and aren't the start of real chunks.  You should have found the AUDO chunk somewhere immediately before these.

# Fixing chunk sizes

Every chunk starts with the same 8 bytes
```
char chunk_name[4];
uint32_t chunk_size;
```

Where chunk_size should be the offset of the start of the next chunk (ie. one past the last byte of this chunk) minus the offset immediately following the chunk_size.

These sizes are generally all incorrect in the dumped file and it can't be parse without correcting them.  For chunks in the middle of the file, calculating the correct size is relatively simple.  For example, if we have identified and verified these chunks:
```
...
[0x520]    OPTN
[0x578]    LANG
...
```

Then the correct size for the OPTN chunk is 0x578 - 0x520 - 8 = 0x50, which should be entered in little endian immediately after the chunk label like this:
```
Offset(h) 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F
          name        size
00000520  4F 50 54 4E 50 00 00 00 00 00 00 80 02 00 00 00  OPTNP......€....
```

We subtract 8 because it needs to be the size starting from after chunk_size to the end, which starts 8 bytes into the chunk.

When calculating the size for FORM and AUDO, there is no next chunk offset to use in the calculation, so the correct value to use is one past the last byte of the file.

This file ends like this:
```
Offset(h) 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F

09CE8540  00 0E 20 10 00 00 00 00 00 00 00 00 00 00 00 00  .. .............
09CE8550  00 80 86 00 40 3B D4 6D                          .€†.@;Ôm
```

So the end offset we calculate from is 0x9ce8558.

Since FORM starts at offset 0, its size is 0x9ce8558 - 0 - 8 = 0x9ce8550
```
Offset(h) 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F
          name        size
00000000  46 4F 52 4D 50 85 CE 09 47 45 4E 38 10 05 00 00  FORMP…Î.GEN8....
```

AUDO starts at offset 0x1e3c058 in this file, and we also calculate its size from one past the end of the file, giving 0x9ce8558 - 0x1e3c058 - 8 = 0x7eac4f8
```
Offset(h) 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F
                                  name        size
01E3C050  44 AE 42 60 82 00 00 00 41 55 44 4F F8 C4 EA 07  D®B`‚...AUDOøÄê.
```

# Fixing the string array size

Starting a few versions ago, the size of the STRG pointer array also started to be corrupted.  Since most chunk types contain pointer or object arrays, it's possible this trick could be extended to other chunks in future versions, but currently it is only applied the the STRG chunk.

The format of the STRG chunk starts similarly to the SPRT chunk we saw above:
```
char chunk_name[4]; // "STRG"
uint32_t chunk_size;
uint32_t array_size;
uint32_t pointer_array[array_size];
```

With the difference being that array_size is usually purposely set to an incorrect value here:
```
Offset(h) 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F

                                              name
01093FF0  D9 E7 17 01 00 00 00 00 CC BD 0A 01 53 54 52 47  Ùç......Ì½..STRG

          chunk_size  array_size  pointer[0]  pointer[1]          
01094000  FC A7 0E 00 XX XX XX XX AC BD 0A 01 BA BD 0A 01  ü§..i_..¬½..º½..

          pointer[2]  ...      
01094010  C8 BD 0A 01 D6 BD 0A 01 0B C7 0A 01 1E C7 0A 01  È½..Ö½...Ç...Ç..
```

We need to calculate the correct array size and fix this.  This is relatively easy because the first element pointed to by pointer[0] will generally start immediately following the end of the array, as we can see here
```
Offset(h) 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F

          ptr[n-2]    ptr[n-1]    pointer[n]  string0_len
010ABDA0  96 E7 17 01 B1 E7 17 01 D5 E7 17 01 09 00 00 00  –ç..±ç..Õç......

          string0 ...
010ABDB0  70 72 6F 74 6F 74 79 70 65 00 09 00 00 00 40 40  prototype.....@@
```

So the length of the pointer array is just the offset of the first string object minus the offset of pointer[0], divided by 4 (since each pointer takes up 4 bytes.)

Looking back at the beginning of the chunk
```
Offset(h) 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F

                                              name
01093FF0  D9 E7 17 01 00 00 00 00 CC BD 0A 01 53 54 52 47  Ùç......Ì½..STRG

          chunk_size  array_size  pointer[0]  pointer[1]
01094000  FC A7 0E 00 XX XX XX XX AC BD 0A 01 BA BD 0A 01  ü§..i_..¬½..º½..
```

The correct array_size is (0x010abdac - 0x1094008) / 4 = 0x5f69
```
Offset(h) 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F

                                              name
01093FF0  D9 E7 17 01 00 00 00 00 CC BD 0A 01 53 54 52 47  Ùç......Ì½..STRG

          chunk_size  array_size  pointer[0]  pointer[1]
01094000  FC A7 0E 00 69 5F 00 00 AC BD 0A 01 BA BD 0A 01  ü§..i_..¬½..º½..
```

Now we're almost there.

# Fixing the sprites

A newer obfuscation added a few versions ago is to scramble some of the location and size values for each sprite.

UndertaleModTool tries to read the sprite objects linearly, calculating their sizes in the file from their widths and heights, rather than reading their offsets from the SPRT pointer array.  When the widths and heights are incorrect, this causes it to miscalculate their sizes and get misaligned, read past the end of the file, etc.  It completely breaks down.

Each sprite needs to be corrected by swapping the values in the file back into the right place like so:
```C
uint32_t tmp = item.OriginX;
item.OriginX = item.Height;
item.Height = item.Width;
item.Width = item.MarginTop;
item.MarginTop = tmp;
```

This is implemented in the included ```undertale_sprt``` project.

# Troubleshooting

Once all of these corrections are performed, the final data.win file should be completely valid and can be read with UndertaleModTool.

If it throws an error while reading a section, then pay attention to which chunk it was trying to read and the details of the errors it throws.  It is often very helpful in explaining which index within a chunk's array it was trying to read, and offsets or values it was trying to read, which are a good starting point for trying to narrow down where the problem is.

Becoming familiar with UndertaleModTool's source code to understand how it parses different chunks and what can cause specific errors it raises is extremely helpful in diagnosing problems.

And that's pretty much it!
SabaKira The Decompiler =)
I always respected you before you poisoned innocent people. R
https://sabakira.net/dfc/
