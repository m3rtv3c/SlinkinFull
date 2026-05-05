program Project1;

uses myUnit,SysUtils;

begin
var F:TAbstractFinder;
// Создаём линейный поисковик с набором данных:
F:=TLineFinder.Create([1,4,2,3,2,1]);
// Выполняем поиск:
Writeln('Индекс элемента "2": ', F.FindOne(2));
Writeln('Все индексы элемента "2": ');
for var I in F.FindAll(2) do Writeln(I);
F.Free;

end.

