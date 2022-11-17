#include "port_show_perifery.h"

#include "stdio.h"
#include "string.h"

void ShowPerifery(char* text, int a, int b, int c) {
    if (text)
    {
        if (!strcmp(text, "cpuid"))
            Logger->debug("external pefifery - CPU identification!!!! - Fix It");
        else if (!strcmp(text, "fninit"))
            Logger->debug("external pefifery - CPU FPU unit turn on!!!! - Fix It");
        else if (!strcmp(text, "port0x3C7"))
            Logger->debug("external pefifery - Read VGA palette!!!! - Fix It");
        else if (!strcmp(text, "port0x3C8"))
            Logger->debug("external pefifery - Write VGA palette!!!! - Fix It");
        else if (!strcmp(text, "initVga320"))
            Logger->debug("external pefifery - Init VGA - resolution 320x200!!!! - Fix It");
        else if (!strcmp(text, "chRes"))
            Logger->debug("external pefifery - Change resolution type:{}!!!! - Fix It", a);
        else if (!strcmp(text, "mouseRes"))
            Logger->debug("external pefifery - Mouse Reset!!!! - Fix It");
        else if (!strcmp(text, "mouseSubr"))
            Logger->debug("external pefifery - Mouse Install subroutine!!!! - Fix It");
        else if (!strcmp(text, "mouseHide"))
            Logger->debug("external pefifery - Mouse hide cursor!!!! - Fix It");
        else if (!strcmp(text, "mouseSetPix"))
            Logger->debug("external pefifery - Mouse set pixel ratio!!!! - Fix It");
        else if (!strcmp(text, "blit320"))
            Logger->debug("external pefifery - Blit320");
        else if (!strcmp(text, "blit640"))
            Logger->debug("external pefifery - Blit640");
        else if (!strcmp(text, "VSYNC"))
            Logger->debug("external pefifery - VSYNC");
        else if (!strcmp(text, "CLEAN_KEYS"))
            Logger->debug("external pefifery - clean pressed keys");
        else if (!strcmp(text, "START_TIMER"))
            Logger->debug("external pefifery - start timer");
        else if (!strcmp(text, "COMPUTE_TIMER"))
            Logger->debug("external pefifery - compute timer");
        else if (!strcmp(text, "MOUSE SET POS"))
            Logger->debug("external pefifery - set mouse pos");
        else
            Logger->debug("unknown external pefifery!!!! - Fix It");
    }
    else
        Logger->debug("unknown external pefifery!!!! - Fix It");
};//for devices comutications

