#include "port_show_perifery.h"

#include "stdio.h"
#include "string.h"

void ShowPerifery(char* text = nullptr, int a = 0, int b = 0, int c = 0) {
    if (text)
    {
        if (!strcmp(text, "cpuid"))
            printf("external pefifery - CPU identification!!!! - Fix It\n");
        else if (!strcmp(text, "fninit"))
            printf("external pefifery - CPU FPU unit turn on!!!! - Fix It\n");
        else if (!strcmp(text, "port0x3C7"))
            printf("external pefifery - Read VGA palette!!!! - Fix It\n");
        else if (!strcmp(text, "port0x3C8"))
            printf("external pefifery - Write VGA palette!!!! - Fix It\n");
        else if (!strcmp(text, "initVga320"))
            printf("external pefifery - Init VGA - resolution 320x200!!!! - Fix It\n");
        else if (!strcmp(text, "chRes"))
            printf("external pefifery - Change resolution type:%d!!!! - Fix It\n", a);
        else if (!strcmp(text, "mouseRes"))
            printf("external pefifery - Mouse Reset!!!! - Fix It\n");
        else if (!strcmp(text, "mouseSubr"))
            printf("external pefifery - Mouse Install subroutine!!!! - Fix It\n");
        else if (!strcmp(text, "mouseHide"))
            printf("external pefifery - Mouse hide cursor!!!! - Fix It\n");
        else if (!strcmp(text, "mouseSetPix"))
            printf("external pefifery - Mouse set pixel ratio!!!! - Fix It\n");
        else if (!strcmp(text, "blit320"))
            printf("external pefifery - Blit320\n");
        else if (!strcmp(text, "blit640"))
            printf("external pefifery - Blit640\n");
        else if (!strcmp(text, "VSYNC"))
            printf("external pefifery - VSYNC\n");
        else if (!strcmp(text, "CLEAN_KEYS"))
            printf("external pefifery - clean pressed keys\n");
        else if (!strcmp(text, "START_TIMER"))
            printf("external pefifery - start timer\n");
        else if (!strcmp(text, "COMPUTE_TIMER"))
            printf("external pefifery - compute timer\n");
        else if (!strcmp(text, "MOUSE SET POS"))
            printf("external pefifery - set mouse pos\n");
        else
            printf("unknown external pefifery!!!! - Fix It\n");
    }
    else
        printf("unknown external pefifery!!!! - Fix It\n");
};//for devices comutications

