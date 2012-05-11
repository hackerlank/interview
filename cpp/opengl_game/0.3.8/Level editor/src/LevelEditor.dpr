program LevelEditor;

uses
  Forms,
  UFormMain in 'UFormMain.pas' {MainForm},
  UObjectFile in 'UObjectFile.pas',
  UFormAbout in 'UFormAbout.pas' {frmAbout},
  ULevelFile in 'ULevelFile.pas',
  Utils in 'Utils.pas',
  UTools in 'UTools.pas',
  UFormScript in 'UFormScript.pas' {frmScriptWnd},
  UFormObjectProps in 'UFormObjectProps.pas' {FormObjectProps},
  UMetaData in 'UMetaData.pas';

{$R *.res}

begin
  Application.Initialize;
  Application.CreateForm(TMainForm, MainForm);
  Application.CreateForm(TfrmAbout, frmAbout);
  Application.CreateForm(TfrmScriptWnd, frmScriptWnd);
  Application.Run;
end.
