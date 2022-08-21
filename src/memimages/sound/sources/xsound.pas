unit XSound;

{ Copyright 1997 by George M. Tzoumas }

{ This program is distributed in the hope that it will be useful, 
but WITHOUT ANY WARRANTY; without even the implied warranty of 
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
Use this software AT YOUR OWN RISK. }

interface

uses SBAudio;

const
  MaxSamples = 16;

type

  TSample = record Data: Pointer; Size, Rate: Word end;

procedure LoadSample(var F: File; SampleNo: Byte; Size: Word);
procedure OpenSample(SampleNo: Byte; FileName: String);
procedure DisposeSample(SampleNo: Byte);

procedure DACSample(SampleNo: Byte);

procedure WaitSample;

procedure SetRate(SampleNo: Byte; Rate: Word);
procedure SignSample(SampleNo: Byte);
procedure MixSample(Source, Dest: Byte);

procedure InitXSound(Language: Byte);

var
  Samples   : Array[1..MaxSamples] of TSample;
  CurSample : Byte;
  Silent    : Boolean;
  ActiveSample: Byte;

implementation

uses Dos, Crt;

function GetSBSettingFromEnv(BLASTER: String; Letter: Char; Hex: Boolean; var Value: Word): Boolean;
var
  EnvStr: String;
  NumStr: String;
  ErrorCode: Integer;
begin
  EnvStr := BLASTER + ' ';
  Delete(EnvStr, 1, Pos(Letter, EnvStr));
  NumStr := Copy(EnvStr, 1, Pos(' ', EnvStr)-1);
  if (Hex) then        Val('$' + NumStr, Value, ErrorCode) else
    Val(NumStr, Value, ErrorCode);
  if (ErrorCode <> 0) then GetSBSettingFromEnv := False else GetSBSettingFromEnv := True;
end;

function GetSBSettingsFromEnv(var IOBase, IRQ, DMA: Word): Boolean;
var
  EnvStr: String;
  I: Byte;
begin
  EnvStr := GetEnv('BLASTER');
  for I := 1 to Length(EnvStr) do EnvStr[I] := UpCase(EnvStr[I]);
  GetSBSettingsFromEnv := True;
  if (EnvStr = '') then        GetSBSettingsFromEnv := False else
  begin
    if (not(GetSBSettingFromEnv(EnvStr, 'A', True, IOBase))) then GetSBSettingsFromEnv := False;
    if (not(GetSBSettingFromEnv(EnvStr, 'I', False, IRQ))) then GetSBSettingsFromEnv := False;
    if (not(GetSBSettingFromEnv(EnvStr, 'D', False, DMA))) then GetSBSettingsFromEnv := False;
  end;
end;

procedure LoadSample(var F: File; SampleNo: Byte; Size: Word);
begin
  GetMem(Samples[SampleNo].Data, Size);
  BlockRead(F, Samples[SampleNo].Data^, Size);
  Samples[SampleNo].Size := Size;
end;

procedure OpenSample(SampleNo: Byte; FileName: String);
var F: File;
begin
  Assign(F, FileName);
  Reset(F, 1);
  LoadSample(f, SampleNo, FileSize(F));
  Close(F);
  SetRate(SampleNo, 22050);
end;

procedure DisposeSample(SampleNo: Byte);
begin
  with Samples[SampleNo] do
  begin
    if Data <> nil then FreeMem(Data, Size);
    Data := nil;
    Size := 0;
  end;
end;

procedure DACSample(SampleNo: Byte);
begin
  with Samples[SampleNo] do PlaySample(Data, Size, Rate);
  ActiveSample := SampleNo;
  Silent := True;
end;

procedure WaitSample;
begin
  with Samples[ActiveSample] do Delay(Size * 1000 div Rate + 1);
end;

procedure SetRate(SampleNo: Byte; Rate: Word);
begin
  Samples[SampleNo].Rate := Rate;
end;

procedure SignSample(SampleNo: Byte);
var s, o, i: Word;
begin
  s := Seg(Samples[SampleNo].Data^);
  o := Ofs(Samples[SampleNo].Data^);
  for i:= 0 to Samples[SampleNo].Size - 1 do Mem[s:o + i] := Mem[s:o + i] - 128;
end;

procedure MixSample(Source, Dest: Byte);
var s1, o1, s2, o2, i, min: Word;
begin
  s1 := Seg(Samples[Source].Data^);
  o1 := Ofs(Samples[Source].Data^);
  s2 := Seg(Samples[Dest].Data^);
  o2 := Ofs(Samples[Dest].Data^);
  min := Samples[Source].Size;
  if min > Samples[Dest].Size then min := Samples[Dest].Size;
  for i := 0 to min - 1 do Mem[s2:o2 + i] := (Word(Mem[s2:o2 + i]) + Mem[s1:o1 + i]) shr 1;
end;

procedure InitXSound(Language: Byte);
begin
  if not GetSBSettingsFromEnv(IOBase, IRQ, DMA) then
  begin
if Language = 0 then
    Writeln('BLASTER environment variable not set.')
else
    Writeln('† £œ«˜™¢ž«ã §œ¨ ™á¢¢¦¤«¦ª BLASTER ›œ¤ â®œ  ¦¨ ©«œå.');
    IOBase := 0; IRQ := 0; DMA := 0;
  end else
  begin
    InitDSP(IOBase, DMA, IRQ);
    ResetDSP;
    if sbStatus = sbOK then
if Language = 0 then
      Writeln('Sound Card initialized using BLASTER environment variable.')
else  Writeln('† ¡á¨«˜ ã®¦¬ ˜¨® ¡¦§¦ ãŸž¡œ ®¨ž© £¦§¦ é¤«˜ª «ž £œ«˜™¢ž«ã §œ¨ ™á¢¢¦¤«¦ª BLASTER.')
    else
      begin
if Language = 0 then
        Writeln('Could not initialize sound card.')
else    Writeln('ƒœ¤ ã«˜¤ ›¬¤˜«ã ž ˜¨® ¡¦§¦åž©ž «žª ¡á¨«˜ª ã®¦¬.');
        IOBase := 0; IRQ := 0; DMA := 0;
      end;
    ActiveSample := 0;
    Silent := True
  end
end;

end.