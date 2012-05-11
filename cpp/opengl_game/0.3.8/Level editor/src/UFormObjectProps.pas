unit UFormObjectProps;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, UObjectFile, StdCtrls, Utils;

type
  TWinControlClass = class of TWinControl;

  TFormObjectProps = class(TForm)
    btnOk: TButton;
    btnCancel: TButton;
    ScrollBox1: TScrollBox;
  public
    Constructor Create(AOwner: TControl; AObject: TObjectFileProps);

  end;

//const
//  FIELD_CLASSES: array [0..3] of TWinControlClass = (TWinControl, TDBEdit, TDBEdit, TDBLookupComboBox);


implementation

{$R *.dfm}

{ TFormObjectProps }

constructor TFormObjectProps.Create(AOwner: TControl;
  AObject: TObjectFileProps);
var
 tmpLex: PParamLexeme;
 tmpCntrl: TControl;
 delta: Integer;
begin
  inherited Create(AOwner);
  Caption := 'prototype: ' + AObject.FindParam('name').ParamValue;

  delta := 8;
  tmpLex := @AObject.FirstParam;
  if tmpLex.ParamType = ptFirst then
    tmpLex := tmpLex^.next;
  while tmpLex^.ParamType <> ptLast do
    begin
      tmpCntrl := TLabel.Create(self);
      with tmpCntrl as TLabel do
        begin
          Caption := tmpLex.ParamName;
          Parent := self;
          Height := Canvas.TextHeight(Caption);
          Width := Canvas.TextWidth(Caption);
          Left := 8;
          Visible := true;
          Top := delta;
          Inc(Delta, Height + 5);
        end;

      tmpCntrl := TEdit.Create(self);
      with tmpCntrl as TEdit do
        begin
          Text := tmpLex.ParamValue;
          Parent := self;
          Height := Canvas.TextHeight(Text)+ 2;
          Width := Canvas.TextWidth(Text) + 10;
          Left := 16;
          Visible := true;
          Top := delta;
          Inc(Delta, Height+ 10);
        end;


      tmpLex := tmpLex^.next;
    end;

  ShowModal();    
end;

end.
