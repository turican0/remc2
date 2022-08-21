#if 1

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <setjmp.h>
#include <direct.h>



#define U64 unsigned __int64
#define S64 signed __int64
#define U32 unsigned int
#define U16 unsigned short

typedef void (__cdecl *_PVFV)(void);
typedef unsigned int size_t;


__declspec(naked) U64 _allmul(U64 value1, U64 value2)
{
  __asm
  {
    mov edx,[esp+8]
    mov ecx,[esp+16]
    or edx,ecx
    jnz full

    mov eax,[esp+4]
    mov ecx,[esp+12]
    mul ecx
    ret 16

    align 16
   full:
    mov eax,[esp+4]
    mul ecx
    mov ecx,eax

    mov eax,[esp+12]
    mul dword ptr [esp+8]
    add ecx,eax
    mov eax,[esp+12]
    mul dword ptr [esp+4]
    add edx,ecx
    ret 16 
  }
}

__declspec(naked) U64 _aulldiv(U64 A, U64 B)
{
  __asm
  {
    mov eax,[esp+8]
    mov ecx,[esp+16]
    mov edx,eax
    or eax,ecx
    jnz large
    
    mov eax,[esp+4]
    mov ecx,[esp+12]

    cmp eax,ecx
    jb smaller

    xor edx,edx
    div ecx
    xor edx,edx
    ret 16   

    align 16
   large:
    cmp edx,ecx
    jb smaller

    cmp ecx,0xf
    ja mid

    or ecx,ecx
    jz biglittle
    
    sub esp,16

    mov [esp],ebx

    mov eax,[esp+4+16]
    mov ebx,[esp+12+16]

    shr ecx,1
    rcr ebx,1
    shr edx,1
    rcr eax,1
    or ecx,ecx
    jz cont
    shr ecx,1
    rcr ebx,1
    shr edx,1
    rcr eax,1
    or ecx,ecx
    jz cont
    shr ecx,1
    rcr ebx,1
    shr edx,1
    rcr eax,1
    or ecx,ecx
    jz cont
    shr ecx,1
    rcr ebx,1
    shr edx,1
    rcr eax,1
    jmp cont
     
   mid:
    mov eax,edx
    xor edx,edx

    sub esp,16
    mov [esp],ebx
    mov ebx,ecx

    add ecx,ecx
    jc cont

    bsr ecx,ecx

    mov edx,[esp+12+16]
    shrd edx,ebx,cl
    mov ebx,edx

    mov edx,eax
    mov eax,[esp+4+16]
    shrd eax,edx,cl
    shr edx,cl

  cont:
    div ebx

    mov ebx,eax
    mov [esp+4],eax

    mov eax,[esp+16+16]
    mul ebx
    or edx,edx
    jnz wrap
    mov ecx,eax
    mov eax,[esp+12+16]
    mul ebx
    add edx,ecx

    mov ecx,[esp+4+16]
    mov ebx,[esp+8+16]

    sub ecx, eax
    sbb ebx, edx

    js wrap
    
    sub ecx, [esp+12+16]
    sbb ebx, [esp+16+16]

    mov eax,[esp+4]

    jae undershot
    
    xor edx,edx
    mov ebx,[esp]
    add esp,16
    ret 16

    align 16
   undershot:
    // we undershot by one
    inc eax
    xor edx,edx

    mov ebx,[esp]
    add esp,16
    ret 16

    align 16
   wrap:  // we overshot by one
    mov eax,[esp+4]
    xor edx,edx
    mov ebx,[esp]
    dec eax
    add esp,16
    ret 16

    align 16
   smaller:
    xor eax,eax
    xor edx,edx
    ret 16

    align 16
   biglittle:
    mov ecx,[esp+12]
    xor edx,edx
    div ecx
    push eax
    mov eax,[esp+4+4]
    div ecx
    pop edx
    ret 16
  }
}

__declspec(naked) S64 _alldiv(S64 value1, S64 value2)
{
  __asm
  {
    mov eax,[esp+8]
    mov ecx,[esp+16]
    mov edx,eax
    or eax,ecx
    jnz large
    
    mov eax,[esp+4]
    mov ecx,[esp+12]

    cmp eax,ecx
    jb smaller

    xor edx,edx
    div ecx
    xor edx,edx
    ret 16   

    align 16
   large:
    cmp edx,ecx
    jb smaller

    push ebx
    
    cmp ecx,0xf
    ja mid

    or ecx,ecx
    jz biglittle
    
    mov eax,[esp+4+4]
    mov ebx,[esp+12+4]

    shr ecx,1
    rcr ebx,1
    shr edx,1
    rcr eax,1
    or ecx,ecx
    jz cont
    shr ecx,1
    rcr ebx,1
    shr edx,1
    rcr eax,1
    or ecx,ecx
    jz cont
    shr ecx,1
    rcr ebx,1
    shr edx,1
    rcr eax,1
    or ecx,ecx
    jz cont
    shr ecx,1
    rcr ebx,1
    shr edx,1
    rcr eax,1
    jmp cont
     
   mid:
    mov eax,edx
    xor edx,edx

    mov ebx,ecx

    add ecx,ecx
    jc cont

    bsr ecx,ecx

    mov edx,[esp+12+4]
    shrd edx,ebx,cl
    mov ebx,edx

    mov edx,eax
    mov eax,[esp+4+4]
    shrd eax,edx,cl
    shr edx,cl

  cont:
    div ebx

    mov ebx,eax
    push eax

    mov eax,[esp+16+4+4]
    mul ebx
    or edx,edx
    jnz wrap
    mov ecx,eax
    mov eax,[esp+12+4+4]
    mul ebx
    add edx,ecx

    mov ecx,[esp+4+4+4]
    mov ebx,[esp+8+4+4]

    sub ecx, eax
    sbb ebx, edx

    js wrap
    
    sub ecx, [esp+12+4+4]
    sbb ebx, [esp+16+4+4]

    pop eax

    jae undershot

    xor edx,edx
    pop ebx
    ret 16

    align 16
   undershot:
    inc eax
    // we undershot by one (already subtracted one from remainder)
    xor edx,edx
    pop ebx
    ret 16

    align 16
   wrap:  // we overshot by one
    pop eax
    dec eax
    
    xor edx,edx
    pop ebx
    ret 16

    align 16
   smaller:
    xor eax,eax
    xor edx,edx
    ret 16

    align 16
   biglittle:
    mov eax,edx
    mov ecx,[esp+12]
    xor edx,edx
    div ecx
    push eax
    mov eax,[esp+4+4]
    div ecx
    pop edx
    pop ebx
    ret 16
  }
}

__declspec(naked) U64 _aullrem(U64 A, U64 B)
{
  __asm
  {
    mov eax,[esp+8]
    mov ecx,[esp+16]
    mov edx,eax
    or eax,ecx
    jnz large
    
    mov eax,[esp+4]
    mov ecx,[esp+12]

    cmp eax,ecx
    jb smaller

    xor edx,edx
    div ecx
    mov eax,edx
    xor edx,edx
    ret 16   

    align 16
   large:
    cmp edx,ecx
    jb smaller

    cmp ecx,0xf
    ja mid

    or ecx,ecx
    jz biglittle
    
    sub esp,16

    mov [esp],ebx

    mov eax,[esp+4+16]
    mov ebx,[esp+12+16]

    shr ecx,1
    rcr ebx,1
    shr edx,1
    rcr eax,1
    or ecx,ecx
    jz cont
    shr ecx,1
    rcr ebx,1
    shr edx,1
    rcr eax,1
    or ecx,ecx
    jz cont
    shr ecx,1
    rcr ebx,1
    shr edx,1
    rcr eax,1
    or ecx,ecx
    jz cont
    shr ecx,1
    rcr ebx,1
    shr edx,1
    rcr eax,1
    jmp cont
     
   mid:
    mov eax,edx
    xor edx,edx

    sub esp,16
    mov [esp],ebx
    mov ebx,ecx

    add ecx,ecx
    jc cont

    bsr ecx,ecx

    mov edx,[esp+12+16]
    shrd edx,ebx,cl
    mov ebx,edx

    mov edx,eax
    mov eax,[esp+4+16]
    shrd eax,edx,cl
    shr edx,cl

  cont:
    div ebx

    mov ebx,eax

    mov eax,[esp+16+16]
    mul ebx
    or edx,edx
    jnz wrap
    mov ecx,eax
    mov eax,[esp+12+16]
    mul ebx
    add edx,ecx

    mov ecx,[esp+4+16]
    mov ebx,[esp+8+16]

    sub ecx, eax
    sbb ebx, edx

    js wrap
    
    sub ecx, [esp+12+16]
    sbb ebx, [esp+16+16]

    jae undershot

    add ecx, [esp+12+16] // add back in our undershoot
    adc ebx, [esp+16+16]
    
    mov eax,ecx
    mov edx,ebx
    mov ebx,[esp]
    add esp,16
    ret 16

    align 16
   undershot:
    // we undershot by one (already subtracted one)
    mov eax,ecx
    mov edx,ebx
    mov ebx,[esp]
    add esp,16
    ret 16

    align 16
   wrap:  // we overshot by one
    add ecx, [esp+12+16]
    adc ebx, [esp+16+16]

    mov eax,ecx
    mov edx,ebx
    mov ebx,[esp]
    add esp,16
    ret 16

    align 16
   smaller:
    mov eax,[esp+4]
    ret 16

    align 16
   biglittle:
    mov eax,edx
    mov ecx,[esp+12]
    xor edx,edx
    div ecx
    mov eax,[esp+4]
    div ecx
    mov eax,edx
    xor edx,edx
    ret 16
  }
}

__declspec(naked) U64 _allrem(U64 value1, U64 value2)
{
  __asm
  {
    sub esp,12
    mov [esp],ebx

    mov ebx,[esp+12+12]
    mov ecx,[esp+16+12]
    mov eax,[esp+4+12]
    mov edx,[esp+8+12]

    sar edx,31
    mov [esp+4],edx

    mov edx,ecx
    sar edx,31
    xor ecx,edx
    xor ebx,edx
    sub ebx,edx
    sbb ecx,edx
    mov edx,[esp+8+12]
    mov [esp+12+12],ebx

    mov ebx,edx
    sar ebx,31
    xor edx,ebx
    xor eax,ebx
    sub eax,ebx
    sbb edx,ebx
    mov [esp+4+12],eax

    mov eax,edx

    or eax,ecx
    jnz large

    mov eax,[esp+4+12]
    mov ecx,[esp+12+12]

    cmp eax,ecx
    jb smaller

    xor edx,edx
    div ecx
    mov eax,edx
    mov edx,[esp+4]
    xor eax,edx
    sub eax,edx
    sbb edx,edx

    mov ebx,[esp]
    add esp,12
    ret 16   

    align 16
   large:
    cmp edx,ecx
    jb smaller

    cmp ecx,0xf
    mov [esp+8+12],edx
    mov [esp+16+12],ecx
    ja mid

    or ecx,ecx
    jz biglittle
    
    mov eax,[esp+4+12]
    mov ebx,[esp+12+12]

    shr ecx,1
    rcr ebx,1
    shr edx,1
    rcr eax,1
    or ecx,ecx
    jz cont
    shr ecx,1
    rcr ebx,1
    shr edx,1
    rcr eax,1
    or ecx,ecx
    jz cont
    shr ecx,1
    rcr ebx,1
    shr edx,1
    rcr eax,1
    or ecx,ecx
    jz cont
    shr ecx,1
    rcr ebx,1
    shr edx,1
    rcr eax,1
    jmp cont
     
   mid:
    mov eax,edx
    xor edx,edx

    mov ebx,ecx

    add ecx,ecx
    jc cont

    bsr ecx,ecx

    mov edx,[esp+12+12]
    shrd edx,ebx,cl
    mov ebx,edx

    mov edx,eax
    mov eax,[esp+4+12]
    shrd eax,edx,cl
    shr edx,cl

  cont:
    div ebx

    mov ebx,eax
    mov [esp+8],eax

    mov eax,[esp+16+12]
    mul ebx
    or edx,edx
    jnz wrap
    mov ecx,eax
    mov eax,[esp+12+12]
    mul ebx
    add edx,ecx

    mov ecx,[esp+4+12]
    mov ebx,[esp+8+12]

    sub ecx, eax
    sbb ebx, edx

    js wrap
    
    sub ecx, [esp+12+12]
    sbb ebx, [esp+16+12]

    mov eax,[esp+8]

    jae undershot
    
    add ecx, [esp+12+12] // add back in our undershoot
    adc ebx, [esp+16+12]
    
    mov eax,ecx
    mov edx,ebx

    mov ebx,[esp+4]
    xor edx,ebx
    xor eax,ebx
    sub eax,ebx
    sbb edx,ebx
    
    mov ebx,[esp]
    add esp,12
    ret 16

    align 16
   undershot:
    // we undershot by one
    mov eax,ecx
    mov edx,ebx

    mov ebx,[esp+4]
    xor edx,ebx
    xor eax,ebx
    sub eax,ebx
    sbb edx,ebx

    mov ebx,[esp]
    add esp,12
    ret 16

    align 16
   wrap:  // we overshot by one
    add ecx, [esp+12+12]
    adc ebx, [esp+16+12]

    mov eax,ecx
    mov edx,ebx

    mov ebx,[esp+4]
    xor edx,ebx
    xor eax,ebx
    sub eax,ebx
    sbb edx,ebx
    mov ebx,[esp]
    add esp,12
    ret 16

    align 16
   smaller:
    mov eax,[esp+4+12]
    mov ebx,[esp+4]
    xor edx,ebx
    xor eax,ebx
    sub eax,ebx
    sbb edx,ebx
    mov ebx,[esp]
    add esp,12
    ret 16

    align 16
   biglittle:
    mov ecx,[esp+12+12]
    xor edx,edx
    div ecx
    mov eax,[esp+4+12]
    div ecx

    mov eax,edx
    mov edx,[esp+4]
    xor eax,edx
    sub eax,edx
    sbb edx,edx

    mov ebx,[esp]
    add esp,12
    ret 16
  }
}

void __declspec(naked) _aulldvrm(U64 value, U64 shift)
{
  __asm
  {
    mov eax,[esp+8]
    mov ecx,[esp+16]
    mov edx,eax
    or eax,ecx
    jnz large
    
    mov eax,[esp+4]
    mov ecx,[esp+12]

    cmp eax,ecx
    jb smaller

    xor edx,edx
    div ecx
    mov ecx,edx
    xor edx,edx
    xor ebx,ebx
    ret 16   

    align 16
   large:
    cmp edx,ecx
    jb smaller

    cmp ecx,0xf
    ja mid

    or ecx,ecx
    jz biglittle
    
    mov eax,[esp+4]
    mov ebx,[esp+12]

    shr ecx,1
    rcr ebx,1
    shr edx,1
    rcr eax,1
    or ecx,ecx
    jz cont
    shr ecx,1
    rcr ebx,1
    shr edx,1
    rcr eax,1
    or ecx,ecx
    jz cont
    shr ecx,1
    rcr ebx,1
    shr edx,1
    rcr eax,1
    or ecx,ecx
    jz cont
    shr ecx,1
    rcr ebx,1
    shr edx,1
    rcr eax,1
    jmp cont
     
   mid:
    mov eax,edx
    xor edx,edx

    mov ebx,ecx

    add ecx,ecx
    jc cont

    bsr ecx,ecx

    mov edx,[esp+12]
    shrd edx,ebx,cl
    mov ebx,edx

    mov edx,eax
    mov eax,[esp+4]
    shrd eax,edx,cl
    shr edx,cl

  cont:
    div ebx

    mov ebx,eax
    push eax

    mov eax,[esp+16+4]
    mul ebx
    or edx,edx
    jnz wrap
    mov ecx,eax
    mov eax,[esp+12+4]
    mul ebx
    add edx,ecx

    mov ecx,[esp+4+4]
    mov ebx,[esp+8+4]

    sub ecx, eax
    sbb ebx, edx

    js wrap
    
    sub ecx, [esp+12+4]
    sbb ebx, [esp+16+4]

    pop eax

    jae undershot

    add ecx, [esp+12] // add back in our undershoot
    adc ebx, [esp+16]
    xor edx,edx
    ret 16

    align 16
   undershot:
    inc eax
    // we undershot by one (already subtracted one from remainder)
    xor edx,edx
    ret 16

    align 16
   wrap:  // we overshot by one
    pop eax
    dec eax
    
    add ecx, [esp+12]
    adc ebx, [esp+16]
    
    xor edx,edx
    ret 16

    align 16
   smaller:
    mov ecx,[esp+4]
    mov ebx,edx
    xor eax,eax
    xor edx,edx
    ret 16

    align 16
   biglittle:
    mov eax,edx
    mov ecx,[esp+12]
    xor edx,edx
    div ecx
    push eax
    mov eax,[esp+4+4]
    div ecx
    mov ecx,edx
    pop edx
    xor ebx,ebx
    ret 16
  }
}

void __declspec(naked) _alldvrm(U64 value, U64 shift)
{
  __asm
  {
    sub esp,12

    mov ebx,[esp+12+12]
    mov ecx,[esp+16+12]
    mov eax,[esp+4+12]
    mov edx,[esp+8+12]

    sar edx,31
    mov [esp+4],edx
    xor edx,ecx
    sar edx,31
    mov [esp],edx

    mov edx,ecx
    sar edx,31
    xor ecx,edx
    xor ebx,edx
    sub ebx,edx
    sbb ecx,edx
    mov edx,[esp+8+12]
    mov [esp+12+12],ebx

    mov ebx,edx
    sar ebx,31
    xor edx,ebx
    xor eax,ebx
    sub eax,ebx
    sbb edx,ebx
    mov [esp+4+12],eax

    mov eax,edx

    or eax,ecx
    jnz large

    mov eax,[esp+4+12]
    mov ecx,[esp+12+12]

    cmp eax,ecx
    jb smaller

    xor edx,edx
    div ecx
    mov ecx,edx
    mov ebx,[esp+4]
    mov edx,[esp]
    xor ecx,ebx
    xor eax,edx
    sub ecx,ebx
    sbb ebx,ebx
    sub eax,edx
    sbb edx,edx

    add esp,12
    ret 16   

    align 16
   large:
    cmp edx,ecx
    jb smaller

    cmp ecx,0xf
    mov [esp+8+12],edx
    mov [esp+16+12],ecx
    ja mid

    or ecx,ecx
    jz biglittle
    
    mov eax,[esp+4+12]
    mov ebx,[esp+12+12]

    shr ecx,1
    rcr ebx,1
    shr edx,1
    rcr eax,1
    or ecx,ecx
    jz cont
    shr ecx,1
    rcr ebx,1
    shr edx,1
    rcr eax,1
    or ecx,ecx
    jz cont
    shr ecx,1
    rcr ebx,1
    shr edx,1
    rcr eax,1
    or ecx,ecx
    jz cont
    shr ecx,1
    rcr ebx,1
    shr edx,1
    rcr eax,1
    jmp cont
     
   mid:
    mov eax,edx
    xor edx,edx

    mov ebx,ecx

    add ecx,ecx
    jc cont

    bsr ecx,ecx

    mov edx,[esp+12+12]
    shrd edx,ebx,cl
    mov ebx,edx

    mov edx,eax
    mov eax,[esp+4+12]
    shrd eax,edx,cl
    shr edx,cl

  cont:
    div ebx

    mov ebx,eax
    mov [esp+8],eax

    mov eax,[esp+16+12]
    mul ebx
    or edx,edx
    jnz wrap
    mov ecx,eax
    mov eax,[esp+12+12]
    mul ebx
    add edx,ecx

    mov ecx,[esp+4+12]
    mov ebx,[esp+8+12]

    sub ecx, eax
    sbb ebx, edx

    js wrap
    
    sub ecx, [esp+12+12]
    sbb ebx, [esp+16+12]

    mov eax,[esp+8]

    jae undershot
    
    add ecx, [esp+12+12] // add back in our undershoot
    adc ebx, [esp+16+12]
    
    mov edx,[esp+4]
    xor ebx,edx
    xor ecx,edx
    sub ecx,edx
    sbb ebx,edx

    mov edx,[esp]
    xor eax,edx
    sub eax,edx
    sbb edx,edx
    
    add esp,12
    ret 16

    align 16
   undershot:
    // we undershot by one
    inc eax
    mov edx,[esp+4]
    xor ebx,edx
    xor ecx,edx
    sub ecx,edx
    sbb ebx,edx

    mov edx,[esp]
    xor eax,edx
    sub eax,edx
    sbb edx,edx

    add esp,12
    ret 16

    align 16
   wrap:  // we overshot by one
    mov eax,[esp+8]

    add ecx, [esp+12+12]
    adc ebx, [esp+16+12]

    dec eax
    mov edx,[esp+4]
    xor ebx,edx
    xor ecx,edx
    sub ecx,edx
    sbb ebx,edx

    mov edx,[esp]
    xor eax,edx
    sub eax,edx
    sbb edx,edx

    add esp,12
    ret 16

    align 16
   smaller:
    mov ebx,edx
    mov edx,[esp+4]
    mov ecx,[esp+4+12]
    xor ebx,edx
    xor ecx,edx
    sub ecx,edx
    sbb ebx,edx
    
    xor eax,eax
    xor edx,edx
    add esp,12
    ret 16

    align 16
   biglittle:
    mov ecx,[esp+12+12]
    xor edx,edx
    div ecx
    mov ebx,eax
    mov eax,[esp+4+12]
    div ecx
    mov ecx,edx
    mov edx,ebx

    mov ebx,[esp]
    xor edx,ebx
    xor eax,ebx
    sub eax,ebx
    sbb edx,ebx

    mov ebx,[esp+4]
    xor ecx,ebx
    sub ecx,ebx
    sbb ebx,ebx

    add esp,12
    ret 16
  }
}

void __declspec(naked) _allshl(S64 value, U32 shift)
{
  __asm
  {
    cmp cl,64
    jae nothing

    cmp cl,32
    jae morethandword

    shld edx, eax, cl
    shl eax, cl
    ret

    align 16
   nothing:
    xor edx,edx
    xor eax,eax
    ret

    align 16
   morethandword:
    sub cl,32
    mov edx,eax
    xor eax,eax
    shl edx,cl
    ret
  }    
}

void __declspec(naked) _allshr(S64 value, U32 shift)
{
  __asm
  {
    cmp cl,64
    jae nothing

    cmp cl,32
    jae morethandword

    shrd eax, edx, cl
    sar edx, cl
    ret

    align 16
   nothing:
    sar edx,31
    mov eax,edx
    ret

    align 16
   morethandword:
    sub cl,32
    mov eax,edx
    xor edx,edx
    sar eax,cl
    ret
  }    
}

void __declspec(naked) _aullshr(U64 value, U32 shift)
{
  __asm
  {
    cmp cl,64
    jae nothing

    cmp cl,32
    jae morethandword

    shrd eax, edx, cl
    shr edx, cl
    ret

    align 16
   nothing:
    shr edx,31
    mov eax,edx
    ret

    align 16
   morethandword:
    sub cl,32
    mov eax,edx
    xor edx,edx
    shr eax,cl
    ret
  }    
}

int
__stdcall
MessageBoxA(
    U32 hWnd,
    char const * lpText,
    char const * lpCaption,
    U32 uType);

int __stdcall GetCursorPos( void * addr);

#define MB_OKCANCEL         0x0001
#define MB_ICONHAND         0x0010
#define MB_TASKMODAL        0x2000
#define IDCANCEL        2

void* __stdcall GetCurrentProcess( void );

U32 __stdcall GetTickCount( void );

int __stdcall TerminateProcess( void * hProcess, U32 uExitCode );

static __declspec(naked) void stk_hoser( void )
{
  __asm
  {
    push ebp
    mov ebp,esp
    push eax
    push ebx
    push ecx
    push edx
    push esi
    push edi
  }    
    
#ifndef FORNONWIN
  if ( MessageBoxA( 0, 
         "A stack error has been encountered. Hit OK to close the application, or cancel to trigger a breakpoint to debug...", 
         "Stack overwrite...", MB_ICONHAND | MB_OKCANCEL | MB_TASKMODAL ) == IDCANCEL )
#endif
  {
    __asm
    {
      pop edi
      pop esi
      pop edx
      pop ecx
      pop ebx
      pop eax
      int 3
      pop ebp
      ret
    }
  }
#ifndef FORNONWIN
  else
  {
    TerminateProcess(GetCurrentProcess(),0xff);
  }
#endif
}

extern unsigned long __security_cookie=0x56463489;

void __declspec(naked) __fastcall __security_check_cookie(unsigned long cookie)
{
  __asm 
  {
    cmp ecx, dword ptr [__security_cookie]
    jne crash
    rep ret 
   crash:
    jmp stk_hoser
  }
}

__declspec(noinline) void __cdecl __security_init_cookie( void )
{          
  static int first=1;
  if ( first )
  {
    first = 0;
    
    __asm
    {
      rdtsc
      rol eax,3
      xor dword ptr [__security_cookie],eax
      rol edx,17
      xor dword ptr [__security_cookie],edx
    }  
#ifndef FORNONWIN
    {
    U32 v[2];
    GetCursorPos(&v);
    __asm
    {
      mov eax,dword ptr [v]
      rol eax,5
      xor dword ptr [__security_cookie],eax

      mov eax,dword ptr [v+4]
      rol eax,15
      xor dword ptr [__security_cookie],eax

      mov eax,dword ptr fs:[0x20]
      rol eax,9
      xor dword ptr [__security_cookie],eax
      mov eax,dword ptr fs:[0x24]
      rol eax,11
      xor dword ptr [__security_cookie],eax
    }
    __security_cookie ^= GetTickCount();
    }
#endif
    do
    {
      __asm
      {
        rdtsc
        rol eax,9
        xor dword ptr [__security_cookie],eax
        rol eax,23
        xor dword ptr [__security_cookie],edx
      }    
    } while ( ( __security_cookie & 0xffff ) == 0 );
      
  }
}

void __declspec(naked) _chkesp(void)
{
  __asm
  {
    jne crash
    rep ret 
   crash:
    jmp stk_hoser
  }
}

#define _PAGESIZE_ 0x1000

void __declspec(naked) _chkstk( size_t bytes)
{
  __asm
  {
        cmp     eax, _PAGESIZE_         ; more than one page?
        jae     short probesetup        ;   yes, go setup probe loop
                                        ;   no
        neg     eax                     ; compute new stack pointer in eax
        add     eax,esp
        add     eax,4
        test    dword ptr [eax],eax     ; probe it
        xchg    eax,esp
        mov     eax,dword ptr [eax]
        push    eax
        ret

probesetup:
        push    ecx                     ; save ecx
        lea     ecx,[esp] + 8           ; compute new stack pointer in ecx
                                        ; correct for return address and
                                        ; saved ecx

probepages:
        sub     ecx,_PAGESIZE_          ; yes, move down a page
        sub     eax,_PAGESIZE_          ; adjust request and...

        test    dword ptr [ecx],eax     ; ...probe it

        cmp     eax,_PAGESIZE_          ; more than one page requested?
        jae     short probepages        ; no

//lastpage:
        sub     ecx,eax                 ; move stack down by eax
        mov     eax,esp                 ; save current tos and do a...

        test    dword ptr [ecx],eax     ; ...probe in case a page was crossed

        mov     esp,ecx                 ; set the new stack pointer

        mov     ecx,dword ptr [eax]     ; recover ecx
        mov     eax,dword ptr [eax + 4] ; recover return address

        push    eax                     ; prepare return address
                                        ; ...probe in case a page was crossed
        ret
  }
}

void __declspec(naked) _alloca_probe_16( size_t bytes)
{
  __asm
  {
        push    ecx
        lea     ecx, [esp] + 8          ; TOS before entering this function
        sub     ecx, eax                ; New TOS
        and     ecx, (16 - 1)           ; Distance from 16 bit align (align down)
        add     eax, ecx                ; Increase allocation size
        sbb     ecx, ecx                ; ecx = 0xFFFFFFFF if size wrapped around
        or      eax, ecx                ; cap allocation size on wraparound
        pop     ecx                     ; Restore ecx
        jmp     _chkstk
  }
}

void __declspec(naked) _alloca_probe( size_t bytes)
{
  __asm
  {
    jmp _chkstk
  }
}


int _fltused = 1;

void __stdcall ExitProcess( int v );

void __cdecl _exit( int rterrnum )
{
  ExitProcess( rterrnum );
}

int __cdecl atexit (_PVFV func )
{
  // todo 
  return(0);
}

void __cdecl exit( int rterrnum )
{
  _exit( rterrnum );
}

#if defined( USE_MSS )
  #include "mss.h"
#elif defined( USE_BINK )
  #include "popmal.h"
#else

#endif

_CRTNOALIAS _CRTRESTRICT void * __cdecl malloc( size_t bytes )
{
  #if defined( USE_MSS )
    return AIL_mem_alloc_lock( bytes );
  #elif defined( USE_BINK )
    return popmalloc( (U32) bytes );
  #else
    return 0; //todo
  #endif
}

_CRTNOALIAS _CRTRESTRICT void * __cdecl calloc( size_t n, size_t s )
{
  void * p;

  p = malloc( n*s );
  if ( p )
  {
    memset( p, 0, n*s );
    return( p );
  }
  return( 0 );
}

_CRTNOALIAS void __cdecl free ( void * ptr )
{
  #if defined( USE_MSS )
    AIL_mem_free_lock( ptr );
  #elif defined( USE_BINK )
    popfree( ptr );
  #else
    return; //todo
  #endif
}

/*
FILE * __cdecl fopen( char const * name, char const * mode )
{
  return( 0 ); //todo
}

int __cdecl fclose( FILE * f )
{
   //todo
   return 0;
}

int __cdecl vfprintf( FILE * f, char const * fmt, va_list ap )
{
   //todo
   return 0;
}

int __cdecl sprintf( char * dest, char const * fmt, va_list ap )
{
   //todo
   return 0;
}

int __cdecl sscanf( char const * buf, char const * fmt, va_list ap )
{
   //todo
   return 0;
}


int _cdecl _getdrive( void )
{
   //todo
   return 0;
}

*/

typedef char const * LPCSTR;
typedef char * LPSTR;
typedef int BOOL;

__declspec(dllimport) BOOL __stdcall SetCurrentDirectoryA(LPCSTR);
__declspec(dllimport) U32 __stdcall GetCurrentDirectoryA(U32 len, LPSTR buf);
__declspec(dllimport) BOOL __stdcall CreateDirectoryA(LPCSTR, U32);
__declspec(dllimport) BOOL __stdcall RemoveDirectoryA(LPCSTR);

int _cdecl _chdir( char const * dir )
{
  if ( SetCurrentDirectoryA( dir ) )
    return( 0 );
  else
    return( -1 );
}


char * _cdecl _getcwd( char * buf, int len )
{
  U32 bytes = GetCurrentDirectoryA( len, buf );
  if ( ( bytes > (U32) len ) || ( bytes == 0 ) )
    return( 0 );
  else
    return( buf );
}


int _cdecl _mkdir(const char * dir)
{
  if ( CreateDirectoryA( dir, 0 ) )
    return( 0 );
  else
    return( -1 );
}

int _cdecl _rmdir(const char * dir)
{
  if ( RemoveDirectoryA( dir ) )
    return( 0 );
  else
    return( -1 );
}

int _cdecl _chdrive( int drive )
{
  char dr[ 3 ];

  dr[ 0 ] = (char)('A' + (char)drive - (char)1);
  dr[ 1 ] = ':';
  dr[ 2 ] = 0;

  return _chdir( dr );
}

int _cdecl toupper( int c )
{
  __asm
  {
    mov eax, c
    cmp eax,'a'
    jb  cont
    cmp eax,'z'
    ja  cont

    sub eax,'a'-'A'

   cont:
  }
}

int _cdecl tolower( int c )
{
  __asm
  {
    mov eax, c
    cmp eax,'A'
    jb  cont
    cmp eax,'Z'
    ja  cont

    add eax,'a'-'A'

   cont:
  }
}


double _atof( char const * str )
{
  char c;
  int neg = 0;
  int expo = 0;
  double v =0;
  double d = 1.0;
  
  c = *str++;

  while ( ( c == ' ' ) || ( c == '\t' ) )
    c = *str++;

  if ( c == '+' )
    c = *str++;

  if ( c == '-' )
  {
    c = *str++;
    neg = 1;
  }

  while ( ( c >= '0' ) && ( c <= '9' ) )
  {
    v = ( v * 10.0 ) + (double) ( c - '0' );
    c = *str++;
  }

  if ( c == '.' )
  {
    c = *str++;
    while ( ( c >= '0' ) && ( c <= '9' ) )
    {
      v = ( v * 10.0 ) + (double) ( c - '0' );
      d = d * 10.0;
      c = *str++;
    }
  }

  v = v / d;

  if ( ( c == 'e' ) || ( c == 'E' ) )
  {
    int eneg = 0;

    c = *str++;
    
    if ( c == '+' )
      c = *str++;

    if ( c == '-' )
    {
      c = *str++;
      eneg = 1;
    }

  
    while ( ( c >= '0' ) && ( c <= '9' ) )
    {
      expo = ( expo* 10 ) + c - '0';
      c = *str++;
    }
    
    if ( eneg )
      expo = -expo;
  }

  if ( expo )
    v = v * pow( 10.0, expo );

  if ( neg )
    v = -v;

  return( v );
}

//#if _MSC_VER < 1400

double atof( char const * str )
{
  return _atof( str );
}

//#endif

int _cdecl _atoi( char const * str )
{
  char c;
  int neg = 0;
  int v = 0;

  c = *str++;

  while ( ( c == ' ' ) || ( c == '\t' ) )
    c = *str++;

  if ( c == '+' )
    c = *str++;

  if ( c == '-' )
  {
    c = *str++;
    neg = 1;
  }

  while ( ( c >= '0' ) && ( c <= '9' ) )
  {
    v = ( v * 10 ) + c - '0';
    c = *str++;
  }

  if ( neg )
    v = -v;
  
  return( v );
}

//#if _MSC_VER < 1400

long _cdecl atol( char const * str )
{
  return _atoi( str );
}

int _cdecl atoi( char const * str )
{
  return _atoi( str );
}

//#endif

char * _itoa( int number, char * str, int radix )
{
  unsigned int n;
  char buf[ 36 ];
  char * s = str;
  char * b = buf + 35;

  *b = 0;
  
  if ( ( radix == 10 ) && ( number < 0 ) )
  {
    n = -number;
    *s++ = '-';
  }
  else
  {
    n = number;
  }

  do
  {
    int v;

    v = n % radix;
    n = n / radix;

    if ( v <= 9 )
    {
      *(--b)=(char) ( v+'0' );
    }
    else
    {
      *(--b)=(char) ( v-10+'a' );
    }

  } while (n);

  strcpy( s, b );
  return str;
}

#if _MSC_VER < 1400

char * itoa( int number, char * str, int radix )
{
  return _itoa( number, str, radix );
}

#endif

char * _ltoa( long number, char * str, int radix )
{
  return _itoa( number, str, radix );
}

/*
typedef char * va_list;

int _vsnprintf(
   char *buffer,
   size_t count,
   const char *format,
   va_list argptr 
)
{
  return( 0 );
}

int vsprintf(
   char *buffer,
   const char *format,
      va_list argptr 
)
{
  return( 0 );
}

char * asctime( const struct tm * timeptr )
{
  return( 0 ); //todo
}

struct tm * localtime( const time_t * timer )
{
  return( 0 ); //todo
}

time_t time( time_t * timer )
{
  time_t ret={0}; //todo
  return( ret );
}


void longjmp( jmp_buf env, int value )
{
}

int _setjmp3( jmp_buf env )
{
  return( 0 );
}
*/


#pragma function(fabs,pow,sinh,log,log10,sin,cos,sqrt,memset,memcpy,strlen,strcmp,strcpy,acos,fmod,strcat)

#if _MSC_VER >= 1400
#pragma function(floor)
#endif


double __declspec(naked) _CIsqrt(double Value)
{
  __asm
  {
    fsqrt
    ret
  }
}

#pragma warning( disable : 4725)

double __declspec(naked) _CItan(double Value)
{
  __asm
  {
    fptan
    fstp ST(0)
    ret
  }
}

double __declspec(naked) _CIsin(double Value)
{
  __asm
  {
    fsin
    ret
  }
}

double __declspec(naked) _CIcos(double Value)
{
  __asm
  {
    fcos
    ret
  }
}

double __declspec(naked) _CIlog10(double Value)
{
  __asm
  {
    fldlg2
    fxch st(1)
    fyl2x
    ret
  }
}

double __declspec(naked) _CIlog(double Value)
{
  __asm
  {
    fldln2
    fxch
    fyl2x
    ret
  }
}

double __declspec(naked) _CIfabs(double Value)
{
  __asm
  {
    fabs
    ret
  }
}


double __declspec(naked) _CIexp(double Value)
{
  __asm
  {
    sub esp,8

    fstcw [esp]
    mov eax,[esp]
    or eax,0xc0
    mov [esp+4],eax
    fldcw [esp+4]

    fldl2e
    fmulp    st(1), st(0)

    fld      st(0)
    frndint
    fxch     st(1)
    fsub     st(0), st(1)
    f2xm1

    fldcw [esp]

    fld1
    faddp    st(1), st(0)
    fscale
    fstp st(1)
    add esp,8
    ret
  }
}

double __declspec(naked) _CIatan2(double Value, double v2)
{
  __asm
  {
    fpatan
    ret
  }
}

double __declspec(naked) _CIatan(double Value)
{
  __asm
  {
    fld1 
    fpatan
    ret
  }
}

static double half=0.5;

double __declspec(naked) _CIpow(double Value,double pw)
{
  __asm
  {
    sub esp,8

    fxch ST(1)
    fldz
    fcomp
    fnstsw ax
    test ah,044h
    jnp zero
    test ah,041h
    jne pos

    fabs

    fld st(1)
    fxch st(1)
    fyl2x

    fstcw [esp]
    mov eax,[esp]
    or eax,0xc0
    mov [esp+4],eax
    fldcw [esp+4]

    fld      st(0)
    frndint
    fxch     st(1)
    fsub     st(0), st(1)
    f2xm1
    fld1
    faddp    st(1), st(0)
    fscale
    fstp st(1)

    fxch st(1)
    fld1
    fld st(1)
    fmul half
    frndint
    fscale
    fstp st(1)

    fldcw [esp]

    fcompp
    fnstsw ax
    test ah,044h
    jnp done
    fchs
    add esp,8
    ret

    align 16
  pos:
    fyl2x

    fstcw [esp]
    mov eax,[esp]
    or eax,0xc0
    mov [esp+4],eax
    fldcw [esp+4]

    fld      st(0)
    frndint
    fxch     st(1)
    fsub     st(0), st(1)
    f2xm1

    fldcw [esp]

    fld1
    faddp    st(1), st(0)
    fscale
    fstp st(1)
  done:  
    add esp,8
    ret

  zero:    
    fstp st(1)
    add esp,8
    ret    
  }
}


double __declspec(naked) _CIacos(double Value)
{
  __asm
  {
    fld st(0)
    fmul st(0),st(0)
    fld1
    fsubr
    fsqrt
    fxch
    fpatan
    ret
  }
}

double __declspec(naked) ceil(double Value)
{
  __asm
  {
    sub esp,8
    fnstcw [esp]
    mov eax,[esp]
    fld qword ptr [esp+4+8]
    or eax,0x800
    and eax,0xfbff
    mov [esp+4],eax
    fldcw [esp+4]
    frndint
    fldcw [esp]
    add esp,8
    ret
  }    
}

double __declspec(naked) floor(double Value)
{
  __asm
  {
    sub esp,8
    fnstcw [esp]
    mov eax,[esp]
    fld qword ptr [esp+4+8]
    or eax,0x400
    and eax,0xf7ff
    mov [esp+4],eax
    fldcw [esp+4]
    frndint
    fldcw [esp]
    add esp,8
    ret
  }    
}


extern void * __safe_se_handler_table[]; /* base of safe handler entry table */
extern unsigned char  __safe_se_handler_count;   /* absolute symbol whose address is
                                                    the count of table entries */

typedef struct {
    U32       Size;
    U32       TimeDateStamp;
    U16       MajorVersion;
    U16       MinorVersion;
    U32       GlobalFlagsClear;
    U32       GlobalFlagsSet;
    U32       CriticalSectionDefaultTimeout;
    U32       DeCommitFreeBlockThreshold;
    U32       DeCommitTotalFreeThreshold;
    U32       LockPrefixTable;            // VA
    U32       MaximumAllocationSize;
    U32       VirtualMemoryThreshold;
    U32       ProcessHeapFlags;
    U32       ProcessAffinityMask;
    U16       CSDVersion;
    U16       Reserved1;
    U32       EditList;                   // VA
    unsigned long * SecurityCookie;
    void *    *SEHandlerTable;
    U32       SEHandlerCount;
} IMAGE_LOAD_CONFIG_DIRECTORY32_2;

const
IMAGE_LOAD_CONFIG_DIRECTORY32_2   _load_config_used = {
    sizeof(IMAGE_LOAD_CONFIG_DIRECTORY32_2),
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    &__security_cookie,
    __safe_se_handler_table,
    (U32)(U32*) &__safe_se_handler_count
};

double __declspec(naked) ldexp(double Value, int expv )
{
  __asm
  {
    fild dword ptr [esp+12]
    fld qword ptr [esp+4]
    fscale
    fstp st(1)
    ret
  }
}

double __declspec(naked) frexp(double Value, int * expv )
{
  __asm
  {
    mov edx,[esp+12]
    mov dword ptr [edx],0
    fld qword ptr [esp+4]
    fldz
    fcomp
    fnstsw ax
    test ah,044h
    jnp zero

    fxtract
    fld1
    fchs
    fxch ST(1)
    fscale
    fxch ST(2)
    fsubrp st(1),st(0)
    fistp dword ptr [edx]
   zero:
    ret
  }
}


double __declspec(naked) fabs( double value )
{
  __asm
  {
    fld qword ptr [esp+4]
    fabs
    ret
  }
}

double __declspec(naked) sin( double value )
{
  __asm
  {
    fld qword ptr [esp+4]
    fsin
    ret
  }
}

double __declspec(naked) cos( double value )
{
  __asm
  {
    fld qword ptr [esp+4]
    fcos
    ret
  }
}

double __declspec(naked) log10( double value )
{
  __asm
  {
    fldlg2
    fld qword ptr [esp+4]
    fyl2x
    ret
  }
}

double __declspec(naked) log( double value )
{
  __asm
  {
    fldln2
    fld qword ptr [esp+4]
    fyl2x
    ret
  }
}

double __cdecl sinh( double value )
{
  double ev;
  __asm
  {
    fld value
    call _CIexp
    fstp qword ptr [ev]
  }

  return( ( ev - ( 1.0 / ev ) ) * half );
}


double __declspec(naked) acos( double value )
{
  __asm
  {
    fld qword ptr [esp+4]
    fld st(0)
    fmul st(0),st(0)
    fld1
    fsubr
    fsqrt
    fxch
    fpatan
    ret
  }
}

double __declspec(naked) pow( double value, double ev )
{
  __asm
  {
    fld qword ptr [esp+4]
    fld qword ptr [esp+12]
    jmp _CIpow
  }
}

long __declspec(naked) _ftol( double value ) // already on fp stack
{
  __asm
  {
    sub esp,16
    fnstcw [esp]
    mov eax,[esp]
    or eax,0xc00
    mov [esp+4],eax
    fldcw [esp+4]
    fistp qword ptr [esp+8]    
    fldcw [esp]
    mov eax,[esp+8]
    mov edx,[esp+12]
    add esp,16
    ret
  }
}

long __declspec(naked) _ftol2_sse( double value ) // already on fp stack
{
  __asm
  {
    jmp _ftol
  }
}

double __declspec(naked) sqrt( double value )
{
  __asm
  {
    fld qword ptr [esp+4]
    fsqrt
    ret
  }
}


double __declspec(naked) fmod( double x, double y )
{
  __asm
  {
    fld qword ptr [esp+12]
    fld qword ptr [esp+4]
    fprem
    fstp st(1)
    ret
  }
}

double __declspec(naked) modf( double x, double * ipart )
{
  __asm
  {
    fld qword ptr [esp+4]
    fld st(0)
    sub esp,8
    fnstcw [esp]
    mov eax,[esp]
    or eax,0xc00
    mov [esp+4],eax
    fldcw [esp+4]
    frndint
    fldcw [esp]
    add esp,8
    mov eax,[esp+12]
    fst qword ptr [eax]
    fsubp st(1),st(0)
    ret
  }
}

static U32 has_sse = 0;  // -1=no, 0=not tested, 1=yes
U32 memcpy_threshold_for_non_cache_pollution = 256 * 1024;
U32 memset_threshold_for_non_cache_pollution = 384 * 1024;

void * __cdecl memset(void *dst, int chr, size_t count_bytes)
{
  __asm
  {
    movzx eax, byte ptr [chr]
    mov ecx, [count_bytes] 
    mov esi,eax
    mov edi, [dst] 

    mov edx,ecx

    shl eax, 8
    or eax,esi

    shl eax, 8
    or eax,esi
    shl eax, 8
    or eax,esi

    cmp ecx, 256
    ja  big

   old:
    // align to dest
    mov ecx,4
    sub ecx,edi
    and ecx,3
    cmp edx,ecx
    jb tiny
    sub edx,ecx
    rep stosb

    mov ecx,edx
    shr ecx, 2
    and edx,3
    rep stosd

   tiny:
    mov ecx, edx
    rep stosb

    jmp done        


    // large copy - do we have SSE?
   big:
   
    cmp [has_sse],0
    jg sse_path
    jl old

    // first time called, detect sse here
    mov esi,eax
    mov eax,1
    cpuid
    shr edx,24 
    and edx,2  ; // sse = 2, no sse = 0
    sub edx,1   ; // sse = 1, no sse = -1
    mov [has_sse],edx
    mov edx,[count_bytes]
    mov eax,esi
    jl  old


   sse_path:
    // store out the dword so we can reload it in a sec
    push eax

    // first, align edi to 128 bytes
    mov ecx,128
    sub ecx,edi
    and ecx,127
    sub edx,ecx

    mov ebx,ecx
    shr ecx, 2
    and ebx,3
    rep stosd

    mov ecx, ebx
    rep stosb

    // we are now edi 128 byte aligned, and edx holds count

    movss xmm0, dword ptr [esp]
    pop eax
    sub edx,128  ; // pre-subtract 128 bytes
    shufps xmm0, xmm0,0
    
  
    // are we at the threshold to use non pollution copy?
    cmp edx,[memset_threshold_for_non_cache_pollution]
    jae largelp

    // cache polluting
    ALIGN 16
   smallestlp:
    movaps qword ptr [edi], xmm0
    movaps qword ptr [edi+16], xmm0
    movaps qword ptr [edi+32], xmm0
    movaps qword ptr [edi+48], xmm0
    sub edx,128
    movaps qword ptr [edi+64], xmm0
    movaps qword ptr [edi+80], xmm0
    movaps qword ptr [edi+96], xmm0
    movaps qword ptr [edi+112], xmm0
    lea edi,[edi+128]
    jge smallestlp
    jmp last


    // non cache polluting
    ALIGN 16
   largelp:
    movntps qword ptr [edi], xmm0
    movntps qword ptr [edi+16], xmm0
    movntps qword ptr [edi+32], xmm0
    movntps qword ptr [edi+48], xmm0
    sub edx,128
    movntps qword ptr [edi+64], xmm0
    movntps qword ptr [edi+80], xmm0
    movntps qword ptr [edi+96], xmm0
    movntps qword ptr [edi+112], xmm0
    lea edi,[edi+128]
    jge largelp

   last:  ; // do remnant
    add edx,128
    mov ecx,edx
    shr ecx,2
    and edx,3
    rep stosd

    mov ecx, edx
    rep stosb

   done:
    mov eax,[dst]
  }
}


void * __cdecl memcpy(void *dst, void *src, size_t count_bytes)
{
  __asm
  {
    mov ecx, [count_bytes] 
    test ecx,ecx
    jz done

    mov esi, [src] 
    mov edi, [dst] 

    cmp esi,edi
    je done

    mov edx,ecx

    cmp ecx, 256
    ja  big

   old:
    // align to dest
    mov ecx,4
    sub ecx,edi
    and ecx,3
    cmp edx,ecx
    jb tiny
    sub edx,ecx
    rep movsb

    mov ecx,edx
    shr ecx, 2
    and edx,3
    rep movsd

   tiny:
    mov ecx, edx
    rep movsb

    jmp done        


    // large copy - do we have SSE?
   big:
   
    cmp [has_sse],0
    jg sse_path
    jl old

    // first time called, detect sse here
    mov eax,1
    cpuid
    shr edx,24 
    and edx,2  ; // sse = 2, no sse = 0
    sub edx,1   ; // sse = 1, no sse = -1
    mov [has_sse],edx
    mov edx,[count_bytes]
    jl  old


   sse_path:
    // first, align edi to 128 bytes
    mov ecx,4
    sub ecx,edi
    and ecx,3
    sub edx,ecx
    rep movsb

    mov ecx,128
    sub ecx,edi
    and ecx,127
    sub edx,ecx
    shr ecx, 2
    rep movsd

    // we are now edi 128 byte aligned, and edx holds count

    sub edi,esi
    sub edx,128  ; // pre-subtract 128 bytes

    // are we aligned?
    test esi,15
    jnz non_aligned
   
    // are we at the threshold to use non pollution copy?
    cmp edx,[memcpy_threshold_for_non_cache_pollution]
    jae largelp

    // cache polluting
    ALIGN 16
   smallestlp:
    movaps xmm0, qword ptr [esi]
    movaps xmm1, qword ptr [esi+16]
    movaps xmm2, qword ptr [esi+32]
    movaps xmm3, qword ptr [esi+48]
    movaps qword ptr [edi+esi], xmm0
    movaps qword ptr [edi+esi+16], xmm1
    movaps qword ptr [edi+esi+32], xmm2
    movaps qword ptr [edi+esi+48], xmm3
    movaps xmm4, qword ptr [esi+64]
    movaps xmm5, qword ptr [esi+80]
    movaps xmm6, qword ptr [esi+96]
    movaps xmm7, qword ptr [esi+112]
    sub edx,128
    movaps qword ptr [edi+esi+64], xmm4
    movaps qword ptr [edi+esi+80], xmm5
    movaps qword ptr [edi+esi+96], xmm6
    movaps qword ptr [edi+esi+112], xmm7
    lea esi,[esi+128]
    jge smallestlp
    jmp last


    // non cache polluting
    ALIGN 16
   largelp:
    prefetchnta [esi + 512]
    movaps xmm0, qword ptr [esi]
    movaps xmm1, qword ptr [esi+16]
    movaps xmm2, qword ptr [esi+32]
    movaps xmm3, qword ptr [esi+48]
    movaps xmm4, qword ptr [esi+64]
    movaps xmm5, qword ptr [esi+80]
    movaps xmm6, qword ptr [esi+96]
    movaps xmm7, qword ptr [esi+112]

    prefetchnta [esi + 512 + 64]
    sub edx,128
    movntps qword ptr [edi+esi], xmm0
    movntps qword ptr [edi+esi+16], xmm1
    movntps qword ptr [edi+esi+32], xmm2
    movntps qword ptr [edi+esi+48], xmm3
    movntps qword ptr [edi+esi+64], xmm4
    movntps qword ptr [edi+esi+80], xmm5
    movntps qword ptr [edi+esi+96], xmm6
    movntps qword ptr [edi+esi+112], xmm7
    lea esi,[esi+128]
    jge largelp
    jmp last

    // non-aligned
   non_aligned:
   
    // are we at the threshold to use non pollution copy?
    cmp edx,[memcpy_threshold_for_non_cache_pollution]
    jae largelpnp

    // cache polluting
    ALIGN 16
   smallestlpnp:
    movups xmm0, qword ptr [esi]
    movups xmm1, qword ptr [esi+16]
    movups xmm2, qword ptr [esi+32]
    movups xmm3, qword ptr [esi+48]
    movaps qword ptr [edi+esi], xmm0
    movaps qword ptr [edi+esi+16], xmm1
    movaps qword ptr [edi+esi+32], xmm2
    movaps qword ptr [edi+esi+48], xmm3
    movups xmm4, qword ptr [esi+64]
    movups xmm5, qword ptr [esi+80]
    movups xmm6, qword ptr [esi+96]
    movups xmm7, qword ptr [esi+112]
    sub edx,128
    movaps qword ptr [edi+esi+64], xmm4
    movaps qword ptr [edi+esi+80], xmm5
    movaps qword ptr [edi+esi+96], xmm6
    movaps qword ptr [edi+esi+112], xmm7
    lea esi,[esi+128]
    jge smallestlpnp
    jmp last


    // non cache polluting
    ALIGN 16
   largelpnp:
    prefetchnta [esi + 512]
    movups xmm0, qword ptr [esi]
    movups xmm1, qword ptr [esi+16]
    movups xmm2, qword ptr [esi+32]
    movups xmm3, qword ptr [esi+48]
    movups xmm4, qword ptr [esi+64]
    movups xmm5, qword ptr [esi+80]
    movups xmm6, qword ptr [esi+96]
    movups xmm7, qword ptr [esi+112]

    prefetchnta [esi + 512 + 64]
    sub edx,128
    movntps qword ptr [edi+esi], xmm0
    movntps qword ptr [edi+esi+16], xmm1
    movntps qword ptr [edi+esi+32], xmm2
    movntps qword ptr [edi+esi+48], xmm3
    movntps qword ptr [edi+esi+64], xmm4
    movntps qword ptr [edi+esi+80], xmm5
    movntps qword ptr [edi+esi+96], xmm6
    movntps qword ptr [edi+esi+112], xmm7
    lea esi,[esi+128]
    jge largelpnp


   last:  ; // do remnant
    add edi,esi
    add edx,128
    mov ecx,edx
    shr ecx,2
    and edx,3
    rep movsd

    mov ecx, edx
    rep movsb

   done:
    mov eax,[dst]
  }
}


__declspec(naked) void * __cdecl memmove(void *dst, void *src, size_t count_bytes)
{
  __asm
  {
    push ebp  
    mov ebp,esp 
    push ebx  
    push esi  
    push edi  
    
    mov ecx, [count_bytes] 
    test ecx,ecx
    jz done

    mov esi, [src] 
    mov edi, [dst] 

    mov edx,ecx

    cmp edi,esi
    jb forward
    je done
    
    add esi,ecx
    cmp esi,edi
    ja reverse

   forward:
    pop edi  
    pop esi  
    pop ebx  
    pop ebp  
    jmp memcpy

   reverse: 
    add edi,ecx
    
    std

    cmp ecx, 256
    ja  big

   old:
    // align to dest
    mov ecx,edi
    dec edi
    dec esi

    cmp edx,ecx
    jb tiny

    sub edx,ecx
    rep movsb

    mov ecx,edx
    and edx,3

    sub edi,3
    sub esi,3
    rep movsd
    add edi,3
    add esi,3

   tiny:
    mov ecx, edx
    rep movsb
    cld

    jmp done        


    // large copy - do we have SSE?
   big:
   
    cmp [has_sse],0
    jg sse_path
    jl old

    // first time called, detect sse here
    mov eax,1
    cpuid
    shr edx,24 
    and edx,2  ; // sse = 2, no sse = 0
    sub edx,1   ; // sse = 1, no sse = -1
    mov [has_sse],edx
    mov edx,[count_bytes]
    jl  old


   sse_path:
    // first, align edi to 128 bytes
    mov ecx,edi
    and ecx,127
    dec edi
    dec esi

    sub edx,ecx
    mov ebx,ecx
    and ecx,3
    shr ebx,2
    rep movsb

    mov ecx,ebx

    sub edi,3
    sub esi,3
    rep movsd


    // we are now edi 128 byte aligned, and edx holds count

    sub edi,127-3
    sub esi,127-3
    sub edi,esi

    sub edx,128  ; // pre-subtract 128 bytes

    // are we aligned?
    test esi,15
    jnz non_aligned
   
    // are we at the threshold to use non pollution copy?
    cmp edx,[memcpy_threshold_for_non_cache_pollution]
    jae largelp

    // cache polluting
    ALIGN 16
   smallestlp:
    movaps xmm0, qword ptr [esi]
    movaps xmm1, qword ptr [esi+16]
    movaps xmm2, qword ptr [esi+32]
    movaps xmm3, qword ptr [esi+48]
    movaps xmm4, qword ptr [esi+64]
    movaps xmm5, qword ptr [esi+80]
    movaps xmm6, qword ptr [esi+96]
    movaps xmm7, qword ptr [esi+112]
    sub edx,128
    movaps qword ptr [edi+esi], xmm0
    movaps qword ptr [edi+esi+16], xmm1
    movaps qword ptr [edi+esi+32], xmm2
    movaps qword ptr [edi+esi+48], xmm3
    movaps qword ptr [edi+esi+64], xmm4
    movaps qword ptr [edi+esi+80], xmm5
    movaps qword ptr [edi+esi+96], xmm6
    movaps qword ptr [edi+esi+112], xmm7
    lea esi,[esi-128]
    jge smallestlp
    jmp last


    // non cache polluting
    ALIGN 16
   largelp:
    prefetchnta [esi - 512]
    movaps xmm0, qword ptr [esi]
    movaps xmm1, qword ptr [esi+16]
    movaps xmm2, qword ptr [esi+32]
    movaps xmm3, qword ptr [esi+48]
    movaps xmm4, qword ptr [esi+64]
    movaps xmm5, qword ptr [esi+80]
    movaps xmm6, qword ptr [esi+96]
    movaps xmm7, qword ptr [esi+112]

    prefetchnta [esi - 512 - 64]
    sub edx,128
    movntps qword ptr [edi+esi], xmm0
    movntps qword ptr [edi+esi+16], xmm1
    movntps qword ptr [edi+esi+32], xmm2
    movntps qword ptr [edi+esi+48], xmm3
    movntps qword ptr [edi+esi+64], xmm4
    movntps qword ptr [edi+esi+80], xmm5
    movntps qword ptr [edi+esi+96], xmm6
    movntps qword ptr [edi+esi+112], xmm7
    lea esi,[esi-128]
    jge largelp
    jmp last

    // non-aligned
   non_aligned:
   
    // are we at the threshold to use non pollution copy?
    cmp edx,[memcpy_threshold_for_non_cache_pollution]
    jae largelpnp

    // cache polluting
    ALIGN 16
   smallestlpnp:
    movups xmm0, qword ptr [esi]
    movups xmm1, qword ptr [esi+16]
    movups xmm2, qword ptr [esi+32]
    movups xmm3, qword ptr [esi+48]
    movups xmm4, qword ptr [esi+64]
    movups xmm5, qword ptr [esi+80]
    movups xmm6, qword ptr [esi+96]
    movups xmm7, qword ptr [esi+112]
    sub edx,128
    movaps qword ptr [edi+esi], xmm0
    movaps qword ptr [edi+esi+16], xmm1
    movaps qword ptr [edi+esi+32], xmm2
    movaps qword ptr [edi+esi+48], xmm3
    movaps qword ptr [edi+esi+64], xmm4
    movaps qword ptr [edi+esi+80], xmm5
    movaps qword ptr [edi+esi+96], xmm6
    movaps qword ptr [edi+esi+112], xmm7
    lea esi,[esi-128]
    jge smallestlpnp
    jmp last


    // non cache polluting
    ALIGN 16
   largelpnp:
    prefetchnta [esi - 512]
    movups xmm0, qword ptr [esi]
    movups xmm1, qword ptr [esi+16]
    movups xmm2, qword ptr [esi+32]
    movups xmm3, qword ptr [esi+48]
    movups xmm4, qword ptr [esi+64]
    movups xmm5, qword ptr [esi+80]
    movups xmm6, qword ptr [esi+96]
    movups xmm7, qword ptr [esi+112]

    prefetchnta [esi - 512 - 64]
    sub edx,128
    movntps qword ptr [edi+esi], xmm0
    movntps qword ptr [edi+esi+16], xmm1
    movntps qword ptr [edi+esi+32], xmm2
    movntps qword ptr [edi+esi+48], xmm3
    movntps qword ptr [edi+esi+64], xmm4
    movntps qword ptr [edi+esi+80], xmm5
    movntps qword ptr [edi+esi+96], xmm6
    movntps qword ptr [edi+esi+112], xmm7
    lea esi,[esi-128]
    jge largelpnp


   last:  ; // do remnant
    lea edi,[edi+esi+127-3]
    add esi,127-3
    add edx,128

    mov ecx,edx
    shr ecx,2
    and edx,3
    rep movsd
    add edi,3
    add esi,3

    mov ecx, edx
    rep movsb
    cld

   done:
    mov eax,[dst]

    pop edi  
    pop esi  
    pop ebx  
    pop ebp  
    ret              
  }
}

char * __cdecl strcpy( char * s1, char const * s2 )
{
  __asm
  {
    mov ecx,[s2] 
    mov edx,[s1]
   lp:
    mov al,[ecx] 
    inc ecx
    mov [edx],al
    inc edx
    cmp al,0 
    jne lp 
    mov eax,[s1]
  }
}

size_t __cdecl strlen( char const * s1 )
{
  __asm
  {
    mov edi,s1
    mov ecx,0xffffffff
    xor eax,eax
    repne scasb
    not ecx
    dec ecx
    mov eax, ecx
  }
}

char * __cdecl strcat( char * s1, char const * s2 )
{
  __asm
  {
    mov esi,[s2] 
    mov edi,[s1]

    mov ecx,0xffffffff
    xor eax,eax
    repne scasb
    dec edi

   lp:
    mov al,[esi] 
    inc esi 
    mov [edi],al
    inc edi
    cmp al,0 
    jne lp 
    mov eax,[s1]
  }
}

int __cdecl _memcmp( void const * s1, void const * s2, size_t n )
{
  __asm
  {
    mov esi,s1
    mov edi,s2
    mov ecx,n
   lp: 
    cmp ecx,0
    je set
    
    mov al,[esi]
    mov dl,[edi]

    dec ecx
    
    cmp al,dl
    jne set

    inc esi
    inc edi

    cmp al,0
    jne lp

   set:
    mov eax,0
    mov edx,0
    seta al
    setb dl
    sub eax,edx
  }
}


void * memchr( void const * m, int chr, size_t bytes )
{
  __asm
  {
    mov esi, m
    mov edx, chr
    mov ecx, bytes

    cmp ecx,0
    jz  notfnd

    lp:
    mov al,[esi] 

    cmp al,dl
    je fnd

    inc esi 

    dec ecx
    jnz lp

   notfnd:
    xor eax,eax
    jmp done

   fnd:
    mov eax,esi

   done:
  }
}

char * strchr( char const * m, int chr )
{
  __asm
  {
    mov eax, m
    mov edx, chr

    lp:
    mov cl,[eax] 

    cmp cl,dl
    je fnd

    inc eax

    cmp cl,0
    jne lp

    xor eax,eax
   fnd:
  }
}


int __cdecl strcmp( char const * s1, char const * s2 )
{
  __asm
  {
    mov ecx,s1
    mov edi,s2
   lp: 
    mov al,[ecx]
    mov dl,[edi]
    cmp al,dl
    jne set

    inc ecx
    inc edi

    cmp al,0
    jne lp

   set:
    mov eax,0
    mov edx,0
    seta al
    setb dl
    sub eax,edx
  }
}

int __cdecl _strncmp( char const * s1, char const * s2, size_t n )
{
  __asm
  {
    mov esi,s1
    mov edi,s2
    mov ecx,n
   lp: 
    cmp ecx,0
    je set
    
    mov al,[esi]
    mov dl,[edi]

    dec ecx
    
    cmp al,dl
    jne set

    inc esi
    inc edi

    cmp al,0
    jne lp

   set:
    mov eax,0
    mov edx,0
    seta al
    setb dl
    sub eax,edx
  }
}

int __cdecl _stricmp( char const * s1, char const * s2 )
{
  __asm
  {
    mov ecx,s1
    mov edi,s2
   lp: 
    mov al,[ecx]
    mov dl,[edi]

    cmp al,'a'
    jb  chkdl
    cmp al,'z'
    ja  chkdl
    sub al,'a'-'A'

   chkdl:
    cmp dl,'a'
    jb  cont
    cmp dl,'z'
    ja  cont
    sub dl,'a'-'A'

   cont:
    cmp al,dl
    jne set

    inc ecx
    inc edi

    cmp al,0
    jne lp

   set:
    mov eax,0
    mov edx,0
    seta al
    setb dl
    sub eax,edx
  }
}

int __cdecl _strnicmp( char const * s1, char const * s2, size_t n )
{
  __asm
  {
    mov esi,s1
    mov edi,s2
    mov ecx,n
   lp: 
    cmp ecx,0
    je set
    
    mov al,[esi]
    mov dl,[edi]

    dec ecx
    
    cmp al,'a'
    jb  chkdl
    cmp al,'z'
    ja  chkdl
    sub al,'a'-'A'

   chkdl:
    cmp dl,'a'
    jb  cont
    cmp dl,'z'
    ja  cont
    sub dl,'a'-'A'

   cont:
    cmp al,dl
    jne set

    inc esi
    inc edi

    cmp al,0
    jne lp

   set:
    mov eax,0
    mov edx,0
    seta al
    setb dl
    sub eax,edx
  }
}

char * __cdecl _strupr( char * s )
{
  __asm
  {
    mov edx,[s] 
   lp:
    mov al,[edx]

    cmp al,'a'
    jb  cont
    cmp al,'z'
    ja  cont
    
    sub al,'a'-'A'
    mov [edx],al

   cont:
    inc edx

    cmp al,0 
    jne lp 
    
    mov eax,[s]
  }
}


char * __cdecl strncpy( char * s1, char const * s2, size_t n )
{
  __asm
  {
    mov edx,n
    mov ecx,[s2] 
    mov edi,[s1]
   lp:
    cmp edx, 0
    je done
    mov al,[ecx] 
    inc ecx 
    dec edx
    mov [edi],al
    inc edi
    cmp al,0 
    jne lp 
   done: 
    mov eax,[s1]
  }
}

char * _cdecl strstr( char const * s1, char const * s2 )
{
  __asm
  {
    mov esi, s1
    mov edi, s2
    mov dl, [edi]
    inc edi

   lp:
    mov al,[esi] 
    inc esi 

    cmp al,dl
    je fnd

    cmp al,0
    jne lp

    xor eax,eax
    jmp done

   fnd:
    mov ecx,esi
    mov ebx,edi

   km:
    mov al,[ecx]
    inc ecx
    mov ah,[ebx]
    inc ebx
    
    cmp al,ah
    jne lp

    cmp al,0
    jne km

    lea eax,[esi-1]
   done:
  }
}

static long lastrnd = 1;

void _cdecl srand ( unsigned int seed )
{
  lastrnd = (long) seed;
}

int _cdecl rand ( void )
{
  lastrnd = lastrnd * 214013 + 2531011;
  return( ( lastrnd >> 16 ) & 0x7fff );
}


#define DLL_PROCESS_ATTACH   1
#define STARTF_USESHOWWINDOW    0x00000001
#define SW_SHOWDEFAULT      10

typedef struct HINSTANCE__ *HINSTANCE;
typedef HINSTANCE HMODULE;      
typedef int BOOL;
typedef void * LPVOID;
typedef unsigned short WORD;
typedef unsigned char * LPBYTE;
typedef char * LPSTR;
typedef char const * LPCSTR;
typedef struct HANDLE__ *HANDLE;

typedef struct _STARTUPINFO {
    U32   cb;
    LPSTR   lpReserved;
    LPSTR   lpDesktop;
    LPSTR   lpTitle;
    U32   dwX;
    U32   dwY;
    U32   dwXSize;
    U32   dwYSize;
    U32   dwXCountChars;
    U32   dwYCountChars;
    U32   dwFillAttribute;
    U32   dwFlags;
    WORD    wShowWindow;
    WORD    cbReserved2;
    LPBYTE  lpReserved2;
    HANDLE  hStdInput;
    HANDLE  hStdOutput;
    HANDLE  hStdError;
} STARTUPINFO, *LPSTARTUPINFO;
  
__declspec(dllimport) BOOL __stdcall MSSDisableThreadLibraryCalls( HMODULE hLibModule );
__declspec(dllimport) BOOL __stdcall DisableThreadLibraryCalls( HMODULE hLibModule );
__declspec(dllimport)HMODULE __stdcall GetModuleHandleA(LPCSTR);
__declspec(dllimport) BOOL __stdcall GetStartupInfoA( STARTUPINFO * start );
__declspec(dllimport) char * __stdcall GetCommandLineA( void );

#ifdef __RADINDLL__
BOOL __stdcall DllMain( HINSTANCE h,U32 reason,LPVOID l);

BOOL __stdcall _DllMainCRTStartup( HINSTANCE hinstDll, U32 fdwReason, LPVOID plvReserved )
{

  if ( fdwReason == DLL_PROCESS_ATTACH )
  {
    __security_init_cookie();
    
    #if defined( USE_MSS )
      MSSDisableThreadLibraryCalls( hinstDll );
    #else
      DisableThreadLibraryCalls( hinstDll );
    #endif
  }

  return DllMain( hinstDll, fdwReason, plvReserved );
}

#elif !defined(__RADCONSOLE__)

int __stdcall WinMain( HINSTANCE this_inst, HINSTANCE prev_inst, LPSTR cmdline, int cmdshow );

void __cdecl WinMainCRTStartup( void )                         
{                                                              
  char *lpszCommandLine;                                       
  STARTUPINFO StartupInfo;                                     
  int cmdshow;                                                 

  __security_init_cookie();
  
  GetStartupInfoA( &StartupInfo );                              
                                                               
  lpszCommandLine = GetCommandLineA();                          
                                                               
  if ( *lpszCommandLine == '"' )  {                            
    lpszCommandLine++;                                         
    while( *lpszCommandLine && (*lpszCommandLine != '"') )     
      lpszCommandLine++;                                       
    if ( *lpszCommandLine == '"' )                             
      lpszCommandLine++;                                       
  } else {                                                     
    while ( *lpszCommandLine > ' ' )                           
      lpszCommandLine++;                                       
  }                                                            
                                                               
  while ( *lpszCommandLine && (*lpszCommandLine <= ' ') )      
    lpszCommandLine++;                            
                                                               
                                                               
  cmdshow=StartupInfo.dwFlags & STARTF_USESHOWWINDOW ?         
            StartupInfo.wShowWindow: SW_SHOWDEFAULT;           
                                                               
  ExitProcess(WinMain(GetModuleHandleA(NULL),NULL,              
                      lpszCommandLine,cmdshow));               
}

#else

int main( int argc, char** argv );

void __cdecl mainCRTStartup( void )                         
{                                                              
  char *lpszCommandLine;                                       
  STARTUPINFO StartupInfo;                                     

  __security_init_cookie();
  
  GetStartupInfoA( &StartupInfo );                              
                                                               
  lpszCommandLine = GetCommandLineA();                          
                                                               
  if ( *lpszCommandLine == '"' )  {                            
    lpszCommandLine++;                                         
    while( *lpszCommandLine && (*lpszCommandLine != '"') )     
      lpszCommandLine++;                                       
    if ( *lpszCommandLine == '"' )                             
      lpszCommandLine++;                                       
  } else {                                                     
    while ( *lpszCommandLine > ' ' )                           
      lpszCommandLine++;                                       
  }                                                            
                                                               
  while ( *lpszCommandLine && (*lpszCommandLine <= ' ') )      
    lpszCommandLine++;                            
                                                               
  ExitProcess(main(1,&lpszCommandLine));               
}

#endif

#endif

// NONOcdep pre $requiresbinary(libc.lib)
//   $replacewith(cswitches,$cswitches,-Gs, -Gs- )

/* @cdep post

   $define( STDLIBs, )
   $define( STD8LIBs, )
   $replacewith(cwinswitches,$cwinswitches,-GZ, )
   $replacewith(c8winswitches,$c8winswitches,-GZ, )

   $if($usingc8,
     $set(CsNoLTCG,$CsNoLTCG $filerequired($outofdate))
     $replacewith(Cs,$Cs,$platformify($file), )
   , )

*/

