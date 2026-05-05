{$mode objfpc}
program testMemoryDatabase;

uses
  SysUtils, BaseModule, Classes;

var
  DB: TPersonDatabaseStream;
  MemStream: TMemoryDatabaseStream;

begin
  DB := TPersonDatabaseStream.Create;
  MemStream := TMemoryDatabaseStream.Create;
  try
    writeln('Введите базу данных (строка END завершает ввод):');
    ReadDatabase(DB, Input);  
    writeln('===== База данных после ввода =====');
    PrintDatabase(DB); 

    DB.SaveToStream(MemStream);  
    writeln('База данных сохранена в память.');

    DB.Clear;  
    writeln('===== База данных после очистки =====');
    PrintDatabase(DB);  

    MemStream.Seek(0, soBeginning); 
    DB.LoadFromStream(MemStream);  
    writeln('===== База данных после загрузки из памяти =====');
    PrintDatabase(DB);

  finally
    MemStream.Free;
    DB.Free; 
  end;
end.
