{$mode objfpc}
program testLoadFromFile;

uses
  SysUtils, BaseModule, Classes;

var
  DB: TPersonDatabaseStream;
  F: TFileStream;

begin
  DB := TPersonDatabaseStream.Create;
  try

    F := TFileStream.Create('db.bin', fmOpenRead);
    try
      DB.LoadFromStream(F);  
      writeln('База данных загружена из файла db.bin');
      PrintDatabase(DB);     
    finally
      F.Free; 
    end;
  finally
    DB.Free;  
  end;
end.
