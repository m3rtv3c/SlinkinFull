program project1;

{$mode objfpc}{$H+}

uses
  SysUtils, testSave, Classes;

var
  DB: TPersonDatabaseStream;
  F: TFileStream;

begin
  DB := TPersonDatabaseStream.Create;
  try
    writeln('Введите базу данных (строка END завершает ввод):');
    ReadDatabase(DB, Input);
    PrintDatabase(DB);

    F := TFileStream.Create('db.bin', fmCreate);
    try
      DB.SaveToStream(F);
      writeln('База данных сохранена в файл db.bin');
    finally
      F.Free;
    end;

  finally
    DB.Free;
  end;
end.

