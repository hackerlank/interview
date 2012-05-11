unit UMetaData;

interface

uses UObjectFile;

var
  ProtoCount, FuncCount: Integer;
  Prototypes, Functions: TObjectFilePropsArr;
  ProtoSelected: PObjectFileProps;  

  function FunctionByName(FunctionName: string): PObjectFileProps;
  function ProtoByName(ProtoName: string): PObjectFileProps;  

implementation

// ёбаный полный перебор в поиске. Это пиздец ящитаю, надо будет дерево делать или ченить не O(n^2)

function FunctionByName(FunctionName: string): PObjectFileProps;
var
  i: Integer;
begin
  Result := nil;
  for i := 0 to FuncCount-1 do
    if Functions[i].FindParam('name').ParamValue = FunctionName then
      begin
        Result := @Functions[i];
        break;
      end;
end;

function ProtoByName(ProtoName: string): PObjectFileProps;
var
  i: Integer;
begin
  Result := nil;
  for i := 0 to ProtoCount-1 do
    if Prototypes[i].FindParam('name').ParamValue = ProtoName then
      begin
        Result := @Prototypes[i];
        break;
      end;
end;


initialization

ProtoCount := 0;
FuncCount := 0;

end.
