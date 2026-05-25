unit Unit1;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, LResources, Forms, Controls, Graphics, Dialogs, ExtCtrls,
  Buttons, Menus, ComCtrls, IpHtml, Types, Math, Unit2;

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
    MIGuide: TMenuItem;
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
    procedure MIGuideClick(Sender: TObject);
    procedure PaintBox1MouseDown(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure PaintBox1MouseMove(Sender: TObject; Shift: TShiftState; X,
      Y: Integer);
    procedure PaintBox1MouseUp(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure PaintBox1Paint(Sender: TObject);
    procedure SBCancelClick(Sender: TObject);
    procedure SBCircleClick(Sender: TObject);
    procedure SBLineClick(Sender: TObject);
    procedure SBMoveClick(Sender: TObject);
    procedure SBRectangleClick(Sender: TObject);
  private
    Shapes: array of TShape;
    CurrentShape: TShape;
    IsDrawing: Boolean;
    IsMoving: Boolean;
    SelectedIdx: Integer;
    DragOffset: TPoint;
    HoveredIdx: Integer;

    procedure DrawSingleShape(ACanvas: TCanvas; const AShape: TShape);
    function GetShapeBoundingRect(const AShape: TShape): TRect;
    function FindShapeAt(P: TPoint): Integer;
    procedure DeleteShape (Idx:Integer);
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
  SBStatus.Panels[0].Text := 'Режим готовности к перемещению';
end;

procedure TForm1.SBCancelClick(Sender: TObject);
begin
    SBStatus.Panels[0].Text := 'Режим удаления';
end;

procedure TForm1.FormCreate(Sender: TObject);
begin
  SBStatus.Panels[0].Text := 'Режим просмотра';
  IsDrawing := False;
  IsMoving := False;
  SelectedIdx := -1;
  HoveredIdx := -1;
  SetLength(Shapes, 0);
  DoubleBuffered := True;
end;

procedure TForm1.MIGuideClick(Sender: TObject);
begin
  Form2.Position := poScreenCenter;
  Form2.Show;
end;

function TForm1.GetShapeBoundingRect(const AShape: TShape): TRect;
begin
  Result.Left := Min(AShape.StartPt.X, AShape.EndPt.X);
  Result.Top := Min(AShape.StartPt.Y, AShape.EndPt.Y);
  Result.Right := Max(AShape.StartPt.X, AShape.EndPt.X);
  Result.Bottom := Max(AShape.StartPt.Y, AShape.EndPt.Y);

  InflateRect(Result, 3, 3);
end;

function TForm1.FindShapeAt(P: TPoint): Integer;
var
  i: Integer;
  R: TRect;
begin
  Result := -1;
  for i := High(Shapes) downto 0 do
  begin
    R := GetShapeBoundingRect(Shapes[i]);
    if PtInRect(R, P) then
    begin
      Result := i;
      Exit;
    end;
  end;
end;

procedure TForm1.PaintBox1MouseDown(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
var
  Pt: TPoint;
  DelIdx: Integer;
begin
  if Button <> mbLeft then Exit;
  Pt := Point(X, Y);

  if SBCancel.Down then
  begin
    DelIdx := FindShapeAt(Pt); // Ищем фигуру под кликом
    if DelIdx <> -1 then
    begin
      DeleteShape(DelIdx); // Удаляем её
    end;
    Exit;
  end;

  //Перемещение
  if SBMove.Down then
  begin
    SBStatus.Panels[0].Text := 'Режим перемещения';
    SelectedIdx := FindShapeAt(Pt);
    if SelectedIdx <> -1 then
    begin
      IsMoving := True;
      DragOffset.X := X - Shapes[SelectedIdx].StartPt.X;  //Рассчитываем смещение курсора относително самой фигуры
      DragOffset.Y := Y - Shapes[SelectedIdx].StartPt.Y;
      PaintBox1.Invalidate;
    end;
    Exit;
  end;

  //Рисование
  if SBLine.Down then CurrentShape.ShapeType := stLine
  else if SBRectangle.Down then CurrentShape.ShapeType := stRectangle
  else if SBCircle.Down then CurrentShape.ShapeType := stCircle
  else Exit;

  IsDrawing := True;
  CurrentShape.StartPt := Pt;
  CurrentShape.EndPt := Pt;
end;

procedure TForm1.PaintBox1MouseMove(Sender: TObject; Shift: TShiftState; X,
  Y: Integer);
var
  W, H: Integer;
  OldHovered: Integer;
begin
  if IsMoving and (SelectedIdx <> -1) then
  begin
    W := Shapes[SelectedIdx].EndPt.X - Shapes[SelectedIdx].StartPt.X; //Сохраняем исходный размер
    H := Shapes[SelectedIdx].EndPt.Y - Shapes[SelectedIdx].StartPt.Y;

    //Меняем начальную точку
    Shapes[SelectedIdx].StartPt := Point(X - DragOffset.X, Y - DragOffset.Y);
    //Конечная точка - новая начальная и переносим размеры
    Shapes[SelectedIdx].EndPt := Point(Shapes[SelectedIdx].StartPt.X + W, Shapes[SelectedIdx].StartPt.Y + H);

    PaintBox1.Invalidate;
    Exit;
  end;

  if IsDrawing then
  begin
    CurrentShape.EndPt := Point(X, Y);
    PaintBox1.Invalidate;
    Exit;
  end;

  if SBMove.Down or SBCancel.Down then
  begin
    OldHovered := HoveredIdx;
    HoveredIdx := FindShapeAt(Point(X, Y));

    if HoveredIdx <> OldHovered then
      PaintBox1.Invalidate;
  end
  else
  begin
    if HoveredIdx <> -1 then
    begin
      HoveredIdx := -1;
      PaintBox1.Invalidate;
    end;
  end;
end;

procedure TForm1.PaintBox1MouseUp(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
begin
  if Button <> mbLeft then Exit;

  if IsMoving then
  begin
    IsMoving := False;
    SelectedIdx := -1;  //Сбрасываем сбор
    HoveredIdx := FindShapeAt(Point(X, Y)); //Осталась ли фигура под курсором
    PaintBox1.Invalidate;
    SBStatus.Panels[0].Text := 'Режим готовности к перемещению';
    Exit;
  end;

  if IsDrawing then
  begin
    CurrentShape.EndPt := Point(X, Y);
    SetLength(Shapes, Length(Shapes) + 1);
    Shapes[High(Shapes)] := CurrentShape;
    IsDrawing := False;
    PaintBox1.Invalidate;
  end;
end;

//Отрисовка фигур
procedure TForm1.DrawSingleShape(ACanvas: TCanvas; const AShape: TShape);
begin
  case AShape.ShapeType of
    stLine:
      ACanvas.Line(AShape.StartPt, AShape.EndPt);
    stRectangle:
      ACanvas.Rectangle(AShape.StartPt.X, AShape.StartPt.Y,
                        AShape.EndPt.X, AShape.EndPt.Y);
    stCircle:
      ACanvas.Ellipse(AShape.StartPt.X, AShape.StartPt.Y,
                      AShape.EndPt.X, AShape.EndPt.Y);
  end;
end;

procedure TForm1.PaintBox1Paint(Sender: TObject);
var
  i: Integer;
begin
  with PaintBox1.Canvas do
  begin
    Brush.Color := clDefault;
    Brush.Style := bsSolid;
    FillRect(0, 0, PaintBox1.Width, PaintBox1.Height);

    Pen.Color := clBlack;
    Pen.Style := psSolid;

    // Рисуем все сохраненные фигуры
    for i := 0 to High(Shapes) do
    begin
      Brush.Color := clWhite;
      Brush.Style := bsSolid;

      if IsMoving and (i = SelectedIdx) then
      begin
        Pen.Width := 3;
        DrawSingleShape(PaintBox1.Canvas, Shapes[i]);
      end
      else if (not IsMoving) and (i = HoveredIdx) then
      begin
        Pen.Width := 2;
        DrawSingleShape(PaintBox1.Canvas, Shapes[i]);
      end
      else
      begin
        Pen.Width := 1;
        DrawSingleShape(PaintBox1.Canvas, Shapes[i]);
      end;
    end;

    if IsDrawing then
    begin
      Brush.Color := clWhite;
      Brush.Style := bsSolid;
      Pen.Width := 1;
      Pen.Style := psSolid;
      DrawSingleShape(PaintBox1.Canvas, CurrentShape);
    end;
  end;
end;


procedure TForm1.DeleteShape(Idx: Integer);
var
  i: Integer;
begin
  if (Idx < 0) or (Idx > High(Shapes)) then Exit;

  for i := Idx to High(Shapes) - 1 do
  begin
    Shapes[i] := Shapes[i + 1];
  end;

  SetLength(Shapes, Length(Shapes) - 1);
  HoveredIdx := -1;
  SelectedIdx := -1;
  PaintBox1.Invalidate;
end;



initialization
  {$I unit1.lrs}

end.

