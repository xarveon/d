names = { b'FORM', b'GEN8', b'OPTN', b'LANG', b'EXTN', b'SOND',
          b'AGRP', b'SPRT', b'BGND', b'PATH', b'SCPT', b'GLOB',
          b'GMEN', b'SHDR', b'FONT', b'TMLN', b'OBJT', b'ROOM',
          b'DAFL', b'TPAG', b'CODE', b'VARI', b'FUNC', b'STRG',
          b'TXTR', b'AUDO', b'EMBI', b'TGIN', b'ACRV', b'SEQN',
          b'TAGS', b'FEDS', b'FEAT', b'PSEM', b'PSYS', b'DBGI',
          b'INST', b'LOCL', b'STAT', b'GEN7', b'RASP', b'PSPS' }

contents = b''

with open('data.win', 'rb') as file:
    contents = file.read()

for i in range(len(contents) - 4):
    if contents[i : i+4] in names:
        print(f'[{hex(i)}]    {contents[i : i+4]}    {contents[i : i+8]}')

print('Done.')
