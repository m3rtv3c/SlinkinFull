program project1;

uses
  testSave, SysUtils;

 var
  DB: TPersonDatabaseStream;
begin
  try
    // Создаем объект базы данных
    DB := TPersonDatabaseStream.Create;
    F:= TFileStream.Create;
    writeln('Введите базу данных (строка END завершает ввод):');
    ReadDatabase(DB, Input);
    PrintDatabase(DB);
    // Создаем поток для записи в файл
    F := TFileStream.Create('db.bin', fmCreate);
    try
      // Сохраняем данные в файл
      DB.SaveToStream(F);
    finally
      F.Free;  // освобождаем ресурс потока
    end;

    writeln('База сохранена в db.bin');
  finally
    DB.Free;  // освобождаем ресурс базы данных
  end;
end.
