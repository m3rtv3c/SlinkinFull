program task1;

//{$mode objfpc}{$H+}

uses
 sysutils;
 var i, N:Integer;
   summ, proizv, chetn, nechetn: Integer;


begin
  summ := 0;
  proizv := 1;
  chetn := 0;
  nechetn := 0;
  if ParamCount = 0 then
  begin
    writeln('нет аргументов');
    Exit;
  end;

  for i := 1 to ParamCount do
  begin
    N := StrToInt(ParamStr(i));
         summ := summ + N;
         if N <> 0 then
         proizv  := proizv * N;
         if N mod 2 = 0 then
         chetn := chetn + 1
         else
         nechetn := nechetn + 1;
end;
 writeln('сумма:', summ);
 writeln('произведение:', proizv);
 writeln('кол-во четных:', chetn);
 writeln('кол-во нечетных:', nechetn);
end.

