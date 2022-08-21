//ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
//лл                                                                       лл
//лл   GEN.H                                                               лл
//лл                                                                       лл
//лл   General-purpose resources                                           лл
//лл                                                                       лл
//лл   Version 1.00 of 04-Jun-91: Initial version (Large model)            лл
//лл           1.01 of 22-Sep-91: file_size() now type long                лл
//лл           1.02 of 14-Oct-91: C++ compatibility added                  лл
//лл                                                                       лл
//лл   C function prototypes                                               лл
//лл   Source compatible with Turbo C++ v1.0 or later                      лл
//лл                                                                       лл
//ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл
//лл                                                                       лл
//лл   Copyright (C) 1991, 1992 Miles Design, Inc.                         лл
//лл                                                                       лл
//лл   Miles Design, Inc.                                                  лл
//лл   10926 Jollyville #308                                               лл
//лл   Austin, TX 78759                                                    лл
//лл   (512) 345-2642 / FAX (512) 338-9630 / BBS (512) 454-9990            лл
//лл                                                                       лл
//ллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллллл

#ifndef GEN_H
#define GEN_H

//
// Universal error codes
//

#define NO_ERROR 0
#define IO_ERROR 1
#define OUT_OF_MEMORY 2
#define FILE_NOT_FOUND 3
#define CANT_WRITE_FILE 4
#define CANT_READ_FILE 5
#define DISK_FULL 6

#ifdef __cplusplus
extern "C" {
#endif

//
// Disk I/O services
//

int cdecl get_disk_error(void);
long cdecl file_size(char far *filename);
char far * cdecl read_file(char far *filename, void far *dest);
char far * cdecl load_driver(char far *filename);
int cdecl write_file(char far *filename, void far *src, unsigned long len);
int cdecl append_file(char far *filename, void far *src, unsigned long len);

//        
// Miscellaneous services
//

void far * cdecl norm(void far *farptr);
void far * cdecl denorm(void far *farptr);
void far * cdecl add_ptr(void far *farptr, long offset);
void far * cdecl far_memmove(void far *dest, void far *src, unsigned long len);
long cdecl ptr_dif(void far *sub2, void far *sub1);

unsigned long cdecl wswap(unsigned long n);
unsigned cdecl bswap(unsigned n);
int cdecl array_DDA(int far *a0, int far *a1, int arysiz);
long cdecl val(char *str, int base);

#ifdef __cplusplus
}
#endif

#endif
