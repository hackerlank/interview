unit Unit1;
{$R *.dfm}

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls;

type
  TForm1 = class(TForm)
    Button1: TButton;
    Button2: TButton;
    procedure Button1Click(Sender: TObject);
    procedure Button2Click(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  Form1: TForm1;

implementation

type LPHPROCESS = ^THandle;
PFARPROC=^FARPROC;
PDWORD =^DWORD;

const
  CLASS_NAME = 'War2Class';
  WINDOW_NAME = 'Warcraft II';
  PROGRAM_NAME = 'C:\Program Files\War2Combat\Warcraft II BNE.exe';
  CRACK_CD_ADDR = $428E17;
  DISP_MODE_WIDTH_ADDR = $48C431;
  DISP_MODE_HEIGHT_ADDR = $48C42C;
var
  ProcessInfo: PROCESS_INFORMATION;
  StartupInfo: TSTARTUPINFO;
  flOldProtect: integer;
  nBytesWritten: Cardinal;
  WriteCount: Cardinal = 0;





procedure PatchAddress(Process: Cardinal; lpAddress: Integer; lpBuffer: Integer; nSize: DWORD);
begin
  Inc(WriteCount);
  if(VirtualProtectEx(Process, Pointer(lpAddress), nSize, $40, @flOldProtect)) then
  begin
    if not (WriteProcessMemory(Process, Pointer(lpAddress), @lpBuffer, nSize, nBytesWritten)) then
      ShowMessage(IntToStr(WriteCount)+': WriteProcessMemory() failed');
  end
    else
    ShowMessage(IntToStr(WriteCount)+': VirtualProtectEx() failed');
end;

procedure PatchNoCD();
begin
  GetStartupInfo(StartupInfo);
  CreateProcess(PAnsiChar(PROGRAM_NAME), nil, nil, nil, FALSE, NORMAL_PRIORITY_CLASS,
  nil, nil, StartupInfo, ProcessInfo);
  PatchAddress(ProcessInfo.hProcess, CRACK_CD_ADDR, 0, 1);
end;

procedure PatchResolution(Width, Height: integer);
begin
  // patching resolution
  PatchAddress(ProcessInfo.hProcess, DISP_MODE_WIDTH_ADDR, width, 2);
  PatchAddress(ProcessInfo.hProcess, DISP_MODE_HEIGHT_ADDR, height, 2);
  PatchAddress(ProcessInfo.hProcess, $401280 + 5, width, 2);
end;

procedure TForm1.Button1Click(Sender: TObject);
begin
    PatchNoCD();
end;

procedure TForm1.Button2Click(Sender: TObject);
begin
    PatchNoCD();
    PatchResolution(1280,1024);
end;

end.
