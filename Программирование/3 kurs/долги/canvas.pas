{$mode objfpc}
program StrictFinders;


uses
  SysUtils;

type
  TIntArray = array of Integer;
  TCompareFunc = function(const A, B: Integer): Integer;

type
  TAbstractFinder = class
  protected
    class var FData: TIntArray;
    class var FCompare: TCompareFunc;
    class var FCurrent: TAbstractFinder;

    FSearchValue: Integer;

    function CheckData: Boolean; virtual; abstract;
    function FindOne: Integer; virtual; abstract;
    function FindAll: TIntArray; virtual; abstract;

  public
    constructor Create(const AData: array of Integer); virtual;
    destructor Destroy; override;

    class procedure SetData(const AData: array of Integer); static;
    class procedure SetCompare(ACompare: TCompareFunc); static;

    class function FindOne(Value: Integer): Integer; overload; static;
    class function FindOne(Value: Integer;
      ACompare: TCompareFunc): Integer; overload; static;

    class function FindAll(Value: Integer): TIntArray; overload; static;
    class function FindAll(Value: Integer;
      ACompare: TCompareFunc): TIntArray; overload; static;
  end;

{ ===== TAbstractFinder ===== }

constructor TAbstractFinder.Create(const AData: array of Integer);
begin
  SetData(AData);
  FCurrent := Self;
end;

destructor TAbstractFinder.Destroy;
begin
  if FCurrent = Self then
    FCurrent := nil;
  inherited;
end;

class procedure TAbstractFinder.SetData(const AData: array of Integer);
var
  i: Integer;
begin
  SetLength(FData, Length(AData));
  for i := 0 to High(AData) do
    FData[i] := AData[i];
end;

class procedure TAbstractFinder.SetCompare(ACompare: TCompareFunc);
begin
  FCompare := ACompare;
end;

class function TAbstractFinder.FindOne(Value: Integer): Integer;
begin
  Result := FindOne(Value, FCompare);
end;

class function TAbstractFinder.FindOne(Value: Integer;
  ACompare: TCompareFunc): Integer;
begin
  if not Assigned(FCurrent) then
    raise Exception.Create('Finder not created');

  FCompare := ACompare;
  FCurrent.FSearchValue := Value;

  Result := FCurrent.FindOne;
end;

class function TAbstractFinder.FindAll(Value: Integer): TIntArray;
begin
  Result := FindAll(Value, FCompare);
end;

class function TAbstractFinder.FindAll(Value: Integer;
  ACompare: TCompareFunc): TIntArray;
begin
  if not Assigned(FCurrent) then
    raise Exception.Create('Finder not created');

  FCompare := ACompare;
  FCurrent.FSearchValue := Value;

  Result := FCurrent.FindAll;
end;

{ ===== ЛИНЕЙНЫЙ ПОИСК ===== }

type
  TLineFinder = class(TAbstractFinder)
  protected
    function CheckData: Boolean; override;
    function FindOne: Integer; override;
    function FindAll: TIntArray; override;
  end;

function TLineFinder.CheckData: Boolean;
begin
  Result := Length(FData) > 0;
end;

function TLineFinder.FindOne: Integer;
var
  i: Integer;
begin
  Result := -1;
  if not CheckData then Exit;

  for i := 0 to High(FData) do
    if FCompare(FData[i], FSearchValue) = 0 then
      Exit(i);
end;

function TLineFinder.FindAll: TIntArray;
var
  i, Count: Integer;
begin
  Result := nil;
  if not CheckData then Exit;

  Count := 0;
  for i := 0 to High(FData) do
    if FCompare(FData[i], FSearchValue) = 0 then
    begin
      SetLength(Result, Count + 1);
      Result[Count] := i;
      Inc(Count);
    end;
end;

{ ===== БИНАРНЫЙ ПОИСК ===== }

type
  TBinFinder = class(TAbstractFinder)
  private
    function IsSorted: Boolean;
  protected
    function CheckData: Boolean; override;
    function FindOne: Integer; override;
    function FindAll: TIntArray; override;
  end;

function TBinFinder.IsSorted: Boolean;
var
  i: Integer;
begin
  Result := True;
  for i := 1 to High(FData) do
    if FData[i] < FData[i - 1] then
      Exit(False);
end;

function TBinFinder.CheckData: Boolean;
begin
  Result := (Length(FData) > 0) and IsSorted;
end;

function TBinFinder.FindOne: Integer;
var
  L, R, M: Integer;
begin
  Result := -1;
  if not CheckData then Exit;

  L := 0;
  R := High(FData);

  while L <= R do
  begin
    M := (L + R) div 2;

    if FCompare(FData[M], FSearchValue) = 0 then
      Exit(M)
    else if FData[M] < FSearchValue then
      L := M + 1
    else
      R := M - 1;
  end;
end;

function TBinFinder.FindAll: TIntArray;
var
  idx: Integer;
begin
  Result := nil;

  idx := FindOne;
  if idx <> -1 then
  begin
    SetLength(Result, 1);
    Result[0] := idx;
  end;
end;

{ ===== Сравнение ===== }

function IntCompare(const A, B: Integer): Integer;
begin
  Result := A - B;
end;

{ ===== ТЕСТ ===== }

var
  F: TAbstractFinder;
  Res: TIntArray;
  i: Integer;

begin
  // Для линейного поиска
  // F := TLineFinder.Create([1,4,2,3,2,1]);

  // Для бинарного поиска (массив отсортирован!)
  F := TBinFinder.Create([1,2,2,3,4,10]);

  TAbstractFinder.SetCompare(@IntCompare);

  Writeln('FindOne(2) = ', TAbstractFinder.FindOne(2));

  Res := TAbstractFinder.FindAll(2);
  Write('FindAll(2): ');
  for i := 0 to High(Res) do
    Write(Res[i], ' ');
  Writeln;

  F.Free;
end.

