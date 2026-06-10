{$mode objfpc}{$H+}
program Exceptions;

uses
  SysUtils, Classes;

type
  TPerson = class; // Опережающее объявление

  {--- Исключения ---}
  EFileNotFound = class(Exception);         // 1.1 Файл отсутствует
  EInvalidDataType = class(Exception);      // 1.2 Некорректный тип данных
  EUnexpectedEOF = class(Exception);        // 1.3 Файл неожиданно завершился
  EFileCreateError = class(Exception);      // 2.1 Невозможно создать файл
  EIndexOutOfBounds = class(Exception);     // 3.1 Выход за пределы массива
  ESameSexParents = class(Exception);       // 4.1 Родители одного пола
  EDuplicateID = class(Exception);          // 4.2 Дубликат ID
  EInvalidParentAge = class(Exception);     // 4.3 Некорректный возраст родителя
  ECyclicRelation = class(Exception);       // 4.4 Циклическая зависимость

  {--- Класс TPerson ---}
  TPerson = class
  private
    FFullName: AnsiString;
    FGender: Boolean; // True - мужчина, False - женщина
    FBirthDate: TDateTime;
    FIdNumber: AnsiString;
    FFather: TPerson;
    FMother: TPerson;
  public
    property FullName: AnsiString read FFullName write FFullName;
    property Gender: Boolean read FGender write FGender;
    property BirthDate: TDateTime read FBirthDate write FBirthDate;
    property IdNumber: AnsiString read FIdNumber write FIdNumber;
    property Father: TPerson read FFather write FFather;
    property Mother: TPerson read FMother write FMother;
  end;

  {--- Класс TPersonList ---}
  TPersonList = class
  private
    FItems: array of TPerson;
    FCount: Integer;
    function GetItem(Index: Integer): TPerson;
    procedure SetItem(Index: Integer; Value: TPerson);
  public
    constructor Create;
    constructor CreateFromFile(const FileName: string); // Дополнительный конструктор
    destructor Destroy; override;

    procedure Add(Person: TPerson);
    procedure Delete(Index: Integer);
    procedure SaveToFile(const FileName: string);
    procedure Validate; // Метод тестирования корректности

    property Count: Integer read FCount;
    property Items[Index: Integer]: TPerson read GetItem write SetItem; default;
  end;

{ TPersonList Implementation }

constructor TPersonList.Create;
begin
  FCount := 0;
  SetLength(FItems, 0);
end;

constructor TPersonList.CreateFromFile(const FileName: string);
var
  F: TextFile;
  P: TPerson;
  I, Total: Integer;
  Line, TempStr: string;
  // Вспомогательные массивы для восстановления связей по ID
  FatherIDs: array of string;
  MotherIDs: array of string;

  function ReadNextLine: string;
  begin
    if EOF(F) then
      raise EUnexpectedEOF.Create('Ошибка 1.3: Файл неожиданно завершился при чтении данных.');
    ReadLn(F, Result);
  end;

begin
  Create;
  if not FileExists(FileName) then
    raise EFileNotFound.Create('Ошибка 1.1: Файл "' + FileName + '" не найден.');

  AssignFile(F, FileName);
  try
    Reset(F);
    
    // Читаем количество записей
    if EOF(F) then raise EUnexpectedEOF.Create('Ошибка 1.3: Пустой файл.');
    ReadLn(F, Line);
    if not TryStrToInt(Line, Total) then
      raise EInvalidDataType.Create('Ошибка 1.2: Ожидалось число (количество элементов), получено: ' + Line);

    SetLength(FatherIDs, Total);
    SetLength(MotherIDs, Total);

    // Этап 1: Чтение базовых данных личностей
    for I := 0 to Total - 1 do
    begin
      P := TPerson.Create;
      try
        P.FullName := ReadNextLine;
        
        TempStr := ReadNextLine;
        if TempStr = '1' then P.Gender := True
        else if TempStr = '0' then P.Gender := False
        else raise EInvalidDataType.Create('Ошибка 1.2: Некорректный формат поля "пол" (ожидалось 1 или 0).');

        TempStr := ReadNextLine;
        if not TryStrToDate(TempStr, P.FBirthDate) then
          raise EInvalidDataType.Create('Ошибка 1.2: Некорректный формат даты рождения: ' + TempStr);

        P.IdNumber := ReadNextLine;
        FatherIDs[I] := ReadNextLine;
        MotherIDs[I] := ReadNextLine;

        Add(P);
      except
        P.Free;
        raise;
      end;
    end;

    // Этап 2: Восстановление связей Father и Mother по ID
    for I := 0 to FCount - 1 do
    begin
      if FatherIDs[I] <> '' then
      begin
        for Total := 0 to FCount - 1 do
          if FItems[Total].IdNumber = FatherIDs[I] then
          begin
            FItems[I].Father := FItems[Total];
            break;
          end;
      end;

      if MotherIDs[I] <> '' then
      begin
        for Total := 0 to FCount - 1 do
          if FItems[Total].IdNumber = MotherIDs[I] then
          begin
            FItems[I].Mother := FItems[Total];
            break;
          end;
      end;
    end;

  finally
    CloseFile(F);
  end;
end;

destructor TPersonList.Destroy;
var
  I: Integer;
begin
  for I := 0 to FCount - 1 do
    FItems[I].Free;
  inherited Destroy;
end;

procedure TPersonList.Add(Person: TPerson);
begin
  Inc(FCount);
  SetLength(FItems, FCount);
  FItems[FCount - 1] := Person;
end;

procedure TPersonList.Delete(Index: Integer);
var
  I: Integer;
begin
  if (Index < 0) or (Index >= FCount) then
    raise EIndexOutOfBounds.Create('Ошибка 3.1: Индекс ' + IntToStr(Index) + ' выходит за пределы массива.');

  FItems[Index].Free;
  for I := Index to FCount - 2 do
    FItems[I] := FItems[I + 1];

  Dec(FCount);
  SetLength(FItems, FCount);
end;

function TPersonList.GetItem(Index: Integer): TPerson;
begin
  if (Index < 0) or (Index >= FCount) then
    raise EIndexOutOfBounds.Create('Ошибка 3.1: Попытка доступа по некорректному индексу: ' + IntToStr(Index));
  Result := FItems[Index];
end;

procedure TPersonList.SetItem(Index: Integer; Value: TPerson);
begin
  if (Index < 0) or (Index >= FCount) then
    raise EIndexOutOfBounds.Create('Ошибка 3.1: Попытка записи по некорректному индексу: ' + IntToStr(Index));
  FItems[Index] := Value;
end;

procedure TPersonList.SaveToFile(const FileName: string);
var
  F: TextFile;
  I: Integer;
  FId, MId: string;
begin
  {$I-}
  AssignFile(F, FileName);
  Rewrite(F);
  {$I+}
  if IOResult <> 0 then
    raise EFileCreateError.Create('Ошибка 2.1: Не удалось создать целевой файл: ' + FileName);

  try
    WriteLn(F, FCount);
    for I := 0 to FCount - 1 do
    begin
      WriteLn(F, FItems[I].FullName);
      if FItems[I].Gender then WriteLn(F, '1') else WriteLn(F, '0');
      WriteLn(F, DateToStr(FItems[I].BirthDate));
      WriteLn(F, FItems[I].IdNumber);
      
      FId := ''; if FItems[I].Father <> nil then FId := FItems[I].Father.IdNumber;
      MId := ''; if FItems[I].Mother <> nil then MId := FItems[I].Mother.IdNumber;
      
      WriteLn(F, FId);
      WriteLn(F, MId);
    end;
  finally
    CloseFile(F);
  end;
end;

procedure TPersonList.Validate;
var
  I, J: Integer;
  P, Slow, Fast: TPerson;
  Age: Integer;
begin
  for I := 0 to FCount - 1 do
  begin
    P := FItems[I];

    // 4.2 Проверка дубликатов ID
    for J := I + 1 to FCount - 1 do
      if P.IdNumber = FItems[J].IdNumber then
        raise EDuplicateID.Create('Ошибка 4.2: Обнаружены разные личности с одинаковым ID: ' + P.IdNumber);

    // 4.1 Проверка пола родителей
    if (P.Father <> nil) and (not P.Father.Gender) then
      raise ESameSexParents.Create('Ошибка 4.1: Отец у "' + P.FullName + '" указан как женщина.');
    if (P.Mother <> nil) and (P.Mother.Gender) then
      raise ESameSexParents.Create('Ошибка 4.1: Мать у "' + P.FullName + '" указана как мужчина.');

    // 4.3 Проверка возраста родителей
    if P.Father <> nil then
    begin
      Age := YearsBetween(P.BirthDate, P.Father.BirthDate);
      if (Age < 10) or (Age > 70) then
        raise EInvalidParentAge.Create('Ошибка 4.3: Некорректный возраст отца (' + IntToStr(Age) + ' лет) для ' + P.FullName);
    end;
    if P.Mother <> nil then
    begin
      Age := YearsBetween(P.BirthDate, P.Mother.BirthDate);
      if (Age < 12) or (Age > 60) then
        raise EInvalidParentAge.Create('Ошибка 4.3: Некорректный возраст матери (' + IntToStr(Age) + ' лет) для ' + P.FullName);
    end;

    // 4.4 Проверка на циклическую зависимость (Алгоритм черепахи и зайца для каждого родительского дерева)
    // Проверка ветки отца
    Slow := P.Father; Fast := P.Father;
    while (Fast <> nil) and (Fast.Father <> nil) do
    begin
      Slow := Slow.Father;
      Fast := Fast.Father.Father;
      if Slow = Fast then
        raise ECyclicRelation.Create('Ошибка 4.4: Обнаружен цикл в родословной личности: ' + P.FullName);
    end;
    // Проверка ветки матери
    Slow := P.Mother; Fast := P.Mother;
    while (Fast <> nil) and (Fast.Mother <> nil) do
    begin
      Slow := Slow.Mother;
      Fast := Fast.Mother.Mother;
      if Slow = Fast then
        raise ECyclicRelation.Create('Ошибка 4.4: Обнаружен цикл в родословной личности: ' + P.FullName);
    end;
  end;
end;


{--- Блок проверки (Тесты) ---}

procedure RunTests;
var
  List: TPersonList;
  P1, P2, P3: TPerson;
begin
  WriteLn('=== Старт тестирования ==='#10);

  // Тест 3.1: Выход за пределы массива
  try
    List := TPersonList.Create;
    try
      WriteLn('Тест 3.1 (Индекс)...');
      P1 := List[5]; // Ошибка
    finally
      List.Free;
    end;
  except
    on E: EIndexOutOfBounds do WriteLn('Успешно перехвачено: ', E.Message);
  end;

  // Тест 1.1: Отсутствие файла
  try
    WriteLn(#10'Тест 1.1 (Файл не найден)...');
    List := TPersonList.CreateFromFile('non_existent_file.txt');
    List.Free;
  except
    on E: EFileNotFound do WriteLn('Успешно перехвачено: ', E.Message);
  end;

  // Создаем корректный список для дальнейших логических тестов
  List := TPersonList.Create;
  try
    P1 := TPerson.Create;
    P1.FullName := 'Иван Иванов'; P1.Gender := True;
    P1.BirthDate := EncodeDate(1980, 5, 10); P1.IdNumber := 'ID111';

    P2 := TPerson.Create;
    P2.FullName := 'Мария Иванова'; P2.Gender := False;
    P2.BirthDate := EncodeDate(1983, 8, 20); P2.IdNumber := 'ID222';

    P3 := TPerson.Create;
    P3.FullName := 'Петр Иванов'; P3.Gender := True;
