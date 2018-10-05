unit SBAudio;

{ Audio Support Unit, Version 3.0, Copyright (c) 1997 by George M. Tzoumas }

{ This program is distributed in the hope that it will be useful, 
but WITHOUT ANY WARRANTY; without even the implied warranty of 
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
Use this software AT YOUR OWN RISK. }

interface

type

  TSampleHeader = record
    Rate : Word;
    Bits,
    Mode : Byte;
    Signed : Boolean;
  end;

const

  sbOK         = 0;
  sbResetError = 1;
  sbWriteError = 2;

  srLow = 11025;
  srMed = 22050;
  srHi  = 44100;

  smMono   = 1;
  smStereo = 2;

  _8Bit  = 1;
  _16Bit = 2;

  Signed   = True;
  Unsigned = False;

var
  IOBase, DMA, IRQ: Word;
  sbStatus : Byte;
  SoundPlaying: Boolean;
  Looping: Boolean;

procedure InitDSP(APort: Word; ADMA, AIRQ: Byte);
procedure WriteDSP(b: Byte);
function ReadDSP: Byte;
procedure ResetDSP;
procedure DSPSetFreq(Freq: Word);

procedure ShutDownSB;
procedure InstallHandler;
procedure UninstallHandler;

procedure SpeakerOn;
procedure SpeakerOff;
function IsSpeakerOn: Boolean;

procedure PauseSound;
procedure ContinueSound;

procedure PlaySample(Sample : Pointer; Size, Freq : Word);

implementation

uses Dos;

const
{ DSP Commands }
  cmDirectDAC = $10;
  cmNormalDMADAC = $14;
  cmDirectADC = $20;
  cmNormalDMAADC = $24;
  cmSetTimeConst = $40;
  cmSetBlockSize = $48;
  cmHaltDMA = $D0;
  cmSpeakerOn = $D1;
  cmSpeakerOff = $D3;
  cmGetSpeakerState = $D8;
  cmContinueDMA = $D4;
  cmGetVersion = $E1;

  DMAChannel: Array[0..3, 1..3] of Byte =
    (($87, $0, $1), ($83, $2, $3), ($81, $2, $3), ($82,$6,$7));

var
  ResetPort, ReadPort, WritePort, PollPort, Int16BitPort: Word;
  IRQStartMask, IRQStopMask, IRQIntVector: Byte;
  IRQHandlerInstalled: Boolean;

  OldIntVector: Pointer;
  OldExitProc: Pointer;

  DMAStartMask, DMAStopMask, DMAModeReg: Byte;

  PICPort: Byte;

procedure InitDSP(APort: Word; ADMA, AIRQ: Byte);
const
  IRQIntNums : array[0..15] of byte =
                    ($08, $09, $0A, $0B, $0C, $0D, $0E, $0F,
                     $70, $71, $72, $73, $74, $75, $76, $77);
begin
  IOBase := APort;
  DMA    := ADMA;
  IRQ    := AIRQ;
  ResetPort := IOBase + $6;
  ReadPort  := IOBase + $A;
  WritePort := IOBase + $C;
  PollPort  := IOBase + $E;
  Int16BitPort   := IOBase + $F;

  IRQIntVector := IRQIntNums[IRQ];
  IRQStopMask  := 1 SHL (IRQ mod 8);
  IRQStartMask := not(IRQStopMask);

  DMAStartMask := DMA + $00; {000000xx}
  DMAStopMask  := DMA + $04; {000001xx}
  DMAModeReg   := DMA + $48; {010010xx}

  if IRQ <= 7 then PICPort := $21 else PICPort := $A1;  {INTC1 / INTC2 }

  ResetDSP;
end;

procedure _CLI; inline ($FA);

procedure _STI; inline ($FB);

procedure XDelay(MS: Byte); assembler;
asm
           mov    ah, 86h
           xor    cx, cx
           xor    dh, dh
           mov    dl, MS
           shl    dx, 5
           int    15h
end;


procedure WriteDSP(b: Byte); assembler;
asm
           mov    dx, WritePort
   @@1:    in     al, dx
           and    al, 128
           jnz     @@1
           mov    al, b
           out    dx, al
end;

function ReadDSP: Byte; assembler;
asm
           mov    dx, PollPort
   @@1:    in     al, dx
           and    al, 128
           jz     @@1
           mov    dx, ReadPort
           in     al, dx
end;

procedure ResetDSP;
var i: Byte;
begin
  Port[ResetPort] := 1;
  XDelay(1);
  Port[ResetPort] := 0;
  i := 1;
  while (ReadDSP <> $AA) and (i < 100) do Inc(i);
  if i < 100 then sbStatus := sbOK else sbStatus := sbResetError;
end;

procedure DSPSetFreq(Freq: Word);
begin
  WriteDSP(cmSetTimeConst);
  WriteDSP(256 - 1000000 Div Freq);
end;

procedure ClearInterrupt;
var Temp: Byte;
begin
  Temp := Port[PollPort];
  Port[$20] := $20;
end;

procedure IntHandler; interrupt;
begin
  ClearInterrupt;
End;


procedure StopSBIRQ;
begin
  Port[PICPort] := Port[PICPort] or IRQStopMask;
end;

procedure StartSBIRQ;
begin
  Port[PICPort] := Port[PICPort] and IRQStartMask;
end;

procedure ShutDownSB;
Begin
  ResetDSP;
  UninstallHandler;
end;

procedure InstallHandler;
Begin
  _CLI;
  StopSBIRQ;
  GetIntVec(IRQIntVector, OldIntVector);
  SetIntVec(IRQIntVector, @IntHandler);
  StartSBIRQ;
  _STI;
  IRQHandlerInstalled := True;
end;

procedure UninstallHandler;
begin
  _CLI;
  StopSBIRQ;
  SetIntVec(IRQIntVector, OldIntVector);
  _STI;
  IRQHandlerInstalled := False;
end;

procedure SpeakerOn;
begin
  WriteDSP(cmSpeakerOn);
end;

procedure SpeakerOff;
begin
  WriteDSP(cmSpeakerOff);
end;

function IsSpeakerOn: Boolean;
begin
  WriteDSP(cmGetSpeakerState);
  if ReadDSP = 0 then IsSpeakerOn := False else IsSpeakerOn := True;
end;

procedure PauseSound;
begin
  WriteDSP(cmHaltDMA);
end;

procedure ContinueSound;
begin
  WriteDSP(cmContinueDMA);
end;

procedure PlaySample(Sample : Pointer; Size, Freq : Word);
var Page, Offset: Word;
begin
  Dec(Size);
  Offset := Seg(Sample^) Shl 4 + Ofs(Sample^);
  Page := (Seg(Sample^) + Ofs(Sample^) Shr 4) Shr 12;
  Port[$0A] := DMAStopMask;
  Port[$0C] := $00; { Clear the internal DMA flip-flop }
  Port[$0B] := DMAModeReg;
  Port[DMAChannel[DMA,2]] := Lo(Offset);
  Port[DMAChannel[DMA,2]] := Hi(Offset);
  Port[DMAChannel[DMA,1]] := Page;
  Port[DMAChannel[DMA,3]] := Lo(Size);
  Port[DMAChannel[DMA,3]] := Hi(Size);
  Port[$0A] := DMAStartMask;
  DSPSetFreq(Freq);
  WriteDSP(cmNormalDMADAC);
  WriteDSP(Lo(Size));
  WriteDSP(Hi(Size));
end;

procedure SBDSPExitProc; far;
begin
  ExitProc := OldExitProc;
  ResetDSP;
  if (IRQHandlerInstalled = True) then UninstallHandler;
end;

begin
{  OldExitProc  := ExitProc;
  ExitProc     := @SBDSPExitProc;}
  SoundPlaying := False;
  Looping      := False;
  IRQHandlerInstalled := False;
end.