#if !defined(_WIN64) && !defined(_XBOX)

#include <windows.h>
#include <excpt.h>

typedef struct EXCEPT_REG
{
  unsigned int prev;
  unsigned int handler;
  unsigned int had_error;
  unsigned int _ebx,_ecx,_esi,_edi,_ebp,_esp,_eip;
} EXCEPT_REG;

#pragma warning(disable : 4733)

#ifdef __cplusplus
extern "C" {
#endif

EXCEPTION_DISPOSITION __cdecl rad_handler(
    struct _EXCEPTION_RECORD *ExceptionRecord,
    void * EstablisherFrame,
    struct _CONTEXT *ContextRecord,
    void * DispatcherContext );

#ifdef __cplusplus
}
#endif

#define RAD_try { EXCEPT_REG er;  \
__asm {                           \
 __asm  mov eax,fs:[0]            \
 __asm  mov [er.prev],eax         \
 __asm  lea eax,rad_handler       \
 __asm  mov [er.handler],eax      \
 __asm  mov [er.had_error],0      \
 __asm  mov [er._ebx],ebx         \
 __asm  mov [er._ecx],ecx         \
 __asm  mov [er._esi],esi         \
 __asm  mov [er._edi],edi         \
 __asm  mov [er._ebp],ebp         \
 __asm  mov [er._esp],esp         \
 __asm  mov edx,esp               \
 __asm  lea eax,[er]              \
 __asm  mov fs:[0],eax            \
 __asm  lea esp,[er._eip+4]       \
 __asm  call next                 \
 __asm  next:                     \
 __asm  mov esp,edx               \
}                                 \
switch ( er.had_error ) { case 0:


#define RAD_except( err ) break; err :
    
#define RAD_end break; } __asm { __asm mov eax,[er.prev] __asm mov fs:[0],eax } }

#endif
