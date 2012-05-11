unit misc;

interface

uses header;

function SelectionByIndex(Map: TMap; Index: LongInt): LongInt;
function UnderCursor(Map: TMap; Cursor: TGameCursor): Longint;
function PointInArea(PointX, PointY, AreaX, AreaWidth, AreaY, AreaHeight: Single): boolean;
function FindByIndex(Map: TMap; Index: Integer): Integer;
function MaskColor(X,Y,MaskWidth,MaskHeight: real): TMapColor;
function ScreenToIso(X,Y: integer): TIsoPoint;
function IsoToScreen(P: TIsoPoint): TPoint; overload;
function IsoToScreen(A,B: integer): TPoint; overload;
function GetAngle(x0, y0: real; x1, y1: real): real;
function PlayerByIndex(Map: TMap; Index: Integer): Integer;
function FirstIndexByRadius(Map: TMap; CX, CY: real; Radius: integer): integer;
function PointInRadius(x,y: real; cx,cy: real; radius: integer): boolean;
function FirstOtherPlayerIndexByRadius(Map: TMap; PlayerIndex: integer; CX, CY: real; Radius: integer): integer;

implementation

uses SysUtils,dialogs,  math, main_editor;

const
  Diag = 33.5; // length of diagonal in mask (uses for converting coord)

// Возвращает цвет заданной точки на маске
function MaskColor(X,Y,MaskWidth,MaskHeight: real): TMapColor;
var
  Q: byte;
  Alpha: real;
  Beta: real;
  OX,OY: real;
  EX,EY: real;
  D: real;
begin
  if (X >= MaskWidth / 2) then
  begin
    if (Y >= MaskHeight / 2)
    then
      Q := 4
    else
      Q := 1;
  end
    else
      begin
        if (Y >= MaskHeight / 2)
        then
          Q := 3
        else
          Q := 2;
      end;


  if (Q = 3) then
  begin
    OX := MaskWidth / 2;
    OY := MaskHeight;
    EX := 0;
    EY := MaskHeight / 2;
    Alpha := RadToDeg(Arcsin(EY / Diag));
    D := Sqrt(Sqr(MaskHeight-Y)+Sqr(OX-X));
    Beta  := RadToDeg(Arcsin((MaskHeight-Y) / D));
    if Beta >= Alpha then Result := mcWhite else Result := mcLime;
  end;

  if (Q = 2) then
  begin
    OX := MaskWidth / 2;
    OY := 0;
    EX := 0;
    EY := MaskHeight / 2;
    Alpha := RadToDeg(Arcsin(EY / Diag));
    D := Sqrt(Sqr(Y)+Sqr(OX-X));
    Beta  := RadToDeg(Arcsin(Y / D));
    if Beta >= Alpha then Result := mcWhite else Result := mcRed;
  end;

  if (Q = 1) then
  begin
    OX := MaskWidth / 2;
    OY := 0;
    EX := MaskWidth;
    EY := MaskHeight / 2;
    Alpha := RadToDeg(Arcsin(EY / Diag));
    D := Sqrt(Sqr(Y)+Sqr(OX-X));
    Beta  := RadToDeg(Arcsin(Y / D));
    if Beta >= Alpha then Result := mcWhite else Result := mcYellow;
  end;

  if (Q = 4) then
  begin
    OX := MaskWidth / 2;
    OY := MaskHeight;
    EX := MaskWidth;
    EY := MaskHeight / 2;
    Alpha := RadToDeg(Arcsin(EY / Diag));
    D := Sqrt(Sqr(MaskHeight-Y)+Sqr(OX-X));
    Beta  := RadToDeg(Arcsin((MaskHeight-Y) / D));
    if Beta >= Alpha then Result := mcWhite else Result := mcBlue;
  end;

end;

function ScreenToIso(X,Y: integer): TIsoPoint;
var
  i,j,k,l:integer;
  RegionX,
  RegionY,
  MouseMapX,
  MouseMapY: real;
  Color: TMapColor;
  DX,
  DY: integer;
begin
  // mouse mapping method
  // we are using mouse mapping method to convert screen coord. to Iso coord.
  RegionX := Int(X / TileWidth);
  RegionY := Int(Y / TileHeight);

  MouseMapX := X mod TileWidth;
  MouseMapY := Y mod TileHeight;

  Color := MaskColor(MouseMapX, MouseMapY, TileWidth, TileHeight);

  case Color of
    mcWhite: begin DX := 0; DY := 0; end;
    mcRed:   begin DX := -1; DY := -1; end;
    mcLime:  begin DX := -1; DY := 1; end;
    mcBlue:  begin DX := 1; DY := 1; end;
    mcYellow: begin DX := 1; DY := -1; end;
  end;

  i := Trunc(RegionX) * TileWidth;
  j := DX * (TileWidth div 2);

  k := Trunc(RegionY) * TileHeight;
  l := DY * (TileHeight div 2);

  Result.I := trunc(i+j) div (TileWidth div 2);
  Result.J := trunc(k+l) div (TileHeight div 2);

end;

function IsoToScreen(P: TIsoPoint): TPoint; overload;
begin
  Result.X := P.I * TileWidth div 2 + MapDX;
  Result.Y := P.J * TileHeight div 2 + MapDY;
end;

function IsoToScreen(A,B: integer): TPoint; overload;
begin
  Result.X := A * TileWidth div 2 + MapDX;
  Result.Y := B * TileHeight div 2 + MapDY;
end;

// возвращает TRUE если точка попадает в заданный прямоугольник
function PointInArea(PointX, PointY, AreaX, AreaWidth, AreaY, AreaHeight: Single): boolean;
begin
  Result := FALSE;
  if (PointX >= AreaX) then
    if (PointX <= AreaX + AreaWidth) then
      if (PointY >= AreaY) then
        if (PointY <= AreaY + AreaHeight) then
          Result := TRUE;
end;

function SelectionByIndex(Map: TMap; Index: LongInt): LongInt;
var
  i: LongInt;
  l: LongInt;
  pred: LongInt;
begin
  l := Length(Map.SelectedChars);
  pred := l - 1;
  Result := - 1;
  for i := 0 to pred do
  begin
    if Map.SelectedChars[i] = Index then
    begin
      Result := i;
      Exit;
    end;
  end;
end;

function PlayerByIndex(Map: TMap; Index: Integer): Integer;
var
  lp: integer;
  lpp: integer;
  i,j: integer;
  lc: integer;
  lcp: integer;
begin
  lp := Length(Map.Players);
  lpp := lp - 1;
  for i := 0 to lpp do
  begin
    lc := Length(Map.Players[i].Characters);
    lcp := lc - 1;
    for j := 0 to lcp do
      if Map.Players[i].Characters[j] = Index then
      begin
        Result := i;
        Exit;
      end;

  end;
end;

function FindByIndex(Map: TMap; Index: Integer): Integer;  // ищет индекс персонажа в массиве по его личному index
var i,l,p: integer;
begin
  l := Length(Map.Characters);
  p := l - 1;
  for i := 0 to p do if Map.Characters[i].Index = Index then
  begin
    Result := i;
    Exit;
  end;
end;

function UnderCursor(Map: TMap; Cursor: TGameCursor): Longint;
var
  i: LongInt;
  L: LongInt;
begin
  L := Length(Map.Characters);
  Result := -1;
  Cursor.ImageName := 'cursor_hand';
  Cursor.DoAnimate := FALSE;
  for i := 0 to L - 1 do
  begin
    with Map.Characters[i] do
    if (Cursor.X >= X + PatternWidth + OffsetX - PatternWidth div 2 - RIX div 2) and
       (Cursor.X <= X + PatternWidth + OffsetX - PatternWidth div 2 + RIX div 2) and
       (Cursor.Y >= Y + PatternHeight + OffsetY - PatternHeight div 2 - RIY div 2) and
       (Cursor.Y <= Y + PatternHeight + OffsetY - PatternHeight div 2 + RIY div 2) then
      begin
        Result := Index;

        Cursor.X := X - PatternWidth div 2+28;
        Cursor.Y := Y - PatternHeight div 2-8;

        if (PlayerByIndex(Map, Index) = HumanPlayer)
        then
          Cursor.ImageName := 'selection.image'
        else
          Cursor.ImageName := 'selection_enemy.image';

        Cursor.DoAnimate := TRUE;
        Cursor.SetAnim(Cursor.ImageName,0,4,0.05,TRUE);
        Exit;
      end;
  end;
end;

// 1 0 7
// 2   6
// 3 4 5

function GetAngle(x0, y0: real; x1, y1: real): real;
begin
  Result := ArcTan2(y0 - y1, x1 - x0) * 180 / Pi;
  if Result < 0 then
    Result := Result + 360;
end;

function PointInRadius(x,y: real; cx,cy: real; radius: integer): boolean;

begin
  //Result := False;
  //if Sqrt(Sqr(cx-x)+Sqr(cy-y)) <= radius then Result := True;
  Result := (Sqrt(Sqr(cx-x)+Sqr(cy-y)) <= Radius);
end;


// возвращает личный индекс первого персонажа, располагающегося в окружности
// заданного радиуса от заданного центра
function FirstIndexByRadius(Map: TMap; CX, CY: real; Radius: integer): integer;
var
  i: integer;
  cl: integer;
  clp: integer;
begin
  cl := Length(Map.Characters);
  clp := cl - 1;

  Result := -1; // failed to find

  for i := 0 to clp do
  begin
    if PointInRadius(Map.Characters[i].X, Map.Characters[i].Y, CX, CY, Radius) then
    begin
      Result := Map.Characters[i].Index;
      Exit;
    end;
  end;

end;

// возвращает личный индекс первого персонажа, принадлежащего другому игроку,
// располагающегся в окружности заданного радиуса от заданного центра
function FirstOtherPlayerIndexByRadius(Map: TMap; PlayerIndex: integer; CX, CY: real; Radius: integer): integer;
var
  i: integer;
  cl: integer;
  clp: integer;
begin
  cl := Length(Map.Characters);
  clp := cl - 1;

  Result := -1; // failed to find

  for i := 0 to clp do
  begin
    if PointInRadius(Map.Characters[i].X, Map.Characters[i].Y, CX, CY, Radius) then

      if (PlayerByIndex(Map, Map.Characters[i].Index) <> PlayerIndex) then
      begin
        Result := Map.Characters[i].Index;
        Exit;
      end;

  end;

end;


end.
