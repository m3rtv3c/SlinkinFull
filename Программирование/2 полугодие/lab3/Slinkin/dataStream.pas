program PeopleDatabaseMenu;
{$mode objfpc}{$H+}

uses
  SysUtils, Classes;

//Типы данных
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

//Класс БД
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

//Хранение в памяти(в буфере_
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

//Реализация
//Create
constructor TPersonDatabaseStream.Create;
begin
  inherited Create;
  SetLength(FPeople,0);
end;

//Деструктор
destructor TPersonDatabaseStream.Destroy;
begin
  SetLength(FPeople,0);
  inherited;
end;

//Добавляем
procedure TPersonDatabaseStream.AddPerson(const P: TPerson);
begin
  SetLength(FPeople, Length(FPeople)+1);
  FPeople[High(FPeople)] := P;
end;

//Подсчет количества записей в БД
function TPersonDatabaseStream.Count: Integer;
begin
  Result := Length(FPeople);
end;

//Получаем индекс человека
function TPersonDatabaseStream.GetPerson(Index: Integer): TPerson;
begin
  if (Index>=0) and (Index<Length(FPeople)) then
    Result := FPeople[Index]
  else
    raise Exception.Create('Index out of bounds');
end;

//Очистка
procedure TPersonDatabaseStream.Clear;
begin
  SetLength(FPeople,0);
end;

//Сохраняем с потоком.(Открываем поток, добавляем туда записи, в конце освобождаем)
procedure TPersonDatabaseStream.SaveToStream(Stream: TStream);
var
  Writer: TWriter;
  i,j: Integer;
begin
  Writer := TWriter.Create(Stream,4096); //Создаем поток
  try
    Writer.WriteInteger(Length(FPeople));//Получаем количество записей, которое будет в потоке
    for i:=0 to High(FPeople) do
    begin
      Writer.WriteString(FPeople[i].fio);
      Writer.WriteString(FPeople[i].gender);
      Writer.WriteString(FPeople[i].birth);
      Writer.WriteString(FPeople[i].id);
      Writer.WriteInteger(Length(FPeople[i].children));
      for j:=0 to High(FPeople[i].children) do
        Writer.WriteString(FPeople[i].children[j]);
    end;
  finally
    Writer.Free;
  end;
end;


//Загружаем данные через поток(Открываем поток, Получаем все данные, в конце освобождаем память)
procedure TPersonDatabaseStream.LoadFromStream(Stream: TStream);
var
  Reader: TReader;
  i,j,n: Integer;
  P: TPerson;
begin
  Reader := TReader.Create(Stream,4096);
  try
    n := Reader.ReadInteger;
    SetLength(FPeople,n);
    for i:=0 to n-1 do
    begin
      P.fio := Reader.ReadString;
      P.gender := Reader.ReadString;
      P.birth := Reader.ReadString;
      P.id := Reader.ReadString;
      SetLength(P.children, Reader.ReadInteger);
      for j:=0 to High(P.children) do
        P.children[j] := Reader.ReadString;
      FPeople[i] := P;
    end;
  finally
    Reader.Free;
  end;
end;


//TmemorydataBaseStream реализация
constructor TMemoryDatabaseStream.Create;
begin
  inherited Create;
  SetLength(FBuffer,0);
  FPosition := 0;
end;


//Запись в память
function TMemoryDatabaseStream.Read(var Buffer; Count: LongInt): LongInt;
var
  BytesAvailable: LongInt;
begin
  BytesAvailable := Length(FBuffer) - FPosition; //Сколько всего доступных байт. FBuffer - массив байт. FPositon - текущая позиция в буфере
  if BytesAvailable <= 0 then Exit(0); // Если байтов нет, то выходим из программы
  if Count > BytesAvailable then Count := BytesAvailable; //Если байтов для чтения больше чем доступно, то ограничиваем количество
  Move(FBuffer[FPosition], Buffer, Count); //Копируем память из FBuffer начиная с FPosition, в Buffer, Count - количество скопированных данных. Храним данные в buffer
  Inc(FPosition, Count); //Обновляем текущую позицию на количество прочитанных байт
  Result := Count; // Количество прочитанных байт
end;

//Чтение из памяти
function TMemoryDatabaseStream.Write(const Buffer; Count: LongInt): LongInt;
begin
  if FPosition + Count > Length(FBuffer) then
    SetLength(FBuffer, FPosition + Count);
  Move(Buffer, FBuffer[FPosition], Count);
  Inc(FPosition, Count);
  Result := Count;
end;

//Ищем в памяти
function TMemoryDatabaseStream.Seek(const Offset: Int64; Origin: TSeekOrigin): Int64;
begin
  case Origin of 
    soBeginning: FPosition := Offset; //Если начало потока, то позиция = offset
    soCurrent: FPosition := FPosition + Offset; //если текущая позиция, то текущую позицию изменяем на offset.
    soEnd: FPosition := Length(FBuffer) + Offset; // Если конец потока, то позиция это длина буфера + оффсет. 
  end;
  if FPosition < 0 then FPosition := 0; // Если слишком больше смещение влево, то устанавливаем границу на 0
  if FPosition > Length(FBuffer) then FPosition := Length(FBuffer); // Если превышает максимальную позицию, то ограничиваем ее длиной, чтобы не выходить за границы
  Result := FPosition;
end;

//Очистка
procedure TMemoryDatabaseStream.Clear;
begin
  SetLength(FBuffer,0);
  FPosition := 0;
end;


//Устанавливаем размер
function TMemoryDatabaseStream.Size: Int64;
begin
  Result := Length(FBuffer);
end;


//Поиск по ID(Для детей)
function FindFIOByID(const id: AnsiString; const db: TPersonDatabaseStream): AnsiString;
var
  i: Integer;
  P: TPerson;
begin
  for i:=0 to db.Count-1 do
  begin
    P := db.GetPerson(i);
    if P.id = id then exit(P.fio);
  end;
  Result := '- - -';
end;

//Вывод БД
procedure PrintDatabase(const db: TPersonDatabaseStream);
var
  i,j: Integer;
  P: TPerson;
  fio: AnsiString;
begin
  writeln('====== База данных ======');
  for i:=0 to db.Count-1 do
  begin
    P := db.GetPerson(i);
    writeln('---------------------------');
    writeln('ФИО: ', P.fio);
    writeln('Пол: ', P.gender);
    writeln('Дата рождения: ', P.birth);
    writeln('ID: ', P.id);
    writeln('Дети:');
    if Length(P.children)=0 then
      writeln('  (нет)')
    else
      for j:=0 to High(P.children) do
      begin
        fio := FindFIOByID(P.children[j], db);
        writeln('  ', P.children[j], ' - ', fio);
      end;
  end;
  writeln('==========================');
end;

//Запись БД
procedure ReadDatabase(var db: TPersonDatabaseStream; var F: Text);
var
  P: TPerson;
  i,n: Integer;
begin
  while not Eof(F) do
  begin
    ReadLn(F,P.fio);
    if P.fio='END' then break;

    ReadLn(F,P.gender);
    ReadLn(F,P.birth);
    ReadLn(F,P.id);
    ReadLn(F,n);
    SetLength(P.children,n);
    for i:=0 to n-1 do ReadLn(F,P.children[i]);

    db.AddPerson(P);
  end;
end;

//Менюшка
procedure Menu(var DB: TPersonDatabaseStream; MemStream: TMemoryDatabaseStream);
var
  choice: Integer;
  F: TFileStream;
begin
  repeat
    writeln;
    writeln('===== МЕНЮ =====');
    writeln('1. Показать базу');
    writeln('2. Сохранить базу в файл db.bin');
    writeln('3. Загрузить базу из файла db.bin');
    writeln('4. Сохранить базу в память');
    writeln('5. Загрузить базу из памяти');
    writeln('6. Очистить базу и ввести новые данные');
    writeln('0. Выход');
    write('Выберите пункт: ');
    readln(choice);

    case choice of
      1: PrintDatabase(DB);
      2: begin
           F := TFileStream.Create('db.bin',fmCreate);
           try DB.SaveToStream(F); finally F.Free; end;
           writeln('База сохранена в db.bin');
         end;
      3: begin
           F := TFileStream.Create('db.bin',fmOpenRead);
           try DB.LoadFromStream(F); finally F.Free; end;
           writeln('База загружена из db.bin');
         end;
      4: begin
           MemStream.Clear;
           DB.SaveToStream(MemStream);
           MemStream.Seek(0, soBeginning);
           writeln('База сохранена в памяти.');
         end;
      5: begin
           if MemStream.Size = 0 then
             writeln('Память пуста. Сначала сохраните базу в память.')
           else
           begin
             DB.Clear;
             MemStream.Seek(0, soBeginning);
             DB.LoadFromStream(MemStream);
             writeln('База загружена из памяти.');
           end;
         end;
      6: begin
           DB.Clear;
           writeln('Введите новые данные (END для завершения):');
           ReadDatabase(DB,Input);
         end;
    else
      if choice<>0 then writeln('Неверный пункт!');
    end;
  until choice=0;
end;


var
  DB: TPersonDatabaseStream;
  MemStream: TMemoryDatabaseStream;

begin
  DB := TPersonDatabaseStream.Create;
  MemStream := TMemoryDatabaseStream.Create;
  try
    writeln('Введите базу данных (строка END завершает ввод):');
    ReadDatabase(DB,Input);
    PrintDatabase(DB);
    Menu(DB, MemStream);
  finally
    MemStream.Free;
    DB.Free;
  end;
end.
