program PeopleDatabase;
{$codepage UTF8}

type
  TStringArray = array of string;

 TPerson = record
  fio: AnsiString;
  gender: AnsiString;
  birth: AnsiString;
  id: AnsiString;
  children: TStringArray;
end;

  TPeopleArray = array of TPerson;

var
  db: TPeopleArray;

{ Ищем ФИО ребенка по ID в БД, если нет → '- - -' }
function FindFIOByID(const id: string; const db: TPeopleArray): string;
var
  i: integer;
begin
  for i := 0 to High(db) do
    if db[i].id = id then
      exit(db[i].fio);

  FindFIOByID := '- - -';
end;

{ Чтение базы данных }
procedure ReadDatabase(var db: TPeopleArray);
var
  p: TPerson;
  n, i: integer;
begin
  SetLength(db, 0);

  while not eof do
  begin
    { Читаем ФИО, пропуская пустые строки }
    repeat
      if eof then exit;
      readln(p.fio);
    until p.fio <> '';

    if p.fio = 'END' then break;

    readln(p.gender);
    readln(p.birth);
    readln(p.id);

    readln(n);
    SetLength(p.children, n);
    for i := 0 to n - 1 do
    begin
      if eof then break;  { безопасно на случай EOF }
      readln(p.children[i]);
    end;

    SetLength(db, Length(db) + 1);
    db[High(db)] := p;
  end;
end;

{ Вывод всей базы данных }
procedure PrintDatabase(const db: TPeopleArray);
var
  i, j: integer;
  fio: string;
begin
  writeln;
  writeln('====== База данных ======');
  for i := 0 to High(db) do
  begin
    writeln('---------------------------');
    writeln('ФИО: ', db[i].fio);
    writeln('Пол: ', db[i].gender);
    writeln('Дата рождения: ', db[i].birth);
    writeln('Номер удостоверения: ', db[i].id);
    writeln('Дети:');

    if Length(db[i].children) = 0 then
      writeln('  (нет)')
    else
      for j := 0 to High(db[i].children) do
      begin
        fio := FindFIOByID(db[i].children[j], db);
        writeln('  ', db[i].children[j], ' - ', fio);
      end;
  end;
  writeln('==========================');
end;

begin
  ReadDatabase(db);
  PrintDatabase(db);
end.
