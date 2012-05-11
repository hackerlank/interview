unit main_editor;

{$R *.dfm}

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, AsphyreTimers, AsphyreSubsc, AsphyreDevices, Menus, ExtCtrls,
  AsphyreDb, AsphyreFonts, AsphyreImages, Asphyre2D, AsphyreSprite, AsphyreCanvas,
  StdCtrls, ComCtrls;

type
  TMainForm = class(TForm)
    MainMenu1: TMainMenu;
    File1: TMenuItem;
    AsphyreDevice: TAsphyreDevice;
    AsphyreTimer: TAsphyreTimer;
    AsphyreCanvas: TAsphyreCanvas;
    AsphyreImages: TAsphyreImages;
    AsphyreFonts: TAsphyreFonts;
    ASDb: TASDb;
    Panel2: TPanel;
    Button1: TButton;
    procedure AsphyreDeviceRender(Sender: TObject);
    procedure AsphyreTimerTimer(Sender: TObject);
    procedure AsphyreDeviceInitialize(Sender: TObject;
      var Success: Boolean);
    procedure FormDestroy(Sender: TObject);
    procedure Button3Click(Sender: TObject);
    procedure Panel2MouseMove(Sender: TObject; Shift: TShiftState; X,
      Y: Integer);
    procedure Panel2MouseDown(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure FormCreate(Sender: TObject);
    procedure Panel2MouseUp(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure AsphyreTimerProcess(Sender: TObject);
    procedure Button1Click(Sender: TObject);
    procedure Button2Click(Sender: TObject);
  private
    { Private declarations }
    function InitAsphyre: boolean;
  public
    { Public declarations }
    procedure RenderSelectFrame;
  end;

var
  MainForm: TMainForm;


implementation

uses header, misc, asphyredef, math, config;

var
  Map: TMap;
  Panel: TGuiPanel;
  Config: TConfig;

  MouseX, MouseY: Integer;
  MoveCount: single;
  SelStr: string = '';
  SelFrmStr: string = '';

function TMainForm.InitAsphyre: boolean;
begin
    AsphyreDevice.WindowHandle:=Panel2.Handle;
    AsphyreDevice.Height:=480;
    AsphyreDevice.Width:=640;
    AsphyreDevice.BitDepth:=bdLow;
    AsphyreDevice.Windowed:=TRUE;
    AsphyreDevice.VSync:=FALSE;
    ASDb.FileName:='data.asdb';
    if (not AsphyreDevice.Initialize()) then begin
      Result:=FALSE;
      AsphyreTimer.Enabled := FALSE;
      showmessage('Cannot init Asphyre!');
    end
    else
      begin
        AsphyreDevice.Clear(0);
        Result:=TRUE;
        AsphyreTimer.Enabled := TRUE;
      end;
end;


procedure TMainForm.RenderSelectFrame;
begin
  AsphyreCanvas.FrameRect(Map.SSelX1,Map.SSelY1,Map.SSelX2-Map.SSelX1,Map.SSelY2-Map.SSelY1,clLime,$FF000001);
end;




procedure TMainForm.AsphyreDeviceInitialize(Sender: TObject;
  var Success: Boolean);
begin
  SpriteEngine := TSpriteEngine.Create;
  SpriteEngine.Canvas := AsphyreCanvas;
  SpriteEngine.Image := AsphyreImages;

  Success := AsphyreFonts.LoadFromASDb(ASDb);
  if Success then
  Success := AsphyreImages.LoadFromASDb(ASDb);
  if not (Success) then Exit;
  AsphyreTimer.Enabled := FALSE;
end;

procedure TMainForm.FormDestroy(Sender: TObject);
begin
  AsphyreTimer.Enabled := FALSE;
  if Map <> nil then Map.Destroy;
  AsphyreDevice.Finalize;
  if SpriteEngine <> nil then SpriteEngine.Destroy;
end;

procedure TMainForm.Button3Click(Sender: TObject);
begin
  Map.Destroy;
  SpriteEngine.Destroy;
end;

procedure TMainForm.Panel2MouseMove(Sender: TObject; Shift: TShiftState; X,
  Y: Integer);
begin
  MouseX := X;
  MouseY := Y;
  if Map <> nil then
  begin
    Map.MX := MouseX;
    Map.MY := MouseY;
    Map.Cursor.X := SpriteEngine.WorldX + X;
    Map.Cursor.Y := SpriteEngine.WorldY + Y;
    //Map.Cell.X := Map.Cursor.X;
    //Map.Cell.Y := Map.Cursor.Y;
    if Map.SSel = FALSE
    then
     begin
      Map.SelectCandidat := UnderCursor(Map,Map.Cursor);
     end
    else
      begin
        Map.SSelX2 := X;
        Map.SSelY2 := Y;

      end;
  end;
end;

procedure TMainForm.Panel2MouseDown(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
var
  l,lp: integer;
  i: integer;
begin
  if Map.SelectCandidat <> -1 then
  begin
   if (not Map.SelectExists(Map.SelectCandidat)) then
   begin
     if PlayerByIndex(Map, Map.SelectCandidat) = HumanPlayer then
     begin
       if Shift = [ssLeft] then Map.CreateSelection(HumanPlayer, smSingle, stGreen, Map.SelectCandidat);
       if Shift = [ssShift, ssLeft] then Map.CreateSelection(HumanPlayer, smAdd, stGreen, Map.SelectCandidat);
     end;
     if PlayerByIndex(Map, Map.SelectCandidat) <> HumanPlayer then
     begin
       if Shift = [ssRight] then
       begin
         if Length(Map.Players[HumanPlayer].SelectedChars) > 0 then
           if PlayerByIndex(Map,Map.SelectCandidat) <> HumanPlayer then
           begin
             // attacking
             l := Length(Map.SelectedChars);
             lp := l - 1;
             for i := 0 to lp do Map.Characters[Map.SelectedChars[i]].AttackTile(Map.a, Map.b);
           end
           else
            begin
              // just moving there
            end;
       end;
       if Shift = [ssLeft] then
       begin
         Map.CreateSelection(PlayerByIndex(Map, Map.SelectCandidat), smSingle, stRed, Map.SelectCandidat);
       end;
     end;

   end
   else
     begin
       if Shift = [ssLeft] then Map.CreateSelection(HumanPlayer, smSingle, stGreen, Map.SelectCandidat);
       if Shift = [ssShift, ssLeft] then Map.CreateSelection(HumanPlayer, smRemove, stGreen, Map.SelectCandidat);
     end;
  end
  else
     if Shift = [ssLeft] then Map.CreateSelection(HumanPlayer, smReset, stGreen, Map.SelectCandidat);

  if (Map.SelectCandidat = -1) then
  begin
    if (Length(Map.Selections) = 1) then
      if Shift = [ssRight] then
      begin
        Map.Characters[Map.SelectedChars[0]].MoveToTile(Map.a, Map.b);
      end;
    if (Length(Map.Selections) = 0) then
      if Shift = [ssLeft] then
      begin
        Map.SSel := TRUE;
        Map.SSelX1 := X;
        Map.SSelY1 := Y;
        Map.SSelX2 := X;
        Map.SSelY2 := Y;
      end;
  end;
end;

procedure TMainForm.FormCreate(Sender: TObject);
begin
  InitAsphyre;
  Map := TMap.Create(20,20);
  //Panel := TGuiPanel.Create;
  Config.DEBUG := true;
  Config.ShowFps := true;
  //Config.

end;

procedure TMainForm.Panel2MouseUp(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
begin
  if (Map.SSel) then Map.SSel := FALSE;
end;

procedure TMainForm.AsphyreTimerTimer(Sender: TObject);
begin
  AsphyreDevice.Render($FF000000,TRUE);
  AsphyreTimer.Process;
  AsphyreDevice.Flip;
end;

procedure TMainForm.AsphyreDeviceRender(Sender: TObject);
begin
  SpriteEngine.Dead;
  MoveCount := 1 / (AsphyreTimer.FrameRate / 85);
  SpriteEngine.Move(MoveCount);
  SpriteEngine.Draw;

  if (Map.SSel) then RenderSelectFrame;
  if (Config.ShowFps) then
    AsphyreFonts[0].TextOut('FPS: ' + IntToStr(AsphyreTimer.FrameRate),5,5,$FFD67563);
  if (Config.DEBUG) then
  begin
    AsphyreFonts[0].TextOut(Format('Map[%d,%d]',[Map.a,Map.b]),5,20,$FFD67563);
    AsphyreFonts[0].TextOut(Format('Cursor[%d,%d]',[MouseX,MouseY]),5,35,$FFD67563);
    AsphyreFonts[0].TextOut('SelectCandidat: ' + IntToStr(Map.SelectCandidat),5,50,$FFD67563);
    AsphyreFonts[0].TextOut('Selected index: ' + Map.SelStr,5,65,$FFD67563);
    AsphyreFonts[0].TextOut('Cursor_Z: ' + Inttostr(Map.Cursor.Z),5,100, $FFD67563);
  end;

end;

procedure TMainForm.AsphyreTimerProcess(Sender: TObject);
begin
  Map.ProcessSelections;
  Map.ProcessSelectFrame;
  Map.ProcessCursor;
  Map.ProcessMovements;
  Map.ProcessAI;
end;

procedure TMainForm.Button1Click(Sender: TObject);
begin
  Map.CreateCharacterAt(Map.Players[0],ctKnight,15,3);
  Map.CreateCharacterAt(Map.Players[0],ctKnight,13,3);
  Map.CreateCharacterAt(Map.Players[0],ctKnight,11,3);
  Map.CreateCharacterAt(Map.Players[0],ctKnight,9,3);
  Map.CreateCharacterAt(Map.Players[0],ctKnight,7,3);
  Map.CreateCharacterAt(Map.Players[0],ctKnight,5,3);

  Map.CreateCharacterAt(Map.Players[1],ctViking,2,7);
  Map.CreateCharacterAt(Map.Players[1],ctViking,4,5);
  Map.CreateCharacterAt(Map.Players[1],ctViking,4,7);
  Map.CreateCharacterAt(Map.Players[1],ctViking,3,1);
  Map.CreateCharacterAt(Map.Players[1],ctViking,9,7);
  Map.CreateCharacterAt(Map.Players[1],ctViking,8,4);

end;

procedure TMainForm.Button2Click(Sender: TObject);
begin
  Map.ProcessAI;
end;

end.
