unit header;

interface

uses AsphyreSprite,sysutils,dialogs;

type
  IntArray = array of integer;                               

  TMapColor = (mcRed, mcLime, mcBlue, mcYellow,  mcWhite);

  TIsoPoint = record
    I,
    J: integer;
  end;

  TPoint = record
    X,
    Y: integer;
  end;

  TCharacterType = (ctKnight, ctBarbar, ctViking, ctArcher, ctDwarf);
  TCharacterState = (csNone, csStand, csAttack, csBeenHit, csRunning, csTippingOver);
  TCursorType = (ctHand);
  TSelectionType = (stRed, stGreen);
  TSelectionMethod = (smAdd, smSingle, smRemove, smReset);
  TAI = (aiNone, aiHumanControl, aiDefense, aiAttack);

  TMovingSprite = class(TAnimatedSprite)
    private

    public
      NX: Single;
      NY: Single;

      Angle: Single;
      NewAngle: Single;
      Direction: integer;

      CurrentState: TCharacterState;
      NextState: TCharacterState;

      DeltaX: Single;
      DeltaY: Single;
      Speed: Single;

      CurrentTile: TIsoPoint;
      TempTile: TIsoPoint;
      PreviousTile: TIsoPoint;

      procedure MoveTo(CX,CY: Single); virtual;
      procedure MoveToTile(ix, iy: integer); virtual;
      procedure AttackTile(ix,iy: integer); virtual;
      procedure Move(const MoveCount: Single); override;
      procedure SetStandDirection; virtual;
      procedure SetMoveDirection; virtual;
      procedure SetAttackDirection; virtual;
      procedure CheckState;
    end;

  TCharacter = class(TMovingSprite)
    private
      constructor Create(CharacterType: TCharacterType);

    public
      NorthStand: word;
      EastStand: word;
      SouthStand: word;
      WestStand: word;
      NorthEastStand: word;
      NorthWestStand: word;
      SouthEastStand: word;
      SouthWestStand: word;

      NorthRun: word;
      EastRun: word;
      SouthRun: word;
      WestRun: word;
      NorthEastRun: word;
      NorthWestRun: word;
      SouthEastRun: word;
      SouthWestRun: word;

      NorthAttack: word;
      EastAttack: word;
      SouthAttack: word;
      WestAttack: word;
      NorthEastAttack: word;
      NorthWestAttack: word;
      SouthEastAttack: word;
      SouthWestAttack: word;

      NorthHit: word;
      EastHit: word;
      SouthHit: word;
      WestHit: word;
      NorthEastHit: word;
      NorthWestHit: word;
      SouthEastHit: word;
      SouthWestHit: word;

      RIX: word; // real image size x
      RIY: word; // real image size y

      // colliding
      CollideRadius: word;
      CollideCX: single;
      CollideCY: single;

      // parametres
      Health: integer;
      Armor: integer;
      Accuracy: integer; // acc. 0-100%
      DamageMin: integer;
      DamageMax: integer;
      Invisible: boolean;
      Hostile: boolean;

      // stats
      Kills: integer;
      XP: integer;

      // AI
      AI: TAI;
      ActionRadius: integer;
      VictimIndex: integer;
      VictimX: integer;
      VictimY: integer;

      Hitted: boolean;
      Hitting: boolean;


      CharacterType: TCharacterType;
      Index: integer;

      procedure SetStandDirection; override;
      procedure SetMoveDirection; override;
      procedure SetAttackDirection; override;

      procedure ProcessStates;

      procedure Move(const MoveCount: Single); override;
      procedure MoveTo(CX,CY: Single); override;
      procedure MoveToTile(ix,iy: integer); override;
      procedure AttackTile(ix,iy: integer); override;

      procedure AttackCharacter(index: integer; CX, CY: integer);
    end;

  TGameCursor = class(TAnimatedSprite)
    private

    public
      CursorType: TCursorType;
      constructor Create(CursorType: TCursorType);
    end;

  TSelection = class(TMovingSprite)
    public
      constructor Create(T: TSelectionType; Spd: Single);

    end;

  TPlayer = class(TObject)
    private

    public
      Index: integer;
      Characters: array of Integer;
      SelectedChars: array of Integer;
      constructor Create(i: integer);
    end;

  TMapTile = class(TSprite)
    private

    public
      DefaultCost: integer;

      Cost: integer;
      Empty: boolean;
      Enabled: boolean; // enabled for walking-on true/false

      constructor Create;
    end;

  TMap = class(TObject)
    private

    public
      SizeX: Longword; // number of cells on X
      SizeY: Longword; // number of cells on Y

      Characters: array of TCharacter; // all characters on map
      Cursor: TGameCursor; // just cursor
      SelTile: TSprite; // cell selection sprite
      TileMask: TSprite; // mask for converting screen coord. to iso. coord.

      Selections: array of TSelection; // all selections

      SelectedChars: IntArray; // indexes of selected characters

      SelectCandidat: integer; // index of character-under-cursor

      SSel: boolean; // selectioning now true/false
      SSelX1: integer; // selection frame X1
      SSelY1: integer; // selection frame Y1
      SSelX2: integer; // selection frame X2
      SSelY2: integer; // selection frame Y2

      MX, MY: integer; // screen mouse coord.
      a,b: integer; // iso. coord.

      SelStr: string; // indexes of selection (DEBUG)
      SelFrmStr: string; // indexes of characters under selection frame (DEBUG)

      Players: array of TPlayer; // all players

      RenderTiles: boolean;

      Tiles: array of array of TMapTile; // all tiles


      constructor Create(SizeX, SizeY: word);
      destructor Destroy; override;
      procedure CreateCharacter(Player: TPlayer; CharType: TCharacterType; PosX, PosY: Single);
      procedure CreateCharacterAt(Player: TPlayer; CharType: TCharacterType; Pos: TIsoPoint); overload;
      procedure CreateCharacterAt(Player: TPlayer; CharType: TCharacterType; A,B: Integer); overload;
      procedure CreateSelection(Player: integer; Method: TSelectionMethod; Tp: TSelectionType; Index: LongInt);

      procedure ProcessSelections;
      procedure ProcessSelectFrame;
      procedure ProcessCursor;
      procedure ProcessMovements;
      procedure ProcessAI;

      function SelectExists(Index: LongInt): boolean;
    end;

  TGuiPanel = class(TSprite)
   public
      Portrait: TSprite;
      HealthBar: TSprite;
      ArmorPic: TSprite;
      WeaponPic: TSprite;
      Health: integer;
      Kills: integer;
      XP: integer;
      Armor: integer;
      DamageMin: integer;
      DamageMax: integer;

      constructor Create;
  end;

var
  SpriteEngine: TSpriteEngine;

const
  HumanPlayer = 0;

  TileWidth = 60;
  TileHeight = 30;
  MapDX = 30;
  MapDY = 15;

  TextBarLife = 0.5;

  // Z-order
  Z_Tile = 0;
  Z_Character = 1;
    {...}
  Z_SelTile = 1000;
  Z_Selection = 1001;

  Z_Panel = 2000;
  Z_Panel_Comp = 2001;

  Z_Cursor = 3000;
implementation

uses main_editor, misc, math;

// --------------------- TMapTile ------------------------

constructor TMapTile.Create;
begin
  inherited Create(SpriteEngine);
  Empty := True;
  Enabled := True;
  DefaultCost := 1;
  Cost := DefaultCost;
  Z := Z_Tile;
end;

// -------------------------------------------------------

// --------------------- TPlayer -------------------------

constructor TPlayer.Create(i: integer);
begin
  SetLength(Characters, 0);
  Index := i;
end;

// --------------------------------------------------------

// ------------------------- TCharacter --------------------

constructor TCharacter.Create(CharacterType: TCharacterType);
begin
  CurrentState := csStand;
  inherited Create(SpriteEngine);

  if CharacterType = ctKnight then
  begin
    AI := aiDefense;
    ImageName := 'knight_battle.image';
    OffsetY := -95;
    OffsetX := -65;
    Health := 100;
    Speed := 0.5;
    Z := Z_Character;

    EastStand := 0;
    NorthStand := 12;
    NorthEastStand := 24;
    NorthWestStand := 36;
    SouthStand := 48;
    SouthEastStand := 60;
    SouthWestStand := 72;
    WestStand := 84;
    EastRun := 192;
    NorthRun := 204;
    NorthEastRun := 216;
    NorthWestRun := 228;
    SouthRun := 240;
    SouthEastRun := 252;
    SouthWestRun := 264;
    WestRun := 276;
  end;

    if CharacterType = ctViking then
    begin
      AI := aiNone;
      ImageName := 'viking_battle.image';
      OffsetY := -95;
      OffsetX := -65;
      Health := 100;
      Speed := 0.5;
      Z := Z_Character;


      EastStand := 0;
      NorthStand := 13;
      NorthEastStand := 26;
      NorthWestStand := 39;
      SouthStand := 52;
      SouthEastStand := 65;
      SouthWestStand := 78;
      WestStand := 91;


      EastRun := 176;
      NorthRun := 184;
      NorthEastRun := 192;
      NorthWestRun := 200;
      SouthRun := 208;
      SouthEastRun := 216;
      SouthWestRun := 224;
      WestRun := 232;
    end;

  CurrentState := csStand;
  NextState := csNone;

  ActionRadius := 100;
  VictimIndex := -1;
  Accuracy := 75; // 75% accuracy
end;

procedure TCharacter.MoveTo(CX,CY: Single);
begin
  inherited;
  SetMoveDirection;
  //VictimIndex := -1;
end;

procedure TCharacter.MoveToTile(ix,iy: integer);
begin
  inherited;
  SetMoveDirection;
  //VictimIndex := -1;
end;

procedure TCharacter.AttackTile(ix,iy: integer);
var p: TPoint;
begin
  P := IsoToScreen(ix, iy);
  MoveTo(P.X, P.Y); // not optimized
  case Direction of
  0: P := IsoToScreen(ix,iy+1);
  1: P := IsoToScreen(ix+1,iy+1);
  2: P := IsoToScreen(ix+1,iy);
  3: P := IsoToScreen(ix+1,iy-1);
  4: P := IsoToScreen(ix,iy-1);
  5: P := IsoToScreen(ix-1,iy-1);
  6: P := IsoToScreen(ix-1,iy);
  7: P := IsoToScreen(ix-1,iy+1);
  end;
  MoveTo(P.X, P.Y);
  NextState := csAttack;
end;

procedure TCharacter.AttackCharacter(index: integer; CX, CY: integer);
var
  p2: TIsoPoint;
  p: TPoint;
begin
  VictimIndex := index;
  VictimX := CX;
  VictimY := CY;
  p2 := ScreenToIso(CX, CY);
  p := IsoToScreen(p2);

  MoveTo(P.X, P.Y); // not optimized
  case Direction of
  0: P := IsoToScreen(p2.I,p2.J+1);
  1: P := IsoToScreen(p2.I+1,p2.J+1);
  2: P := IsoToScreen(p2.I+1,p2.J);
  3: P := IsoToScreen(p2.I+1,p2.J-1);
  4: P := IsoToScreen(p2.I,p2.J-1);
  5: P := IsoToScreen(p2.I-1,p2.J-1);
  6: P := IsoToScreen(p2.I-1,p2.J);
  7: P := IsoToScreen(p2.I-1,p2.J+1);
  end;
  MoveTo(P.X, P.Y);
  NextState := csAttack;

end;

procedure TCharacter.SetStandDirection;
var d15: integer;
begin
  inherited;
  DeltaX := 0.0;
  DeltaY := 0.0;
  d15:=Round(NewAngle) div 15;
  case d15 of
    23,24,0: begin PatternIndex:=EastStand; MirrorX:=FALSE; end;
    5,6: begin PatternIndex:=NorthStand; MirrorX:=FALSE; end;
    11,12: begin PatternIndex:=WestStand; MirrorX:=FALSE; end;
    17,18: begin PatternIndex:=SouthStand; MirrorX:=FALSE; end;

    1,2,3,4: begin PatternIndex:=NorthEastStand; MirrorX:=FALSE; end;
    7,8,9,10: begin PatternIndex:=NorthWestStand; MirrorX:=FALSE; end;
    13,14,15,16: begin PatternIndex:=SouthWestStand; MirrorX:=FALSE; end;
    19,20,21,22: begin PatternIndex:=SouthEastStand; MirrorX:=FALSE; end;

  end;
  DoAnimate := FALSE;
end;

procedure TCharacter.SetMoveDirection;
var d15: integer;
begin
  d15:=Round(NewAngle) div 15;
  case d15 of
    23,24,0: begin AnimStart:=EastRun; MirrorX:=FALSE; Direction := 6 end;
    5,6: begin AnimStart:=NorthRun; MirrorX:=FALSE; Direction := 0 end;
    11,12: begin AnimStart:=WestRun; MirrorX:=FALSE; Direction := 2 end;
    17,18: begin AnimStart:=SouthRun; MirrorX:=FALSE; Direction := 4 end;

    1,2,3,4: begin AnimStart:=NorthEastRun; MirrorX:=FALSE; Direction := 7 end;
    7,8,9,10: begin AnimStart:=NorthWestRun; MirrorX:=FALSE; Direction := 1 end;
    13,14,15,16: begin AnimStart:=SouthWestRun; MirrorX:=FALSE; Direction := 3 end;
    19,20,21,22: begin AnimStart:=SouthEastRun; MirrorX:=FALSE; Direction := 5 end;

  end;
  if CharacterType = ctKnight then AnimCount := 12;
  if CharacterType = ctViking then AnimCount := 8;
  AnimSpeed := 0.1;
  AnimLooped := TRUE;
  DoAnimate := TRUE;
end;

procedure TCharacter.SetAttackDirection;
var d15: integer;
begin
  d15:=Round(NewAngle) div 15;
  case d15 of
    23,24,0: begin AnimStart:=EastStand; MirrorX:=FALSE; Direction := 6 end;
    5,6: begin AnimStart:=NorthStand; MirrorX:=FALSE; Direction := 0 end;
    11,12: begin AnimStart:=WestStand; MirrorX:=FALSE; Direction := 2 end;
    17,18: begin AnimStart:=SouthStand; MirrorX:=FALSE; Direction := 4 end;

    1,2,3,4: begin AnimStart:=NorthEastStand; MirrorX:=FALSE; Direction := 7 end;
    7,8,9,10: begin AnimStart:=NorthWestStand; MirrorX:=FALSE; Direction := 1 end;
    13,14,15,16: begin AnimStart:=SouthWestStand; MirrorX:=FALSE; Direction := 3 end;
    19,20,21,22: begin AnimStart:=SouthEastStand; MirrorX:=FALSE; Direction := 5 end;

  end;
  if CharacterType = ctKnight then AnimCount := 12;
  if CharacterType = ctViking then AnimCount := 8;
  AnimSpeed := 0.1;
  AnimLooped := TRUE;

  DoAnimate := TRUE;
end;

procedure TCharacter.ProcessStates;
begin
  if (CurrentState = csRunning) then
    if (NextState = csStand) then
      VictimIndex := -1;

  if (CurrentState = csAttack) then
    if
end;

procedure TCharacter.Move(const MoveCount: Single);
begin
  // просто инхеритед
  inherited;

end;



// -------------------------------------------------------------------------------

// ------------------------------- TMovingSprite -------------------------------

procedure TMovingSprite.MoveTo(CX,CY: Single);
var
  SinPhi, CosPhi: Extended;
begin
  if (CX < 0) or (CY < 0) then Exit; // Check range

  NewAngle:= GetAngle(X, Y, CX, CY);

  SinCos(DegToRad(NewAngle), SinPhi, CosPhi);
  DeltaX := CosPhi;
  DeltaY := -SinPhi;
  NX := CX;
  NY := CY;

  Angle := NewAngle;
  CurrentState := csRunning;
  NextState := csStand;
end;

procedure TMovingSprite.MoveToTile(ix, iy: integer);
var
  SinPhi, CosPhi: Extended;
  CX,CY: integer;
begin
  if (ix < 0) or (iy < 0) then Exit; // Check range

  CX := ix * TileWidth div 2 + MapDX;
  CY := iy * TileHeight div 2 + MapDY;
  NewAngle:= GetAngle(X, Y, CX, CY);


  SinCos(DegToRad(NewAngle), SinPhi, CosPhi);
  DeltaX := CosPhi;
  DeltaY := -SinPhi;
  NX := CX;
  NY := CY;

  Angle := NewAngle;
  CurrentState := csRunning;
  NextState := csStand;
end;

procedure TMovingSprite.AttackTile(ix,iy: integer);
begin
  CurrentState := csRunning;
  NextState := csAttack;
end;

procedure TMovingSprite.SetStandDirection;
begin
  CurrentState := csStand;
  NextState := csNone;
end;

procedure TMovingSprite.SetMoveDirection;
begin
  CurrentState := csRunning;
  NextState := csStand;
end;

procedure TMovingSprite.SetAttackDirection;
begin
  CurrentState := csRunning;
  NextState := csAttack;
end;

procedure TMovingSprite.Move(const MoveCount: Single);
begin
  inherited;
  if (Sqr(NX - X) + Sqr(NY - Y) < Sqr(Speed)) then
  begin
    DeltaX := 0.0;
    DeltaY := 0.0;
    if (CurrentState = csRunning) then
    begin
      if (NextState = csStand) then
      begin
        SetStandDirection;
        CurrentState := csStand;
        NextState := csNone;
      end;
      if (NextState = csAttack) then
      begin
        SetAttackDirection;
        CurrentState := csAttack;
        NextState := csStand;
      end;
    end;
    Exit;
  end;
  X := X + DeltaX * MoveCount * Speed;
  Y := Y + DeltaY * MoveCount * Speed;
  Z := Trunc(Y);

  PreviousTile := TempTile;
  CurrentTile := ScreenToIso(Trunc(X),Trunc(Y));
  TempTile := CurrentTile;
end;

procedure TMovingSprite.CheckState;
begin

end;

// --------------------------------------------------------------------


// --------------------------------- TMap ----------------------------

constructor TMap.Create(SizeX, SizeY: word);
var
  a,b: integer;
begin
  SizeX := SizeX;
  SizeY := SizeY;
  RenderTiles := False;

  SetLength(Characters, 0);
  SetLength(Selections, 0);
  SetLength(SelectedChars, 0);
  SetLength(Players, 1);
  SelectCandidat := -1;
  SSel := FALSE;
  Cursor := TGameCursor.Create(ctHand);
  Cursor.Z := Z_Cursor;


  // this sprite shows what cell is now under mouse cursor
  SelTile := TSprite.Create(SpriteEngine);
  SelTile.ImageName := 'cellsel.image';
  SelTile.Z := Z_SelTile;
  // this mask is using for converting screen coord. to Iso coord.
  TileMask := TSprite.Create(SpriteEngine);
  TileMask.ImageName := 'mask.image';
  TileMask.Visible := FALSE;

  if (RenderTiles) then
  begin

  SetLength(Tiles, SizeX, SizeY);

  for a := 0 to SizeX div 2 - 1 do
  begin
    for b := 0 to SizeY - 1 do
    begin
      Tiles[a, b] := TMapTile.Create;
      Tiles[a, b].ImageName := 'grass1.image';
      Tiles[a, b].X := TileWidth * a;
      Tiles[a, b].Y := TileHeight * b;

      If b > 0 then

      {Четные - вверх}
      If b mod 2 = 0 then
      begin
        Tiles[a, b].Y := TileHeight * b-TileHeight*(b div 2);
      end;

      {Нечетные - вверх и вбок}
      if b mod 2 = 1 then
      begin
        Tiles[a, b].X := TileWidth * a+(TileWidth div 2);
        Tiles[a, b].Y := TileHeight * b-(TileHeight div 2)-TileHeight*(b div 2);
      end;
    end;
  end;

  end;

  SetLength(Players, 2);
  Players[0] := TPlayer.Create(0);
  Players[1] := TPlayer.Create(1);

end;

destructor TMap.Destroy;
begin
  inherited Destroy;
end;

procedure TMap.ProcessCursor;
var
  P: TIsoPoint;
begin
  P := ScreenToIso(MX,MY);
  a := P.I;
  b := P.J;
  SelTile.X := a * TileWidth div 2;
  SelTile.Y := b * TileHeight div 2;
end;

procedure TMap.ProcessMovements;
var
  i,l,p: integer;
  b: boolean;
begin
  l := Length(Characters);
  p := l - 1;
  for i := 0 to p do
  begin
    //Tiles[Characters[i].CurrentTile.I,Characters[i].CurrentTile.J].Empty := False;
    //Tiles[Characters[i].PreviousTile.I,Characters[i].PreviousTile.J].Empty := True;
    Characters[i].ProcessStates;
    if Characters[i].VictimIndex <> -1 then
    //begin
    if (Characters[Characters[i].VictimIndex].X <> Characters[i].VictimX) or
       (Characters[Characters[i].VictimIndex].Y <> Characters[i].VictimY) then

       //showmessage(inttostr(Round(Characters[Characters[i].VictimIndex].X))+' '+ inttostr(Round(Characters[i].VictimX)));
          Characters[i].AttackCharacter(Characters[i].VictimIndex,
                                        Round(Characters[Characters[i].VictimIndex].X),
                                        Round(Characters[Characters[i].VictimIndex].Y));
                                        //else
                                          //showmessage('zomg');
    //if selectcandidat <> - 1 then
    //if Characters[Self.SelectCandidat].VictimIndex <> -1 then
    //begin
    //MainForm.AsphyreFonts[0].TextOut('VictimIndex.X: '+IntToStr(Round(Characters[Characters[Self.SelectCandidat].VictimIndex].X)), 50,300,$FFD67563);
    //MainForm.AsphyreFonts[0].TextOut('VictimX: '+IntToStr(Round(Characters[Self.SelectCandidat].VictimX)), 50,350,$FFD67563);
    //end;
    //end;
  end;
end;

procedure TMap.CreateCharacter(Player: TPlayer; CharType: TCharacterType; PosX, PosY: Single);
var
  L: LongInt;
  ll: integer;
begin
  L := Length(Characters);
  SetLength(Characters, L + 1);
  Characters[L] := TCharacter.Create(CharType);
  with Characters[L] do
  begin
    CharacterType := CharType;
    RIX := 58;
    RIY := 72;
    CollideRadius := 10;
    SetPos(PosX, PosY);
    Index := L;
    SetStandDirection;
  end;
  ll := Length(Player.Characters);
  SetLength(Player.Characters, ll + 1);
  Player.Characters[ll] := L;
end;

procedure TMap.CreateCharacterAt(Player: TPlayer; CharType: TCharacterType; Pos: TIsoPoint);
var
  L: LongInt;
  ll: integer;
  ScrPos: TPoint;
begin
  L := Length(Characters);
  SetLength(Characters, L + 1);
  Characters[L] := TCharacter.Create(CharType);
  with Characters[L] do
  begin
    CharacterType := CharType;
    RIX := 58;
    RIY := 72;
    CollideRadius := 10;
    ScrPos := IsoToScreen(Pos);
    SetPos(ScrPos.X, ScrPos.Y);
    Index := L;
    SetStandDirection;
  end;
  ll := Length(Player.Characters);
  SetLength(Player.Characters, ll + 1);
  Player.Characters[ll] := L;
end;

procedure TMap.CreateCharacterAt(Player: TPlayer; CharType: TCharacterType; A,B: Integer);
var
  L: LongInt;
  ll: integer;
  ScrPos: TPoint;
begin
  L := Length(Characters);
  SetLength(Characters, L + 1);
  Characters[L] := TCharacter.Create(CharType);
  with Characters[L] do
  begin
    CharacterType := CharType;
    RIX := 58;
    RIY := 72;
    CollideRadius := 10;
    ScrPos := IsoToScreen(A,B);
    SetPos(ScrPos.X, ScrPos.Y);
    Index := L;
    SetStandDirection;
  end;
  ll := Length(Player.Characters);
  SetLength(Player.Characters, ll + 1);
  Player.Characters[ll] := L;
end;

function TMap.SelectExists(Index: LongInt): boolean;
var
  i: LongInt;
  l: LongInt;
  pred: LongInt;
begin
  l := Length(SelectedChars);
  pred := l - 1;
  Result := FALSE;
  for i := 0 to pred do
  begin
    if SelectedChars[i] = Index then
    begin
      Result := TRUE;
      Exit;
    end;
  end;
end;

procedure TMap.ProcessSelections;
var
  i, l, p: integer;
begin
  SelStr := '';
  l := Length(SelectedChars);
  p := l - 1;
  if l > 0 then
  for i := 0 to p do
  begin
    SelStr := SelStr +IntToStr(SelectedChars[i]) + ' ';
    Selections[i].X := Characters[SelectedChars[i]].X;
    Selections[i].Y := Characters[SelectedChars[i]].Y;
  end;
end;

procedure TMap.ProcessSelectFrame;
var
  i,l,p, AreaX, AreaY,
  AreaWidth, AreaHeight: integer;
begin
  l := Length(Characters);
  p := l - 1;
  if (SSel) then
  begin
    SelFrmStr := '';
    if SSelX2 > SSelX1
    then
      AreaX := SSelX1
    else
      AreaX := SSelX2;

    if SSelY2 > SSelY1
    then
      AreaY := SSelY1
    else
      AreaY := SSelY2;

    AreaWidth := Abs(SSelX2 - SSelX1);
    AreaHeight := Abs(SSelY2 - SSelY1);
    if l > 0 then
      for i := 0 to p do
      begin
       if PointInArea(Characters[i].X, Characters[i].Y, AreaX, AreaWidth, AreaY, AreaHeight) then
        if PlayerByIndex(Self, Characters[i].Index) = HumanPlayer then
        begin
          if (not SelectExists(Characters[i].Index)) then
          begin
            SelFrmStr := SelFrmStr + IntToStr(Characters[i].Index) + ' ';
            CreateSelection(HumanPlayer, smAdd, stGreen, Characters[i].Index);
          end;
        end
          else
          begin

            if (SelectExists(Characters[i].Index)) then
            begin

              CreateSelection(HumanPlayer, smRemove, stGreen, Characters[i].Index);
            end;
          end;
      end;
  end;
end;

procedure TMap.CreateSelection(Player: integer; Method: TSelectionMethod; Tp: TSelectionType; Index: LongInt);
var
  sl: LongInt;
  slp: Integer;
  i: LongInt;
  deselected: LongInt;
  pred: LongInt;
begin

  if (Length(SelectedChars) = 0) and (Method = smSingle) then
  begin
    SetLength(Players[Player].SelectedChars, 1);
    Players[Player].SelectedChars[0] := Index;

    SetLength(Selections, 1);
    SetLength(SelectedChars, 1);
    SelectedChars[0] := Index;

    if Player = HumanPlayer
    then
      Selections[0] := TSelection.Create(stGreen, Characters[Index].Speed)
    else
      Selections[0] := TSelection.Create(stRed, Characters[Index].Speed);

    with Selections[0] do SetPos(Characters[Index].X, Characters[Index].Y, Z_Selection);

  end
  else
    begin
      case Method of
        smSingle:
          begin
            sl := Length(Selections);
            slp := sl - 1;
            for i := 0 to slp do
            begin
              Selections[i].Free;
              SelectedChars[i] := -1;
              //Players[Player].SelectedChars[i] := -1;
            end;
            SetLength(Selections, 1);
            SetLength(SelectedChars, 1);
            SetLength(Players[Player].SelectedChars, 1);

            if Player = HumanPlayer
            then
              Selections[0] := TSelection.Create(stGreen, Characters[Index].Speed)
            else
              Selections[0] := TSelection.Create(stRed, Characters[Index].Speed);

            SelectedChars[0] := Index;
            Players[Player].SelectedChars[0] := Index;

            with Selections[0] do SetPos(Characters[Index].X, Characters[Index].Y, Z_Selection);
          end;
        smAdd:
          begin
            sl := Length(Selections);
            SetLength(Selections, sl + 1);
            SetLength(SelectedChars, Length(SelectedChars) + 1);
            SetLength(Players[Player].SelectedChars, Length(Players[Player].SelectedChars)+1);
            Selections[sl] := TSelection.Create(stGreen, Characters[Index].Speed);
            SelectedChars[sl] := Index;
            with Selections[sl] do
            begin
              SetPos(Characters[Index].X, Characters[Index].Y, Z_Selection);
            end;
          end;
        smRemove:
          begin
            sl := Length(Selections);
            deselected := SelectionByIndex(Self, Index);
            Selections[deselected].Free;
            SelectedChars[deselected] := -1;
            pred := sl-2;
            for i := deselected to pred do
              begin
                Selections[i] := Selections[i+1];
                SelectedChars[i] := SelectedChars[i+1];
              end;
            SetLength(Selections, sl-1);
            SetLength(SelectedChars, sl-1);
            SetLength(Players[Player].SelectedChars, Length(Players[Player].SelectedChars)-1);
          end;
        smReset:
          begin
            sl := Length(Selections);
            if sl <> 0 then
            begin
              pred := sl-1;
              for i := 0 to pred do
              begin
                Selections[i].Free;
                SelectedChars[i] := -1;
              end;
              SetLength(Selections, 0);
              SetLength(SelectedChars, 0);
              SetLength(Players[Player].SelectedChars, 0);
            end;
          end;

      end;
    end;
end;

procedure TMap.ProcessAI;
var
  c: integer;
  lc: integer;
  lcp: integer;
  i: integer;
  t: TIsoPoint;
begin
  lc := Length(Characters);
  lcp := lc - 1;
  for i := 0 to lcp do
  begin
    case Characters[i].AI of
      aiNone, aiHumanControl: exit;
      aiDefense:
        begin
          c := FirstOtherPlayerIndexByRadius(Self, PlayerByIndex(Self, i), Characters[i].X, Characters[i].Y, Characters[i].ActionRadius);
          if (c <> -1) then
          begin
            if (Characters[i].NextState <> csAttack) then
            begin
              //t := ScreenToIso(Round(Characters[c].X), Round(Characters[c].Y));
              //Characters[i].AttackTile(t.I, t.J);
              if (Characters[i].VictimIndex = -1) then
                Characters[i].AttackCharacter(c, Round(Characters[c].X), Round(Characters[c].Y));
            end;
          end;

        end;
    end;


  end;
end;

// ---------------------------------------------------------------------------

// ------------------------------- TSelection ---------------------------------

constructor TSelection.Create(T: TSelectionType; Spd: Single);
begin
  inherited Create(SpriteEngine);
  Speed := Spd;
  if T = stGreen
  then
    ImageName := 'selected.image'
  else
    ImageName := 'selected_enemy.image';
  OffsetX := -PatternWidth div 2;
  OffsetY := -PatternHeight;
  Z := Z_Selection;
end;

// ----------------------------------------------------------------------------


// ------------------------------- _TCursor -----------------------------------

constructor TGameCursor.Create(CursorType: TCursorType);
begin
  inherited Create(SpriteEngine);
  CursorType := CursorType;
  case CursorType of
    ctHand: ImageName := 'cursor_hand';
  end;
  Z := Z_Cursor;
  DoAnimate := False;

end;

// ----------------------------------------------------------------------------

// ------------------------------- TGuiPanel ----------------------------------

constructor TGuiPanel.Create;
begin
  inherited Create(SpriteEngine);
  X := 0;
  Y := 320;
  Self.ImageName := 'panel.image';
  Visible := True;
  Self.Portrait := TSprite.Create(SpriteEngine);
  Self.HealthBar := TSprite.Create(SpriteEngine);
  Self.ArmorPic := TSprite.Create(SpriteEngine);
  Self.WeaponPic := TSprite.Create(SpriteEngine);
  Self.Portrait.ImageName := 'viking_face.image';
  Self.HealthBar.ImageName := 'healthbar.image';
  Self.ArmorPic.ImageName := 'shield.image';
  Self.WeaponPic.ImageName := 'weapon.image';
  Self.Portrait.Visible := True;
  Self.HealthBar.Visible := True;
  Self.ArmorPic.Visible := True;
  Self.WeaponPic.Visible := True;

  Self.Portrait.X := 40 + Self.X;
  Self.Portrait.Y := 35 + Self.Y;
  Self.Portrait.Z := Z_Panel_Comp;

  Self.HealthBar.X := 20 + Self.X;
  Self.HealthBar.Y := 20 + Self.Y;
  Self.HealthBar.Z := Z_Panel_Comp;

  Self.ArmorPic.X := 150 + Self.X;
  Self.ArmorPic.Y := 25 + Self.Y;
  Self.ArmorPic.Z := Z_Panel_Comp;

  Self.WeaponPic.X := 150 + Self.X;
  Self.WeaponPic.Y := 85 + Self.Y;
  Self.WeaponPic.Z := Z_Panel_Comp;






end;

end.
