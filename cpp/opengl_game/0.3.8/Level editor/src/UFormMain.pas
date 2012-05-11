unit UFormMain;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, Menus, StdCtrls, ExtCtrls, UObjectFile, UFormAbout, XPMan, Grids,
  ULevelFile, Utils, UTools, Buttons, ComCtrls, math,
  IniFiles, UFormObjectProps, UMetaData;

const
  INI_FILE_PATH = 'Config\Level editor.ini';
type
  TMainForm = class(TForm)
    MainMenu: TMainMenu;
    miFile: TMenuItem;
    mifOpen: TMenuItem;
    mifSave: TMenuItem;
    mifSaveas: TMenuItem;
    mifExit: TMenuItem;
    miAbout: TMenuItem;
    OpenDialog: TOpenDialog;
    SaveDialog: TSaveDialog;
    pnlMainBase: TPanel;
    pnlLeftBase: TPanel;
    pbPaintBox: TPaintBox;
    XPManifest: TXPManifest;
    dgObjects: TDrawGrid;
    miView: TMenuItem;
    mivObjects: TMenuItem;
    mifNew: TMenuItem;
    mifDiv1: TMenuItem;
    pnlDrawGrid: TPanel;
    pnlTools: TPanel;
    pnlFirstParam: TPanel;
    pnlSecondParam: TPanel;
    btnBrush: TSpeedButton;
    btnHand: TSpeedButton;
    btnSelect: TSpeedButton;
    miEdit: TMenuItem;
    StatusBar: TStatusBar;
    mieDelete: TMenuItem;
    pnlParamBase: TPanel;
    mivScripts: TMenuItem;
    pnlKeybrdInput: TPanel;
    btnSetCords: TButton;
    mivGrid: TMenuItem;
    miTools: TMenuItem;
    mitBrush: TMenuItem;
    mitGrip: TMenuItem;
    mitSelect: TMenuItem;
    pnlFill: TPanel;
    btnFill: TButton;
    gbKeybInput: TGroupBox;
    pnlInputX: TPanel;
    pnlInputY: TPanel;
    pnlInputBtns: TPanel;
    procedure pbPaintBoxDblClick(Sender: TObject);
    procedure dgObjectsDblClick(Sender: TObject);
    procedure mifSaveClick(Sender: TObject);
    procedure mifCloseClick(Sender: TObject);
    procedure btnFillClick(Sender: TObject);
    procedure mitGripClick(Sender: TObject);
    procedure mitSelectClick(Sender: TObject);
    procedure mitBrushClick(Sender: TObject);
    procedure mivGridClick(Sender: TObject);
    procedure btnSetCordsClick(Sender: TObject);
    procedure mivScriptsClick(Sender: TObject);
    procedure mieDeleteClick(Sender: TObject);
    procedure dgObjectsSelectCell(Sender: TObject; ACol, ARow: Integer;
      var CanSelect: Boolean);
    procedure sbHorizontalScroll(Sender: TObject; ScrollCode: TScrollCode;
      var ScrollPos: Integer);
    procedure sbVerticalScroll(Sender: TObject; ScrollCode: TScrollCode;
      var ScrollPos: Integer);
    procedure btnClick(Sender: TObject);
    procedure pbPaintBoxMouseDown(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure pbPaintBoxMouseUp(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure pbPaintBoxMouseMove(Sender: TObject; Shift: TShiftState; X,
      Y: Integer);
    procedure FormDestroy(Sender: TObject);
    procedure mifNewClick(Sender: TObject);
    procedure pbPaintBoxPaint(Sender: TObject);
    procedure mivObjectsClick(Sender: TObject);
    procedure dgObjectsDrawCell(Sender: TObject; ACol, ARow: Integer;
      Rect: TRect; State: TGridDrawState);
    procedure miAboutClick(Sender: TObject);
    procedure mifSaveasClick(Sender: TObject);
    procedure mifOpenClick(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure mifExitClick(Sender: TObject);
  private
    procedure CenterControl(Control : TControl);

  public
    fIni: TINIfile;
    ModulePath: string;
    ObjectsPath: string;
    TexturesPath: string;
    Saved: Boolean;
    FileName: string;
    numObjects: Integer;
    Level: TLevelFile;
    xpos, ypos: Integer;
    Tools: TToolArr;
    Tool: TTool;
    cID: Integer;
    ErrFlag: boolean;
    function GetObjectFromGrid(ARow, ACol : Integer): PObjectFileProps;
  end;

var
  MainForm : TMainForm;

implementation

uses UFormScript;

{$R *.dfm}

procedure TMainForm.dgObjectsDblClick(Sender: TObject);
begin
  TFormObjectProps.Create(self, ProtoSelected^);
end;

procedure TMainForm.dgObjectsDrawCell(Sender: TObject; ACol, ARow: Integer;
  Rect: TRect; State: TGridDrawState);
var
  srcRect : TRect;
  obj : PObjectFileProps;
begin
  obj := GetObjectFromGrid(ARow, ACol);
  if (obj = nil) or (obj.GetPreview() = nil) then
    begin
      dgObjects.Canvas.Brush.Color := clBtnFace;
      dgObjects.Canvas.FillRect(Rect);
      Exit;
    end;
  with srcRect do
    begin
      left := 0;
      top := 0;
      right := obj.GetPreview().Width;
      bottom := obj.GetPreview().height;
    end;
  dgObjects.Canvas.Brush.Color := clBtnFace;
  dgObjects.Canvas.BrushCopy(Rect, obj.GetPreview(), srcRect, MAGIC_COLOR);
end;

procedure TMainForm.dgObjectsSelectCell(Sender: TObject; ACol, ARow: Integer;
  var CanSelect: Boolean);
var
  numCols : Integer;
begin
  if numObjects <= 0 then
    Exit;
  numCols := dgObjects.ColCount;
  if (ARow*numCols + ACol > numObjects-1) then
    ProtoSelected := nil
  else
    ProtoSelected := @Prototypes[ARow*numCols + ACol];
end;

procedure TMainForm.FormCreate(Sender: TObject);
var
  hfile : THandle;
  filename, FunctionName : string;
  fdata : _WIN32_FIND_DATAA;
  i, j, ParamCount : Integer;
  tmp : TObjectFileProps;
begin
  ErrFlag := false;
  cID := 0;
  tmp := TObjectFileProps.Create;
  pbPaintBox.ControlStyle := pbPaintBox.ControlStyle + [csOpaque];
  SetLength(Prototypes, MAX_OBJECTS);

  SaveDialog.Filter := OBJECT_FILE_FILTER;
  OpenDialog.Filter := OBJECT_FILE_FILTER;
  SaveDialog.DefaultExt := DEF_EXT;

  SetLength(ModulePath, MAX_PATH);
  SetLength(ObjectsPath, MAX_PATH);
  SetLength(TexturesPath, MAX_PATH);
  SetLength(FileName, MAX_PATH);

  if GetModuleFileName(0, PAnsiChar(@ModulePath[1]), 1024) = 0 then
    MsgLastErr();
  myExtractFilePath(ModulePath, ModulePath);
  fIni := TIniFile.Create(ModulePath + INI_FILE_PATH);
  myAppend(ObjectsPath, ModulePath, fIni.ReadString('General', 'ProtoPath', ''));
  myAppend(TexturesPath, ModulePath, fIni.ReadString('General', 'TexturesPath', ''));

  SetCurrentDir(ModulePath);

  FuncCount := fIni.ReadInteger('Functions', 'FunctionsCount', 0);
  SetLength(Functions, FuncCount);
  for i := 0 to FuncCount-1 do
    begin
      Functions[i] := TObjectFileProps.Create;
      FunctionName := fIni.ReadString('Functions', 'Function' + IntToStr(i+1), '');
      Functions[i].AddLexeme(ptString, 'name', FunctionName);
      ParamCount := fIni.ReadInteger(FunctionName, 'ParamsCount', 0);
      for j := 0 to ParamCount-1 do
        Functions[i].AddLexeme(ptString, fIni.ReadString(FunctionName,
          'Param'+IntToStr(j+1), ''), '');
      Functions[i].AddLexeme(ptLast, '', '');
    end;

  SetCurrentDir(ObjectsPath);
  hfile := FindFirstFile('*.*', fdata);
  FindNextFile(hfile, fdata);
  numObjects := 0;
  while (FindNextFile(hfile, fdata)) and (numObjects <= MAX_OBJECTS) do
    begin
      if hfile = INVALID_HANDLE_VALUE then
        MsgLastErr();
      myAppend(Filename, ObjectsPath, fdata.cFileName);
      if fdata.cFileName[0] in ['.', '_', '!'] then
        Continue;
      try
        if not tmp.LoadFromFile(Filename) then
          begin
            //ShowMessage(tmp.GetError);
            Log('Error', 'Unable to parse %s: %s', [Filename, tmp.GetError()]);
            ErrFlag := true;
            Continue;
          end;
        if not tmp.LoadImage() then
          begin
            //ShowMessage(tmp.GetError);
            ErrFlag := true;
            Continue;
          end;
      Inc(numObjects);
      Prototypes[numObjects-1] := TObjectFileProps.Create;
      Prototypes[numObjects-1].Assign(tmp);
      tmp.FakeList;
      Inc(ProtoCount);
      except
        ErrFlag := true;
        Log('AHTUNG', 'Exception while trying to load %s', [fdata.cFileName]);
        Prototypes[numObjects-1].Free;
        Dec(numObjects);
      end;
    end;

  dgObjects.RowCount := (numObjects + 2) div 3;

  if numObjects <= 0 then
    begin
      dgObjects.RowCount := 0;
      dgObjects.ColCount := 0;
      dgObjects.Enabled := false;
    end;

  for i := 0 to numobjects-2 do
    for j := i+1 to numobjects - 1 do
      begin
        if Prototypes[i].FindParam('name').ParamValue =
            Prototypes[j].FindParam('name').ParamValue then
              begin
                Log('Warning', 'More than one object with name %s found',
                  [Prototypes[i].FindParam('name').ParamValue]);
                ShowMessage('Warning: More than one object with name ''' +
                  Prototypes[i].FindParam('name').ParamValue + ''' found');
              end;        
      end;

  Level := TLevelFile.Create(Prototypes, @xpos, @ypos);
  Level.SnapThreshold := fIni.ReadInteger('General', 'SnapThreshold', DEFAULT_SNAP_THRESHOLD);
  CreateTools(Tools, Prototypes, Level);
  Tool := Tools[btnBrush.tag-1];
  Tools[btnSelect.Tag-1].AssignCoords(@xpos, @ypos);
  Tools[btnHand.Tag-1].AssignCoords(@xpos, @ypos);
  Tools[btnBrush.Tag-1].AssignCoords(@xpos, @ypos);
  ProtoSelected := @Prototypes[0];
  if ErrFlag then
    ;//ShowMessage('There were errors. [20:04] <Anonymous0028w> "®банные лент€и, запилите уже все объекты!"');
end;

procedure TMainForm.FormDestroy(Sender: TObject);
begin
// Free mem here
  Level.Free;
end;

function TMainForm.GetObjectFromGrid(ARow, ACol: Integer): PObjectFileProps;
var
  numCols: Integer;
begin
  numCols := dgObjects.ColCount;
  if (ARow*numCols + ACol > numObjects-1) then
    Result := nil
  else
    Result := @Prototypes[ARow*numCols + ACol];
end;

procedure TMainForm.mivGridClick(Sender: TObject);
begin
//....
// Grid.show
end;

procedure TMainForm.miAboutClick(Sender: TObject);
begin
  CenterControl(frmAbout);
  frmAbout.ShowModal;
end;

procedure TMainForm.mieDeleteClick(Sender: TObject);
begin
  Tools[0].Clear();
  Level.ClearSelected;
  Level.ClearSelection;
  pbPaintBox.Repaint;
end;


procedure TMainForm.mifCloseClick(Sender: TObject);
begin
  Level.Clear;
  pbPaintBox.Repaint;
end;

procedure TMainForm.mifExitClick(Sender: TObject);
begin
  Application.Terminate();
end;

procedure TMainForm.mifNewClick(Sender: TObject);
begin
  Tools[0].Clear();
  Level.Clear;
  xpos := 0;
  ypos := 0;
  pbPaintBox.Repaint;
end;

procedure TMainForm.mifOpenClick(Sender: TObject);
begin
  if OpenDialog.Execute then
    begin
      xpos := 0;
      ypos := 0;
      Level.Clear;
      Level.LoadFromFile(OpenDialog.FileName);
      frmScriptWnd.mmoScript.SetTextBuf(Level.pScript);
      frmScriptWnd.mmoScript.Update();
    end;
end;

procedure TMainForm.mifSaveasClick(Sender: TObject);
begin
  if SaveDialog.Execute then
    begin
      Level.pScript := frmScriptWnd.mmoScript.Lines.GetText;
      Level.SaveToFile(SaveDialog.FileName);
    end;
end;

procedure TMainForm.mifSaveClick(Sender: TObject);
begin
// .... 
end;

procedure TMainForm.mitBrushClick(Sender: TObject);
begin
  btnBrush.Down := true;
  btnBrush.Click();
end;

procedure TMainForm.mitGripClick(Sender: TObject);
begin
  btnHand.Down := true;
  btnHand.Click();
end;

procedure TMainForm.mitSelectClick(Sender: TObject);
begin
  btnSelect.Down := true;
  btnSelect.Click();
end;

procedure TMainForm.mivObjectsClick(Sender: TObject);
begin
  pnlLeftBase.Visible := not pnlLeftBase.Visible;
  with TMenuItem(Sender) do
    Checked := not Checked;
end;

procedure TMainForm.mivScriptsClick(Sender: TObject);
begin
  frmScriptWnd.ShowModal;
end;

procedure TMainForm.pbPaintBoxDblClick(Sender: TObject);
begin
  TFormObjectProps.Create(self, Level.FirstSelected().Params);
end;

procedure TMainForm.pbPaintBoxMouseDown(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
begin
  if numObjects <= 0 then
    Exit;
    begin
      pbPaintBox.Cursor := Tool.GetCursor;
      Tool.MouseDown(Button, Shift, x - xpos, y - ypos);
      StatusBar.Panels[0].Text := 'Objects Count: ' + IntToStr(Level.CountObjects);
      StatusBar.Panels[1].Text := 'MapTypes Count:' + IntToStr(Level.GetNumType);
    end;
end;

procedure TMainForm.pbPaintBoxMouseMove(Sender: TObject; Shift: TShiftState; X,
  Y: Integer);
begin
  // test
  x :=  EnsureRange(x, 0, pbPaintBox.Width);
  y :=  EnsureRange(y, 0, pbPaintBox.height);
  // test
  Tool.MouseMove(Shift, x - xpos, y - ypos);
  pbPaintBox.Cursor := Tool.GetCursor;
  pbPaintBox.Repaint;

  StatusBar.Panels[2].Text :='X: ' + IntToStr(x - xpos );
  StatusBar.Panels[3].Text :='Y: ' + IntToStr(y - ypos );
end;

procedure TMainForm.pbPaintBoxMouseUp(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
begin
  Tool.MouseUp(Button, Shift, x - xpos, y - ypos);
  pbPaintBox.Cursor := Tool.GetCursor;
  pbPaintBox.Repaint;
  
end;

procedure TMainForm.pbPaintBoxPaint(Sender: TObject);
begin
  Level.DrawObjects(Rect(0, 0, pbPaintBox.Width, pbPaintBox.Height),
    pbPaintBox.Canvas);
  Tool.Paint(pbPaintBox.Canvas);

end;

procedure TMainForm.sbHorizontalScroll(Sender: TObject; ScrollCode: TScrollCode;
  var ScrollPos: Integer);
begin
  xpos := -ScrollPos;
  pbPaintBox.Repaint;
end;

procedure TMainForm.sbVerticalScroll(Sender: TObject; ScrollCode: TScrollCode;
  var ScrollPos: Integer);
begin
  ypos := -ScrollPos;
  pbPaintBox.Repaint;
end;



procedure TMainForm.btnClick(Sender: TObject);
begin
  Tool := Tools[TSpeedButton(Sender).Tag-1];
end;

procedure TMainForm.btnSetCordsClick(Sender: TObject);
var
  x, y: Integer;
begin
  if numObjects <= 0 then
    Exit;
  if Level.GetSelectionUpperLeft(x, y) then
    begin
      //TODO
      //Level.OffsetSelected(- x + StrToInt(FrmX.ediParamValue.Text),
      //  - y + StrToInt(FrmY.ediParamValue.Text));
      pbPaintBox.Repaint;
    end;
end;


procedure TMainForm.btnFillClick(Sender: TObject);
var
  i, dx, dy, ik, j, jk: Integer;
  x, y: Integer;
begin
  if numObjects <= 0 then
    Exit;
  //TODO
  //ik := StrToInt(FrmIterX.ediParamValue.Text);
  //jk := StrToInt(FrmIterY.ediParamValue.Text);

  //dx := StrToInt(FrmDeltaX.ediParamValue.Text);
  //dy := StrToInt(FrmDeltaY.ediParamValue.Text);

  //x := StrToInt(FrmX.ediParamValue.Text);
  //y := StrToInt(FrmY.ediParamValue.Text);

  for j := 1 to jk do
    begin
      //x := StrToInt(FrmX.ediParamValue.Text);
      for i := 1 to ik do
        begin
          Tools[btnBrush.Tag-1].MouseDown(mbLeft, [], x, y);
          Tools[btnBrush.Tag-1].SetState(STATE_IDLE);
          x := x + dx;
        end;
      y := y + dy;
    end;

end;

procedure TMainForm.CenterControl(Control: TControl);
begin
  Control.Left := GetSystemMetrics(SM_CXSCREEN) div 2 - Control.Width div 2;
  Control.Top := GetSystemMetrics(SM_CYSCREEN) div 2 - Control.Height div 2;
end;

end.
