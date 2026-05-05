program TypeInspector;

{$mode objfpc}
{$H+}
{$interfaces CORBA}
uses
  SysUtils, TypInfo;
type
generic TTypeInfo<T> = class
  public
    procedure Inspect(const TypeName: string);
  end;

procedure TTypeInfo.Inspect(const TypeName: string);
var
  V: T;
  LowVal, HighVal: T;
  PrevVal, NextVal: String;
  ByteArr: PByte;
  Size: Integer;
  IsMin, IsMax: Boolean;
begin
  Randomize;

  LowVal := Low(T);
  HighVal := High(T);
  Size := SizeOf(T);

  FillChar(V, Size, 0);
  ByteArr := @V;
  for var i := 0 to Size - 1 do
    ByteArr[i] := Random(256);

  if V < LowVal then V := LowVal;
  if V > HighVal then V := HighVal;

  IsMin := V = LowVal;
  IsMax := V = HighVal;

  WriteLn('Исследуемый тип: ', TypeName);

  Write('Нижняя граница: ');
  if TypeName = 'Char' then
    WriteLn('#', Ord(LowVal))
  else if TypeName = 'Boolean' then
    WriteLn(BoolToStr(Low(Boolean), True))
  else
    WriteLn(LowVal);

  Write('Верхняя граница: ');
  if TypeName = 'Char' then
    WriteLn('#', Ord(HighVal))
  else if TypeName = 'Boolean' then
    WriteLn(BoolToStr(High(Boolean), True))
  else
    WriteLn(HighVal);

  WriteLn('Байт на переменную: ', Size);

  Write('Случайное значение: ');
  if TypeName = 'Char' then
    WriteLn('#', Ord(V))
  else if TypeName = 'Boolean' then
    WriteLn(BoolToStr(Boolean(V), True))
  else
    WriteLn(V);

  Write('Предыдущее значение: ');
  if IsMin then
    WriteLn('Overflow')
  else
    if TypeName = 'Char' then
      WriteLn('#', Ord(Pred(V)))
    else if TypeName = 'Boolean' then
      WriteLn(BoolToStr(Pred(Boolean(V)), True))
    else
      WriteLn(Pred(V));

  Write('Последующее значение: ');
  if IsMax then
    WriteLn('Overflow')
  else
    if TypeName = 'Char' then
      WriteLn('#', Ord(Succ(V)))
    else if TypeName = 'Boolean' then
      WriteLn(BoolToStr(Succ(Boolean(V)), True))
    else
      WriteLn(Succ(V));

  Write('Содержимое оперативной памяти: ');
  for var i := 0 to Size - 1 do
    Write(ByteArr[i], ' ');
  WriteLn;
end;


procedure ProcessType(const TypeName: string);
begin
  case ParseTypeName(TypeName) of
    stShortInt:  specialize TTypeInfo<ShortInt>.Inspect;
    stSmallInt:  specialize TTypeInfo<SmallInt>.Inspect;
    stLongInt:   specialize TTypeInfo<LongInt>.Inspect;
    stLongWord:  specialize TTypeInfo<LongWord>.Inspect;
    stInt64:     specialize TTypeInfo<Int64>.Inspect;
    stByte:      specialize TTypeInfo<Byte>.Inspect;
    stWord:      specialize TTypeInfo<Word>.Inspect;
    stCardinal:  specialize TTypeInfo<Cardinal>.Inspect;
    stQWord:     specialize TTypeInfo<QWord>.Inspect;
    stChar:      specialize TTypeInfo<Char>.Inspect;
    stBoolean:   specialize TTypeInfo<Boolean>.Inspect;
  else
    WriteLn('Error');
  end;
end;

var
  InputType: string;

begin
  Write('Введите тип: ');
  ReadLn(InputType);
  ProcessType(InputType);
end.
