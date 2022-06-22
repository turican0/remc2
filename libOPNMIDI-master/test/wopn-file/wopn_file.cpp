#include <catch.hpp>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "wopn/wopn_file.h"

static const char *test_files[] = {
    "fm_banks/xg.wopn",
    "fm_banks/gs-by-papiezak-and-sneakernets.wopn",
};

static WOPNFile *LoadBankFromFile(const char *path);

TEST_CASE("[WOPNFile] Load, Save, Load")
{
    for (int version : {1, 2}) {
        for (const char *test_file : test_files) {
            fprintf(stderr, "--- Test '%s' with version %d\n", test_file, version);

            WOPNFile *wopn = LoadBankFromFile(test_file);
            REQUIRE(wopn != nullptr);

            size_t size = WOPN_CalculateBankFileSize(wopn, version);
            char *mem = new char[size];

            REQUIRE(WOPN_SaveBankToMem(wopn, mem, size, version, 0) == 0);

            {
                int error;
                WOPNFile *wopn2 = WOPN_LoadBankFromMem(mem, size, &error);
                if(!wopn2)
                    fprintf(stderr, "--- Loading error %d\n", error);
                REQUIRE(wopn2);

                if(wopn->version == version)
                    REQUIRE(WOPN_BanksCmp(wopn, wopn2) == 1);
                else {
                    REQUIRE(wopn->banks_count_melodic == wopn2->banks_count_melodic);
                    REQUIRE(wopn->banks_count_percussion == wopn2->banks_count_percussion);
                    REQUIRE(wopn->lfo_freq == wopn2->lfo_freq);
                    REQUIRE(wopn->volume_model == wopn2->volume_model);
                }

                WOPN_Free(wopn2);
            }

            delete[] mem;

            {
                unsigned melo_banks = wopn->banks_count_melodic;
                unsigned drum_banks = wopn->banks_count_percussion;

                for(unsigned Bi = 0; Bi < melo_banks + drum_banks; ++Bi)
                {
                    const WOPNBank &bank = (Bi < melo_banks) ?
                        wopn->banks_melodic[Bi] : wopn->banks_percussive[Bi - melo_banks];

                    for(unsigned Pi = 0; Pi < 128; ++Pi)
                    {
                        OPNIFile opni = {};
                        opni.version = version;
                        opni.is_drum = Bi >= melo_banks;
                        opni.inst = bank.ins[Pi];

                        size = WOPN_CalculateInstFileSize(&opni, version);
                        mem = new char[size];

                        REQUIRE(WOPN_SaveInstToMem(&opni, mem, size, version) == 0);

                        OPNIFile opni2 = {};
                        int error = WOPN_LoadInstFromMem(&opni2, mem, size);
                        if(error != 0)
                            fprintf(stderr, "--- Loading error %d\n", error);
                        REQUIRE(error == 0);

                        if(version <= 2 && opni.inst.delay_on_ms == 0 && opni.inst.delay_off_ms == 0)
                        {
                            // version 2 has no blank flag, but has null delays
                            // problem: cannot deduce a blank program, as OPNI does not save delays
                            opni2.inst.inst_flags |= WOPN_Ins_IsBlank;
                        }

                        size_t compare_size = sizeof(OPNIFile) - 2 * sizeof(uint16_t);
                        REQUIRE(memcmp(&opni, &opni2, compare_size) == 0);

                        delete[] mem;
                    }
                }
            }

            WOPN_Free(wopn);
        }
    }
}

static WOPNFile *LoadBankFromFile(const char *path)
{
    WOPNFile *file = nullptr;
    FILE *fh;
    struct stat st;
    char *mem = nullptr;

    if(!(fh = fopen(path, "rb")) || fstat(fileno(fh), &st) != 0)
        goto fail;

    mem = new char[st.st_size];
    if(fread(mem, st.st_size, 1, fh) != 1)
        goto fail;

    int error;
    file = WOPN_LoadBankFromMem(mem, st.st_size, &error);
    if(!file)
        fprintf(stderr, "--- Loading error %d\n", error);

fail:
    delete[] mem;
    if (fh) fclose(fh);
    return file;
}
