program untitled;

uses
  Forms,
  main_editor in 'main_editor.pas' {MainForm},
  header in 'header.pas',
  misc in 'misc.pas',
  config in 'config.pas';

{$R *.res}

begin
  Application.Initialize;
  Application.CreateForm(TMainForm, MainForm);
  Application.Run;
end.
