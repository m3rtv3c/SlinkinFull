unit BaseModule;

{$mode objfpc}{$H+}

interface

uses
  SysUtils, Classes;

type
  TStringArray = array of AnsiString;

  TPerson = record
    fio: AnsiString;
    gender: AnsiString;
    birth: AnsiString;
    id: AnsiString;
    children: TStringArray;
  end;

  TPeopleArray = array of TPerson;

  TPersonDatabaseStream = class
  private
    FPeople: TPeopleArray;
  public
    constructor Create;
    destructor Destroy; override;

    procedure AddPerson(const P: TPerson);
    function Count: Integer;
    function GetPerson(Index: Integer): TPerson;
    procedure Clear;

    procedure SaveToStream(Stream: TStream);
    procedure LoadFromStream(Stream: TStream);
  end;

  TMemoryDatabaseStream = class(TStream)
  private
    FBuffer: array of Byte;
    FPosition: Int64;
  public
    constructor Create;
    function Read(var Buffer; Count: LongInt): LongInt; override;
    function Write(const Buffer; Count: LongInt): LongInt; override;
    function Seek(const Offset: Int64; Origin: TSeekOrigin): Int64; override;
    procedure Clear;
    function Size: Int64;
  end;

  procedure PrintDatabase(const db: TPersonDatabaseStream);
  procedure ReadDatabase(var db: TPersonDatabaseStream; var F: Text);
  function FindFIOByID(const id: AnsiString; const db: TPersonDatabaseStream): AnsiString;

implementation

{ TPersonDatabaseStream }

constructor TPersonDatabaseStream.Create;
begin
  inherited Create;
  SetLength(FPeople, 0);
end;

destructor TPersonDatabaseStream.Destroy;
begin
  SetLength(FPeople, 0);
  inherited;
end;

procedure TPersonDatabaseStream.AddPerson(const P: TPerson);
begin
  SetLength(FPeople, Length(FPeople) + 1);
  FPeople[High(FPeople)] := P;
end;

function TPersonDatabaseStream.Count: Integer;
begin
  Result := Length(FPeople);
end;

function TPersonDatabaseStream.GetPerson(Index: Integer): TPerson;
begin
  if (Index >= 0) and (Index < Length(FPeople)) then
    Result := FPeople[Index]
  else
    raise Exception.Create('Index out of bounds');
end;

procedure TPersonDatabaseStream.Clear;
begin
  SetLength(FPeople, 0);
end;

procedure TPersonDatabaseStream.SaveToStream(Stream: TStream);
var
  Writer: TWriter;
  i, j: Integer;
begin
  Writer := TWriter.Create(Stream, 4096);
  try
    Writer.WriteInteger(Length(FPeople));
    for i := 0 to High(FPeople) do
    begin
      Writer.WriteString(FPeople[i].fio);
      Writer.WriteString(FPeople[i].gender);
      Writer.WriteString(FPeople[i].birth);
      Writer.WriteString(FPeople[i].id);
      Writer.WriteInteger(Length(FPeople[i].children));
      for j := 0 to High(FPeople[i].children) do
        Writer.WriteString(FPeople[i].children[j]);
    end;
  finally
    Writer.Free;
  end;
end;

procedure TPersonDatabaseStream.LoadFromStream(Stream: TStream);
var
  Reader: TReader;
  i, j, n: Integer;
  P: TPerson;
begin
  Reader := TReader.Create(Stream, 4096);
  try
    n := Reader.ReadInteger;
    SetLength(FPeople, n);
    for i := 0 to n - 1 do
    begin
      P.fio := Reader.ReadString;
      P.gender := Reader.ReadString;
      P.birth := Reader.ReadString;
      P.id := Reader.ReadString;
      SetLength(P.children, Reader.ReadInteger);
      for j := 0 to High(P.children) do
        P.children[j] := Reader.ReadString;
      FPeople[i] := P;
    end;
  finally
    Reader.Free;
  end;
end;

{ TMemoryDatabaseStream }

constructor TMemoryDatabaseStream.Create;
begin
  inherited Create;
  SetLength(FBuffer, 0);
  FPosition := 0;
end;

function TMemoryDatabaseStream.Read(var Buffer; Count: LongInt): LongInt;
var
  BytesAvailable: LongInt;
begin
  BytesAvailable := Length(FBuffer) - FPosition;
  if BytesAvailable <= 0 then Exit(0);
  if Count > BytesAvailable then Count := BytesAvailable;
  Move(FBuffer[FPosition], Buffer, Count);
  Inc(FPosition, Count);
  Result := Count;
end;

function TMemoryDatabaseStream.Write(const Buffer; Count: LongInt): LongInt;
begin
  if FPosition + Count > Length(FBuffer) then
    SetLength(FBuffer, FPosition + Count);
  Move(Buffer, FBuffer[FPosition], Count);
  Inc(FPosition, Count);
  Result := Count;
end;

function TMemoryDatabaseStream.Seek(const Offset: Int64; Origin: TSeekOrigin): Int64;
begin
  case Origin of
    soBeginning: FPosition := Offset;
    soCurrent: FPosition := FPosition + Offset;
    soEnd: FPosition := Length(FBuffer) + Offset;
  end;
  if FPosition < 0 then FPosition := 0;
  if FPosition > Length(FBuffer) then FPosition := Length(FBuffer);
  Result := FPosition;
end;

procedure TMemoryDatabaseStream.Clear;
begin
  SetLength(FBuffer, 0);
  FPosition := 0;
end;

function TMemoryDatabaseStream.Size: Int64;
begin
  Result := Length(FBuffer);
end;

{ Дополнительные процедуры }

function FindFIOByID(const id: AnsiString; const db: TPersonDatabaseStream): AnsiString;
var
  i: Integer;
  P: TPerson;
begin
  for i := 0 to db.Count - 1 do
  begin
    P := db.GetPerson(i);
    if P.id = id then exit(P.fio);
  end;
  Result := '- - -';
end;

procedure PrintDatabase(const db: TPersonDatabaseStream);
var
  i, j: Integer;
  P: TPerson;
  fio: AnsiString;
begin
  writeln('====== База данных ======');
  for i := 0 to db.Count - 1 do
  begin
    P := db.GetPerson(i);
    writeln('---------------------------');
    writeln('ФИО: ', P.fio);
    writeln('Пол: ', P.gender);
    writeln('Дата рождения: ', P.birth);
    writeln('ID: ', P.id);
    writeln('Дети:');
    if Length(P.children) = 0 then
      writeln('  (нет)')
    else
      for j := 0 to High(P.children) do
      begin
        fio := FindFIOByID(P.children[j], db);
        writeln('  ', P.children[j], ' - ', fio);
      end;
  end;
  writeln('==========================');
end;

procedure ReadDatabase(var db: TPersonDatabaseStream; var F: Text);
var
  P: TPerson;
  i, n: Integer;
begin
  while not Eof(F) do
  begin
    ReadLn(F, P.fio);
    if P.fio = 'END' then break;

    ReadLn(F, P.gender);
    ReadLn(F, P.birth);
    ReadLn(F, P.id);
    ReadLn(F, n);
    SetLength(P.children, n);
    for i := 0 to n - 1 do ReadLn(F, P.children[i]);

    db.AddPerson(P);
  end;
end;

end.
