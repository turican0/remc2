"""Converts the regression-test recordings from the old layout (no spells) to the current one.

The current layout (portability/InputRecorder.cpp) stores, for every player, the spell state the
player enters the level with: SpellsEnabled int16[26], SpellIndexes uint8[26], SpellLevels uint8[26],
SpellsExperience int32[26], written right after the player's turn count.  remc2 records it at the
end of InitialiseSpells_54A50 and writes it back at the start of the same function on playback.

The old recordings have no such block, so the values are taken from the original game: frame 0 of
the DOSBox memimages of the test that plays the recording (the state right after the level was
initialised).  SpellsEnabled holds spell entity indices by frame 0, while InitialiseSpells leaves
1/0 in it, so any non-zero value becomes 1.  Where no test plays a level, the level-start memimage
(levelN, N = level + 1) is used.  The turns themselves are copied byte for byte.

    python convert_old_recordings.py
"""
import os
import struct

HERE = os.path.dirname(os.path.abspath(__file__))
TEST_DIR = os.path.dirname(HERE)
# the reference data kept in the repository; the build output copy only for the folders the
# repository does not have (afterloadtest3, afterloadtest5)
MEMIMAGES_SOURCE = os.path.join(TEST_DIR, 'memimages', 'regressions')
MEMIMAGES_BUILD = os.path.join(TEST_DIR, '..', 'x64', 'Debug', 'memimages', 'regressions')


def memimage_folder(folder):
    source = os.path.join(MEMIMAGES_SOURCE, folder)
    return source if os.path.isdir(source) else os.path.join(MEMIMAGES_BUILD, folder)


SIGNATURE = b'MC2-HD-Recording'
D41A0_SIZE = 0x36E16

# recording -> {level: memimage folder whose frame 0 provides the spells}
SOURCES = {
    'Levels-1-5-Recording.bin': {0: 'afterloadtest3', 1: 'afterloadtest2', 2: 'level3', 3: 'level4', 4: 'level5'},
    'level-1-DosBox-Recording.bin': {0: 'afterloadtest5'},
    'Crash-Level-1.bin': {0: 'level1'},
}
ORIGINALS = {
    'Levels-1-5-Recording.bin': os.path.join(TEST_DIR, 'Levels-1-5-Recording.bin'),
    'level-1-DosBox-Recording.bin': os.path.join(MEMIMAGES_BUILD, 'afterloadtest5', 'level-1-DosBox-Recording.bin'),
    'Crash-Level-1.bin': os.path.join(TEST_DIR, 'Crash-Level-1.bin'),
}


def read_old(data):
    """[(level, [(player, [(turn, bytes)])])] from the layout without spells."""
    assert data[:16] == SIGNATURE
    p, levels = 16, []
    while p < len(data):
        level, count = struct.unpack_from('<HH', data, p)
        p += 4
        players = []
        for _ in range(count):
            player, turns = struct.unpack_from('<HI', data, p)
            p += 6
            records = []
            for _ in range(turns):
                turn, size = struct.unpack_from('<II', data, p)
                p += 8
                records.append((turn, data[p:p + size]))
                p += size
            players.append((player, records))
        levels.append((level, players))
    assert p == len(data), 'not the old layout'
    return levels


def spells_at_frame0(folder, player):
    with open(os.path.join(memimage_folder(folder), 'sequence-002285FF-00356038.bin'), 'rb') as f:
        d41a0 = f.read(D41A0_SIZE)
    str611 = 0x2BDE + player * 0x84C + 0x3E6 + 0x263   # array_0x2BDE[player].dword_0x3E6.str_611
    experience = struct.unpack_from('<26i', d41a0, str611)
    enabled = [1 if v else 0 for v in struct.unpack_from('<26h', d41a0, str611 + 0xD0)]
    indexes = d41a0[str611 + 0x138:str611 + 0x138 + 26]
    levels = d41a0[str611 + 0x1BA:str611 + 0x1BA + 26]
    return enabled, indexes, levels, experience


def write_new(levels, spell_source):
    out = bytearray(SIGNATURE)
    for level, players in levels:
        out += struct.pack('<HH', level, len(players))
        for player, records in players:
            enabled, indexes, spell_levels, experience = spells_at_frame0(spell_source[level], player)
            out += struct.pack('<HI', player, len(records))
            out += struct.pack('<26h', *enabled) + bytes(indexes) + bytes(spell_levels) + struct.pack('<26i', *experience)
            for turn, payload in records:
                out += struct.pack('<II', turn, len(payload)) + payload
    return bytes(out)


def read_new(data):
    p, levels = 16, []
    while p < len(data):
        level, count = struct.unpack_from('<HH', data, p)
        p += 4
        players = []
        for _ in range(count):
            player, turns = struct.unpack_from('<HI', data, p)
            p += 6 + 208
            records = []
            for _ in range(turns):
                turn, size = struct.unpack_from('<II', data, p)
                p += 8
                records.append((turn, data[p:p + size]))
                p += size
            players.append((player, records))
        levels.append((level, players))
    assert p == len(data), 'written file does not parse in the new layout'
    return levels


for name, source in SOURCES.items():
    old = read_old(open(ORIGINALS[name], 'rb').read())
    new_data = write_new(old, source)
    target = os.path.join(HERE, name)
    open(target, 'wb').write(new_data)
    assert read_new(new_data) == old, 'turns differ after conversion'
    summary = ', '.join('L%d<-%s' % (level, source[level]) for level, _ in old)
    print('%-30s %7d -> %7d bytes, turns identical, spells: %s' % (name, os.path.getsize(ORIGINALS[name]), len(new_data), summary))
