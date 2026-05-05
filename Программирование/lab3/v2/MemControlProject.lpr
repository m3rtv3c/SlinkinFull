{$mode objfpc}
uses heaptrc, memcontrol, sysutils;

var
  x: TIntMemStorage;
  y: TFloatMemStorage;
  z: TIntFileStorage;
begin
  x := TIntMemStorage.Create();
  writeln('---TIntMemStorage---');
  x[5] := 7;
  x[3] := 8;
  writeln('x[1]: ', x[1]);
  writeln('x[3]: ', x[3]);
  writeln('x[5]: ', x[5]);
  writeln('Размер: ', x.count);
  x.free;

  y := TFloatMemStorage.Create();
  writeln('---TFloatMemStorage---');
  y[2] := 1.2;
  writeln('y[0]: ', y[0]:0:2);
  writeln('y[2]: ', y[2]:0:2);
  writeln('Размер: ', y.count);
  y.free;

  z := TIntFileStorage.Create('test.dat');
  writeln('---TIntFileStorage---');
  //writeln('z[2]: ', z[2]);
  //z[2] := 1;
  //writeln('z[2] = 1: ', z[2]);
  //z[0] := -11;
  //writeln('z[0] = -11: ', z[0]);
  //writeln('z[1000000] = 123: ', z[1000000]);
  z[1000000] := 123;
  writeln('z[1000000]: ', z[1000000]);
  writeln('Размер: ', z.count);
  z.free;

  writeln('---Файл закрыт---');
  z := TIntFileStorage.Create('test.dat');
  writeln('---Снова открываем файл---');
  writeln('Открыли снова z[0]: ', z[0]);
  writeln('Открыли снова z[2]: ', z[2]);
  writeln('Открыли снова z[1000000]: ', z[1000000]);
  z.free;

end.

