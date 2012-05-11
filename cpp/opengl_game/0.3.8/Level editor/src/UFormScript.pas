unit UFormScript;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls;

type
  TfrmScriptWnd = class(TForm)
    mmoScript: TMemo;
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  frmScriptWnd: TfrmScriptWnd;

implementation

{$R *.dfm}

end.
