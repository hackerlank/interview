Unit Utils;

interface

uses Windows, SysUtils, Graphics, Math;

const
  STACK_ARRAY_ALLOC_STEP = 4;
  LOG_FILE_NAME = 'Log/Level editor.log';

type

  TStack = class(TObject)  // стэк. TODO: дописать сюда очередь
  private
    iCount: Integer;
    Data: array of Pointer;
  public
    constructor Create();
    procedure Push(Src: Pointer);
    function  Pop(): Pointer;
    function Empty(): Boolean;
  end;

  PListNode = ^TListNode;
  TListNode = class(TObject)
  protected
    next, prev: PListNode;
  end;

  TList = class(TObject)  // класс связного списка должен быть здесь
  public
    FirstNode: PListNode;
  end;

  function  IntToStr(Num: Integer): String;
  function  StrToInt(const S: String): Integer;
  procedure MsgLastErr();
  function  MyGetMem(Size: LongWord): Pointer;
  procedure MyFreeMem(p: Pointer);
  procedure SwapRGB( data: Pointer; Size: Integer);

  function  isFileExist(FileName: PChar): Boolean;
  function  StrLen(str: string): Integer;
  procedure myExtractFilePath(var Dst: string; Src: string);
  procedure myAppend(var Dst: string; a, b: string);
  procedure Log(section, text: string; const args: array of const);
  function TextExtentEx(Text: string; ACanvas: TCanvas): TPoint;
  function isPointInsideRect(const APoint: TPoint; ARect: TRect): Boolean;

  //Пропускает хуйню в строке. Возвращает true если достигнут конец строки.
  function SkipWhiteSpace(var iPos: Integer; sStr: string): Boolean;
  function SkipLuaWhiteSpace(var iPos: Integer; sStr: string): Boolean;
  function LuaFindCommentBeginWith(AStrSrc, AStrComment: string): Boolean;  

implementation

var
  LogFile: textfile;

function StrLen(str: string): Integer;
var
 i: Integer;
begin
  i := 1;
  while str[i] <> #0 do
    Inc(i);
  Result := i-1;
end;

procedure myExtractFilePath(var Dst: string; Src: string);
var
  i: Integer;
begin
  for i := StrLen(Src) downto 1 do
    if Src[i] in [':', '/', '\'] then
      break;
  Dst := Copy(Src, 1, i);
end;

procedure myAppend(var Dst: string; a, b: string);
var
 i, la, lb, ld: Integer;
begin
  ld := Length(Dst);
  la := StrLen(a);
  lb := StrLen(b);
  if ld < la + lb then
    Exit;
  i := 1;
  while i <= la+lb do
    begin
      if i <= la then
        Dst[i] := a[i]
      else
        Dst[i] := b[i-la];
      Inc(i);
    end;
    Dst[i] := #0;
    Dst[i+1] := #0;    
end;

function IntToStr(Num: Integer): String;
begin
 Str(Num, result);
end;

function StrToInt(const S: string): Integer;
var
  E: Integer;
begin
  Val(S, Result, E);
end;

procedure MsgLastErr();
var
  lpmsgbuf: array of char;
begin
 FormatMessage(
    FORMAT_MESSAGE_ALLOCATE_BUFFER or FORMAT_MESSAGE_FROM_SYSTEM,
    nil,
    GetLastError(),
    0,
    @lpMsgBuf,
    0,
    nil);
 MessageBox( 0, pchar(lpMsgBuf), 'GetLastError', MB_OK or MB_ICONINFORMATION );
 LocalFree( integer(@lpMsgBuf) );
end;

function MyGetMem(Size: longword): Pointer;
 begin
  result := VirtualAlloc( nil, size, MEM_COMMIT, PAGE_READWRITE );
 end;

procedure MyFreeMem(p: Pointer);
 begin
  if ( p = nil ) then 
   Exit;
  VirtualFree( p, 0, MEM_RELEASE);
 end;

procedure SwapRGB(data: Pointer; Size: Integer);
asm
 mov ebx, eax
 mov ecx, size
 @@loop :
  mov al,[ebx+0]
  mov ah,[ebx+2]
  mov [ebx+2],al
  mov [ebx+0],ah
  add ebx,3
  dec ecx
 jnz @@loop
end;


function isFileExist( FileName: PChar): Boolean;
var
 hfile: Thandle;
begin
 hfile := CreateFile(FileName, GENERIC_READ, FILE_SHARE_READ, nil,
   OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
 Result := hFile <> INVALID_HANDLE_VALUE;
 if Result then
  FileClose(hfile);
end;

procedure Log(section, text: string; const args: array of const);
begin
  Append(Logfile);
  WriteLn(LogFile,'[', section, ']', #9, Format(text, Args));
  CloseFile(LogFile);  
end;

function TextExtentEx(Text: string; ACanvas: TCanvas): TPoint;
var
  i, l, il, lCount: Integer; // i - index, il - last index, l - Text length, lCount - lines count
  temp: string;
begin
  with Result do
    begin
      X := 0;
      Y := 0;
    end;
  l := Length(Text);
  if l = 0 then
    Exit;
  i := 0;
  il := 1;
  lCount := 0;
  while i <= l do
    begin
      Inc(i);
      if Text[i] = #13 then
        with Result, ACanvas do
          begin
            temp := Copy(Text, il, i-il);
            Inc(i, 2);
            il := i;
            X := Max(X, TextExtent(temp).cx);
            Inc(Y, TextExtent(temp).cy);
            Inc(lCount);
          end;
    end;
end;

function isPointInsideRect(const APoint: TPoint; ARect: TRect): Boolean;
begin
  Result := true;
  with APoint, ARect do
    begin
      if (X < Left) or (X > Right) or (Y < Top) or (Y > Bottom) then
        Result := false;
    end;
end;


function SkipWhiteSpace(var iPos: Integer; sStr: string): Boolean;
begin
  while (iPos <= Length(sStr)) and (sStr[iPos] in [' ',#13,#9,#10]) do
  begin
    Inc(iPos);
  end;
end;

function SkipLuaWhiteSpace(var iPos: Integer; sStr: string): Boolean;
type
  TWSState = (wsStart, wsOpMinus1, wsOpMinus2, wsOpBrace1, wsOpBrace2, wsClBrace1,
    wsClBrace2, wsSpace, wsEnd, wsError, wsAnyLineChar, wsAnyBlockChar);
var
  state: TWSState;
begin
  state := wsStart;
  while not (state in [wsEnd, wsError]) do
    case state of
      wsStart:
        begin
          if iPos > Length(sStr) then
            state := wsEnd
          else if sStr[iPos] in [' ', #13, #10, #9] then
            state := wsSpace
          else if sStr[iPos] = '-' then
            state := wsOpMinus1
          else
            state := wsEnd;
          Continue;
        end;
      wsSpace:
        begin
          SkipWhiteSpace(iPos, sStr);
          if iPos > Length(sStr) then
            state := wsEnd
          else if sStr[iPos] = '-' then
            state := wsOpMinus1
          else
            state := wsEnd;
          Continue;
        end;
      wsOpMinus1:
        begin
          Inc(iPos);
          if iPos > length(sStr) then
            state := wsError
          else if sStr[iPos] = '-' then
            state := wsOpMinus2
          else
            state := wsError;
          Continue;
        end;
      wsOpMinus2:
        begin
          Inc(iPos);
          if iPos > length(sStr) then
            state := wsEnd
          else if sStr[iPos] = '[' then
            state := wsOpBrace1
          else
            state := wsAnyLineChar;
          Continue;
        end;
      wsAnyLineChar:
        begin
          while (iPos <= Length(sStr)) and (sStr[iPos] <> #13) do
            inc(iPos);
          if iPos > Length(sStr) then
            state := wsEnd
          else
            state := wsSpace;
          Continue;
        end;
      wsOpBrace1:
        begin
          Inc(iPos);
          if iPos > Length(sStr) then
            state := wsEnd
          else if sStr[iPos] = '[' then
            state := wsOpBrace2
          else
            state := wsAnyLineChar;
          Continue;
        end;
      wsOpBrace2:
        begin
          Inc(iPos);
          if iPos > Length(sStr) then
            state := wsError
          else
            state := wsAnyBlockChar;
          Continue;
        end;
      wsAnyBlockChar:
        begin
          if iPos > Length(sStr) then
            state := wsError
          else if sStr = ']' then
            state := wsClBrace1
          else
            Inc(iPos);
          Continue;
        end;
      wsClBrace1:
        begin
          Inc(iPos);
          if iPos > Length(sStr) then
            state := wsError
          else if sStr[iPos] = ']' then
            state := wsClBrace2
          else
            state := wsAnyBlockChar;
          Continue;
        end;
      wsClBrace2:
        begin
          Inc(iPos);
          state := wsStart;
        end;
    end;
end;

function LuaFindCommentBeginWith(AStrSrc, AStrComment: string): Boolean;
type
  TState = (stMinus1, stMinus2, stNothingSpecial, stTryCompare, stEnd);
var
  i: Integer;
  State: TState;
begin
  Result := false;
  State := stNothingSpecial;
  i := 1;
  while State <> stEnd do
    case State of
      stMinus1:
        begin
          Inc(i);
          if i > Length(AStrSrc) then
            State := stEnd
          else if AStrSrc[i] = '-' then
            State := stMinus2
          else
            State := stNothingSpecial;
        end;
      stMinus2:
        begin
          Inc(i);
          if (i + Length(AStrComment)) > Length(AStrSrc) then
            State := stEnd
          else if AnsiCompareText(Copy(AStrSrc, i, Length(AStrComment)), AStrComment) = 0 then
            begin
              Result := true;
              Exit;
            end
          else
            State := stNothingSpecial;
        end;
      stNothingSpecial:
        begin
          if i > Length(AStrSrc) then
            State := stEnd
          else if AStrSrc[i] = '-' then
            State := stMinus1
          else
            Inc(i);
        end;
    end;
end;


{ TStack }

constructor TStack.Create;
begin
  iCount := 0;
end;

function TStack.Empty: Boolean;
begin
  Result := iCount = 0;
end;

function TStack.Pop: Pointer;
var
  delta: Integer;
begin
  Dec(iCount);
  Result := Data[iCount];
  Delta := (Length(Data) - iCount) div STACK_ARRAY_ALLOC_STEP;
  if Delta > 0 then
    SetLength(Data, STACK_ARRAY_ALLOC_STEP*Delta);
end;

procedure TStack.Push(Src: Pointer);
begin
  Inc(iCount);
  if iCount = 1 then
    SetLength(Data, 1);
  if Length(Data) mod STACK_ARRAY_ALLOC_STEP <> 0 then
    SetLength(Data, Length(Data) div STACK_ARRAY_ALLOC_STEP + STACK_ARRAY_ALLOC_STEP);
  Data[iCount-1] := Src;
end;

initialization
  AssignFile(Logfile, LOG_FILE_NAME);
  Rewrite(Logfile);
  CloseFile(LogFile);
  Append(Logfile);


end.

