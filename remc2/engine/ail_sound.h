#ifndef AIL_SOUND_ACTIVE
#define AIL_SOUND_ACTIVE
#include "../dosbox_files/config.h"

#define MSS_STRUCT struct

typedef MSS_STRUCT                     // VDI interface register structure
{
	Bit16s AX;
	Bit16s BX;
	Bit16s CX;
	Bit16s DX;
	Bit16s SI;
	Bit16s DI;
}
VDI_CALL;

typedef void (* AILTIMERCB)    (Bit32u user);

typedef Bit32s HTIMER;                             // Handle to timer

#define REALFAR unsigned int


typedef MSS_STRUCT                      // I/O parameters structure
{
	Bit16s IO;
	Bit16s IRQ;
	Bit16s DMA_8_bit;
	Bit16s DMA_16_bit;
	Bit32s IO_reserved[4];
}
IO_PARMS;

typedef MSS_STRUCT                      // Standard MSS 3.X VDI driver header
{
   Bit8s     ID[6];                    // "AIL3xxx" ID string, followed by ^Z

   Bit32u    driver_version;

   REALFAR  common_IO_configurations;
   Bit16u    num_IO_configurations;

   REALFAR  environment_string;

   IO_PARMS IO;

   Bit16s     service_rate;

   Bit16u    busy;

   Bit16u    driver_num;             // Driver number

   Bit16u    this_ISR;               // Offset of INT 66H dispatcher
   REALFAR  prev_ISR;               // Pointer to previous INT 66H ISR

   Bit8s     scratch[126];           // Shared scratch workspace

   Bit8s     dev_name[80];           // Device name (VDI version >= 1.12 only)
}
VDI_HDR;

typedef MSS_STRUCT                   // Handle to driver
{
   Bit8u allbuffer[4096];
}
_DIG_DRIVER;

typedef _DIG_DRIVER* HDIGDRIVER;    // Handle to digital driver

typedef MSS_STRUCT                   // Handle to driver
{
   REALFAR  seg;                 // Seg:off pointer to driver (off=0)
   Bit32u    sel;                   // Selector for driver (off=0)
   void*	buf;                 // Protected-mode pointer to driver
   Bit32u    size;                  // Size of driver image
   VDI_HDR *VHDR;                // Pointer to driver header (same as buf)
   Bit32s     type;                 // AIL3DIG or AIL3MDI (see below)
   
   Bit32s     initialized;          // 1 if hardware successfully init'd, else 0

   Bit32s     PM_ISR;               // -1 if no PM ISR hooked, else IRQ #

   HTIMER   server;              // DRV_SERVE periodic timer, if requested

								 // Vector to high-level destructor, if any

   void (*destructor)(HDIGDRIVER);

   // High-level descriptor (DIG_ or MDI_DRIVER)
void        *descriptor;
}
AIL_DRIVERx;

typedef MSS_STRUCT                   // Handle to driver
{
   REALFAR AIL_DRIVER_var0_seg;
	Bit32u AIL_DRIVER_var1_sel;
	Bit8u* AIL_DRIVER_var2_buf;
	Bit32u AIL_DRIVER_var3_size;
	VDI_HDR* AIL_DRIVER_var4_VHDR;
	Bit32s AIL_DRIVER_var5;
	Bit32s AIL_DRIVER_var6;
	Bit32s AIL_DRIVER_var7;
	Bit32s AIL_DRIVER_var8;
	Bit32s AIL_DRIVER_var9;
	Bit32s AIL_DRIVER_var10;
}
AIL_DRIVER;


#endif //AIL_SOUND_
