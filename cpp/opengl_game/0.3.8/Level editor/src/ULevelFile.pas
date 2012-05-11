unit ULevelFile;

interface

uses SysUtils, UObjectFile, types, Windows, Graphics, classes, consts, Utils,
  Math, UMetaData;

const
  DEFAULT_SNAP_THRESHOLD = 10;
  END_OF_USER_SCRIPT_PART = 'UserScriptEnd';  // метка в level-скрипте, что конца юзерскрипта достигли
  EOUSP_LENGTH = 13; // Длина той строки, что выше
  //NOTE: Lua чуствителен к регистру

type

  PObjectPropsNode = ^TObjectPropsNode;
  TObjectPropsNode = record
    SnapStateX: Boolean;  // состояние снэппинга, если true, то к данному объекту идёт снэп, иначе - нет
    SnapStateY: Boolean;
    SnappedX, SnappedY  : Boolean;
    HasBeenSnappedX, HasBeenSnappedY: Boolean;
    XSnapIndex, YSnapIndex: Integer;
    Selected : Boolean;
    x, px    : Integer;
    y, py    : Integer;
    width    : Integer;
    height   : Integer;
    prev     : PObjectPropsNode;
    next     : PObjectPropsNode;
    Proto    : PObjectFileProps;  // Указатель на соответствующ этому экземпляру прототип
    Params   : TObjectFileProps;  // Уникальный для этого объекта список параметров
    function GetX(): Integer;
    function GetY(): Integer;
    procedure SetX(AX: Integer);
    procedure SetY(AY: Integer);
  end;

  TLevelFile = class(TObject)
  private
    FirstObject: PObjectPropsNode;
    numObjects: cardinal;
    buffer: TBitmap;  // А это что??
    numTypes: cardinal;
    xpos, ypos: PInteger; // Что это за координаты, чего бля?
  public
    pScript: PAnsiChar;
    SnapThreshold: Integer;    
    procedure TrySnap();
    function GetSelectionUpperLeft(var x, y: Integer): Boolean;
    function numObjectsSelected(): Integer;
    procedure OffsetSelected(dx, dy: Integer);
    function PtInSelection(x, y: Integer): Boolean;
    function GetObjectRect(DstObj: TObjectPropsNode): TRect;
    procedure Clear();
    procedure ClearSelection();
    procedure SelectObjects(x1, y1, x2, y2: Integer);
    procedure AddToSelection(x1, y1, x2, y2: Integer);
    procedure ClearSelected();
    constructor Create(vObjects: TObjectFilePropsArr; vxpos, vypos: PInteger);
    function CountObjects():cardinal;
    function GetNumType(): cardinal;
    procedure Free();
    function AddObject(AX, AY: Integer): PObjectPropsNode;
    procedure DelObject(AObject: PObjectPropsNode);
    function SaveToFile(filename: string): Boolean;
    function LoadFromFile(filename: string): Boolean;
    function LoadScript(filename: string): Boolean;
    procedure DrawObjects(BoundingRect: TRect; canvas: TCanvas);
    function FirstSelected(): TObjectPropsNode;
  end;

implementation

{ Скопипастено из Graphics.pas
  Исключая тот факт, что временные битмапы
  не создаются во время выполнения. Также
  часть процедуры для старых версий венды удалена.
}

procedure OutOfResources;
begin
  raise EOutOfResources.Create(SOutOfResources);
end;

procedure GDIError;
var
  ErrorCode: Integer;
  Buf: array [Byte] of Char;
begin
  ErrorCode := GetLastError;
  if (ErrorCode <> 0) and (FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, nil,
    ErrorCode, LOCALE_USER_DEFAULT, Buf, sizeof(Buf), nil) <> 0) then
    raise EOutOfResources.Create(Buf)
  else
    OutOfResources;
end;

function GDICheck(Value: Integer): Integer;
begin
  if Value = 0 then GDIError;
  Result := Value;
end;

function MyTransparentStretchBlt(DstDC: HDC; DstX, DstY, DstW, DstH: Integer;
  SrcDC: HDC; SrcX, SrcY, SrcW, SrcH: Integer; maskhandle: HBITMAP; MaskX,
  MaskY: Integer): Boolean;
const
  ROP_DstCopy = $00AA0029;
begin
  Result := True;
  if (Win32Platform = VER_PLATFORM_WIN32_NT) and (SrcW = DstW) and (SrcH = DstH) then
  begin
      MaskBlt(DstDC, DstX, DstY, DstW, DstH, SrcDC, SrcX, SrcY, maskhandle, MaskX,
        MaskY, MakeRop4(ROP_DstCopy, SrcCopy));
      Exit;
  end;
end;

{ TLevelFile }

function TLevelFile.AddObject(AX, AY: Integer): PObjectPropsNode;
var
  cObj, tmp: ^PObjectPropsNode;
  ParamTmp: PParamLexeme;
begin
  // Вместо этого ебаного поиска конца ввести LastNode
  tmp := nil;
  cObj := @FirstObject;
  while cObj^ <> nil do
    begin
      tmp := cObj;
      cObj := @(cObj^.next);
    end;
  cObj^ := VirtualAlloc(nil, SizeOf(TObjectPropsNode), MEM_COMMIT,
  PAGE_READWRITE);

  with cObj^^ do
    begin
      Proto := ProtoSelected;
      // assign params: inherite from tpersistent and overload assign();
      Params := TObjectFileProps.Create();
      ParamTmp := Proto.FindParam('FunctionName');
      if ParamTmp <> nil then
        FunctionByName(ParamTmp.ParamValue)^.SuperMegaAssignTo(@Params);
      Params.FindParam('ProtoName').ParamValue := Proto.FindParam('name').ParamValue;
      SetX(AX);
      SetY(AY);
      width := Proto.GetPreview().Width;
      height := Proto.GetPreview().Height;
      selected := false;
      SnapStateX := true;
      SnapStateY := true;            
      SnappedX := false;
      SnappedY := false;
      HasBeenSnappedX := false;
      HasBeenSnappedY := false;                 
      next := nil;
      if cObj = @FirstObject then
        prev := nil
      else
        prev := tmp^;
    end;

//  Log('LEVEL', 'Object with ID %d of type %s added', [cObj^.objectID,
//    GetObjectByID(Prototypes, _typeID).FindParam('NAME').ParamValue]);
  Result := cObj^;
end;

procedure TLevelFile.AddToSelection(x1, y1, x2, y2: Integer);
var
  cObj: PObjectPropsNode;
  tmpRect: TRect;
begin
  cObj := FirstObject;
  While cObj <> nil do
    begin
      with cObj^ do
        if IntersectRect(tmpRect, Rect(x1, y1, x2, y2), Rect(x, y,
        x + width, y + height)) then
          cObj^.selected := not (cObj^.selected);
      cObj := cObj^.next;
    end;
end;



procedure TLevelFile.Clear;
var
  tmp, cObj: PObjectPropsNode;
begin
  cObj := FirstObject;
  if cObj = nil then
    Exit;
  while cObj^.next <> nil do
    cObj := cObj^.next;
  while cObj <> nil do
    begin
      tmp := cObj;
      cObj := cObj^.prev;
      DelObject(tmp);
    end;
end;

procedure TLevelFile.ClearSelected;
var
  tmp, cObj: PObjectPropsNode;
begin
  cObj := FirstObject;
  while cObj <> nil do
    begin
      tmp := cObj;
      cObj := cObj^.next;
      if tmp^.selected then
        DelObject(tmp);
    end;
end;

procedure TLevelFile.ClearSelection;
var
  cObj: PObjectPropsNode;
begin
  cObj := FirstObject;
  While cObj <> nil do
    begin
      cObj^.selected := false;;
      cObj := cObj^.next;
    end;
end;

function TLevelFile.CountObjects: cardinal;
var
  cObj: PObjectPropsNode;
begin
cObj := FirstObject;
Result := 0;
while cObj <> nil do
  begin
    Inc(Result);
    cObj := cObj^.next;
  end;
end;


constructor TLevelFile.Create(vObjects: TObjectFilePropsArr; vxpos, vypos: PInteger);
begin
  inherited Create;
  SnapThreshold := DEFAULT_SNAP_THRESHOLD;
  Prototypes := vObjects;
  Buffer := TBitmap.Create;
  FirstObject := nil;
  numObjects := 0;
  numTypes := 0;
  xpos := vxpos;
  ypos := vypos;
  pScript := nil;
end;

procedure TLevelFile.DelObject(AObject: PObjectPropsNode);
var
  temp: PObjectPropsNode;
begin
  temp := AObject;
  if temp^.prev <> nil then
    temp^.prev^.next := temp^.next;
  if temp^.next <> nil then
    temp.next^.prev := temp^.prev;

  if temp = FirstObject then
    FirstObject := FirstObject^.next;
  VirtualFree(temp, 0, MEM_RELEASE);
end;

procedure TLevelFile.DrawObjects(BoundingRect: TRect; canvas: TCanvas);
var
  srcRect, dstRect, tmpRect, GoodRect, BBox: TRect; //OMG...
  cObj: PObjectPropsNode;
begin
  with GoodRect do
    begin
      left   := MAXINT;
      Right  := -MAXINT;
      Top    := MAXINT;
      Bottom := -MAXINT;
    end;
  buffer.Width := BoundingRect.Right;
  buffer.Height := BoundingRect.Bottom;
  with buffer.Canvas do
    begin
      Brush.Color := clBlack;
      buffer.Canvas.FillRect(BoundingRect);

    // White cross in the middle of level
{
      pen.Color := clwhite;
      moveto(Round(GetWidth / 2) + xpos^,
        Round(GetHeight / 2) - 100 + ypos^);
      lineto(Round(GetWidth / 2) + xpos^,
        Round(GetHeight / 2) + 100 + ypos^);
      moveto(Round(GetWidth / 2) - 100 + xpos^,
        Round(GetHeight / 2) + ypos^);
      lineto(Round(GetWidth / 2) + 100 + xpos^,
        Round(GetHeight / 2) + ypos^);
}
      cObj := FirstObject;
      while cObj <> nil do
        begin
          with srcRect do
            begin
              left := 0;
              top := 0;
              right := CObj^.Proto.GetPreview().Width;
              bottom := CObj^.Proto.GetPreview().height;
            end;
          dstRect := Rect(cObj^.x , cObj^.y, srcRect.Right +
            cObj^.x, srcRect.Bottom + cObj^.y);
          OffsetRect(dstRect, xpos^, ypos^);
          if cObj^.selected then
            begin
              with GoodRect do
                begin
                  left    := Min(Left, cObj^.x + xpos^);
                  Right   := Max(Right, cObj^.x + cobj^.width + xpos^);
                  Top     := Min(Top, cObj^.y + ypos^);
                  Bottom  := Max(Bottom, cObj^.y + cObj^.height + ypos^);
                end;
            end;
          if IntersectRect(tmpRect, BoundingRect, dstRect) then
            begin

              MyTransparentStretchBlt(Handle, cObj^.x + xpos^, cObj^.y + ypos^, cObj^.width,
                cObj^.height, CObj^.Proto.GetPreview.Canvas.Handle, 0, 0,
                cObj^.width, cObj^.height, CObj^.Proto.MaskHandle, 0, 0);
{              if cObj^.selected then
                with cObj^ do
                  begin
                    Pen.width := 1;
                    Pen.Style := psDash;
                    Brush.Style := bsClear;
                    Pen.Color := RGB(125, 0, 90);
                    Rectangle(x+xpos^, y+ypos^, x+width+xpos^, y+height+ypos^);
                  end;}
            end;
          cObj := cObj^.next;
        end;

      cObj := FirstObject;
      while cObj <> nil do
        begin
          dstRect := Rect(cObj^.x , cObj^.y, srcRect.Right +
            cObj^.x, srcRect.Bottom + cObj^.y);
          OffsetRect(dstRect, xpos^, ypos^);
          if IntersectRect(tmpRect, BoundingRect, dstRect) then
            begin
              if cObj^.selected then
                with cObj^ do
                  begin
                    Pen.width := 1;
                    Pen.Style := psDot;
                    Brush.Style := bsClear;
                    Pen.Color := RGB(125, 0, 90);
                    Rectangle(x+xpos^, y+ypos^, x+width+xpos^, y+height+ypos^);
                  end;
                BBox := cOBj^.Proto.BBOX;
                with cObj^ do
                  begin
                    Pen.width := 1;
                    Pen.Style := psDot;
                    Brush.Style := bsClear;
                    Pen.Color := RGB(0, 90, 125);
                    Rectangle(x + BBox.Left+xpos^,y +  BBox.Top+ypos^,
                            x + BBox.Right+xpos^,y +  BBox.Bottom+ypos^);
                  end;
            end;
          cObj := cObj^.next;
        end;



      Pen.width := 1;
      Pen.Style := psDot;
      Brush.Style := bsClear;
      Pen.Color := RGB(240, 111, 20);
      Rectangle(GoodRect);

    end;
  canvas.Draw(0, 0, buffer);
end;

function TLevelFile.FirstSelected: TObjectPropsNode;
var
  cObj: PObjectPropsNode;
begin
  cObj := FirstObject;
  while (not CObj^.Selected) and (cObj <> nil) do
    cObj := cObj^.next;
  Result := cObj^;
end;

procedure TLevelFile.Free;
var
  temp, cObj: PObjectPropsNode;
begin
  inherited Free;
  Buffer.Free;
  cObj := FirstObject;
  while cObj <> nil do
    begin
      temp := cObj;
      cobj := cObj^.next;
//      Log('LEVEL', 'Object with ID %d of type %s deleted', [temp^.objectID,
//        GetObjectByID(Prototypes, temp^.typeID).FindParam('NAME').ParamValue]);
      VirtualFree(temp, 0, MEM_RELEASE);
    end;
end;


function TLevelFile.GetNumType: cardinal;
begin
  Result := numTypes;
end;

function TLevelFile.GetObjectRect(DstObj: TObjectPropsNode): TRect;
begin
  with DstObj, Result do
    begin
      Left   := x;
      Right  := x + width;
      Top    := y;
      Bottom := y + height;
    end;
end;

function TLevelFile.GetSelectionUpperLeft(var x, y: Integer): Boolean;
var
  cObj: PObjectPropsNode;
begin
  cObj := FirstObject;
  Result := false;
  if cObj = nil then
    Exit;
  x := MAXINT;
  y := MAXINT;
  while cObj <> nil do
    begin
      if cObj^.selected then
        begin
          if x > cObj^.x then
            x := cObj^.x;
          if y > cObj^.y then
            y := cObj^.y;
        end;
      cObj := cObj^.next;
    end;
  if (x = MAXINT) or (y = MAXINT) then
    Exit;
  Result := true;
end;


function TLevelFile.LoadFromFile(filename: string): Boolean;
var
  sfile: file;
  i, j, ArgCount: Integer;
  ch: char;
  FuncName, buffer, tmp: string;
  objTmp: PObjectPropsNode;
  paramTmp: PObjectFileProps;
  LexTmp: PParamLexeme;
  Args: array of string;
  ArgTypes: array of TParamType;

  function SkipSpace(): Boolean;
  begin
    while (not Eof(sFile)) and (ch in [' ',#13,#9,#10]) do
      begin
        BlockRead(sFile, ch, 1);
      end;
    Result := Eof(sFile);
  end;

begin
  Result := false;
  buffer := '';
  AssignFile(sfile, filename);
  Reset(sfile, 1);
  SetLength(tmp, EOUSP_LENGTH);
  while 1=1 do
    begin
      BlockRead(sFile, ch, 1);
      if Eof(sFile) then
        break;
      if ch <> '-' then
        buffer := buffer + ch
      else
        begin
          buffer := buffer + ch;
          BlockRead(sFile, ch, 1);
          if Eof(sFile) then
            break;
          if ch <> '-' then
            buffer := buffer + ch
          else
            begin
              buffer := buffer + ch;
              BlockRead(sFile, tmp[1], EOUSP_LENGTH);
              if CompareStr(tmp, END_OF_USER_SCRIPT_PART) <> 0 then
                begin
                  buffer := buffer + tmp;
                end
              else
                break;
            end;
        end;
    end;
  tmp := '';
  pScript := @(buffer[1]);

  while not Eof(sFile) do
    begin
      BlockRead(sFile, ch, 1);
      SkipSpace();

      while (ch <> #13) and (not Eof(sFile)) do
        begin
          tmp := tmp + ch;
          BlockRead(sFile, ch, 1);
        end;
      ArgCount := 1;
      for i := 1 to Length(tmp) do
        begin
          if tmp[i] = ',' then
            Inc(ArgCount);
        end;
      i := 1;
      while tmp[i] <> '(' do
        begin
          FuncName := FuncName + tmp[i];
          Inc(i);
        end;
      Inc(i);
      SetLength(Args, ArgCount);
      SetLength(ArgTypes, ArgCount);
      for j := 0 to ArgCount-1 do
        begin
          if tmp[i] in ['0'..'9', '-', '+'] then
            ArgTypes[j] := ptInteger
          else if tmp[i] = '"' then
            begin
              ArgTypes[j] := ptString;
              Inc(i)
            end
          else
            ArgTypes[j] := ptConst;
          while not (tmp[i] in [',', ')', '"']) do
            begin
              Args[j] := Args[j] + Tmp[i];
              Inc(i);
            end;
          case tmp[i] of
            ',':
              Inc(i, 2);
            ')':
              ;
            '"':
              Inc(i, 3);
          end;
        end;
      tmp := '';
                 // Вот тут (где-то тут) имена ф-й хорощо бы кешировать, и если совпадают, то ебаный
                 // квадратичный поиск много лишних раз не вызывать  // хотя нет!
      ProtoSelected := ProtoByName(Args[0]);
      objTmp := AddObject(0, 0);
      FuncName := '';

      LexTmp := objTmp.Params.FirstParam.next.next;
      for i := 0 to ArgCount-1 do
        begin
          if LexTmp^.ParamName = 'x' then
            begin
              objTmp.x := StrToInt(Args[i]);
            end;
          if LexTmp^.ParamName = 'y' then
            begin
              objTmp.y := StrToInt(Args[i]);
            end;
          LexTmp.ParamValue :=Args[i];
          LexTmp.ParamType :=ArgTypes[i];
          LexTmp := LexTmp.next;
          Args[i] := '';
        end;
      // Prototypes.
      //ID := GetTypeIDbyName(Prototypes, name);
      //AddObject(ID, x, y, p1, p2);
    end;

  CloseFile(sfile);
  Result := true;
  
  Exit;
  //LoadScript(filename);
end;

function TLevelFile.LoadScript(filename: string): Boolean;
var
  sfile: file;
  chcount, offset: Integer;
  ch: char;
  pch: PCHAR;
  SIG: array [0..2] of char;
  script: array of char;
begin
  AssignFIle(sfile, filename);
  reset(sfile, 1);
  blockread(sfile, ch, 1);
  while ch <> 'E' do
    begin
      blockread(sfile, ch, 1);
      if ch = 'E' then
        begin
          SIG[0] := ch;
          BlockRead(sfile, ch, 1);
          SIG[1] := ch;
          BlockRead(sfile, ch, 1);
          SIG[2] := ch;

          if sig = 'EOL' then
            begin
                offset := FilePos(sfile);
                break;
            end;
        end;
    end;
  chcount := FileSize(sfile) - offset;
  SetLength(script, chcount+1);
  pch := @(script[0]);
  blockread(sfile, pch^, chcount);
  script[chcount] := #0;

  pScript := pch;//pch;
  CloseFile(sfile);
  Result := true;
end;

function TLevelFile.numObjectsSelected: Integer;
var
  cObj: PObjectPropsNode;
  counter: Integer;
begin
  Result := -1;
  cObj := FirstObject;
  if (cObj = nil) then
    Exit;
  Counter := 0;
  while cObj <> nil do
    begin
      if cObj^.selected then
        Inc(counter);
      cObj := cObj^.next;
    end;
end;

procedure TLevelFile.OffsetSelected(dx, dy: Integer);
var
  cObj: PObjectPropsNode;
begin
  cObj := FirstObject;
  if cObj = nil then
    Exit;
  while cObj <> nil do
    begin
      if cObj^.selected then
        begin
          cObj^.SetX(cObj^.GetX() + dx);
          cObj^.SetY(cObj^.GetY() + dy);
        end;
      cObj := cObj^.next;
    end;
end;

function TLevelFile.PtInSelection(x, y: Integer): Boolean;
var
  cObj: PObjectPropsNode;
begin
  Result := true;
  cObj := FirstObject;
  if cObj = nil then
    begin
      Result := false;
      Exit;
    end;
  while cObj <> nil do
    begin
      if cObj^.selected then
        if ptinrect(Rect(cObj^.x, cObj^.y, cObj^.x + cObj^.width, cObj^.y
           + cObj^.height), Point(x, y)) then
           Exit;
      cObj := cObj^.next;
    end;
  Result := false;
end;

function TLevelFile.SaveToFile(filename: string): Boolean;
var
  dfile: file;
  strtmp: string;
  slen, i, j: Integer;
  Yes: Boolean;
  chtmp: char;
  tmpObj: PObjectPropsNode;
  typec: cardinal;
  LexTmp: PParamLexeme;
  ExTypes: array of Integer;
  numExTypes: Integer;
  cObj: PObjectPropsNode;
begin
  cObj := FirstObject;
  chtmp := #0;
  AssignFile(dfile, filename);
  rewrite(dfile, 1);
  Yes := false;
  numExTypes := 0;
  //blockwrite(dfile, numTypes, sizeof(numTypes));

  BlockWrite(dfile, pScript^, Length(pScript));
  BlockWrite(dfile, #13#10'--' + END_OF_USER_SCRIPT_PART+#13#10, EOUSP_LENGTH+6);

  SetLength(ExTypes, numTypes);


  while CObj <> nil do
    begin
//      for i := 0 to numExTypes-1 do
  //      if ExTypes[i] = CObj^.typeID then
    //      Yes := true;
    //  if not Yes then
     //   begin
      //    Inc(numExTypes);
       //   ExTypes[numExTypes-1] := cObj^.typeID;
       // end;
//      Yes := false;

      strtmp := cObj.Params.FirstParam.next^.ParamValue + '(';
      LexTmp := cObj.Params.FirstParam.next^.next;

      while LexTmp <> nil do
        begin
          if (LexTmp.ParamValue <> '') and
             (LexTmp.prev.ParamName <> 'name') then
            strtmp := strtmp + ', ';
          if LexTmp.ParamValue <> '' then
            begin
              if LexTmp.ParamType = ptString then
                strtmp := strtmp + '"';
                strtmp := strtmp + LexTmp.ParamValue;
              if LexTmp.ParamType = ptString then
                strtmp := strtmp + '"';
            end;
            if LexTmp.next = nil then
              strtmp := strtmp + ');'#13#10;
          LexTmp := LexTmp.next;
        end;


      slen := strLen(PAnsiChar(strtmp));
      for j := 1 to slen do
        blockwrite(dfile, strtmp[j], 1);


      CObj := CObj^.next;
    end;
              {
  for i := 0 to numExTypes-1 do
    begin
      CObj := FirstObject;
      while (CObj <> nil) and (CObj^.typeID <> ExTypes[i]) do
        CObj := CObj.next;

      strtmp := 'CreateSprite("' + GetObjectByID(Prototypes, CObj^.typeID).FindParam('name').ParamValue +
      ', ';
      slen := strLen(PAnsiChar(strtmp));
      for j := 1 to slen do
        blockwrite(dfile, strtmp[j], 1);
      blockwrite(dfile, chtmp, 1);
      typec := CountObjectsType(cObj^.typeID);
      blockwrite(dfile, typec, 4);

        tmpObj := FirstObject;
        while tmpObj <> nil do
          begin
            if tmpObj^.typeID = ExTypes[i] then
              begin
                blockwrite(dfile, tmpObj^.x, 4);
                blockwrite(dfile, tmpObj^.y, 4);
                blockwrite(dfile, tmpObj^.param1, 2);
                blockwrite(dfile, tmpObj^.param2, 2);
                blockwrite(dfile, tmpObj^.objectID, 4);
              end;
            tmpObj := tmpObj^.next;
          end;
    end;
  BlockWrite(dfile, 'EOL', 3);  }      
  // тра-та та-та та-та та...хитрый план ебать кота
  CloseFile(dfile);
  Result := true;
end;

procedure TLevelFile.SelectObjects(x1, y1, x2, y2: Integer);
var
  cObj: PObjectPropsNode;
  tmpRect: TRect;
begin
  cObj := FirstObject;
  While cObj <> nil do
    begin
      with cObj^ do
        if IntersectRect(tmpRect, Rect(x1, y1, x2, y2), Rect(x, y,
        x + width, y + height)) then
          cObj^.selected := true;
      cObj := cObj^.next;
    end;
end;


function SegIn(a1, a2, b1, b2: Integer): Boolean;
begin
  Result := true;
  if (a2 < b1) or (b2 < a1) then
    Result := false;
end;


procedure TLevelFile.TrySnap;
var
  cObj, srcObj: PObjectPropsNode;
  b1, b2: TRect;  // Короткие имена здесь, потому что иначе слишком громоздко и
  p1, p2: TPoint;  // нихрена не понятно 1 - src 2 - всё остальное
  i: Integer;
  
  function DistX(index: Integer): Integer;
  begin
    case index of
      1:  Result := abs( p2.x + b2.Left - p1.x - b1.right );
      2:  Result := abs( p2.x + b2.Right - p1.x - b1.Left );
      3:  Result := abs( p2.x + b2.Left - p1.x - b1.left );
      4:  Result := abs( p2.x + b2.Right - p1.x - b1.Right );
    end;
  end;
  function DistY(index: Integer): Integer;
  begin
    case index of
      1:  Result := abs( p2.y + b2.Top - p1.y - b1.Bottom );
      2:  Result := abs( p2.y + b2.Bottom - p1.y - b1.Top );
      3:  Result := abs( p2.y + b2.Top - p1.y - b1.Top );
      4:  Result := abs( p2.y + b2.Bottom - p1.y - b1.Bottom );
    end;
    end;

  procedure SetX(index: Integer);
  begin
    case index of
      1: srcObj^.x := p2.x + b2.Left - b1.Right;
      2: srcObj^.x := p2.x + b2.Right - b1.Left;
      3: srcObj^.x := p2.x + b2.Left;
      4: srcObj^.x := p2.x + b2.Right - b1.Right;
    end;
    cObj^.SnapStateX := false;
    cObj^.XSnapIndex := index;
    srcObj^.SnappedX := true;
  end;

  procedure SetY(index: Integer);
  begin
    case index of
      1: srcObj^.y := p2.y + b2.Top - b1.Bottom;
      2: srcObj^.y := p2.y + b2.Bottom - b1.Top;
      3: srcObj^.y := p2.y + b2.Top;
      4: srcObj^.y := p2.y + b2.Bottom - b1.Bottom;
    end;
    cObj^.SnapStateY := false;
    cObj^.YSnapIndex := index;
    srcObj^.SnappedY := true;    
  end;

begin
  cObj := FirstObject;
  srcObj := nil;
  While cObj <> nil do
    begin
      if cObj^.selected then
        begin
          srcObj := cObj;
          break;
        end;
      cObj := cObj^.next;
    end;
  if srcObj = nil then
    Exit;

  b1 := srcObj^.Proto.BBOX;
  if srcObj^.SnappedX then
    p1.x := srcObj^.px
  else
    p1.x := srcObj^.x;
  if srcObj^.SnappedY then
    p1.y := srcObj^.py
  else
    p1.y := srcObj^.y;

  cObj := FirstObject;
  While cObj <> nil do
    begin
      if cObj = srcObj then
        begin
          cObj := cObj.next;
          Continue;
        end;
      b2 := cObj^.Proto.BBOX;
      p2 := Point(cObj^.x, cObj^.y);

      case cObj^.SnapStateX of
        true:
          if SegIn(p2.y + b2.Top, p2.y + b2.Bottom, p1.y + b1.Top, p1.y + b1.Bottom) then
            for i := 1 to 4 do
              if DistX(i) < SnapThreshold then
                begin
                  SetX(i);
                  break;
                end;
        false:
          if DistX(cObj^.XSnapIndex) > SnapThreshold then
            begin
              cObj^.SnapStateX := true;
              srcObj^.SnappedX := false;
            end;
      end;

      case cObj^.SnapStateY of
        true:
          if SegIn(p2.x + b2.Left, p2.x + b2.Right, p1.x + b1.Left, p1.x + b1.Right) then
            for i := 1 to 4 do
              if DistY(i) < SnapThreshold then
                begin
                  SetY(i);
                  break;
                end;
        false:
          begin
          if DistY(cObj^.YSnapIndex) > SnapThreshold then
            begin
              cObj^.SnapStateY := true;
              srcObj^.SnappedY := false;
            end;
          end;
      end;

      cObj := cObj^.next;
    end;

end;


{ TObjectPropsNode }

function TObjectPropsNode.GetX: Integer;
begin
  if SnappedX and HasBeenSnappedX  then
    Result := px
  else
    Result := x;
end;

function TObjectPropsNode.GetY: Integer;
begin
  if SnappedY and HasBeenSnappedY then
    Result := py
  else
    Result := y;
end;

procedure TObjectPropsNode.SetX(AX: Integer);
var
  ParamTmp: PParamLexeme;
begin
  if SnappedX and HasBeenSnappedX then
    px := AX
  else if (HasBeenSnappedX) and (not SnappedX) then
    begin
      x := px;
      HasBeenSnappedX := false;
    end
  else if (SnappedX) and (not HasBeenSnappedX) then
    begin
      px := AX;
      HasBeenSnappedX := true;
    end
  else
      x := AX;
  ParamTmp := nil;
  if Params <> nil then
    ParamTmp := Params.FindParam('x');
  if ParamTmp <> nil then
    begin
      ParamTmp.ParamValue := IntToStr(x);
      ParamTmp.ParamType := ptInteger;
    end;
end;

procedure TObjectPropsNode.SetY(AY: Integer);
var
  ParamTmp: PParamLexeme;
begin
  if SnappedY and HasBeenSnappedY then
    py := AY
  else if (HasBeenSnappedY) and (not SnappedY) then
    begin
      y := py;
      HasBeenSnappedY := false;
    end
  else if (SnappedY) and (not HasBeenSnappedY) then
    begin
      py := AY;
      HasBeenSnappedY := true;
    end
  else
      y := AY;

  ParamTmp := nil;
  if Params <> nil then
    ParamTmp := Params.FindParam('y');
  if ParamTmp <> nil then
    begin
      ParamTmp.ParamValue := IntToStr(y);
      ParamTmp.ParamType := ptInteger;
    end;
end;

end.
