unit UObjectFile;

interface

uses SysUtils, Windows, Graphics, Dialogs, types, Utils, PNGImage, StrUtils,
  IniFiles;

const
  MAX_OBJECTS = 256;
  MAGIC_COLOR = $00ec9d;
  OBJECT_FILE_FILTER = 'Level files (*.lua)|*.LUA|All files (*.*)|*.*';
  SIGNATURES : array [1..3] of string = ('BM', 'JFIF', 'PNG');
  DEF_EXT = 'lua';
  STR_DO_NOT_LOAD = 'Forbidden';
  SDNL_LENGTH = 9;  

type
TParamType = (ptFirst, ptString, ptInteger, ptFloat, ptConst, ptComplex, ptLast);
TDocumentType = (bmp1, jpg, png, bmp2, notimage);

PParamLexeme = ^TParamLexeme;
TParamLexeme = record         // ¬ообще элемент св€зного списка.
  ParamName : string;         // название параметра
  ParamValue: string;         // значение параметра в строковом виде
  ParamType : TParamType;     // тип параметра (сторка, константа, целое, дробное, сложное)
  Opt       : Integer;        // доолнительна€ переменна€
  Opt2      : Real;           // вещественна€ дополнительна€ перменна€
  next      : PParamLexeme;   // следующа€ лексема в св€зном списке
  prev      : PParamLexeme;   // предыдуща€ лексема
end;

PObjectFileProps = ^TObjectFileProps;
TObjectFileProps = class
private
  image: TBitmap;
  preview: TBitmap;
  ErrorString, ProtoFileName: string;
  procedure ClearList();
  procedure SkipSpace(var iPos: Integer; sStr: string);
  procedure ExtractLexeme(var iPos: Integer; sStr: string);
public
  FirstParam: TParamLexeme;
  BBOX: TRect;
  MaskDC: HDC;
  MaskHandle: THANDLE;
  procedure AddLexeme(ptype: TParamType; pname: string; pvalue: string);
  constructor Create();
  procedure Free();
  function GetImage(): TBitmap;
  function GetPreview(): TBitmap;
  function AnalyzeString(sStr: string):boolean;
  function GetError(): string;
  function FindParam(DestName: string): PParamLexeme;
  function LoadImage(): Boolean;
  function LoadFromFile(FileName: string): Boolean;
  function LoadFunctionsListFromINI(IniFileName: string): Boolean;
  function DumpToFile(FileName: string): Boolean;
  procedure Assign(source: TObjectFileProps);
  procedure SuperMegaAssignTo(ADst: PObjectFileProps);
  procedure FakeList();
end;

TObjectFilePropsArr = array of TObjectFileProps;

implementation

var
  Dildo: TBitmap;
  ModulePath: string;
  ObjectsPath: string;
  TexturesPath: string;

function SearchStep(const src: TBitmap; const a1, a2, b1, b2: Integer; hor: boolean): Integer;
var
  i, j, color: Integer;
  finded: boolean;
begin
  Result := 0;
  finded := false;
  j := a1;
  while j <> a2 do
    begin
      i := b1;
      while i <> b2 do
        begin
          if hor then
            color := src.Canvas.Pixels[j, i]
          else
            color := src.Canvas.Pixels[i, j];
          if (color <> MAGIC_COLOR) then
            begin
                finded := true;
                break;
            end;
          if b1 <= b2 then
            Inc(i)
          else
            Dec(i);
        end;
      if finded then
        begin
          Result := j;
          Exit;
        end;

      if (a1 <= a2) then
        Inc(j)
      else
        Dec(j);
    end;
end;

procedure GenBBOX(src: TBitmap;  const srcRect: TRect; var dstRect: TRect);
var
  i, j: Integer;
  finded: boolean;
begin
  with srcRect do
    begin
      dstRect.top     := SearchStep(src, top, bottom, left, right, false);
      dstRect.bottom  := SearchStep(src, bottom, top, left, right, false) + 1;
      dstRect.left    := SearchStep(src, left, right, top, bottom, true);
      dstRect.right   := SearchStep(src, right, left, top, bottom, true) + 1;
    end;
end;

{ TObjectFileProps }

procedure TObjectFileProps.AddLexeme(ptype: TParamType; pname, pvalue: string);
var
  tPrev, ParamTmp: PParamLexeme;
  i: Integer;
begin
  ParamTmp := @FirstParam;
  while ParamTmp.next <> nil do
    ParamTmp := ParamTmp.next;
    tPrev := ParamTmp;
    ParamTmp := VirtualAlloc(nil, SizeOf(TParamLexeme), MEM_COMMIT,
                      PAGE_READWRITE);
    tPrev^.next := ParamTmp;
    with ParamTmp^ do
      begin
        ParamType   := ptype;
        ParamName   := pname;
        ParamValue  := pvalue;
        next        := nil;
        prev        := tPrev;
        if ParamType = ptInteger then
          try
            Opt := StrToInt(pvalue);
          except
            on EConvertError do
              begin
                ErrorString := 'Invalid floating point value.';
                Exit;
              end;
          end;

        if ParamType = ptFloat then
          try
            for  i := 0 to strlen(pvalue)-1 do
              if pvalue[i] = '.' then
                pvalue[i] := Decimalseparator;
            Opt2 := StrToFloat(pvalue);
          except
            on EConvertError do
              begin
                ErrorString := 'Invalid floating point value.';
                Exit;
              end;
          end;

    end;
end;

function TObjectFileProps.AnalyzeString(sStr: string): boolean;
var
  iPos: Integer;
  ParamTmp: PParamLexeme;
begin
      iPos := 1;
    while (iPos <= Length(sStr))do
      begin
        SkipSpace(iPos, sStr);
        ExtractLexeme(iPos, sStr);
        if (ErrorString <> '')then
          begin
            ErrorString := '';
            Result := false;
            Exit;
          end;
    end;
    AddLexeme(ptLast, '', '');
    ParamTmp := FirstParam.next;
    Result := true;
end;

procedure TObjectFileProps.Assign(source: TObjectFileProps);
var
  ParamTmp: PParamLexeme;
begin
  source := source;
  BBOX := source.BBOX;
  FirstParam := source.FirstParam;
  MaskDC := source.MaskDC;
  MaskHandle := source.MaskHandle;
  if source.image <> nil then
    begin
      if image = nil then
        image := TBitmap.Create;
      image.Assign(source.image);
    end;
  if source.preview <> nil then
    begin
      if preview = nil then
        preview := TBitmap.Create;
      preview.Assign(source.preview);
    end;
  ErrorString := source.ErrorString;
end;

procedure TObjectFileProps.ClearList;
var
  tPrev: TParamLexeme;
  ParamTmp: PParamLexeme;
begin
  ParamTmp := @FirstParam;
  while ParamTmp.next <> nil do
    ParamTmp := ParamTmp.next;
  tPrev := ParamTmp^;
  while tPrev.ParamType <> ptFirst do
    begin
      VirtualFree(ParamTmp, 0, MEM_RELEASE);
      ParamTmp := tPrev.prev;
      tPrev := tPrev.prev^;
    end;
  ParamTmp := @FirstParam;
  with FirstParam do
    begin
      ParamType := ptFirst;
      ParamName := '';
      ParamValue := '';
      Opt := 0;
      next := nil;
      prev := nil;
    end;
end;

constructor TObjectFileProps.Create;
begin
  inherited Create;
  with FirstParam do
    begin
      ParamType := ptFirst;
      ParamName := '';
      ParamValue := '';
      Opt := 0;
      next := nil;
      prev := nil;
    end;
  Image := nil;
  preview := nil;
end;

Procedure TObjectFileProps.Free;
begin
  inherited Free;
  Image.Free;
  Preview.Free;
  ClearList;
end;

procedure TObjectFileProps.ExtractLexeme(var iPos: Integer; sStr: string);
var
 i: Integer;
 name, value: string;
 ptype: TParamType;
 Stack: TStack;
 OpCurBr, ClCurBr: char;
begin
  OpCurBr := '{';
  ClCurBr := '}';
  Stack := TStack.Create();
  if iPos > Length(sStr) then
    Exit;
  i := iPos;
  if sStr[i] = '#' then
    begin
      while (sStr[i] <> #13) and (iPos <= Length(sStr)) and (sStr[i] <> #10) do
        Inc(i);
      iPos := i;
      Exit;
    end;
  while sStr[i] in ['A'..'Z', 'a'..'z', '_', '1'..'9', '0'] do
    Inc(i);
  name := Copy(sStr, iPos, i - iPos);
  SkipSpace(i, sStr);
  if sStr[i] <> '=' then
    begin
      ErrorString := 'SyntaxError';
      Exit;
    end;
  Inc(i);
  SkipSpace(i, sStr);
  if sStr[i] = '"' then
    begin
      Inc(i);
      ptype := ptString;
    end
  else
    if sStr[i] = '{' then
      begin
        Inc(i);
        ptype := ptComplex;
      end
    else
      ptype := ptConst;

  if ptype = ptComplex then
    begin
      iPos := i;
      Stack.Push(@OpCurBr);
      while not Stack.Empty() do
        begin
          if sStr[i] = '{' then
            Stack.Push(@OpCurBr);
          if sStr[i] = '}' then
            if Stack.Pop() = @ClCurBr then
              begin
                Stack.Push(@ClCurBr);
                Stack.Push(@ClCurBr);                
              end;
          Inc(i);
        end;
    end
  else
    begin
      iPos := i;
      while (not (sStr[i] in ['"', ';'])) and
        (sStr[i] in ['0'..'9', 'a'..'z', 'A'..'Z', '-', '_', '.', ' ']) do
        begin
          Inc(i);
        end;
    end;

  value := copy(sStr, iPos, i-iPos);
  if (ptype = ptString) then
    if sStr[i] = '"' then
      Inc(i)
    else
      begin
        ErrorString := 'Syntax error: unclosed ''"''';
        exit;
      end;
  if sStr[i] = ';' then
    Inc(i)
  else
    if ptype <> ptComplex then
      begin
        ErrorString := 'Syntax error: missiing '';''';
        Exit;
      end;
  iPos := i;
  i := 1;
  if (ptype <> ptString) then
    begin
      while value[i] in ['0'..'9', '-', '.'] do
        Inc(i);
      if i = Length(value)+1 then
        pType := ptFloat;
      i := 1;        
      while value[i] in ['0'..'9', '-'] do
        Inc(i);
      if i = Length(value)+1 then
        pType := ptInteger;
    end;
  AddLexeme(pType, name, value);
  Stack.Free();
end;

procedure TObjectFileProps.FakeList;
begin
  FirstParam.next := nil;
  FirstParam.prev := nil;
end;

function TObjectFileProps.FindParam(DestName: string): PParamLexeme;
var
  ParamTmp: PParamLexeme;
begin
  result := nil;
  ParamTmp := @FirstParam;
  while (ParamTmp <> nil) and (ParamTmp^.ParamType <> ptLast) do
    begin
      if ParamTmp^.ParamName = DestName then
       begin
        Result := ParamTmp;
        ParamTmp := FirstParam.Next; 
        Exit;
       end;
      ParamTmp := ParamTmp^.next;
    end;
  Log('Error', 'Parameter %s not found in parameter list of %s prototype',
    [DestName, ProtoFileName]);
end;

function TObjectFileProps.GetError: string;
begin
  Result := ErrorString;
end;


function TObjectFileProps.GetImage: TBitmap;
begin
 if image = nil then
  begin
    Result := nil;
    ShowMessage('Error image has not been loaded');
    Exit;
  end
 else
  Result := image;
end;


function TObjectFileProps.GetPreview: TBitmap;
begin
 if (preview.width = 0) or (preview.Height = 0) then
  begin
    Result := Dildo; //nil;
    ///ShowMessage('Error preview has not been loaded');
    //Exit;
  end
 else
  Result := preview;
end;

function IdentifyDocType(FileName : string) : TDocumentType;
var
  sig : array [1..10] of char;
  image : file;
  i : Integer;
begin
  Result := notimage;
  try
    AssignFile(image, FileName);
    Reset(image, 1);
    blockread(image, sig, 10);
    for i := 1 to 10 do
      if sig[i] = #0 then
        sig[i] := #65;
    Result := notImage;
    for i := 1 to 3 do
      if AnsiContainsStr(sig, SIGNATURES[i]) = true then
        Result := TDocumentType(i-1);
    CloseFile(image);
  except
   on EInOutError do
      Log('Warning', 'Can'' opne file %s', [Filename]);
  end;
end;


function TObjectFileProps.LoadImage(): Boolean;
var
  ParamTmp: PParamLexeme;
  path: string;
  x, y, width, height: Integer;
  pngpic: TPNGImage;
  doctype: TDocumentType;
  filename: string;
begin
  Result := true;

  if image <> nil then
    begin
      image.Free;
      image := nil;
    end;
  if preview <> nil then
    begin
      preview.Free;
      preview := nil;
    end;

  Image := TBitmap.Create;
  preview := TBitmap.Create;

  SetLength(path, 1024);
  ParamTmp := FindParam('texture'); // »м€ файла текстуры без расширени€
  if ParamTmp = nil then
    begin
      result := false;
      Exit;
    end;
  GetModuleFileName(0, PAnsiChar(@path[1]), 1024);
  path := ExtractFilePath(path);
  SetCurrentDir(path);
  filename := path + 'textures\' + ParamTmp.ParamValue;
  doctype := IdentifyDocType(filename + '.bmp');
  if doctype = notimage then
    begin
      doctype := IdentifyDocType(filename + '.png');
      if doctype <> notimage then
      filename := filename + '.png';      
    end
  else
   filename := filename + '.bmp';

  case DocType of
    bmp1, bmp2:
      try
        Image.LoadFromFile(filename);
      except
        Log('Warning', 'Unable to load image %s. In object %s',
           ['textures\' + ParamTmp.ParamValue + '.bmp', FindParam('name')^.ParamValue]);
        Result := false;
        Exit;
      end;
    png:
      begin
        pngpic := TPNGImage.Create;
        try
          pngpic.LoadFromFile(filename);
          Image.Assign(pngpic);
        finally
          pngpic.Free;
        end;
      end;
  end;

  x := 0; y := 0; width := 0; height := 0;
  width := FindParam('frame_width').Opt;
  height := FindParam('frame_height').Opt;
  ParamTmp := FindParam('behaviour'); // ќн его не найдЄт, € гарантирую это.
  if (ParamTmp <> nil) and (ParamTmp.ParamValue = 'animated') then
    begin
      x := (FindParam('START_PATTERN').Opt mod (FindParam('IMAGE_WIDTH').Opt
        div width)) * width;
      y := (FindParam('START_PATTERN').Opt div (FindParam('IMAGE_WIDTH').Opt
        div width)) * height;
    end;
  preview.Width := width;
  preview.Height := height;
  preview.Canvas.CopyRect(Rect(0, 0, width, height), image.Canvas,
    Rect(x, y, x+ width, y + height));
  preview.TransparentColor := MAGIC_COLOR;
  preview.Transparent := true;
  MaskHandle := Preview.MaskHandle;
  MaskDC := CreateCompatibleDC(0);
  MaskHandle := SelectObject(MaskDC, MaskHandle);
  MaskHandle := (CreateCompatibleBitmap(preview.Canvas.Handle, 1, 1));
  MaskHandle  := SelectObject(MaskDC, MaskHandle);
  GenBBOX(preview, Rect(0, 0, preview.Width-1, preview.Height-1), BBOX);
  Image.Free;
  Image := nil;
end;

function TObjectFileProps.LoadFromFile(FileName: string): Boolean;
var
  Src: File;
  temp: String;
  chSym: char;
begin
  result := true;
  ProtoFileName := Copy(FileName, 1, StrLen(FileName));
  temp := '';
  ClearList;
  if FileName = '' then
    begin
      Result := false;
      Exit;
    end;
  try
    AssignFile(Src, FileName);
    Reset(Src, 1);
    while not Eof(Src) do
      begin
        BlockRead(Src, chSym, 1);
        temp := temp + chSym;
      end;
    if LuaFindCommentBeginWith(temp, STR_DO_NOT_LOAD) then
      begin
        Result := false;
        Exit;
      end;
    Result := AnalyzeString(temp);
    CloseFile(Src);
  except
    Result := false;
    ShowMessage('File Parse error');
    Exit;
  end;
end;

function TObjectFileProps.LoadFunctionsListFromINI(
  IniFileName: string): Boolean;
var
 IniFile: TIniFile;
begin
  IniFile := TIniFile.Create(IniFileName);
  IniFile.ReadString('General', 'ProtoPath', '');
//  IniFile.
end;

function TObjectFileProps.DumpToFile(FileName: string): Boolean;
var
  Dst: Text;
  ParamTmp: PParamLexeme;
begin
  ParamTmp := FirstParam.next;
  AssignFile(Dst, FileName);
  Rewrite(Dst);
  while ParamTmp^.ParamType <> ptLast do
    begin
      case ParamTmp^.ParamType of
        ptString:
          WriteLn(Dst, ParamTmp^.ParamName, ' = "', ParamTmp^.ParamValue +'";');
        ptComplex:
          WriteLn(Dst, ParamTmp^.ParamName, ' ='#13#10'{', ParamTmp^.ParamValue);
        else
          WriteLn(Dst, ParamTmp^.ParamName, ' = ', ParamTmp^.ParamValue, ';')
      end;
      ParamTmp := ParamTmp^.next;
    end;
  CloseFile(Dst);
  Result := true;
end;


procedure TObjectFileProps.SkipSpace(var iPos: Integer; sStr: string);
begin
  SkipLuaWhiteSpace(iPos, sStr);
end;

procedure TObjectFileProps.SuperMegaAssignTo(ADst: PObjectFileProps);
var
  srcTmp, dstTmp: PParamLexeme;
begin
  ADst.image := image;
  ADst.preview := preview;
  ADst.ErrorString := '';
  ADst.BBOX := BBOX;
  ADst.MaskDC := MaskDC;
  ADst.MaskHandle := MaskHandle;
  ADst^.ClearList();
  srcTmp := FirstParam.next;
  while srcTmp <> nil do
    begin
      with srcTmp^ do
        ADst.AddLexeme(ParamType, ParamName, ParamValue);
      srcTmp := srcTmp^.next;
    end;
end;

initialization
  SetLength(ModulePath, MAX_PATH);
  SetLength(ObjectsPath, MAX_PATH);
  SetLength(TexturesPath, MAX_PATH);

  if GetModuleFileName(0, PAnsiChar(@ModulePath[1]), 1024) = 0 then
    MsgLastErr();
  myExtractFilePath(ModulePath, ModulePath);
  myAppend(ObjectsPath, ModulePath, 'Objects\');
  myAppend(TexturesPath, ModulePath, 'Textures\');


  SetCurrentDir(TexturesPath);

  Dildo := TBitmap.Create;
  if FileExists('dildo2.bmp') then
    Dildo.LoadFromFile('dildo2.bmp')
  else
    begin
      Dildo.Width := 64;
      Dildo.Height := 64;
    end;

end.
