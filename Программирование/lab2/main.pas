unit main;

{$mode objfpc}{$H+}

//Сохраняй свой проект у себя в папке :)
interface

uses
  Classes, SysUtils, LResources, Forms, Controls, Graphics, Dialogs, DBCtrls,
  DBGrids, ExtCtrls, StdCtrls;

type

  { TForm1 }

  TForm1 = class(TForm)
    Image1: TImage;
    Panel1: TPanel;
    ScrollBox1: TScrollBox;
    ScrollBox2: TScrollBox;
    StaticText1: TStaticText;
    procedure DBNavigator1Click(Sender: TObject; Button: TDBNavButtonType);
    procedure FormCreate(Sender: TObject);
    procedure FormShow(Sender: TObject);
    procedure Image1Click(Sender: TObject);
    procedure StaticText1Click(Sender: TObject);
  private

  public

  end;

var
  Form1: TForm1;

implementation
uses FRMUNIT;

{ TForm1 }

procedure TForm1. FormShow (Sender: TObject);
   var frm:TFramel; i:integer;
begin
  for i:=1 to 3 do begin
    frm:=TFramel.Create(Form1);
    frm.name:=frm.name+inttostr(i);
    frm.Parent:=sb;
    frm.top:=(i-1)*(frm.Height+1);
    frm.align:=alTop;
  end;

end;

procedure TForm1.FormCreate(Sender: TObject);
begin

end;

procedure TForm1.FormShow(Sender: TObject);
begin

end;

procedure TForm1.Image1Click(Sender: TObject);
begin

end;

procedure TForm1.StaticText1Click(Sender: TObject);
begin

end;

initialization
  {$I main.lrs}

end.

