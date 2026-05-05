program nevGeany;

{$mode objfpc}{$H+}

uses
  Classes, SysUtils, LResources, Forms, Controls, Graphics, Dialogs, StdCtrls,
  Menus, ExtCtrls,Interfaces;

type
  TForm1 = class(TForm)
  private
    FMinW, FMaxW: Integer;
    FMinH, FMaxH: Integer;
    FStep: Integer;
    FMode: Integer;
    Panel1: TPanel;
    bDemoPlus, bDemoMinus: TButton;
    eHeight, eWidth: TEdit;
    Label1, LabelH: TLabel;
    timeSize: TTimer;
    procedure CenterForm;
    procedure CenterPanel;
    procedure UpdateEdits;
    procedure FormChangeBounds(Sender: TObject);
    procedure LockControls(ALock: Boolean);
    procedure bDemoPlusClick(Sender: TObject);
    procedure bDemoMinusClick(Sender: TObject);
    procedure eHeightEditingDone(Sender: TObject);
    procedure eWidthEditingDone(Sender: TObject);
    procedure timeSizeTimer(Sender: TObject);
  public
    constructor Create(TheOwner: TComponent); override;
  end;

constructor TForm1.Create(TheOwner: TComponent);
begin
  inherited Create(TheOwner);
  Width := 500;
  Height := 100;

  FMinW := 500;
  FMaxW := 1000;
  FMinH := 100;
  FMaxH := 500;
  FStep := 1;

  Constraints.MinWidth := FMinW;
  Constraints.MaxWidth := FMaxW;
  Constraints.MinHeight := FMinH;
  Constraints.MaxHeight := FMaxH;

  // Панель
  Panel1 := TPanel.Create(Self);
  Panel1.Parent := Self;
  Panel1.Width := 500;
  Panel1.Height := 100;
  Panel1.BevelOuter := bvNone;

  // Метки
  LabelH := TLabel.Create(Self);
  LabelH.Parent := Panel1;
  LabelH.Caption := 'Высота:';
  LabelH.Top := 16;
  LabelH.Left := 40;

  Label1 := TLabel.Create(Self);
  Label1.Parent := Panel1;
  Label1.Caption := 'Ширина:';
  Label1.Top := 16;
  Label1.Left := 264;

  // Текстбоксы
  eHeight := TEdit.Create(Self);
  eHeight.Parent := Panel1;
  eHeight.Top := 32;
  eHeight.Left := 40;
  eHeight.Width := 186;
  eHeight.Height := 28;
  eHeight.Text := IntToStr(Height);
  eHeight.OnEditingDone := @eHeightEditingDone;

  eWidth := TEdit.Create(Self);
  eWidth.Parent := Panel1;
  eWidth.Top := 32;
  eWidth.Left := 264;
  eWidth.Width := 186;
  eWidth.Height := 28;
  eWidth.Text := IntToStr(Width);
  eWidth.OnEditingDone := @eWidthEditingDone;

  // Кнопки
  bDemoPlus := TButton.Create(Self);
  bDemoPlus.Parent := Panel1;
  bDemoPlus.Caption := 'Демо+';
  bDemoPlus.Top := 64;
  bDemoPlus.Left := 40;
  bDemoPlus.Width := 186;
  bDemoPlus.Height := 27;
  bDemoPlus.OnClick := @bDemoPlusClick;

  bDemoMinus := TButton.Create(Self);
  bDemoMinus.Parent := Panel1;
  bDemoMinus.Caption := 'Демо-';
  bDemoMinus.Top := 64;
  bDemoMinus.Left := 264;
  bDemoMinus.Width := 186;
  bDemoMinus.Height := 27;
  bDemoMinus.OnClick := @bDemoMinusClick;

  // Таймер
  timeSize := TTimer.Create(Self);
  timeSize.Interval := 20;
  timeSize.OnTimer := @timeSizeTimer;
  timeSize.Enabled := False;

  Icon.LoadFromFile('/home/student/Документы/330/Программирование/3 kurs/lab2/nevalyashka.ico');
  OnChangeBounds := @FormChangeBounds;
  CenterForm;
  CenterPanel;
end;

procedure TForm1.CenterForm;
begin
  Left := (Screen.Width - Width) div 2;
  Top := (Screen.Height - Height) div 2;
end;

procedure TForm1.CenterPanel;
begin
  Panel1.Left := (ClientWidth - Panel1.Width) div 2;
  Panel1.Top := (ClientHeight - Panel1.Height) div 2;
end;

procedure TForm1.FormChangeBounds(Sender: TObject);
begin
  UpdateEdits;
  CenterPanel;
  CenterForm;
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

procedure TForm1.bDemoMinusClick(Sender: TObject);
begin
  FMode := -1;
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

procedure TForm1.timeSizeTimer(Sender: TObject);
begin
  if FMode = 1 then
  begin
    if Width < FMaxW then Width := Width + FStep;
    if Height < FMaxH then Height := Height + FStep;
    if (Width = FMaxW) and (Height = FMaxH) then
    begin
      timeSize.Enabled := False;
      LockControls(False);
    end;
  end
  else if FMode = -1 then
  begin
    if Width > FMinW then Width := Width - FStep;
    if Height > FMinH then Height := Height - FStep;
    if (Width = FMinW) and (Height = FMinH) then
    begin
      timeSize.Enabled := False;
      LockControls(False);
    end;
  end;
end;

var
  Form1: TForm1;

begin
  Application.Initialize;
  Application.CreateForm(TForm1, Form1);
  Application.Run;
end.
