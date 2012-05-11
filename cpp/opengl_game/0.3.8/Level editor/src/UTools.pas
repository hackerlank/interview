unit UTools;

interface

uses
  Windows, Math, Controls, Classes, Graphics, ClipBrd, UObjectFile, ULevelFile,
    types;

const
  TOOLS_NUMBER  = 3;

  STATE_IDLE    = 0;
  STATE_MOVING  = 1;
  STATE_PASTED  = 2;
  STATE_DRAWING = 4;


type
  TSelectionProps = record
    R: TRect;
    procedure Clear();
    function Width(): Integer;
    function Height(): Integer;
    procedure SetRect(x0, y0, x1, y1: Integer);
  end;

  TTool = class
  private
    x1, x2   : Integer;
    y1, y2   : Integer;
    State    : Integer;
    isDrawing: Boolean;
    Objects  : TObjectFilePropsArr;
    Level    : TLevelFile;
    xpos     : PInteger;
    ypos     : PInteger;
  public

    constructor Create(vObjects: TObjectFilePropsArr;
       vLevel: TLevelFile); virtual;
    procedure MouseDown(Button: TMouseButton; Shift: TShiftState;
       X, Y: Integer); virtual;
    procedure MouseUp(Button: TMouseButton; Shift: TShiftState;
       X, Y: Integer); virtual;
    procedure MouseMove(Shift: TShiftState; X, Y: Integer); virtual;
    procedure AssignCoords(x, y: PInteger); virtual;
    procedure Draw(x, y: Integer); virtual;
    procedure Paint(canvas: TCanvas); virtual;
    procedure SetState(NewState: Integer);
    function GetCursor():TCursor;  virtual;
    procedure Clear(); virtual; abstract;
  end;

  TBasicSelectionTool = class(TTool)
  public
    procedure Paste; virtual; abstract;
    procedure Cut; virtual; abstract;
    procedure Copy; virtual; abstract;
  end;
TToolArr = array of TTool;

procedure CreateTools(var dest: TToolArr; vObjects: TObjectFilePropsArr;
   vLevel: TLevelFile);
   
implementation

type

TPenTool = class(TTool)
private
  SelectedObj: PObjectPropsNode;
  SelRect: TRect;
  dx, dy: Integer;
public
  constructor Create(vObjects: TObjectFilePropsArr;
     vLevel: TLevelFile); override;
  procedure MouseDown(Button: TMouseButton; Shift: TShiftState;
     X, Y: Integer); override;
  procedure Draw(x, y: Integer); override;
  procedure Paint(canvas: TCanvas); override;
  function GetCursor():TCursor;  override;
  procedure Clear(); override;
end;

THandTool = class(TTool)
public
  constructor Create(vObjects: TObjectFilePropsArr;
     vLevel: TLevelFile); override;
  procedure Draw(x, y: Integer); override;
end;

TSelectionTool = class(TBasicSelectionTool)
public
  dx, dy: Integer;
  constructor Create(vObjects: TObjectFilePropsArr;
     vLevel: TLevelFile); override;
  procedure Paint(canvas: TCanvas); override;
  procedure Draw(x, y: Integer); override;
  procedure MouseMove(Shift: TShiftState; X, Y: Integer); override;
  procedure MouseDown(Button: TMouseButton; Shift: TShiftState;
     X, Y: Integer); override;
  procedure MouseUp(Button: TMouseButton; Shift: TShiftState;
     X, Y: Integer); override;
  function GetCursor():TCursor; override;
  procedure Copy(); override;
  procedure Paste(); override;
  procedure Cut(); override;
end;

type
  TToolClassIndex = (ttPen, ttHand, ttSelection);
  TToolClass = class of TTool;

const
  aTools: array [TToolClassIndex] of TToolClass  = (TPenTool, THandTool,
    TSelectionTool);

  { TTool }

procedure TTool.AssignCoords(x, y: PInteger);
begin
  xpos := x;
  ypos := y;
end;

constructor TTool.Create(vObjects: TObjectFilePropsArr; vLevel: TLevelFile);
begin
  Objects := vObjects;
  Level := vLevel;
  x1 := 0;
  x2 := 0;
  y1 := 0;
  y2 := 0;
  isDrawing := false;
end;

procedure TTool.Paint(canvas: TCanvas);
begin
//..
end;

procedure TTool.SetState(NewState: Integer);
begin
  State := NewState;
end;

procedure TTool.Draw;
begin
end;

function TTool.GetCursor: TCursor;
begin
  result := crArrow;
end;

procedure TTool.MouseDown(Button: TMouseButton; Shift: TShiftState; X, Y: Integer);
begin
  if Button = mbLeft then
    begin
      isDrawing := true;
      x1 := x;
      y1 := y;
      x2 := x;
      y2 := y;
    end;
end;

procedure TTool.MouseUp(Button: TMouseButton; Shift: TShiftState; X, Y: Integer);
begin
  if Button = mbLeft then
    begin
      isDrawing := false;
      x2 := x;
      y2 := y;
    end;
end;

procedure TTool.MouseMove(Shift: TShiftState; X, Y: Integer);
begin
  if not isDrawing then
    Exit;
  x2 := x;
  y2 := y;
  Draw(x,y);
end;

{ TPenTool }

procedure TPenTool.Clear;
begin
  SelectedObj := nil;
  with SelRect do
    begin
      left := 0;
      Right := 0;
      Top := 0;
      Bottom := 0;
    end;
end;

constructor TPenTool.Create(vObjects: TObjectFilePropsArr; vLevel: TLevelFile);
begin
  inherited;
  SetState(STATE_IDLE);
  dx := 0;
  dy := 0;
end;

procedure TPenTool.Draw(x, y: Integer);
begin
  if state <> STATE_MOVING then
    Exit;
  dx :=  x - dx;
  dy :=  y - dy;
  SelectedObj.SetX(SelectedObj.x + dx);
  SelectedObj.SetY(SelectedObj.y + dy);
  SelRect := Level.GetObjectRect(SelectedObj^);
  dx := x;
  dy := y;
end;

function TPenTool.GetCursor: TCursor;
begin
  case state of
    STATE_IDLE:
      Result := crCross;
    STATE_MOVING:
      Result := crSizeAll;
  end;
end;

procedure TPenTool.MouseDown(Button: TMouseButton; Shift: TShiftState; X,
  Y: Integer);
begin
  inherited;
  dx := x1;
  dy := y1;
  case state of
    STATE_IDLE:
      begin
        Level.ClearSelection;
        SelectedObj := Level.AddObject(x, y);
        SelRect := Level.GetObjectRect(SelectedObj^);
        SelectedObj.selected := true;
        state := STATE_MOVING;
      end;
    STATE_MOVING:
      begin
        if not ptinrect(SelRect, Point(x, y)) then
          begin
            state := STATE_IDLE;
            MouseDown(Button, Shift, x, y);
            Exit;
          end;
      end;
  end;
end;

procedure TPenTool.Paint(canvas: TCanvas);
begin
  canvas.Pen.width := 1;
  canvas.Pen.Style := psDot;
  canvas.Brush.Style := bsClear;
  canvas.Pen.Color := RGB(180, 0, 30);
  OffsetRect(SelRect, xpos^, ypos^);
  canvas.Rectangle(SelRect);
  OffsetRect(SelRect, -xpos^, -ypos^);
end;

{ THandTool }

constructor THandTool.Create(vObjects: TObjectFilePropsArr; vLevel: TLevelFile);
begin
  inherited;
end;

procedure THandTool.Draw(x, y: Integer);
var
  oxpos, oypos: Integer;
begin
  oxpos := xpos^;
  oypos := ypos^;
  xpos^ := xpos^ + x2 - x1;
  ypos^ := ypos^ + y2 - y1;
end;

{ TSelectionTool }

procedure TSelectionTool.Copy;
begin
end;

constructor TSelectionTool.Create(vObjects: TObjectFilePropsArr; vLevel: TLevelFile);
begin
  inherited ;
  State := STATE_IDLE;
end;

procedure TSelectionTool.Cut;
begin
end;

procedure TSelectionTool.Draw(x, y: Integer);
begin
  Inherited;
end;

function TSelectionTool.GetCursor: TCursor;
begin
  case state of
    STATE_IDLE:
      Result := crCross;
    STATE_MOVING:
      Result := crSizeAll;
  end;
end;

procedure TSelectionTool.MouseDown(Button: TMouseButton; Shift: TShiftState; X, Y: Integer);
var
  rct: TRect;
begin
  Inherited;
  if mbRight = Button then
    Level.ClearSelection;
  dx := x;
  dy := y;
  if Level.PtInSelection(x, y) then
    state := STATE_MOVING
  else
    begin
      state := STATE_IDLE;
      with rct do
        begin
          Left   := x1;
          Right  := x1+1;
          Top    := y1;
          Bottom := y1+1;
          Level.ClearSelection;
          Level.SelectObjects(left, top, right, bottom);
        end;
    end;
end;

procedure TSelectionTool.MouseMove(Shift: TShiftState; X, Y: Integer);
var
  rct: TRect;
begin
  inherited;
  if not isDrawing then
    Exit;
  case state of
    STATE_IDLE:
      begin
        with rct do
          begin
            Left   := Min(x1, x2);
            Right  := Max(x1, x2);
            Top    := Min(y1, y2);
            Bottom := Max(y1, y2);


            begin
              if ssCtrl in Shift then
                Level.AddToSelection(left, top, right, bottom)
              else
                begin
                  if not (ssShift in Shift) then
                    Level.ClearSelection;
                  Level.SelectObjects(left, top, right, bottom);
                end;
            end;
        end;
      end;
    STATE_MOVING:
      begin
        if not isDrawing then
          Exit;
        dx := x - dx;
        dy := y - dy;

        // snapping будет приблизительно в этом месте, и еще там ^

        Level.OffsetSelected(dx, dy);

        Level.TrySnap;          

        dx := x;
        dy := y;
      end;
    end;
end;

procedure TSelectionTool.MouseUp(Button: TMouseButton; Shift: TShiftState; X,
  Y: Integer);
begin
  inherited;
//  Level.ClearSelection;
//  Level.SelectObjects(x1, y1, x2, y2);
//  state := STATE_IDLE;
  x1 := -1;
  y1 := -1;
  x2 := -1;
  y1 := -1;
end;

procedure TSelectionTool.Paint(canvas: TCanvas);
begin
  if state <> STATE_IDLE then
    Exit;
  canvas.Pen.width := 1;
  canvas.Pen.Style := psDash;
  canvas.Brush.Style := bsClear;
  canvas.Pen.Color := RGB(125, 0, 90);
  canvas.Rectangle(x1 + xpos^ , y1 + ypos^, x2 + xpos^ , y2 + ypos^);
end;

procedure TSelectionTool.Paste;
begin
end;

{ TSelectionProps }

function TSelectionProps.Width: Integer;
begin
  Result := abs(R.Right - R.Left);
end;

function TSelectionProps.Height: Integer;
begin
  Result := abs(R.Bottom - R.Top);
end;

procedure TSelectionProps.Clear;
begin
  R := Rect(0, 0, 0, 0);
end;

procedure TSelectionProps.SetRect(x0, y0, x1, y1: Integer);
begin
  R.Left := Min(x0, x1);
  R.Right := Max(x0, x1);
  R.Top := Min(y0, y1);
  R.Bottom := Max(y0, y1);
end;

procedure CreateTools(var dest: TToolArr; vObjects: TObjectFilePropsArr;
   vLevel: TLevelFile);
var
 i: Integer;
begin
  SetLength(dest, TOOLS_NUMBER);
  for i := 0 to TOOLS_NUMBER-1 do
    dest[i] :=  aTools[TToolClassIndex(i)].Create(vObjects, vLevel);
end;

end.

