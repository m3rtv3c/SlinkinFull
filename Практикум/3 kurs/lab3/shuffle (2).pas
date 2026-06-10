{$mode objfpc}
 var s:ansistring;
     i:integer;
begin
 randomize;
 while not eof() do begin
  readln(s);
  i:=pos('shgpi.edu.ru',s);
  if i=0 then continue;
  writeln(copy(s,1,i-1),'new',random(1000),'.',copy(s,i,length(s)));
 end;
end.