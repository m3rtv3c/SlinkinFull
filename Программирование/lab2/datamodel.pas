unit datamodel;

{$mode ObjFPC}{$H+}

interface

uses
  Classes, SysUtils, SQLDB, DB, PQConnection, LResources;

type

  { TDataModule1 }

  TDataModule1 = class(TDataModule)
    DataSource1: TDataSource;
    SQLC: TSQLConnector;
    SQLQ: TSQLQuery;
    SQLT: TSQLTransaction;
    procedure DataModuleDestroy(Sender: TObject);
  private

  public

  end;

var
  DataModule1: TDataModule1;

implementation

{ TDataModule1 }

procedure TDataModule1.DataModuleDestroy(Sender: TObject);
begin

end;

initialization
  {$I datemodel.lrs}

end.

