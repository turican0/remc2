Merge_DestMono_SrcMono_Src8_NoVolume_NoResample PROC ; 0

  ; Load sample data
  xor   eax, eax
  mov   al, byte ptr [PTR_ESI]
  sub   eax, 080h
  shl   eax, 8

  ; Apply volume
  shl   eax, 11

  ; Merge sample data loop
  ALIGN 16
  merge_loop0:

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   PTR_EDI, 4
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit0

  ; Move the source pointer
  inc   PTR_ESI
  cmp   PTR_ESI, src_end
  jae   src_end_exit0

  ; Load sample data
  xor   eax, eax
  mov   al, byte ptr [PTR_ESI]
  sub   eax, 080h
  shl   eax, 8

  ; Apply volume
  shl   eax, 11

  ; End loop
  jmp   merge_loop0

  ; Jump out point if end of dest is reached
  dest_end_exit0:
  inc   PTR_ESI

  ; Jump out point if end of src is reached
  src_end_exit0:

  ; Routine end
  ret

Merge_DestMono_SrcMono_Src8_NoVolume_NoResample ENDP


Merge_DestStereo_SrcMono_Src8_NoVolume_NoResample PROC ; 1

  ; Load sample data
  xor   eax, eax
  mov   al, byte ptr [PTR_ESI]
  sub   eax, 080h
  shl   eax, 8

  ; Apply volume
  shl   eax, 11

  ; Merge sample data loop
  ALIGN 16
  merge_loop1:

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   [PTR_EDI+4], eax
  add   PTR_EDI, 8
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit1

  ; Move the source pointer
  inc   PTR_ESI
  cmp   PTR_ESI, src_end
  jae   src_end_exit1

  ; Load sample data
  xor   eax, eax
  mov   al, byte ptr [PTR_ESI]
  sub   eax, 080h
  shl   eax, 8

  ; Apply volume
  shl   eax, 11

  ; End loop
  jmp   merge_loop1

  ; Jump out point if end of dest is reached
  dest_end_exit1:
  inc   PTR_ESI

  ; Jump out point if end of src is reached
  src_end_exit1:

  ; Routine end
  ret

Merge_DestStereo_SrcMono_Src8_NoVolume_NoResample ENDP


Merge_DestMono_SrcMono_Src16_NoVolume_NoResample PROC ; 2

  ; Load sample data
  movsx eax, word ptr [PTR_ESI]

  ; Apply volume
  shl   eax, 11

  ; Merge sample data loop
  ALIGN 16
  merge_loop2:

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   PTR_EDI, 4
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit2

  ; Move the source pointer
  add   PTR_ESI, 2
  cmp   PTR_ESI, src_end
  jae   src_end_exit2

  ; Load sample data
  movsx eax, word ptr [PTR_ESI]

  ; Apply volume
  shl   eax, 11

  ; End loop
  jmp   merge_loop2

  ; Jump out point if end of dest is reached
  dest_end_exit2:
  add   PTR_ESI, 2

  ; Jump out point if end of src is reached
  src_end_exit2:

  ; Routine end
  ret

Merge_DestMono_SrcMono_Src16_NoVolume_NoResample ENDP


Merge_DestStereo_SrcMono_Src16_NoVolume_NoResample PROC ; 3

  ; Load sample data
  movsx eax, word ptr [PTR_ESI]

  ; Apply volume
  shl   eax, 11

  ; Merge sample data loop
  ALIGN 16
  merge_loop3:

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   [PTR_EDI+4], eax
  add   PTR_EDI, 8
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit3

  ; Move the source pointer
  add   PTR_ESI, 2
  cmp   PTR_ESI, src_end
  jae   src_end_exit3

  ; Load sample data
  movsx eax, word ptr [PTR_ESI]

  ; Apply volume
  shl   eax, 11

  ; End loop
  jmp   merge_loop3

  ; Jump out point if end of dest is reached
  dest_end_exit3:
  add   PTR_ESI, 2

  ; Jump out point if end of src is reached
  src_end_exit3:

  ; Routine end
  ret

Merge_DestStereo_SrcMono_Src16_NoVolume_NoResample ENDP


Merge_DestMono_SrcStereo_Src8_NoVolume_NoResample PROC ; 8

  ; Load sample data
  xor   ebx, ebx
  xor   eax, eax
  mov   ax, word ptr [PTR_ESI]
  mov   bl, ah
  and   eax, 0ffh
  sub   ebx, 080h
  sub   eax, 080h
  shl   ebx, 8
  shl   eax, 8

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; Apply volume
  shl   eax, 11

  ; Merge sample data loop
  ALIGN 16
  merge_loop8:

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   PTR_EDI, 4
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit8

  ; Move the source pointer
  add   PTR_ESI, 2
  cmp   PTR_ESI, src_end
  jae   src_end_exit8

  ; Load sample data
  xor   ebx, ebx
  xor   eax, eax
  mov   ax, word ptr [PTR_ESI]
  mov   bl, ah
  and   eax, 0ffh
  sub   ebx, 080h
  sub   eax, 080h
  shl   ebx, 8
  shl   eax, 8

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; Apply volume
  shl   eax, 11

  ; End loop
  jmp   merge_loop8

  ; Jump out point if end of dest is reached
  dest_end_exit8:
  add   PTR_ESI, 2

  ; Jump out point if end of src is reached
  src_end_exit8:

  ; Routine end
  ret

Merge_DestMono_SrcStereo_Src8_NoVolume_NoResample ENDP


Merge_DestStereo_SrcStereo_Src8_NoVolume_NoResample PROC ; 9

  ; Load sample data
  xor   ebx, ebx
  xor   eax, eax
  mov   ax, word ptr [PTR_ESI]
  mov   bl, ah
  and   eax, 0ffh
  sub   ebx, 080h
  sub   eax, 080h
  shl   ebx, 8
  shl   eax, 8

  ; Apply volume
  shl   eax, 11
  shl   ebx, 11

  ; Merge sample data loop
  ALIGN 16
  merge_loop9:

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   [PTR_EDI+4], ebx
  add   PTR_EDI, 8
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit9

  ; Move the source pointer
  add   PTR_ESI, 2
  cmp   PTR_ESI, src_end
  jae   src_end_exit9

  ; Load sample data
  xor   ebx, ebx
  xor   eax, eax
  mov   ax, word ptr [PTR_ESI]
  mov   bl, ah
  and   eax, 0ffh
  sub   ebx, 080h
  sub   eax, 080h
  shl   ebx, 8
  shl   eax, 8

  ; Apply volume
  shl   eax, 11
  shl   ebx, 11

  ; End loop
  jmp   merge_loop9

  ; Jump out point if end of dest is reached
  dest_end_exit9:
  add   PTR_ESI, 2

  ; Jump out point if end of src is reached
  src_end_exit9:

  ; Routine end
  ret

Merge_DestStereo_SrcStereo_Src8_NoVolume_NoResample ENDP


Merge_DestMono_SrcStereo_Src16_NoVolume_NoResample PROC ; 10

  ; Load sample data
  mov   ebx, dword ptr [PTR_ESI]
  movsx eax, bx
  sar   ebx, 16

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; Apply volume
  shl   eax, 11

  ; Merge sample data loop
  ALIGN 16
  merge_loop10:

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   PTR_EDI, 4
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit10

  ; Move the source pointer
  add   PTR_ESI, 4
  cmp   PTR_ESI, src_end
  jae   src_end_exit10

  ; Load sample data
  mov   ebx, dword ptr [PTR_ESI]
  movsx eax, bx
  sar   ebx, 16

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; Apply volume
  shl   eax, 11

  ; End loop
  jmp   merge_loop10

  ; Jump out point if end of dest is reached
  dest_end_exit10:
  add   PTR_ESI, 4

  ; Jump out point if end of src is reached
  src_end_exit10:

  ; Routine end
  ret

Merge_DestMono_SrcStereo_Src16_NoVolume_NoResample ENDP


Merge_DestStereo_SrcStereo_Src16_NoVolume_NoResample PROC ; 11

  ; Load sample data
  mov   ebx, dword ptr [PTR_ESI]
  movsx eax, bx
  sar   ebx, 16

  ; Apply volume
  shl   eax, 11
  shl   ebx, 11

  ; Merge sample data loop
  ALIGN 16
  merge_loop11:

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   [PTR_EDI+4], ebx
  add   PTR_EDI, 8
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit11

  ; Move the source pointer
  add   PTR_ESI, 4
  cmp   PTR_ESI, src_end
  jae   src_end_exit11

  ; Load sample data
  mov   ebx, dword ptr [PTR_ESI]
  movsx eax, bx
  sar   ebx, 16

  ; Apply volume
  shl   eax, 11
  shl   ebx, 11

  ; End loop
  jmp   merge_loop11

  ; Jump out point if end of dest is reached
  dest_end_exit11:
  add   PTR_ESI, 4

  ; Jump out point if end of src is reached
  src_end_exit11:

  ; Routine end
  ret

Merge_DestStereo_SrcStereo_Src16_NoVolume_NoResample ENDP


Merge_DestMono_SrcFlipped_Src8_NoVolume_NoResample PROC ; 24

  ; Load sample data
  xor   eax, eax
  xor   ebx, ebx
  mov   bx, word ptr [PTR_ESI]
  mov   al, bh
  and   ebx, 0ffh
  sub   eax, 080h
  sub   ebx, 080h
  shl   eax, 8
  shl   ebx, 8

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; Apply volume
  shl   eax, 11

  ; Merge sample data loop
  ALIGN 16
  merge_loop24:

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   PTR_EDI, 4
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit24

  ; Move the source pointer
  add   PTR_ESI, 2
  cmp   PTR_ESI, src_end
  jae   src_end_exit24

  ; Load sample data
  xor   eax, eax
  xor   ebx, ebx
  mov   bx, word ptr [PTR_ESI]
  mov   al, bh
  and   ebx, 0ffh
  sub   eax, 080h
  sub   ebx, 080h
  shl   eax, 8
  shl   ebx, 8

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; Apply volume
  shl   eax, 11

  ; End loop
  jmp   merge_loop24

  ; Jump out point if end of dest is reached
  dest_end_exit24:
  add   PTR_ESI, 2

  ; Jump out point if end of src is reached
  src_end_exit24:

  ; Routine end
  ret

Merge_DestMono_SrcFlipped_Src8_NoVolume_NoResample ENDP


Merge_DestStereo_SrcFlipped_Src8_NoVolume_NoResample PROC ; 25

  ; Load sample data
  xor   eax, eax
  xor   ebx, ebx
  mov   bx, word ptr [PTR_ESI]
  mov   al, bh
  and   ebx, 0ffh
  sub   eax, 080h
  sub   ebx, 080h
  shl   eax, 8
  shl   ebx, 8

  ; Apply volume
  shl   eax, 11
  shl   ebx, 11

  ; Merge sample data loop
  ALIGN 16
  merge_loop25:

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   [PTR_EDI+4], ebx
  add   PTR_EDI, 8
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit25

  ; Move the source pointer
  add   PTR_ESI, 2
  cmp   PTR_ESI, src_end
  jae   src_end_exit25

  ; Load sample data
  xor   eax, eax
  xor   ebx, ebx
  mov   bx, word ptr [PTR_ESI]
  mov   al, bh
  and   ebx, 0ffh
  sub   eax, 080h
  sub   ebx, 080h
  shl   eax, 8
  shl   ebx, 8

  ; Apply volume
  shl   eax, 11
  shl   ebx, 11

  ; End loop
  jmp   merge_loop25

  ; Jump out point if end of dest is reached
  dest_end_exit25:
  add   PTR_ESI, 2

  ; Jump out point if end of src is reached
  src_end_exit25:

  ; Routine end
  ret

Merge_DestStereo_SrcFlipped_Src8_NoVolume_NoResample ENDP


Merge_DestMono_SrcFlipped_Src16_NoVolume_NoResample PROC ; 26

  ; Load sample data
  mov   eax, dword ptr [PTR_ESI]
  movsx ebx, ax
  sar   eax, 16

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; Apply volume
  shl   eax, 11

  ; Merge sample data loop
  ALIGN 16
  merge_loop26:

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   PTR_EDI, 4
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit26

  ; Move the source pointer
  add   PTR_ESI, 4
  cmp   PTR_ESI, src_end
  jae   src_end_exit26

  ; Load sample data
  mov   eax, dword ptr [PTR_ESI]
  movsx ebx, ax
  sar   eax, 16

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; Apply volume
  shl   eax, 11

  ; End loop
  jmp   merge_loop26

  ; Jump out point if end of dest is reached
  dest_end_exit26:
  add   PTR_ESI, 4

  ; Jump out point if end of src is reached
  src_end_exit26:

  ; Routine end
  ret

Merge_DestMono_SrcFlipped_Src16_NoVolume_NoResample ENDP


Merge_DestStereo_SrcFlipped_Src16_NoVolume_NoResample PROC ; 27

  ; Load sample data
  mov   eax, dword ptr [PTR_ESI]
  movsx ebx, ax
  sar   eax, 16

  ; Apply volume
  shl   eax, 11
  shl   ebx, 11

  ; Merge sample data loop
  ALIGN 16
  merge_loop27:

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   [PTR_EDI+4], ebx
  add   PTR_EDI, 8
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit27

  ; Move the source pointer
  add   PTR_ESI, 4
  cmp   PTR_ESI, src_end
  jae   src_end_exit27

  ; Load sample data
  mov   eax, dword ptr [PTR_ESI]
  movsx ebx, ax
  sar   eax, 16

  ; Apply volume
  shl   eax, 11
  shl   ebx, 11

  ; End loop
  jmp   merge_loop27

  ; Jump out point if end of dest is reached
  dest_end_exit27:
  add   PTR_ESI, 4

  ; Jump out point if end of src is reached
  src_end_exit27:

  ; Routine end
  ret

Merge_DestStereo_SrcFlipped_Src16_NoVolume_NoResample ENDP


Merge_DestMono_SrcMono_Src8_NoVolume_Resample PROC ; 32

  ; adjust fractional
  shl   edx, 16

  ; Load sample data
  xor   eax, eax
  mov   al, byte ptr [PTR_ESI]
  sub   eax, 080h
  shl   eax, 8

  ; Apply volume
  shl   eax, 11

  ; Merge sample data loop
  ALIGN 16
  merge_loop32:

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   PTR_EDI, 4
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit32

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  sbb   PTR_EAX, PTR_EAX
  add   PTR_ESI, [STEPTBL_PTR][PTR_SIZE+PTR_EAX*PTR_SIZE]
  cmp   PTR_ESI, src_end
  jae   src_end_exit32

  ; Load sample data
  xor   eax, eax
  mov   al, byte ptr [PTR_ESI]
  sub   eax, 080h
  shl   eax, 8

  ; Apply volume
  shl   eax, 11

  ; End loop
  jmp   merge_loop32

  ; Jump out point if end of dest is reached
  dest_end_exit32:

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  sbb   PTR_EAX, PTR_EAX
  add   PTR_ESI, [STEPTBL_PTR][PTR_SIZE+PTR_EAX*PTR_SIZE]

  ; Jump out point if end of src is reached
  src_end_exit32:

  ; adjust fractional
  shr   edx, 16

  ; Routine end
  ret

Merge_DestMono_SrcMono_Src8_NoVolume_Resample ENDP


Merge_DestStereo_SrcMono_Src8_NoVolume_Resample PROC ; 33

  ; adjust fractional
  shl   edx, 16

  ; Load sample data
  xor   eax, eax
  mov   al, byte ptr [PTR_ESI]
  sub   eax, 080h
  shl   eax, 8

  ; Apply volume
  shl   eax, 11

  ; Merge sample data loop
  ALIGN 16
  merge_loop33:

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   [PTR_EDI+4], eax
  add   PTR_EDI, 8
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit33

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  sbb   PTR_EAX, PTR_EAX
  add   PTR_ESI, [STEPTBL_PTR][PTR_SIZE+PTR_EAX*PTR_SIZE]
  cmp   PTR_ESI, src_end
  jae   src_end_exit33

  ; Load sample data
  xor   eax, eax
  mov   al, byte ptr [PTR_ESI]
  sub   eax, 080h
  shl   eax, 8

  ; Apply volume
  shl   eax, 11

  ; End loop
  jmp   merge_loop33

  ; Jump out point if end of dest is reached
  dest_end_exit33:

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  sbb   PTR_EAX, PTR_EAX
  add   PTR_ESI, [STEPTBL_PTR][PTR_SIZE+PTR_EAX*PTR_SIZE]

  ; Jump out point if end of src is reached
  src_end_exit33:

  ; adjust fractional
  shr   edx, 16

  ; Routine end
  ret

Merge_DestStereo_SrcMono_Src8_NoVolume_Resample ENDP


Merge_DestMono_SrcMono_Src16_NoVolume_Resample PROC ; 34

  ; adjust fractional
  shl   edx, 16

  ; Load sample data
  movsx eax, word ptr [PTR_ESI]

  ; Apply volume
  shl   eax, 11

  ; Merge sample data loop
  ALIGN 16
  merge_loop34:

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   PTR_EDI, 4
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit34

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  sbb   PTR_EAX, PTR_EAX
  add   PTR_ESI, [STEPTBL_PTR][PTR_SIZE+PTR_EAX*PTR_SIZE]
  cmp   PTR_ESI, src_end
  jae   src_end_exit34

  ; Load sample data
  movsx eax, word ptr [PTR_ESI]

  ; Apply volume
  shl   eax, 11

  ; End loop
  jmp   merge_loop34

  ; Jump out point if end of dest is reached
  dest_end_exit34:

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  sbb   PTR_EAX, PTR_EAX
  add   PTR_ESI, [STEPTBL_PTR][PTR_SIZE+PTR_EAX*PTR_SIZE]

  ; Jump out point if end of src is reached
  src_end_exit34:

  ; adjust fractional
  shr   edx, 16

  ; Routine end
  ret

Merge_DestMono_SrcMono_Src16_NoVolume_Resample ENDP


Merge_DestStereo_SrcMono_Src16_NoVolume_Resample PROC ; 35

  ; adjust fractional
  shl   edx, 16

  ; Load sample data
  movsx eax, word ptr [PTR_ESI]

  ; Apply volume
  shl   eax, 11

  ; Merge sample data loop
  ALIGN 16
  merge_loop35:

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   [PTR_EDI+4], eax
  add   PTR_EDI, 8
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit35

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  sbb   PTR_EAX, PTR_EAX
  add   PTR_ESI, [STEPTBL_PTR][PTR_SIZE+PTR_EAX*PTR_SIZE]
  cmp   PTR_ESI, src_end
  jae   src_end_exit35

  ; Load sample data
  movsx eax, word ptr [PTR_ESI]

  ; Apply volume
  shl   eax, 11

  ; End loop
  jmp   merge_loop35

  ; Jump out point if end of dest is reached
  dest_end_exit35:

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  sbb   PTR_EAX, PTR_EAX
  add   PTR_ESI, [STEPTBL_PTR][PTR_SIZE+PTR_EAX*PTR_SIZE]

  ; Jump out point if end of src is reached
  src_end_exit35:

  ; adjust fractional
  shr   edx, 16

  ; Routine end
  ret

Merge_DestStereo_SrcMono_Src16_NoVolume_Resample ENDP


Merge_DestMono_SrcMono_Src8_NoVolume_Filtered PROC ; 36

  ; check to see if we have to call the upsampling version
  cmp   [playback_ratio], 10000h
  jle   Merge_DestMono_SrcMono_Src8_NoVolume_UpFiltered

  ; build average dividers
  mov   ecx, edx
  mov   ebx, [playback_ratio]
  xor   edx, edx
  mov   eax, 08000000h  ; 100000000f/32
  div   ebx
  dec   eax
  mov   [divider_l], eax
  mov   edx, ecx

  ; load initial sample
  mov   ecx, [cur_l]

  ; handle start up loop management
  mov  eax, edx
  and  edx, 03fffffffh
  test eax, 080000000h
  jnz  whole_continue36
  test eax, 040000000h
  jnz  last_continue36

  ; Merge sample data loop
  ALIGN 16
  merge_loop36:
  mov   ebx, 65536
  sub   ebx, edx
  add   edx, [playback_ratio]

  ; weight the initial sample
  imul  ecx, ebx
  sar   ecx, 16
  cmp   edx, 65536*2
  jl    skip_loop36

  ; loop to load all of the full sample points
  whole_loop36:
  cmp   PTR_ESI, [src_end]
  jae   src_whole_exit36
  whole_continue36:

  ; Load sample data
  xor   eax, eax
  mov   al, byte ptr [PTR_ESI]
  sub   eax, 080h
  shl   eax, 8
  add   ecx, eax
  sub   edx, 65536
  inc   PTR_ESI
  cmp   edx, 65536*2
  jae   whole_loop36

  skip_loop36:
  and   edx, 0ffffh
  last_continue36:
  cmp   PTR_ESI, [src_end]
  jae   src_last_exit36

  ; Load sample data
  xor   eax, eax
  mov   al, byte ptr [PTR_ESI]
  sub   eax, 080h
  shl   eax, 8
  inc   PTR_ESI
  mov   ebx, ecx
  mov   ecx, eax

  ; weight the final sample
  imul  eax, edx
  sar   eax, 16
  add   eax, ebx
  imul  eax, [divider_l]

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   PTR_EDI, 4
  cmp   PTR_EDI, dest_end
  jb    merge_loop36

  ; Jump out point if end of dest is reached
  dest_end_exit36:
  jmp   src_save_value36

  ; jump out when src is exceed, but save our current loop position
  src_whole_exit36:
  or   edx, 080000000h
  src_last_exit36:
  or   edx, 040000000h
  src_save_value36:
  mov  [cur_l], ecx

  ; Routine end
  ret

Merge_DestMono_SrcMono_Src8_NoVolume_UpFiltered:

  ; adjust fractional
  shl   edx, 16

  ; Load sample data
  xor   eax, eax
  mov   al, byte ptr [PTR_ESI]
  sub   eax, 080h
  shl   eax, 8

  ; rotate filter values
  mov   ecx, [cur_l]
  mov   [cur_l], eax

  ; Merge sample data loop
  ALIGN 16
  merge_loop548:

  ; Upsample the data points
  mov   ebx, edx
  mov   eax, [cur_l]
  shr   ebx, 17
  sub   eax, ecx
  imul  eax, ebx
  sar   eax, 15
  add   eax, ecx

  ; Apply volume
  shl   eax, 11

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   PTR_EDI, 4
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit548

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  jnc   merge_loop548

  ; Move the source pointer
  inc   PTR_ESI
  cmp   PTR_ESI, src_end
  jae   src_end_exit548

  ; Load sample data
  xor   eax, eax
  mov   al, byte ptr [PTR_ESI]
  sub   eax, 080h
  shl   eax, 8

  ; rotate filter values
  mov   ecx, [cur_l]
  mov   [cur_l], eax

  ; End loop
  jmp   merge_loop548

  ; Jump out point if end of dest is reached
  dest_end_exit548:
  inc   PTR_ESI
  add   edx, [step_fract]
  jc    skip_filter_adjust548

  ; rotate filter values
  mov   [cur_l], ecx

  ; un-increment the source to skip the early source adjustment
  dec   PTR_ESI

  skip_filter_adjust548:

  ; Jump out point if end of src is reached
  src_end_exit548:

  ; adjust fractional
  shr   edx, 16

  ; Routine end
  ret

Merge_DestMono_SrcMono_Src8_NoVolume_Filtered ENDP


Merge_DestStereo_SrcMono_Src8_NoVolume_Filtered PROC ; 37

  ; check to see if we have to call the upsampling version
  cmp   [playback_ratio], 10000h
  jle   Merge_DestStereo_SrcMono_Src8_NoVolume_UpFiltered

  ; build average dividers
  mov   ecx, edx
  mov   ebx, [playback_ratio]
  xor   edx, edx
  mov   eax, 08000000h  ; 100000000f/32
  div   ebx
  dec   eax
  mov   [divider_l], eax
  mov   edx, ecx

  ; load initial sample
  mov   ecx, [cur_l]

  ; handle start up loop management
  mov  eax, edx
  and  edx, 03fffffffh
  test eax, 080000000h
  jnz  whole_continue37
  test eax, 040000000h
  jnz  last_continue37

  ; Merge sample data loop
  ALIGN 16
  merge_loop37:
  mov   ebx, 65536
  sub   ebx, edx
  add   edx, [playback_ratio]

  ; weight the initial sample
  imul  ecx, ebx
  sar   ecx, 16
  cmp   edx, 65536*2
  jl    skip_loop37

  ; loop to load all of the full sample points
  whole_loop37:
  cmp   PTR_ESI, [src_end]
  jae   src_whole_exit37
  whole_continue37:

  ; Load sample data
  xor   eax, eax
  mov   al, byte ptr [PTR_ESI]
  sub   eax, 080h
  shl   eax, 8
  add   ecx, eax
  sub   edx, 65536
  inc   PTR_ESI
  cmp   edx, 65536*2
  jae   whole_loop37

  skip_loop37:
  and   edx, 0ffffh
  last_continue37:
  cmp   PTR_ESI, [src_end]
  jae   src_last_exit37

  ; Load sample data
  xor   eax, eax
  mov   al, byte ptr [PTR_ESI]
  sub   eax, 080h
  shl   eax, 8
  inc   PTR_ESI
  mov   ebx, ecx
  mov   ecx, eax

  ; weight the final sample
  imul  eax, edx
  sar   eax, 16
  add   eax, ebx
  imul  eax, [divider_l]

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   [PTR_EDI+4], eax
  add   PTR_EDI, 8
  cmp   PTR_EDI, dest_end
  jb    merge_loop37

  ; Jump out point if end of dest is reached
  dest_end_exit37:
  jmp   src_save_value37

  ; jump out when src is exceed, but save our current loop position
  src_whole_exit37:
  or   edx, 080000000h
  src_last_exit37:
  or   edx, 040000000h
  src_save_value37:
  mov  [cur_l], ecx

  ; Routine end
  ret

Merge_DestStereo_SrcMono_Src8_NoVolume_UpFiltered:

  ; adjust fractional
  shl   edx, 16

  ; Load sample data
  xor   eax, eax
  mov   al, byte ptr [PTR_ESI]
  sub   eax, 080h
  shl   eax, 8

  ; rotate filter values
  mov   ecx, [cur_l]
  mov   [cur_l], eax

  ; Merge sample data loop
  ALIGN 16
  merge_loop549:

  ; Upsample the data points
  mov   ebx, edx
  mov   eax, [cur_l]
  shr   ebx, 17
  sub   eax, ecx
  imul  eax, ebx
  sar   eax, 15
  add   eax, ecx

  ; Apply volume
  shl   eax, 11

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   [PTR_EDI+4], eax
  add   PTR_EDI, 8
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit549

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  jnc   merge_loop549

  ; Move the source pointer
  inc   PTR_ESI
  cmp   PTR_ESI, src_end
  jae   src_end_exit549

  ; Load sample data
  xor   eax, eax
  mov   al, byte ptr [PTR_ESI]
  sub   eax, 080h
  shl   eax, 8

  ; rotate filter values
  mov   ecx, [cur_l]
  mov   [cur_l], eax

  ; End loop
  jmp   merge_loop549

  ; Jump out point if end of dest is reached
  dest_end_exit549:
  inc   PTR_ESI
  add   edx, [step_fract]
  jc    skip_filter_adjust549

  ; rotate filter values
  mov   [cur_l], ecx

  ; un-increment the source to skip the early source adjustment
  dec   PTR_ESI

  skip_filter_adjust549:

  ; Jump out point if end of src is reached
  src_end_exit549:

  ; adjust fractional
  shr   edx, 16

  ; Routine end
  ret

Merge_DestStereo_SrcMono_Src8_NoVolume_Filtered ENDP


Merge_DestMono_SrcMono_Src16_NoVolume_Filtered PROC ; 38

  ; check to see if we have to call the upsampling version
  cmp   [playback_ratio], 10000h
  jle   Merge_DestMono_SrcMono_Src16_NoVolume_UpFiltered

  ; build average dividers
  mov   ecx, edx
  mov   ebx, [playback_ratio]
  xor   edx, edx
  mov   eax, 08000000h  ; 100000000f/32
  div   ebx
  dec   eax
  mov   [divider_l], eax
  mov   edx, ecx

  ; load initial sample
  mov   ecx, [cur_l]

  ; handle start up loop management
  mov  eax, edx
  and  edx, 03fffffffh
  test eax, 080000000h
  jnz  whole_continue38
  test eax, 040000000h
  jnz  last_continue38

  ; Merge sample data loop
  ALIGN 16
  merge_loop38:
  mov   ebx, 65536
  sub   ebx, edx
  add   edx, [playback_ratio]

  ; weight the initial sample
  imul  ecx, ebx
  sar   ecx, 16
  cmp   edx, 65536*2
  jl    skip_loop38

  ; loop to load all of the full sample points
  whole_loop38:
  cmp   PTR_ESI, [src_end]
  jae   src_whole_exit38
  whole_continue38:

  ; Load sample data
  movsx eax, word ptr [PTR_ESI]
  add   ecx, eax
  sub   edx, 65536
  add   PTR_ESI, 2
  cmp   edx, 65536*2
  jae   whole_loop38

  skip_loop38:
  and   edx, 0ffffh
  last_continue38:
  cmp   PTR_ESI, [src_end]
  jae   src_last_exit38

  ; Load sample data
  movsx eax, word ptr [PTR_ESI]
  add   PTR_ESI, 2
  mov   ebx, ecx
  mov   ecx, eax

  ; weight the final sample
  imul  eax, edx
  sar   eax, 16
  add   eax, ebx
  imul  eax, [divider_l]

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   PTR_EDI, 4
  cmp   PTR_EDI, dest_end
  jb    merge_loop38

  ; Jump out point if end of dest is reached
  dest_end_exit38:
  jmp   src_save_value38

  ; jump out when src is exceed, but save our current loop position
  src_whole_exit38:
  or   edx, 080000000h
  src_last_exit38:
  or   edx, 040000000h
  src_save_value38:
  mov  [cur_l], ecx

  ; Routine end
  ret

Merge_DestMono_SrcMono_Src16_NoVolume_UpFiltered:

  ; adjust fractional
  shl   edx, 16

  ; Load sample data
  movsx eax, word ptr [PTR_ESI]

  ; rotate filter values
  mov   ecx, [cur_l]
  mov   [cur_l], eax

  ; Merge sample data loop
  ALIGN 16
  merge_loop550:

  ; Upsample the data points
  mov   ebx, edx
  mov   eax, [cur_l]
  shr   ebx, 17
  sub   eax, ecx
  imul  eax, ebx
  sar   eax, 15
  add   eax, ecx

  ; Apply volume
  shl   eax, 11

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   PTR_EDI, 4
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit550

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  jnc   merge_loop550

  ; Move the source pointer
  add   PTR_ESI, 2
  cmp   PTR_ESI, src_end
  jae   src_end_exit550

  ; Load sample data
  movsx eax, word ptr [PTR_ESI]

  ; rotate filter values
  mov   ecx, [cur_l]
  mov   [cur_l], eax

  ; End loop
  jmp   merge_loop550

  ; Jump out point if end of dest is reached
  dest_end_exit550:
  add   PTR_ESI, 2
  add   edx, [step_fract]
  jc    skip_filter_adjust550

  ; rotate filter values
  mov   [cur_l], ecx

  ; un-increment the source to skip the early source adjustment
  sub   PTR_ESI, 2

  skip_filter_adjust550:

  ; Jump out point if end of src is reached
  src_end_exit550:

  ; adjust fractional
  shr   edx, 16

  ; Routine end
  ret

Merge_DestMono_SrcMono_Src16_NoVolume_Filtered ENDP


Merge_DestStereo_SrcMono_Src16_NoVolume_Filtered PROC ; 39

  ; check to see if we have to call the upsampling version
  cmp   [playback_ratio], 10000h
  jle   Merge_DestStereo_SrcMono_Src16_NoVolume_UpFiltered

  ; build average dividers
  mov   ecx, edx
  mov   ebx, [playback_ratio]
  xor   edx, edx
  mov   eax, 08000000h  ; 100000000f/32
  div   ebx
  dec   eax
  mov   [divider_l], eax
  mov   edx, ecx

  ; load initial sample
  mov   ecx, [cur_l]

  ; handle start up loop management
  mov  eax, edx
  and  edx, 03fffffffh
  test eax, 080000000h
  jnz  whole_continue39
  test eax, 040000000h
  jnz  last_continue39

  ; Merge sample data loop
  ALIGN 16
  merge_loop39:
  mov   ebx, 65536
  sub   ebx, edx
  add   edx, [playback_ratio]

  ; weight the initial sample
  imul  ecx, ebx
  sar   ecx, 16
  cmp   edx, 65536*2
  jl    skip_loop39

  ; loop to load all of the full sample points
  whole_loop39:
  cmp   PTR_ESI, [src_end]
  jae   src_whole_exit39
  whole_continue39:

  ; Load sample data
  movsx eax, word ptr [PTR_ESI]
  add   ecx, eax
  sub   edx, 65536
  add   PTR_ESI, 2
  cmp   edx, 65536*2
  jae   whole_loop39

  skip_loop39:
  and   edx, 0ffffh
  last_continue39:
  cmp   PTR_ESI, [src_end]
  jae   src_last_exit39

  ; Load sample data
  movsx eax, word ptr [PTR_ESI]
  add   PTR_ESI, 2
  mov   ebx, ecx
  mov   ecx, eax

  ; weight the final sample
  imul  eax, edx
  sar   eax, 16
  add   eax, ebx
  imul  eax, [divider_l]

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   [PTR_EDI+4], eax
  add   PTR_EDI, 8
  cmp   PTR_EDI, dest_end
  jb    merge_loop39

  ; Jump out point if end of dest is reached
  dest_end_exit39:
  jmp   src_save_value39

  ; jump out when src is exceed, but save our current loop position
  src_whole_exit39:
  or   edx, 080000000h
  src_last_exit39:
  or   edx, 040000000h
  src_save_value39:
  mov  [cur_l], ecx

  ; Routine end
  ret

Merge_DestStereo_SrcMono_Src16_NoVolume_UpFiltered:

  ; adjust fractional
  shl   edx, 16

  ; Load sample data
  movsx eax, word ptr [PTR_ESI]

  ; rotate filter values
  mov   ecx, [cur_l]
  mov   [cur_l], eax

  ; Merge sample data loop
  ALIGN 16
  merge_loop551:

  ; Upsample the data points
  mov   ebx, edx
  mov   eax, [cur_l]
  shr   ebx, 17
  sub   eax, ecx
  imul  eax, ebx
  sar   eax, 15
  add   eax, ecx

  ; Apply volume
  shl   eax, 11

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   [PTR_EDI+4], eax
  add   PTR_EDI, 8
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit551

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  jnc   merge_loop551

  ; Move the source pointer
  add   PTR_ESI, 2
  cmp   PTR_ESI, src_end
  jae   src_end_exit551

  ; Load sample data
  movsx eax, word ptr [PTR_ESI]

  ; rotate filter values
  mov   ecx, [cur_l]
  mov   [cur_l], eax

  ; End loop
  jmp   merge_loop551

  ; Jump out point if end of dest is reached
  dest_end_exit551:
  add   PTR_ESI, 2
  add   edx, [step_fract]
  jc    skip_filter_adjust551

  ; rotate filter values
  mov   [cur_l], ecx

  ; un-increment the source to skip the early source adjustment
  sub   PTR_ESI, 2

  skip_filter_adjust551:

  ; Jump out point if end of src is reached
  src_end_exit551:

  ; adjust fractional
  shr   edx, 16

  ; Routine end
  ret

Merge_DestStereo_SrcMono_Src16_NoVolume_Filtered ENDP


Merge_DestMono_SrcStereo_Src8_NoVolume_Resample PROC ; 40

  ; adjust fractional
  shl   edx, 16

  ; Load sample data
  xor   ebx, ebx
  xor   eax, eax
  mov   ax, word ptr [PTR_ESI]
  mov   bl, ah
  and   eax, 0ffh
  sub   ebx, 080h
  sub   eax, 080h
  shl   ebx, 8
  shl   eax, 8

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; Apply volume
  shl   eax, 11

  ; Merge sample data loop
  ALIGN 16
  merge_loop40:

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   PTR_EDI, 4
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit40

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  sbb   PTR_EAX, PTR_EAX
  add   PTR_ESI, [STEPTBL_PTR][PTR_SIZE+PTR_EAX*PTR_SIZE]
  cmp   PTR_ESI, src_end
  jae   src_end_exit40

  ; Load sample data
  xor   ebx, ebx
  xor   eax, eax
  mov   ax, word ptr [PTR_ESI]
  mov   bl, ah
  and   eax, 0ffh
  sub   ebx, 080h
  sub   eax, 080h
  shl   ebx, 8
  shl   eax, 8

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; Apply volume
  shl   eax, 11

  ; End loop
  jmp   merge_loop40

  ; Jump out point if end of dest is reached
  dest_end_exit40:

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  sbb   PTR_EAX, PTR_EAX
  add   PTR_ESI, [STEPTBL_PTR][PTR_SIZE+PTR_EAX*PTR_SIZE]

  ; Jump out point if end of src is reached
  src_end_exit40:

  ; adjust fractional
  shr   edx, 16

  ; Routine end
  ret

Merge_DestMono_SrcStereo_Src8_NoVolume_Resample ENDP


Merge_DestStereo_SrcStereo_Src8_NoVolume_Resample PROC ; 41

  ; adjust fractional
  shl   edx, 16

  ; Load sample data
  xor   ebx, ebx
  xor   eax, eax
  mov   ax, word ptr [PTR_ESI]
  mov   bl, ah
  and   eax, 0ffh
  sub   ebx, 080h
  sub   eax, 080h
  shl   ebx, 8
  shl   eax, 8

  ; Apply volume
  shl   eax, 11
  shl   ebx, 11

  ; Merge sample data loop
  ALIGN 16
  merge_loop41:

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   [PTR_EDI+4], ebx
  add   PTR_EDI, 8
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit41

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  sbb   PTR_EAX, PTR_EAX
  add   PTR_ESI, [STEPTBL_PTR][PTR_SIZE+PTR_EAX*PTR_SIZE]
  cmp   PTR_ESI, src_end
  jae   src_end_exit41

  ; Load sample data
  xor   ebx, ebx
  xor   eax, eax
  mov   ax, word ptr [PTR_ESI]
  mov   bl, ah
  and   eax, 0ffh
  sub   ebx, 080h
  sub   eax, 080h
  shl   ebx, 8
  shl   eax, 8

  ; Apply volume
  shl   eax, 11
  shl   ebx, 11

  ; End loop
  jmp   merge_loop41

  ; Jump out point if end of dest is reached
  dest_end_exit41:

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  sbb   PTR_EAX, PTR_EAX
  add   PTR_ESI, [STEPTBL_PTR][PTR_SIZE+PTR_EAX*PTR_SIZE]

  ; Jump out point if end of src is reached
  src_end_exit41:

  ; adjust fractional
  shr   edx, 16

  ; Routine end
  ret

Merge_DestStereo_SrcStereo_Src8_NoVolume_Resample ENDP


Merge_DestMono_SrcStereo_Src16_NoVolume_Resample PROC ; 42

  ; adjust fractional
  shl   edx, 16

  ; Load sample data
  mov   ebx, dword ptr [PTR_ESI]
  movsx eax, bx
  sar   ebx, 16

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; Apply volume
  shl   eax, 11

  ; Merge sample data loop
  ALIGN 16
  merge_loop42:

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   PTR_EDI, 4
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit42

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  sbb   PTR_EAX, PTR_EAX
  add   PTR_ESI, [STEPTBL_PTR][PTR_SIZE+PTR_EAX*PTR_SIZE]
  cmp   PTR_ESI, src_end
  jae   src_end_exit42

  ; Load sample data
  mov   ebx, dword ptr [PTR_ESI]
  movsx eax, bx
  sar   ebx, 16

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; Apply volume
  shl   eax, 11

  ; End loop
  jmp   merge_loop42

  ; Jump out point if end of dest is reached
  dest_end_exit42:

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  sbb   PTR_EAX, PTR_EAX
  add   PTR_ESI, [STEPTBL_PTR][PTR_SIZE+PTR_EAX*PTR_SIZE]

  ; Jump out point if end of src is reached
  src_end_exit42:

  ; adjust fractional
  shr   edx, 16

  ; Routine end
  ret

Merge_DestMono_SrcStereo_Src16_NoVolume_Resample ENDP


Merge_DestStereo_SrcStereo_Src16_NoVolume_Resample PROC ; 43

  ; adjust fractional
  shl   edx, 16

  ; Load sample data
  mov   ebx, dword ptr [PTR_ESI]
  movsx eax, bx
  sar   ebx, 16

  ; Apply volume
  shl   eax, 11
  shl   ebx, 11

  ; Merge sample data loop
  ALIGN 16
  merge_loop43:

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   [PTR_EDI+4], ebx
  add   PTR_EDI, 8
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit43

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  sbb   PTR_EAX, PTR_EAX
  add   PTR_ESI, [STEPTBL_PTR][PTR_SIZE+PTR_EAX*PTR_SIZE]
  cmp   PTR_ESI, src_end
  jae   src_end_exit43

  ; Load sample data
  mov   ebx, dword ptr [PTR_ESI]
  movsx eax, bx
  sar   ebx, 16

  ; Apply volume
  shl   eax, 11
  shl   ebx, 11

  ; End loop
  jmp   merge_loop43

  ; Jump out point if end of dest is reached
  dest_end_exit43:

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  sbb   PTR_EAX, PTR_EAX
  add   PTR_ESI, [STEPTBL_PTR][PTR_SIZE+PTR_EAX*PTR_SIZE]

  ; Jump out point if end of src is reached
  src_end_exit43:

  ; adjust fractional
  shr   edx, 16

  ; Routine end
  ret

Merge_DestStereo_SrcStereo_Src16_NoVolume_Resample ENDP


Merge_DestMono_SrcStereo_Src8_NoVolume_Filtered PROC ; 44

  ; check to see if we have to call the upsampling version
  cmp   [playback_ratio], 10000h
  jle   Merge_DestMono_SrcStereo_Src8_NoVolume_UpFiltered

  ; build average dividers
  mov   ecx, edx
  mov   ebx, [playback_ratio]
  xor   edx, edx
  mov   eax, 08000000h  ; 100000000f/32
  div   ebx
  dec   eax
  mov   [divider_l], eax
  mov   edx, ecx

  ; load initial sample
  mov   ecx, [cur_l]

  ; handle start up loop management
  mov  eax, edx
  and  edx, 03fffffffh
  test eax, 080000000h
  jnz  whole_continue44
  test eax, 040000000h
  jnz  last_continue44

  ; Merge sample data loop
  ALIGN 16
  merge_loop44:
  mov   ebx, 65536
  sub   ebx, edx
  add   edx, [playback_ratio]

  ; weight the initial sample
  imul  ecx, ebx
  sar   ecx, 16
  cmp   edx, 65536*2
  jl    skip_loop44

  ; loop to load all of the full sample points
  whole_loop44:
  cmp   PTR_ESI, [src_end]
  jae   src_whole_exit44
  whole_continue44:

  ; Load sample data
  xor   ebx, ebx
  xor   eax, eax
  mov   ax, word ptr [PTR_ESI]
  mov   bl, ah
  and   eax, 0ffh
  sub   ebx, 080h
  sub   eax, 080h
  shl   ebx, 8
  shl   eax, 8

  ; Merge left and right channels for mono dest
  add   eax, ebx
  add   ecx, eax
  sub   edx, 65536
  add   PTR_ESI, 2
  cmp   edx, 65536*2
  jae   whole_loop44

  skip_loop44:
  and   edx, 0ffffh
  last_continue44:
  cmp   PTR_ESI, [src_end]
  jae   src_last_exit44

  ; Load sample data
  xor   ebx, ebx
  xor   eax, eax
  mov   ax, word ptr [PTR_ESI]
  mov   bl, ah
  and   eax, 0ffh
  sub   ebx, 080h
  sub   eax, 080h
  shl   ebx, 8
  shl   eax, 8

  ; Merge left and right channels for mono dest
  add   eax, ebx
  add   PTR_ESI, 2
  mov   ebx, ecx
  mov   ecx, eax

  ; weight the final sample
  imul  eax, edx
  sar   eax, 16
  add   eax, ebx
  imul  eax, [divider_l]

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   PTR_EDI, 4
  cmp   PTR_EDI, dest_end
  jb    merge_loop44

  ; Jump out point if end of dest is reached
  dest_end_exit44:
  jmp   src_save_value44

  ; jump out when src is exceed, but save our current loop position
  src_whole_exit44:
  or   edx, 080000000h
  src_last_exit44:
  or   edx, 040000000h
  src_save_value44:
  mov  [cur_l], ecx

  ; Routine end
  ret

Merge_DestMono_SrcStereo_Src8_NoVolume_UpFiltered:

  ; adjust fractional
  shl   edx, 16

  ; Load sample data
  xor   ebx, ebx
  xor   eax, eax
  mov   ax, word ptr [PTR_ESI]
  mov   bl, ah
  and   eax, 0ffh
  sub   ebx, 080h
  sub   eax, 080h
  shl   ebx, 8
  shl   eax, 8

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; rotate filter values
  mov   ecx, [cur_l]
  mov   [cur_l], eax

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; Merge sample data loop
  ALIGN 16
  merge_loop556:

  ; Upsample the data points
  mov   ebx, edx
  mov   eax, [cur_l]
  shr   ebx, 17
  sub   eax, ecx
  imul  eax, ebx
  sar   eax, 15
  add   eax, ecx

  ; Apply volume
  shl   eax, 11

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   PTR_EDI, 4
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit556

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  jnc   merge_loop556

  ; Move the source pointer
  add   PTR_ESI, 2
  cmp   PTR_ESI, src_end
  jae   src_end_exit556

  ; Load sample data
  xor   ebx, ebx
  xor   eax, eax
  mov   ax, word ptr [PTR_ESI]
  mov   bl, ah
  and   eax, 0ffh
  sub   ebx, 080h
  sub   eax, 080h
  shl   ebx, 8
  shl   eax, 8

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; rotate filter values
  mov   ecx, [cur_l]
  mov   [cur_l], eax

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; End loop
  jmp   merge_loop556

  ; Jump out point if end of dest is reached
  dest_end_exit556:
  add   PTR_ESI, 2
  add   edx, [step_fract]
  jc    skip_filter_adjust556

  ; rotate filter values
  mov   [cur_l], ecx

  ; un-increment the source to skip the early source adjustment
  sub   PTR_ESI, 2

  skip_filter_adjust556:

  ; Jump out point if end of src is reached
  src_end_exit556:

  ; adjust fractional
  shr   edx, 16

  ; Routine end
  ret

Merge_DestMono_SrcStereo_Src8_NoVolume_Filtered ENDP


Merge_DestStereo_SrcStereo_Src8_NoVolume_Filtered PROC ; 45

  ; check to see if we have to call the upsampling version
  cmp   [playback_ratio], 10000h
  jle   Merge_DestStereo_SrcStereo_Src8_NoVolume_UpFiltered

  ; build average dividers
  mov   ecx, edx
  mov   ebx, [playback_ratio]
  xor   edx, edx
  mov   eax, 08000000h  ; 100000000f/32
  div   ebx
  dec   eax
  mov   [divider_l], eax
  xor   edx, edx
  mov   eax, 08000000h  ; 100000000f/32
  div   ebx
  dec   eax
  mov   [divider_r], eax
  mov   edx, ecx

  ; load initial sample
  mov   ecx, [cur_l]

  ; Save registers
  push  PTR_EBP

  ; handle start up loop management
  mov  eax, edx
  and  edx, 03fffffffh
  test eax, 080000000h
  jnz  whole_continue45
  test eax, 040000000h
  jnz  last_continue45

  ; Merge sample data loop
  ALIGN 16
  merge_loop45:
  mov   ebx, 65536
  sub   ebx, edx
  add   edx, [playback_ratio]

  ; weight the initial sample
  mov   eax, [cur_r]
  imul  ecx, ebx
  imul  eax, ebx
  sar   ecx, 16
  sar   eax, 16
  cmp   edx, 65536*2
  mov   [cur_r], eax
  jl    skip_loop45

  ; loop to load all of the full sample points
  whole_loop45:
  cmp   PTR_ESI, [src_end]
  jae   src_whole_exit45
  whole_continue45:

  ; Load sample data
  xor   ebx, ebx
  xor   eax, eax
  mov   ax, word ptr [PTR_ESI]
  mov   bl, ah
  and   eax, 0ffh
  sub   ebx, 080h
  sub   eax, 080h
  shl   ebx, 8
  shl   eax, 8
  add   ecx, eax
  add   [cur_r], ebx
  sub   edx, 65536
  add   PTR_ESI, 2
  cmp   edx, 65536*2
  jae   whole_loop45

  skip_loop45:
  and   edx, 0ffffh
  cmp   PTR_ESI, [src_end]
  jae   src_last_exit45
  last_continue45:

  ; Load sample data
  xor   ebx, ebx
  xor   eax, eax
  mov   ax, word ptr [PTR_ESI]
  mov   bl, ah
  and   eax, 0ffh
  sub   ebx, 080h
  sub   eax, 080h
  shl   ebx, 8
  shl   eax, 8
  add   PTR_ESI, 2
  mov   ebp, ecx
  mov   ecx, eax

  ; weight the final sample
  imul  eax, edx
  sar   eax, 16
  add   eax, ebp
  mov   ebp, [cur_r]
  mov   [cur_r], ebx
  imul  ebx, edx
  sar   ebx, 16
  add   ebx, ebp
  imul  eax, [divider_l]
  imul  ebx, [divider_r]

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   [PTR_EDI+4], ebx
  add   PTR_EDI, 8
  cmp   PTR_EDI, dest_end
  jb    merge_loop45

  ; Jump out point if end of dest is reached
  dest_end_exit45:
  jmp   src_save_value45

  ; jump out when src is exceed, but save our current loop position
  src_whole_exit45:
  or   edx, 080000000h
  src_last_exit45:
  or   edx, 040000000h
  src_save_value45:
  mov  [cur_l], ecx

  ; Restore registers
  pop   PTR_EBP

  ; Routine end
  ret

Merge_DestStereo_SrcStereo_Src8_NoVolume_UpFiltered:

  ; adjust fractional
  shl   edx, 16

  ; Save registers
  push  PTR_EBP

  ; Load sample data
  xor   ebx, ebx
  xor   eax, eax
  mov   ax, word ptr [PTR_ESI]
  mov   bl, ah
  and   eax, 0ffh
  sub   ebx, 080h
  sub   eax, 080h
  shl   ebx, 8
  shl   eax, 8

  ; rotate filter values
  mov   ecx, [cur_l]
  mov   [cur_l], eax
  mov   eax, [cur_r]
  mov   [cur_r], ebx
  mov   [cur_r2], eax

  ; Merge sample data loop
  ALIGN 16
  merge_loop557:

  ; Upsample the data points
  mov   ebp, edx
  mov   eax, [cur_l]
  mov   ebx, [cur_r]
  shr   ebp, 17
  sub   eax, ecx
  sub   ebx, [cur_r2]
  imul  eax, ebp
  imul  ebx, ebp
  sar   eax, 15
  sar   ebx, 15
  add   eax, ecx
  add   ebx, [cur_r2]

  ; Apply volume
  shl   eax, 11
  shl   ebx, 11

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   [PTR_EDI+4], ebx
  add   PTR_EDI, 8
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit557

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  jnc   merge_loop557

  ; Move the source pointer
  add   PTR_ESI, 2
  cmp   PTR_ESI, src_end
  jae   src_end_exit557

  ; Load sample data
  xor   ebx, ebx
  xor   eax, eax
  mov   ax, word ptr [PTR_ESI]
  mov   bl, ah
  and   eax, 0ffh
  sub   ebx, 080h
  sub   eax, 080h
  shl   ebx, 8
  shl   eax, 8

  ; rotate filter values
  mov   ecx, [cur_l]
  mov   [cur_l], eax
  mov   eax, [cur_r]
  mov   [cur_r], ebx
  mov   [cur_r2], eax

  ; End loop
  jmp   merge_loop557

  ; Jump out point if end of dest is reached
  dest_end_exit557:
  add   PTR_ESI, 2
  add   edx, [step_fract]
  jc    skip_filter_adjust557

  ; rotate filter values
  mov   [cur_l], ecx
  mov   ecx, [cur_r2]
  mov   [cur_r], ecx

  ; un-increment the source to skip the early source adjustment
  sub   PTR_ESI, 2

  skip_filter_adjust557:

  ; Jump out point if end of src is reached
  src_end_exit557:

  ; Restore registers
  pop   PTR_EBP

  ; adjust fractional
  shr   edx, 16

  ; Routine end
  ret

Merge_DestStereo_SrcStereo_Src8_NoVolume_Filtered ENDP


Merge_DestMono_SrcStereo_Src16_NoVolume_Filtered PROC ; 46

  ; check to see if we have to call the upsampling version
  cmp   [playback_ratio], 10000h
  jle   Merge_DestMono_SrcStereo_Src16_NoVolume_UpFiltered

  ; build average dividers
  mov   ecx, edx
  mov   ebx, [playback_ratio]
  xor   edx, edx
  mov   eax, 08000000h  ; 100000000f/32
  div   ebx
  dec   eax
  mov   [divider_l], eax
  mov   edx, ecx

  ; load initial sample
  mov   ecx, [cur_l]

  ; handle start up loop management
  mov  eax, edx
  and  edx, 03fffffffh
  test eax, 080000000h
  jnz  whole_continue46
  test eax, 040000000h
  jnz  last_continue46

  ; Merge sample data loop
  ALIGN 16
  merge_loop46:
  mov   ebx, 65536
  sub   ebx, edx
  add   edx, [playback_ratio]

  ; weight the initial sample
  imul  ecx, ebx
  sar   ecx, 16
  cmp   edx, 65536*2
  jl    skip_loop46

  ; loop to load all of the full sample points
  whole_loop46:
  cmp   PTR_ESI, [src_end]
  jae   src_whole_exit46
  whole_continue46:

  ; Load sample data
  mov   ebx, dword ptr [PTR_ESI]
  movsx eax, bx
  sar   ebx, 16

  ; Merge left and right channels for mono dest
  add   eax, ebx
  add   ecx, eax
  sub   edx, 65536
  add   PTR_ESI, 4
  cmp   edx, 65536*2
  jae   whole_loop46

  skip_loop46:
  and   edx, 0ffffh
  last_continue46:
  cmp   PTR_ESI, [src_end]
  jae   src_last_exit46

  ; Load sample data
  mov   ebx, dword ptr [PTR_ESI]
  movsx eax, bx
  sar   ebx, 16

  ; Merge left and right channels for mono dest
  add   eax, ebx
  add   PTR_ESI, 4
  mov   ebx, ecx
  mov   ecx, eax

  ; weight the final sample
  imul  eax, edx
  sar   eax, 16
  add   eax, ebx
  imul  eax, [divider_l]

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   PTR_EDI, 4
  cmp   PTR_EDI, dest_end
  jb    merge_loop46

  ; Jump out point if end of dest is reached
  dest_end_exit46:
  jmp   src_save_value46

  ; jump out when src is exceed, but save our current loop position
  src_whole_exit46:
  or   edx, 080000000h
  src_last_exit46:
  or   edx, 040000000h
  src_save_value46:
  mov  [cur_l], ecx

  ; Routine end
  ret

Merge_DestMono_SrcStereo_Src16_NoVolume_UpFiltered:

  ; adjust fractional
  shl   edx, 16

  ; Load sample data
  mov   ebx, dword ptr [PTR_ESI]
  movsx eax, bx
  sar   ebx, 16

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; rotate filter values
  mov   ecx, [cur_l]
  mov   [cur_l], eax

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; Merge sample data loop
  ALIGN 16
  merge_loop558:

  ; Upsample the data points
  mov   ebx, edx
  mov   eax, [cur_l]
  shr   ebx, 17
  sub   eax, ecx
  imul  eax, ebx
  sar   eax, 15
  add   eax, ecx

  ; Apply volume
  shl   eax, 11

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   PTR_EDI, 4
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit558

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  jnc   merge_loop558

  ; Move the source pointer
  add   PTR_ESI, 4
  cmp   PTR_ESI, src_end
  jae   src_end_exit558

  ; Load sample data
  mov   ebx, dword ptr [PTR_ESI]
  movsx eax, bx
  sar   ebx, 16

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; rotate filter values
  mov   ecx, [cur_l]
  mov   [cur_l], eax

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; End loop
  jmp   merge_loop558

  ; Jump out point if end of dest is reached
  dest_end_exit558:
  add   PTR_ESI, 4
  add   edx, [step_fract]
  jc    skip_filter_adjust558

  ; rotate filter values
  mov   [cur_l], ecx

  ; un-increment the source to skip the early source adjustment
  sub   PTR_ESI, 4

  skip_filter_adjust558:

  ; Jump out point if end of src is reached
  src_end_exit558:

  ; adjust fractional
  shr   edx, 16

  ; Routine end
  ret

Merge_DestMono_SrcStereo_Src16_NoVolume_Filtered ENDP


Merge_DestStereo_SrcStereo_Src16_NoVolume_Filtered PROC ; 47

  ; check to see if we have to call the upsampling version
  cmp   [playback_ratio], 10000h
  jle   Merge_DestStereo_SrcStereo_Src16_NoVolume_UpFiltered

  ; build average dividers
  mov   ecx, edx
  mov   ebx, [playback_ratio]
  xor   edx, edx
  mov   eax, 08000000h  ; 100000000f/32
  div   ebx
  dec   eax
  mov   [divider_l], eax
  xor   edx, edx
  mov   eax, 08000000h  ; 100000000f/32
  div   ebx
  dec   eax
  mov   [divider_r], eax
  mov   edx, ecx

  ; load initial sample
  mov   ecx, [cur_l]

  ; Save registers
  push  PTR_EBP

  ; handle start up loop management
  mov  eax, edx
  and  edx, 03fffffffh
  test eax, 080000000h
  jnz  whole_continue47
  test eax, 040000000h
  jnz  last_continue47

  ; Merge sample data loop
  ALIGN 16
  merge_loop47:
  mov   ebx, 65536
  sub   ebx, edx
  add   edx, [playback_ratio]

  ; weight the initial sample
  mov   eax, [cur_r]
  imul  ecx, ebx
  imul  eax, ebx
  sar   ecx, 16
  sar   eax, 16
  cmp   edx, 65536*2
  mov   [cur_r], eax
  jl    skip_loop47

  ; loop to load all of the full sample points
  whole_loop47:
  cmp   PTR_ESI, [src_end]
  jae   src_whole_exit47
  whole_continue47:

  ; Load sample data
  mov   ebx, dword ptr [PTR_ESI]
  movsx eax, bx
  sar   ebx, 16
  add   ecx, eax
  add   [cur_r], ebx
  sub   edx, 65536
  add   PTR_ESI, 4
  cmp   edx, 65536*2
  jae   whole_loop47

  skip_loop47:
  and   edx, 0ffffh
  cmp   PTR_ESI, [src_end]
  jae   src_last_exit47
  last_continue47:

  ; Load sample data
  mov   ebx, dword ptr [PTR_ESI]
  movsx eax, bx
  sar   ebx, 16
  add   PTR_ESI, 4
  mov   ebp, ecx
  mov   ecx, eax

  ; weight the final sample
  imul  eax, edx
  sar   eax, 16
  add   eax, ebp
  mov   ebp, [cur_r]
  mov   [cur_r], ebx
  imul  ebx, edx
  sar   ebx, 16
  add   ebx, ebp
  imul  eax, [divider_l]
  imul  ebx, [divider_r]

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   [PTR_EDI+4], ebx
  add   PTR_EDI, 8
  cmp   PTR_EDI, dest_end
  jb    merge_loop47

  ; Jump out point if end of dest is reached
  dest_end_exit47:
  jmp   src_save_value47

  ; jump out when src is exceed, but save our current loop position
  src_whole_exit47:
  or   edx, 080000000h
  src_last_exit47:
  or   edx, 040000000h
  src_save_value47:
  mov  [cur_l], ecx

  ; Restore registers
  pop   PTR_EBP

  ; Routine end
  ret

Merge_DestStereo_SrcStereo_Src16_NoVolume_UpFiltered:

  ; adjust fractional
  shl   edx, 16

  ; Save registers
  push  PTR_EBP

  ; Load sample data
  mov   ebx, dword ptr [PTR_ESI]
  movsx eax, bx
  sar   ebx, 16

  ; rotate filter values
  mov   ecx, [cur_l]
  mov   [cur_l], eax
  mov   eax, [cur_r]
  mov   [cur_r], ebx
  mov   [cur_r2], eax

  ; Merge sample data loop
  ALIGN 16
  merge_loop559:

  ; Upsample the data points
  mov   ebp, edx
  mov   eax, [cur_l]
  mov   ebx, [cur_r]
  shr   ebp, 17
  sub   eax, ecx
  sub   ebx, [cur_r2]
  imul  eax, ebp
  imul  ebx, ebp
  sar   eax, 15
  sar   ebx, 15
  add   eax, ecx
  add   ebx, [cur_r2]

  ; Apply volume
  shl   eax, 11
  shl   ebx, 11

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   [PTR_EDI+4], ebx
  add   PTR_EDI, 8
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit559

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  jnc   merge_loop559

  ; Move the source pointer
  add   PTR_ESI, 4
  cmp   PTR_ESI, src_end
  jae   src_end_exit559

  ; Load sample data
  mov   ebx, dword ptr [PTR_ESI]
  movsx eax, bx
  sar   ebx, 16

  ; rotate filter values
  mov   ecx, [cur_l]
  mov   [cur_l], eax
  mov   eax, [cur_r]
  mov   [cur_r], ebx
  mov   [cur_r2], eax

  ; End loop
  jmp   merge_loop559

  ; Jump out point if end of dest is reached
  dest_end_exit559:
  add   PTR_ESI, 4
  add   edx, [step_fract]
  jc    skip_filter_adjust559

  ; rotate filter values
  mov   [cur_l], ecx
  mov   ecx, [cur_r2]
  mov   [cur_r], ecx

  ; un-increment the source to skip the early source adjustment
  sub   PTR_ESI, 4

  skip_filter_adjust559:

  ; Jump out point if end of src is reached
  src_end_exit559:

  ; Restore registers
  pop   PTR_EBP

  ; adjust fractional
  shr   edx, 16

  ; Routine end
  ret

Merge_DestStereo_SrcStereo_Src16_NoVolume_Filtered ENDP


Merge_DestMono_SrcFlipped_Src8_NoVolume_Resample PROC ; 56

  ; adjust fractional
  shl   edx, 16

  ; Load sample data
  xor   eax, eax
  xor   ebx, ebx
  mov   bx, word ptr [PTR_ESI]
  mov   al, bh
  and   ebx, 0ffh
  sub   eax, 080h
  sub   ebx, 080h
  shl   eax, 8
  shl   ebx, 8

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; Apply volume
  shl   eax, 11

  ; Merge sample data loop
  ALIGN 16
  merge_loop56:

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   PTR_EDI, 4
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit56

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  sbb   PTR_EAX, PTR_EAX
  add   PTR_ESI, [STEPTBL_PTR][PTR_SIZE+PTR_EAX*PTR_SIZE]
  cmp   PTR_ESI, src_end
  jae   src_end_exit56

  ; Load sample data
  xor   eax, eax
  xor   ebx, ebx
  mov   bx, word ptr [PTR_ESI]
  mov   al, bh
  and   ebx, 0ffh
  sub   eax, 080h
  sub   ebx, 080h
  shl   eax, 8
  shl   ebx, 8

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; Apply volume
  shl   eax, 11

  ; End loop
  jmp   merge_loop56

  ; Jump out point if end of dest is reached
  dest_end_exit56:

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  sbb   PTR_EAX, PTR_EAX
  add   PTR_ESI, [STEPTBL_PTR][PTR_SIZE+PTR_EAX*PTR_SIZE]

  ; Jump out point if end of src is reached
  src_end_exit56:

  ; adjust fractional
  shr   edx, 16

  ; Routine end
  ret

Merge_DestMono_SrcFlipped_Src8_NoVolume_Resample ENDP


Merge_DestStereo_SrcFlipped_Src8_NoVolume_Resample PROC ; 57

  ; adjust fractional
  shl   edx, 16

  ; Load sample data
  xor   eax, eax
  xor   ebx, ebx
  mov   bx, word ptr [PTR_ESI]
  mov   al, bh
  and   ebx, 0ffh
  sub   eax, 080h
  sub   ebx, 080h
  shl   eax, 8
  shl   ebx, 8

  ; Apply volume
  shl   eax, 11
  shl   ebx, 11

  ; Merge sample data loop
  ALIGN 16
  merge_loop57:

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   [PTR_EDI+4], ebx
  add   PTR_EDI, 8
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit57

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  sbb   PTR_EAX, PTR_EAX
  add   PTR_ESI, [STEPTBL_PTR][PTR_SIZE+PTR_EAX*PTR_SIZE]
  cmp   PTR_ESI, src_end
  jae   src_end_exit57

  ; Load sample data
  xor   eax, eax
  xor   ebx, ebx
  mov   bx, word ptr [PTR_ESI]
  mov   al, bh
  and   ebx, 0ffh
  sub   eax, 080h
  sub   ebx, 080h
  shl   eax, 8
  shl   ebx, 8

  ; Apply volume
  shl   eax, 11
  shl   ebx, 11

  ; End loop
  jmp   merge_loop57

  ; Jump out point if end of dest is reached
  dest_end_exit57:

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  sbb   PTR_EAX, PTR_EAX
  add   PTR_ESI, [STEPTBL_PTR][PTR_SIZE+PTR_EAX*PTR_SIZE]

  ; Jump out point if end of src is reached
  src_end_exit57:

  ; adjust fractional
  shr   edx, 16

  ; Routine end
  ret

Merge_DestStereo_SrcFlipped_Src8_NoVolume_Resample ENDP


Merge_DestMono_SrcFlipped_Src16_NoVolume_Resample PROC ; 58

  ; adjust fractional
  shl   edx, 16

  ; Load sample data
  mov   eax, dword ptr [PTR_ESI]
  movsx ebx, ax
  sar   eax, 16

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; Apply volume
  shl   eax, 11

  ; Merge sample data loop
  ALIGN 16
  merge_loop58:

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   PTR_EDI, 4
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit58

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  sbb   PTR_EAX, PTR_EAX
  add   PTR_ESI, [STEPTBL_PTR][PTR_SIZE+PTR_EAX*PTR_SIZE]
  cmp   PTR_ESI, src_end
  jae   src_end_exit58

  ; Load sample data
  mov   eax, dword ptr [PTR_ESI]
  movsx ebx, ax
  sar   eax, 16

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; Apply volume
  shl   eax, 11

  ; End loop
  jmp   merge_loop58

  ; Jump out point if end of dest is reached
  dest_end_exit58:

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  sbb   PTR_EAX, PTR_EAX
  add   PTR_ESI, [STEPTBL_PTR][PTR_SIZE+PTR_EAX*PTR_SIZE]

  ; Jump out point if end of src is reached
  src_end_exit58:

  ; adjust fractional
  shr   edx, 16

  ; Routine end
  ret

Merge_DestMono_SrcFlipped_Src16_NoVolume_Resample ENDP


Merge_DestStereo_SrcFlipped_Src16_NoVolume_Resample PROC ; 59

  ; adjust fractional
  shl   edx, 16

  ; Load sample data
  mov   eax, dword ptr [PTR_ESI]
  movsx ebx, ax
  sar   eax, 16

  ; Apply volume
  shl   eax, 11
  shl   ebx, 11

  ; Merge sample data loop
  ALIGN 16
  merge_loop59:

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   [PTR_EDI+4], ebx
  add   PTR_EDI, 8
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit59

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  sbb   PTR_EAX, PTR_EAX
  add   PTR_ESI, [STEPTBL_PTR][PTR_SIZE+PTR_EAX*PTR_SIZE]
  cmp   PTR_ESI, src_end
  jae   src_end_exit59

  ; Load sample data
  mov   eax, dword ptr [PTR_ESI]
  movsx ebx, ax
  sar   eax, 16

  ; Apply volume
  shl   eax, 11
  shl   ebx, 11

  ; End loop
  jmp   merge_loop59

  ; Jump out point if end of dest is reached
  dest_end_exit59:

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  sbb   PTR_EAX, PTR_EAX
  add   PTR_ESI, [STEPTBL_PTR][PTR_SIZE+PTR_EAX*PTR_SIZE]

  ; Jump out point if end of src is reached
  src_end_exit59:

  ; adjust fractional
  shr   edx, 16

  ; Routine end
  ret

Merge_DestStereo_SrcFlipped_Src16_NoVolume_Resample ENDP


Merge_DestMono_SrcFlipped_Src8_NoVolume_Filtered PROC ; 60

  ; check to see if we have to call the upsampling version
  cmp   [playback_ratio], 10000h
  jle   Merge_DestMono_SrcFlipped_Src8_NoVolume_UpFiltered

  ; build average dividers
  mov   ecx, edx
  mov   ebx, [playback_ratio]
  xor   edx, edx
  mov   eax, 08000000h  ; 100000000f/32
  div   ebx
  dec   eax
  mov   [divider_l], eax
  mov   edx, ecx

  ; load initial sample
  mov   ecx, [cur_l]

  ; handle start up loop management
  mov  eax, edx
  and  edx, 03fffffffh
  test eax, 080000000h
  jnz  whole_continue60
  test eax, 040000000h
  jnz  last_continue60

  ; Merge sample data loop
  ALIGN 16
  merge_loop60:
  mov   ebx, 65536
  sub   ebx, edx
  add   edx, [playback_ratio]

  ; weight the initial sample
  imul  ecx, ebx
  sar   ecx, 16
  cmp   edx, 65536*2
  jl    skip_loop60

  ; loop to load all of the full sample points
  whole_loop60:
  cmp   PTR_ESI, [src_end]
  jae   src_whole_exit60
  whole_continue60:

  ; Load sample data
  xor   eax, eax
  xor   ebx, ebx
  mov   bx, word ptr [PTR_ESI]
  mov   al, bh
  and   ebx, 0ffh
  sub   eax, 080h
  sub   ebx, 080h
  shl   eax, 8
  shl   ebx, 8

  ; Merge left and right channels for mono dest
  add   eax, ebx
  add   ecx, eax
  sub   edx, 65536
  add   PTR_ESI, 2
  cmp   edx, 65536*2
  jae   whole_loop60

  skip_loop60:
  and   edx, 0ffffh
  last_continue60:
  cmp   PTR_ESI, [src_end]
  jae   src_last_exit60

  ; Load sample data
  xor   eax, eax
  xor   ebx, ebx
  mov   bx, word ptr [PTR_ESI]
  mov   al, bh
  and   ebx, 0ffh
  sub   eax, 080h
  sub   ebx, 080h
  shl   eax, 8
  shl   ebx, 8

  ; Merge left and right channels for mono dest
  add   eax, ebx
  add   PTR_ESI, 2
  mov   ebx, ecx
  mov   ecx, eax

  ; weight the final sample
  imul  eax, edx
  sar   eax, 16
  add   eax, ebx
  imul  eax, [divider_l]

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   PTR_EDI, 4
  cmp   PTR_EDI, dest_end
  jb    merge_loop60

  ; Jump out point if end of dest is reached
  dest_end_exit60:
  jmp   src_save_value60

  ; jump out when src is exceed, but save our current loop position
  src_whole_exit60:
  or   edx, 080000000h
  src_last_exit60:
  or   edx, 040000000h
  src_save_value60:
  mov  [cur_l], ecx

  ; Routine end
  ret

Merge_DestMono_SrcFlipped_Src8_NoVolume_UpFiltered:

  ; adjust fractional
  shl   edx, 16

  ; Load sample data
  xor   eax, eax
  xor   ebx, ebx
  mov   bx, word ptr [PTR_ESI]
  mov   al, bh
  and   ebx, 0ffh
  sub   eax, 080h
  sub   ebx, 080h
  shl   eax, 8
  shl   ebx, 8

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; rotate filter values
  mov   ecx, [cur_l]
  mov   [cur_l], eax

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; Merge sample data loop
  ALIGN 16
  merge_loop572:

  ; Upsample the data points
  mov   ebx, edx
  mov   eax, [cur_l]
  shr   ebx, 17
  sub   eax, ecx
  imul  eax, ebx
  sar   eax, 15
  add   eax, ecx

  ; Apply volume
  shl   eax, 11

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   PTR_EDI, 4
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit572

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  jnc   merge_loop572

  ; Move the source pointer
  add   PTR_ESI, 2
  cmp   PTR_ESI, src_end
  jae   src_end_exit572

  ; Load sample data
  xor   eax, eax
  xor   ebx, ebx
  mov   bx, word ptr [PTR_ESI]
  mov   al, bh
  and   ebx, 0ffh
  sub   eax, 080h
  sub   ebx, 080h
  shl   eax, 8
  shl   ebx, 8

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; rotate filter values
  mov   ecx, [cur_l]
  mov   [cur_l], eax

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; End loop
  jmp   merge_loop572

  ; Jump out point if end of dest is reached
  dest_end_exit572:
  add   PTR_ESI, 2
  add   edx, [step_fract]
  jc    skip_filter_adjust572

  ; rotate filter values
  mov   [cur_l], ecx

  ; un-increment the source to skip the early source adjustment
  sub   PTR_ESI, 2

  skip_filter_adjust572:

  ; Jump out point if end of src is reached
  src_end_exit572:

  ; adjust fractional
  shr   edx, 16

  ; Routine end
  ret

Merge_DestMono_SrcFlipped_Src8_NoVolume_Filtered ENDP


Merge_DestStereo_SrcFlipped_Src8_NoVolume_Filtered PROC ; 61

  ; check to see if we have to call the upsampling version
  cmp   [playback_ratio], 10000h
  jle   Merge_DestStereo_SrcFlipped_Src8_NoVolume_UpFiltered

  ; build average dividers
  mov   ecx, edx
  mov   ebx, [playback_ratio]
  xor   edx, edx
  mov   eax, 08000000h  ; 100000000f/32
  div   ebx
  dec   eax
  mov   [divider_l], eax
  xor   edx, edx
  mov   eax, 08000000h  ; 100000000f/32
  div   ebx
  dec   eax
  mov   [divider_r], eax
  mov   edx, ecx

  ; load initial sample
  mov   ecx, [cur_l]

  ; Save registers
  push  PTR_EBP

  ; handle start up loop management
  mov  eax, edx
  and  edx, 03fffffffh
  test eax, 080000000h
  jnz  whole_continue61
  test eax, 040000000h
  jnz  last_continue61

  ; Merge sample data loop
  ALIGN 16
  merge_loop61:
  mov   ebx, 65536
  sub   ebx, edx
  add   edx, [playback_ratio]

  ; weight the initial sample
  mov   eax, [cur_r]
  imul  ecx, ebx
  imul  eax, ebx
  sar   ecx, 16
  sar   eax, 16
  cmp   edx, 65536*2
  mov   [cur_r], eax
  jl    skip_loop61

  ; loop to load all of the full sample points
  whole_loop61:
  cmp   PTR_ESI, [src_end]
  jae   src_whole_exit61
  whole_continue61:

  ; Load sample data
  xor   eax, eax
  xor   ebx, ebx
  mov   bx, word ptr [PTR_ESI]
  mov   al, bh
  and   ebx, 0ffh
  sub   eax, 080h
  sub   ebx, 080h
  shl   eax, 8
  shl   ebx, 8
  add   ecx, eax
  add   [cur_r], ebx
  sub   edx, 65536
  add   PTR_ESI, 2
  cmp   edx, 65536*2
  jae   whole_loop61

  skip_loop61:
  and   edx, 0ffffh
  cmp   PTR_ESI, [src_end]
  jae   src_last_exit61
  last_continue61:

  ; Load sample data
  xor   eax, eax
  xor   ebx, ebx
  mov   bx, word ptr [PTR_ESI]
  mov   al, bh
  and   ebx, 0ffh
  sub   eax, 080h
  sub   ebx, 080h
  shl   eax, 8
  shl   ebx, 8
  add   PTR_ESI, 2
  mov   ebp, ecx
  mov   ecx, eax

  ; weight the final sample
  imul  eax, edx
  sar   eax, 16
  add   eax, ebp
  mov   ebp, [cur_r]
  mov   [cur_r], ebx
  imul  ebx, edx
  sar   ebx, 16
  add   ebx, ebp
  imul  eax, [divider_l]
  imul  ebx, [divider_r]

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   [PTR_EDI+4], ebx
  add   PTR_EDI, 8
  cmp   PTR_EDI, dest_end
  jb    merge_loop61

  ; Jump out point if end of dest is reached
  dest_end_exit61:
  jmp   src_save_value61

  ; jump out when src is exceed, but save our current loop position
  src_whole_exit61:
  or   edx, 080000000h
  src_last_exit61:
  or   edx, 040000000h
  src_save_value61:
  mov  [cur_l], ecx

  ; Restore registers
  pop   PTR_EBP

  ; Routine end
  ret

Merge_DestStereo_SrcFlipped_Src8_NoVolume_UpFiltered:

  ; adjust fractional
  shl   edx, 16

  ; Save registers
  push  PTR_EBP

  ; Load sample data
  xor   eax, eax
  xor   ebx, ebx
  mov   bx, word ptr [PTR_ESI]
  mov   al, bh
  and   ebx, 0ffh
  sub   eax, 080h
  sub   ebx, 080h
  shl   eax, 8
  shl   ebx, 8

  ; rotate filter values
  mov   ecx, [cur_l]
  mov   [cur_l], eax
  mov   eax, [cur_r]
  mov   [cur_r], ebx
  mov   [cur_r2], eax

  ; Merge sample data loop
  ALIGN 16
  merge_loop573:

  ; Upsample the data points
  mov   ebp, edx
  mov   eax, [cur_l]
  mov   ebx, [cur_r]
  shr   ebp, 17
  sub   eax, ecx
  sub   ebx, [cur_r2]
  imul  eax, ebp
  imul  ebx, ebp
  sar   eax, 15
  sar   ebx, 15
  add   eax, ecx
  add   ebx, [cur_r2]

  ; Apply volume
  shl   eax, 11
  shl   ebx, 11

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   [PTR_EDI+4], ebx
  add   PTR_EDI, 8
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit573

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  jnc   merge_loop573

  ; Move the source pointer
  add   PTR_ESI, 2
  cmp   PTR_ESI, src_end
  jae   src_end_exit573

  ; Load sample data
  xor   eax, eax
  xor   ebx, ebx
  mov   bx, word ptr [PTR_ESI]
  mov   al, bh
  and   ebx, 0ffh
  sub   eax, 080h
  sub   ebx, 080h
  shl   eax, 8
  shl   ebx, 8

  ; rotate filter values
  mov   ecx, [cur_l]
  mov   [cur_l], eax
  mov   eax, [cur_r]
  mov   [cur_r], ebx
  mov   [cur_r2], eax

  ; End loop
  jmp   merge_loop573

  ; Jump out point if end of dest is reached
  dest_end_exit573:
  add   PTR_ESI, 2
  add   edx, [step_fract]
  jc    skip_filter_adjust573

  ; rotate filter values
  mov   [cur_l], ecx
  mov   ecx, [cur_r2]
  mov   [cur_r], ecx

  ; un-increment the source to skip the early source adjustment
  sub   PTR_ESI, 2

  skip_filter_adjust573:

  ; Jump out point if end of src is reached
  src_end_exit573:

  ; Restore registers
  pop   PTR_EBP

  ; adjust fractional
  shr   edx, 16

  ; Routine end
  ret

Merge_DestStereo_SrcFlipped_Src8_NoVolume_Filtered ENDP


Merge_DestMono_SrcFlipped_Src16_NoVolume_Filtered PROC ; 62

  ; check to see if we have to call the upsampling version
  cmp   [playback_ratio], 10000h
  jle   Merge_DestMono_SrcFlipped_Src16_NoVolume_UpFiltered

  ; build average dividers
  mov   ecx, edx
  mov   ebx, [playback_ratio]
  xor   edx, edx
  mov   eax, 08000000h  ; 100000000f/32
  div   ebx
  dec   eax
  mov   [divider_l], eax
  mov   edx, ecx

  ; load initial sample
  mov   ecx, [cur_l]

  ; handle start up loop management
  mov  eax, edx
  and  edx, 03fffffffh
  test eax, 080000000h
  jnz  whole_continue62
  test eax, 040000000h
  jnz  last_continue62

  ; Merge sample data loop
  ALIGN 16
  merge_loop62:
  mov   ebx, 65536
  sub   ebx, edx
  add   edx, [playback_ratio]

  ; weight the initial sample
  imul  ecx, ebx
  sar   ecx, 16
  cmp   edx, 65536*2
  jl    skip_loop62

  ; loop to load all of the full sample points
  whole_loop62:
  cmp   PTR_ESI, [src_end]
  jae   src_whole_exit62
  whole_continue62:

  ; Load sample data
  mov   eax, dword ptr [PTR_ESI]
  movsx ebx, ax
  sar   eax, 16

  ; Merge left and right channels for mono dest
  add   eax, ebx
  add   ecx, eax
  sub   edx, 65536
  add   PTR_ESI, 4
  cmp   edx, 65536*2
  jae   whole_loop62

  skip_loop62:
  and   edx, 0ffffh
  last_continue62:
  cmp   PTR_ESI, [src_end]
  jae   src_last_exit62

  ; Load sample data
  mov   eax, dword ptr [PTR_ESI]
  movsx ebx, ax
  sar   eax, 16

  ; Merge left and right channels for mono dest
  add   eax, ebx
  add   PTR_ESI, 4
  mov   ebx, ecx
  mov   ecx, eax

  ; weight the final sample
  imul  eax, edx
  sar   eax, 16
  add   eax, ebx
  imul  eax, [divider_l]

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   PTR_EDI, 4
  cmp   PTR_EDI, dest_end
  jb    merge_loop62

  ; Jump out point if end of dest is reached
  dest_end_exit62:
  jmp   src_save_value62

  ; jump out when src is exceed, but save our current loop position
  src_whole_exit62:
  or   edx, 080000000h
  src_last_exit62:
  or   edx, 040000000h
  src_save_value62:
  mov  [cur_l], ecx

  ; Routine end
  ret

Merge_DestMono_SrcFlipped_Src16_NoVolume_UpFiltered:

  ; adjust fractional
  shl   edx, 16

  ; Load sample data
  mov   eax, dword ptr [PTR_ESI]
  movsx ebx, ax
  sar   eax, 16

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; rotate filter values
  mov   ecx, [cur_l]
  mov   [cur_l], eax

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; Merge sample data loop
  ALIGN 16
  merge_loop574:

  ; Upsample the data points
  mov   ebx, edx
  mov   eax, [cur_l]
  shr   ebx, 17
  sub   eax, ecx
  imul  eax, ebx
  sar   eax, 15
  add   eax, ecx

  ; Apply volume
  shl   eax, 11

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   PTR_EDI, 4
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit574

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  jnc   merge_loop574

  ; Move the source pointer
  add   PTR_ESI, 4
  cmp   PTR_ESI, src_end
  jae   src_end_exit574

  ; Load sample data
  mov   eax, dword ptr [PTR_ESI]
  movsx ebx, ax
  sar   eax, 16

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; rotate filter values
  mov   ecx, [cur_l]
  mov   [cur_l], eax

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; End loop
  jmp   merge_loop574

  ; Jump out point if end of dest is reached
  dest_end_exit574:
  add   PTR_ESI, 4
  add   edx, [step_fract]
  jc    skip_filter_adjust574

  ; rotate filter values
  mov   [cur_l], ecx

  ; un-increment the source to skip the early source adjustment
  sub   PTR_ESI, 4

  skip_filter_adjust574:

  ; Jump out point if end of src is reached
  src_end_exit574:

  ; adjust fractional
  shr   edx, 16

  ; Routine end
  ret

Merge_DestMono_SrcFlipped_Src16_NoVolume_Filtered ENDP


Merge_DestStereo_SrcFlipped_Src16_NoVolume_Filtered PROC ; 63

  ; check to see if we have to call the upsampling version
  cmp   [playback_ratio], 10000h
  jle   Merge_DestStereo_SrcFlipped_Src16_NoVolume_UpFiltered

  ; build average dividers
  mov   ecx, edx
  mov   ebx, [playback_ratio]
  xor   edx, edx
  mov   eax, 08000000h  ; 100000000f/32
  div   ebx
  dec   eax
  mov   [divider_l], eax
  xor   edx, edx
  mov   eax, 08000000h  ; 100000000f/32
  div   ebx
  dec   eax
  mov   [divider_r], eax
  mov   edx, ecx

  ; load initial sample
  mov   ecx, [cur_l]

  ; Save registers
  push  PTR_EBP

  ; handle start up loop management
  mov  eax, edx
  and  edx, 03fffffffh
  test eax, 080000000h
  jnz  whole_continue63
  test eax, 040000000h
  jnz  last_continue63

  ; Merge sample data loop
  ALIGN 16
  merge_loop63:
  mov   ebx, 65536
  sub   ebx, edx
  add   edx, [playback_ratio]

  ; weight the initial sample
  mov   eax, [cur_r]
  imul  ecx, ebx
  imul  eax, ebx
  sar   ecx, 16
  sar   eax, 16
  cmp   edx, 65536*2
  mov   [cur_r], eax
  jl    skip_loop63

  ; loop to load all of the full sample points
  whole_loop63:
  cmp   PTR_ESI, [src_end]
  jae   src_whole_exit63
  whole_continue63:

  ; Load sample data
  mov   eax, dword ptr [PTR_ESI]
  movsx ebx, ax
  sar   eax, 16
  add   ecx, eax
  add   [cur_r], ebx
  sub   edx, 65536
  add   PTR_ESI, 4
  cmp   edx, 65536*2
  jae   whole_loop63

  skip_loop63:
  and   edx, 0ffffh
  cmp   PTR_ESI, [src_end]
  jae   src_last_exit63
  last_continue63:

  ; Load sample data
  mov   eax, dword ptr [PTR_ESI]
  movsx ebx, ax
  sar   eax, 16
  add   PTR_ESI, 4
  mov   ebp, ecx
  mov   ecx, eax

  ; weight the final sample
  imul  eax, edx
  sar   eax, 16
  add   eax, ebp
  mov   ebp, [cur_r]
  mov   [cur_r], ebx
  imul  ebx, edx
  sar   ebx, 16
  add   ebx, ebp
  imul  eax, [divider_l]
  imul  ebx, [divider_r]

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   [PTR_EDI+4], ebx
  add   PTR_EDI, 8
  cmp   PTR_EDI, dest_end
  jb    merge_loop63

  ; Jump out point if end of dest is reached
  dest_end_exit63:
  jmp   src_save_value63

  ; jump out when src is exceed, but save our current loop position
  src_whole_exit63:
  or   edx, 080000000h
  src_last_exit63:
  or   edx, 040000000h
  src_save_value63:
  mov  [cur_l], ecx

  ; Restore registers
  pop   PTR_EBP

  ; Routine end
  ret

Merge_DestStereo_SrcFlipped_Src16_NoVolume_UpFiltered:

  ; adjust fractional
  shl   edx, 16

  ; Save registers
  push  PTR_EBP

  ; Load sample data
  mov   eax, dword ptr [PTR_ESI]
  movsx ebx, ax
  sar   eax, 16

  ; rotate filter values
  mov   ecx, [cur_l]
  mov   [cur_l], eax
  mov   eax, [cur_r]
  mov   [cur_r], ebx
  mov   [cur_r2], eax

  ; Merge sample data loop
  ALIGN 16
  merge_loop575:

  ; Upsample the data points
  mov   ebp, edx
  mov   eax, [cur_l]
  mov   ebx, [cur_r]
  shr   ebp, 17
  sub   eax, ecx
  sub   ebx, [cur_r2]
  imul  eax, ebp
  imul  ebx, ebp
  sar   eax, 15
  sar   ebx, 15
  add   eax, ecx
  add   ebx, [cur_r2]

  ; Apply volume
  shl   eax, 11
  shl   ebx, 11

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   [PTR_EDI+4], ebx
  add   PTR_EDI, 8
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit575

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  jnc   merge_loop575

  ; Move the source pointer
  add   PTR_ESI, 4
  cmp   PTR_ESI, src_end
  jae   src_end_exit575

  ; Load sample data
  mov   eax, dword ptr [PTR_ESI]
  movsx ebx, ax
  sar   eax, 16

  ; rotate filter values
  mov   ecx, [cur_l]
  mov   [cur_l], eax
  mov   eax, [cur_r]
  mov   [cur_r], ebx
  mov   [cur_r2], eax

  ; End loop
  jmp   merge_loop575

  ; Jump out point if end of dest is reached
  dest_end_exit575:
  add   PTR_ESI, 4
  add   edx, [step_fract]
  jc    skip_filter_adjust575

  ; rotate filter values
  mov   [cur_l], ecx
  mov   ecx, [cur_r2]
  mov   [cur_r], ecx

  ; un-increment the source to skip the early source adjustment
  sub   PTR_ESI, 4

  skip_filter_adjust575:

  ; Jump out point if end of src is reached
  src_end_exit575:

  ; Restore registers
  pop   PTR_EBP

  ; adjust fractional
  shr   edx, 16

  ; Routine end
  ret

Merge_DestStereo_SrcFlipped_Src16_NoVolume_Filtered ENDP


Merge_DestMono_SrcMono_Src8_Volume_NoResample PROC ; 64

  ; Load sample data
  xor   eax, eax
  mov   al, byte ptr [PTR_ESI]
  sub   eax, 080h
  shl   eax, 8

  ; Apply volume
  imul  eax, [scale_left]

  ; Merge sample data loop
  ALIGN 16
  merge_loop64:

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   PTR_EDI, 4
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit64

  ; Move the source pointer
  inc   PTR_ESI
  cmp   PTR_ESI, src_end
  jae   src_end_exit64

  ; Load sample data
  xor   eax, eax
  mov   al, byte ptr [PTR_ESI]
  sub   eax, 080h
  shl   eax, 8

  ; Apply volume
  imul  eax, [scale_left]

  ; End loop
  jmp   merge_loop64

  ; Jump out point if end of dest is reached
  dest_end_exit64:
  inc   PTR_ESI

  ; Jump out point if end of src is reached
  src_end_exit64:

  ; Routine end
  ret

Merge_DestMono_SrcMono_Src8_Volume_NoResample ENDP


Merge_DestStereo_SrcMono_Src8_Volume_NoResample PROC ; 65

  ; Load sample data
  xor   eax, eax
  mov   al, byte ptr [PTR_ESI]
  sub   eax, 080h
  shl   eax, 8

  ; Duplicate the left channel into the right
  mov   ebx, eax

  ; Apply volume
  imul  eax, [scale_left]
  imul  ebx, [scale_right]

  ; Merge sample data loop
  ALIGN 16
  merge_loop65:

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   [PTR_EDI+4], ebx
  add   PTR_EDI, 8
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit65

  ; Move the source pointer
  inc   PTR_ESI
  cmp   PTR_ESI, src_end
  jae   src_end_exit65

  ; Load sample data
  xor   eax, eax
  mov   al, byte ptr [PTR_ESI]
  sub   eax, 080h
  shl   eax, 8

  ; Duplicate the left channel into the right
  mov   ebx, eax

  ; Apply volume
  imul  eax, [scale_left]
  imul  ebx, [scale_right]

  ; End loop
  jmp   merge_loop65

  ; Jump out point if end of dest is reached
  dest_end_exit65:
  inc   PTR_ESI

  ; Jump out point if end of src is reached
  src_end_exit65:

  ; Routine end
  ret

Merge_DestStereo_SrcMono_Src8_Volume_NoResample ENDP


Merge_DestMono_SrcMono_Src16_Volume_NoResample PROC ; 66

  ; Load sample data
  movsx eax, word ptr [PTR_ESI]

  ; Apply volume
  imul  eax, [scale_left]

  ; Merge sample data loop
  ALIGN 16
  merge_loop66:

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   PTR_EDI, 4
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit66

  ; Move the source pointer
  add   PTR_ESI, 2
  cmp   PTR_ESI, src_end
  jae   src_end_exit66

  ; Load sample data
  movsx eax, word ptr [PTR_ESI]

  ; Apply volume
  imul  eax, [scale_left]

  ; End loop
  jmp   merge_loop66

  ; Jump out point if end of dest is reached
  dest_end_exit66:
  add   PTR_ESI, 2

  ; Jump out point if end of src is reached
  src_end_exit66:

  ; Routine end
  ret

Merge_DestMono_SrcMono_Src16_Volume_NoResample ENDP


Merge_DestStereo_SrcMono_Src16_Volume_NoResample PROC ; 67

  ; Load sample data
  movsx eax, word ptr [PTR_ESI]

  ; Duplicate the left channel into the right
  mov   ebx, eax

  ; Apply volume
  imul  eax, [scale_left]
  imul  ebx, [scale_right]

  ; Merge sample data loop
  ALIGN 16
  merge_loop67:

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   [PTR_EDI+4], ebx
  add   PTR_EDI, 8
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit67

  ; Move the source pointer
  add   PTR_ESI, 2
  cmp   PTR_ESI, src_end
  jae   src_end_exit67

  ; Load sample data
  movsx eax, word ptr [PTR_ESI]

  ; Duplicate the left channel into the right
  mov   ebx, eax

  ; Apply volume
  imul  eax, [scale_left]
  imul  ebx, [scale_right]

  ; End loop
  jmp   merge_loop67

  ; Jump out point if end of dest is reached
  dest_end_exit67:
  add   PTR_ESI, 2

  ; Jump out point if end of src is reached
  src_end_exit67:

  ; Routine end
  ret

Merge_DestStereo_SrcMono_Src16_Volume_NoResample ENDP


Merge_DestMono_SrcStereo_Src8_Volume_NoResample PROC ; 72

  ; Load sample data
  xor   ebx, ebx
  xor   eax, eax
  mov   ax, word ptr [PTR_ESI]
  mov   bl, ah
  and   eax, 0ffh
  sub   ebx, 080h
  sub   eax, 080h
  shl   ebx, 8
  shl   eax, 8

  ; Apply volume
  imul  eax, [scale_left]
  imul  ebx, [scale_right]

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; Merge sample data loop
  ALIGN 16
  merge_loop72:

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   PTR_EDI, 4
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit72

  ; Move the source pointer
  add   PTR_ESI, 2
  cmp   PTR_ESI, src_end
  jae   src_end_exit72

  ; Load sample data
  xor   ebx, ebx
  xor   eax, eax
  mov   ax, word ptr [PTR_ESI]
  mov   bl, ah
  and   eax, 0ffh
  sub   ebx, 080h
  sub   eax, 080h
  shl   ebx, 8
  shl   eax, 8

  ; Apply volume
  imul  eax, [scale_left]
  imul  ebx, [scale_right]

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; End loop
  jmp   merge_loop72

  ; Jump out point if end of dest is reached
  dest_end_exit72:
  add   PTR_ESI, 2

  ; Jump out point if end of src is reached
  src_end_exit72:

  ; Routine end
  ret

Merge_DestMono_SrcStereo_Src8_Volume_NoResample ENDP


Merge_DestStereo_SrcStereo_Src8_Volume_NoResample PROC ; 73

  ; Load sample data
  xor   ebx, ebx
  xor   eax, eax
  mov   ax, word ptr [PTR_ESI]
  mov   bl, ah
  and   eax, 0ffh
  sub   ebx, 080h
  sub   eax, 080h
  shl   ebx, 8
  shl   eax, 8

  ; Apply volume
  imul  eax, [scale_left]
  imul  ebx, [scale_right]

  ; Merge sample data loop
  ALIGN 16
  merge_loop73:

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   [PTR_EDI+4], ebx
  add   PTR_EDI, 8
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit73

  ; Move the source pointer
  add   PTR_ESI, 2
  cmp   PTR_ESI, src_end
  jae   src_end_exit73

  ; Load sample data
  xor   ebx, ebx
  xor   eax, eax
  mov   ax, word ptr [PTR_ESI]
  mov   bl, ah
  and   eax, 0ffh
  sub   ebx, 080h
  sub   eax, 080h
  shl   ebx, 8
  shl   eax, 8

  ; Apply volume
  imul  eax, [scale_left]
  imul  ebx, [scale_right]

  ; End loop
  jmp   merge_loop73

  ; Jump out point if end of dest is reached
  dest_end_exit73:
  add   PTR_ESI, 2

  ; Jump out point if end of src is reached
  src_end_exit73:

  ; Routine end
  ret

Merge_DestStereo_SrcStereo_Src8_Volume_NoResample ENDP


Merge_DestMono_SrcStereo_Src16_Volume_NoResample PROC ; 74

  ; Load sample data
  mov   ebx, dword ptr [PTR_ESI]
  movsx eax, bx
  sar   ebx, 16

  ; Apply volume
  imul  eax, [scale_left]
  imul  ebx, [scale_right]

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; Merge sample data loop
  ALIGN 16
  merge_loop74:

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   PTR_EDI, 4
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit74

  ; Move the source pointer
  add   PTR_ESI, 4
  cmp   PTR_ESI, src_end
  jae   src_end_exit74

  ; Load sample data
  mov   ebx, dword ptr [PTR_ESI]
  movsx eax, bx
  sar   ebx, 16

  ; Apply volume
  imul  eax, [scale_left]
  imul  ebx, [scale_right]

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; End loop
  jmp   merge_loop74

  ; Jump out point if end of dest is reached
  dest_end_exit74:
  add   PTR_ESI, 4

  ; Jump out point if end of src is reached
  src_end_exit74:

  ; Routine end
  ret

Merge_DestMono_SrcStereo_Src16_Volume_NoResample ENDP


Merge_DestStereo_SrcStereo_Src16_Volume_NoResample PROC ; 75

  ; Load sample data
  mov   ebx, dword ptr [PTR_ESI]
  movsx eax, bx
  sar   ebx, 16

  ; Apply volume
  imul  eax, [scale_left]
  imul  ebx, [scale_right]

  ; Merge sample data loop
  ALIGN 16
  merge_loop75:

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   [PTR_EDI+4], ebx
  add   PTR_EDI, 8
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit75

  ; Move the source pointer
  add   PTR_ESI, 4
  cmp   PTR_ESI, src_end
  jae   src_end_exit75

  ; Load sample data
  mov   ebx, dword ptr [PTR_ESI]
  movsx eax, bx
  sar   ebx, 16

  ; Apply volume
  imul  eax, [scale_left]
  imul  ebx, [scale_right]

  ; End loop
  jmp   merge_loop75

  ; Jump out point if end of dest is reached
  dest_end_exit75:
  add   PTR_ESI, 4

  ; Jump out point if end of src is reached
  src_end_exit75:

  ; Routine end
  ret

Merge_DestStereo_SrcStereo_Src16_Volume_NoResample ENDP


Merge_DestMono_SrcFlipped_Src8_Volume_NoResample PROC ; 88

  ; Load sample data
  xor   eax, eax
  xor   ebx, ebx
  mov   bx, word ptr [PTR_ESI]
  mov   al, bh
  and   ebx, 0ffh
  sub   eax, 080h
  sub   ebx, 080h
  shl   eax, 8
  shl   ebx, 8

  ; Apply volume
  imul  eax, [scale_left]
  imul  ebx, [scale_right]

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; Merge sample data loop
  ALIGN 16
  merge_loop88:

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   PTR_EDI, 4
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit88

  ; Move the source pointer
  add   PTR_ESI, 2
  cmp   PTR_ESI, src_end
  jae   src_end_exit88

  ; Load sample data
  xor   eax, eax
  xor   ebx, ebx
  mov   bx, word ptr [PTR_ESI]
  mov   al, bh
  and   ebx, 0ffh
  sub   eax, 080h
  sub   ebx, 080h
  shl   eax, 8
  shl   ebx, 8

  ; Apply volume
  imul  eax, [scale_left]
  imul  ebx, [scale_right]

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; End loop
  jmp   merge_loop88

  ; Jump out point if end of dest is reached
  dest_end_exit88:
  add   PTR_ESI, 2

  ; Jump out point if end of src is reached
  src_end_exit88:

  ; Routine end
  ret

Merge_DestMono_SrcFlipped_Src8_Volume_NoResample ENDP


Merge_DestStereo_SrcFlipped_Src8_Volume_NoResample PROC ; 89

  ; Load sample data
  xor   eax, eax
  xor   ebx, ebx
  mov   bx, word ptr [PTR_ESI]
  mov   al, bh
  and   ebx, 0ffh
  sub   eax, 080h
  sub   ebx, 080h
  shl   eax, 8
  shl   ebx, 8

  ; Apply volume
  imul  eax, [scale_left]
  imul  ebx, [scale_right]

  ; Merge sample data loop
  ALIGN 16
  merge_loop89:

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   [PTR_EDI+4], ebx
  add   PTR_EDI, 8
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit89

  ; Move the source pointer
  add   PTR_ESI, 2
  cmp   PTR_ESI, src_end
  jae   src_end_exit89

  ; Load sample data
  xor   eax, eax
  xor   ebx, ebx
  mov   bx, word ptr [PTR_ESI]
  mov   al, bh
  and   ebx, 0ffh
  sub   eax, 080h
  sub   ebx, 080h
  shl   eax, 8
  shl   ebx, 8

  ; Apply volume
  imul  eax, [scale_left]
  imul  ebx, [scale_right]

  ; End loop
  jmp   merge_loop89

  ; Jump out point if end of dest is reached
  dest_end_exit89:
  add   PTR_ESI, 2

  ; Jump out point if end of src is reached
  src_end_exit89:

  ; Routine end
  ret

Merge_DestStereo_SrcFlipped_Src8_Volume_NoResample ENDP


Merge_DestMono_SrcFlipped_Src16_Volume_NoResample PROC ; 90

  ; Load sample data
  mov   eax, dword ptr [PTR_ESI]
  movsx ebx, ax
  sar   eax, 16

  ; Apply volume
  imul  eax, [scale_left]
  imul  ebx, [scale_right]

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; Merge sample data loop
  ALIGN 16
  merge_loop90:

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   PTR_EDI, 4
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit90

  ; Move the source pointer
  add   PTR_ESI, 4
  cmp   PTR_ESI, src_end
  jae   src_end_exit90

  ; Load sample data
  mov   eax, dword ptr [PTR_ESI]
  movsx ebx, ax
  sar   eax, 16

  ; Apply volume
  imul  eax, [scale_left]
  imul  ebx, [scale_right]

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; End loop
  jmp   merge_loop90

  ; Jump out point if end of dest is reached
  dest_end_exit90:
  add   PTR_ESI, 4

  ; Jump out point if end of src is reached
  src_end_exit90:

  ; Routine end
  ret

Merge_DestMono_SrcFlipped_Src16_Volume_NoResample ENDP


Merge_DestStereo_SrcFlipped_Src16_Volume_NoResample PROC ; 91

  ; Load sample data
  mov   eax, dword ptr [PTR_ESI]
  movsx ebx, ax
  sar   eax, 16

  ; Apply volume
  imul  eax, [scale_left]
  imul  ebx, [scale_right]

  ; Merge sample data loop
  ALIGN 16
  merge_loop91:

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   [PTR_EDI+4], ebx
  add   PTR_EDI, 8
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit91

  ; Move the source pointer
  add   PTR_ESI, 4
  cmp   PTR_ESI, src_end
  jae   src_end_exit91

  ; Load sample data
  mov   eax, dword ptr [PTR_ESI]
  movsx ebx, ax
  sar   eax, 16

  ; Apply volume
  imul  eax, [scale_left]
  imul  ebx, [scale_right]

  ; End loop
  jmp   merge_loop91

  ; Jump out point if end of dest is reached
  dest_end_exit91:
  add   PTR_ESI, 4

  ; Jump out point if end of src is reached
  src_end_exit91:

  ; Routine end
  ret

Merge_DestStereo_SrcFlipped_Src16_Volume_NoResample ENDP


Merge_DestMono_SrcMono_Src8_Volume_Resample PROC ; 96

  ; adjust fractional
  shl   edx, 16

  ; Load sample data
  xor   eax, eax
  mov   al, byte ptr [PTR_ESI]
  sub   eax, 080h
  shl   eax, 8

  ; Apply volume
  imul  eax, [scale_left]

  ; Merge sample data loop
  ALIGN 16
  merge_loop96:

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   PTR_EDI, 4
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit96

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  sbb   PTR_EAX, PTR_EAX
  add   PTR_ESI, [STEPTBL_PTR][PTR_SIZE+PTR_EAX*PTR_SIZE]
  cmp   PTR_ESI, src_end
  jae   src_end_exit96

  ; Load sample data
  xor   eax, eax
  mov   al, byte ptr [PTR_ESI]
  sub   eax, 080h
  shl   eax, 8

  ; Apply volume
  imul  eax, [scale_left]

  ; End loop
  jmp   merge_loop96

  ; Jump out point if end of dest is reached
  dest_end_exit96:

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  sbb   PTR_EAX, PTR_EAX
  add   PTR_ESI, [STEPTBL_PTR][PTR_SIZE+PTR_EAX*PTR_SIZE]

  ; Jump out point if end of src is reached
  src_end_exit96:

  ; adjust fractional
  shr   edx, 16

  ; Routine end
  ret

Merge_DestMono_SrcMono_Src8_Volume_Resample ENDP


Merge_DestStereo_SrcMono_Src8_Volume_Resample PROC ; 97

  ; adjust fractional
  shl   edx, 16

  ; Load sample data
  xor   eax, eax
  mov   al, byte ptr [PTR_ESI]
  sub   eax, 080h
  shl   eax, 8

  ; Duplicate the left channel into the right
  mov   ebx, eax

  ; Apply volume
  imul  eax, [scale_left]
  imul  ebx, [scale_right]

  ; Merge sample data loop
  ALIGN 16
  merge_loop97:

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   [PTR_EDI+4], ebx
  add   PTR_EDI, 8
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit97

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  sbb   PTR_EAX, PTR_EAX
  add   PTR_ESI, [STEPTBL_PTR][PTR_SIZE+PTR_EAX*PTR_SIZE]
  cmp   PTR_ESI, src_end
  jae   src_end_exit97

  ; Load sample data
  xor   eax, eax
  mov   al, byte ptr [PTR_ESI]
  sub   eax, 080h
  shl   eax, 8

  ; Duplicate the left channel into the right
  mov   ebx, eax

  ; Apply volume
  imul  eax, [scale_left]
  imul  ebx, [scale_right]

  ; End loop
  jmp   merge_loop97

  ; Jump out point if end of dest is reached
  dest_end_exit97:

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  sbb   PTR_EAX, PTR_EAX
  add   PTR_ESI, [STEPTBL_PTR][PTR_SIZE+PTR_EAX*PTR_SIZE]

  ; Jump out point if end of src is reached
  src_end_exit97:

  ; adjust fractional
  shr   edx, 16

  ; Routine end
  ret

Merge_DestStereo_SrcMono_Src8_Volume_Resample ENDP


Merge_DestMono_SrcMono_Src16_Volume_Resample PROC ; 98

  ; adjust fractional
  shl   edx, 16

  ; Load sample data
  movsx eax, word ptr [PTR_ESI]

  ; Apply volume
  imul  eax, [scale_left]

  ; Merge sample data loop
  ALIGN 16
  merge_loop98:

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   PTR_EDI, 4
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit98

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  sbb   PTR_EAX, PTR_EAX
  add   PTR_ESI, [STEPTBL_PTR][PTR_SIZE+PTR_EAX*PTR_SIZE]
  cmp   PTR_ESI, src_end
  jae   src_end_exit98

  ; Load sample data
  movsx eax, word ptr [PTR_ESI]

  ; Apply volume
  imul  eax, [scale_left]

  ; End loop
  jmp   merge_loop98

  ; Jump out point if end of dest is reached
  dest_end_exit98:

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  sbb   PTR_EAX, PTR_EAX
  add   PTR_ESI, [STEPTBL_PTR][PTR_SIZE+PTR_EAX*PTR_SIZE]

  ; Jump out point if end of src is reached
  src_end_exit98:

  ; adjust fractional
  shr   edx, 16

  ; Routine end
  ret

Merge_DestMono_SrcMono_Src16_Volume_Resample ENDP


Merge_DestStereo_SrcMono_Src16_Volume_Resample PROC ; 99

  ; adjust fractional
  shl   edx, 16

  ; Load sample data
  movsx eax, word ptr [PTR_ESI]

  ; Duplicate the left channel into the right
  mov   ebx, eax

  ; Apply volume
  imul  eax, [scale_left]
  imul  ebx, [scale_right]

  ; Merge sample data loop
  ALIGN 16
  merge_loop99:

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   [PTR_EDI+4], ebx
  add   PTR_EDI, 8
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit99

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  sbb   PTR_EAX, PTR_EAX
  add   PTR_ESI, [STEPTBL_PTR][PTR_SIZE+PTR_EAX*PTR_SIZE]
  cmp   PTR_ESI, src_end
  jae   src_end_exit99

  ; Load sample data
  movsx eax, word ptr [PTR_ESI]

  ; Duplicate the left channel into the right
  mov   ebx, eax

  ; Apply volume
  imul  eax, [scale_left]
  imul  ebx, [scale_right]

  ; End loop
  jmp   merge_loop99

  ; Jump out point if end of dest is reached
  dest_end_exit99:

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  sbb   PTR_EAX, PTR_EAX
  add   PTR_ESI, [STEPTBL_PTR][PTR_SIZE+PTR_EAX*PTR_SIZE]

  ; Jump out point if end of src is reached
  src_end_exit99:

  ; adjust fractional
  shr   edx, 16

  ; Routine end
  ret

Merge_DestStereo_SrcMono_Src16_Volume_Resample ENDP


Merge_DestMono_SrcMono_Src8_Volume_Filtered PROC ; 100

  ; check to see if we have to call the upsampling version
  cmp   [playback_ratio], 10000h
  jle   Merge_DestMono_SrcMono_Src8_Volume_UpFiltered

  ; build average dividers
  mov   ecx, edx
  mov   ebx, [playback_ratio]
  xor   edx, edx
  mov   eax, 65536  ; 100000000f/2048/32
  imul  [scale_left]
  div   ebx
  dec   eax
  mov   [divider_l], eax
  mov   edx, ecx

  ; load initial sample
  mov   ecx, [cur_l]

  ; handle start up loop management
  mov  eax, edx
  and  edx, 03fffffffh
  test eax, 080000000h
  jnz  whole_continue100
  test eax, 040000000h
  jnz  last_continue100

  ; Merge sample data loop
  ALIGN 16
  merge_loop100:
  mov   ebx, 65536
  sub   ebx, edx
  add   edx, [playback_ratio]

  ; weight the initial sample
  imul  ecx, ebx
  sar   ecx, 16
  cmp   edx, 65536*2
  jl    skip_loop100

  ; loop to load all of the full sample points
  whole_loop100:
  cmp   PTR_ESI, [src_end]
  jae   src_whole_exit100
  whole_continue100:

  ; Load sample data
  xor   eax, eax
  mov   al, byte ptr [PTR_ESI]
  sub   eax, 080h
  shl   eax, 8
  add   ecx, eax
  sub   edx, 65536
  inc   PTR_ESI
  cmp   edx, 65536*2
  jae   whole_loop100

  skip_loop100:
  and   edx, 0ffffh
  last_continue100:
  cmp   PTR_ESI, [src_end]
  jae   src_last_exit100

  ; Load sample data
  xor   eax, eax
  mov   al, byte ptr [PTR_ESI]
  sub   eax, 080h
  shl   eax, 8
  inc   PTR_ESI
  mov   ebx, ecx
  mov   ecx, eax

  ; weight the final sample
  imul  eax, edx
  sar   eax, 16
  add   eax, ebx
  imul  eax, [divider_l]

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   PTR_EDI, 4
  cmp   PTR_EDI, dest_end
  jb    merge_loop100

  ; Jump out point if end of dest is reached
  dest_end_exit100:
  jmp   src_save_value100

  ; jump out when src is exceed, but save our current loop position
  src_whole_exit100:
  or   edx, 080000000h
  src_last_exit100:
  or   edx, 040000000h
  src_save_value100:
  mov  [cur_l], ecx

  ; Routine end
  ret

Merge_DestMono_SrcMono_Src8_Volume_UpFiltered:

  ; adjust fractional
  shl   edx, 16

  ; Load sample data
  xor   eax, eax
  mov   al, byte ptr [PTR_ESI]
  sub   eax, 080h
  shl   eax, 8

  ; rotate filter values
  mov   ecx, [cur_l]
  mov   [cur_l], eax

  ; Merge sample data loop
  ALIGN 16
  merge_loop612:

  ; Upsample the data points
  mov   ebx, edx
  mov   eax, [cur_l]
  shr   ebx, 17
  sub   eax, ecx
  imul  eax, ebx
  sar   eax, 15
  add   eax, ecx

  ; Apply volume
  imul  eax, [scale_left]

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   PTR_EDI, 4
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit612

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  jnc   merge_loop612

  ; Move the source pointer
  inc   PTR_ESI
  cmp   PTR_ESI, src_end
  jae   src_end_exit612

  ; Load sample data
  xor   eax, eax
  mov   al, byte ptr [PTR_ESI]
  sub   eax, 080h
  shl   eax, 8

  ; rotate filter values
  mov   ecx, [cur_l]
  mov   [cur_l], eax

  ; End loop
  jmp   merge_loop612

  ; Jump out point if end of dest is reached
  dest_end_exit612:
  inc   PTR_ESI
  add   edx, [step_fract]
  jc    skip_filter_adjust612

  ; rotate filter values
  mov   [cur_l], ecx

  ; un-increment the source to skip the early source adjustment
  dec   PTR_ESI

  skip_filter_adjust612:

  ; Jump out point if end of src is reached
  src_end_exit612:

  ; adjust fractional
  shr   edx, 16

  ; Routine end
  ret

Merge_DestMono_SrcMono_Src8_Volume_Filtered ENDP


Merge_DestStereo_SrcMono_Src8_Volume_Filtered PROC ; 101

  ; check to see if we have to call the upsampling version
  cmp   [playback_ratio], 10000h
  jle   Merge_DestStereo_SrcMono_Src8_Volume_UpFiltered

  ; build average dividers
  mov   ecx, edx
  mov   ebx, [playback_ratio]
  xor   edx, edx
  mov   eax, 65536  ; 100000000f/2048/32
  imul  [scale_left]
  div   ebx
  dec   eax
  mov   [divider_l], eax
  xor   edx, edx
  mov   eax, 65536  ; 100000000f/2048/32
  imul  [scale_right]
  div   ebx
  dec   eax
  mov   [divider_r], eax
  mov   edx, ecx

  ; load initial sample
  mov   ecx, [cur_l]

  ; handle start up loop management
  mov  eax, edx
  and  edx, 03fffffffh
  test eax, 080000000h
  jnz  whole_continue101
  test eax, 040000000h
  jnz  last_continue101

  ; Merge sample data loop
  ALIGN 16
  merge_loop101:
  mov   ebx, 65536
  sub   ebx, edx
  add   edx, [playback_ratio]

  ; weight the initial sample
  imul  ecx, ebx
  sar   ecx, 16
  cmp   edx, 65536*2
  jl    skip_loop101

  ; loop to load all of the full sample points
  whole_loop101:
  cmp   PTR_ESI, [src_end]
  jae   src_whole_exit101
  whole_continue101:

  ; Load sample data
  xor   eax, eax
  mov   al, byte ptr [PTR_ESI]
  sub   eax, 080h
  shl   eax, 8
  add   ecx, eax
  sub   edx, 65536
  inc   PTR_ESI
  cmp   edx, 65536*2
  jae   whole_loop101

  skip_loop101:
  and   edx, 0ffffh
  last_continue101:
  cmp   PTR_ESI, [src_end]
  jae   src_last_exit101

  ; Load sample data
  xor   eax, eax
  mov   al, byte ptr [PTR_ESI]
  sub   eax, 080h
  shl   eax, 8
  inc   PTR_ESI
  mov   ebx, ecx
  mov   ecx, eax

  ; weight the final sample
  imul  eax, edx
  sar   eax, 16
  add   eax, ebx

  ; Duplicate the left channel into the right
  mov   ebx, eax
  imul  eax, [divider_l]
  imul  ebx, [divider_r]

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   [PTR_EDI+4], ebx
  add   PTR_EDI, 8
  cmp   PTR_EDI, dest_end
  jb    merge_loop101

  ; Jump out point if end of dest is reached
  dest_end_exit101:
  jmp   src_save_value101

  ; jump out when src is exceed, but save our current loop position
  src_whole_exit101:
  or   edx, 080000000h
  src_last_exit101:
  or   edx, 040000000h
  src_save_value101:
  mov  [cur_l], ecx

  ; Routine end
  ret

Merge_DestStereo_SrcMono_Src8_Volume_UpFiltered:

  ; adjust fractional
  shl   edx, 16

  ; Load sample data
  xor   eax, eax
  mov   al, byte ptr [PTR_ESI]
  sub   eax, 080h
  shl   eax, 8

  ; rotate filter values
  mov   ecx, [cur_l]
  mov   [cur_l], eax

  ; Merge sample data loop
  ALIGN 16
  merge_loop613:

  ; Upsample the data points
  mov   ebx, edx
  mov   eax, [cur_l]
  shr   ebx, 17
  sub   eax, ecx
  imul  eax, ebx
  sar   eax, 15
  add   eax, ecx

  ; Duplicate the left channel into the right
  mov   ebx, eax

  ; Apply volume
  imul  eax, [scale_left]
  imul  ebx, [scale_right]

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   [PTR_EDI+4], ebx
  add   PTR_EDI, 8
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit613

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  jnc   merge_loop613

  ; Move the source pointer
  inc   PTR_ESI
  cmp   PTR_ESI, src_end
  jae   src_end_exit613

  ; Load sample data
  xor   eax, eax
  mov   al, byte ptr [PTR_ESI]
  sub   eax, 080h
  shl   eax, 8

  ; rotate filter values
  mov   ecx, [cur_l]
  mov   [cur_l], eax

  ; End loop
  jmp   merge_loop613

  ; Jump out point if end of dest is reached
  dest_end_exit613:
  inc   PTR_ESI
  add   edx, [step_fract]
  jc    skip_filter_adjust613

  ; rotate filter values
  mov   [cur_l], ecx

  ; un-increment the source to skip the early source adjustment
  dec   PTR_ESI

  skip_filter_adjust613:

  ; Jump out point if end of src is reached
  src_end_exit613:

  ; adjust fractional
  shr   edx, 16

  ; Routine end
  ret

Merge_DestStereo_SrcMono_Src8_Volume_Filtered ENDP


Merge_DestMono_SrcMono_Src16_Volume_Filtered PROC ; 102

  ; check to see if we have to call the upsampling version
  cmp   [playback_ratio], 10000h
  jle   Merge_DestMono_SrcMono_Src16_Volume_UpFiltered

  ; build average dividers
  mov   ecx, edx
  mov   ebx, [playback_ratio]
  xor   edx, edx
  mov   eax, 65536  ; 100000000f/2048/32
  imul  [scale_left]
  div   ebx
  dec   eax
  mov   [divider_l], eax
  mov   edx, ecx

  ; load initial sample
  mov   ecx, [cur_l]

  ; handle start up loop management
  mov  eax, edx
  and  edx, 03fffffffh
  test eax, 080000000h
  jnz  whole_continue102
  test eax, 040000000h
  jnz  last_continue102

  ; Merge sample data loop
  ALIGN 16
  merge_loop102:
  mov   ebx, 65536
  sub   ebx, edx
  add   edx, [playback_ratio]

  ; weight the initial sample
  imul  ecx, ebx
  sar   ecx, 16
  cmp   edx, 65536*2
  jl    skip_loop102

  ; loop to load all of the full sample points
  whole_loop102:
  cmp   PTR_ESI, [src_end]
  jae   src_whole_exit102
  whole_continue102:

  ; Load sample data
  movsx eax, word ptr [PTR_ESI]
  add   ecx, eax
  sub   edx, 65536
  add   PTR_ESI, 2
  cmp   edx, 65536*2
  jae   whole_loop102

  skip_loop102:
  and   edx, 0ffffh
  last_continue102:
  cmp   PTR_ESI, [src_end]
  jae   src_last_exit102

  ; Load sample data
  movsx eax, word ptr [PTR_ESI]
  add   PTR_ESI, 2
  mov   ebx, ecx
  mov   ecx, eax

  ; weight the final sample
  imul  eax, edx
  sar   eax, 16
  add   eax, ebx
  imul  eax, [divider_l]

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   PTR_EDI, 4
  cmp   PTR_EDI, dest_end
  jb    merge_loop102

  ; Jump out point if end of dest is reached
  dest_end_exit102:
  jmp   src_save_value102

  ; jump out when src is exceed, but save our current loop position
  src_whole_exit102:
  or   edx, 080000000h
  src_last_exit102:
  or   edx, 040000000h
  src_save_value102:
  mov  [cur_l], ecx

  ; Routine end
  ret

Merge_DestMono_SrcMono_Src16_Volume_UpFiltered:

  ; adjust fractional
  shl   edx, 16

  ; Load sample data
  movsx eax, word ptr [PTR_ESI]

  ; rotate filter values
  mov   ecx, [cur_l]
  mov   [cur_l], eax

  ; Merge sample data loop
  ALIGN 16
  merge_loop614:

  ; Upsample the data points
  mov   ebx, edx
  mov   eax, [cur_l]
  shr   ebx, 17
  sub   eax, ecx
  imul  eax, ebx
  sar   eax, 15
  add   eax, ecx

  ; Apply volume
  imul  eax, [scale_left]

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   PTR_EDI, 4
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit614

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  jnc   merge_loop614

  ; Move the source pointer
  add   PTR_ESI, 2
  cmp   PTR_ESI, src_end
  jae   src_end_exit614

  ; Load sample data
  movsx eax, word ptr [PTR_ESI]

  ; rotate filter values
  mov   ecx, [cur_l]
  mov   [cur_l], eax

  ; End loop
  jmp   merge_loop614

  ; Jump out point if end of dest is reached
  dest_end_exit614:
  add   PTR_ESI, 2
  add   edx, [step_fract]
  jc    skip_filter_adjust614

  ; rotate filter values
  mov   [cur_l], ecx

  ; un-increment the source to skip the early source adjustment
  sub   PTR_ESI, 2

  skip_filter_adjust614:

  ; Jump out point if end of src is reached
  src_end_exit614:

  ; adjust fractional
  shr   edx, 16

  ; Routine end
  ret

Merge_DestMono_SrcMono_Src16_Volume_Filtered ENDP


Merge_DestStereo_SrcMono_Src16_Volume_Filtered PROC ; 103

  ; check to see if we have to call the upsampling version
  cmp   [playback_ratio], 10000h
  jle   Merge_DestStereo_SrcMono_Src16_Volume_UpFiltered

  ; build average dividers
  mov   ecx, edx
  mov   ebx, [playback_ratio]
  xor   edx, edx
  mov   eax, 65536  ; 100000000f/2048/32
  imul  [scale_left]
  div   ebx
  dec   eax
  mov   [divider_l], eax
  xor   edx, edx
  mov   eax, 65536  ; 100000000f/2048/32
  imul  [scale_right]
  div   ebx
  dec   eax
  mov   [divider_r], eax
  mov   edx, ecx

  ; load initial sample
  mov   ecx, [cur_l]

  ; handle start up loop management
  mov  eax, edx
  and  edx, 03fffffffh
  test eax, 080000000h
  jnz  whole_continue103
  test eax, 040000000h
  jnz  last_continue103

  ; Merge sample data loop
  ALIGN 16
  merge_loop103:
  mov   ebx, 65536
  sub   ebx, edx
  add   edx, [playback_ratio]

  ; weight the initial sample
  imul  ecx, ebx
  sar   ecx, 16
  cmp   edx, 65536*2
  jl    skip_loop103

  ; loop to load all of the full sample points
  whole_loop103:
  cmp   PTR_ESI, [src_end]
  jae   src_whole_exit103
  whole_continue103:

  ; Load sample data
  movsx eax, word ptr [PTR_ESI]
  add   ecx, eax
  sub   edx, 65536
  add   PTR_ESI, 2
  cmp   edx, 65536*2
  jae   whole_loop103

  skip_loop103:
  and   edx, 0ffffh
  last_continue103:
  cmp   PTR_ESI, [src_end]
  jae   src_last_exit103

  ; Load sample data
  movsx eax, word ptr [PTR_ESI]
  add   PTR_ESI, 2
  mov   ebx, ecx
  mov   ecx, eax

  ; weight the final sample
  imul  eax, edx
  sar   eax, 16
  add   eax, ebx

  ; Duplicate the left channel into the right
  mov   ebx, eax
  imul  eax, [divider_l]
  imul  ebx, [divider_r]

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   [PTR_EDI+4], ebx
  add   PTR_EDI, 8
  cmp   PTR_EDI, dest_end
  jb    merge_loop103

  ; Jump out point if end of dest is reached
  dest_end_exit103:
  jmp   src_save_value103

  ; jump out when src is exceed, but save our current loop position
  src_whole_exit103:
  or   edx, 080000000h
  src_last_exit103:
  or   edx, 040000000h
  src_save_value103:
  mov  [cur_l], ecx

  ; Routine end
  ret

Merge_DestStereo_SrcMono_Src16_Volume_UpFiltered:

  ; adjust fractional
  shl   edx, 16

  ; Load sample data
  movsx eax, word ptr [PTR_ESI]

  ; rotate filter values
  mov   ecx, [cur_l]
  mov   [cur_l], eax

  ; Merge sample data loop
  ALIGN 16
  merge_loop615:

  ; Upsample the data points
  mov   ebx, edx
  mov   eax, [cur_l]
  shr   ebx, 17
  sub   eax, ecx
  imul  eax, ebx
  sar   eax, 15
  add   eax, ecx

  ; Duplicate the left channel into the right
  mov   ebx, eax

  ; Apply volume
  imul  eax, [scale_left]
  imul  ebx, [scale_right]

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   [PTR_EDI+4], ebx
  add   PTR_EDI, 8
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit615

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  jnc   merge_loop615

  ; Move the source pointer
  add   PTR_ESI, 2
  cmp   PTR_ESI, src_end
  jae   src_end_exit615

  ; Load sample data
  movsx eax, word ptr [PTR_ESI]

  ; rotate filter values
  mov   ecx, [cur_l]
  mov   [cur_l], eax

  ; End loop
  jmp   merge_loop615

  ; Jump out point if end of dest is reached
  dest_end_exit615:
  add   PTR_ESI, 2
  add   edx, [step_fract]
  jc    skip_filter_adjust615

  ; rotate filter values
  mov   [cur_l], ecx

  ; un-increment the source to skip the early source adjustment
  sub   PTR_ESI, 2

  skip_filter_adjust615:

  ; Jump out point if end of src is reached
  src_end_exit615:

  ; adjust fractional
  shr   edx, 16

  ; Routine end
  ret

Merge_DestStereo_SrcMono_Src16_Volume_Filtered ENDP


Merge_DestMono_SrcStereo_Src8_Volume_Resample PROC ; 104

  ; adjust fractional
  shl   edx, 16

  ; Load sample data
  xor   ebx, ebx
  xor   eax, eax
  mov   ax, word ptr [PTR_ESI]
  mov   bl, ah
  and   eax, 0ffh
  sub   ebx, 080h
  sub   eax, 080h
  shl   ebx, 8
  shl   eax, 8

  ; Apply volume
  imul  eax, [scale_left]
  imul  ebx, [scale_right]

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; Merge sample data loop
  ALIGN 16
  merge_loop104:

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   PTR_EDI, 4
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit104

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  sbb   PTR_EAX, PTR_EAX
  add   PTR_ESI, [STEPTBL_PTR][PTR_SIZE+PTR_EAX*PTR_SIZE]
  cmp   PTR_ESI, src_end
  jae   src_end_exit104

  ; Load sample data
  xor   ebx, ebx
  xor   eax, eax
  mov   ax, word ptr [PTR_ESI]
  mov   bl, ah
  and   eax, 0ffh
  sub   ebx, 080h
  sub   eax, 080h
  shl   ebx, 8
  shl   eax, 8

  ; Apply volume
  imul  eax, [scale_left]
  imul  ebx, [scale_right]

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; End loop
  jmp   merge_loop104

  ; Jump out point if end of dest is reached
  dest_end_exit104:

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  sbb   PTR_EAX, PTR_EAX
  add   PTR_ESI, [STEPTBL_PTR][PTR_SIZE+PTR_EAX*PTR_SIZE]

  ; Jump out point if end of src is reached
  src_end_exit104:

  ; adjust fractional
  shr   edx, 16

  ; Routine end
  ret

Merge_DestMono_SrcStereo_Src8_Volume_Resample ENDP


Merge_DestStereo_SrcStereo_Src8_Volume_Resample PROC ; 105

  ; adjust fractional
  shl   edx, 16

  ; Load sample data
  xor   ebx, ebx
  xor   eax, eax
  mov   ax, word ptr [PTR_ESI]
  mov   bl, ah
  and   eax, 0ffh
  sub   ebx, 080h
  sub   eax, 080h
  shl   ebx, 8
  shl   eax, 8

  ; Apply volume
  imul  eax, [scale_left]
  imul  ebx, [scale_right]

  ; Merge sample data loop
  ALIGN 16
  merge_loop105:

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   [PTR_EDI+4], ebx
  add   PTR_EDI, 8
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit105

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  sbb   PTR_EAX, PTR_EAX
  add   PTR_ESI, [STEPTBL_PTR][PTR_SIZE+PTR_EAX*PTR_SIZE]
  cmp   PTR_ESI, src_end
  jae   src_end_exit105

  ; Load sample data
  xor   ebx, ebx
  xor   eax, eax
  mov   ax, word ptr [PTR_ESI]
  mov   bl, ah
  and   eax, 0ffh
  sub   ebx, 080h
  sub   eax, 080h
  shl   ebx, 8
  shl   eax, 8

  ; Apply volume
  imul  eax, [scale_left]
  imul  ebx, [scale_right]

  ; End loop
  jmp   merge_loop105

  ; Jump out point if end of dest is reached
  dest_end_exit105:

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  sbb   PTR_EAX, PTR_EAX
  add   PTR_ESI, [STEPTBL_PTR][PTR_SIZE+PTR_EAX*PTR_SIZE]

  ; Jump out point if end of src is reached
  src_end_exit105:

  ; adjust fractional
  shr   edx, 16

  ; Routine end
  ret

Merge_DestStereo_SrcStereo_Src8_Volume_Resample ENDP


Merge_DestMono_SrcStereo_Src16_Volume_Resample PROC ; 106

  ; adjust fractional
  shl   edx, 16

  ; Load sample data
  mov   ebx, dword ptr [PTR_ESI]
  movsx eax, bx
  sar   ebx, 16

  ; Apply volume
  imul  eax, [scale_left]
  imul  ebx, [scale_right]

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; Merge sample data loop
  ALIGN 16
  merge_loop106:

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   PTR_EDI, 4
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit106

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  sbb   PTR_EAX, PTR_EAX
  add   PTR_ESI, [STEPTBL_PTR][PTR_SIZE+PTR_EAX*PTR_SIZE]
  cmp   PTR_ESI, src_end
  jae   src_end_exit106

  ; Load sample data
  mov   ebx, dword ptr [PTR_ESI]
  movsx eax, bx
  sar   ebx, 16

  ; Apply volume
  imul  eax, [scale_left]
  imul  ebx, [scale_right]

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; End loop
  jmp   merge_loop106

  ; Jump out point if end of dest is reached
  dest_end_exit106:

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  sbb   PTR_EAX, PTR_EAX
  add   PTR_ESI, [STEPTBL_PTR][PTR_SIZE+PTR_EAX*PTR_SIZE]

  ; Jump out point if end of src is reached
  src_end_exit106:

  ; adjust fractional
  shr   edx, 16

  ; Routine end
  ret

Merge_DestMono_SrcStereo_Src16_Volume_Resample ENDP


Merge_DestStereo_SrcStereo_Src16_Volume_Resample PROC ; 107

  ; adjust fractional
  shl   edx, 16

  ; Load sample data
  mov   ebx, dword ptr [PTR_ESI]
  movsx eax, bx
  sar   ebx, 16

  ; Apply volume
  imul  eax, [scale_left]
  imul  ebx, [scale_right]

  ; Merge sample data loop
  ALIGN 16
  merge_loop107:

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   [PTR_EDI+4], ebx
  add   PTR_EDI, 8
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit107

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  sbb   PTR_EAX, PTR_EAX
  add   PTR_ESI, [STEPTBL_PTR][PTR_SIZE+PTR_EAX*PTR_SIZE]
  cmp   PTR_ESI, src_end
  jae   src_end_exit107

  ; Load sample data
  mov   ebx, dword ptr [PTR_ESI]
  movsx eax, bx
  sar   ebx, 16

  ; Apply volume
  imul  eax, [scale_left]
  imul  ebx, [scale_right]

  ; End loop
  jmp   merge_loop107

  ; Jump out point if end of dest is reached
  dest_end_exit107:

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  sbb   PTR_EAX, PTR_EAX
  add   PTR_ESI, [STEPTBL_PTR][PTR_SIZE+PTR_EAX*PTR_SIZE]

  ; Jump out point if end of src is reached
  src_end_exit107:

  ; adjust fractional
  shr   edx, 16

  ; Routine end
  ret

Merge_DestStereo_SrcStereo_Src16_Volume_Resample ENDP


Merge_DestMono_SrcStereo_Src8_Volume_Filtered PROC ; 108

  ; check to see if we have to call the upsampling version
  cmp   [playback_ratio], 10000h
  jle   Merge_DestMono_SrcStereo_Src8_Volume_UpFiltered

  ; build average dividers
  mov   ecx, edx
  mov   ebx, [playback_ratio]
  xor   edx, edx
  mov   eax, 65536  ; 100000000f/2048/32
  imul  [scale_left]
  div   ebx
  dec   eax
  mov   [divider_l], eax
  xor   edx, edx
  mov   eax, 65536  ; 100000000f/2048/32
  imul  [scale_right]
  div   ebx
  dec   eax
  mov   [divider_r], eax
  mov   edx, ecx

  ; load initial sample
  mov   ecx, [cur_l]

  ; Save registers
  push  PTR_EBP

  ; handle start up loop management
  mov  eax, edx
  and  edx, 03fffffffh
  test eax, 080000000h
  jnz  whole_continue108
  test eax, 040000000h
  jnz  last_continue108

  ; Merge sample data loop
  ALIGN 16
  merge_loop108:
  mov   ebx, 65536
  sub   ebx, edx
  add   edx, [playback_ratio]

  ; weight the initial sample
  mov   eax, [cur_r]
  imul  ecx, ebx
  imul  eax, ebx
  sar   ecx, 16
  sar   eax, 16
  cmp   edx, 65536*2
  mov   [cur_r], eax
  jl    skip_loop108

  ; loop to load all of the full sample points
  whole_loop108:
  cmp   PTR_ESI, [src_end]
  jae   src_whole_exit108
  whole_continue108:

  ; Load sample data
  xor   ebx, ebx
  xor   eax, eax
  mov   ax, word ptr [PTR_ESI]
  mov   bl, ah
  and   eax, 0ffh
  sub   ebx, 080h
  sub   eax, 080h
  shl   ebx, 8
  shl   eax, 8
  add   ecx, eax
  add   [cur_r], ebx
  sub   edx, 65536
  add   PTR_ESI, 2
  cmp   edx, 65536*2
  jae   whole_loop108

  skip_loop108:
  and   edx, 0ffffh
  cmp   PTR_ESI, [src_end]
  jae   src_last_exit108
  last_continue108:

  ; Load sample data
  xor   ebx, ebx
  xor   eax, eax
  mov   ax, word ptr [PTR_ESI]
  mov   bl, ah
  and   eax, 0ffh
  sub   ebx, 080h
  sub   eax, 080h
  shl   ebx, 8
  shl   eax, 8
  add   PTR_ESI, 2
  mov   ebp, ecx
  mov   ecx, eax

  ; weight the final sample
  imul  eax, edx
  sar   eax, 16
  add   eax, ebp
  mov   ebp, [cur_r]
  mov   [cur_r], ebx
  imul  ebx, edx
  sar   ebx, 16
  add   ebx, ebp
  imul  eax, [divider_l]
  imul  ebx, [divider_r]

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   PTR_EDI, 4
  cmp   PTR_EDI, dest_end
  jb    merge_loop108

  ; Jump out point if end of dest is reached
  dest_end_exit108:
  jmp   src_save_value108

  ; jump out when src is exceed, but save our current loop position
  src_whole_exit108:
  or   edx, 080000000h
  src_last_exit108:
  or   edx, 040000000h
  src_save_value108:
  mov  [cur_l], ecx

  ; Restore registers
  pop   PTR_EBP

  ; Routine end
  ret

Merge_DestMono_SrcStereo_Src8_Volume_UpFiltered:

  ; adjust fractional
  shl   edx, 16

  ; Save registers
  push  PTR_EBP

  ; Load sample data
  xor   ebx, ebx
  xor   eax, eax
  mov   ax, word ptr [PTR_ESI]
  mov   bl, ah
  and   eax, 0ffh
  sub   ebx, 080h
  sub   eax, 080h
  shl   ebx, 8
  shl   eax, 8

  ; rotate filter values
  mov   ecx, [cur_l]
  mov   [cur_l], eax
  mov   eax, [cur_r]
  mov   [cur_r], ebx
  mov   [cur_r2], eax

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; Merge sample data loop
  ALIGN 16
  merge_loop620:

  ; Upsample the data points
  mov   ebp, edx
  mov   eax, [cur_l]
  mov   ebx, [cur_r]
  shr   ebp, 17
  sub   eax, ecx
  sub   ebx, [cur_r2]
  imul  eax, ebp
  imul  ebx, ebp
  sar   eax, 15
  sar   ebx, 15
  add   eax, ecx
  add   ebx, [cur_r2]

  ; Apply volume
  imul  eax, [scale_left]
  imul  ebx, [scale_right]

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   PTR_EDI, 4
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit620

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  jnc   merge_loop620

  ; Move the source pointer
  add   PTR_ESI, 2
  cmp   PTR_ESI, src_end
  jae   src_end_exit620

  ; Load sample data
  xor   ebx, ebx
  xor   eax, eax
  mov   ax, word ptr [PTR_ESI]
  mov   bl, ah
  and   eax, 0ffh
  sub   ebx, 080h
  sub   eax, 080h
  shl   ebx, 8
  shl   eax, 8

  ; rotate filter values
  mov   ecx, [cur_l]
  mov   [cur_l], eax
  mov   eax, [cur_r]
  mov   [cur_r], ebx
  mov   [cur_r2], eax

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; End loop
  jmp   merge_loop620

  ; Jump out point if end of dest is reached
  dest_end_exit620:
  add   PTR_ESI, 2
  add   edx, [step_fract]
  jc    skip_filter_adjust620

  ; rotate filter values
  mov   [cur_l], ecx
  mov   ecx, [cur_r2]
  mov   [cur_r], ecx

  ; un-increment the source to skip the early source adjustment
  sub   PTR_ESI, 2

  skip_filter_adjust620:

  ; Jump out point if end of src is reached
  src_end_exit620:

  ; Restore registers
  pop   PTR_EBP

  ; adjust fractional
  shr   edx, 16

  ; Routine end
  ret

Merge_DestMono_SrcStereo_Src8_Volume_Filtered ENDP


Merge_DestStereo_SrcStereo_Src8_Volume_Filtered PROC ; 109

  ; check to see if we have to call the upsampling version
  cmp   [playback_ratio], 10000h
  jle   Merge_DestStereo_SrcStereo_Src8_Volume_UpFiltered

  ; build average dividers
  mov   ecx, edx
  mov   ebx, [playback_ratio]
  xor   edx, edx
  mov   eax, 65536  ; 100000000f/2048/32
  imul  [scale_left]
  div   ebx
  dec   eax
  mov   [divider_l], eax
  xor   edx, edx
  mov   eax, 65536  ; 100000000f/2048/32
  imul  [scale_right]
  div   ebx
  dec   eax
  mov   [divider_r], eax
  mov   edx, ecx

  ; load initial sample
  mov   ecx, [cur_l]

  ; Save registers
  push  PTR_EBP

  ; handle start up loop management
  mov  eax, edx
  and  edx, 03fffffffh
  test eax, 080000000h
  jnz  whole_continue109
  test eax, 040000000h
  jnz  last_continue109

  ; Merge sample data loop
  ALIGN 16
  merge_loop109:
  mov   ebx, 65536
  sub   ebx, edx
  add   edx, [playback_ratio]

  ; weight the initial sample
  mov   eax, [cur_r]
  imul  ecx, ebx
  imul  eax, ebx
  sar   ecx, 16
  sar   eax, 16
  cmp   edx, 65536*2
  mov   [cur_r], eax
  jl    skip_loop109

  ; loop to load all of the full sample points
  whole_loop109:
  cmp   PTR_ESI, [src_end]
  jae   src_whole_exit109
  whole_continue109:

  ; Load sample data
  xor   ebx, ebx
  xor   eax, eax
  mov   ax, word ptr [PTR_ESI]
  mov   bl, ah
  and   eax, 0ffh
  sub   ebx, 080h
  sub   eax, 080h
  shl   ebx, 8
  shl   eax, 8
  add   ecx, eax
  add   [cur_r], ebx
  sub   edx, 65536
  add   PTR_ESI, 2
  cmp   edx, 65536*2
  jae   whole_loop109

  skip_loop109:
  and   edx, 0ffffh
  cmp   PTR_ESI, [src_end]
  jae   src_last_exit109
  last_continue109:

  ; Load sample data
  xor   ebx, ebx
  xor   eax, eax
  mov   ax, word ptr [PTR_ESI]
  mov   bl, ah
  and   eax, 0ffh
  sub   ebx, 080h
  sub   eax, 080h
  shl   ebx, 8
  shl   eax, 8
  add   PTR_ESI, 2
  mov   ebp, ecx
  mov   ecx, eax

  ; weight the final sample
  imul  eax, edx
  sar   eax, 16
  add   eax, ebp
  mov   ebp, [cur_r]
  mov   [cur_r], ebx
  imul  ebx, edx
  sar   ebx, 16
  add   ebx, ebp
  imul  eax, [divider_l]
  imul  ebx, [divider_r]

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   [PTR_EDI+4], ebx
  add   PTR_EDI, 8
  cmp   PTR_EDI, dest_end
  jb    merge_loop109

  ; Jump out point if end of dest is reached
  dest_end_exit109:
  jmp   src_save_value109

  ; jump out when src is exceed, but save our current loop position
  src_whole_exit109:
  or   edx, 080000000h
  src_last_exit109:
  or   edx, 040000000h
  src_save_value109:
  mov  [cur_l], ecx

  ; Restore registers
  pop   PTR_EBP

  ; Routine end
  ret

Merge_DestStereo_SrcStereo_Src8_Volume_UpFiltered:

  ; adjust fractional
  shl   edx, 16

  ; Save registers
  push  PTR_EBP

  ; Load sample data
  xor   ebx, ebx
  xor   eax, eax
  mov   ax, word ptr [PTR_ESI]
  mov   bl, ah
  and   eax, 0ffh
  sub   ebx, 080h
  sub   eax, 080h
  shl   ebx, 8
  shl   eax, 8

  ; rotate filter values
  mov   ecx, [cur_l]
  mov   [cur_l], eax
  mov   eax, [cur_r]
  mov   [cur_r], ebx
  mov   [cur_r2], eax

  ; Merge sample data loop
  ALIGN 16
  merge_loop621:

  ; Upsample the data points
  mov   ebp, edx
  mov   eax, [cur_l]
  mov   ebx, [cur_r]
  shr   ebp, 17
  sub   eax, ecx
  sub   ebx, [cur_r2]
  imul  eax, ebp
  imul  ebx, ebp
  sar   eax, 15
  sar   ebx, 15
  add   eax, ecx
  add   ebx, [cur_r2]

  ; Apply volume
  imul  eax, [scale_left]
  imul  ebx, [scale_right]

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   [PTR_EDI+4], ebx
  add   PTR_EDI, 8
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit621

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  jnc   merge_loop621

  ; Move the source pointer
  add   PTR_ESI, 2
  cmp   PTR_ESI, src_end
  jae   src_end_exit621

  ; Load sample data
  xor   ebx, ebx
  xor   eax, eax
  mov   ax, word ptr [PTR_ESI]
  mov   bl, ah
  and   eax, 0ffh
  sub   ebx, 080h
  sub   eax, 080h
  shl   ebx, 8
  shl   eax, 8

  ; rotate filter values
  mov   ecx, [cur_l]
  mov   [cur_l], eax
  mov   eax, [cur_r]
  mov   [cur_r], ebx
  mov   [cur_r2], eax

  ; End loop
  jmp   merge_loop621

  ; Jump out point if end of dest is reached
  dest_end_exit621:
  add   PTR_ESI, 2
  add   edx, [step_fract]
  jc    skip_filter_adjust621

  ; rotate filter values
  mov   [cur_l], ecx
  mov   ecx, [cur_r2]
  mov   [cur_r], ecx

  ; un-increment the source to skip the early source adjustment
  sub   PTR_ESI, 2

  skip_filter_adjust621:

  ; Jump out point if end of src is reached
  src_end_exit621:

  ; Restore registers
  pop   PTR_EBP

  ; adjust fractional
  shr   edx, 16

  ; Routine end
  ret

Merge_DestStereo_SrcStereo_Src8_Volume_Filtered ENDP


Merge_DestMono_SrcStereo_Src16_Volume_Filtered PROC ; 110

  ; check to see if we have to call the upsampling version
  cmp   [playback_ratio], 10000h
  jle   Merge_DestMono_SrcStereo_Src16_Volume_UpFiltered

  ; build average dividers
  mov   ecx, edx
  mov   ebx, [playback_ratio]
  xor   edx, edx
  mov   eax, 65536  ; 100000000f/2048/32
  imul  [scale_left]
  div   ebx
  dec   eax
  mov   [divider_l], eax
  xor   edx, edx
  mov   eax, 65536  ; 100000000f/2048/32
  imul  [scale_right]
  div   ebx
  dec   eax
  mov   [divider_r], eax
  mov   edx, ecx

  ; load initial sample
  mov   ecx, [cur_l]

  ; Save registers
  push  PTR_EBP

  ; handle start up loop management
  mov  eax, edx
  and  edx, 03fffffffh
  test eax, 080000000h
  jnz  whole_continue110
  test eax, 040000000h
  jnz  last_continue110

  ; Merge sample data loop
  ALIGN 16
  merge_loop110:
  mov   ebx, 65536
  sub   ebx, edx
  add   edx, [playback_ratio]

  ; weight the initial sample
  mov   eax, [cur_r]
  imul  ecx, ebx
  imul  eax, ebx
  sar   ecx, 16
  sar   eax, 16
  cmp   edx, 65536*2
  mov   [cur_r], eax
  jl    skip_loop110

  ; loop to load all of the full sample points
  whole_loop110:
  cmp   PTR_ESI, [src_end]
  jae   src_whole_exit110
  whole_continue110:

  ; Load sample data
  mov   ebx, dword ptr [PTR_ESI]
  movsx eax, bx
  sar   ebx, 16
  add   ecx, eax
  add   [cur_r], ebx
  sub   edx, 65536
  add   PTR_ESI, 4
  cmp   edx, 65536*2
  jae   whole_loop110

  skip_loop110:
  and   edx, 0ffffh
  cmp   PTR_ESI, [src_end]
  jae   src_last_exit110
  last_continue110:

  ; Load sample data
  mov   ebx, dword ptr [PTR_ESI]
  movsx eax, bx
  sar   ebx, 16
  add   PTR_ESI, 4
  mov   ebp, ecx
  mov   ecx, eax

  ; weight the final sample
  imul  eax, edx
  sar   eax, 16
  add   eax, ebp
  mov   ebp, [cur_r]
  mov   [cur_r], ebx
  imul  ebx, edx
  sar   ebx, 16
  add   ebx, ebp
  imul  eax, [divider_l]
  imul  ebx, [divider_r]

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   PTR_EDI, 4
  cmp   PTR_EDI, dest_end
  jb    merge_loop110

  ; Jump out point if end of dest is reached
  dest_end_exit110:
  jmp   src_save_value110

  ; jump out when src is exceed, but save our current loop position
  src_whole_exit110:
  or   edx, 080000000h
  src_last_exit110:
  or   edx, 040000000h
  src_save_value110:
  mov  [cur_l], ecx

  ; Restore registers
  pop   PTR_EBP

  ; Routine end
  ret

Merge_DestMono_SrcStereo_Src16_Volume_UpFiltered:

  ; adjust fractional
  shl   edx, 16

  ; Save registers
  push  PTR_EBP

  ; Load sample data
  mov   ebx, dword ptr [PTR_ESI]
  movsx eax, bx
  sar   ebx, 16

  ; rotate filter values
  mov   ecx, [cur_l]
  mov   [cur_l], eax
  mov   eax, [cur_r]
  mov   [cur_r], ebx
  mov   [cur_r2], eax

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; Merge sample data loop
  ALIGN 16
  merge_loop622:

  ; Upsample the data points
  mov   ebp, edx
  mov   eax, [cur_l]
  mov   ebx, [cur_r]
  shr   ebp, 17
  sub   eax, ecx
  sub   ebx, [cur_r2]
  imul  eax, ebp
  imul  ebx, ebp
  sar   eax, 15
  sar   ebx, 15
  add   eax, ecx
  add   ebx, [cur_r2]

  ; Apply volume
  imul  eax, [scale_left]
  imul  ebx, [scale_right]

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   PTR_EDI, 4
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit622

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  jnc   merge_loop622

  ; Move the source pointer
  add   PTR_ESI, 4
  cmp   PTR_ESI, src_end
  jae   src_end_exit622

  ; Load sample data
  mov   ebx, dword ptr [PTR_ESI]
  movsx eax, bx
  sar   ebx, 16

  ; rotate filter values
  mov   ecx, [cur_l]
  mov   [cur_l], eax
  mov   eax, [cur_r]
  mov   [cur_r], ebx
  mov   [cur_r2], eax

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; End loop
  jmp   merge_loop622

  ; Jump out point if end of dest is reached
  dest_end_exit622:
  add   PTR_ESI, 4
  add   edx, [step_fract]
  jc    skip_filter_adjust622

  ; rotate filter values
  mov   [cur_l], ecx
  mov   ecx, [cur_r2]
  mov   [cur_r], ecx

  ; un-increment the source to skip the early source adjustment
  sub   PTR_ESI, 4

  skip_filter_adjust622:

  ; Jump out point if end of src is reached
  src_end_exit622:

  ; Restore registers
  pop   PTR_EBP

  ; adjust fractional
  shr   edx, 16

  ; Routine end
  ret

Merge_DestMono_SrcStereo_Src16_Volume_Filtered ENDP


Merge_DestStereo_SrcStereo_Src16_Volume_Filtered PROC ; 111

  ; check to see if we have to call the upsampling version
  cmp   [playback_ratio], 10000h
  jle   Merge_DestStereo_SrcStereo_Src16_Volume_UpFiltered

  ; build average dividers
  mov   ecx, edx
  mov   ebx, [playback_ratio]
  xor   edx, edx
  mov   eax, 65536  ; 100000000f/2048/32
  imul  [scale_left]
  div   ebx
  dec   eax
  mov   [divider_l], eax
  xor   edx, edx
  mov   eax, 65536  ; 100000000f/2048/32
  imul  [scale_right]
  div   ebx
  dec   eax
  mov   [divider_r], eax
  mov   edx, ecx

  ; load initial sample
  mov   ecx, [cur_l]

  ; Save registers
  push  PTR_EBP

  ; handle start up loop management
  mov  eax, edx
  and  edx, 03fffffffh
  test eax, 080000000h
  jnz  whole_continue111
  test eax, 040000000h
  jnz  last_continue111

  ; Merge sample data loop
  ALIGN 16
  merge_loop111:
  mov   ebx, 65536
  sub   ebx, edx
  add   edx, [playback_ratio]

  ; weight the initial sample
  mov   eax, [cur_r]
  imul  ecx, ebx
  imul  eax, ebx
  sar   ecx, 16
  sar   eax, 16
  cmp   edx, 65536*2
  mov   [cur_r], eax
  jl    skip_loop111

  ; loop to load all of the full sample points
  whole_loop111:
  cmp   PTR_ESI, [src_end]
  jae   src_whole_exit111
  whole_continue111:

  ; Load sample data
  mov   ebx, dword ptr [PTR_ESI]
  movsx eax, bx
  sar   ebx, 16
  add   ecx, eax
  add   [cur_r], ebx
  sub   edx, 65536
  add   PTR_ESI, 4
  cmp   edx, 65536*2
  jae   whole_loop111

  skip_loop111:
  and   edx, 0ffffh
  cmp   PTR_ESI, [src_end]
  jae   src_last_exit111
  last_continue111:

  ; Load sample data
  mov   ebx, dword ptr [PTR_ESI]
  movsx eax, bx
  sar   ebx, 16
  add   PTR_ESI, 4
  mov   ebp, ecx
  mov   ecx, eax

  ; weight the final sample
  imul  eax, edx
  sar   eax, 16
  add   eax, ebp
  mov   ebp, [cur_r]
  mov   [cur_r], ebx
  imul  ebx, edx
  sar   ebx, 16
  add   ebx, ebp
  imul  eax, [divider_l]
  imul  ebx, [divider_r]

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   [PTR_EDI+4], ebx
  add   PTR_EDI, 8
  cmp   PTR_EDI, dest_end
  jb    merge_loop111

  ; Jump out point if end of dest is reached
  dest_end_exit111:
  jmp   src_save_value111

  ; jump out when src is exceed, but save our current loop position
  src_whole_exit111:
  or   edx, 080000000h
  src_last_exit111:
  or   edx, 040000000h
  src_save_value111:
  mov  [cur_l], ecx

  ; Restore registers
  pop   PTR_EBP

  ; Routine end
  ret

Merge_DestStereo_SrcStereo_Src16_Volume_UpFiltered:

  ; adjust fractional
  shl   edx, 16

  ; Save registers
  push  PTR_EBP

  ; Load sample data
  mov   ebx, dword ptr [PTR_ESI]
  movsx eax, bx
  sar   ebx, 16

  ; rotate filter values
  mov   ecx, [cur_l]
  mov   [cur_l], eax
  mov   eax, [cur_r]
  mov   [cur_r], ebx
  mov   [cur_r2], eax

  ; Merge sample data loop
  ALIGN 16
  merge_loop623:

  ; Upsample the data points
  mov   ebp, edx
  mov   eax, [cur_l]
  mov   ebx, [cur_r]
  shr   ebp, 17
  sub   eax, ecx
  sub   ebx, [cur_r2]
  imul  eax, ebp
  imul  ebx, ebp
  sar   eax, 15
  sar   ebx, 15
  add   eax, ecx
  add   ebx, [cur_r2]

  ; Apply volume
  imul  eax, [scale_left]
  imul  ebx, [scale_right]

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   [PTR_EDI+4], ebx
  add   PTR_EDI, 8
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit623

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  jnc   merge_loop623

  ; Move the source pointer
  add   PTR_ESI, 4
  cmp   PTR_ESI, src_end
  jae   src_end_exit623

  ; Load sample data
  mov   ebx, dword ptr [PTR_ESI]
  movsx eax, bx
  sar   ebx, 16

  ; rotate filter values
  mov   ecx, [cur_l]
  mov   [cur_l], eax
  mov   eax, [cur_r]
  mov   [cur_r], ebx
  mov   [cur_r2], eax

  ; End loop
  jmp   merge_loop623

  ; Jump out point if end of dest is reached
  dest_end_exit623:
  add   PTR_ESI, 4
  add   edx, [step_fract]
  jc    skip_filter_adjust623

  ; rotate filter values
  mov   [cur_l], ecx
  mov   ecx, [cur_r2]
  mov   [cur_r], ecx

  ; un-increment the source to skip the early source adjustment
  sub   PTR_ESI, 4

  skip_filter_adjust623:

  ; Jump out point if end of src is reached
  src_end_exit623:

  ; Restore registers
  pop   PTR_EBP

  ; adjust fractional
  shr   edx, 16

  ; Routine end
  ret

Merge_DestStereo_SrcStereo_Src16_Volume_Filtered ENDP


Merge_DestMono_SrcFlipped_Src8_Volume_Resample PROC ; 120

  ; adjust fractional
  shl   edx, 16

  ; Load sample data
  xor   eax, eax
  xor   ebx, ebx
  mov   bx, word ptr [PTR_ESI]
  mov   al, bh
  and   ebx, 0ffh
  sub   eax, 080h
  sub   ebx, 080h
  shl   eax, 8
  shl   ebx, 8

  ; Apply volume
  imul  eax, [scale_left]
  imul  ebx, [scale_right]

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; Merge sample data loop
  ALIGN 16
  merge_loop120:

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   PTR_EDI, 4
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit120

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  sbb   PTR_EAX, PTR_EAX
  add   PTR_ESI, [STEPTBL_PTR][PTR_SIZE+PTR_EAX*PTR_SIZE]
  cmp   PTR_ESI, src_end
  jae   src_end_exit120

  ; Load sample data
  xor   eax, eax
  xor   ebx, ebx
  mov   bx, word ptr [PTR_ESI]
  mov   al, bh
  and   ebx, 0ffh
  sub   eax, 080h
  sub   ebx, 080h
  shl   eax, 8
  shl   ebx, 8

  ; Apply volume
  imul  eax, [scale_left]
  imul  ebx, [scale_right]

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; End loop
  jmp   merge_loop120

  ; Jump out point if end of dest is reached
  dest_end_exit120:

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  sbb   PTR_EAX, PTR_EAX
  add   PTR_ESI, [STEPTBL_PTR][PTR_SIZE+PTR_EAX*PTR_SIZE]

  ; Jump out point if end of src is reached
  src_end_exit120:

  ; adjust fractional
  shr   edx, 16

  ; Routine end
  ret

Merge_DestMono_SrcFlipped_Src8_Volume_Resample ENDP


Merge_DestStereo_SrcFlipped_Src8_Volume_Resample PROC ; 121

  ; adjust fractional
  shl   edx, 16

  ; Load sample data
  xor   eax, eax
  xor   ebx, ebx
  mov   bx, word ptr [PTR_ESI]
  mov   al, bh
  and   ebx, 0ffh
  sub   eax, 080h
  sub   ebx, 080h
  shl   eax, 8
  shl   ebx, 8

  ; Apply volume
  imul  eax, [scale_left]
  imul  ebx, [scale_right]

  ; Merge sample data loop
  ALIGN 16
  merge_loop121:

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   [PTR_EDI+4], ebx
  add   PTR_EDI, 8
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit121

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  sbb   PTR_EAX, PTR_EAX
  add   PTR_ESI, [STEPTBL_PTR][PTR_SIZE+PTR_EAX*PTR_SIZE]
  cmp   PTR_ESI, src_end
  jae   src_end_exit121

  ; Load sample data
  xor   eax, eax
  xor   ebx, ebx
  mov   bx, word ptr [PTR_ESI]
  mov   al, bh
  and   ebx, 0ffh
  sub   eax, 080h
  sub   ebx, 080h
  shl   eax, 8
  shl   ebx, 8

  ; Apply volume
  imul  eax, [scale_left]
  imul  ebx, [scale_right]

  ; End loop
  jmp   merge_loop121

  ; Jump out point if end of dest is reached
  dest_end_exit121:

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  sbb   PTR_EAX, PTR_EAX
  add   PTR_ESI, [STEPTBL_PTR][PTR_SIZE+PTR_EAX*PTR_SIZE]

  ; Jump out point if end of src is reached
  src_end_exit121:

  ; adjust fractional
  shr   edx, 16

  ; Routine end
  ret

Merge_DestStereo_SrcFlipped_Src8_Volume_Resample ENDP


Merge_DestMono_SrcFlipped_Src16_Volume_Resample PROC ; 122

  ; adjust fractional
  shl   edx, 16

  ; Load sample data
  mov   eax, dword ptr [PTR_ESI]
  movsx ebx, ax
  sar   eax, 16

  ; Apply volume
  imul  eax, [scale_left]
  imul  ebx, [scale_right]

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; Merge sample data loop
  ALIGN 16
  merge_loop122:

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   PTR_EDI, 4
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit122

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  sbb   PTR_EAX, PTR_EAX
  add   PTR_ESI, [STEPTBL_PTR][PTR_SIZE+PTR_EAX*PTR_SIZE]
  cmp   PTR_ESI, src_end
  jae   src_end_exit122

  ; Load sample data
  mov   eax, dword ptr [PTR_ESI]
  movsx ebx, ax
  sar   eax, 16

  ; Apply volume
  imul  eax, [scale_left]
  imul  ebx, [scale_right]

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; End loop
  jmp   merge_loop122

  ; Jump out point if end of dest is reached
  dest_end_exit122:

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  sbb   PTR_EAX, PTR_EAX
  add   PTR_ESI, [STEPTBL_PTR][PTR_SIZE+PTR_EAX*PTR_SIZE]

  ; Jump out point if end of src is reached
  src_end_exit122:

  ; adjust fractional
  shr   edx, 16

  ; Routine end
  ret

Merge_DestMono_SrcFlipped_Src16_Volume_Resample ENDP


Merge_DestStereo_SrcFlipped_Src16_Volume_Resample PROC ; 123

  ; adjust fractional
  shl   edx, 16

  ; Load sample data
  mov   eax, dword ptr [PTR_ESI]
  movsx ebx, ax
  sar   eax, 16

  ; Apply volume
  imul  eax, [scale_left]
  imul  ebx, [scale_right]

  ; Merge sample data loop
  ALIGN 16
  merge_loop123:

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   [PTR_EDI+4], ebx
  add   PTR_EDI, 8
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit123

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  sbb   PTR_EAX, PTR_EAX
  add   PTR_ESI, [STEPTBL_PTR][PTR_SIZE+PTR_EAX*PTR_SIZE]
  cmp   PTR_ESI, src_end
  jae   src_end_exit123

  ; Load sample data
  mov   eax, dword ptr [PTR_ESI]
  movsx ebx, ax
  sar   eax, 16

  ; Apply volume
  imul  eax, [scale_left]
  imul  ebx, [scale_right]

  ; End loop
  jmp   merge_loop123

  ; Jump out point if end of dest is reached
  dest_end_exit123:

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  sbb   PTR_EAX, PTR_EAX
  add   PTR_ESI, [STEPTBL_PTR][PTR_SIZE+PTR_EAX*PTR_SIZE]

  ; Jump out point if end of src is reached
  src_end_exit123:

  ; adjust fractional
  shr   edx, 16

  ; Routine end
  ret

Merge_DestStereo_SrcFlipped_Src16_Volume_Resample ENDP


Merge_DestMono_SrcFlipped_Src8_Volume_Filtered PROC ; 124

  ; check to see if we have to call the upsampling version
  cmp   [playback_ratio], 10000h
  jle   Merge_DestMono_SrcFlipped_Src8_Volume_UpFiltered

  ; build average dividers
  mov   ecx, edx
  mov   ebx, [playback_ratio]
  xor   edx, edx
  mov   eax, 65536  ; 100000000f/2048/32
  imul  [scale_left]
  div   ebx
  dec   eax
  mov   [divider_l], eax
  xor   edx, edx
  mov   eax, 65536  ; 100000000f/2048/32
  imul  [scale_right]
  div   ebx
  dec   eax
  mov   [divider_r], eax
  mov   edx, ecx

  ; load initial sample
  mov   ecx, [cur_l]

  ; Save registers
  push  PTR_EBP

  ; handle start up loop management
  mov  eax, edx
  and  edx, 03fffffffh
  test eax, 080000000h
  jnz  whole_continue124
  test eax, 040000000h
  jnz  last_continue124

  ; Merge sample data loop
  ALIGN 16
  merge_loop124:
  mov   ebx, 65536
  sub   ebx, edx
  add   edx, [playback_ratio]

  ; weight the initial sample
  mov   eax, [cur_r]
  imul  ecx, ebx
  imul  eax, ebx
  sar   ecx, 16
  sar   eax, 16
  cmp   edx, 65536*2
  mov   [cur_r], eax
  jl    skip_loop124

  ; loop to load all of the full sample points
  whole_loop124:
  cmp   PTR_ESI, [src_end]
  jae   src_whole_exit124
  whole_continue124:

  ; Load sample data
  xor   eax, eax
  xor   ebx, ebx
  mov   bx, word ptr [PTR_ESI]
  mov   al, bh
  and   ebx, 0ffh
  sub   eax, 080h
  sub   ebx, 080h
  shl   eax, 8
  shl   ebx, 8
  add   ecx, eax
  add   [cur_r], ebx
  sub   edx, 65536
  add   PTR_ESI, 2
  cmp   edx, 65536*2
  jae   whole_loop124

  skip_loop124:
  and   edx, 0ffffh
  cmp   PTR_ESI, [src_end]
  jae   src_last_exit124
  last_continue124:

  ; Load sample data
  xor   eax, eax
  xor   ebx, ebx
  mov   bx, word ptr [PTR_ESI]
  mov   al, bh
  and   ebx, 0ffh
  sub   eax, 080h
  sub   ebx, 080h
  shl   eax, 8
  shl   ebx, 8
  add   PTR_ESI, 2
  mov   ebp, ecx
  mov   ecx, eax

  ; weight the final sample
  imul  eax, edx
  sar   eax, 16
  add   eax, ebp
  mov   ebp, [cur_r]
  mov   [cur_r], ebx
  imul  ebx, edx
  sar   ebx, 16
  add   ebx, ebp
  imul  eax, [divider_l]
  imul  ebx, [divider_r]

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   PTR_EDI, 4
  cmp   PTR_EDI, dest_end
  jb    merge_loop124

  ; Jump out point if end of dest is reached
  dest_end_exit124:
  jmp   src_save_value124

  ; jump out when src is exceed, but save our current loop position
  src_whole_exit124:
  or   edx, 080000000h
  src_last_exit124:
  or   edx, 040000000h
  src_save_value124:
  mov  [cur_l], ecx

  ; Restore registers
  pop   PTR_EBP

  ; Routine end
  ret

Merge_DestMono_SrcFlipped_Src8_Volume_UpFiltered:

  ; adjust fractional
  shl   edx, 16

  ; Save registers
  push  PTR_EBP

  ; Load sample data
  xor   eax, eax
  xor   ebx, ebx
  mov   bx, word ptr [PTR_ESI]
  mov   al, bh
  and   ebx, 0ffh
  sub   eax, 080h
  sub   ebx, 080h
  shl   eax, 8
  shl   ebx, 8

  ; rotate filter values
  mov   ecx, [cur_l]
  mov   [cur_l], eax
  mov   eax, [cur_r]
  mov   [cur_r], ebx
  mov   [cur_r2], eax

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; Merge sample data loop
  ALIGN 16
  merge_loop636:

  ; Upsample the data points
  mov   ebp, edx
  mov   eax, [cur_l]
  mov   ebx, [cur_r]
  shr   ebp, 17
  sub   eax, ecx
  sub   ebx, [cur_r2]
  imul  eax, ebp
  imul  ebx, ebp
  sar   eax, 15
  sar   ebx, 15
  add   eax, ecx
  add   ebx, [cur_r2]

  ; Apply volume
  imul  eax, [scale_left]
  imul  ebx, [scale_right]

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   PTR_EDI, 4
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit636

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  jnc   merge_loop636

  ; Move the source pointer
  add   PTR_ESI, 2
  cmp   PTR_ESI, src_end
  jae   src_end_exit636

  ; Load sample data
  xor   eax, eax
  xor   ebx, ebx
  mov   bx, word ptr [PTR_ESI]
  mov   al, bh
  and   ebx, 0ffh
  sub   eax, 080h
  sub   ebx, 080h
  shl   eax, 8
  shl   ebx, 8

  ; rotate filter values
  mov   ecx, [cur_l]
  mov   [cur_l], eax
  mov   eax, [cur_r]
  mov   [cur_r], ebx
  mov   [cur_r2], eax

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; End loop
  jmp   merge_loop636

  ; Jump out point if end of dest is reached
  dest_end_exit636:
  add   PTR_ESI, 2
  add   edx, [step_fract]
  jc    skip_filter_adjust636

  ; rotate filter values
  mov   [cur_l], ecx
  mov   ecx, [cur_r2]
  mov   [cur_r], ecx

  ; un-increment the source to skip the early source adjustment
  sub   PTR_ESI, 2

  skip_filter_adjust636:

  ; Jump out point if end of src is reached
  src_end_exit636:

  ; Restore registers
  pop   PTR_EBP

  ; adjust fractional
  shr   edx, 16

  ; Routine end
  ret

Merge_DestMono_SrcFlipped_Src8_Volume_Filtered ENDP


Merge_DestStereo_SrcFlipped_Src8_Volume_Filtered PROC ; 125

  ; check to see if we have to call the upsampling version
  cmp   [playback_ratio], 10000h
  jle   Merge_DestStereo_SrcFlipped_Src8_Volume_UpFiltered

  ; build average dividers
  mov   ecx, edx
  mov   ebx, [playback_ratio]
  xor   edx, edx
  mov   eax, 65536  ; 100000000f/2048/32
  imul  [scale_left]
  div   ebx
  dec   eax
  mov   [divider_l], eax
  xor   edx, edx
  mov   eax, 65536  ; 100000000f/2048/32
  imul  [scale_right]
  div   ebx
  dec   eax
  mov   [divider_r], eax
  mov   edx, ecx

  ; load initial sample
  mov   ecx, [cur_l]

  ; Save registers
  push  PTR_EBP

  ; handle start up loop management
  mov  eax, edx
  and  edx, 03fffffffh
  test eax, 080000000h
  jnz  whole_continue125
  test eax, 040000000h
  jnz  last_continue125

  ; Merge sample data loop
  ALIGN 16
  merge_loop125:
  mov   ebx, 65536
  sub   ebx, edx
  add   edx, [playback_ratio]

  ; weight the initial sample
  mov   eax, [cur_r]
  imul  ecx, ebx
  imul  eax, ebx
  sar   ecx, 16
  sar   eax, 16
  cmp   edx, 65536*2
  mov   [cur_r], eax
  jl    skip_loop125

  ; loop to load all of the full sample points
  whole_loop125:
  cmp   PTR_ESI, [src_end]
  jae   src_whole_exit125
  whole_continue125:

  ; Load sample data
  xor   eax, eax
  xor   ebx, ebx
  mov   bx, word ptr [PTR_ESI]
  mov   al, bh
  and   ebx, 0ffh
  sub   eax, 080h
  sub   ebx, 080h
  shl   eax, 8
  shl   ebx, 8
  add   ecx, eax
  add   [cur_r], ebx
  sub   edx, 65536
  add   PTR_ESI, 2
  cmp   edx, 65536*2
  jae   whole_loop125

  skip_loop125:
  and   edx, 0ffffh
  cmp   PTR_ESI, [src_end]
  jae   src_last_exit125
  last_continue125:

  ; Load sample data
  xor   eax, eax
  xor   ebx, ebx
  mov   bx, word ptr [PTR_ESI]
  mov   al, bh
  and   ebx, 0ffh
  sub   eax, 080h
  sub   ebx, 080h
  shl   eax, 8
  shl   ebx, 8
  add   PTR_ESI, 2
  mov   ebp, ecx
  mov   ecx, eax

  ; weight the final sample
  imul  eax, edx
  sar   eax, 16
  add   eax, ebp
  mov   ebp, [cur_r]
  mov   [cur_r], ebx
  imul  ebx, edx
  sar   ebx, 16
  add   ebx, ebp
  imul  eax, [divider_l]
  imul  ebx, [divider_r]

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   [PTR_EDI+4], ebx
  add   PTR_EDI, 8
  cmp   PTR_EDI, dest_end
  jb    merge_loop125

  ; Jump out point if end of dest is reached
  dest_end_exit125:
  jmp   src_save_value125

  ; jump out when src is exceed, but save our current loop position
  src_whole_exit125:
  or   edx, 080000000h
  src_last_exit125:
  or   edx, 040000000h
  src_save_value125:
  mov  [cur_l], ecx

  ; Restore registers
  pop   PTR_EBP

  ; Routine end
  ret

Merge_DestStereo_SrcFlipped_Src8_Volume_UpFiltered:

  ; adjust fractional
  shl   edx, 16

  ; Save registers
  push  PTR_EBP

  ; Load sample data
  xor   eax, eax
  xor   ebx, ebx
  mov   bx, word ptr [PTR_ESI]
  mov   al, bh
  and   ebx, 0ffh
  sub   eax, 080h
  sub   ebx, 080h
  shl   eax, 8
  shl   ebx, 8

  ; rotate filter values
  mov   ecx, [cur_l]
  mov   [cur_l], eax
  mov   eax, [cur_r]
  mov   [cur_r], ebx
  mov   [cur_r2], eax

  ; Merge sample data loop
  ALIGN 16
  merge_loop637:

  ; Upsample the data points
  mov   ebp, edx
  mov   eax, [cur_l]
  mov   ebx, [cur_r]
  shr   ebp, 17
  sub   eax, ecx
  sub   ebx, [cur_r2]
  imul  eax, ebp
  imul  ebx, ebp
  sar   eax, 15
  sar   ebx, 15
  add   eax, ecx
  add   ebx, [cur_r2]

  ; Apply volume
  imul  eax, [scale_left]
  imul  ebx, [scale_right]

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   [PTR_EDI+4], ebx
  add   PTR_EDI, 8
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit637

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  jnc   merge_loop637

  ; Move the source pointer
  add   PTR_ESI, 2
  cmp   PTR_ESI, src_end
  jae   src_end_exit637

  ; Load sample data
  xor   eax, eax
  xor   ebx, ebx
  mov   bx, word ptr [PTR_ESI]
  mov   al, bh
  and   ebx, 0ffh
  sub   eax, 080h
  sub   ebx, 080h
  shl   eax, 8
  shl   ebx, 8

  ; rotate filter values
  mov   ecx, [cur_l]
  mov   [cur_l], eax
  mov   eax, [cur_r]
  mov   [cur_r], ebx
  mov   [cur_r2], eax

  ; End loop
  jmp   merge_loop637

  ; Jump out point if end of dest is reached
  dest_end_exit637:
  add   PTR_ESI, 2
  add   edx, [step_fract]
  jc    skip_filter_adjust637

  ; rotate filter values
  mov   [cur_l], ecx
  mov   ecx, [cur_r2]
  mov   [cur_r], ecx

  ; un-increment the source to skip the early source adjustment
  sub   PTR_ESI, 2

  skip_filter_adjust637:

  ; Jump out point if end of src is reached
  src_end_exit637:

  ; Restore registers
  pop   PTR_EBP

  ; adjust fractional
  shr   edx, 16

  ; Routine end
  ret

Merge_DestStereo_SrcFlipped_Src8_Volume_Filtered ENDP


Merge_DestMono_SrcFlipped_Src16_Volume_Filtered PROC ; 126

  ; check to see if we have to call the upsampling version
  cmp   [playback_ratio], 10000h
  jle   Merge_DestMono_SrcFlipped_Src16_Volume_UpFiltered

  ; build average dividers
  mov   ecx, edx
  mov   ebx, [playback_ratio]
  xor   edx, edx
  mov   eax, 65536  ; 100000000f/2048/32
  imul  [scale_left]
  div   ebx
  dec   eax
  mov   [divider_l], eax
  xor   edx, edx
  mov   eax, 65536  ; 100000000f/2048/32
  imul  [scale_right]
  div   ebx
  dec   eax
  mov   [divider_r], eax
  mov   edx, ecx

  ; load initial sample
  mov   ecx, [cur_l]

  ; Save registers
  push  PTR_EBP

  ; handle start up loop management
  mov  eax, edx
  and  edx, 03fffffffh
  test eax, 080000000h
  jnz  whole_continue126
  test eax, 040000000h
  jnz  last_continue126

  ; Merge sample data loop
  ALIGN 16
  merge_loop126:
  mov   ebx, 65536
  sub   ebx, edx
  add   edx, [playback_ratio]

  ; weight the initial sample
  mov   eax, [cur_r]
  imul  ecx, ebx
  imul  eax, ebx
  sar   ecx, 16
  sar   eax, 16
  cmp   edx, 65536*2
  mov   [cur_r], eax
  jl    skip_loop126

  ; loop to load all of the full sample points
  whole_loop126:
  cmp   PTR_ESI, [src_end]
  jae   src_whole_exit126
  whole_continue126:

  ; Load sample data
  mov   eax, dword ptr [PTR_ESI]
  movsx ebx, ax
  sar   eax, 16
  add   ecx, eax
  add   [cur_r], ebx
  sub   edx, 65536
  add   PTR_ESI, 4
  cmp   edx, 65536*2
  jae   whole_loop126

  skip_loop126:
  and   edx, 0ffffh
  cmp   PTR_ESI, [src_end]
  jae   src_last_exit126
  last_continue126:

  ; Load sample data
  mov   eax, dword ptr [PTR_ESI]
  movsx ebx, ax
  sar   eax, 16
  add   PTR_ESI, 4
  mov   ebp, ecx
  mov   ecx, eax

  ; weight the final sample
  imul  eax, edx
  sar   eax, 16
  add   eax, ebp
  mov   ebp, [cur_r]
  mov   [cur_r], ebx
  imul  ebx, edx
  sar   ebx, 16
  add   ebx, ebp
  imul  eax, [divider_l]
  imul  ebx, [divider_r]

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   PTR_EDI, 4
  cmp   PTR_EDI, dest_end
  jb    merge_loop126

  ; Jump out point if end of dest is reached
  dest_end_exit126:
  jmp   src_save_value126

  ; jump out when src is exceed, but save our current loop position
  src_whole_exit126:
  or   edx, 080000000h
  src_last_exit126:
  or   edx, 040000000h
  src_save_value126:
  mov  [cur_l], ecx

  ; Restore registers
  pop   PTR_EBP

  ; Routine end
  ret

Merge_DestMono_SrcFlipped_Src16_Volume_UpFiltered:

  ; adjust fractional
  shl   edx, 16

  ; Save registers
  push  PTR_EBP

  ; Load sample data
  mov   eax, dword ptr [PTR_ESI]
  movsx ebx, ax
  sar   eax, 16

  ; rotate filter values
  mov   ecx, [cur_l]
  mov   [cur_l], eax
  mov   eax, [cur_r]
  mov   [cur_r], ebx
  mov   [cur_r2], eax

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; Merge sample data loop
  ALIGN 16
  merge_loop638:

  ; Upsample the data points
  mov   ebp, edx
  mov   eax, [cur_l]
  mov   ebx, [cur_r]
  shr   ebp, 17
  sub   eax, ecx
  sub   ebx, [cur_r2]
  imul  eax, ebp
  imul  ebx, ebp
  sar   eax, 15
  sar   ebx, 15
  add   eax, ecx
  add   ebx, [cur_r2]

  ; Apply volume
  imul  eax, [scale_left]
  imul  ebx, [scale_right]

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   PTR_EDI, 4
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit638

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  jnc   merge_loop638

  ; Move the source pointer
  add   PTR_ESI, 4
  cmp   PTR_ESI, src_end
  jae   src_end_exit638

  ; Load sample data
  mov   eax, dword ptr [PTR_ESI]
  movsx ebx, ax
  sar   eax, 16

  ; rotate filter values
  mov   ecx, [cur_l]
  mov   [cur_l], eax
  mov   eax, [cur_r]
  mov   [cur_r], ebx
  mov   [cur_r2], eax

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; End loop
  jmp   merge_loop638

  ; Jump out point if end of dest is reached
  dest_end_exit638:
  add   PTR_ESI, 4
  add   edx, [step_fract]
  jc    skip_filter_adjust638

  ; rotate filter values
  mov   [cur_l], ecx
  mov   ecx, [cur_r2]
  mov   [cur_r], ecx

  ; un-increment the source to skip the early source adjustment
  sub   PTR_ESI, 4

  skip_filter_adjust638:

  ; Jump out point if end of src is reached
  src_end_exit638:

  ; Restore registers
  pop   PTR_EBP

  ; adjust fractional
  shr   edx, 16

  ; Routine end
  ret

Merge_DestMono_SrcFlipped_Src16_Volume_Filtered ENDP


Merge_DestStereo_SrcFlipped_Src16_Volume_Filtered PROC ; 127

  ; check to see if we have to call the upsampling version
  cmp   [playback_ratio], 10000h
  jle   Merge_DestStereo_SrcFlipped_Src16_Volume_UpFiltered

  ; build average dividers
  mov   ecx, edx
  mov   ebx, [playback_ratio]
  xor   edx, edx
  mov   eax, 65536  ; 100000000f/2048/32
  imul  [scale_left]
  div   ebx
  dec   eax
  mov   [divider_l], eax
  xor   edx, edx
  mov   eax, 65536  ; 100000000f/2048/32
  imul  [scale_right]
  div   ebx
  dec   eax
  mov   [divider_r], eax
  mov   edx, ecx

  ; load initial sample
  mov   ecx, [cur_l]

  ; Save registers
  push  PTR_EBP

  ; handle start up loop management
  mov  eax, edx
  and  edx, 03fffffffh
  test eax, 080000000h
  jnz  whole_continue127
  test eax, 040000000h
  jnz  last_continue127

  ; Merge sample data loop
  ALIGN 16
  merge_loop127:
  mov   ebx, 65536
  sub   ebx, edx
  add   edx, [playback_ratio]

  ; weight the initial sample
  mov   eax, [cur_r]
  imul  ecx, ebx
  imul  eax, ebx
  sar   ecx, 16
  sar   eax, 16
  cmp   edx, 65536*2
  mov   [cur_r], eax
  jl    skip_loop127

  ; loop to load all of the full sample points
  whole_loop127:
  cmp   PTR_ESI, [src_end]
  jae   src_whole_exit127
  whole_continue127:

  ; Load sample data
  mov   eax, dword ptr [PTR_ESI]
  movsx ebx, ax
  sar   eax, 16
  add   ecx, eax
  add   [cur_r], ebx
  sub   edx, 65536
  add   PTR_ESI, 4
  cmp   edx, 65536*2
  jae   whole_loop127

  skip_loop127:
  and   edx, 0ffffh
  cmp   PTR_ESI, [src_end]
  jae   src_last_exit127
  last_continue127:

  ; Load sample data
  mov   eax, dword ptr [PTR_ESI]
  movsx ebx, ax
  sar   eax, 16
  add   PTR_ESI, 4
  mov   ebp, ecx
  mov   ecx, eax

  ; weight the final sample
  imul  eax, edx
  sar   eax, 16
  add   eax, ebp
  mov   ebp, [cur_r]
  mov   [cur_r], ebx
  imul  ebx, edx
  sar   ebx, 16
  add   ebx, ebp
  imul  eax, [divider_l]
  imul  ebx, [divider_r]

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   [PTR_EDI+4], ebx
  add   PTR_EDI, 8
  cmp   PTR_EDI, dest_end
  jb    merge_loop127

  ; Jump out point if end of dest is reached
  dest_end_exit127:
  jmp   src_save_value127

  ; jump out when src is exceed, but save our current loop position
  src_whole_exit127:
  or   edx, 080000000h
  src_last_exit127:
  or   edx, 040000000h
  src_save_value127:
  mov  [cur_l], ecx

  ; Restore registers
  pop   PTR_EBP

  ; Routine end
  ret

Merge_DestStereo_SrcFlipped_Src16_Volume_UpFiltered:

  ; adjust fractional
  shl   edx, 16

  ; Save registers
  push  PTR_EBP

  ; Load sample data
  mov   eax, dword ptr [PTR_ESI]
  movsx ebx, ax
  sar   eax, 16

  ; rotate filter values
  mov   ecx, [cur_l]
  mov   [cur_l], eax
  mov   eax, [cur_r]
  mov   [cur_r], ebx
  mov   [cur_r2], eax

  ; Merge sample data loop
  ALIGN 16
  merge_loop639:

  ; Upsample the data points
  mov   ebp, edx
  mov   eax, [cur_l]
  mov   ebx, [cur_r]
  shr   ebp, 17
  sub   eax, ecx
  sub   ebx, [cur_r2]
  imul  eax, ebp
  imul  ebx, ebp
  sar   eax, 15
  sar   ebx, 15
  add   eax, ecx
  add   ebx, [cur_r2]

  ; Apply volume
  imul  eax, [scale_left]
  imul  ebx, [scale_right]

  ; Merge sample data into output buffer
  add   [PTR_EDI], eax
  add   [PTR_EDI+4], ebx
  add   PTR_EDI, 8
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit639

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  jnc   merge_loop639

  ; Move the source pointer
  add   PTR_ESI, 4
  cmp   PTR_ESI, src_end
  jae   src_end_exit639

  ; Load sample data
  mov   eax, dword ptr [PTR_ESI]
  movsx ebx, ax
  sar   eax, 16

  ; rotate filter values
  mov   ecx, [cur_l]
  mov   [cur_l], eax
  mov   eax, [cur_r]
  mov   [cur_r], ebx
  mov   [cur_r2], eax

  ; End loop
  jmp   merge_loop639

  ; Jump out point if end of dest is reached
  dest_end_exit639:
  add   PTR_ESI, 4
  add   edx, [step_fract]
  jc    skip_filter_adjust639

  ; rotate filter values
  mov   [cur_l], ecx
  mov   ecx, [cur_r2]
  mov   [cur_r], ecx

  ; un-increment the source to skip the early source adjustment
  sub   PTR_ESI, 4

  skip_filter_adjust639:

  ; Jump out point if end of src is reached
  src_end_exit639:

  ; Restore registers
  pop   PTR_EBP

  ; adjust fractional
  shr   edx, 16

  ; Routine end
  ret

Merge_DestStereo_SrcFlipped_Src16_Volume_Filtered ENDP


Merge_DestMono_SrcMono_Src8_Copy16_NoResample PROC ; 128

  ; Load sample data
  xor   eax, eax
  mov   al, byte ptr [PTR_ESI]
  sub   eax, 080h
  shl   eax, 8

  ; Merge sample data loop
  ALIGN 16
  merge_loop128:

  ; Copy sample data into output buffer
  mov   [PTR_EDI], ax
  add   PTR_EDI, 2
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit128

  ; Move the source pointer
  inc   PTR_ESI
  cmp   PTR_ESI, src_end
  jae   src_end_exit128

  ; Load sample data
  xor   eax, eax
  mov   al, byte ptr [PTR_ESI]
  sub   eax, 080h
  shl   eax, 8

  ; End loop
  jmp   merge_loop128

  ; Jump out point if end of dest is reached
  dest_end_exit128:
  inc   PTR_ESI

  ; Jump out point if end of src is reached
  src_end_exit128:

  ; Routine end
  ret

Merge_DestMono_SrcMono_Src8_Copy16_NoResample ENDP


Merge_DestStereo_SrcMono_Src8_Copy16_NoResample PROC ; 129

  ; Load sample data
  xor   eax, eax
  mov   al, byte ptr [PTR_ESI]
  sub   eax, 080h
  shl   eax, 8

  ; Merge sample data loop
  ALIGN 16
  merge_loop129:

  ; Copy sample data into output buffer
  mov   [PTR_EDI], ax
  mov   [PTR_EDI+2], ax
  add   PTR_EDI, 4
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit129

  ; Move the source pointer
  inc   PTR_ESI
  cmp   PTR_ESI, src_end
  jae   src_end_exit129

  ; Load sample data
  xor   eax, eax
  mov   al, byte ptr [PTR_ESI]
  sub   eax, 080h
  shl   eax, 8

  ; End loop
  jmp   merge_loop129

  ; Jump out point if end of dest is reached
  dest_end_exit129:
  inc   PTR_ESI

  ; Jump out point if end of src is reached
  src_end_exit129:

  ; Routine end
  ret

Merge_DestStereo_SrcMono_Src8_Copy16_NoResample ENDP


Merge_DestMono_SrcMono_Src16_Copy16_NoResample PROC ; 130

  ; Load sample data
  movsx eax, word ptr [PTR_ESI]

  ; Merge sample data loop
  ALIGN 16
  merge_loop130:

  ; Copy sample data into output buffer
  mov   [PTR_EDI], ax
  add   PTR_EDI, 2
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit130

  ; Move the source pointer
  add   PTR_ESI, 2
  cmp   PTR_ESI, src_end
  jae   src_end_exit130

  ; Load sample data
  movsx eax, word ptr [PTR_ESI]

  ; End loop
  jmp   merge_loop130

  ; Jump out point if end of dest is reached
  dest_end_exit130:
  add   PTR_ESI, 2

  ; Jump out point if end of src is reached
  src_end_exit130:

  ; Routine end
  ret

Merge_DestMono_SrcMono_Src16_Copy16_NoResample ENDP


Merge_DestStereo_SrcMono_Src16_Copy16_NoResample PROC ; 131

  ; Load sample data
  movsx eax, word ptr [PTR_ESI]

  ; Merge sample data loop
  ALIGN 16
  merge_loop131:

  ; Copy sample data into output buffer
  mov   [PTR_EDI], ax
  mov   [PTR_EDI+2], ax
  add   PTR_EDI, 4
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit131

  ; Move the source pointer
  add   PTR_ESI, 2
  cmp   PTR_ESI, src_end
  jae   src_end_exit131

  ; Load sample data
  movsx eax, word ptr [PTR_ESI]

  ; End loop
  jmp   merge_loop131

  ; Jump out point if end of dest is reached
  dest_end_exit131:
  add   PTR_ESI, 2

  ; Jump out point if end of src is reached
  src_end_exit131:

  ; Routine end
  ret

Merge_DestStereo_SrcMono_Src16_Copy16_NoResample ENDP


Merge_DestMono_SrcStereo_Src8_Copy16_NoResample PROC ; 136

  ; Load sample data
  xor   ebx, ebx
  xor   eax, eax
  mov   ax, word ptr [PTR_ESI]
  mov   bl, ah
  and   eax, 0ffh
  sub   ebx, 080h
  sub   eax, 080h
  shl   ebx, 8
  shl   eax, 8

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; Shift right to avoid clipping on direct copies
  sar   eax, 1

  ; Merge sample data loop
  ALIGN 16
  merge_loop136:

  ; Copy sample data into output buffer
  mov   [PTR_EDI], ax
  add   PTR_EDI, 2
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit136

  ; Move the source pointer
  add   PTR_ESI, 2
  cmp   PTR_ESI, src_end
  jae   src_end_exit136

  ; Load sample data
  xor   ebx, ebx
  xor   eax, eax
  mov   ax, word ptr [PTR_ESI]
  mov   bl, ah
  and   eax, 0ffh
  sub   ebx, 080h
  sub   eax, 080h
  shl   ebx, 8
  shl   eax, 8

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; Shift right to avoid clipping on direct copies
  sar   eax, 1

  ; End loop
  jmp   merge_loop136

  ; Jump out point if end of dest is reached
  dest_end_exit136:
  add   PTR_ESI, 2

  ; Jump out point if end of src is reached
  src_end_exit136:

  ; Routine end
  ret

Merge_DestMono_SrcStereo_Src8_Copy16_NoResample ENDP


Merge_DestStereo_SrcStereo_Src8_Copy16_NoResample PROC ; 137

  ; Load sample data
  xor   ebx, ebx
  xor   eax, eax
  mov   ax, word ptr [PTR_ESI]
  mov   bl, ah
  and   eax, 0ffh
  sub   ebx, 080h
  sub   eax, 080h
  shl   ebx, 8
  shl   eax, 8

  ; Merge sample data loop
  ALIGN 16
  merge_loop137:

  ; Copy sample data into output buffer
  mov   [PTR_EDI], ax
  mov   [PTR_EDI+2], bx
  add   PTR_EDI, 4
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit137

  ; Move the source pointer
  add   PTR_ESI, 2
  cmp   PTR_ESI, src_end
  jae   src_end_exit137

  ; Load sample data
  xor   ebx, ebx
  xor   eax, eax
  mov   ax, word ptr [PTR_ESI]
  mov   bl, ah
  and   eax, 0ffh
  sub   ebx, 080h
  sub   eax, 080h
  shl   ebx, 8
  shl   eax, 8

  ; End loop
  jmp   merge_loop137

  ; Jump out point if end of dest is reached
  dest_end_exit137:
  add   PTR_ESI, 2

  ; Jump out point if end of src is reached
  src_end_exit137:

  ; Routine end
  ret

Merge_DestStereo_SrcStereo_Src8_Copy16_NoResample ENDP


Merge_DestMono_SrcStereo_Src16_Copy16_NoResample PROC ; 138

  ; Load sample data
  mov   ebx, dword ptr [PTR_ESI]
  movsx eax, bx
  sar   ebx, 16

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; Shift right to avoid clipping on direct copies
  sar   eax, 1

  ; Merge sample data loop
  ALIGN 16
  merge_loop138:

  ; Copy sample data into output buffer
  mov   [PTR_EDI], ax
  add   PTR_EDI, 2
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit138

  ; Move the source pointer
  add   PTR_ESI, 4
  cmp   PTR_ESI, src_end
  jae   src_end_exit138

  ; Load sample data
  mov   ebx, dword ptr [PTR_ESI]
  movsx eax, bx
  sar   ebx, 16

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; Shift right to avoid clipping on direct copies
  sar   eax, 1

  ; End loop
  jmp   merge_loop138

  ; Jump out point if end of dest is reached
  dest_end_exit138:
  add   PTR_ESI, 4

  ; Jump out point if end of src is reached
  src_end_exit138:

  ; Routine end
  ret

Merge_DestMono_SrcStereo_Src16_Copy16_NoResample ENDP


Merge_DestStereo_SrcStereo_Src16_Copy16_NoResample PROC ; 139

  ; Load sample data
  mov   ebx, dword ptr [PTR_ESI]
  movsx eax, bx
  sar   ebx, 16

  ; Merge sample data loop
  ALIGN 16
  merge_loop139:

  ; Copy sample data into output buffer
  mov   [PTR_EDI], ax
  mov   [PTR_EDI+2], bx
  add   PTR_EDI, 4
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit139

  ; Move the source pointer
  add   PTR_ESI, 4
  cmp   PTR_ESI, src_end
  jae   src_end_exit139

  ; Load sample data
  mov   ebx, dword ptr [PTR_ESI]
  movsx eax, bx
  sar   ebx, 16

  ; End loop
  jmp   merge_loop139

  ; Jump out point if end of dest is reached
  dest_end_exit139:
  add   PTR_ESI, 4

  ; Jump out point if end of src is reached
  src_end_exit139:

  ; Routine end
  ret

Merge_DestStereo_SrcStereo_Src16_Copy16_NoResample ENDP


Merge_DestMono_SrcFlipped_Src8_Copy16_NoResample PROC ; 152

  ; Load sample data
  xor   eax, eax
  xor   ebx, ebx
  mov   bx, word ptr [PTR_ESI]
  mov   al, bh
  and   ebx, 0ffh
  sub   eax, 080h
  sub   ebx, 080h
  shl   eax, 8
  shl   ebx, 8

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; Shift right to avoid clipping on direct copies
  sar   eax, 1

  ; Merge sample data loop
  ALIGN 16
  merge_loop152:

  ; Copy sample data into output buffer
  mov   [PTR_EDI], ax
  add   PTR_EDI, 2
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit152

  ; Move the source pointer
  add   PTR_ESI, 2
  cmp   PTR_ESI, src_end
  jae   src_end_exit152

  ; Load sample data
  xor   eax, eax
  xor   ebx, ebx
  mov   bx, word ptr [PTR_ESI]
  mov   al, bh
  and   ebx, 0ffh
  sub   eax, 080h
  sub   ebx, 080h
  shl   eax, 8
  shl   ebx, 8

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; Shift right to avoid clipping on direct copies
  sar   eax, 1

  ; End loop
  jmp   merge_loop152

  ; Jump out point if end of dest is reached
  dest_end_exit152:
  add   PTR_ESI, 2

  ; Jump out point if end of src is reached
  src_end_exit152:

  ; Routine end
  ret

Merge_DestMono_SrcFlipped_Src8_Copy16_NoResample ENDP


Merge_DestStereo_SrcFlipped_Src8_Copy16_NoResample PROC ; 153

  ; Load sample data
  xor   eax, eax
  xor   ebx, ebx
  mov   bx, word ptr [PTR_ESI]
  mov   al, bh
  and   ebx, 0ffh
  sub   eax, 080h
  sub   ebx, 080h
  shl   eax, 8
  shl   ebx, 8

  ; Merge sample data loop
  ALIGN 16
  merge_loop153:

  ; Copy sample data into output buffer
  mov   [PTR_EDI], ax
  mov   [PTR_EDI+2], bx
  add   PTR_EDI, 4
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit153

  ; Move the source pointer
  add   PTR_ESI, 2
  cmp   PTR_ESI, src_end
  jae   src_end_exit153

  ; Load sample data
  xor   eax, eax
  xor   ebx, ebx
  mov   bx, word ptr [PTR_ESI]
  mov   al, bh
  and   ebx, 0ffh
  sub   eax, 080h
  sub   ebx, 080h
  shl   eax, 8
  shl   ebx, 8

  ; End loop
  jmp   merge_loop153

  ; Jump out point if end of dest is reached
  dest_end_exit153:
  add   PTR_ESI, 2

  ; Jump out point if end of src is reached
  src_end_exit153:

  ; Routine end
  ret

Merge_DestStereo_SrcFlipped_Src8_Copy16_NoResample ENDP


Merge_DestMono_SrcFlipped_Src16_Copy16_NoResample PROC ; 154

  ; Load sample data
  mov   eax, dword ptr [PTR_ESI]
  movsx ebx, ax
  sar   eax, 16

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; Shift right to avoid clipping on direct copies
  sar   eax, 1

  ; Merge sample data loop
  ALIGN 16
  merge_loop154:

  ; Copy sample data into output buffer
  mov   [PTR_EDI], ax
  add   PTR_EDI, 2
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit154

  ; Move the source pointer
  add   PTR_ESI, 4
  cmp   PTR_ESI, src_end
  jae   src_end_exit154

  ; Load sample data
  mov   eax, dword ptr [PTR_ESI]
  movsx ebx, ax
  sar   eax, 16

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; Shift right to avoid clipping on direct copies
  sar   eax, 1

  ; End loop
  jmp   merge_loop154

  ; Jump out point if end of dest is reached
  dest_end_exit154:
  add   PTR_ESI, 4

  ; Jump out point if end of src is reached
  src_end_exit154:

  ; Routine end
  ret

Merge_DestMono_SrcFlipped_Src16_Copy16_NoResample ENDP


Merge_DestStereo_SrcFlipped_Src16_Copy16_NoResample PROC ; 155

  ; Load sample data
  mov   eax, dword ptr [PTR_ESI]
  movsx ebx, ax
  sar   eax, 16

  ; Merge sample data loop
  ALIGN 16
  merge_loop155:

  ; Copy sample data into output buffer
  mov   [PTR_EDI], ax
  mov   [PTR_EDI+2], bx
  add   PTR_EDI, 4
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit155

  ; Move the source pointer
  add   PTR_ESI, 4
  cmp   PTR_ESI, src_end
  jae   src_end_exit155

  ; Load sample data
  mov   eax, dword ptr [PTR_ESI]
  movsx ebx, ax
  sar   eax, 16

  ; End loop
  jmp   merge_loop155

  ; Jump out point if end of dest is reached
  dest_end_exit155:
  add   PTR_ESI, 4

  ; Jump out point if end of src is reached
  src_end_exit155:

  ; Routine end
  ret

Merge_DestStereo_SrcFlipped_Src16_Copy16_NoResample ENDP


Merge_DestMono_SrcMono_Src8_Copy16_Resample PROC ; 160

  ; adjust fractional
  shl   edx, 16

  ; Load sample data
  xor   eax, eax
  mov   al, byte ptr [PTR_ESI]
  sub   eax, 080h
  shl   eax, 8

  ; Merge sample data loop
  ALIGN 16
  merge_loop160:

  ; Copy sample data into output buffer
  mov   [PTR_EDI], ax
  add   PTR_EDI, 2
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit160

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  sbb   PTR_EAX, PTR_EAX
  add   PTR_ESI, [STEPTBL_PTR][PTR_SIZE+PTR_EAX*PTR_SIZE]
  cmp   PTR_ESI, src_end
  jae   src_end_exit160

  ; Load sample data
  xor   eax, eax
  mov   al, byte ptr [PTR_ESI]
  sub   eax, 080h
  shl   eax, 8

  ; End loop
  jmp   merge_loop160

  ; Jump out point if end of dest is reached
  dest_end_exit160:

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  sbb   PTR_EAX, PTR_EAX
  add   PTR_ESI, [STEPTBL_PTR][PTR_SIZE+PTR_EAX*PTR_SIZE]

  ; Jump out point if end of src is reached
  src_end_exit160:

  ; adjust fractional
  shr   edx, 16

  ; Routine end
  ret

Merge_DestMono_SrcMono_Src8_Copy16_Resample ENDP


Merge_DestStereo_SrcMono_Src8_Copy16_Resample PROC ; 161

  ; adjust fractional
  shl   edx, 16

  ; Load sample data
  xor   eax, eax
  mov   al, byte ptr [PTR_ESI]
  sub   eax, 080h
  shl   eax, 8

  ; Merge sample data loop
  ALIGN 16
  merge_loop161:

  ; Copy sample data into output buffer
  mov   [PTR_EDI], ax
  mov   [PTR_EDI+2], ax
  add   PTR_EDI, 4
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit161

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  sbb   PTR_EAX, PTR_EAX
  add   PTR_ESI, [STEPTBL_PTR][PTR_SIZE+PTR_EAX*PTR_SIZE]
  cmp   PTR_ESI, src_end
  jae   src_end_exit161

  ; Load sample data
  xor   eax, eax
  mov   al, byte ptr [PTR_ESI]
  sub   eax, 080h
  shl   eax, 8

  ; End loop
  jmp   merge_loop161

  ; Jump out point if end of dest is reached
  dest_end_exit161:

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  sbb   PTR_EAX, PTR_EAX
  add   PTR_ESI, [STEPTBL_PTR][PTR_SIZE+PTR_EAX*PTR_SIZE]

  ; Jump out point if end of src is reached
  src_end_exit161:

  ; adjust fractional
  shr   edx, 16

  ; Routine end
  ret

Merge_DestStereo_SrcMono_Src8_Copy16_Resample ENDP


Merge_DestMono_SrcMono_Src16_Copy16_Resample PROC ; 162

  ; adjust fractional
  shl   edx, 16

  ; Load sample data
  movsx eax, word ptr [PTR_ESI]

  ; Merge sample data loop
  ALIGN 16
  merge_loop162:

  ; Copy sample data into output buffer
  mov   [PTR_EDI], ax
  add   PTR_EDI, 2
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit162

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  sbb   PTR_EAX, PTR_EAX
  add   PTR_ESI, [STEPTBL_PTR][PTR_SIZE+PTR_EAX*PTR_SIZE]
  cmp   PTR_ESI, src_end
  jae   src_end_exit162

  ; Load sample data
  movsx eax, word ptr [PTR_ESI]

  ; End loop
  jmp   merge_loop162

  ; Jump out point if end of dest is reached
  dest_end_exit162:

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  sbb   PTR_EAX, PTR_EAX
  add   PTR_ESI, [STEPTBL_PTR][PTR_SIZE+PTR_EAX*PTR_SIZE]

  ; Jump out point if end of src is reached
  src_end_exit162:

  ; adjust fractional
  shr   edx, 16

  ; Routine end
  ret

Merge_DestMono_SrcMono_Src16_Copy16_Resample ENDP


Merge_DestStereo_SrcMono_Src16_Copy16_Resample PROC ; 163

  ; adjust fractional
  shl   edx, 16

  ; Load sample data
  movsx eax, word ptr [PTR_ESI]

  ; Merge sample data loop
  ALIGN 16
  merge_loop163:

  ; Copy sample data into output buffer
  mov   [PTR_EDI], ax
  mov   [PTR_EDI+2], ax
  add   PTR_EDI, 4
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit163

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  sbb   PTR_EAX, PTR_EAX
  add   PTR_ESI, [STEPTBL_PTR][PTR_SIZE+PTR_EAX*PTR_SIZE]
  cmp   PTR_ESI, src_end
  jae   src_end_exit163

  ; Load sample data
  movsx eax, word ptr [PTR_ESI]

  ; End loop
  jmp   merge_loop163

  ; Jump out point if end of dest is reached
  dest_end_exit163:

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  sbb   PTR_EAX, PTR_EAX
  add   PTR_ESI, [STEPTBL_PTR][PTR_SIZE+PTR_EAX*PTR_SIZE]

  ; Jump out point if end of src is reached
  src_end_exit163:

  ; adjust fractional
  shr   edx, 16

  ; Routine end
  ret

Merge_DestStereo_SrcMono_Src16_Copy16_Resample ENDP


Merge_DestMono_SrcMono_Src8_Copy16_Filtered PROC ; 164

  ; check to see if we have to call the upsampling version
  cmp   [playback_ratio], 10000h
  jle   Merge_DestMono_SrcMono_Src8_Copy16_UpFiltered

  ; build average dividers
  mov   ecx, edx
  mov   ebx, [playback_ratio]
  xor   edx, edx
  mov   eax, 08000000h  ; 100000000f/32
  div   ebx
  dec   eax
  mov   [divider_l], eax
  mov   edx, ecx

  ; load initial sample
  mov   ecx, [cur_l]

  ; handle start up loop management
  mov  eax, edx
  and  edx, 03fffffffh
  test eax, 080000000h
  jnz  whole_continue164
  test eax, 040000000h
  jnz  last_continue164

  ; Merge sample data loop
  ALIGN 16
  merge_loop164:
  mov   ebx, 65536
  sub   ebx, edx
  add   edx, [playback_ratio]

  ; weight the initial sample
  imul  ecx, ebx
  sar   ecx, 16
  cmp   edx, 65536*2
  jl    skip_loop164

  ; loop to load all of the full sample points
  whole_loop164:
  cmp   PTR_ESI, [src_end]
  jae   src_whole_exit164
  whole_continue164:

  ; Load sample data
  xor   eax, eax
  mov   al, byte ptr [PTR_ESI]
  sub   eax, 080h
  shl   eax, 8
  add   ecx, eax
  sub   edx, 65536
  inc   PTR_ESI
  cmp   edx, 65536*2
  jae   whole_loop164

  skip_loop164:
  and   edx, 0ffffh
  last_continue164:
  cmp   PTR_ESI, [src_end]
  jae   src_last_exit164

  ; Load sample data
  xor   eax, eax
  mov   al, byte ptr [PTR_ESI]
  sub   eax, 080h
  shl   eax, 8
  inc   PTR_ESI
  mov   ebx, ecx
  mov   ecx, eax

  ; weight the final sample
  imul  eax, edx
  sar   eax, 16
  add   eax, ebx
  imul  eax, [divider_l]

  ; Scale back down after divide
  sar   eax, 11

  ; Copy sample data into output buffer
  mov   [PTR_EDI], ax
  add   PTR_EDI, 2
  cmp   PTR_EDI, dest_end
  jb    merge_loop164

  ; Jump out point if end of dest is reached
  dest_end_exit164:
  jmp   src_save_value164

  ; jump out when src is exceed, but save our current loop position
  src_whole_exit164:
  or   edx, 080000000h
  src_last_exit164:
  or   edx, 040000000h
  src_save_value164:
  mov  [cur_l], ecx

  ; Routine end
  ret

Merge_DestMono_SrcMono_Src8_Copy16_UpFiltered:

  ; adjust fractional
  shl   edx, 16

  ; Load sample data
  xor   eax, eax
  mov   al, byte ptr [PTR_ESI]
  sub   eax, 080h
  shl   eax, 8

  ; rotate filter values
  mov   ecx, [cur_l]
  mov   [cur_l], eax

  ; Merge sample data loop
  ALIGN 16
  merge_loop676:

  ; Upsample the data points
  mov   ebx, edx
  mov   eax, [cur_l]
  shr   ebx, 17
  sub   eax, ecx
  imul  eax, ebx
  sar   eax, 15
  add   eax, ecx

  ; Copy sample data into output buffer
  mov   [PTR_EDI], ax
  add   PTR_EDI, 2
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit676

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  jnc   merge_loop676

  ; Move the source pointer
  inc   PTR_ESI
  cmp   PTR_ESI, src_end
  jae   src_end_exit676

  ; Load sample data
  xor   eax, eax
  mov   al, byte ptr [PTR_ESI]
  sub   eax, 080h
  shl   eax, 8

  ; rotate filter values
  mov   ecx, [cur_l]
  mov   [cur_l], eax

  ; End loop
  jmp   merge_loop676

  ; Jump out point if end of dest is reached
  dest_end_exit676:
  inc   PTR_ESI
  add   edx, [step_fract]
  jc    skip_filter_adjust676

  ; rotate filter values
  mov   [cur_l], ecx

  ; un-increment the source to skip the early source adjustment
  dec   PTR_ESI

  skip_filter_adjust676:

  ; Jump out point if end of src is reached
  src_end_exit676:

  ; adjust fractional
  shr   edx, 16

  ; Routine end
  ret

Merge_DestMono_SrcMono_Src8_Copy16_Filtered ENDP


Merge_DestStereo_SrcMono_Src8_Copy16_Filtered PROC ; 165

  ; check to see if we have to call the upsampling version
  cmp   [playback_ratio], 10000h
  jle   Merge_DestStereo_SrcMono_Src8_Copy16_UpFiltered

  ; build average dividers
  mov   ecx, edx
  mov   ebx, [playback_ratio]
  xor   edx, edx
  mov   eax, 08000000h  ; 100000000f/32
  div   ebx
  dec   eax
  mov   [divider_l], eax
  mov   edx, ecx

  ; load initial sample
  mov   ecx, [cur_l]

  ; handle start up loop management
  mov  eax, edx
  and  edx, 03fffffffh
  test eax, 080000000h
  jnz  whole_continue165
  test eax, 040000000h
  jnz  last_continue165

  ; Merge sample data loop
  ALIGN 16
  merge_loop165:
  mov   ebx, 65536
  sub   ebx, edx
  add   edx, [playback_ratio]

  ; weight the initial sample
  imul  ecx, ebx
  sar   ecx, 16
  cmp   edx, 65536*2
  jl    skip_loop165

  ; loop to load all of the full sample points
  whole_loop165:
  cmp   PTR_ESI, [src_end]
  jae   src_whole_exit165
  whole_continue165:

  ; Load sample data
  xor   eax, eax
  mov   al, byte ptr [PTR_ESI]
  sub   eax, 080h
  shl   eax, 8
  add   ecx, eax
  sub   edx, 65536
  inc   PTR_ESI
  cmp   edx, 65536*2
  jae   whole_loop165

  skip_loop165:
  and   edx, 0ffffh
  last_continue165:
  cmp   PTR_ESI, [src_end]
  jae   src_last_exit165

  ; Load sample data
  xor   eax, eax
  mov   al, byte ptr [PTR_ESI]
  sub   eax, 080h
  shl   eax, 8
  inc   PTR_ESI
  mov   ebx, ecx
  mov   ecx, eax

  ; weight the final sample
  imul  eax, edx
  sar   eax, 16
  add   eax, ebx
  imul  eax, [divider_l]

  ; Scale back down after divide
  sar   eax, 11

  ; Copy sample data into output buffer
  mov   [PTR_EDI], ax
  mov   [PTR_EDI+2], ax
  add   PTR_EDI, 4
  cmp   PTR_EDI, dest_end
  jb    merge_loop165

  ; Jump out point if end of dest is reached
  dest_end_exit165:
  jmp   src_save_value165

  ; jump out when src is exceed, but save our current loop position
  src_whole_exit165:
  or   edx, 080000000h
  src_last_exit165:
  or   edx, 040000000h
  src_save_value165:
  mov  [cur_l], ecx

  ; Routine end
  ret

Merge_DestStereo_SrcMono_Src8_Copy16_UpFiltered:

  ; adjust fractional
  shl   edx, 16

  ; Load sample data
  xor   eax, eax
  mov   al, byte ptr [PTR_ESI]
  sub   eax, 080h
  shl   eax, 8

  ; rotate filter values
  mov   ecx, [cur_l]
  mov   [cur_l], eax

  ; Merge sample data loop
  ALIGN 16
  merge_loop677:

  ; Upsample the data points
  mov   ebx, edx
  mov   eax, [cur_l]
  shr   ebx, 17
  sub   eax, ecx
  imul  eax, ebx
  sar   eax, 15
  add   eax, ecx

  ; Copy sample data into output buffer
  mov   [PTR_EDI], ax
  mov   [PTR_EDI+2], ax
  add   PTR_EDI, 4
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit677

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  jnc   merge_loop677

  ; Move the source pointer
  inc   PTR_ESI
  cmp   PTR_ESI, src_end
  jae   src_end_exit677

  ; Load sample data
  xor   eax, eax
  mov   al, byte ptr [PTR_ESI]
  sub   eax, 080h
  shl   eax, 8

  ; rotate filter values
  mov   ecx, [cur_l]
  mov   [cur_l], eax

  ; End loop
  jmp   merge_loop677

  ; Jump out point if end of dest is reached
  dest_end_exit677:
  inc   PTR_ESI
  add   edx, [step_fract]
  jc    skip_filter_adjust677

  ; rotate filter values
  mov   [cur_l], ecx

  ; un-increment the source to skip the early source adjustment
  dec   PTR_ESI

  skip_filter_adjust677:

  ; Jump out point if end of src is reached
  src_end_exit677:

  ; adjust fractional
  shr   edx, 16

  ; Routine end
  ret

Merge_DestStereo_SrcMono_Src8_Copy16_Filtered ENDP


Merge_DestMono_SrcMono_Src16_Copy16_Filtered PROC ; 166

  ; check to see if we have to call the upsampling version
  cmp   [playback_ratio], 10000h
  jle   Merge_DestMono_SrcMono_Src16_Copy16_UpFiltered

  ; build average dividers
  mov   ecx, edx
  mov   ebx, [playback_ratio]
  xor   edx, edx
  mov   eax, 08000000h  ; 100000000f/32
  div   ebx
  dec   eax
  mov   [divider_l], eax
  mov   edx, ecx

  ; load initial sample
  mov   ecx, [cur_l]

  ; handle start up loop management
  mov  eax, edx
  and  edx, 03fffffffh
  test eax, 080000000h
  jnz  whole_continue166
  test eax, 040000000h
  jnz  last_continue166

  ; Merge sample data loop
  ALIGN 16
  merge_loop166:
  mov   ebx, 65536
  sub   ebx, edx
  add   edx, [playback_ratio]

  ; weight the initial sample
  imul  ecx, ebx
  sar   ecx, 16
  cmp   edx, 65536*2
  jl    skip_loop166

  ; loop to load all of the full sample points
  whole_loop166:
  cmp   PTR_ESI, [src_end]
  jae   src_whole_exit166
  whole_continue166:

  ; Load sample data
  movsx eax, word ptr [PTR_ESI]
  add   ecx, eax
  sub   edx, 65536
  add   PTR_ESI, 2
  cmp   edx, 65536*2
  jae   whole_loop166

  skip_loop166:
  and   edx, 0ffffh
  last_continue166:
  cmp   PTR_ESI, [src_end]
  jae   src_last_exit166

  ; Load sample data
  movsx eax, word ptr [PTR_ESI]
  add   PTR_ESI, 2
  mov   ebx, ecx
  mov   ecx, eax

  ; weight the final sample
  imul  eax, edx
  sar   eax, 16
  add   eax, ebx
  imul  eax, [divider_l]

  ; Scale back down after divide
  sar   eax, 11

  ; Copy sample data into output buffer
  mov   [PTR_EDI], ax
  add   PTR_EDI, 2
  cmp   PTR_EDI, dest_end
  jb    merge_loop166

  ; Jump out point if end of dest is reached
  dest_end_exit166:
  jmp   src_save_value166

  ; jump out when src is exceed, but save our current loop position
  src_whole_exit166:
  or   edx, 080000000h
  src_last_exit166:
  or   edx, 040000000h
  src_save_value166:
  mov  [cur_l], ecx

  ; Routine end
  ret

Merge_DestMono_SrcMono_Src16_Copy16_UpFiltered:

  ; adjust fractional
  shl   edx, 16

  ; Load sample data
  movsx eax, word ptr [PTR_ESI]

  ; rotate filter values
  mov   ecx, [cur_l]
  mov   [cur_l], eax

  ; Merge sample data loop
  ALIGN 16
  merge_loop678:

  ; Upsample the data points
  mov   ebx, edx
  mov   eax, [cur_l]
  shr   ebx, 17
  sub   eax, ecx
  imul  eax, ebx
  sar   eax, 15
  add   eax, ecx

  ; Copy sample data into output buffer
  mov   [PTR_EDI], ax
  add   PTR_EDI, 2
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit678

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  jnc   merge_loop678

  ; Move the source pointer
  add   PTR_ESI, 2
  cmp   PTR_ESI, src_end
  jae   src_end_exit678

  ; Load sample data
  movsx eax, word ptr [PTR_ESI]

  ; rotate filter values
  mov   ecx, [cur_l]
  mov   [cur_l], eax

  ; End loop
  jmp   merge_loop678

  ; Jump out point if end of dest is reached
  dest_end_exit678:
  add   PTR_ESI, 2
  add   edx, [step_fract]
  jc    skip_filter_adjust678

  ; rotate filter values
  mov   [cur_l], ecx

  ; un-increment the source to skip the early source adjustment
  sub   PTR_ESI, 2

  skip_filter_adjust678:

  ; Jump out point if end of src is reached
  src_end_exit678:

  ; adjust fractional
  shr   edx, 16

  ; Routine end
  ret

Merge_DestMono_SrcMono_Src16_Copy16_Filtered ENDP


Merge_DestStereo_SrcMono_Src16_Copy16_Filtered PROC ; 167

  ; check to see if we have to call the upsampling version
  cmp   [playback_ratio], 10000h
  jle   Merge_DestStereo_SrcMono_Src16_Copy16_UpFiltered

  ; build average dividers
  mov   ecx, edx
  mov   ebx, [playback_ratio]
  xor   edx, edx
  mov   eax, 08000000h  ; 100000000f/32
  div   ebx
  dec   eax
  mov   [divider_l], eax
  mov   edx, ecx

  ; load initial sample
  mov   ecx, [cur_l]

  ; handle start up loop management
  mov  eax, edx
  and  edx, 03fffffffh
  test eax, 080000000h
  jnz  whole_continue167
  test eax, 040000000h
  jnz  last_continue167

  ; Merge sample data loop
  ALIGN 16
  merge_loop167:
  mov   ebx, 65536
  sub   ebx, edx
  add   edx, [playback_ratio]

  ; weight the initial sample
  imul  ecx, ebx
  sar   ecx, 16
  cmp   edx, 65536*2
  jl    skip_loop167

  ; loop to load all of the full sample points
  whole_loop167:
  cmp   PTR_ESI, [src_end]
  jae   src_whole_exit167
  whole_continue167:

  ; Load sample data
  movsx eax, word ptr [PTR_ESI]
  add   ecx, eax
  sub   edx, 65536
  add   PTR_ESI, 2
  cmp   edx, 65536*2
  jae   whole_loop167

  skip_loop167:
  and   edx, 0ffffh
  last_continue167:
  cmp   PTR_ESI, [src_end]
  jae   src_last_exit167

  ; Load sample data
  movsx eax, word ptr [PTR_ESI]
  add   PTR_ESI, 2
  mov   ebx, ecx
  mov   ecx, eax

  ; weight the final sample
  imul  eax, edx
  sar   eax, 16
  add   eax, ebx
  imul  eax, [divider_l]

  ; Scale back down after divide
  sar   eax, 11

  ; Copy sample data into output buffer
  mov   [PTR_EDI], ax
  mov   [PTR_EDI+2], ax
  add   PTR_EDI, 4
  cmp   PTR_EDI, dest_end
  jb    merge_loop167

  ; Jump out point if end of dest is reached
  dest_end_exit167:
  jmp   src_save_value167

  ; jump out when src is exceed, but save our current loop position
  src_whole_exit167:
  or   edx, 080000000h
  src_last_exit167:
  or   edx, 040000000h
  src_save_value167:
  mov  [cur_l], ecx

  ; Routine end
  ret

Merge_DestStereo_SrcMono_Src16_Copy16_UpFiltered:

  ; adjust fractional
  shl   edx, 16

  ; Load sample data
  movsx eax, word ptr [PTR_ESI]

  ; rotate filter values
  mov   ecx, [cur_l]
  mov   [cur_l], eax

  ; Merge sample data loop
  ALIGN 16
  merge_loop679:

  ; Upsample the data points
  mov   ebx, edx
  mov   eax, [cur_l]
  shr   ebx, 17
  sub   eax, ecx
  imul  eax, ebx
  sar   eax, 15
  add   eax, ecx

  ; Copy sample data into output buffer
  mov   [PTR_EDI], ax
  mov   [PTR_EDI+2], ax
  add   PTR_EDI, 4
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit679

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  jnc   merge_loop679

  ; Move the source pointer
  add   PTR_ESI, 2
  cmp   PTR_ESI, src_end
  jae   src_end_exit679

  ; Load sample data
  movsx eax, word ptr [PTR_ESI]

  ; rotate filter values
  mov   ecx, [cur_l]
  mov   [cur_l], eax

  ; End loop
  jmp   merge_loop679

  ; Jump out point if end of dest is reached
  dest_end_exit679:
  add   PTR_ESI, 2
  add   edx, [step_fract]
  jc    skip_filter_adjust679

  ; rotate filter values
  mov   [cur_l], ecx

  ; un-increment the source to skip the early source adjustment
  sub   PTR_ESI, 2

  skip_filter_adjust679:

  ; Jump out point if end of src is reached
  src_end_exit679:

  ; adjust fractional
  shr   edx, 16

  ; Routine end
  ret

Merge_DestStereo_SrcMono_Src16_Copy16_Filtered ENDP


Merge_DestMono_SrcStereo_Src8_Copy16_Resample PROC ; 168

  ; adjust fractional
  shl   edx, 16

  ; Load sample data
  xor   ebx, ebx
  xor   eax, eax
  mov   ax, word ptr [PTR_ESI]
  mov   bl, ah
  and   eax, 0ffh
  sub   ebx, 080h
  sub   eax, 080h
  shl   ebx, 8
  shl   eax, 8

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; Shift right to avoid clipping on direct copies
  sar   eax, 1

  ; Merge sample data loop
  ALIGN 16
  merge_loop168:

  ; Copy sample data into output buffer
  mov   [PTR_EDI], ax
  add   PTR_EDI, 2
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit168

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  sbb   PTR_EAX, PTR_EAX
  add   PTR_ESI, [STEPTBL_PTR][PTR_SIZE+PTR_EAX*PTR_SIZE]
  cmp   PTR_ESI, src_end
  jae   src_end_exit168

  ; Load sample data
  xor   ebx, ebx
  xor   eax, eax
  mov   ax, word ptr [PTR_ESI]
  mov   bl, ah
  and   eax, 0ffh
  sub   ebx, 080h
  sub   eax, 080h
  shl   ebx, 8
  shl   eax, 8

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; Shift right to avoid clipping on direct copies
  sar   eax, 1

  ; End loop
  jmp   merge_loop168

  ; Jump out point if end of dest is reached
  dest_end_exit168:

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  sbb   PTR_EAX, PTR_EAX
  add   PTR_ESI, [STEPTBL_PTR][PTR_SIZE+PTR_EAX*PTR_SIZE]

  ; Jump out point if end of src is reached
  src_end_exit168:

  ; adjust fractional
  shr   edx, 16

  ; Routine end
  ret

Merge_DestMono_SrcStereo_Src8_Copy16_Resample ENDP


Merge_DestStereo_SrcStereo_Src8_Copy16_Resample PROC ; 169

  ; adjust fractional
  shl   edx, 16

  ; Load sample data
  xor   ebx, ebx
  xor   eax, eax
  mov   ax, word ptr [PTR_ESI]
  mov   bl, ah
  and   eax, 0ffh
  sub   ebx, 080h
  sub   eax, 080h
  shl   ebx, 8
  shl   eax, 8

  ; Merge sample data loop
  ALIGN 16
  merge_loop169:

  ; Copy sample data into output buffer
  mov   [PTR_EDI], ax
  mov   [PTR_EDI+2], bx
  add   PTR_EDI, 4
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit169

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  sbb   PTR_EAX, PTR_EAX
  add   PTR_ESI, [STEPTBL_PTR][PTR_SIZE+PTR_EAX*PTR_SIZE]
  cmp   PTR_ESI, src_end
  jae   src_end_exit169

  ; Load sample data
  xor   ebx, ebx
  xor   eax, eax
  mov   ax, word ptr [PTR_ESI]
  mov   bl, ah
  and   eax, 0ffh
  sub   ebx, 080h
  sub   eax, 080h
  shl   ebx, 8
  shl   eax, 8

  ; End loop
  jmp   merge_loop169

  ; Jump out point if end of dest is reached
  dest_end_exit169:

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  sbb   PTR_EAX, PTR_EAX
  add   PTR_ESI, [STEPTBL_PTR][PTR_SIZE+PTR_EAX*PTR_SIZE]

  ; Jump out point if end of src is reached
  src_end_exit169:

  ; adjust fractional
  shr   edx, 16

  ; Routine end
  ret

Merge_DestStereo_SrcStereo_Src8_Copy16_Resample ENDP


Merge_DestMono_SrcStereo_Src16_Copy16_Resample PROC ; 170

  ; adjust fractional
  shl   edx, 16

  ; Load sample data
  mov   ebx, dword ptr [PTR_ESI]
  movsx eax, bx
  sar   ebx, 16

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; Shift right to avoid clipping on direct copies
  sar   eax, 1

  ; Merge sample data loop
  ALIGN 16
  merge_loop170:

  ; Copy sample data into output buffer
  mov   [PTR_EDI], ax
  add   PTR_EDI, 2
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit170

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  sbb   PTR_EAX, PTR_EAX
  add   PTR_ESI, [STEPTBL_PTR][PTR_SIZE+PTR_EAX*PTR_SIZE]
  cmp   PTR_ESI, src_end
  jae   src_end_exit170

  ; Load sample data
  mov   ebx, dword ptr [PTR_ESI]
  movsx eax, bx
  sar   ebx, 16

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; Shift right to avoid clipping on direct copies
  sar   eax, 1

  ; End loop
  jmp   merge_loop170

  ; Jump out point if end of dest is reached
  dest_end_exit170:

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  sbb   PTR_EAX, PTR_EAX
  add   PTR_ESI, [STEPTBL_PTR][PTR_SIZE+PTR_EAX*PTR_SIZE]

  ; Jump out point if end of src is reached
  src_end_exit170:

  ; adjust fractional
  shr   edx, 16

  ; Routine end
  ret

Merge_DestMono_SrcStereo_Src16_Copy16_Resample ENDP


Merge_DestStereo_SrcStereo_Src16_Copy16_Resample PROC ; 171

  ; adjust fractional
  shl   edx, 16

  ; Load sample data
  mov   ebx, dword ptr [PTR_ESI]
  movsx eax, bx
  sar   ebx, 16

  ; Merge sample data loop
  ALIGN 16
  merge_loop171:

  ; Copy sample data into output buffer
  mov   [PTR_EDI], ax
  mov   [PTR_EDI+2], bx
  add   PTR_EDI, 4
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit171

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  sbb   PTR_EAX, PTR_EAX
  add   PTR_ESI, [STEPTBL_PTR][PTR_SIZE+PTR_EAX*PTR_SIZE]
  cmp   PTR_ESI, src_end
  jae   src_end_exit171

  ; Load sample data
  mov   ebx, dword ptr [PTR_ESI]
  movsx eax, bx
  sar   ebx, 16

  ; End loop
  jmp   merge_loop171

  ; Jump out point if end of dest is reached
  dest_end_exit171:

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  sbb   PTR_EAX, PTR_EAX
  add   PTR_ESI, [STEPTBL_PTR][PTR_SIZE+PTR_EAX*PTR_SIZE]

  ; Jump out point if end of src is reached
  src_end_exit171:

  ; adjust fractional
  shr   edx, 16

  ; Routine end
  ret

Merge_DestStereo_SrcStereo_Src16_Copy16_Resample ENDP


Merge_DestMono_SrcStereo_Src8_Copy16_Filtered PROC ; 172

  ; check to see if we have to call the upsampling version
  cmp   [playback_ratio], 10000h
  jle   Merge_DestMono_SrcStereo_Src8_Copy16_UpFiltered

  ; build average dividers
  mov   ecx, edx
  mov   ebx, [playback_ratio]
  xor   edx, edx
  mov   eax, 08000000h  ; 100000000f/32
  div   ebx
  dec   eax
  mov   [divider_l], eax
  mov   edx, ecx

  ; load initial sample
  mov   ecx, [cur_l]

  ; handle start up loop management
  mov  eax, edx
  and  edx, 03fffffffh
  test eax, 080000000h
  jnz  whole_continue172
  test eax, 040000000h
  jnz  last_continue172

  ; Merge sample data loop
  ALIGN 16
  merge_loop172:
  mov   ebx, 65536
  sub   ebx, edx
  add   edx, [playback_ratio]

  ; weight the initial sample
  imul  ecx, ebx
  sar   ecx, 16
  cmp   edx, 65536*2
  jl    skip_loop172

  ; loop to load all of the full sample points
  whole_loop172:
  cmp   PTR_ESI, [src_end]
  jae   src_whole_exit172
  whole_continue172:

  ; Load sample data
  xor   ebx, ebx
  xor   eax, eax
  mov   ax, word ptr [PTR_ESI]
  mov   bl, ah
  and   eax, 0ffh
  sub   ebx, 080h
  sub   eax, 080h
  shl   ebx, 8
  shl   eax, 8

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; Shift right to avoid clipping on direct copies
  sar   eax, 1
  add   ecx, eax
  sub   edx, 65536
  add   PTR_ESI, 2
  cmp   edx, 65536*2
  jae   whole_loop172

  skip_loop172:
  and   edx, 0ffffh
  last_continue172:
  cmp   PTR_ESI, [src_end]
  jae   src_last_exit172

  ; Load sample data
  xor   ebx, ebx
  xor   eax, eax
  mov   ax, word ptr [PTR_ESI]
  mov   bl, ah
  and   eax, 0ffh
  sub   ebx, 080h
  sub   eax, 080h
  shl   ebx, 8
  shl   eax, 8

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; Shift right to avoid clipping on direct copies
  sar   eax, 1
  add   PTR_ESI, 2
  mov   ebx, ecx
  mov   ecx, eax

  ; weight the final sample
  imul  eax, edx
  sar   eax, 16
  add   eax, ebx
  imul  eax, [divider_l]

  ; Scale back down after divide
  sar   eax, 11

  ; Copy sample data into output buffer
  mov   [PTR_EDI], ax
  add   PTR_EDI, 2
  cmp   PTR_EDI, dest_end
  jb    merge_loop172

  ; Jump out point if end of dest is reached
  dest_end_exit172:
  jmp   src_save_value172

  ; jump out when src is exceed, but save our current loop position
  src_whole_exit172:
  or   edx, 080000000h
  src_last_exit172:
  or   edx, 040000000h
  src_save_value172:
  mov  [cur_l], ecx

  ; Routine end
  ret

Merge_DestMono_SrcStereo_Src8_Copy16_UpFiltered:

  ; adjust fractional
  shl   edx, 16

  ; Load sample data
  xor   ebx, ebx
  xor   eax, eax
  mov   ax, word ptr [PTR_ESI]
  mov   bl, ah
  and   eax, 0ffh
  sub   ebx, 080h
  sub   eax, 080h
  shl   ebx, 8
  shl   eax, 8

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; Shift right to avoid clipping on direct copies
  sar   eax, 1

  ; rotate filter values
  mov   ecx, [cur_l]
  mov   [cur_l], eax

  ; Merge sample data loop
  ALIGN 16
  merge_loop684:

  ; Upsample the data points
  mov   ebx, edx
  mov   eax, [cur_l]
  shr   ebx, 17
  sub   eax, ecx
  imul  eax, ebx
  sar   eax, 15
  add   eax, ecx

  ; Copy sample data into output buffer
  mov   [PTR_EDI], ax
  add   PTR_EDI, 2
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit684

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  jnc   merge_loop684

  ; Move the source pointer
  add   PTR_ESI, 2
  cmp   PTR_ESI, src_end
  jae   src_end_exit684

  ; Load sample data
  xor   ebx, ebx
  xor   eax, eax
  mov   ax, word ptr [PTR_ESI]
  mov   bl, ah
  and   eax, 0ffh
  sub   ebx, 080h
  sub   eax, 080h
  shl   ebx, 8
  shl   eax, 8

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; Shift right to avoid clipping on direct copies
  sar   eax, 1

  ; rotate filter values
  mov   ecx, [cur_l]
  mov   [cur_l], eax

  ; End loop
  jmp   merge_loop684

  ; Jump out point if end of dest is reached
  dest_end_exit684:
  add   PTR_ESI, 2
  add   edx, [step_fract]
  jc    skip_filter_adjust684

  ; rotate filter values
  mov   [cur_l], ecx

  ; un-increment the source to skip the early source adjustment
  sub   PTR_ESI, 2

  skip_filter_adjust684:

  ; Jump out point if end of src is reached
  src_end_exit684:

  ; adjust fractional
  shr   edx, 16

  ; Routine end
  ret

Merge_DestMono_SrcStereo_Src8_Copy16_Filtered ENDP


Merge_DestStereo_SrcStereo_Src8_Copy16_Filtered PROC ; 173

  ; check to see if we have to call the upsampling version
  cmp   [playback_ratio], 10000h
  jle   Merge_DestStereo_SrcStereo_Src8_Copy16_UpFiltered

  ; build average dividers
  mov   ecx, edx
  mov   ebx, [playback_ratio]
  xor   edx, edx
  mov   eax, 08000000h  ; 100000000f/32
  div   ebx
  dec   eax
  mov   [divider_l], eax
  xor   edx, edx
  mov   eax, 08000000h  ; 100000000f/32
  div   ebx
  dec   eax
  mov   [divider_r], eax
  mov   edx, ecx

  ; load initial sample
  mov   ecx, [cur_l]

  ; Save registers
  push  PTR_EBP

  ; handle start up loop management
  mov  eax, edx
  and  edx, 03fffffffh
  test eax, 080000000h
  jnz  whole_continue173
  test eax, 040000000h
  jnz  last_continue173

  ; Merge sample data loop
  ALIGN 16
  merge_loop173:
  mov   ebx, 65536
  sub   ebx, edx
  add   edx, [playback_ratio]

  ; weight the initial sample
  mov   eax, [cur_r]
  imul  ecx, ebx
  imul  eax, ebx
  sar   ecx, 16
  sar   eax, 16
  cmp   edx, 65536*2
  mov   [cur_r], eax
  jl    skip_loop173

  ; loop to load all of the full sample points
  whole_loop173:
  cmp   PTR_ESI, [src_end]
  jae   src_whole_exit173
  whole_continue173:

  ; Load sample data
  xor   ebx, ebx
  xor   eax, eax
  mov   ax, word ptr [PTR_ESI]
  mov   bl, ah
  and   eax, 0ffh
  sub   ebx, 080h
  sub   eax, 080h
  shl   ebx, 8
  shl   eax, 8
  add   ecx, eax
  add   [cur_r], ebx
  sub   edx, 65536
  add   PTR_ESI, 2
  cmp   edx, 65536*2
  jae   whole_loop173

  skip_loop173:
  and   edx, 0ffffh
  cmp   PTR_ESI, [src_end]
  jae   src_last_exit173
  last_continue173:

  ; Load sample data
  xor   ebx, ebx
  xor   eax, eax
  mov   ax, word ptr [PTR_ESI]
  mov   bl, ah
  and   eax, 0ffh
  sub   ebx, 080h
  sub   eax, 080h
  shl   ebx, 8
  shl   eax, 8
  add   PTR_ESI, 2
  mov   ebp, ecx
  mov   ecx, eax

  ; weight the final sample
  imul  eax, edx
  sar   eax, 16
  add   eax, ebp
  mov   ebp, [cur_r]
  mov   [cur_r], ebx
  imul  ebx, edx
  sar   ebx, 16
  add   ebx, ebp
  imul  eax, [divider_l]
  imul  ebx, [divider_r]

  ; Scale back down after divide
  sar   eax, 11
  sar   ebx, 11

  ; Copy sample data into output buffer
  mov   [PTR_EDI], ax
  mov   [PTR_EDI+2], bx
  add   PTR_EDI, 4
  cmp   PTR_EDI, dest_end
  jb    merge_loop173

  ; Jump out point if end of dest is reached
  dest_end_exit173:
  jmp   src_save_value173

  ; jump out when src is exceed, but save our current loop position
  src_whole_exit173:
  or   edx, 080000000h
  src_last_exit173:
  or   edx, 040000000h
  src_save_value173:
  mov  [cur_l], ecx

  ; Restore registers
  pop   PTR_EBP

  ; Routine end
  ret

Merge_DestStereo_SrcStereo_Src8_Copy16_UpFiltered:

  ; adjust fractional
  shl   edx, 16

  ; Save registers
  push  PTR_EBP

  ; Load sample data
  xor   ebx, ebx
  xor   eax, eax
  mov   ax, word ptr [PTR_ESI]
  mov   bl, ah
  and   eax, 0ffh
  sub   ebx, 080h
  sub   eax, 080h
  shl   ebx, 8
  shl   eax, 8

  ; rotate filter values
  mov   ecx, [cur_l]
  mov   [cur_l], eax
  mov   eax, [cur_r]
  mov   [cur_r], ebx
  mov   [cur_r2], eax

  ; Merge sample data loop
  ALIGN 16
  merge_loop685:

  ; Upsample the data points
  mov   ebp, edx
  mov   eax, [cur_l]
  mov   ebx, [cur_r]
  shr   ebp, 17
  sub   eax, ecx
  sub   ebx, [cur_r2]
  imul  eax, ebp
  imul  ebx, ebp
  sar   eax, 15
  sar   ebx, 15
  add   eax, ecx
  add   ebx, [cur_r2]

  ; Copy sample data into output buffer
  mov   [PTR_EDI], ax
  mov   [PTR_EDI+2], bx
  add   PTR_EDI, 4
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit685

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  jnc   merge_loop685

  ; Move the source pointer
  add   PTR_ESI, 2
  cmp   PTR_ESI, src_end
  jae   src_end_exit685

  ; Load sample data
  xor   ebx, ebx
  xor   eax, eax
  mov   ax, word ptr [PTR_ESI]
  mov   bl, ah
  and   eax, 0ffh
  sub   ebx, 080h
  sub   eax, 080h
  shl   ebx, 8
  shl   eax, 8

  ; rotate filter values
  mov   ecx, [cur_l]
  mov   [cur_l], eax
  mov   eax, [cur_r]
  mov   [cur_r], ebx
  mov   [cur_r2], eax

  ; End loop
  jmp   merge_loop685

  ; Jump out point if end of dest is reached
  dest_end_exit685:
  add   PTR_ESI, 2
  add   edx, [step_fract]
  jc    skip_filter_adjust685

  ; rotate filter values
  mov   [cur_l], ecx
  mov   ecx, [cur_r2]
  mov   [cur_r], ecx

  ; un-increment the source to skip the early source adjustment
  sub   PTR_ESI, 2

  skip_filter_adjust685:

  ; Jump out point if end of src is reached
  src_end_exit685:

  ; Restore registers
  pop   PTR_EBP

  ; adjust fractional
  shr   edx, 16

  ; Routine end
  ret

Merge_DestStereo_SrcStereo_Src8_Copy16_Filtered ENDP


Merge_DestMono_SrcStereo_Src16_Copy16_Filtered PROC ; 174

  ; check to see if we have to call the upsampling version
  cmp   [playback_ratio], 10000h
  jle   Merge_DestMono_SrcStereo_Src16_Copy16_UpFiltered

  ; build average dividers
  mov   ecx, edx
  mov   ebx, [playback_ratio]
  xor   edx, edx
  mov   eax, 08000000h  ; 100000000f/32
  div   ebx
  dec   eax
  mov   [divider_l], eax
  mov   edx, ecx

  ; load initial sample
  mov   ecx, [cur_l]

  ; handle start up loop management
  mov  eax, edx
  and  edx, 03fffffffh
  test eax, 080000000h
  jnz  whole_continue174
  test eax, 040000000h
  jnz  last_continue174

  ; Merge sample data loop
  ALIGN 16
  merge_loop174:
  mov   ebx, 65536
  sub   ebx, edx
  add   edx, [playback_ratio]

  ; weight the initial sample
  imul  ecx, ebx
  sar   ecx, 16
  cmp   edx, 65536*2
  jl    skip_loop174

  ; loop to load all of the full sample points
  whole_loop174:
  cmp   PTR_ESI, [src_end]
  jae   src_whole_exit174
  whole_continue174:

  ; Load sample data
  mov   ebx, dword ptr [PTR_ESI]
  movsx eax, bx
  sar   ebx, 16

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; Shift right to avoid clipping on direct copies
  sar   eax, 1
  add   ecx, eax
  sub   edx, 65536
  add   PTR_ESI, 4
  cmp   edx, 65536*2
  jae   whole_loop174

  skip_loop174:
  and   edx, 0ffffh
  last_continue174:
  cmp   PTR_ESI, [src_end]
  jae   src_last_exit174

  ; Load sample data
  mov   ebx, dword ptr [PTR_ESI]
  movsx eax, bx
  sar   ebx, 16

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; Shift right to avoid clipping on direct copies
  sar   eax, 1
  add   PTR_ESI, 4
  mov   ebx, ecx
  mov   ecx, eax

  ; weight the final sample
  imul  eax, edx
  sar   eax, 16
  add   eax, ebx
  imul  eax, [divider_l]

  ; Scale back down after divide
  sar   eax, 11

  ; Copy sample data into output buffer
  mov   [PTR_EDI], ax
  add   PTR_EDI, 2
  cmp   PTR_EDI, dest_end
  jb    merge_loop174

  ; Jump out point if end of dest is reached
  dest_end_exit174:
  jmp   src_save_value174

  ; jump out when src is exceed, but save our current loop position
  src_whole_exit174:
  or   edx, 080000000h
  src_last_exit174:
  or   edx, 040000000h
  src_save_value174:
  mov  [cur_l], ecx

  ; Routine end
  ret

Merge_DestMono_SrcStereo_Src16_Copy16_UpFiltered:

  ; adjust fractional
  shl   edx, 16

  ; Load sample data
  mov   ebx, dword ptr [PTR_ESI]
  movsx eax, bx
  sar   ebx, 16

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; Shift right to avoid clipping on direct copies
  sar   eax, 1

  ; rotate filter values
  mov   ecx, [cur_l]
  mov   [cur_l], eax

  ; Merge sample data loop
  ALIGN 16
  merge_loop686:

  ; Upsample the data points
  mov   ebx, edx
  mov   eax, [cur_l]
  shr   ebx, 17
  sub   eax, ecx
  imul  eax, ebx
  sar   eax, 15
  add   eax, ecx

  ; Copy sample data into output buffer
  mov   [PTR_EDI], ax
  add   PTR_EDI, 2
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit686

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  jnc   merge_loop686

  ; Move the source pointer
  add   PTR_ESI, 4
  cmp   PTR_ESI, src_end
  jae   src_end_exit686

  ; Load sample data
  mov   ebx, dword ptr [PTR_ESI]
  movsx eax, bx
  sar   ebx, 16

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; Shift right to avoid clipping on direct copies
  sar   eax, 1

  ; rotate filter values
  mov   ecx, [cur_l]
  mov   [cur_l], eax

  ; End loop
  jmp   merge_loop686

  ; Jump out point if end of dest is reached
  dest_end_exit686:
  add   PTR_ESI, 4
  add   edx, [step_fract]
  jc    skip_filter_adjust686

  ; rotate filter values
  mov   [cur_l], ecx

  ; un-increment the source to skip the early source adjustment
  sub   PTR_ESI, 4

  skip_filter_adjust686:

  ; Jump out point if end of src is reached
  src_end_exit686:

  ; adjust fractional
  shr   edx, 16

  ; Routine end
  ret

Merge_DestMono_SrcStereo_Src16_Copy16_Filtered ENDP


Merge_DestStereo_SrcStereo_Src16_Copy16_Filtered PROC ; 175

  ; check to see if we have to call the upsampling version
  cmp   [playback_ratio], 10000h
  jle   Merge_DestStereo_SrcStereo_Src16_Copy16_UpFiltered

  ; build average dividers
  mov   ecx, edx
  mov   ebx, [playback_ratio]
  xor   edx, edx
  mov   eax, 08000000h  ; 100000000f/32
  div   ebx
  dec   eax
  mov   [divider_l], eax
  xor   edx, edx
  mov   eax, 08000000h  ; 100000000f/32
  div   ebx
  dec   eax
  mov   [divider_r], eax
  mov   edx, ecx

  ; load initial sample
  mov   ecx, [cur_l]

  ; Save registers
  push  PTR_EBP

  ; handle start up loop management
  mov  eax, edx
  and  edx, 03fffffffh
  test eax, 080000000h
  jnz  whole_continue175
  test eax, 040000000h
  jnz  last_continue175

  ; Merge sample data loop
  ALIGN 16
  merge_loop175:
  mov   ebx, 65536
  sub   ebx, edx
  add   edx, [playback_ratio]

  ; weight the initial sample
  mov   eax, [cur_r]
  imul  ecx, ebx
  imul  eax, ebx
  sar   ecx, 16
  sar   eax, 16
  cmp   edx, 65536*2
  mov   [cur_r], eax
  jl    skip_loop175

  ; loop to load all of the full sample points
  whole_loop175:
  cmp   PTR_ESI, [src_end]
  jae   src_whole_exit175
  whole_continue175:

  ; Load sample data
  mov   ebx, dword ptr [PTR_ESI]
  movsx eax, bx
  sar   ebx, 16
  add   ecx, eax
  add   [cur_r], ebx
  sub   edx, 65536
  add   PTR_ESI, 4
  cmp   edx, 65536*2
  jae   whole_loop175

  skip_loop175:
  and   edx, 0ffffh
  cmp   PTR_ESI, [src_end]
  jae   src_last_exit175
  last_continue175:

  ; Load sample data
  mov   ebx, dword ptr [PTR_ESI]
  movsx eax, bx
  sar   ebx, 16
  add   PTR_ESI, 4
  mov   ebp, ecx
  mov   ecx, eax

  ; weight the final sample
  imul  eax, edx
  sar   eax, 16
  add   eax, ebp
  mov   ebp, [cur_r]
  mov   [cur_r], ebx
  imul  ebx, edx
  sar   ebx, 16
  add   ebx, ebp
  imul  eax, [divider_l]
  imul  ebx, [divider_r]

  ; Scale back down after divide
  sar   eax, 11
  sar   ebx, 11

  ; Copy sample data into output buffer
  mov   [PTR_EDI], ax
  mov   [PTR_EDI+2], bx
  add   PTR_EDI, 4
  cmp   PTR_EDI, dest_end
  jb    merge_loop175

  ; Jump out point if end of dest is reached
  dest_end_exit175:
  jmp   src_save_value175

  ; jump out when src is exceed, but save our current loop position
  src_whole_exit175:
  or   edx, 080000000h
  src_last_exit175:
  or   edx, 040000000h
  src_save_value175:
  mov  [cur_l], ecx

  ; Restore registers
  pop   PTR_EBP

  ; Routine end
  ret

Merge_DestStereo_SrcStereo_Src16_Copy16_UpFiltered:

  ; adjust fractional
  shl   edx, 16

  ; Save registers
  push  PTR_EBP

  ; Load sample data
  mov   ebx, dword ptr [PTR_ESI]
  movsx eax, bx
  sar   ebx, 16

  ; rotate filter values
  mov   ecx, [cur_l]
  mov   [cur_l], eax
  mov   eax, [cur_r]
  mov   [cur_r], ebx
  mov   [cur_r2], eax

  ; Merge sample data loop
  ALIGN 16
  merge_loop687:

  ; Upsample the data points
  mov   ebp, edx
  mov   eax, [cur_l]
  mov   ebx, [cur_r]
  shr   ebp, 17
  sub   eax, ecx
  sub   ebx, [cur_r2]
  imul  eax, ebp
  imul  ebx, ebp
  sar   eax, 15
  sar   ebx, 15
  add   eax, ecx
  add   ebx, [cur_r2]

  ; Copy sample data into output buffer
  mov   [PTR_EDI], ax
  mov   [PTR_EDI+2], bx
  add   PTR_EDI, 4
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit687

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  jnc   merge_loop687

  ; Move the source pointer
  add   PTR_ESI, 4
  cmp   PTR_ESI, src_end
  jae   src_end_exit687

  ; Load sample data
  mov   ebx, dword ptr [PTR_ESI]
  movsx eax, bx
  sar   ebx, 16

  ; rotate filter values
  mov   ecx, [cur_l]
  mov   [cur_l], eax
  mov   eax, [cur_r]
  mov   [cur_r], ebx
  mov   [cur_r2], eax

  ; End loop
  jmp   merge_loop687

  ; Jump out point if end of dest is reached
  dest_end_exit687:
  add   PTR_ESI, 4
  add   edx, [step_fract]
  jc    skip_filter_adjust687

  ; rotate filter values
  mov   [cur_l], ecx
  mov   ecx, [cur_r2]
  mov   [cur_r], ecx

  ; un-increment the source to skip the early source adjustment
  sub   PTR_ESI, 4

  skip_filter_adjust687:

  ; Jump out point if end of src is reached
  src_end_exit687:

  ; Restore registers
  pop   PTR_EBP

  ; adjust fractional
  shr   edx, 16

  ; Routine end
  ret

Merge_DestStereo_SrcStereo_Src16_Copy16_Filtered ENDP


Merge_DestMono_SrcFlipped_Src8_Copy16_Resample PROC ; 184

  ; adjust fractional
  shl   edx, 16

  ; Load sample data
  xor   eax, eax
  xor   ebx, ebx
  mov   bx, word ptr [PTR_ESI]
  mov   al, bh
  and   ebx, 0ffh
  sub   eax, 080h
  sub   ebx, 080h
  shl   eax, 8
  shl   ebx, 8

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; Shift right to avoid clipping on direct copies
  sar   eax, 1

  ; Merge sample data loop
  ALIGN 16
  merge_loop184:

  ; Copy sample data into output buffer
  mov   [PTR_EDI], ax
  add   PTR_EDI, 2
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit184

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  sbb   PTR_EAX, PTR_EAX
  add   PTR_ESI, [STEPTBL_PTR][PTR_SIZE+PTR_EAX*PTR_SIZE]
  cmp   PTR_ESI, src_end
  jae   src_end_exit184

  ; Load sample data
  xor   eax, eax
  xor   ebx, ebx
  mov   bx, word ptr [PTR_ESI]
  mov   al, bh
  and   ebx, 0ffh
  sub   eax, 080h
  sub   ebx, 080h
  shl   eax, 8
  shl   ebx, 8

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; Shift right to avoid clipping on direct copies
  sar   eax, 1

  ; End loop
  jmp   merge_loop184

  ; Jump out point if end of dest is reached
  dest_end_exit184:

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  sbb   PTR_EAX, PTR_EAX
  add   PTR_ESI, [STEPTBL_PTR][PTR_SIZE+PTR_EAX*PTR_SIZE]

  ; Jump out point if end of src is reached
  src_end_exit184:

  ; adjust fractional
  shr   edx, 16

  ; Routine end
  ret

Merge_DestMono_SrcFlipped_Src8_Copy16_Resample ENDP


Merge_DestStereo_SrcFlipped_Src8_Copy16_Resample PROC ; 185

  ; adjust fractional
  shl   edx, 16

  ; Load sample data
  xor   eax, eax
  xor   ebx, ebx
  mov   bx, word ptr [PTR_ESI]
  mov   al, bh
  and   ebx, 0ffh
  sub   eax, 080h
  sub   ebx, 080h
  shl   eax, 8
  shl   ebx, 8

  ; Merge sample data loop
  ALIGN 16
  merge_loop185:

  ; Copy sample data into output buffer
  mov   [PTR_EDI], ax
  mov   [PTR_EDI+2], bx
  add   PTR_EDI, 4
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit185

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  sbb   PTR_EAX, PTR_EAX
  add   PTR_ESI, [STEPTBL_PTR][PTR_SIZE+PTR_EAX*PTR_SIZE]
  cmp   PTR_ESI, src_end
  jae   src_end_exit185

  ; Load sample data
  xor   eax, eax
  xor   ebx, ebx
  mov   bx, word ptr [PTR_ESI]
  mov   al, bh
  and   ebx, 0ffh
  sub   eax, 080h
  sub   ebx, 080h
  shl   eax, 8
  shl   ebx, 8

  ; End loop
  jmp   merge_loop185

  ; Jump out point if end of dest is reached
  dest_end_exit185:

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  sbb   PTR_EAX, PTR_EAX
  add   PTR_ESI, [STEPTBL_PTR][PTR_SIZE+PTR_EAX*PTR_SIZE]

  ; Jump out point if end of src is reached
  src_end_exit185:

  ; adjust fractional
  shr   edx, 16

  ; Routine end
  ret

Merge_DestStereo_SrcFlipped_Src8_Copy16_Resample ENDP


Merge_DestMono_SrcFlipped_Src16_Copy16_Resample PROC ; 186

  ; adjust fractional
  shl   edx, 16

  ; Load sample data
  mov   eax, dword ptr [PTR_ESI]
  movsx ebx, ax
  sar   eax, 16

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; Shift right to avoid clipping on direct copies
  sar   eax, 1

  ; Merge sample data loop
  ALIGN 16
  merge_loop186:

  ; Copy sample data into output buffer
  mov   [PTR_EDI], ax
  add   PTR_EDI, 2
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit186

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  sbb   PTR_EAX, PTR_EAX
  add   PTR_ESI, [STEPTBL_PTR][PTR_SIZE+PTR_EAX*PTR_SIZE]
  cmp   PTR_ESI, src_end
  jae   src_end_exit186

  ; Load sample data
  mov   eax, dword ptr [PTR_ESI]
  movsx ebx, ax
  sar   eax, 16

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; Shift right to avoid clipping on direct copies
  sar   eax, 1

  ; End loop
  jmp   merge_loop186

  ; Jump out point if end of dest is reached
  dest_end_exit186:

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  sbb   PTR_EAX, PTR_EAX
  add   PTR_ESI, [STEPTBL_PTR][PTR_SIZE+PTR_EAX*PTR_SIZE]

  ; Jump out point if end of src is reached
  src_end_exit186:

  ; adjust fractional
  shr   edx, 16

  ; Routine end
  ret

Merge_DestMono_SrcFlipped_Src16_Copy16_Resample ENDP


Merge_DestStereo_SrcFlipped_Src16_Copy16_Resample PROC ; 187

  ; adjust fractional
  shl   edx, 16

  ; Load sample data
  mov   eax, dword ptr [PTR_ESI]
  movsx ebx, ax
  sar   eax, 16

  ; Merge sample data loop
  ALIGN 16
  merge_loop187:

  ; Copy sample data into output buffer
  mov   [PTR_EDI], ax
  mov   [PTR_EDI+2], bx
  add   PTR_EDI, 4
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit187

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  sbb   PTR_EAX, PTR_EAX
  add   PTR_ESI, [STEPTBL_PTR][PTR_SIZE+PTR_EAX*PTR_SIZE]
  cmp   PTR_ESI, src_end
  jae   src_end_exit187

  ; Load sample data
  mov   eax, dword ptr [PTR_ESI]
  movsx ebx, ax
  sar   eax, 16

  ; End loop
  jmp   merge_loop187

  ; Jump out point if end of dest is reached
  dest_end_exit187:

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  sbb   PTR_EAX, PTR_EAX
  add   PTR_ESI, [STEPTBL_PTR][PTR_SIZE+PTR_EAX*PTR_SIZE]

  ; Jump out point if end of src is reached
  src_end_exit187:

  ; adjust fractional
  shr   edx, 16

  ; Routine end
  ret

Merge_DestStereo_SrcFlipped_Src16_Copy16_Resample ENDP


Merge_DestMono_SrcFlipped_Src8_Copy16_Filtered PROC ; 188

  ; check to see if we have to call the upsampling version
  cmp   [playback_ratio], 10000h
  jle   Merge_DestMono_SrcFlipped_Src8_Copy16_UpFiltered

  ; build average dividers
  mov   ecx, edx
  mov   ebx, [playback_ratio]
  xor   edx, edx
  mov   eax, 08000000h  ; 100000000f/32
  div   ebx
  dec   eax
  mov   [divider_l], eax
  mov   edx, ecx

  ; load initial sample
  mov   ecx, [cur_l]

  ; handle start up loop management
  mov  eax, edx
  and  edx, 03fffffffh
  test eax, 080000000h
  jnz  whole_continue188
  test eax, 040000000h
  jnz  last_continue188

  ; Merge sample data loop
  ALIGN 16
  merge_loop188:
  mov   ebx, 65536
  sub   ebx, edx
  add   edx, [playback_ratio]

  ; weight the initial sample
  imul  ecx, ebx
  sar   ecx, 16
  cmp   edx, 65536*2
  jl    skip_loop188

  ; loop to load all of the full sample points
  whole_loop188:
  cmp   PTR_ESI, [src_end]
  jae   src_whole_exit188
  whole_continue188:

  ; Load sample data
  xor   eax, eax
  xor   ebx, ebx
  mov   bx, word ptr [PTR_ESI]
  mov   al, bh
  and   ebx, 0ffh
  sub   eax, 080h
  sub   ebx, 080h
  shl   eax, 8
  shl   ebx, 8

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; Shift right to avoid clipping on direct copies
  sar   eax, 1
  add   ecx, eax
  sub   edx, 65536
  add   PTR_ESI, 2
  cmp   edx, 65536*2
  jae   whole_loop188

  skip_loop188:
  and   edx, 0ffffh
  last_continue188:
  cmp   PTR_ESI, [src_end]
  jae   src_last_exit188

  ; Load sample data
  xor   eax, eax
  xor   ebx, ebx
  mov   bx, word ptr [PTR_ESI]
  mov   al, bh
  and   ebx, 0ffh
  sub   eax, 080h
  sub   ebx, 080h
  shl   eax, 8
  shl   ebx, 8

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; Shift right to avoid clipping on direct copies
  sar   eax, 1
  add   PTR_ESI, 2
  mov   ebx, ecx
  mov   ecx, eax

  ; weight the final sample
  imul  eax, edx
  sar   eax, 16
  add   eax, ebx
  imul  eax, [divider_l]

  ; Scale back down after divide
  sar   eax, 11

  ; Copy sample data into output buffer
  mov   [PTR_EDI], ax
  add   PTR_EDI, 2
  cmp   PTR_EDI, dest_end
  jb    merge_loop188

  ; Jump out point if end of dest is reached
  dest_end_exit188:
  jmp   src_save_value188

  ; jump out when src is exceed, but save our current loop position
  src_whole_exit188:
  or   edx, 080000000h
  src_last_exit188:
  or   edx, 040000000h
  src_save_value188:
  mov  [cur_l], ecx

  ; Routine end
  ret

Merge_DestMono_SrcFlipped_Src8_Copy16_UpFiltered:

  ; adjust fractional
  shl   edx, 16

  ; Load sample data
  xor   eax, eax
  xor   ebx, ebx
  mov   bx, word ptr [PTR_ESI]
  mov   al, bh
  and   ebx, 0ffh
  sub   eax, 080h
  sub   ebx, 080h
  shl   eax, 8
  shl   ebx, 8

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; Shift right to avoid clipping on direct copies
  sar   eax, 1

  ; rotate filter values
  mov   ecx, [cur_l]
  mov   [cur_l], eax

  ; Merge sample data loop
  ALIGN 16
  merge_loop700:

  ; Upsample the data points
  mov   ebx, edx
  mov   eax, [cur_l]
  shr   ebx, 17
  sub   eax, ecx
  imul  eax, ebx
  sar   eax, 15
  add   eax, ecx

  ; Copy sample data into output buffer
  mov   [PTR_EDI], ax
  add   PTR_EDI, 2
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit700

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  jnc   merge_loop700

  ; Move the source pointer
  add   PTR_ESI, 2
  cmp   PTR_ESI, src_end
  jae   src_end_exit700

  ; Load sample data
  xor   eax, eax
  xor   ebx, ebx
  mov   bx, word ptr [PTR_ESI]
  mov   al, bh
  and   ebx, 0ffh
  sub   eax, 080h
  sub   ebx, 080h
  shl   eax, 8
  shl   ebx, 8

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; Shift right to avoid clipping on direct copies
  sar   eax, 1

  ; rotate filter values
  mov   ecx, [cur_l]
  mov   [cur_l], eax

  ; End loop
  jmp   merge_loop700

  ; Jump out point if end of dest is reached
  dest_end_exit700:
  add   PTR_ESI, 2
  add   edx, [step_fract]
  jc    skip_filter_adjust700

  ; rotate filter values
  mov   [cur_l], ecx

  ; un-increment the source to skip the early source adjustment
  sub   PTR_ESI, 2

  skip_filter_adjust700:

  ; Jump out point if end of src is reached
  src_end_exit700:

  ; adjust fractional
  shr   edx, 16

  ; Routine end
  ret

Merge_DestMono_SrcFlipped_Src8_Copy16_Filtered ENDP


Merge_DestStereo_SrcFlipped_Src8_Copy16_Filtered PROC ; 189

  ; check to see if we have to call the upsampling version
  cmp   [playback_ratio], 10000h
  jle   Merge_DestStereo_SrcFlipped_Src8_Copy16_UpFiltered

  ; build average dividers
  mov   ecx, edx
  mov   ebx, [playback_ratio]
  xor   edx, edx
  mov   eax, 08000000h  ; 100000000f/32
  div   ebx
  dec   eax
  mov   [divider_l], eax
  xor   edx, edx
  mov   eax, 08000000h  ; 100000000f/32
  div   ebx
  dec   eax
  mov   [divider_r], eax
  mov   edx, ecx

  ; load initial sample
  mov   ecx, [cur_l]

  ; Save registers
  push  PTR_EBP

  ; handle start up loop management
  mov  eax, edx
  and  edx, 03fffffffh
  test eax, 080000000h
  jnz  whole_continue189
  test eax, 040000000h
  jnz  last_continue189

  ; Merge sample data loop
  ALIGN 16
  merge_loop189:
  mov   ebx, 65536
  sub   ebx, edx
  add   edx, [playback_ratio]

  ; weight the initial sample
  mov   eax, [cur_r]
  imul  ecx, ebx
  imul  eax, ebx
  sar   ecx, 16
  sar   eax, 16
  cmp   edx, 65536*2
  mov   [cur_r], eax
  jl    skip_loop189

  ; loop to load all of the full sample points
  whole_loop189:
  cmp   PTR_ESI, [src_end]
  jae   src_whole_exit189
  whole_continue189:

  ; Load sample data
  xor   eax, eax
  xor   ebx, ebx
  mov   bx, word ptr [PTR_ESI]
  mov   al, bh
  and   ebx, 0ffh
  sub   eax, 080h
  sub   ebx, 080h
  shl   eax, 8
  shl   ebx, 8
  add   ecx, eax
  add   [cur_r], ebx
  sub   edx, 65536
  add   PTR_ESI, 2
  cmp   edx, 65536*2
  jae   whole_loop189

  skip_loop189:
  and   edx, 0ffffh
  cmp   PTR_ESI, [src_end]
  jae   src_last_exit189
  last_continue189:

  ; Load sample data
  xor   eax, eax
  xor   ebx, ebx
  mov   bx, word ptr [PTR_ESI]
  mov   al, bh
  and   ebx, 0ffh
  sub   eax, 080h
  sub   ebx, 080h
  shl   eax, 8
  shl   ebx, 8
  add   PTR_ESI, 2
  mov   ebp, ecx
  mov   ecx, eax

  ; weight the final sample
  imul  eax, edx
  sar   eax, 16
  add   eax, ebp
  mov   ebp, [cur_r]
  mov   [cur_r], ebx
  imul  ebx, edx
  sar   ebx, 16
  add   ebx, ebp
  imul  eax, [divider_l]
  imul  ebx, [divider_r]

  ; Scale back down after divide
  sar   eax, 11
  sar   ebx, 11

  ; Copy sample data into output buffer
  mov   [PTR_EDI], ax
  mov   [PTR_EDI+2], bx
  add   PTR_EDI, 4
  cmp   PTR_EDI, dest_end
  jb    merge_loop189

  ; Jump out point if end of dest is reached
  dest_end_exit189:
  jmp   src_save_value189

  ; jump out when src is exceed, but save our current loop position
  src_whole_exit189:
  or   edx, 080000000h
  src_last_exit189:
  or   edx, 040000000h
  src_save_value189:
  mov  [cur_l], ecx

  ; Restore registers
  pop   PTR_EBP

  ; Routine end
  ret

Merge_DestStereo_SrcFlipped_Src8_Copy16_UpFiltered:

  ; adjust fractional
  shl   edx, 16

  ; Save registers
  push  PTR_EBP

  ; Load sample data
  xor   eax, eax
  xor   ebx, ebx
  mov   bx, word ptr [PTR_ESI]
  mov   al, bh
  and   ebx, 0ffh
  sub   eax, 080h
  sub   ebx, 080h
  shl   eax, 8
  shl   ebx, 8

  ; rotate filter values
  mov   ecx, [cur_l]
  mov   [cur_l], eax
  mov   eax, [cur_r]
  mov   [cur_r], ebx
  mov   [cur_r2], eax

  ; Merge sample data loop
  ALIGN 16
  merge_loop701:

  ; Upsample the data points
  mov   ebp, edx
  mov   eax, [cur_l]
  mov   ebx, [cur_r]
  shr   ebp, 17
  sub   eax, ecx
  sub   ebx, [cur_r2]
  imul  eax, ebp
  imul  ebx, ebp
  sar   eax, 15
  sar   ebx, 15
  add   eax, ecx
  add   ebx, [cur_r2]

  ; Copy sample data into output buffer
  mov   [PTR_EDI], ax
  mov   [PTR_EDI+2], bx
  add   PTR_EDI, 4
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit701

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  jnc   merge_loop701

  ; Move the source pointer
  add   PTR_ESI, 2
  cmp   PTR_ESI, src_end
  jae   src_end_exit701

  ; Load sample data
  xor   eax, eax
  xor   ebx, ebx
  mov   bx, word ptr [PTR_ESI]
  mov   al, bh
  and   ebx, 0ffh
  sub   eax, 080h
  sub   ebx, 080h
  shl   eax, 8
  shl   ebx, 8

  ; rotate filter values
  mov   ecx, [cur_l]
  mov   [cur_l], eax
  mov   eax, [cur_r]
  mov   [cur_r], ebx
  mov   [cur_r2], eax

  ; End loop
  jmp   merge_loop701

  ; Jump out point if end of dest is reached
  dest_end_exit701:
  add   PTR_ESI, 2
  add   edx, [step_fract]
  jc    skip_filter_adjust701

  ; rotate filter values
  mov   [cur_l], ecx
  mov   ecx, [cur_r2]
  mov   [cur_r], ecx

  ; un-increment the source to skip the early source adjustment
  sub   PTR_ESI, 2

  skip_filter_adjust701:

  ; Jump out point if end of src is reached
  src_end_exit701:

  ; Restore registers
  pop   PTR_EBP

  ; adjust fractional
  shr   edx, 16

  ; Routine end
  ret

Merge_DestStereo_SrcFlipped_Src8_Copy16_Filtered ENDP


Merge_DestMono_SrcFlipped_Src16_Copy16_Filtered PROC ; 190

  ; check to see if we have to call the upsampling version
  cmp   [playback_ratio], 10000h
  jle   Merge_DestMono_SrcFlipped_Src16_Copy16_UpFiltered

  ; build average dividers
  mov   ecx, edx
  mov   ebx, [playback_ratio]
  xor   edx, edx
  mov   eax, 08000000h  ; 100000000f/32
  div   ebx
  dec   eax
  mov   [divider_l], eax
  mov   edx, ecx

  ; load initial sample
  mov   ecx, [cur_l]

  ; handle start up loop management
  mov  eax, edx
  and  edx, 03fffffffh
  test eax, 080000000h
  jnz  whole_continue190
  test eax, 040000000h
  jnz  last_continue190

  ; Merge sample data loop
  ALIGN 16
  merge_loop190:
  mov   ebx, 65536
  sub   ebx, edx
  add   edx, [playback_ratio]

  ; weight the initial sample
  imul  ecx, ebx
  sar   ecx, 16
  cmp   edx, 65536*2
  jl    skip_loop190

  ; loop to load all of the full sample points
  whole_loop190:
  cmp   PTR_ESI, [src_end]
  jae   src_whole_exit190
  whole_continue190:

  ; Load sample data
  mov   eax, dword ptr [PTR_ESI]
  movsx ebx, ax
  sar   eax, 16

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; Shift right to avoid clipping on direct copies
  sar   eax, 1
  add   ecx, eax
  sub   edx, 65536
  add   PTR_ESI, 4
  cmp   edx, 65536*2
  jae   whole_loop190

  skip_loop190:
  and   edx, 0ffffh
  last_continue190:
  cmp   PTR_ESI, [src_end]
  jae   src_last_exit190

  ; Load sample data
  mov   eax, dword ptr [PTR_ESI]
  movsx ebx, ax
  sar   eax, 16

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; Shift right to avoid clipping on direct copies
  sar   eax, 1
  add   PTR_ESI, 4
  mov   ebx, ecx
  mov   ecx, eax

  ; weight the final sample
  imul  eax, edx
  sar   eax, 16
  add   eax, ebx
  imul  eax, [divider_l]

  ; Scale back down after divide
  sar   eax, 11

  ; Copy sample data into output buffer
  mov   [PTR_EDI], ax
  add   PTR_EDI, 2
  cmp   PTR_EDI, dest_end
  jb    merge_loop190

  ; Jump out point if end of dest is reached
  dest_end_exit190:
  jmp   src_save_value190

  ; jump out when src is exceed, but save our current loop position
  src_whole_exit190:
  or   edx, 080000000h
  src_last_exit190:
  or   edx, 040000000h
  src_save_value190:
  mov  [cur_l], ecx

  ; Routine end
  ret

Merge_DestMono_SrcFlipped_Src16_Copy16_UpFiltered:

  ; adjust fractional
  shl   edx, 16

  ; Load sample data
  mov   eax, dword ptr [PTR_ESI]
  movsx ebx, ax
  sar   eax, 16

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; Shift right to avoid clipping on direct copies
  sar   eax, 1

  ; rotate filter values
  mov   ecx, [cur_l]
  mov   [cur_l], eax

  ; Merge sample data loop
  ALIGN 16
  merge_loop702:

  ; Upsample the data points
  mov   ebx, edx
  mov   eax, [cur_l]
  shr   ebx, 17
  sub   eax, ecx
  imul  eax, ebx
  sar   eax, 15
  add   eax, ecx

  ; Copy sample data into output buffer
  mov   [PTR_EDI], ax
  add   PTR_EDI, 2
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit702

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  jnc   merge_loop702

  ; Move the source pointer
  add   PTR_ESI, 4
  cmp   PTR_ESI, src_end
  jae   src_end_exit702

  ; Load sample data
  mov   eax, dword ptr [PTR_ESI]
  movsx ebx, ax
  sar   eax, 16

  ; Merge left and right channels for mono dest
  add   eax, ebx

  ; Shift right to avoid clipping on direct copies
  sar   eax, 1

  ; rotate filter values
  mov   ecx, [cur_l]
  mov   [cur_l], eax

  ; End loop
  jmp   merge_loop702

  ; Jump out point if end of dest is reached
  dest_end_exit702:
  add   PTR_ESI, 4
  add   edx, [step_fract]
  jc    skip_filter_adjust702

  ; rotate filter values
  mov   [cur_l], ecx

  ; un-increment the source to skip the early source adjustment
  sub   PTR_ESI, 4

  skip_filter_adjust702:

  ; Jump out point if end of src is reached
  src_end_exit702:

  ; adjust fractional
  shr   edx, 16

  ; Routine end
  ret

Merge_DestMono_SrcFlipped_Src16_Copy16_Filtered ENDP


Merge_DestStereo_SrcFlipped_Src16_Copy16_Filtered PROC ; 191

  ; check to see if we have to call the upsampling version
  cmp   [playback_ratio], 10000h
  jle   Merge_DestStereo_SrcFlipped_Src16_Copy16_UpFiltered

  ; build average dividers
  mov   ecx, edx
  mov   ebx, [playback_ratio]
  xor   edx, edx
  mov   eax, 08000000h  ; 100000000f/32
  div   ebx
  dec   eax
  mov   [divider_l], eax
  xor   edx, edx
  mov   eax, 08000000h  ; 100000000f/32
  div   ebx
  dec   eax
  mov   [divider_r], eax
  mov   edx, ecx

  ; load initial sample
  mov   ecx, [cur_l]

  ; Save registers
  push  PTR_EBP

  ; handle start up loop management
  mov  eax, edx
  and  edx, 03fffffffh
  test eax, 080000000h
  jnz  whole_continue191
  test eax, 040000000h
  jnz  last_continue191

  ; Merge sample data loop
  ALIGN 16
  merge_loop191:
  mov   ebx, 65536
  sub   ebx, edx
  add   edx, [playback_ratio]

  ; weight the initial sample
  mov   eax, [cur_r]
  imul  ecx, ebx
  imul  eax, ebx
  sar   ecx, 16
  sar   eax, 16
  cmp   edx, 65536*2
  mov   [cur_r], eax
  jl    skip_loop191

  ; loop to load all of the full sample points
  whole_loop191:
  cmp   PTR_ESI, [src_end]
  jae   src_whole_exit191
  whole_continue191:

  ; Load sample data
  mov   eax, dword ptr [PTR_ESI]
  movsx ebx, ax
  sar   eax, 16
  add   ecx, eax
  add   [cur_r], ebx
  sub   edx, 65536
  add   PTR_ESI, 4
  cmp   edx, 65536*2
  jae   whole_loop191

  skip_loop191:
  and   edx, 0ffffh
  cmp   PTR_ESI, [src_end]
  jae   src_last_exit191
  last_continue191:

  ; Load sample data
  mov   eax, dword ptr [PTR_ESI]
  movsx ebx, ax
  sar   eax, 16
  add   PTR_ESI, 4
  mov   ebp, ecx
  mov   ecx, eax

  ; weight the final sample
  imul  eax, edx
  sar   eax, 16
  add   eax, ebp
  mov   ebp, [cur_r]
  mov   [cur_r], ebx
  imul  ebx, edx
  sar   ebx, 16
  add   ebx, ebp
  imul  eax, [divider_l]
  imul  ebx, [divider_r]

  ; Scale back down after divide
  sar   eax, 11
  sar   ebx, 11

  ; Copy sample data into output buffer
  mov   [PTR_EDI], ax
  mov   [PTR_EDI+2], bx
  add   PTR_EDI, 4
  cmp   PTR_EDI, dest_end
  jb    merge_loop191

  ; Jump out point if end of dest is reached
  dest_end_exit191:
  jmp   src_save_value191

  ; jump out when src is exceed, but save our current loop position
  src_whole_exit191:
  or   edx, 080000000h
  src_last_exit191:
  or   edx, 040000000h
  src_save_value191:
  mov  [cur_l], ecx

  ; Restore registers
  pop   PTR_EBP

  ; Routine end
  ret

Merge_DestStereo_SrcFlipped_Src16_Copy16_UpFiltered:

  ; adjust fractional
  shl   edx, 16

  ; Save registers
  push  PTR_EBP

  ; Load sample data
  mov   eax, dword ptr [PTR_ESI]
  movsx ebx, ax
  sar   eax, 16

  ; rotate filter values
  mov   ecx, [cur_l]
  mov   [cur_l], eax
  mov   eax, [cur_r]
  mov   [cur_r], ebx
  mov   [cur_r2], eax

  ; Merge sample data loop
  ALIGN 16
  merge_loop703:

  ; Upsample the data points
  mov   ebp, edx
  mov   eax, [cur_l]
  mov   ebx, [cur_r]
  shr   ebp, 17
  sub   eax, ecx
  sub   ebx, [cur_r2]
  imul  eax, ebp
  imul  ebx, ebp
  sar   eax, 15
  sar   ebx, 15
  add   eax, ecx
  add   ebx, [cur_r2]

  ; Copy sample data into output buffer
  mov   [PTR_EDI], ax
  mov   [PTR_EDI+2], bx
  add   PTR_EDI, 4
  cmp   PTR_EDI, dest_end
  jae   dest_end_exit703

  ; Add to accumulator and advance the source correctly
  add   edx, [step_fract]
  jnc   merge_loop703

  ; Move the source pointer
  add   PTR_ESI, 4
  cmp   PTR_ESI, src_end
  jae   src_end_exit703

  ; Load sample data
  mov   eax, dword ptr [PTR_ESI]
  movsx ebx, ax
  sar   eax, 16

  ; rotate filter values
  mov   ecx, [cur_l]
  mov   [cur_l], eax
  mov   eax, [cur_r]
  mov   [cur_r], ebx
  mov   [cur_r2], eax

  ; End loop
  jmp   merge_loop703

  ; Jump out point if end of dest is reached
  dest_end_exit703:
  add   PTR_ESI, 4
  add   edx, [step_fract]
  jc    skip_filter_adjust703

  ; rotate filter values
  mov   [cur_l], ecx
  mov   ecx, [cur_r2]
  mov   [cur_r], ecx

  ; un-increment the source to skip the early source adjustment
  sub   PTR_ESI, 4

  skip_filter_adjust703:

  ; Jump out point if end of src is reached
  src_end_exit703:

  ; Restore registers
  pop   PTR_EBP

  ; adjust fractional
  shr   edx, 16

  ; Routine end
  ret

Merge_DestStereo_SrcFlipped_Src16_Copy16_Filtered ENDP


vector_table    LABEL PTR_LABEL
  PTR_TAG Merge_DestMono_SrcMono_Src8_NoVolume_NoResample
  PTR_TAG Merge_DestStereo_SrcMono_Src8_NoVolume_NoResample
  PTR_TAG Merge_DestMono_SrcMono_Src16_NoVolume_NoResample
  PTR_TAG Merge_DestStereo_SrcMono_Src16_NoVolume_NoResample
  PTR_TAG Merge_DestMono_SrcMono_Src8_NoVolume_NoResample
  PTR_TAG Merge_DestStereo_SrcMono_Src8_NoVolume_NoResample
  PTR_TAG Merge_DestMono_SrcMono_Src16_NoVolume_NoResample
  PTR_TAG Merge_DestStereo_SrcMono_Src16_NoVolume_NoResample
  PTR_TAG Merge_DestMono_SrcStereo_Src8_NoVolume_NoResample
  PTR_TAG Merge_DestStereo_SrcStereo_Src8_NoVolume_NoResample
  PTR_TAG Merge_DestMono_SrcStereo_Src16_NoVolume_NoResample
  PTR_TAG Merge_DestStereo_SrcStereo_Src16_NoVolume_NoResample
  PTR_TAG Merge_DestMono_SrcStereo_Src8_NoVolume_NoResample
  PTR_TAG Merge_DestStereo_SrcStereo_Src8_NoVolume_NoResample
  PTR_TAG Merge_DestMono_SrcStereo_Src16_NoVolume_NoResample
  PTR_TAG Merge_DestStereo_SrcStereo_Src16_NoVolume_NoResample
  PTR_TAG Merge_DestMono_SrcMono_Src8_NoVolume_NoResample
  PTR_TAG Merge_DestStereo_SrcMono_Src8_NoVolume_NoResample
  PTR_TAG Merge_DestMono_SrcMono_Src16_NoVolume_NoResample
  PTR_TAG Merge_DestStereo_SrcMono_Src16_NoVolume_NoResample
  PTR_TAG Merge_DestMono_SrcMono_Src8_NoVolume_NoResample
  PTR_TAG Merge_DestStereo_SrcMono_Src8_NoVolume_NoResample
  PTR_TAG Merge_DestMono_SrcMono_Src16_NoVolume_NoResample
  PTR_TAG Merge_DestStereo_SrcMono_Src16_NoVolume_NoResample
  PTR_TAG Merge_DestMono_SrcFlipped_Src8_NoVolume_NoResample
  PTR_TAG Merge_DestStereo_SrcFlipped_Src8_NoVolume_NoResample
  PTR_TAG Merge_DestMono_SrcFlipped_Src16_NoVolume_NoResample
  PTR_TAG Merge_DestStereo_SrcFlipped_Src16_NoVolume_NoResample
  PTR_TAG Merge_DestMono_SrcFlipped_Src8_NoVolume_NoResample
  PTR_TAG Merge_DestStereo_SrcFlipped_Src8_NoVolume_NoResample
  PTR_TAG Merge_DestMono_SrcFlipped_Src16_NoVolume_NoResample
  PTR_TAG Merge_DestStereo_SrcFlipped_Src16_NoVolume_NoResample
  PTR_TAG Merge_DestMono_SrcMono_Src8_NoVolume_Resample
  PTR_TAG Merge_DestStereo_SrcMono_Src8_NoVolume_Resample
  PTR_TAG Merge_DestMono_SrcMono_Src16_NoVolume_Resample
  PTR_TAG Merge_DestStereo_SrcMono_Src16_NoVolume_Resample
  PTR_TAG Merge_DestMono_SrcMono_Src8_NoVolume_Filtered
  PTR_TAG Merge_DestStereo_SrcMono_Src8_NoVolume_Filtered
  PTR_TAG Merge_DestMono_SrcMono_Src16_NoVolume_Filtered
  PTR_TAG Merge_DestStereo_SrcMono_Src16_NoVolume_Filtered
  PTR_TAG Merge_DestMono_SrcStereo_Src8_NoVolume_Resample
  PTR_TAG Merge_DestStereo_SrcStereo_Src8_NoVolume_Resample
  PTR_TAG Merge_DestMono_SrcStereo_Src16_NoVolume_Resample
  PTR_TAG Merge_DestStereo_SrcStereo_Src16_NoVolume_Resample
  PTR_TAG Merge_DestMono_SrcStereo_Src8_NoVolume_Filtered
  PTR_TAG Merge_DestStereo_SrcStereo_Src8_NoVolume_Filtered
  PTR_TAG Merge_DestMono_SrcStereo_Src16_NoVolume_Filtered
  PTR_TAG Merge_DestStereo_SrcStereo_Src16_NoVolume_Filtered
  PTR_TAG Merge_DestMono_SrcMono_Src8_NoVolume_Resample
  PTR_TAG Merge_DestStereo_SrcMono_Src8_NoVolume_Resample
  PTR_TAG Merge_DestMono_SrcMono_Src16_NoVolume_Resample
  PTR_TAG Merge_DestStereo_SrcMono_Src16_NoVolume_Resample
  PTR_TAG Merge_DestMono_SrcMono_Src8_NoVolume_Filtered
  PTR_TAG Merge_DestStereo_SrcMono_Src8_NoVolume_Filtered
  PTR_TAG Merge_DestMono_SrcMono_Src16_NoVolume_Filtered
  PTR_TAG Merge_DestStereo_SrcMono_Src16_NoVolume_Filtered
  PTR_TAG Merge_DestMono_SrcFlipped_Src8_NoVolume_Resample
  PTR_TAG Merge_DestStereo_SrcFlipped_Src8_NoVolume_Resample
  PTR_TAG Merge_DestMono_SrcFlipped_Src16_NoVolume_Resample
  PTR_TAG Merge_DestStereo_SrcFlipped_Src16_NoVolume_Resample
  PTR_TAG Merge_DestMono_SrcFlipped_Src8_NoVolume_Filtered
  PTR_TAG Merge_DestStereo_SrcFlipped_Src8_NoVolume_Filtered
  PTR_TAG Merge_DestMono_SrcFlipped_Src16_NoVolume_Filtered
  PTR_TAG Merge_DestStereo_SrcFlipped_Src16_NoVolume_Filtered
  PTR_TAG Merge_DestMono_SrcMono_Src8_Volume_NoResample
  PTR_TAG Merge_DestStereo_SrcMono_Src8_Volume_NoResample
  PTR_TAG Merge_DestMono_SrcMono_Src16_Volume_NoResample
  PTR_TAG Merge_DestStereo_SrcMono_Src16_Volume_NoResample
  PTR_TAG Merge_DestMono_SrcMono_Src8_Volume_NoResample
  PTR_TAG Merge_DestStereo_SrcMono_Src8_Volume_NoResample
  PTR_TAG Merge_DestMono_SrcMono_Src16_Volume_NoResample
  PTR_TAG Merge_DestStereo_SrcMono_Src16_Volume_NoResample
  PTR_TAG Merge_DestMono_SrcStereo_Src8_Volume_NoResample
  PTR_TAG Merge_DestStereo_SrcStereo_Src8_Volume_NoResample
  PTR_TAG Merge_DestMono_SrcStereo_Src16_Volume_NoResample
  PTR_TAG Merge_DestStereo_SrcStereo_Src16_Volume_NoResample
  PTR_TAG Merge_DestMono_SrcStereo_Src8_Volume_NoResample
  PTR_TAG Merge_DestStereo_SrcStereo_Src8_Volume_NoResample
  PTR_TAG Merge_DestMono_SrcStereo_Src16_Volume_NoResample
  PTR_TAG Merge_DestStereo_SrcStereo_Src16_Volume_NoResample
  PTR_TAG Merge_DestMono_SrcMono_Src8_Volume_NoResample
  PTR_TAG Merge_DestStereo_SrcMono_Src8_Volume_NoResample
  PTR_TAG Merge_DestMono_SrcMono_Src16_Volume_NoResample
  PTR_TAG Merge_DestStereo_SrcMono_Src16_Volume_NoResample
  PTR_TAG Merge_DestMono_SrcMono_Src8_Volume_NoResample
  PTR_TAG Merge_DestStereo_SrcMono_Src8_Volume_NoResample
  PTR_TAG Merge_DestMono_SrcMono_Src16_Volume_NoResample
  PTR_TAG Merge_DestStereo_SrcMono_Src16_Volume_NoResample
  PTR_TAG Merge_DestMono_SrcFlipped_Src8_Volume_NoResample
  PTR_TAG Merge_DestStereo_SrcFlipped_Src8_Volume_NoResample
  PTR_TAG Merge_DestMono_SrcFlipped_Src16_Volume_NoResample
  PTR_TAG Merge_DestStereo_SrcFlipped_Src16_Volume_NoResample
  PTR_TAG Merge_DestMono_SrcFlipped_Src8_Volume_NoResample
  PTR_TAG Merge_DestStereo_SrcFlipped_Src8_Volume_NoResample
  PTR_TAG Merge_DestMono_SrcFlipped_Src16_Volume_NoResample
  PTR_TAG Merge_DestStereo_SrcFlipped_Src16_Volume_NoResample
  PTR_TAG Merge_DestMono_SrcMono_Src8_Volume_Resample
  PTR_TAG Merge_DestStereo_SrcMono_Src8_Volume_Resample
  PTR_TAG Merge_DestMono_SrcMono_Src16_Volume_Resample
  PTR_TAG Merge_DestStereo_SrcMono_Src16_Volume_Resample
  PTR_TAG Merge_DestMono_SrcMono_Src8_Volume_Filtered
  PTR_TAG Merge_DestStereo_SrcMono_Src8_Volume_Filtered
  PTR_TAG Merge_DestMono_SrcMono_Src16_Volume_Filtered
  PTR_TAG Merge_DestStereo_SrcMono_Src16_Volume_Filtered
  PTR_TAG Merge_DestMono_SrcStereo_Src8_Volume_Resample
  PTR_TAG Merge_DestStereo_SrcStereo_Src8_Volume_Resample
  PTR_TAG Merge_DestMono_SrcStereo_Src16_Volume_Resample
  PTR_TAG Merge_DestStereo_SrcStereo_Src16_Volume_Resample
  PTR_TAG Merge_DestMono_SrcStereo_Src8_Volume_Filtered
  PTR_TAG Merge_DestStereo_SrcStereo_Src8_Volume_Filtered
  PTR_TAG Merge_DestMono_SrcStereo_Src16_Volume_Filtered
  PTR_TAG Merge_DestStereo_SrcStereo_Src16_Volume_Filtered
  PTR_TAG Merge_DestMono_SrcMono_Src8_Volume_Resample
  PTR_TAG Merge_DestStereo_SrcMono_Src8_Volume_Resample
  PTR_TAG Merge_DestMono_SrcMono_Src16_Volume_Resample
  PTR_TAG Merge_DestStereo_SrcMono_Src16_Volume_Resample
  PTR_TAG Merge_DestMono_SrcMono_Src8_Volume_Filtered
  PTR_TAG Merge_DestStereo_SrcMono_Src8_Volume_Filtered
  PTR_TAG Merge_DestMono_SrcMono_Src16_Volume_Filtered
  PTR_TAG Merge_DestStereo_SrcMono_Src16_Volume_Filtered
  PTR_TAG Merge_DestMono_SrcFlipped_Src8_Volume_Resample
  PTR_TAG Merge_DestStereo_SrcFlipped_Src8_Volume_Resample
  PTR_TAG Merge_DestMono_SrcFlipped_Src16_Volume_Resample
  PTR_TAG Merge_DestStereo_SrcFlipped_Src16_Volume_Resample
  PTR_TAG Merge_DestMono_SrcFlipped_Src8_Volume_Filtered
  PTR_TAG Merge_DestStereo_SrcFlipped_Src8_Volume_Filtered
  PTR_TAG Merge_DestMono_SrcFlipped_Src16_Volume_Filtered
  PTR_TAG Merge_DestStereo_SrcFlipped_Src16_Volume_Filtered
  PTR_TAG Merge_DestMono_SrcMono_Src8_Copy16_NoResample
  PTR_TAG Merge_DestStereo_SrcMono_Src8_Copy16_NoResample
  PTR_TAG Merge_DestMono_SrcMono_Src16_Copy16_NoResample
  PTR_TAG Merge_DestStereo_SrcMono_Src16_Copy16_NoResample
  PTR_TAG Merge_DestMono_SrcMono_Src8_Copy16_NoResample
  PTR_TAG Merge_DestStereo_SrcMono_Src8_Copy16_NoResample
  PTR_TAG Merge_DestMono_SrcMono_Src16_Copy16_NoResample
  PTR_TAG Merge_DestStereo_SrcMono_Src16_Copy16_NoResample
  PTR_TAG Merge_DestMono_SrcStereo_Src8_Copy16_NoResample
  PTR_TAG Merge_DestStereo_SrcStereo_Src8_Copy16_NoResample
  PTR_TAG Merge_DestMono_SrcStereo_Src16_Copy16_NoResample
  PTR_TAG Merge_DestStereo_SrcStereo_Src16_Copy16_NoResample
  PTR_TAG Merge_DestMono_SrcStereo_Src8_Copy16_NoResample
  PTR_TAG Merge_DestStereo_SrcStereo_Src8_Copy16_NoResample
  PTR_TAG Merge_DestMono_SrcStereo_Src16_Copy16_NoResample
  PTR_TAG Merge_DestStereo_SrcStereo_Src16_Copy16_NoResample
  PTR_TAG Merge_DestMono_SrcMono_Src8_Copy16_NoResample
  PTR_TAG Merge_DestStereo_SrcMono_Src8_Copy16_NoResample
  PTR_TAG Merge_DestMono_SrcMono_Src16_Copy16_NoResample
  PTR_TAG Merge_DestStereo_SrcMono_Src16_Copy16_NoResample
  PTR_TAG Merge_DestMono_SrcMono_Src8_Copy16_NoResample
  PTR_TAG Merge_DestStereo_SrcMono_Src8_Copy16_NoResample
  PTR_TAG Merge_DestMono_SrcMono_Src16_Copy16_NoResample
  PTR_TAG Merge_DestStereo_SrcMono_Src16_Copy16_NoResample
  PTR_TAG Merge_DestMono_SrcFlipped_Src8_Copy16_NoResample
  PTR_TAG Merge_DestStereo_SrcFlipped_Src8_Copy16_NoResample
  PTR_TAG Merge_DestMono_SrcFlipped_Src16_Copy16_NoResample
  PTR_TAG Merge_DestStereo_SrcFlipped_Src16_Copy16_NoResample
  PTR_TAG Merge_DestMono_SrcFlipped_Src8_Copy16_NoResample
  PTR_TAG Merge_DestStereo_SrcFlipped_Src8_Copy16_NoResample
  PTR_TAG Merge_DestMono_SrcFlipped_Src16_Copy16_NoResample
  PTR_TAG Merge_DestStereo_SrcFlipped_Src16_Copy16_NoResample
  PTR_TAG Merge_DestMono_SrcMono_Src8_Copy16_Resample
  PTR_TAG Merge_DestStereo_SrcMono_Src8_Copy16_Resample
  PTR_TAG Merge_DestMono_SrcMono_Src16_Copy16_Resample
  PTR_TAG Merge_DestStereo_SrcMono_Src16_Copy16_Resample
  PTR_TAG Merge_DestMono_SrcMono_Src8_Copy16_Filtered
  PTR_TAG Merge_DestStereo_SrcMono_Src8_Copy16_Filtered
  PTR_TAG Merge_DestMono_SrcMono_Src16_Copy16_Filtered
  PTR_TAG Merge_DestStereo_SrcMono_Src16_Copy16_Filtered
  PTR_TAG Merge_DestMono_SrcStereo_Src8_Copy16_Resample
  PTR_TAG Merge_DestStereo_SrcStereo_Src8_Copy16_Resample
  PTR_TAG Merge_DestMono_SrcStereo_Src16_Copy16_Resample
  PTR_TAG Merge_DestStereo_SrcStereo_Src16_Copy16_Resample
  PTR_TAG Merge_DestMono_SrcStereo_Src8_Copy16_Filtered
  PTR_TAG Merge_DestStereo_SrcStereo_Src8_Copy16_Filtered
  PTR_TAG Merge_DestMono_SrcStereo_Src16_Copy16_Filtered
  PTR_TAG Merge_DestStereo_SrcStereo_Src16_Copy16_Filtered
  PTR_TAG Merge_DestMono_SrcMono_Src8_Copy16_Resample
  PTR_TAG Merge_DestStereo_SrcMono_Src8_Copy16_Resample
  PTR_TAG Merge_DestMono_SrcMono_Src16_Copy16_Resample
  PTR_TAG Merge_DestStereo_SrcMono_Src16_Copy16_Resample
  PTR_TAG Merge_DestMono_SrcMono_Src8_Copy16_Filtered
  PTR_TAG Merge_DestStereo_SrcMono_Src8_Copy16_Filtered
  PTR_TAG Merge_DestMono_SrcMono_Src16_Copy16_Filtered
  PTR_TAG Merge_DestStereo_SrcMono_Src16_Copy16_Filtered
  PTR_TAG Merge_DestMono_SrcFlipped_Src8_Copy16_Resample
  PTR_TAG Merge_DestStereo_SrcFlipped_Src8_Copy16_Resample
  PTR_TAG Merge_DestMono_SrcFlipped_Src16_Copy16_Resample
  PTR_TAG Merge_DestStereo_SrcFlipped_Src16_Copy16_Resample
  PTR_TAG Merge_DestMono_SrcFlipped_Src8_Copy16_Filtered
  PTR_TAG Merge_DestStereo_SrcFlipped_Src8_Copy16_Filtered
  PTR_TAG Merge_DestMono_SrcFlipped_Src16_Copy16_Filtered
  PTR_TAG Merge_DestStereo_SrcFlipped_Src16_Copy16_Filtered
