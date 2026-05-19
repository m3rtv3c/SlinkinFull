unit Unit1;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, LResources, Forms, Controls, Graphics, Dialogs, ExtCtrls,
  Buttons, Menus, ComCtrls;

type
  TShapeType = (stLine, stRectangle, stCircle);
  TShape = record
    ShapeType: TShapeType;
    StartPt: TPoint;
    EndPt: TPoint;
  end;

  { TForm1 }
  TForm1 = class(TForm)
    FlowPanel1: TFlowPanel;
    MainMenu1: TMainMenu;
    MIFile: TMenuItem;
    MIEdit: TMenuItem;
    MIHelp: TMenuItem;
    MILoad: TMenuItem;
    MIUpload: TMenuItem;
    PaintBox1: TPaintBox;
    SBLine: TSpeedButton;
    SBCancel: TSpeedButton;
    SBRectangle: TSpeedButton;
    SBCircle: TSpeedButton;
    SBMove: TSpeedButton;
    SBStatus: TStatusBar;
    procedure FormCreate(Sender: TObject);
    procedure PaintBox1MouseDown(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure PaintBox1MouseMove(Sender: TObject; Shift: TShiftState; X,
      Y: Integer);
    procedure PaintBox1MouseUp(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure PaintBox1Paint(Sender: TObject);
    procedure SBCircleClick(Sender: TObject);
    procedure SBLineClick(Sender: TObject);
    procedure SBMoveClick(Sender: TObject);
    procedure SBRectangleClick(Sender: TObject);
  private
    Shapes: array of TShape;
    CurrentShape: TShape;
    IsDrawing: Boolean;
  public
  end;

var
  Form1: TForm1;

implementation

{ TForm1 }

procedure TForm1.SBLineClick(Sender: TObject);
begin
  SBStatus.Panels[0].Text := 'Отрисовка линии';
end;

procedure TForm1.SBRectangleClick(Sender: TObject);
begin
  SBStatus.Panels[0].Text := 'Отрисовка прямоугольника';
end;

procedure TForm1.SBCircleClick(Sender: TObject);
begin
  SBStatus.Panels[0].Text := 'Отрисовка окружности';
end;

procedure TForm1.SBMoveClick(Sender: TObject);
begin
  SBStatus.Panels[0].Text := 'Режим перемещения';
end;

procedure TForm1.FormCreate(Sender: TObject);
begin
  SBStatus.Panels[0].Text := 'Режим просмотра';
  IsDrawing := False;
  SetLength(Shapes, 0);
end;

procedure TForm1.PaintBox1MouseDown(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
begin
  if Button <> mbLeft then Exit;

  if SBLine.Down then CurrentShape.ShapeType := stLine
  else if SBRectangle.Down then CurrentShape.ShapeType := stRectangle
  else if SBCircle.Down then CurrentShape.ShapeType := stCircle
  else Exit;

  IsDrawing := True;
  CurrentShape.StartPt := Point(X, Y);
  CurrentShape.EndPt := Point(X, Y);
end;

procedure TForm1.PaintBox1MouseMove(Sender: TObject; Shift: TShiftState; X,
  Y: Integer);
begin
  if IsDrawing then
  begin
    CurrentShape.EndPt := Point(X, Y);
    PaintBox1.Invalidate;
  end;
end;

procedure TForm1.PaintBox1MouseUp(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
begin
  if IsDrawing then
  begin
    CurrentShape.EndPt := Point(X, Y);

    SetLength(Shapes, Length(Shapes) + 1);
    Shapes[High(Shapes)] := CurrentShape;

    IsDrawing := False;
    PaintBox1.Invalidate;
  end;
end;

procedure TForm1.PaintBox1Paint(Sender: TObject);
var
  i: Integer;
begin
  with PaintBox1.Canvas do
  begin
    Brush.Color := clGray;
    FillRect(0, 0, PaintBox1.Width, PaintBox1.Height);

    Pen.Color := clBlack;
    Pen.Width := 1;

    for i := 0 to High(Shapes) do
    begin
      case Shapes[i].ShapeType of
        stLine:
          Line(Shapes[i].StartPt, Shapes[i].EndPt);
        stRectangle:
          Rectangle(Shapes[i].StartPt.X, Shapes[i].StartPt.Y,
                    Shapes[i].EndPt.X, Shapes[i].EndPt.Y);
        stCircle:
          Ellipse(Shapes[i].StartPt.X, Shapes[i].StartPt.Y,
                  Shapes[i].EndPt.X, Shapes[i].EndPt.Y);
      end;
    end;

    if IsDrawing then
    begin
      case CurrentShape.ShapeType of
        stLine:
          Line(CurrentShape.StartPt, CurrentShape.EndPt);
        stRectangle:
          Rectangle(CurrentShape.StartPt.X, CurrentShape.StartPt.Y,
                    CurrentShape.EndPt.X, CurrentShape.EndPt.Y);
        stCircle:
          Ellipse(CurrentShape.StartPt.X, CurrentShape.StartPt.Y,
                  CurrentShape.EndPt.X, CurrentShape.EndPt.Y);
      end;
    end;
  end;
end;

initialization
  {$I unit1.lrs}

end.
