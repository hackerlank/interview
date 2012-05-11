object Form1: TForm1
  Left = 194
  Top = 108
  BorderStyle = bsSingle
  Caption = 'Form1'
  ClientHeight = 173
  ClientWidth = 242
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Button1: TButton
    Left = 16
    Top = 8
    Width = 209
    Height = 25
    Caption = '&Load WarCraft II'
    TabOrder = 0
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 16
    Top = 40
    Width = 209
    Height = 25
    Caption = 'Load And &Patch Resolution (1280x1024)'
    TabOrder = 1
    OnClick = Button2Click
  end
end
