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
	/*0*/Bit16s IO;
	/*2*/Bit16s IRQ;
	/*4*/Bit16s DMA_8_bit;
	/*6*/Bit16s DMA_16_bit;
	/*8*/Bit32s IO_reserved[4];
	//24
}
IO_PARMS;

typedef MSS_STRUCT                      // Standard MSS 3.X VDI driver header
{
	/*0*/Bit8s ID[8];
	/*8*/Bit16u driver_version;
	/*10*/Bit16u VDI_HDR_var10;
	/*12*/Bit16u VDI_HDR_var12;
	/*14*/Bit16u num_IO_configurations;//03
	/*16*/void* environment_string;//2c380506	
	//20
	/*22*/IO_PARMS IO;//22
	/*20*/Bit16u scratch0;
	/*46*/Bit16s VDI_HDR_var46;//busy
	/*48*/Bit16u VDI_HDR_var48;//driver_num
	/*50*/Bit16u VDI_HDR_var50;
	/*52*/Bit16u VDI_HDR_var52_this_ISR;
	/*54*/Bit16u VDI_HDR_var54_prev_ISR;
   /*58*/Bit8s scratch[122];           // Shared scratch workspace
   
   Bit8s     dev_name[80];           // Device name (VDI version >= 1.12 only)
}
VDI_HDR;

typedef MSS_STRUCT                      // Standard MSS 3.X VDI driver header
{
	/*0*/Bit8s     ID[6];                  // "AIL3xxx" ID string, followed by ^Z

	/*6*/Bit32u    driver_version;

   /*10*/REALFAR  common_IO_configurations;
   /*14*/Bit16u    num_IO_configurations;

   /*16*/REALFAR  environment_string;

   /*20*/IO_PARMS IO;

   /*44*/Bit16s     service_rate;

   /*46*/Bit16u    busy;

   /*48*/Bit16u    driver_num;            // Driver number

   /*50*/Bit16u    this_ISR;               // Offset of INT 66H dispatcher
   ///*52*/REALFAR  prev_ISR;               // Pointer to previous INT 66H ISR
   /*52*/Bit16u    prev_ISR;
   /*54*/Bit8u*    VDI_HDR_var54;
   /*56*/Bit8s     scratch[124];           // Shared scratch workspace

   Bit8s     dev_name[80];           // Device name (VDI version >= 1.12 only)
}
VDI_HDRorig;

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
/*0*/REALFAR AIL_DRIVER_var0_seg;//remove this! segment must be zero!
/*4*/Bit32u AIL_DRIVER_var1_sel;
/*8*/Bit8u* AIL_DRIVER_var2_buf;
/*12*/Bit32u AIL_DRIVER_var3_size;
/*16*/VDI_HDR* AIL_DRIVER_var4_VHDR;
/*20*/Bit32s AIL_DRIVER_var5;
/*24*/Bit32s AIL_DRIVER_var6;
/*28*/Bit32s AIL_DRIVER_var7;
/*32*/Bit32s AIL_DRIVER_var8;
/*36*/void(*AIL_DRIVER_var9)(HDIGDRIVER);
/*40*/void* AIL_DRIVER_var10;
/*44*/
}
AIL_DRIVER;

typedef MSS_STRUCT                   // Initialization file structure
{
   char     device_name[128];    // Device name
   char     driver_name[128];    // Driver filename
   IO_PARMS IO;                  // I/O parameters for driver
}
AIL_INI;

typedef MSS_STRUCT                   // Initialization file structure
{
   /*0*/AIL_DRIVER* var0_aildrv;
	/*1*/Bit32s var1_aildrv;
	/*2*/Bit32u var2_aildrv;
	 Bit32u var3_aildrv;
	/*2*/Bit32u var4_aildrv;
	Bit32u var5_aildrv;
	Bit32u var6_aildrv;
	Bit32u var7_aildrv;
	/*13*/Bit32u var8_aildrv;
	/*13*/Bit32u var9_aildrv;
	/*13*/Bit32u var10_aildrv;
	 Bit32u var11_aildrv;
	/*13*/Bit32u var13_aildrv;
	/*13*/Bit32u var14_aildrv;
	/*13*/Bit32u var15_aildrv;
	/*13*/Bit32u var16_aildrv;
	/*13*/Bit32u var17_aildrv;
	/*13*/Bit32u var18_aildrv;
	/*13*/Bit32u var19_aildrv;
	/*20*/ Bit8u* var20_aildrv_dig_drv;
	/*13*/Bit32u var21_aildrv;
	/*13*/Bit32u var22_aildrv;
	/*13*/Bit8u* var23_aildrv;
	/*13*/Bit32u var24_aildrv;
	/*13*/Bit32u var25_aildrv;
	/*13*/Bit32u var26_aildrv;
	/*13*/Bit32u var36_aildrv;
	/*13*/Bit32u var40_aildrv;
	/*13*/Bit32u var46_aildrv;
  
  ///*28/*/ Bit16s var28_aildrv;
  /*4*/	Bit8u scratch[144];
}
unkstr1;

#endif //AIL_SOUND_
