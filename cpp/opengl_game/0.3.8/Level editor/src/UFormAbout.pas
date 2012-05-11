unit UFormAbout;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls, jpeg, ExtCtrls;

type
  TfrmAbout = class(TForm)
    btnOk: TButton;
    mmoTextAbout: TMemo;
    pnlImage: TPanel;
    pnlBase: TPanel;
    pnlBtns: TPanel;
    imgAbout: TImage;
  end;

var
  frmAbout: TfrmAbout;

implementation

{$R *.dfm}

end.
