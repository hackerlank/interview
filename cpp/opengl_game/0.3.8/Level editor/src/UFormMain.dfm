object MainForm: TMainForm
  Left = 146
  Top = 77
  Caption = 'Level editor'
  ClientHeight = 684
  ClientWidth = 932
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  Menu = MainMenu
  OldCreateOrder = False
  Position = poDesigned
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  PixelsPerInch = 96
  TextHeight = 13
  object pnlMainBase: TPanel
    Left = 241
    Top = 0
    Width = 691
    Height = 665
    Align = alClient
    TabOrder = 0
    ExplicitHeight = 645
    object pbPaintBox: TPaintBox
      Left = 1
      Top = 1
      Width = 689
      Height = 663
      Align = alClient
      Constraints.MinHeight = 512
      Constraints.MinWidth = 512
      OnDblClick = pbPaintBoxDblClick
      OnMouseDown = pbPaintBoxMouseDown
      OnMouseMove = pbPaintBoxMouseMove
      OnMouseUp = pbPaintBoxMouseUp
      OnPaint = pbPaintBoxPaint
      ExplicitLeft = 6
      ExplicitTop = 20
      ExplicitWidth = 512
      ExplicitHeight = 544
    end
  end
  object pnlLeftBase: TPanel
    Left = 0
    Top = 0
    Width = 241
    Height = 665
    Align = alLeft
    TabOrder = 1
    ExplicitHeight = 645
    object pnlDrawGrid: TPanel
      Left = 1
      Top = 1
      Width = 239
      Height = 360
      Align = alTop
      TabOrder = 0
      object dgObjects: TDrawGrid
        Left = 10
        Top = 9
        Width = 223
        Height = 345
        ColCount = 3
        DefaultRowHeight = 64
        FixedCols = 0
        RowCount = 10
        FixedRows = 0
        Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine]
        TabOrder = 0
        OnDblClick = dgObjectsDblClick
        OnDrawCell = dgObjectsDrawCell
        OnSelectCell = dgObjectsSelectCell
      end
    end
    object pnlTools: TPanel
      Left = 1
      Top = 361
      Width = 239
      Height = 32
      Align = alTop
      TabOrder = 1
      object btnBrush: TSpeedButton
        Tag = 1
        Left = 10
        Top = 6
        Width = 23
        Height = 22
        GroupIndex = 1
        Down = True
        Glyph.Data = {
          36030000424D3603000000000000360000002800000010000000100000000100
          1800000000000003000000000000000000000000000000000000FFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFF000000000000000000FFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF000000
          000000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFF000000000000000000000000000000000000FF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          000000000000000000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF000000FFFFFF00000000000000000000
          0000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFF000000FFFFFFFFFFFF000000000000FFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000000000000000
          0000000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF000000000000000000FFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFF000000000000000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF000000000000000000FFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFF000000000000000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF000000FFFF
          FF000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFF000000FFFFFF000000FFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FF000000000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF}
        OnClick = btnClick
      end
      object btnHand: TSpeedButton
        Tag = 2
        Left = 39
        Top = 6
        Width = 23
        Height = 22
        GroupIndex = 1
        Glyph.Data = {
          36030000424D3603000000000000360000002800000010000000100000000100
          1800000000000003000000000000000000000000000000000000FFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFF0000000000000000000000000000000000000000000000
          00FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF000000D7D7D7D7
          D7D7D7D7D7D7D7D7D7D7D7D7D7D7000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFF000000000000D7D7D7D7D7D7D7D7D7D7D7D7D7D7D7D7D7D70000
          00FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF000000D7D7D7D7D7D7D7D7D7D7
          D7D7D7D7D7D7D7D7D7D7D7D7D7D7000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          000000D7D7D7D7D7D7D7D7D7D7D7D7D7D7D7D7D7D7D7D7D7D7D7D7D7D7D7D7D7
          D7000000FFFFFFFFFFFFFFFFFFFFFFFF000000D7D7D7D7D7D7D7D7D7D7D7D7D7
          D7D7D7D7D7D7D7D7D7D7D7D7D7D7D7D7D7D7D7D7000000FFFFFFFFFFFF000000
          D7D7D7D7D7D7D7D7D7D7D7D7D7D7D7D7D7D7D7D7D7D7D7D7D7D7D7D7D7D7D7D7
          D7D7D7D7000000FFFFFF000000D7D7D7D7D7D7D7D7D7000000D7D7D7D7D7D7D7
          D7D7D7D7D7D7D7D7D7D7D7D7D7D7D7D7D7D7D7D7000000FFFFFF000000D7D7D7
          D7D7D7000000000000D7D7D7D7D7D7D7D7D7D7D7D7D7D7D7D7D7D7D7D7D7D7D7
          D7D7D7D7D7D7D7000000FFFFFF000000000000FFFFFF000000D7D7D7D7D7D7D7
          D7D7D7D7D7D7D7D7D7D7D7D7D7D7000000D7D7D7D7D7D7000000FFFFFFFFFFFF
          FFFFFF000000D7D7D7D7D7D7000000D7D7D7D7D7D7000000D7D7D7D7D7D70000
          00D7D7D7D7D7D7000000FFFFFFFFFFFFFFFFFF000000D7D7D7D7D7D7000000D7
          D7D7D7D7D7000000D7D7D7D7D7D7000000D7D7D7D7D7D7000000FFFFFFFFFFFF
          000000D7D7D7D7D7D7000000000000D7D7D7D7D7D7000000D7D7D7D7D7D70000
          00000000D7D7D7000000FFFFFFFFFFFF000000D7D7D7D7D7D7000000000000D7
          D7D7D7D7D7000000D7D7D7D7D7D7000000FFFFFF000000FFFFFFFFFFFFFFFFFF
          FFFFFF000000000000FFFFFF000000D7D7D7D7D7D7000000000000000000FFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00
          0000000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF}
        OnClick = btnClick
      end
      object btnSelect: TSpeedButton
        Tag = 3
        Left = 68
        Top = 6
        Width = 23
        Height = 22
        GroupIndex = 1
        Glyph.Data = {
          36030000424D3603000000000000360000002800000010000000100000000100
          1800000000000003000000000000000000000000000000000000FFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF000000000000FFFFFF000000000000FF
          FFFF000000000000FFFFFF000000000000FFFFFF000000FFFFFFFFFFFF000000
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFF000000FFFFFFFFFFFF000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFF000000FFFFFFFFFFFF000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF000000FFFFFFFFFFFF000000
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF000000FFFFFFFFFFFF000000
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFF000000FFFFFFFFFFFF000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFF000000FFFFFFFFFFFF000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF000000FFFFFFFFFFFF000000
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF000000FFFFFFFFFFFFFFFFFF
          000000000000FFFFFF000000000000FFFFFF000000000000FFFFFF0000000000
          00FFFFFF000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF}
        OnClick = btnClick
      end
    end
    object pnlParamBase: TPanel
      Left = 1
      Top = 393
      Width = 239
      Height = 56
      Align = alTop
      TabOrder = 2
      object pnlFirstParam: TPanel
        Left = 1
        Top = 1
        Width = 111
        Height = 54
        Align = alClient
        TabOrder = 0
      end
      object pnlSecondParam: TPanel
        Left = 112
        Top = 1
        Width = 126
        Height = 54
        Align = alRight
        TabOrder = 1
      end
    end
    object pnlKeybrdInput: TPanel
      Left = 1
      Top = 449
      Width = 239
      Height = 160
      Align = alTop
      TabOrder = 3
      object gbKeybInput: TGroupBox
        AlignWithMargins = True
        Left = 6
        Top = 1
        Width = 227
        Height = 153
        Margins.Left = 5
        Margins.Top = 0
        Margins.Right = 5
        Margins.Bottom = 5
        Align = alClient
        Caption = 'Keyboard input'
        TabOrder = 0
        object pnlInputX: TPanel
          Left = 2
          Top = 15
          Width = 55
          Height = 136
          Align = alLeft
          BevelOuter = bvNone
          TabOrder = 0
        end
        object pnlInputY: TPanel
          Left = 57
          Top = 15
          Width = 56
          Height = 136
          Align = alLeft
          BevelOuter = bvNone
          TabOrder = 1
        end
        object pnlInputBtns: TPanel
          Left = 113
          Top = 15
          Width = 112
          Height = 136
          Align = alClient
          BevelEdges = []
          BevelOuter = bvNone
          Ctl3D = True
          ParentCtl3D = False
          TabOrder = 2
          object btnFill: TButton
            Left = 6
            Top = 105
            Width = 99
            Height = 23
            Caption = '&Fill'
            Enabled = False
            TabOrder = 0
            OnClick = btnFillClick
          end
          object btnSetCords: TButton
            Left = 6
            Top = 75
            Width = 99
            Height = 24
            Caption = '&Set'
            TabOrder = 1
            OnClick = btnSetCordsClick
          end
        end
      end
    end
    object pnlFill: TPanel
      Left = 1
      Top = 609
      Width = 239
      Height = 72
      Align = alTop
      TabOrder = 4
    end
  end
  object StatusBar: TStatusBar
    Left = 0
    Top = 665
    Width = 932
    Height = 19
    Panels = <
      item
        Text = 'Objects count:'
        Width = 100
      end
      item
        Text = 'MapTypes Count:'
        Width = 100
      end
      item
        Text = 'X:'
        Width = 50
      end
      item
        Text = 'Y:'
        Width = 50
      end>
    ExplicitTop = 645
  end
  object MainMenu: TMainMenu
    Left = 568
    Top = 24
    object miFile: TMenuItem
      Caption = '&File'
      object mifNew: TMenuItem
        Caption = '&New'
        OnClick = mifNewClick
      end
      object mifOpen: TMenuItem
        Caption = '&Open...'
        OnClick = mifOpenClick
      end
      object mifSave: TMenuItem
        Caption = '&Save'
        Enabled = False
        OnClick = mifSaveClick
      end
      object mifSaveas: TMenuItem
        Caption = 'Save &as'
        OnClick = mifSaveasClick
      end
      object mifDiv1: TMenuItem
        Caption = '-'
      end
      object mifExit: TMenuItem
        Caption = '&Exit'
        OnClick = mifExitClick
      end
    end
    object miEdit: TMenuItem
      Caption = '&Edit'
      object mieDelete: TMenuItem
        Caption = '&Delete'
        ShortCut = 46
        OnClick = mieDeleteClick
      end
    end
    object miTools: TMenuItem
      Caption = '&Tools'
      object mitBrush: TMenuItem
        Caption = '&Brush'
        ShortCut = 66
        OnClick = mitBrushClick
      end
      object mitGrip: TMenuItem
        Caption = '&Grip Tool'
        ShortCut = 71
        OnClick = mitGripClick
      end
      object mitSelect: TMenuItem
        Caption = '&Selection'
        ShortCut = 82
        OnClick = mitSelectClick
      end
    end
    object miView: TMenuItem
      Caption = '&View'
      object mivObjects: TMenuItem
        Caption = '&Objects'
        Checked = True
        ShortCut = 49231
        OnClick = mivObjectsClick
      end
      object mivScripts: TMenuItem
        Caption = '&Scripts'
        OnClick = mivScriptsClick
      end
      object mivGrid: TMenuItem
        Caption = '&Grid'
        Enabled = False
        OnClick = mivGridClick
      end
    end
    object miAbout: TMenuItem
      Caption = '&About...'
      OnClick = miAboutClick
    end
  end
  object OpenDialog: TOpenDialog
    Left = 536
    Top = 24
  end
  object SaveDialog: TSaveDialog
    Left = 504
    Top = 24
  end
  object XPManifest: TXPManifest
    Left = 472
    Top = 24
  end
end
