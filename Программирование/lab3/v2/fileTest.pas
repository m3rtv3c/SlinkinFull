{$mode objfpc}
uses heaptrc,sysutils,memcontrol;

var
  z: TIntFileStorage;
begin
  writeln('---Проверка test.dat---');
  z := TIntFileStorage.Create('test.dat');
  
  writeln('Старое значение z[2] = 1, Сейчас: ', z[2]);  
  writeln('Старое значение z[1000000]= 123, Сейчас: ', z[1000000]);  
  
  writeln('Старое значение z[0] = -11, Сейчас ', z[0]);  
  
  z.free;
end.
