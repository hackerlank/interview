object MainForm: TMainForm
  Left = 287
  Top = 0
  Width = 755
  Height = 538
  Caption = 'MainForm'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  PixelsPerInch = 96
  TextHeight = 13
  object Panel2: TPanel
    Left = 1
    Top = 0
    Width = 640
    Height = 480
    Cursor = -1
    Align = alCustom
    Caption = 'Panel2'
    TabOrder = 0
    OnMouseDown = Panel2MouseDown
    OnMouseMove = Panel2MouseMove
    OnMouseUp = Panel2MouseUp
  end
  object Button1: TButton
    Left = 648
    Top = 0
    Width = 75
    Height = 25
    Caption = 'Soldiers'
    TabOrder = 1
    OnClick = Button1Click
  end
  object MainMenu1: TMainMenu
    Left = 680
    Top = 88
    object File1: TMenuItem
      Caption = 'File'
    end
  end
  object AsphyreDevice: TAsphyreDevice
    Width = 640
    Height = 480
    BitDepth = bdHigh
    Refresh = 0
    Windowed = True
    VSync = False
    HardwareTL = True
    DepthBuffer = False
    WindowHandle = 0
    OnInitialize = AsphyreDeviceInitialize
    OnRender = AsphyreDeviceRender
    Left = 648
    Top = 88
  end
  object AsphyreTimer: TAsphyreTimer
    Speed = 60.000000000000000000
    MaxFPS = 100
    Enabled = False
    OnTimer = AsphyreTimerTimer
    OnProcess = AsphyreTimerProcess
    Left = 712
    Top = 88
  end
  object AsphyreCanvas: TAsphyreCanvas
    Publisher = AsphyreDevice
    AlphaTesting = True
    VertexCache = 4096
    Antialias = True
    Dithering = False
    Left = 648
    Top = 64
  end
  object AsphyreImages: TAsphyreImages
    Publisher = AsphyreDevice
    MipMappping = False
    Left = 680
    Top = 64
  end
  object AsphyreFonts: TAsphyreFonts
    Publisher = AsphyreDevice
    Canvas = AsphyreCanvas
    Left = 648
    Top = 32
  end
  object ASDb: TASDb
    OpenMode = opUpdate
    Left = 712
    Top = 64
  end
end
