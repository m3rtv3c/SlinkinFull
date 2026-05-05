unit Unit1;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, LResources, Forms, Controls, Graphics, Dialogs, StdCtrls,
  Menus, ExtCtrls;

type

  { TForm1 }

  TForm1 = class(TForm)
    bDemoPlus: TButton;
    bDemoMinus: TButton;
    eHeight: TEdit;
    eWidth: TEdit;
    Label1: TLabel;
    labelH: TLabel;
    Panel1: TPanel;
    timeSize: TTimer;
    procedure bDemoMinusClick(Sender: TObject);
    procedure bDemoPlusClick(Sender: TObject);
    procedure eHeightEditingDone(Sender: TObject);
    procedure eWidthEditingDone(Sender: TObject);
    procedure FormChangeBounds(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure FormResize(Sender: TObject);
    procedure timeSizeTimer(Sender: TObject);
  private
    FMinW, FMaxW: Integer;
    FMinH, FMaxH: Integer;
    FStep: Integer;
    FMode: Integer;
    procedure CenterForm;
    procedure CenterPanel;
    procedure UpdateEdits;
    procedure LockControls(Alock:Boolean);
  public

  end;

var
  Form1: TForm1;

implementation

{ TForm1 }


procedure TForm1.FormCreate(Sender: TObject);
begin
  FMinW := 500;
  FMaxW := 1000;
  FMinH := 100;
  FMaxH := 500;

  Constraints.MinWidth := FMinW;
  Constraints.MaxWidth := FMaxW;
  Constraints.MinHeight := FMinH;
  Constraints.MaxHeight := FMaxH;
  FStep := 1;

  UpdateEdits;
  CenterForm;
  CenterPanel;
end;

procedure TForm1.CenterPanel;
begin
  Panel1.Left := (ClientWidth - Panel1.Width) div 2;
  Panel1.Top := (ClientHeight - Panel1.Height) div 2;
end;


procedure TForm1.CenterForm;
begin
  Left := (Screen.Width - Width) div 2;
  Top := (Screen.Height - Height) div 2;
end;

procedure TForm1.UpdateEdits;
begin
  eWidth.Text := IntToStr(Width);
  eHeight.Text := IntToStr(Height);
end;

procedure TForm1.LockControls(ALock: Boolean);
begin
  eWidth.Enabled := not ALock;
  eHeight.Enabled := not ALock;
  bDemoPlus.Enabled := not ALock;
  bDemoMinus.Enabled := not ALock;
end;




procedure TForm1.bDemoPlusClick(Sender: TObject);
begin
  FMode := 1;
  LockControls(True);
  timeSize.Enabled := True;
end;



procedure TForm1.eHeightEditingDone(Sender: TObject);
var
  v: Integer;
begin
  if TryStrToInt(eHeight.Text, v) then
  begin
    if v < FMinH then v := FMinH;
    if v > FMaxH then v := FMaxH;
    Height := v;
  end
  else
    eHeight.Text := IntToStr(Height);
end;



procedure TForm1.eWidthEditingDone(Sender: TObject);
var
  v: Integer;
begin
  if TryStrToInt(eWidth.Text, v) then
  begin
    if v < FMinW then v := FMinW;
    if v > FMaxW then v := FMaxW;
    Width := v;
  end
  else
    eWidth.Text := IntToStr(Width);
end;

procedure TForm1.FormChangeBounds(Sender: TObject);
begin
     CenterForm;
     UpdateEdits;
   CenterPanel;
end;

procedure TForm1.bDemoMinusClick(Sender: TObject);
begin
  FMode := -1;
  LockControls(True);
  timeSize.Enabled := True;
end;


procedure TForm1.FormResize(Sender: TObject);
begin
   UpdateEdits;
   CenterPanel;

end;



procedure TForm1.timeSizeTimer(Sender: TObject);
begin
  if FMode = 1 then
  begin
    if (Width < FMaxW) then
      Width := Width + FStep;
    if (Height < FMaxH) then
      Height := Height + FStep;

    if (Width = FMaxW) and (Height = FMaxH) then
    begin
      timeSize.Enabled := False;
      LockControls(False);
    end;
  end
  else if FMode = -1 then
  begin
    if (Width > FMinW) then
      Width := Width - FStep;
    if (Height > FMinH) then
      Height := Height - FStep;

    if (Width = FMinW) and (Height = FMinH) then
    begin
      timeSize.Enabled := False;
      LockControls(False);
    end;
  end;
end;







initialization
  {$I unit1.lrs}

end.

