unit Unit2;

{$mode ObjFPC}

interface

uses
  Classes, SysUtils, LResources, Forms, Controls, Graphics, Dialogs, IpHtml;

type

  { TForm2 }

  TForm2 = class(TForm)
    IpHtmlPanel1: TIpHtmlPanel;
    procedure FormCreate(Sender: TObject);
  private
    procedure LoadHelpContent;
  public

  end;

var
  Form2: TForm2;

implementation


procedure TForm2.LoadHelpContent;
const
  HtmlPath = 'help.html';
var
  HtmlList: TStringList;
begin
  if FileExists(HtmlPath) then
  begin
    HtmlList := TStringList.Create;
    try
      HtmlList.LoadFromFile(HtmlPath);
      IpHtmlPanel1.SetHtmlFromStr(HtmlList.Text);
      IpHtmlPanel1.Visible := True;
    finally
      HtmlList.Free;
    end;
  end
  else
    ShowMessage('Файл не найден: ' + HtmlPath);
end;

procedure TForm2.FormCreate(Sender: TObject);
begin
     LoadHelpContent;
end;

initialization
  {$I unit2.lrs}

end.

