/*WORD __CS__;
WORD __DS__;
WORD __ES__;
WORD __SS__;
WORD __FS__;
WORD __GS__;*/

#include "sub_main.h"
bool _CF;
bool _ZF;
bool _SF;

void myLOBYTE(WORD *exx, BYTE el) {
    *exx=((*exx & 0xff00) + el);
}

void myHIBYTE(WORD *exx, BYTE eh) {
    *exx=((eh << 8) + (*exx & 0xff));
}

void myLOWORD(DWORD *exx, WORD el) {
    *exx = ((*exx & 0xffff0000) + el);
}

void myHIWORD(DWORD *exx, WORD eh) {
    *exx = ((eh << 16) + (*exx & 0xffff));
}

WORD mygetHIWORD(DWORD exx) {
    return ((exx & 0xffff0000)>>16);
}

BYTE BYTE1(DWORD a) {
    return  (a & 0xff);
}

BYTE BYTE2(DWORD a) {
    return  ((a & 0xff00)>>8);
}

DWORD dword_1805AC;
DWORD dword_1805A8;
DWORD dword_1805A4;
DWORD dword_180598;
DWORD dword_180594;
DWORD dword_180590;
DWORD dword_1805A0;
DWORD dword_18059C;
WORD word_1805C8;
WORD word_1805C2;
BYTE byte_E36D1;
BYTE byte_D419C;
BYTE byte_D41AD;
WORD word_1805C4;
WORD word_1805C6;
DWORD dword_E36C4;
WORD word_17FF5A;
DWORD dword_17FF24;
DWORD dword_17FF38;
DWORD dword_17FF44;
DWORD dword_17FF14;
DWORD dword_17FF20;
int* unk_17FF28;
WORD word_1803EA;
WORD word_1803EC;
DWORD dword_17FF40;
WORD word_17FF4A;

int* unk_1803C0;
WORD word_1803C3;
DWORD unk_180484;
char* aErrorCantAlloc_0;

char* aNetwork;
char* aLevel;
char* aSkipscreens;
char* aHarddrive;
char* aNocd;
char* aShowversion;
char* aShowversion2;
char* aDetectoff;
char* aLangcheck;
char* aMavio;
char* aExtern;
char* aVfx1;
char* aScc;
char* aSpellsedit;
char* aMusic2;
char* aExternalCtrlAt;
BYTE IsTable[20];// tom stub
WORD word_E2A24;
DWORD dword_17FF10;
DWORD dword_17FF0C;
WORD word_17FF18;
DWORD dword_E2A6C;
DWORD dword_E2A70;
char* aFdmErrorFreein;
BYTE byte_E2A28;
DWORD unk_18048B;
WORD word_17FF58;
WORD word_17FF56;
DWORD unk_180470;
BYTE byte_1804A1;
BYTE byte_180471;
BYTE byte_18049E;
BYTE byte_180472;
DWORD dword_180486;
BYTE byte_1803EE;
BYTE byte_18048A;
DWORD dword_180084;
DWORD dword_180088;
DWORD dword_180473;
DWORD dword_180214;
DWORD dword_D41A0;
DWORD dword_D41A4;
char* aErrorCantAlloc;
DWORD dword_17ECA0[4608];
BYTE byte_E29E8;
DWORD dword_17FEA0;

int dword_17E0A0[768];
int dword_17E0A4[1];
int dword_17E0A8[1];

DWORD dword_17ECA4;
DWORD dword_17ECAC;
DWORD dword_17ECA8;
BYTE byte_17ECB0;
BYTE byte_17ECB1;
DWORD dword_17FEA4;
DWORD dword_17FEA8;
DWORD dword_17FEAC;
DWORD dword_17FEB0;
BYTE byte_E379B;

char* aCopyrightC1995;
char* aAllRightsReser;
char* aS_0;

BYTE byte_D4B80;
DWORD dword_E3768;
DWORD dword_D4198;
BYTE byte_E1274;
WORD word_E127A;
DWORD dword_E12AE;
DWORD dword_E12AA;
DWORD dword_E1286;
DWORD dword_E127E;
DWORD dword_E1282;
BYTE byte_D4B51;
DWORD dword_F42A4;
DWORD dword_181C04;
DWORD dword_181BF0;
DWORD dword_181BF4;
DWORD dword_181BF8;
char* aAilReleaseTime;
DWORD dword_E3E9C;
DWORD dword_E3FF2;
BYTE byte_DB734;
BYTE byte_E37FE;
BYTE byte_E3798;
BYTE byte_E3799;
BYTE byte_E379A;
BYTE byte_E37FC;
BYTE byte_E37FD;
BYTE byte_E3815;
BYTE byte_E39B8;
DWORD dword_180C78;
DWORD dword_E37A0;
DWORD dword_E37A4;
DWORD dword_E37A8;
DWORD dword_E3808;
DWORD dword_E380C;
DWORD dword_E3810;
DWORD dword_181BFC;
DWORD dword_181C08;
DWORD dword_181C0C;
DWORD dword_181C10;
DWORD dword_E3FEE;
char* a02u02u02u02u;
DWORD unk_D226C;
WORD word_E3802;
DWORD unk_D2270;
char* aAilStopSequenc;
char* aAilBranchIndex;
DWORD dword_181DE8;
DWORD dword_E4A94;
WORD word_180662;
WORD word_180B30;
WORD word_E39A8;
WORD word_E39AA;
WORD word_E39AC;
WORD word_E39AE;
WORD word_E39B0;
WORD word_E39B2;
WORD word_E39B4;
WORD word_E39B6;
char* aAilEndSequence;

char* aAilMapSequence;


char* aAilCallDriver0;

char* aAilReleaseChan;
char* aResultD;

char* aAilDelayD;

int loc_A0000;

char unk_180BE0;
char* aSbawe32Mdi_1;
char byte_180C84;
char* aW;

DWORD dword_180C7C;
DWORD dword_181E2C;
WORD word_181E30;
DWORD dword_181E26;
WORD word_181E2A;
BYTE byte_E3E54;
char* aAilShutdown;

DWORD dword_E3E3C;
DWORD dword_181C50;
DWORD dword_181DAC;
BYTE byte_181C90;
DWORD unk_181D90;
DWORD dword_E3E38;
DWORD dword_181DA8;
char* aAilUninstallDr;
char* aAilRestoreUse1;
DWORD dword_E3FF8;
char* aAilSetRealVect;
char* aAilReleaseAllT;
DWORD dword_E3FE6;
char* aAilLockChannel;
char* aAilEndSample0x;
DWORD dword_180B4C;

BYTE byte_E388D;
DWORD dword_180CA0;
DWORD unk_180750;
BYTE byte_E3E44;
BYTE byte_E3E45;
DWORD dword_181E10;
char* aAilSetSampleUs;

int aSearchd;
int aDataBuild00Dat;
int aWscreen;
DWORD unk_180452;
WORD word_180455;




signed int sub_89B60(unsigned __int8 a1)
{
    signed int v1; // esi
    __int16 v2; // ax
    BOOL v3; // zf

    v1 = -1;
    dword_1805AC = 0;
    dword_1805A8 = 0;
    dword_1805A4 = 0;
    dword_180598 = 0;
    dword_180594 = 0;
    dword_180590 = 0;
    dword_1805A0 = 0;
    dword_18059C = 0;
    word_1805C8 = 0;


    switch (a1)
    {
    /*case 1u:
        v1 = sub_8B600((int)&unk_18058C);
        if ((signed __int16)v1 != -1)
            goto LABEL_3;
        break;
    case 2u:
        if ((unsigned __int16)sub_75650())
        {
            v1 = 1;
            printf(aVfxInitialised);
            word_1805C8 = 1;
            v2 = a1;
            dword_1805A0 = 0;
            dword_18059C = 0;
            goto LABEL_12;
        }
        break;
    case 4u:
        memset(&unk_180560, 0, 44);
        sub_8BB40((int)&unk_180560, 8);
        v3 = byte_180584 == 0;
        goto LABEL_10;
    case 6u:
        memset(&unk_180560, 0, 44);
        v1 = 1;
        v2 = a1;
        goto LABEL_12;*/ //tom stub
    case 7u:
        v2 = a1;
        v1 = 1;
        goto LABEL_12;
    /*case 8u:
    case 9u:
    LABEL_3:
        word_1805C8 = 1;
        break;
    case 0xBu:
        memset(&unk_180560, 0, 44);
        sub_8BB40((int)&unk_180560, 4);
        v3 = byte_180584 == 0;*/ //tom stub
    LABEL_10:
        if (!v3)
        {
            /*sub_8BBE0((int)&unk_180560);
            v1 = 1;
            v2 = a1;*/ //tom stub
        LABEL_12:
            word_1805C2 = v2;
        }
        break;
    default:
        break;
    }
    word_1805C2 = a1;
    if ((signed __int16)v1 > 0)
        byte_E36D1 = a1;
    return v1;
}

// Watcom v9-*1.5  32bit DOS runtime
int unknown_libname_1(char *a1)
{
    char *i; // eax
    char v2; // cl
    int v3; // ebx
    int v4; // ebx

    for (i = a1; IsTable[(unsigned __int8)(*i + 1)] & 2; ++i)
        ;
    v2 = *i;
    if (*i == 43 || v2 == 45)
        ++i;
    v3 = 0;
    while (IsTable[(unsigned __int8)(*i + 1)] & 0x20)
    {
        v4 = (unsigned __int8)*i++ + 10 * v3;
        v3 = v4 - 48;
    }
    if (v2 == 45)
        v3 = -v3;
    return v3;
}

int __cdecl sub_98FF5(char *a1)
{
    char *i; // eax
    char v2; // cl
    int v3; // ebx
    int v4; // ebx

    for (i = a1; IsTable[(unsigned __int8)(*i + 1)] & 2; ++i)
        ;
    v2 = *i;
    if (*i == 43 || v2 == 45)
        ++i;
    v3 = 0;
    while (IsTable[(unsigned __int8)(*i + 1)] & 0x20)
    {
        v4 = (unsigned __int8)*i++ + 10 * v3;
        v3 = v4 - 48;
    }
    if (v2 == 45)
        v3 = -v3;
    return v3;
}

int int386(int intno, union REGS *inregs, union REGS *outregs)
{
    //int 31,1 - nic

    /*struct SREGS    segregs;

    segread(&segregs);
    return(int386x(intno, inregs, outregs, &segregs));*/ //tom stub
    return 0;
}

signed __int16 sub_85E40()
{
    signed __int16 result; // ax

    if (word_E2A24)
        return 1;
    dword_17FF10 = 4096;
    dword_17FF0C = 256;
    int386(49, (union REGS *)(int)&dword_17FF0C, (union REGS *)(int)&dword_17FF0C);
    word_E2A24 = dword_17FF0C;
    word_17FF5A = word_17FF18;
    myLOBYTE((WORD*)&result,(dword_17FF24 == 0));
    //LOBYTE(result) = dword_17FF24 == 0;
    myHIBYTE((WORD*)&result, 0);
    //HIBYTE(result) = 0;
    return result;
}

__int16 __cdecl sub_85EB0(__int16 a1)
{
    __int16 result; // ax

    myLOWORD(&dword_17FF10,a1);
    //LOWORD(dword_17FF10) = a1;
    myLOWORD(&dword_17FF0C, 256);
    //LOWORD(dword_17FF0C) = 256;
    int386(49, (union REGS *)(int)&dword_17FF0C, (union REGS *)(int)&dword_17FF0C);
    if (dword_17FF24)
        result = 0;
    else
        result = dword_17FF0C;
    return result;
}

bool sub_85FD0()
{
    __int16 v0; // ax

    dword_E2A6C = sub_85EB0(2);
    v0 = sub_85EB0(256);
    dword_E2A70 = v0;
    return dword_E2A6C && v0;
}

int sub_86010()
{
    dword_17FF38 = 0;
    dword_17FF44 = 5376;
    dword_17FF0C = 768;
    dword_17FF10 = 47;
    dword_17FF14 = 0;
    dword_17FF20 = (int)&unk_17FF28;
    int386(49, (union REGS *)(int)&dword_17FF0C, (union REGS *)(int)&dword_17FF0C);
    word_1803EA = dword_17FF38;
    word_1803EC = dword_17FF40;
    return dword_17FF38;
}

int sub_86550()
{
    dword_17FF38 = 0;
    dword_17FF44 = 5388;
    dword_17FF0C = 768;
    dword_17FF10 = 47;
    dword_17FF14 = 0;
    dword_17FF20 = (int)&unk_17FF28;
    int386(49, (union REGS *)(int)&dword_17FF0C, (union REGS *)(int)&dword_17FF0C);
    return dword_17FF38;
}

__int16 __cdecl sub_85F00(__int16 a1)
{
    __int16 result; // ax

    word_17FF18 = a1;
    myLOWORD(&dword_17FF0C,256);
    int386(49, (union REGS *)(int)&dword_17FF0C, (union REGS *)(int)&dword_17FF0C);
    if (dword_17FF24)
        printf(aFdmErrorFreein);
    myLOBYTE((WORD*)&result,(dword_17FF24 == 0));
    myHIBYTE((WORD*)&result,0);
    return result;
}

char sub_86BD0()
{
    char result; // al

    if (dword_E2A6C)
        result = sub_85F00(dword_E2A6C);
    if (dword_E2A70)
        result = sub_85F00(dword_E2A70);
    byte_E2A28 = 0;
    dword_E2A6C = 0;
    dword_E2A70 = 0;
    return result;
}

void qmemcpy(int** a,char* b,unsigned int c) {//tom stub
}

__int16 __cdecl sub_86370(unsigned __int16 a1, char a2)
{
    int v2; // ecx
    __int16 result; // ax
    char *v4; // esi
    int v5; // ebx

    if (!dword_E2A6C)
        return 0;
    v2 = dword_E2A70;
    if (!dword_E2A70)
        return 0;
    v4 = (char *)(16 * dword_E2A6C);
    *v4 = 26;
    v4[1] = 0;
    v4[2] = 3;
    *(WORD *)(v4 + 3) = 0;
    v4[13] = 0;
    *((WORD *)v4 + 9) = 7;
    *((WORD *)v4 + 10) = 0;
    *(DWORD *)(v4 + 22) = 0;
    v5 = 16 * v2;
    *(DWORD *)(v4 + 14) = v2 << 16;
    *(BYTE *)v5 = 11;
    *(BYTE *)(v5 + 1) = a2;
    dword_17FF38 = 0;
    dword_17FF10 = 47;
    word_17FF4A = dword_E2A6C;
    dword_17FF14 = 0;
    dword_17FF40 = a1;
    dword_17FF44 = 5392;
    dword_17FF0C = 768;
    dword_17FF20 = (int)&unk_17FF28;
    int386(49, (union REGS *)(int)&dword_17FF0C, (union REGS *)(int)&dword_17FF0C);
    qmemcpy(&unk_1803C0, v4, 0x1Au);
    result = word_1803C3;
    unk_180484 = *(DWORD *)v5;
    *((WORD *)&unk_180484 + 2) = *(WORD *)(v5 + 4);
    *((BYTE *)&unk_180484 + 6) = *(BYTE *)(v5 + 6);
    return result;
}

int __cdecl sub_85F60(int a1)
{
    return 75 * BYTE1(a1) + 4500 * BYTE2(a1) + (unsigned __int8)a1;
}

__int16 __cdecl sub_86460(unsigned __int16 a1)
{
    int v1; // ecx
    __int16 result; // ax
    char *v3; // esi
    int v4; // ebx
    __int16 v5; // ax

    if (!dword_E2A6C)
        return 0;
    v1 = dword_E2A70;
    if (!dword_E2A70)
        return 0;
    v3 = (char *)(16 * dword_E2A6C);
    *v3 = 26;
    v3[1] = 0;
    v3[2] = 3;
    *(WORD *)(v3 + 3) = 0;
    v3[13] = 0;
    *((WORD *)v3 + 9) = 11;
    *((WORD *)v3 + 10) = 0;
    *(DWORD *)(v3 + 22) = 0;
    v4 = 16 * v1;
    *(DWORD *)(v3 + 14) = v1 << 16;
    v5 = dword_E2A6C;
    *(BYTE *)(16 * v1) = 12;
    word_17FF4A = v5;
    dword_17FF38 = 0;
    dword_17FF10 = 47;
    dword_17FF14 = 0;
    dword_17FF40 = a1;
    dword_17FF44 = 5392;
    dword_17FF0C = 768;
    dword_17FF20 = (int)&unk_17FF28;
    int386(49, (union REGS *)(int)&dword_17FF0C, (union REGS *)(int)&dword_17FF0C);
    qmemcpy(&unk_1803C0, v3, 0x1Au);
    result = word_1803C3;
    unk_18048B = *(DWORD *)v4;
    *((DWORD *)&unk_18048B + 1) = *(DWORD *)(v4 + 4);
    *((WORD *)&unk_18048B + 4) = *(WORD *)(v4 + 8);
    *((BYTE *)&unk_18048B + 10) = *(BYTE *)(v4 + 10);
    return result;
}

__int16 __cdecl sub_86270(unsigned __int16 a1)
{
    int v1; // ecx
    __int16 result; // ax
    char *v3; // esi
    int v4; // ebx

    if (!dword_E2A6C)
        return 0;
    v1 = dword_E2A70;
    if (!dword_E2A70)
        return 0;
    v3 = (char *)(16 * dword_E2A6C);
    *v3 = 26;
    v3[1] = 0;
    v3[2] = 3;
    *(WORD *)(v3 + 3) = 0;
    v3[13] = 0;
    *((WORD *)v3 + 9) = 7;
    *((WORD *)v3 + 10) = 0;
    *(DWORD *)(v3 + 22) = 0;
    v4 = 16 * v1;
    *(DWORD *)(v3 + 14) = v1 << 16;
    *(BYTE *)(16 * v1) = 10;
    word_17FF58 = 0;
    word_17FF56 = 0;
    word_17FF4A = dword_E2A6C;
    dword_17FF38 = 0;
    dword_17FF14 = 0;
    dword_17FF10 = 47;
    dword_17FF40 = a1;
    dword_17FF44 = 5392;
    dword_17FF0C = 768;
    dword_17FF20 = (int)&unk_17FF28;
    int386(49, (union REGS *)(int)&dword_17FF0C, (union REGS *)(int)&dword_17FF0C);
    qmemcpy(&unk_1803C0, v3, 0x1Au);
    result = word_1803C3;
    unk_180470 = *(DWORD *)v4;
    *((WORD *)&unk_180470 + 2) = *(WORD *)(v4 + 4);
    *((BYTE *)&unk_180470 + 6) = *(BYTE *)(v4 + 6);
    return result;
}

#if defined(__WIN32__) || defined(_WIN32) || defined(WIN32) || defined(__WINDOWS__) || defined(__TOS_WIN__)

#include <windows.h>

inline void j___delay(unsigned long ms)
{
    Sleep(ms);
}

#else  /* presume POSIX */

#include <unistd.h>

inline void j___delay(unsigned long ms)
{
    usleep(ms * 1000);
}

#endif 

BOOL sub_86A00()
{
    BOOL v0; // ax
    int v1; // ebx
    unsigned __int16 v2; // bx
    int v3; // esi
    int v4; // ST04_4
    signed __int16 v5; // bx
    unsigned __int16 i; // ax
    int v7; // edi
    unsigned __int16 v8; // bx
    int v9; // eax
    unsigned __int16 v10; // bx
    int v11; // esi
    int v12; // eax
    unsigned __int16 v13; // ax
    int v14; // edx
    int v15; // ecx

    sub_85E40();
    v0 = sub_85FD0();
    if (v0)
    {
        if (sub_86010())
        {
            sub_86550();
            v1 = 0;
            while ((((unsigned __int16)sub_86270(word_1803EC) >> 8) & 0x80u) != 0)
            {
                j___delay(1000);
                if ((WORD)++v1 == 4)
                {
                    myLOBYTE((WORD*)&v0,sub_86BD0());
                    return v0;
                }
            }
            byte_1804A1 = byte_180471;
            v2 = (unsigned __int8)byte_180471;
            byte_18049E = byte_180472;
            while (v2 <= (signed int)(unsigned __int8)byte_180472)
            {
                sub_86370(word_1803EC, v2);
                v3 = v2;
                v4 = dword_180486;
                byte_1803EE[(BYTE*)v2++] = (byte_18048A & 0x40) == 0;
                dword_180084[(DWORD*)v3] = sub_85F60(v4);
            }
            v5 = 0;
            for (i = (unsigned __int8)byte_180471; i <= (signed int)(unsigned __int8)byte_180472; ++i)
            {
                if (byte_1803EE[(BYTE*)i])
                    v5 = 1;
            }
            if (v5)
            {
                v7 = dword_180088[(DWORD*)0];
                v8 = (unsigned __int8)byte_180471;
                while (1)
                {
                    v9 = v8;
                    if (v8 > (signed int)(unsigned __int8)byte_180472)
                        break;
                    ++v8;
                    dword_180084[(DWORD*)v9] -= v7;
                }
                v10 = (unsigned __int8)byte_180471;
                while (v10 <= (signed int)(unsigned __int8)byte_180472)
                {
                    v11 = v10;
                    if (v10 == (unsigned __int8)byte_180472)
                        v12 = sub_85F60(dword_180473) - v7;
                    else
                        v12 = dword_180088[(DWORD*)v11];
                    dword_180214[(DWORD*)v11] = v12;
                    v13 = v10;
                    v14 = dword_180084[(DWORD*)v10];
                    v15 = dword_180214[(DWORD*)v10++];
                    dword_180214[(DWORD*)v13] = v15 - v14;
                }
                myLOBYTE((WORD*)&v0,sub_86460(word_1803EC));
                byte_E2A28 = 1;
            }
            else
            {
                myLOBYTE((WORD*)&v0,sub_86BD0());
            }
        }
        else
        {
            myLOBYTE((WORD*)&v0,sub_86BD0());
        }
    }
    return v0;
}
/*
BOOL sub_86A00()
{
    BOOL v0; // ax
    int v1; // ebx
    unsigned __int16 v2; // bx
    int v3; // esi
    int v4; // ST04_4
    signed __int16 v5; // bx
    unsigned __int16 i; // ax
    int v7; // edi
    unsigned __int16 v8; // bx
    int v9; // eax
    unsigned __int16 v10; // bx
    int v11; // esi
    int v12; // eax
    unsigned __int16 v13; // ax
    int v14; // edx
    int v15; // ecx

    sub_85E40();
    v0 = sub_85FD0();
    if (v0)
    {
        if (sub_86010())
        {
            sub_86550();
            v1 = 0;
            while ((((unsigned __int16)sub_86270(word_1803EC) >> 8) & 0x80u) != 0)
            {
                j___delay(1000);
                if ((WORD)++v1 == 4)
                {
                    myLOBYTE((DWORD*)&v0,sub_86BD0());
                    return v0;
                }
            }
            byte_1804A1 = byte_180471;
            v2 = (unsigned __int8)byte_180471;
            byte_18049E = byte_180472;
            while (v2 <= (signed int)(unsigned __int8)byte_180472)
            {
                sub_86370(word_1803EC, v2);
                v3 = v2;
                v4 = dword_180486;
                byte_1803EE[(BYTE*)v2++] = (byte_18048A & 0x40) == 0;
                dword_180084[(DWORD*)v3] = sub_85F60(v4);
            }
            v5 = 0;
            for (i = (unsigned __int8)byte_180471; i <= (signed int)(unsigned __int8)byte_180472; ++i)
            {
                if (byte_1803EE[(BYTE*)i])
                    v5 = 1;
            }
            if (v5)
            {
                v7 = dword_180088[(DWORD*)0];
                v8 = (unsigned __int8)byte_180471;
                while (1)
                {
                    v9 = v8;
                    if (v8 > (signed int)(unsigned __int8)byte_180472)
                        break;
                    ++v8;
                    dword_180084[(DWORD*)v9] -= v7;
                }
                v10 = (unsigned __int8)byte_180471;
                while (v10 <= (signed int)(unsigned __int8)byte_180472)
                {
                    v11 = v10;
                    if (v10 == (unsigned __int8)byte_180472)
                        v12 = sub_85F60(dword_180473) - v7;
                    else
                        v12 = dword_180088[(DWORD*)v11];
                    dword_180214[(DWORD*)v11] = v12;
                    v13 = v10;
                    v14 = dword_180084[(DWORD*)v10];
                    v15 = dword_180214[(DWORD*)v10++];
                    dword_180214[(DWORD*)v13] = v15 - v14;
                }
                myLOBYTE((DWORD*)&v0,sub_86460(word_1803EC));
                byte_E2A28 = 1;
            }
            else
            {
                myLOBYTE((DWORD*)&v0,sub_86BD0());
            }
        }
        else
        {
            myLOBYTE((DWORD*)&v0,sub_86BD0());
        }
    }
    return v0;
}*/

char sub_83E00(int a1, int a2)
{
    int *i; // eax
    char v4; // bl
    int v5; // edi

    if (a2 == *(DWORD *)(a1 + 4))
    {
        *(BYTE *)(a1 + 16) = byte_E29E8;
    }
    else
    {
        for (i = (int*)&dword_17ECA0; i < (int*)&dword_17FEA0 && i[1]; i = (int *)((char *)i + 18))
            ;
        if (i == (int*)&dword_17FEA0)
            return 0;
        i[1] = *(DWORD *)(a1 + 4) - a2;
        *i = a2 + *(DWORD *)a1;
        v4 = byte_E29E8;
        *(DWORD *)(a1 + 4) = a2;
        *(BYTE *)(a1 + 16) = v4;
        *((BYTE *)i + 17) = *(BYTE *)(a1 + 17);
        i[2] = *(DWORD *)(a1 + 8);
        i[3] = a1;
        v5 = *(DWORD *)(a1 + 8);
        if (v5)
            *(DWORD *)(v5 + 12) = (DWORD)i;
        *(DWORD *)(a1 + 8) = (DWORD)i;
    }
    return 1;
}

bool expand(DWORD* array, int newLength)//tom stub banana ba na na
{
    //int * array = (int*)malloc(sizeof(int) * arrayLength);
    array = (DWORD*)realloc(array, sizeof(int) * newLength);
    return true;
}

int __cdecl sub_85450(unsigned int a1)
{
    int v1; // ebx
    int v3; // [esp+0h] [ebp-38h]
    unsigned int v4; // [esp+4h] [ebp-34h]
    int v5; // [esp+1Ch] [ebp-1Ch]
    //int v6; // [esp+28h] [ebp-10h]
    int v6=0; // [esp+28h] [ebp-10h] // tom stub

    //int v7; // [esp+34h] [ebp-4h]
    int v7=0; // [esp+34h] [ebp-4h] // tom stub
    v1 = 0;
    if (a1 < (unsigned int)&loc_A0000)
    {
        v4 = (a1 + 15) >> 4;
        v3 = 256;
        int386(49, (union REGS *)(int)&v3, (union REGS *)(int)&v5);
        if (!v7)
            v1 = v5 | (v6 << 16);
    }
    return v1;
}

void sub_10000()
{
    while (1)
        __debugbreak();
}

int sub_85060(int a1)
{
    return *(DWORD *)(a1 + 4);
}

int sub_9AD16(unsigned __int16 a1)
{
    int result; // eax
    char v2; // [esp+0h] [ebp-38h]
    int v3; // [esp+1Ch] [ebp-1Ch]
    int v4; // [esp+28h] [ebp-10h]

    if (a1)
    {
        v3 = 257;
        v4 = a1;
        result = int386(49, (union REGS *)(int)&v3, (union REGS *)(int)&v2);
    }
    return result;
}

void sub_85070()//266070
{
    signed int v0; // ebx
    int *v1; // ST04_4
    signed int v2; // ebx
    char *v3; // ST04_4
    char *v4; // ebx
    int v5; // esi
    unsigned int v6; // eax
    int v7; // edi
    int v8; // edx
    int v9; // eax
    signed int i; // ebx
    int v11; // eax
    signed int j; // edi
    int k; // ebx
    int v14; // edx
    int v15; // eax
    signed int v16; // ebx
    int v17; // eax
    int l; // ebx
    int v19; // edx
    int v20; // eax
    int v21; // [esp+0h] [ebp-Ch]
    int v22; // [esp+8h] [ebp-4h]

    if (!dword_17E0A8[0])//dword_17E0A8 //266079
    {
        v0 = 0;
        v21 = sub_85450((unsigned int)sub_10000);//26608B
        //ebx=0
        //esp(35514C)=esp(355148+4)
        //mov [ebp(355158)-c]=0
        do
        {
            v1 = dword_17E0A0+(3 * v0++);
            memset(v1, 0, 12);//eax=esp+4,edx=esp+8,ecx=esp+c //16E0AC
        } while (v0 < 256);//v0 - ebx
        v2 = 0;
        while (v2 < 256)
        {
            v3 = (char*)(dword_17ECA0 + (18 * v2++));
            memset(v3, 0, 18);
        }
        v4 = (char *)&loc_A0000;
        v5 = 0;
        while ((signed int)v4 >= 1024)
        {
            v6 = sub_85450((unsigned int)v4);
            if (v6)
            {
                v7 = (unsigned __int16)v6;
                v8 = v6 >> 16;
                v9 = 3 * v5;
                dword_17E0A8[v9] = (int)v4;//dword_17E0A8 //tom error
                ++v5;
                dword_17E0A4[v9] = v8;//dword_17E0A4
                v4 += 1024;
                dword_17E0A0[v9] = 16 * v7;
            }
            v4 -= 1024;
        }
        for (i = 0x1000000; i >= 4096; i -= 4096)
        {
            v11 = (int)malloc(i);
            dword_17E0A0[(3 * v5)] = v11;
            if (v11)
            {
                for (j = 4096; j > 0; j -= 16)
                {
                    v22 = 3 * v5;
                    if (expand((DWORD*)dword_17E0A0[(3 * v5)], i + j))
                    {
                        dword_17E0A8[v22] = i + j;//dword_17E0A8
                        dword_17E0A4[v22] = 0;//dword_17E0A4
                        break;
                    }
                }
                i += 4096;
                ++v5;
            }
        }
        for (k = 4096; k >= 16; k -= 16)
        {
            v14 = (int)malloc(k);
            v15 = 3 * v5;
            dword_17E0A0[(3 * v5)] = v14;
            if (v14)
            {
                ++v5;
                dword_17E0A8[v15] = k;//dword_17E0A8
                k += 4096;
                dword_17E0A4[v15] = 0;//dword_17E0A4
            }
        }
        v16 = 0;
        //void qsort(void *base, size_t nmemb, size_t size,
        //int(*compare)(const void *, const void *)) {
        qsort((void*)dword_17E0A0, v5, 12, (int(__cdecl *)(const void *, const void *))sub_85060);
        do
        {
            v17 = 9 * v16++;
            *(int *)((char *)&dword_17ECA4 + 2 * v17) = 0;
        } while (v16 < 256);
        for (l = 0; l < v5; ++l)
        {
            v19 = 18 * l;
            *(int *)((char *)&dword_17ECA0 + v19) = dword_17E0A0[(3 * l)];
            *(int *)((char *)&dword_17ECA4 + v19) = dword_17E0A0[2+(3 * l)];//dword_17E0A8
            if (l)
                *(int *)((char *)&dword_17ECAC + v19) = (int)&dword_17ECA0 + 18 * (l - 1);
            else
                *(int *)((char *)&dword_17ECAC + v19) = 0;
            v20 = 9 * l;
            *(int *)((char *)&dword_17ECA8 + 2 * v20) = (int)&dword_17ECA0 + 18 * (l + 1);
            byte_17ECB0[(BYTE*)(2 * v20)] = 0;
            byte_17ECB1[(BYTE*)(2 * v20)] = l;
        }
        *(int *)((char *)&dword_17ECA8 + 18 * (v5 - 1)) = 0;
        sub_9AD16(mygetHIWORD(v21));
    }
}

int *sub_85350()
{
    int *result; // eax
    unsigned int v1; // edx
    int v2; // edx

    result = (int*)&dword_17ECA0;
    dword_17FEA0 = 0;
    dword_17FEA4 = 0;
    dword_17FEA8 = 0;
    dword_17FEAC = 0;
    dword_17FEB0 = -1;
    while (result)
    {
        if (*((BYTE *)result + 16))
        {
            v2 = result[1];
            dword_17FEA8 += v2;
            dword_17FEA0 += v2;
        }
        else
        {
            v1 = result[1];
            dword_17FEA4 += v1;
            if (v1 > dword_17FEAC)
                dword_17FEAC = v1;
            if (result[1] < (unsigned int)dword_17FEB0)
                dword_17FEB0 = result[1];
            dword_17FEA0 += result[1];
        }
        result = (int *)result[2];
    }
    myLOBYTE((WORD*)&dword_17FEA0,dword_17FEA0 & 0xFC);
    myLOBYTE((WORD*)&dword_17FEA4,dword_17FEA4 & 0xFC);
    myLOBYTE((WORD*)&dword_17FEAC,dword_17FEAC & 0xFC);
    myLOBYTE((WORD*)&dword_17FEA8,dword_17FEA8 & 0xFC);
    myLOBYTE((WORD*)&dword_17FEB0,dword_17FEB0 & 0xFC);
    return result;
}

int sub_83D70(int a1)
{
    int *v1; // eax
    unsigned int v2; // edi
    int *v3; // ebx
    unsigned int v4; // esi
    unsigned int v5; // edx

    sub_85070();
    v1 = (int*)&dword_17ECA0;
    v2 = -1;
    v4 = a1 + 271;
    v3 = 0;
    myLOWORD((DWORD*)&v4,((a1 + 271) & 0xFFF0));
    while (v1)
    {
        v5 = v1[1];
        if (v4 <= v5 && v2 > v5 && !*((BYTE *)v1 + 16) && dword_17E0A4[(3 * *((unsigned __int8 *)v1 + 17))])//dword_17E0A4
        {
            v3 = v1;
            v2 = v1[1];
        }
        v1 = (int *)v1[2];
    }
    if (!v3 || !sub_83E00((int)v3, v4))
        return 0;
    sub_85350();
    memset((void*)*v3, 0, v4);//tom proverit
    return *v3;
}

int sub_83CD0(int a1)
{
    int *v1; // eax
    unsigned int v2; // ecx
    int *v3; // ebx
    unsigned int v4; // esi
    unsigned int v5; // edx

    sub_85070();
    v1 = (int*)&dword_17ECA0;
    v2 = -1;
    v4 = a1 + 259;
    v3 = 0;
    myLOWORD((DWORD*)&v4,((a1 + 259) & 0xFFFC));
    while (v1)
    {
        v5 = v1[1];
        if (v4 <= v5 && v2 > v5 && !*((BYTE *)v1 + 16) && !dword_17E0A4[(3 * *((unsigned __int8 *)v1 + 17))])//dword_17E0A4
        {
            v3 = v1;
            v2 = v1[1];
        }
        v1 = (int *)v1[2];
    }
    if (!v3 || !sub_83E00((int)v3, v4))
        return sub_83D70(v4);
    sub_85350();
    memset((void*)*v3, 0, v4);
    return *v3;
}

int sub_84000(int a1)
{
    int result; // eax
    int v2; // edx
    int v3; // ebx

    result = a1;
    if (*(DWORD *)(a1 + 12))
    {
        *(BYTE *)(a1 + 16) = 0;
        v2 = *(DWORD *)(a1 + 12);
        if (*(BYTE *)(a1 + 17) == *(BYTE *)(v2 + 17) && !*(BYTE *)(v2 + 16))
        {
            v3 = *(DWORD *)(a1 + 8);
            if (v3)
                *(DWORD *)(v3 + 12) = v2;
            *(DWORD *)(*(DWORD *)(a1 + 12) + 8) = *(DWORD *)(a1 + 8);
            *(DWORD *)(*(DWORD *)(a1 + 12) + 4) += *(DWORD *)(a1 + 4);
            *(DWORD *)(a1 + 4) = 0;
        }
    }
    return result;
}

void sub_83E80(int a1)
{
    int *v1; // eax
    char v2; // bl
    int *i; // ebx

    if (a1)
    {
        v1 = (int*)&dword_17ECA0;
        v2 = 0;
        while (v1)
        {
            if (a1 == *v1)
            {
                v2 = 1;
                *((BYTE *)v1 + 16) = 0;
                break;
            }
            v1 = (int *)v1[2];
        }
        if (v2 == 1)
        {
            for (i = (int*)&dword_17ECA0; i; i = (int *)i[2])
            {
                if (!*((BYTE *)i + 16))
                    sub_84000((int)i);
            }
        }
        sub_85350();
    }
}

void sub_99080(char a1)
{
    byte_E379B = a1 != 0;
}

int sub_70890()
{
    printf(aCopyrightC1995);
    printf(aAllRightsReser);
    return printf(aS_0);
}

void sub_72D04()
{
    signed int i; // [esp+0h] [ebp-8h]
    signed int j; // [esp+4h] [ebp-4h]

    if (byte_E1274)
    {
        for (i = 0; word_E127A > i; ++i)
        {
            while (*(BYTE *)(dword_E12AE[(DWORD*)i] + 49) == -1)
                ;
        }
        while (*(BYTE *)(dword_E12AA + 49) == -1)
            ;
        sub_83E80(dword_E12AA);
        for (j = 0; j < 8; ++j)
        {
            sub_83E80(dword_E12AE[(DWORD*)j]);
            sub_83E80(dword_E1286[(DWORD*)j]);
        }
        sub_83E80(dword_E127E);
        sub_83E80(dword_E1282);
        byte_E1274 = 0;
    }
}

void sub_5C450()
{
    if (*(WORD *)(dword_D41A4 + 216))
        sub_72D04();
}

int sub_54600()
{
    int result; // eax
    char v1; // [esp+0h] [ebp-38h]
    __int16 v2; // [esp+1Ch] [ebp-1Ch]

    v2 = 0;
    result = int386(51, (union REGS *)(int)&v2, (union REGS *)(int)&v1);
    dword_E3768 = 0;
    return result;
}

void sub_8C2DE()
{
    ;
}

int sub_75420()
{
    //return dos_setvect(9, dword_17D430, (unsigned __int16)word_17D434); // tom stub set interupt vector
    return 0;
}

void sub_A0EEC()
{
    ++dword_E3FF2;
}

void sub_91BD0()
{
    sub_A0EEC();
}

void sub_A0EF9()
{
    --dword_E3FF2;
}

void sub_91BF0()
{
    sub_A0EF9();
}

void __cdecl sub_A171D(int a1)
{
    sub_91BD0();
    if (a1 != -1)
        *(int *)((char *)dword_E3E9C + a1) = 0;
    sub_91BF0();
}

int sub_A16A2()
{
    return dword_E3FEE;
}

signed int sub_916F0()
{
    unsigned int i; // [esp+4h] [ebp-14h]
    unsigned int j; // [esp+4h] [ebp-14h]
    int v3; // [esp+8h] [ebp-10h]
    int v4; // [esp+Ch] [ebp-Ch]
    int v5; // [esp+10h] [ebp-8h]
    int v6; // [esp+14h] [ebp-4h]

    sub_A0EEC();
    v6 = dword_181BFC;
    v5 = dword_181C0C;
    v4 = dword_181C08;
    v3 = dword_181C10;
    sub_A0EF9();
    if (dword_181C04 == 1)
    {
        fprintf((FILE*)dword_181BF0, a02u02u02u02u, v6, v5, v4, v3);
    }
    else
    {
        for (i = 0; i < 0xE; ++i)
            fprintf((FILE*)dword_181BF0, (const char *)&unk_D226C);
    }
    for (j = 1; j < dword_181C04; ++j)
        fprintf((FILE*)dword_181BF0, (const char *)&unk_D2270);
    return 1;
}

void sub_92DC0(int a1)
{
    bool v1; // [esp+0h] [ebp-4h]

    ++dword_181C04;
    v1 = dword_181BF4 && (dword_181C04 == 1 || dword_181BF8) && !sub_A16A2() && sub_916F0();
    if (v1)
        fprintf((FILE*)dword_181BF0, aAilReleaseTime, a1);
    sub_A171D(a1);
    --dword_181C04;
}

void __outbyte(unsigned short Port,unsigned char Data)//tom stub out port
{

}

unsigned char __inbyte(unsigned short Port)//tom stub in port
{
    unsigned char oubyte;
    return oubyte;
}

void sub_6FE20()
{
    int v0; // et1
    int v1; // eax

    if (byte_DB734 == 1)
    {
        v0 = *(DWORD *)(dword_D41A4 + 2380);
        __outbyte(0x43u, 0x36u);
        __outbyte(0x40u, 0);
        __outbyte(0x40u, 0);
        /*v1 = dos_setvect(8, dword_F5330, (unsigned __int16)word_F5334); // tom stub dos vector
        _wcpp_1_unwind_leave__130(v1);*/
    }
    byte_DB734 = 0;
}

DWORD * sub_A5530(int a1, char a2, unsigned int a3, unsigned __int8 a4)
{
    DWORD *result; // eax
    int v5; // [esp+0h] [ebp-8h]
    unsigned int v6; // [esp+4h] [ebp-4h]

    v6 = a2 & 0xF0;
    result = (DWORD *)(a2 & 0xF);
    v5 = a2 & 0xF;
    if (v6 < 0xC0)
    {
        if (v6 == 176)
        {
            if (a3 < 0x5D)
            {
                if (a3 < 0xA)
                {
                    if (a3 < 6)
                    {
                        if (a3 == 1)
                        {
                            result = (DWORD *)(a1 + 4 * v5);
                            result[160] = a4;
                        }
                    }
                    else if (a3 <= 6)
                    {
                        result = (DWORD *)(a1 + 4 * v5);
                        result[272] = a4;
                    }
                    else if (a3 == 7)
                    {
                        result = (DWORD *)(a1 + 4 * v5);
                        result[176] = a4;
                    }
                }
                else if (a3 <= 0xA)
                {
                    result = (DWORD *)(a1 + 4 * v5);
                    result[192] = a4;
                }
                else if (a3 < 0x40)
                {
                    if (a3 == 11)
                    {
                        result = (DWORD *)(a1 + 4 * v5);
                        result[208] = a4;
                    }
                }
                else if (a3 <= 0x40)
                {
                    result = (DWORD *)(a1 + 4 * v5);
                    result[224] = a4;
                }
                else if (a3 == 91)
                {
                    result = (DWORD *)(a1 + 4 * v5);
                    result[240] = a4;
                }
            }
            else if (a3 <= 0x5D)
            {
                result = (DWORD *)(a1 + 4 * v5);
                result[256] = a4;
            }
            else if (a3 < 0x70)
            {
                if (a3 < 0x6E)
                {
                    if (a3 == 107)
                    {
                        result = (DWORD *)(a1 + 4 * v5);
                        result[80] = a4;
                    }
                }
                else if (a3 <= 0x6E)
                {
                    result = (DWORD *)(a1 + 4 * v5);
                    result[48] = a4;
                }
                else
                {
                    result = (DWORD *)(a1 + 4 * v5);
                    result[64] = a4;
                }
            }
            else if (a3 <= 0x70)
            {
                result = (DWORD *)(a1 + 4 * v5);
                result[96] = a4;
            }
            else if (a3 < 0x73)
            {
                if (a3 == 114)
                {
                    result = (DWORD *)(a1 + 4 * v5);
                    result[112] = a4;
                }
            }
            else if (a3 <= 0x73)
            {
                result = (DWORD *)(a1 + 4 * v5);
                result[128] = a4;
            }
            else if (a3 == 119)
            {
                result = (DWORD *)(a1 + 4 * v5);
                result[144] = a4;
            }
        }
    }
    else if (v6 <= 0xC0)
    {
        result = (DWORD *)(a1 + 4 * v5);
        *result = (unsigned __int8)a3;
    }
    else if (v6 == 224)
    {
        *(DWORD *)(a1 + 4 * v5 + 64) = (unsigned __int8)a3;
        result = (DWORD *)(a1 + 4 * v5);
        result[32] = a4;
    }
    return result;
}

DWORD *__cdecl sub_A8900(DWORD *a1, int a2)
{
    DWORD *result; // eax
    int v3; // [esp+0h] [ebp-Ch]
    signed int i; // [esp+4h] [ebp-8h]
    signed int v5; // [esp+8h] [ebp-4h]
    signed int j; // [esp+8h] [ebp-4h]

    if (a1)
    {
        result = a1;
        if (a1[3])
        {
            v5 = *(signed __int16 *)(a1[3] + 8);
            v3 = a1[3] + 10;
            for (i = 0; i < v5 && *(unsigned __int16 *)(v3 + 6 * i) != a2; ++i)
                ;
            result = (DWORD *)i;
            if (i != v5)
            {
                a1[5] = *(DWORD *)(6 * i + v3 + 2) + a1[4] + 8;
                result = a1;
                a1[12] = 0;
                if (!dword_181DE8)
                {
                    for (j = 0; j < 4; ++j)
                    {
                        a1[j + 33] = -1;
                        result = (DWORD *)j;
                    }
                }
            }
        }
    }
    return result;
}

DWORD * sub_97480(DWORD *a1, int a2)
{
    DWORD *result; // eax
    bool v3; // [esp+0h] [ebp-4h]

    ++dword_181C04;
    v3 = dword_181BF4 && (dword_181C04 == 1 || dword_181BF8) && !sub_A16A2() && sub_916F0();
    if (v3)
        fprintf((FILE*)dword_181BF0, aAilBranchIndex, a1, a2);
    result = sub_A8900(a1, a2);
    --dword_181C04;
    return result;
}

bool sub_9D650(unsigned int a1, unsigned int a2)
{
    unsigned int v3; // [esp+0h] [ebp-54h]
    unsigned int v4; // [esp+4h] [ebp-50h]
    int v5; // [esp+Ch] [ebp-48h]
    unsigned int v6; // [esp+10h] [ebp-44h]
    int v7; // [esp+14h] [ebp-40h]
    unsigned int v8; // [esp+1Ch] [ebp-38h]
    int v9; // [esp+20h] [ebp-34h]
    char v10; // [esp+28h] [ebp-2Ch]
    int v11; // [esp+40h] [ebp-14h]
    unsigned int v12; // [esp+48h] [ebp-Ch]
    unsigned int v13; // [esp+4Ch] [ebp-8h]
    int v14; // [esp+50h] [ebp-4h]

    v14 = 0;
    if (a1 >= a2)
        v3 = a2;
    else
        v3 = a1;
    v13 = v3;
    if (a1 < a2)
        v4 = a2;
    else
        v4 = a1;
    v12 = v4 - v13 + 1;
    v5 = 1537;
    v6 = v13 >> 16;
    v7 = (unsigned __int16)v13;
    v8 = v12 >> 16;
    v9 = (unsigned __int16)(v4 - v13 + 1);
    int386(49, (union REGS *)(int)&v5, (union REGS *)(int)&v10);
    return v11 == 0;
}

bool sub_9D590(unsigned int a1, unsigned int a2)
{
    unsigned int v3; // [esp+0h] [ebp-54h]
    unsigned int v4; // [esp+4h] [ebp-50h]
    int v5; // [esp+Ch] [ebp-48h]
    unsigned int v6; // [esp+10h] [ebp-44h]
    int v7; // [esp+14h] [ebp-40h]
    unsigned int v8; // [esp+1Ch] [ebp-38h]
    int v9; // [esp+20h] [ebp-34h]
    char v10; // [esp+28h] [ebp-2Ch]
    int v11; // [esp+40h] [ebp-14h]
    unsigned int v12; // [esp+48h] [ebp-Ch]
    unsigned int v13; // [esp+4Ch] [ebp-8h]
    int v14; // [esp+50h] [ebp-4h]

    v14 = 0;
    if (a1 >= a2)
        v3 = a2;
    else
        v3 = a1;
    v13 = v3;
    if (a1 < a2)
        v4 = a2;
    else
        v4 = a1;
    v12 = v4 - v13 + 1;
    v5 = 1536;
    v6 = v13 >> 16;
    v7 = (unsigned __int16)v13;
    v8 = v12 >> 16;
    v9 = (unsigned __int16)(v4 - v13 + 1);
    int386(49, (union REGS *)(int)&v5, (union REGS *)(int)&v10);
    return v11 == 0;
}

bool sub_A4330()
{
    bool result; // eax

    if (!dword_E4A94)
    {
        result = sub_9D590((unsigned int)sub_A4330, (unsigned int)sub_A4CB0);
        dword_E4A94 = 1;
    }
    return result;
}

bool sub_A4CB0()
{
    bool result; // eax

    if (dword_E4A94)
    {
        result = sub_9D650((unsigned int)sub_A4330, (unsigned int)sub_A4CB0);
        dword_E4A94 = 0;
    }
    return result;
}

int sub_91F70(int a1, int a2, WORD *a3, WORD *a4)
{
    unsigned int i; // [esp+0h] [ebp-10h]
    unsigned int j; // [esp+0h] [ebp-10h]
    bool v7; // [esp+4h] [ebp-Ch]
    int v8; // [esp+Ch] [ebp-4h]

    ++dword_181C04;
    v7 = dword_181BF4 && (dword_181C04 == 1 || dword_181BF8) && !sub_A16A2() && sub_916F0();
    if (v7)
        fprintf((FILE*)dword_181BF0, aAilCallDriver0, a1, a2, a3, a4);
    v8 = sub_A158B(a1, a2, a3, a4);
    if (dword_181BF4 && (dword_181C04 == 1 || dword_181BF8) && !sub_A16A2())
    {
        for (i = 0; i < 0xE; ++i)
            fprintf((FILE*)dword_181BF0, (const char *)&unk_D226C);
        for (j = 1; j < dword_181C04; ++j)
            fprintf((FILE*)dword_181BF0, (const char *)&unk_D2270);
        fprintf((FILE*)dword_181BF0, aResultD, v8);
    }
    --dword_181C04;
    return v8;
}

int sub_A4EB0(int a1)
{
    int result; // eax
    char v2; // [esp+0h] [ebp-Ch]
    __int16 v3; // [esp+4h] [ebp-8h]

    result = a1;
    if (*(DWORD *)(a1 + 424) > 0)
    {
        v3 = *(WORD *)(a1 + 424);
        sub_91F70(*(DWORD *)a1, 1282, (WORD*)&v2, 0);
        *(DWORD *)(a1 + 424) = 0;
        result = a1;
        *(DWORD *)(a1 + 428) = 0;
    }
    return result;
}

int sub_A5F30(int *a1, int a2)
{
    signed int i; // [esp+0h] [ebp-4h]

    for (i = 0; i < 32; ++i)
    {
        if (a1[i + 342] == a2)
        {
            sub_A5850((int)a1, a1[i + 342] | 0x80, a1[i + 374], 0, 0);
            a1[i + 342] = -1;
        }
    }
    return sub_A4EB0(*a1);
}

DWORD * sub_A5850(int a1, char a2, unsigned int a3, signed int a4, int a5)
{
    DWORD *result; // eax
    int v6; // eax
    DWORD *v7; // [esp+0h] [ebp-14h]
    int v8; // [esp+4h] [ebp-10h]
    int v9; // [esp+8h] [ebp-Ch]
    signed int i; // [esp+Ch] [ebp-8h]
    signed int j; // [esp+Ch] [ebp-8h]
    DWORD *v12; // [esp+Ch] [ebp-8h]
    int v13; // [esp+10h] [ebp-4h]

    v7 = *(DWORD **)a1;
    v13 = a2 & 0xF0;
    v8 = a2 & 0xF;
    result = *(DWORD **)(a1 + 4 * v8 + 148);
    v9 = *(DWORD *)(a1 + 4 * v8 + 148);
    if (v13 == 176 || v13 == 192 || v13 == 224)
        result = sub_A5530(a1 + 212, v8 | v13, a3, a4);
    if (v13 != 176)
        goto LABEL_79;
    if (a5)
    {
        result = (DWORD *)(a1 + 4 * v8);
        if (result[181] != -1)
        {
            a4 = *(DWORD *)(a1 + 4 * v8 + 724);
            result = (DWORD *)(a1 + 4 * v8);
            result[181] = -1;
        }
    }
    if (a3 < 0x6F)
    {
        if (a3 < 0x6C)
        {
            if (a3 >= 6)
            {
                if (a3 <= 6)
                {
                    sub_A5850(a1, v8 | 0xB0, 0x64u, 0, 0);
                    sub_A5850(a1, v8 | 0xB0, 0x65u, 0, 0);
                    sub_A5850(a1, v8 | 0xB0, 0x26u, 0, 0);
                }
                else if (a3 == 7)
                {
                    a4 = *(DWORD *)(a1 + 56) * a4 * v7[108] / 16129;
                    if (a4 > 127)
                        a4 = 127;
                    if (a4 < 0)
                        a4 = 0;
                }
            }
            goto LABEL_79;
        }
        if (a3 <= 0x6C)
        {
            if (*(DWORD *)(a1 + 28))
                *(DWORD *)(a1 + 4 * v8 + 724) = (*(int(__cdecl **)(int, int, signed int))(a1 + 28))(a1, v8, a4);
            goto LABEL_79;
        }
        if (a3 <= 0x6D)
        {
            result = sub_97480((DWORD *)a1, a4);
        }
        else if (a4 < 64)
        {
            result = &v7[v9];
            if (result[8] == 1)
            {
                sub_A5F30((int *)a1, v8);
                sub_980D0(v7, v9 + 1);
                result = (DWORD *)sub_98170((DWORD *)a1, v8 + 1, v8 + 1);
            }
        }
        else
        {
            result = &v7[v9];
            if (result[8] != 1)
            {
                result = (DWORD *)sub_97F90(v7);
                v12 = result;
                if (result)
                {
                    sub_98170((DWORD *)a1, v8 + 1, (int)result);
                    result = (DWORD *)a1;
                    v7[(DWORD)v12 + 23] = a1;
                }
            }
        }
    }
    else if (a3 <= 0x6F)
    {
        result = &v7[v9];
        if (result[8] != 1)
        {
            if (a4 >= 64)
            {
                result = &v7[v9];
                result[8] = 2;
            }
            else
            {
                result = &v7[v9];
                result[8] = 0;
            }
        }
    }
    else if (a3 < 0x75)
    {
        if (a3 < 0x73)
            goto LABEL_79;
        if (a3 <= 0x73)
        {
            *(DWORD *)(a1 + 4 * v8 + 724) = *(unsigned __int8 *)(a4 + *(DWORD *)(a1 + 24));
        LABEL_79:
            if (v7[v9 + 8] != 1 || (result = (DWORD *)v7[v9 + 24], result == (DWORD *)a1))
            {
                if (v13 == 144)
                {
                    ++v7[v9 + 88];
                }
                else if (v13 == 128)
                {
                    --v7[v9 + 88];
                }
                v7[v9 + 56] = a1;
                if (v13 != 144 || (result = (DWORD *)(a1 + 4 * v8), result[133] < 64))
                {
                    if (!v7[104]
                        || (result = (DWORD *)((int(__cdecl *)(DWORD *, int, int, unsigned int, signed int))v7[104])(
                            v7,
                            a1,
                            v9 | v13,
                            a3,
                            a4)) == 0)
                    {
                        result = (DWORD *)sub_A4F10((int)v7, v9 | v13, a3, a4);
                    }
                }
            }
            return result;
        }
        for (i = 0; i < 4; ++i)
        {
            result = (DWORD *)(a1 + 4 * i);
            if (result[33] == -1)
                break;
            result = (DWORD *)i;
        }
        if (i != 4)
        {
            *(DWORD *)(4 * i + a1 + 132) = a4;
            result = *(DWORD **)(a1 + 20);
            *(DWORD *)(4 * i + a1 + 116) = (DWORD)result;
        }
    }
    else if (a3 <= 0x75)
    {
        if (a4 >= 64)
        {
            for (j = 3; j >= 0; --j)
            {
                result = (DWORD *)(a1 + 4 * j);
                if (result[33] != -1)
                    break;
                result = (DWORD *)j;
            }
            if (j != -1)
            {
                if (*(DWORD *)(a1 + 4 * j + 132))
                {
                    v6 = a1 + 4 * j;
                    if (--*(DWORD *)(v6 + 132))
                    {
                        result = (DWORD *)a1;
                        *(DWORD *)(a1 + 20) = *(DWORD *)(a1 + 4 * j + 116);
                    }
                    else
                    {
                        result = (DWORD *)(a1 + 4 * j);
                        result[33] = -1;
                    }
                }
                else
                {
                    result = (DWORD *)a1;
                    *(DWORD *)(a1 + 20) = *(DWORD *)(a1 + 4 * j + 116);
                }
            }
        }
    }
    else if (a3 <= 0x76)
    {
        *(DWORD *)(a1 + 92) = 0;
        *(DWORD *)(a1 + 96) = 0;
        *(DWORD *)(a1 + 108) = 0;
        *(DWORD *)(a1 + 108) -= *(DWORD *)(a1 + 104);
        result = (DWORD *)a1;
        if (*(DWORD *)(a1 + 36))
            result = (DWORD *)(*(int(__cdecl **)(DWORD *, int, DWORD, DWORD))(a1 + 36))(v7, a1, 0, 0);
    }
    else
    {
        if (a3 != 119)
            goto LABEL_79;
        result = (DWORD *)a1;
        if (*(DWORD *)(a1 + 32))
            result = (DWORD *)(*(int(__cdecl **)(int, int, signed int))(a1 + 32))(a1, v8, a4);
    }
    return result;
}



signed int sub_A4E10(char a1)
{
    unsigned int v2; // [esp+0h] [ebp-8h]

    v2 = a1 & 0xF0;
    if (v2 < 0xB0)
    {
        if (v2 < 0x90)
        {
            if (v2 != 128)
                return 0;
        }
        else if (v2 > 0x90 && v2 != 160)
        {
            return 0;
        }
        return 3;
    }
    if (v2 > 0xB0)
    {
        if (v2 < 0xD0)
        {
            if (v2 != 192)
                return 0;
        }
        else if (v2 > 0xD0)
        {
            if (v2 != 224)
                return 0;
            return 3;
        }
        return 2;
    }
    return 3;
}

int sub_A4F10(int a1, char a2, char a3, char a4)
{
    int v4; // ebx
    int v5; // edx
    int v6; // ebx
    int v7; // edx
    int v8; // ebx
    int v9; // edx
    int result; // eax
    signed int v11; // [esp+0h] [ebp-4h]

    v11 = sub_A4E10(a2);
    if ((unsigned int)(v11 + *(DWORD *)(a1 + 428)) > 0x200)
        sub_A4EB0(a1);
    v4 = *(DWORD *)(a1 + 8);
    v5 = (*(DWORD *)(a1 + 428))++;
    *(BYTE *)(v4 + v5 + 256) = a2;
    v6 = *(DWORD *)(a1 + 8);
    v7 = (*(DWORD *)(a1 + 428))++;
    *(BYTE *)(v6 + v7 + 256) = a3;
    if (v11 == 3)
    {
        v8 = *(DWORD *)(a1 + 8);
        v9 = (*(DWORD *)(a1 + 428))++;
        *(BYTE *)(v8 + v9 + 256) = a4;
    }
    result = a1;
    ++*(DWORD *)(result + 424);
    return result;
}

DWORD *__cdecl sub_A5FD0(int a1, int a2)
{
    DWORD *result; // eax

    if (*(DWORD *)(a1 + 4 * a2 + 660) != -1)
        sub_A5850(a1, a2 | 0xB0, 0x72u, *(DWORD *)(a1 + 4 * a2 + 660), 0);
    if (*(DWORD *)(a1 + 4 * a2 + 212) != -1)
        sub_A5850(a1, a2 | 0xC0, *(DWORD *)(a1 + 4 * a2 + 212), 0, 0);
    if (*(DWORD *)(a1 + 4 * a2 + 340) != -1)
        sub_A5850(a1, a2 | 0xE0, *(DWORD *)(a1 + 4 * a2 + 276), *(DWORD *)(a1 + 4 * a2 + 340), 0);
    if (*(DWORD *)(a1 + 4 * a2 + 532) != -1)
        sub_A5850(a1, a2 | 0xB0, 0x6Bu, *(DWORD *)(a1 + 4 * a2 + 532), 0);
    if (*(DWORD *)(a1 + 4 * a2 + 468) != -1)
        sub_A5850(a1, a2 | 0xB0, 0x6Fu, *(DWORD *)(a1 + 4 * a2 + 468), 0);
    if (*(DWORD *)(a1 + 4 * a2 + 596) != -1)
        sub_A5850(a1, a2 | 0xB0, 0x70u, *(DWORD *)(a1 + 4 * a2 + 596), 0);
    if (*(DWORD *)(a1 + 4 * a2 + 852) != -1)
        sub_A5850(a1, a2 | 0xB0, 1u, *(DWORD *)(a1 + 4 * a2 + 852), 0);
    if (*(DWORD *)(a1 + 4 * a2 + 916) != -1)
        sub_A5850(a1, a2 | 0xB0, 7u, *(DWORD *)(a1 + 4 * a2 + 916), 0);
    if (*(DWORD *)(a1 + 4 * a2 + 980) != -1)
        sub_A5850(a1, a2 | 0xB0, 0xAu, *(DWORD *)(a1 + 4 * a2 + 980), 0);
    if (*(DWORD *)(a1 + 4 * a2 + 1044) != -1)
        sub_A5850(a1, a2 | 0xB0, 0xBu, *(DWORD *)(a1 + 4 * a2 + 1044), 0);
    if (*(DWORD *)(a1 + 4 * a2 + 1108) != -1)
        sub_A5850(a1, a2 | 0xB0, 0x40u, *(DWORD *)(a1 + 4 * a2 + 1108), 0);
    if (*(DWORD *)(a1 + 4 * a2 + 1172) != -1)
        sub_A5850(a1, a2 | 0xB0, 0x5Bu, *(DWORD *)(a1 + 4 * a2 + 1172), 0);
    if (*(DWORD *)(a1 + 4 * a2 + 1236) != -1)
        sub_A5850(a1, a2 | 0xB0, 0x5Du, *(DWORD *)(a1 + 4 * a2 + 1236), 0);
    result = (DWORD *)(a1 + 4 * a2);
    if (result[325] != -1)
        result = sub_A5850(a1, a2 | 0xB0, 6u, *(DWORD *)(a1 + 4 * a2 + 1300), 0);
    return result;
}

DWORD * sub_A8EA0(DWORD *a1, int a2)
{
    DWORD *result; // eax
    int v3; // [esp+0h] [ebp-10h]
    int v4; // [esp+4h] [ebp-Ch]
    signed int i; // [esp+8h] [ebp-8h]
    int v6; // [esp+Ch] [ebp-4h]

    v4 = a2 - 1;
    result = &a1[a2 - 1];
    if (result[8] == 1)
    {
        ++a1[5];
        a1[v4 + 8] = a1[v4 + 72];
        a1[v4 + 56] = a1[v4 + 40];
        sub_A4F10((int)a1, v4 | 0xB0, 64, 0);
        v6 = a1[7];
        v3 = a1[6];
        while (v6)
        {
            if (*(DWORD *)(v3 + 4) != 1)
            {
                for (i = 0; i < 32; ++i)
                {
                    if (*(DWORD *)(v3 + 4 * i + 1368) != -1 && *(DWORD *)(v3 + 4 * *(DWORD *)(v3 + 4 * i + 1368) + 148) == v4)
                    {
                        sub_A5850(v3, *(unsigned int *)(v3 + 4 * i + 1368) | 0x80, *(DWORD *)(v3 + 4 * i + 1496), 0, 0);
                        *(DWORD *)(v3 + 4 * i + 1368) = -1;
                    }
                }
            }
            --v6;
            v3 += 1816;
        }
        if (a1[v4 + 40])
        {
            if (*(DWORD *)(a1[v4 + 40] + 4) != 1)
                sub_A5FD0(a1[v4 + 40], v4);
        }
        result = a1;
        --result[5];
    }
    return result;
}

int sub_980D0(DWORD *a1, int a2)
{
    int result; // eax
    bool v3; // [esp+0h] [ebp-4h]

    ++dword_181C04;
    v3 = dword_181BF4 && (dword_181C04 == 1 || dword_181BF8) && !sub_A16A2() && sub_916F0();
    if (v3)
        fprintf((FILE*)dword_181BF0, aAilReleaseChan, a1, a2);
    result = (int)sub_A8EA0(a1, a2);
    --dword_181C04;
    return result;
}

DWORD * sub_A9080(DWORD *a1, int a2, int a3)
{
    DWORD *result; // eax

    if (a1)
    {
        a1[a2 + 36] = a3 - 1;
        result = (DWORD *)(4 * a3 + *a1);
        if (result[7] == 1)
        {
            result = *(DWORD **)(4 * a3 + *a1 + 92);
            if (result != a1)
            {
                result = a1;
                *(DWORD *)(*a1 + 4 * a3 + 92) = (DWORD)a1;
            }
        }
    }
    return result;
}

int sub_98170(DWORD *a1, int a2, int a3)
{
    int result; // eax
    bool v4; // [esp+0h] [ebp-4h]

    ++dword_181C04;
    v4 = dword_181BF4 && (dword_181C04 == 1 || dword_181BF8) && !sub_A16A2() && sub_916F0();
    if (v4)
        fprintf((FILE*)dword_181BF0, aAilMapSequence, a1, a2, a3);
    result = (int)sub_A9080(a1, a2, a3);
    --dword_181C04;
    return result;
}

int sub_A8BF0(DWORD *a1)
{
    int v2; // [esp+0h] [ebp-18h]
    int v3; // [esp+4h] [ebp-14h]
    signed int v4; // [esp+8h] [ebp-10h]
    signed int v5; // [esp+Ch] [ebp-Ch]
    signed int k; // [esp+10h] [ebp-8h]
    signed int i; // [esp+14h] [ebp-4h]
    signed int j; // [esp+14h] [ebp-4h]
    int v9; // [esp+14h] [ebp-4h]

    ++a1[5];
    v5 = -1;
    v4 = 0x7FFFFFFF;
    for (i = 8; i >= 1; --i)
    {
        if (i != 9 && a1[i + 8] != 1 && a1[i + 8] != 2 && a1[i + 88] < v4)
        {
            v4 = a1[i + 88];
            v5 = i;
        }
    }
    if (v5 == -1)
    {
        for (j = 8; j >= 1; --j)
        {
            if (j != 9 && a1[j + 8] != 1 && a1[j + 88] < v4)
            {
                v4 = a1[j + 88];
                v5 = j;
            }
        }
    }
    if (v5 == -1)
    {
        --a1[5];
        v2 = 0;
    }
    else
    {
        sub_A4F10((int)a1, v5 | 0xB0, 64, 0);
        v9 = a1[7];
        v3 = a1[6];
        while (v9)
        {
            if (*(DWORD *)(v3 + 4) != 1)
            {
                for (k = 0; k < 32; ++k)
                {
                    if (*(DWORD *)(v3 + 4 * k + 1368) != -1 && *(DWORD *)(v3 + 4 * *(DWORD *)(v3 + 4 * k + 1368) + 148) == v5)
                    {
                        sub_A5850(v3, *(unsigned int *)(v3 + 4 * k + 1368) | 0x80, *(DWORD *)(v3 + 4 * k + 1496), 0, 0);
                        *(DWORD *)(v3 + 4 * k + 1368) = -1;
                    }
                }
            }
            --v9;
            v3 += 1816;
        }
        a1[v5 + 72] = a1[v5 + 8];
        a1[v5 + 8] = 1;
        a1[v5 + 24] = 0;
        a1[v5 + 40] = a1[v5 + 56];
        --a1[5];
        v2 = v5 + 1;
    }
    return v2;
}

int sub_97F90(DWORD *a1)
{
    unsigned int i; // [esp+0h] [ebp-10h]
    unsigned int j; // [esp+0h] [ebp-10h]
    bool v4; // [esp+4h] [ebp-Ch]
    int v5; // [esp+Ch] [ebp-4h]

    ++dword_181C04;
    v4 = dword_181BF4 && (dword_181C04 == 1 || dword_181BF8) && !sub_A16A2() && sub_916F0();
    if (v4)
        fprintf((FILE*)dword_181BF0, aAilLockChannel, a1);
    v5 = sub_A8BF0(a1);
    if (dword_181BF4 && (dword_181C04 == 1 || dword_181BF8) && !sub_A16A2())
    {
        for (i = 0; i < 0xE; ++i)
            fprintf((FILE*)dword_181BF0, (const char *)&unk_D226C);
        for (j = 1; j < dword_181C04; ++j)
            fprintf((FILE*)dword_181BF0, (const char *)&unk_D2270);
        fprintf((FILE*)dword_181BF0, aResultD, v5);
    }
    --dword_181C04;
    return v5;
}



int sub_A158B(int a1, __int16 a2, WORD *a3, WORD *a4)
{
    __int16 v8; // dx
    __int16 v9; // si
    __int16 v10; // di
    __int16 v11; // cx
    __int16 v12; // dx
    __int16 v13; // si
    __int16 v14; // di
    int v15; // ST00_4
    __int16 v17; // [esp+Eh] [ebp-3Ah]
    int v18; // [esp+10h] [ebp-38h]
    __int16 v19; // [esp+1Eh] [ebp-2Ah]
    __int16 v20; // [esp+22h] [ebp-26h]
    __int16 v21; // [esp+26h] [ebp-22h]
    __int16 v22; // [esp+2Ah] [ebp-1Eh]
    __int16 v23; // [esp+2Eh] [ebp-1Ah]
    __int16 v24; // [esp+30h] [ebp-18h]
    __int16 v25; // [esp+32h] [ebp-16h]

    sub_91BD0();
    memset(&v17, 0, sizeof(v17));
    //_CF = __CFSHR__(50, 2); tom stub
    _ZF = 0;
    _SF = 0;
    memset(&v18, 0, 0x30u);
    v25 = 0;
    v24 = 0;
    //__asm { pushfw }//tom stub
    v23 = 0;
    v22 = a2;
    if (a3)
    {
        v8 = a3[3];
        v9 = a3[4];
        v10 = a3[5];
        v21 = a3[2];
        v20 = v8;
        myHIWORD((DWORD*)&v18,v9);
        v17 = v10;
    }
    v19 = *(WORD *)(*(DWORD *)(a1 + 8) + 50);
    //__asm { int     31h; /*DPMI Services   ax = func xxxxh*/ } //stub tom
    if (a4)
    {
        v11 = v21;
        v12 = v20;
        v13 = HIWORD(v18);
        v14 = v17;
        *a4 = v22;
        a4[2] = v11;
        a4[3] = v12;
        a4[4] = v13;
        a4[5] = v14;
        a4[1] = v19;
    }
    v15 = v22;
    sub_91BF0();
    return v15;
}

int sub_92160()
{
    return sub_A16A2();
}

unsigned __int8 __cdecl sub_A1665(int a1)
{
    unsigned __int8 result; // al
    unsigned __int16 v2; // dx
    int i; // ecx
    unsigned __int8 v4; // al

    //result = MEMORY[0x463];//stub tom
    //v2 = MEMORY[0x463] + 6;//stub tom
    for (i = a1; i; --i)
    {
        do
            v4 = __inbyte(v2);
        while (!(v4 & 8));
        do
            result = __inbyte(v2);
        while (result & 8);
    }
    return result;
}

unsigned __int8 __cdecl sub_920D0(int a1)
{
    unsigned __int8 result; // al
    bool v2; // [esp+0h] [ebp-4h]

    ++dword_181C04;
    v2 = dword_181BF4 && (dword_181C04 == 1 || dword_181BF8) && !sub_A16A2() && sub_916F0();
    if (v2)
        fprintf((FILE*)dword_181BF0, aAilDelayD, a1);
    result = sub_A1665(a1);
    --dword_181C04;
    return result;
}

char sub_A5E50(int *a1)
{
    int v1; // eax
    int v3; // [esp+0h] [ebp-8h]
    signed int i; // [esp+4h] [ebp-4h]

    v3 = 0;
    for (i = 0; i < 32; ++i)
    {
        if (a1[i + 342] != -1)
        {
            sub_A5850((int)a1, a1[i + 342] | 0x80, a1[i + 374], 0, 0);
            a1[i + 342] = -1;
            ++v3;
        }
    }
    a1[341] = 0;
    myLOBYTE((WORD*)&v1,sub_A4EB0(*a1));
    if (v3)
    {
        v1 = sub_92160();
        if (!v1)
            myLOBYTE((WORD*)&v1,sub_920D0(3));
    }
    return v1;
}

int *__cdecl sub_A8050(int *a1)
{
    int *result; // eax
    int v2; // [esp+0h] [ebp-Ch]
    signed int i; // [esp+4h] [ebp-8h]
    DWORD *v4; // [esp+8h] [ebp-4h]

    if (a1)
    {
        result = a1;
        if (a1[1] == 4)
        {
            a1[1] = 8;
            sub_A5E50(a1);
            result = (int *)*a1;
            v4 = (DWORD *)*a1;
            for (i = 0; i < 16; ++i)
            {
                v2 = a1[i + 37];
                if (a1[i + 277] >= 64)
                    sub_A4F10((int)v4, v2 | 0xB0, 64, 0);
                if (a1[i + 117] >= 64)
                    v4[v2 + 8] = 0;
                if (a1[i + 149] >= 64)
                    sub_A4F10((int)v4, v2 | 0xB0, 112, 0);
                if (a1[i + 101] >= 64)
                    sub_980D0(v4, v2 + 1);
                result = (int *)i;
            }
        }
    }
    return result;
}

int sub_95DE0(int *a1)
{
    int result; // eax
    bool v2; // [esp+0h] [ebp-4h]

    ++dword_181C04;
    v2 = dword_181BF4 && (dword_181C04 == 1 || dword_181BF8) && !sub_A16A2() && sub_916F0();
    if (v2)
        fprintf((FILE*)dword_181BF0, aAilStopSequenc, a1);
    result = (int)sub_A8050(a1);
    --dword_181C04;
    return result;
}

int sub_9F170(int a1, unsigned __int16 a2)
{
    int v3; // [esp+0h] [ebp-20h]
    int v4; // [esp+Ch] [ebp-14h]
    int v5; // [esp+18h] [ebp-8h]
    int v6; // [esp+1Ch] [ebp-4h]

    v3 = 257;
    v4 = a2;
    int386(49, (union REGS *)(int)&v3, (union REGS *)(int)&v3);
    if (v5)
        v6 = -1;
    else
        v6 = 0;
    return v6;
}

int sub_9F6D0(int *a1, __int16 a2)
{
    __int16 v3; // [esp+0h] [ebp-10h]
    __int16 v4; // [esp+4h] [ebp-Ch]
    __int16 v5; // [esp+6h] [ebp-Ah]

    sub_9F170(*(int *)byte_E3E54, *(unsigned __int16 *)&byte_E3E54[(BYTE*)4]);
    *(WORD *)&byte_E3E54[(BYTE*)4] = 0;
    *(DWORD *)byte_E3E54 = 0;
    v4 = 18;
    v5 = a2;
    sub_91F70(*a1, 1286, (WORD*)&v3, (WORD*)&v3);
    return v3;
}

void sub_9FA80()
{
    if (byte_E37FC && !stricmp(&unk_180BE0, aSbawe32Mdi_1) && !stricmp(&byte_180C84, aW))
    {
        sub_9F6D0((int *)dword_180C7C, 1);
        sub_9F170(dword_181E2C, word_181E30);
        sub_9F170(dword_181E26, word_181E2A);
        byte_E3815 = 0;
    }
}

int sub_A8250(int a1)
{
    int result; // eax

    if (a1)
    {
        result = a1;
        if (*(DWORD *)(a1 + 4) != 1)
        {
            sub_95DE0((int *)a1);
            *(DWORD *)(a1 + 4) = 2;
            result = a1;
            if (*(DWORD *)(a1 + 40))
                result = (*(int(__cdecl **)(int))(a1 + 40))(a1);
        }
    }
    return result;
}

int __cdecl sub_95F00(int a1)
{
    int result; // eax
    bool v2; // [esp+0h] [ebp-4h]

    ++dword_181C04;
    v2 = dword_181BF4 && (dword_181C04 == 1 || dword_181BF8) && !sub_A16A2() && sub_916F0();
    if (v2)
        fprintf((FILE*)dword_181BF0, aAilEndSequence, a1);
    result = sub_A8250(a1);
    --dword_181C04;
    return result;
}

bool sub_9D710(unsigned int a1, int a2)
{
    return sub_9D590(a1, a2 + a1);
}

bool sub_9E1B0()
{
    bool result; // eax

    if (!dword_E3E3C)
    {
        sub_9D590((unsigned int)sub_9E1B0, (unsigned int)sub_9EDD0);
        sub_9D710((unsigned int)dword_181C50, 64);
        sub_9D710((unsigned int)dword_181DAC, 76);
        sub_9D710((unsigned int)&byte_181C90, 256);
        sub_9D710((unsigned int)&unk_181D90, 24);
        sub_9D710((unsigned int)&dword_E3E38, 4);
        result = sub_9D710((unsigned int)&dword_181DA8, 4);
        dword_E3E3C = 1;
    }
    return result;
}

bool __cdecl sub_9D740(unsigned int a1, int a2)
{
    return sub_9D650(a1, a2 + a1);
}

bool sub_9EDD0()
{
    bool result; // eax

    if (dword_E3E3C)
    {
        sub_9D650((unsigned int)sub_9E1B0, (unsigned int)sub_9EDD0);
        sub_9D740((unsigned int)dword_181C50, 64);
        sub_9D740((unsigned int)dword_181DAC, 76);
        sub_9D740((unsigned int)&byte_181C90, 256);
        sub_9D740((unsigned int)&unk_181D90, 24);
        sub_9D740((unsigned int)&dword_E3E38, 4);
        result = sub_9D740((unsigned int)&dword_181DA8, 4);
        dword_E3E3C = 0;
    }
    return result;
}

signed int __cdecl sub_A14DB(int a1)
{
    signed int result; // eax

    if (a1 != -1 && a1 == dword_E3FF8)
    {
        result = 517;
        //__asm { int     31h; DPMI Services   ax = func xxxxh } //tom stub
        dword_E3FF8 = -1;
    }
    return result;
}

signed int __cdecl sub_91E90(int a1)
{
    signed int result; // eax
    bool v2; // [esp+0h] [ebp-4h]

    ++dword_181C04;
    v2 = dword_181BF4 && (dword_181C04 == 1 || dword_181BF8) && !sub_A16A2() && sub_916F0();
    if (v2)
        fprintf((FILE*)dword_181BF0, aAilRestoreUse1, a1);
    result = sub_A14DB(a1);
    --dword_181C04;
    return result;
}

signed int sub_A1249()
{
    signed int result; // eax

    result = 513;
    //__asm { int     31h; DPMI Services   ax = func xxxxh } //stub tom
    return result;
}

signed int __cdecl sub_91D50(int a1, int a2)
{
    signed int result; // eax
    bool v3; // [esp+0h] [ebp-4h]

    ++dword_181C04;
    v3 = dword_181BF4 && (dword_181C04 == 1 || dword_181BF8) && !sub_A16A2() && sub_916F0();
    if (v3)
        fprintf((FILE*)dword_181BF0, aAilSetRealVect, a1, a2);
    result = sub_A1249();
    --dword_181C04;
    return result;
}

void __cdecl sub_9D560(int a1)
{
    sub_83E80(a1);
}

int off_E3E34(unsigned int mem) {//tom stub free memory
    return 0;
};

int sub_9D490(unsigned int a1, int a2)
{
    int result; // eax

    if (a1)
    {
        sub_9D740(a1, a2);
        result = off_E3E34(a1);
    }
    return result;
}

int sub_9EA60(unsigned int a1)
{
    int v1; // ST0C_4
    int v2; // ST08_4
    int result; // eax
    signed int i; // [esp+0h] [ebp-4h]

    if (*(DWORD *)(a1 + 36))
        (*(void(__cdecl **)(DWORD))(a1 + 36))(*(DWORD *)(a1 + 40));
    if (*(DWORD *)(a1 + 32) != -1)
        sub_92DC0(*(DWORD *)(a1 + 32));
    if (*(DWORD *)(a1 + 24))
    {
        if (*(DWORD *)(a1 + 28) != -1)
            sub_91E90(*(DWORD *)(a1 + 28));
        sub_91F70(a1, 774, 0, 0);
    }
    sub_91D50(102, *(DWORD *)(*(DWORD *)(a1 + 16) + 54));
    v1 = *(DWORD *)(a1 + 4);
    v2 = *(DWORD *)a1;
    sub_9D560(*(DWORD *)(a1 + 8));
    result = sub_9D490(a1, 44);
    for (i = 0; i < 16; ++i)
    {
        if (dword_181C50[(DWORD*)i] == a1)
            dword_181C50[(DWORD*)i] = 0;
        result = i;
    }
    return result;
}

int sub_93160(unsigned int a1)
{
    int result; // eax
    bool v2; // [esp+0h] [ebp-4h]

    ++dword_181C04;
    v2 = dword_181BF4 && (dword_181C04 == 1 || dword_181BF8) && !sub_A16A2() && sub_916F0();
    if (v2)
        fprintf((FILE*)dword_181BF0, aAilUninstallDr, a1);
    result = sub_9EA60(a1);
    --dword_181C04;
    return result;
}

void sub_A1744()
{
    int v0; // esi
    unsigned __int8 v1; // of

    sub_91BD0();
    v0 = 56;
    do
    {
        sub_92DC0(v0);
        ///v1 = __OFSUB__(v0, 4);//tom stub
        v0 -= 4;
    } while (!((v0 < 0) ^ v1));
    sub_91BF0();
}

void sub_92E50()
{
    bool v0; // [esp+0h] [ebp-4h]

    ++dword_181C04;
    v0 = dword_181BF4 && (dword_181C04 == 1 || dword_181BF8) && !sub_A16A2() && sub_916F0();
    if (v0)
        fprintf((FILE*)dword_181BF0, aAilReleaseAllT);
    sub_A1744();
    --dword_181C04;
}

char __cdecl sub_A102C(int a1)
{
    unsigned int v1; // et0
    char result; // al
    unsigned int v3; // [esp-4h] [ebp-10h]
    void *retaddr; // [esp+10h] [ebp+4h]

    //v1 = __readeflags();//tom stub
    v3 = v1;
    //_disable();//tom stub
    __outbyte(0x43u, 0x36u);
    dword_E3FE6 = a1;
    __outbyte(0x40u, a1);
    result = BYTE1(a1);
    __outbyte(0x40u, BYTE1(a1));
    //_disable();//tom stub
    if (BYTE1((DWORD)retaddr) & 2)
        //_enable();//tom stub
    //__writeeflags(v3);//tom stub
    return result;
}

signed int sub_A11E2()
{
    unsigned int v0; // et0
    signed int result; // eax
    unsigned int v2; // [esp-4h] [ebp-10h]

    //v0 = __readeflags();//tom stub
    v2 = v0;
    //_disable();//tom stub
    sub_A102C(0);
    result = 9480;
    //__asm { int     21h; DOS - SET INTERRUPT VECTOR }//tom stub
    //_disable();//tom stub
    if (v2 & 0x200)
        //_enable();//tom stub
    //__writeeflags(v2);//tom stub
    return result;
}

signed int sub_9ED70()
{
    signed int i; // [esp+0h] [ebp-8h]
    unsigned int v2; // [esp+4h] [ebp-4h]

    for (i = 15; i >= 0; --i)
    {
        v2 = dword_181C50[(DWORD*)i];
        if (v2)
            sub_93160(v2);
    }
    sub_92E50();
    return sub_A11E2();
}

signed int sub_919C0()
{
    signed int result; // eax
    bool v1; // [esp+0h] [ebp-4h]

    ++dword_181C04;
    v1 = dword_181BF4 && (dword_181C04 == 1 || dword_181BF8) && !sub_A16A2() && sub_916F0();
    if (v1)
        fprintf((FILE*)dword_181BF0, aAilShutdown);
    result = sub_9ED70();
    if (dword_181BF4 && (dword_181C04 == 1 || dword_181BF8))
    {
        result = sub_A16A2();
        if (!result)
            result = fclose((FILE*)dword_181BF0);
    }
    --dword_181C04;
    return result;
}

void sub_99C90()
{
    if (byte_E37FE)
    {
        if (word_E3802)
        {
            sub_95DE0((int *)dword_180C78);
            sub_95F00(dword_180C78);
            word_E3802 = 0;
        }
        if (byte_E3815 == 1)
            sub_9FA80();
        if (!byte_E379A)
            sub_919C0();
        if (dword_E3808)
        {
            sub_83E80(dword_E3808);
            dword_E380C = 0;
        }
        if (dword_E3810)
            sub_83E80(dword_E3810);
        byte_E37FC = 0;
        byte_E37FD = 0;
        byte_E37FE = 0;
    }
}

int sub_A3DA0(int a1)
{
    int result; // eax

    if (a1)
    {
        result = a1;
        if (*(DWORD *)(a1 + 4) != 1)
        {
            result = a1;
            if (*(DWORD *)(a1 + 4) != 2)
            {
                *(DWORD *)(a1 + 4) = 2;
                if (*(DWORD *)(a1 + 2124))
                    (*(void(__cdecl **)(int))(a1 + 2124))(a1);
                result = a1;
                if (*(DWORD *)(a1 + 2128))
                    result = (*(int(__cdecl **)(int))(a1 + 2128))(a1);
            }
        }
    }
    return result;
}

int sub_93D00(int a1)
{
    int result; // eax
    bool v2; // [esp+0h] [ebp-4h]

    ++dword_181C04;
    v2 = dword_181BF4 && (dword_181C04 == 1 || dword_181BF8) && !sub_A16A2() && sub_916F0();
    if (v2)
        fprintf((FILE*)dword_181BF0, aAilEndSample0x, a1);
    result = sub_A3DA0(a1);
    --dword_181C04;
    return result;
}

char *sub_8F850()
{
    char *result; // eax
    char *i; // [esp+0h] [ebp-4h]

    if (byte_E388D)
    {
        sub_92DC0(dword_180CA0[(DWORD*)0]);
        byte_E388D = 0;
        for (i = (char *)&unk_180750; ; i += 23)
        {
            result = i;
            if ((unsigned int)i >= dword_180B4C)
                break;
            i[20] = 0;
            i[21] = 0;
        }
    }
    return result;
}

int sub_A3F00(int a1, int a2, int a3)
{
    int result; // eax

    if (a1)
    {
        result = a3;
        *(DWORD *)(4 * a2 + a1 + 2132) = a3;
    }
    return result;
}

int sub_953E0(int a1, int a2, int a3)
{
    int result; // eax
    bool v4; // [esp+0h] [ebp-4h]

    ++dword_181C04;
    v4 = dword_181BF4 && (dword_181C04 == 1 || dword_181BF8) && !sub_A16A2() && sub_916F0();
    if (v4)
        fprintf((FILE*)dword_181BF0, aAilSetSampleUs, a1, a2, a3);
    result = sub_A3F00(a1, a2, a3);
    --dword_181C04;
    return result;
}

int sub_9F040()
{
    int result; // eax

    if (byte_E3E44)
    {
        byte_E3E45 = 1;
        sub_93D00(dword_181E10);
        sub_953E0(dword_181E10, 0, 0);
        sub_953E0(dword_181E10, 1, 0);
        sub_953E0(dword_181E10, 2, 0);
        sub_953E0(dword_181E10, 3, 0);
        sub_953E0(dword_181E10, 4, 0);
        sub_953E0(dword_181E10, 5, 0);
        sub_953E0(dword_181E10, 6, 0);
        result = sub_953E0(dword_181E10, 7, 0);
    }
    return result;
}

int sub_8D8F0()
{
    int *i; // [esp+0h] [ebp-4h]

    if (byte_E3798 && byte_E3799)
    {
        sub_8F850();
        for (i = (int *)&unk_180750; (unsigned int)i < dword_180B4C; i = (int *)((char *)i + 23))
        {
            sub_93D00(*i);
            *((WORD *)i + 9) = 0;
            *(int *)((char *)i + 14) = 0;
            *((BYTE *)i + 20) = 0;
            *((BYTE *)i + 21) = 0;
        }
    }
    return sub_9F040();
}

void sub_99C10()
{
    if (byte_E379A)
    {
        sub_8D8F0();
        if (!byte_E37FE)
            sub_919C0();
        if (dword_E37A0)
        {
            sub_83E80(dword_E37A0);
            dword_E37A4 = 0;
        }
        if (dword_E37A8)
            sub_83E80(dword_E37A8);
        byte_E3798 = 0;
        byte_E3799 = 0;
        byte_E379A = 0;
    }
}

unsigned __int8 __cdecl outp(unsigned __int16 a1, unsigned __int8 a2)
{
    unsigned __int8 result; // al

    result = a2;
    __outbyte(a1, a2);
    return result;
}

int sub_91420()
{
    __int16 v0; // ST08_2

    v0 = word_180B30;
    outp(word_180B30 + 4, 50);
    outp(549, word_E39AC);
    outp(v0 + 4, 51);
    outp(v0 + 5, word_E39AE);
    outp(v0 + 4, 52);
    outp(v0 + 5, word_E39A8);
    outp(v0 + 4, 53);
    outp(v0 + 5, word_E39AA);
    outp(v0 + 4, 68);
    outp(v0 + 5, word_E39B0);
    outp(v0 + 4, 69);
    outp(v0 + 5, word_E39B2);
    outp(v0 + 4, 70);
    outp(v0 + 5, word_E39B4);
    outp(v0 + 4, 71);
    return outp(v0 + 5, word_E39B6);
}

void sub_986E0()
{
    sub_99C90();
    sub_99C10();
    if (byte_E39B8)
        sub_91420();
}

void sub_46F50()
{
    if (byte_D4B51)
        sub_92DC0(dword_F42A4);
    else
        sub_6FE20();
    sub_986E0();
}

int sub_8C21F()
{
    char v1; // [esp+0h] [ebp-38h]
    int v2; // [esp+1Ch] [ebp-1Ch]

    v2 = (unsigned __int16)word_180662;
    return int386(16, (union REGS *)(int)&v2, (union REGS *)(int)&v1);
}

DWORD *__cdecl sub_9A2F5(int a1)
{
    DWORD *result; // eax

    result = *(DWORD **)(a1 + 28);
    if (*result)
    {
        sub_83E80(**(DWORD **)(a1 + 28));
        result = *(DWORD **)(a1 + 28);
        *result = 0;
    }
    return result;
}

int sub_90D3F(int a1)
{
    int result; // eax

    while (1)
    {
        result = a1;
        if (!*(DWORD *)(a1 + 28))
            break;
        sub_9A2F5(a1);
        a1 += 44;
    }
    return result;
}

signed int sub_5C060()
{
    sub_90D3F((int)aSearchd);
    sub_90D3F((int)aDataBuild00Dat);
    sub_90D3F((int)aWscreen);
    return 1;
}

char sub_86860(unsigned __int16 a1)
{
    int v2; // esi
    __int16 v3; // ax

    if (!byte_E2A28)
        return 0;
    if (!dword_E2A6C || !dword_E2A70)
        return 0;
    v2 = 16 * dword_E2A70;
    *(BYTE *)v2 = 13;
    v3 = dword_E2A70;
    *(BYTE *)(v2 + 1) = 0;
    *(BYTE *)(v2 + 2) = -123;
    *(WORD *)(v2 + 3) = 0;
    word_17FF4A = v3;
    dword_17FF38 = 0;
    dword_17FF10 = 47;
    dword_17FF14 = 0;
    dword_17FF20 = (int)&unk_17FF28;
    dword_17FF40 = a1;
    dword_17FF0C = 768;
    dword_17FF44 = 5392;
    int386(49, (union REGS *)(int)&dword_17FF0C, (union REGS *)(int)&dword_17FF0C);
    unk_180452 = *(DWORD *)v2;
    v2 += 4;
    *((DWORD *)&unk_180452 + 1) = *(DWORD *)v2;
    v2 += 4;
    *((DWORD *)&unk_180452 + 2) = *(DWORD *)v2;
    *((BYTE *)&unk_180452 + 12) = *(BYTE *)(v2 + 4);
    return word_180455;
}

char sub_5BC20()
{
    if (byte_D4B80 == 1)
    {
        sub_5C450();
        if (dword_E3768)
            sub_54600();
        if (*(BYTE *)(dword_D41A4 + 22) & 8)
            sub_8C2DE();
        else
            sub_75420();
        sub_46F50();
        sub_8C21F();
        sub_72D04();
        sub_6FE20();
        sub_5C060();
    }
    sub_83E80(dword_D4198);
    sub_83E80(dword_D41A0);
    sub_83E80(dword_D41A4);
    sub_86860(word_1803EC);
    return sub_86BD0();
}

int sub_56210(unsigned __int16 a1, int a2)
{
    int v2; // ebx
    char v3; // dl
    char *v4; // edi
    char *v5; // esi
    char v6; // al
    char v7; // al
    unsigned __int8 v8; // al
    int v9; // ebx
    int v10; // esi
    int result; // eax
    char v12; // [esp+0h] [ebp-46h]
    int v13; // [esp+80h] [ebp+3Ah]
    char v14; // [esp+84h] [ebp+3Eh]
    unsigned __int8 v15; // [esp+88h] [ebp+42h]
    char v16; // [esp+8Ch] [ebp+46h]
    char v17; // [esp+90h] [ebp+4Ah]
    char v18; // [esp+94h] [ebp+4Eh]
    char v19; // [esp+98h] [ebp+52h]
    char v20; // [esp+9Ch] [ebp+56h]
    char v21; // [esp+A0h] [ebp+5Ah]
    char v22; // [esp+A4h] [ebp+5Eh]
    char v23; // [esp+A8h] [ebp+62h]
    unsigned __int8 v24; // [esp+ACh] [ebp+66h]
    char v25; // [esp+B0h] [ebp+6Ah]
    char v26; // [esp+B4h] [ebp+6Eh]
    char v27; // [esp+B8h] [ebp+72h]
    char v28; // [esp+BCh] [ebp+76h]

    v2 = 1;
    v13 = -1;
    v24 = 67;
    v14 = 0;
    v15 = 0;
    v23 = 0;
    v17 = 0;
    v28 = 0;
    v27 = 0;
    v22 = 0;
    v20 = 0;
    v21 = 0;
    v16 = 0;
    v18 = 0;
    v26 = 0;
    v19 = 0;
    v25 = 0;
    sub_89B60((unsigned char)7u);
    while ((signed __int16)v2 < (signed int)a1)
    {
        v3 = **(BYTE **)(a2 + 4 * (signed __int16)v2);
        if (v3 == 45 || v3 == 47)
        {
            v4 = &v12;
            v5 = (char *)(*(DWORD *)(a2 + 4 * (signed __int16)v2) + 1);
            do
            {
                v6 = *v5;
                *v4 = *v5;
                if (!v6)
                    break;
                v7 = v5[1];
                v5 += 2;
                v4[1] = v7;
                v4 += 2;
            } while (v7);
        }
        if (stricmp(aNetwork, &v12))
        {
            if (!stricmp(aLevel, &v12))
            {
                v15 = unknown_libname_1((char*)*(DWORD *)(a2 + 4 * (signed __int16)++v2));
                byte_D419C = v15;
                v27 = 1;
            }
            else if (!stricmp(aHarddrive, &v12))
            {
                v24 = **(BYTE **)(a2 + 4 * (signed __int16)++v2);
                v8 = toupper(v24);
                v24 = v8;
                if (v8 < 0x41u || v8 > 0x5Au)
                    v24 = 67;
            }
            else if (!stricmp(aSkipscreens, &v12))
            {
                byte_D41AD = 1;
            }
            else if (!stricmp(aNocd, &v12))
            {
                v22 = 1;
            }
            else if (!stricmp(aShowversion, &v12))
            {
                v20 = 1;
            }
            else if (!stricmp(aShowversion2, &v12))
            {
                v21 = 1;
            }
            else if (!stricmp(aDetectoff, &v12))
            {
                v26 = 1;
            }
            else if (!stricmp(aLangcheck, &v12))
            {
                v19 = 1;
            }
            else if (!stricmp(&aMavio[2], &v12))
            {
                v9 = v2 + 1;
                v10 = *(DWORD *)(a2 + 4 * (signed __int16)v9);
                v2 = v9 + 1;
                word_1805C4 = unknown_libname_1((char*)v10);
                word_1805C6 = unknown_libname_1((char*)*(DWORD *)(a2 + 4 * (signed __int16)v2));
                sub_89B60(1u);
            }
            else if (!stricmp(aExtern, &v12))
            {
                dword_E36C4 = sub_98FF5(*(char **)(a2 + 4 * (signed __int16)++v2));
                printf(aExternalCtrlAt);
                sub_89B60(6u);
            }
            else if (!stricmp(aVfx1, &v12))
            {
                sub_89B60(2u);
            }
            else if (!stricmp(&aScc[3], &v12))
            {
                v18 = unknown_libname_1((char*)*(DWORD *)(a2 + 4 * (signed __int16)++v2));
            }
            else if (!stricmp(aSpellsedit, &v12))
            {
                v28 = 1;
            }
            else if (!stricmp(aMusic2, &v12))
            {
                v25 = 1;
            }
        }
        ++v2;
    }
    if (!v22)
        sub_86A00();
    dword_D41A0 = sub_83CD0(224791);
    if (!dword_D41A0)
    {
        printf(aErrorCantAlloc_0);
    LABEL_46:
        exit(-1);
    }
    dword_D41A4 = sub_83CD0(38547);
    if (!dword_D41A4)
    {
        sub_83E80(dword_D41A0);
        printf(aErrorCantAlloc);
        goto LABEL_46;
    }
    memset((void*)dword_D41A0, 0, 224791);
    memset((void*)dword_D41A4, 0, 38547);
    if (v14)
        *(DWORD *)(dword_D41A4 + 22) |= 0x800008u;
    if (v23)
        *(BYTE *)(dword_D41A4 + 24) |= 2u;
    if (v17)
        *(BYTE *)(dword_D41A4 + 24) |= 8u;
    if (v28)
        *(BYTE *)(dword_D41A4 + 24) |= 0x10u;
    if (!v22 && byte_E2A28)
        *(BYTE *)(dword_D41A4 + 24) |= 0x40u;
    if (v27)
        *(BYTE *)(dword_D41A4 + 24) |= 0x80u;
    if (v16)
        *(BYTE *)(dword_D41A4 + 23) |= 2u;
    if (v20)
        *(BYTE *)(dword_D41A4 + 25) |= 8u;
    if (v19)
        *(BYTE *)(dword_D41A4 + 25) |= 0x10u;
    if (v25)
        *(BYTE *)(dword_D41A4 + 25) |= 0x40u;
    if (v26)
        sub_99080(0);
    result = dword_D41A4;
    *(WORD *)(dword_D41A4 + 43) = v15;
    *(WORD *)(result + 45) = v13;
    *(BYTE *)(result + 182) = v24;
    if (v18)
        *(BYTE *)(result + 25) = (*(BYTE *)(result + 25) | 4) & 0xFD;
    if (v21)
    {
        sub_70890();
        sub_5BC20();
        exit(1);
    }
    return result;
}

int sub_main(int argc, char **argv, char **envp)//236f70
{
    signed int v3; // edi
    unsigned __int16 v4; // si
    __int16 v6; // [esp+0h] [ebp-1Ch]
    __int16 v7; // [esp+Ch] [ebp-10h]

    //memset(&v6, 0, 28);//tom stub
    v7 = 0;
    v6 = 13057;
    //int386(33, (int)&v6, (int)&v6); tom stub
    //signal(7, 1); //tom stub SIGBREAK
    //signal(4, 1); //tom stub SIGILL
    //signal(6, 1); //tom stub SIGTERM
    sub_56210(argc, (int)argv); //tom stub //237210
    //sub_5B8D0(); //tom stub
    //sub_46830(0, v3, v4); //tom stub
    //sub_5BC20(); //tom stub
    //sub_56730(); //tom stub
    //reg_eip =0x00236F84;
    //reg_eip = 0x00236Fa2;
    //reg_eip = 0x00236Fba;
    //reg_eip = 0x00236Fc6;
    //reg_eip = 0x00236Fcf;
    DWORD EAX= mem_readw(reg_ebp+0xc);
    CPU_Push32(EAX);//push eax
    //reg_eip = 0x00236Fd4;
    reg_eip = 0x00236Fd9;//-error creating setup directories
    return 0;
}
