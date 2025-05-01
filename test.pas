// -----------------------------------------------------------------------------
// Copyright(c) 2025 Jens Kallup
// all rights reserved.
//
// ATTENTION:
// This file was automaticaly created from: 
// All changes will be lost on next convert run !
// -----------------------------------------------------------------------------
{$mode delphi}
UNIT test;

INTERFACE

USES
  Interfaces, SysUtils, Classes, Forms, Dialogs;

TYPE
  TParamScope = (
    PS_UNKNOWN   = 0,
    PS_LOCAL     = 1,
    PS_GLOBAL    = 2,
    PS_PRIVATE   = 3,
    PS_PROTECTED = 4
  );
  TParamType = (
    PT_UNKNOWN = 0,
    PT_NUMERIC = 1,
    PT_STRING  = 2,
    PT_OBJECT  = 3  );

TYPE
  TDBParameterClass = CLASS(TObject)
  public
    pName  : String      ;
    pScope : TParamScope ;
    pType  : TParamType  ;
    pSize  : Integer     ;
    pData  : PChar       ;
  END;

TYPE
  TDBClass_06200f92_7181_4db7_8e79_9d5cee2d6891 = CLASS(TForm)
  PRIVATE
    FParameter: ARRAY [0..1] OF TDBParameterClass;
  PUBLIC
    PROCEDURE Init;
    PROCEDURE DeInit;
  END;

TYPE
  TDBClass_57b5bbea_4f3e_40f3_9c06_59204ddaa18f = CLASS(TForm)
  PRIVATE
    FParameter: ARRAY [0..1] OF TDBParameterClass;
  PUBLIC
    PROCEDURE Init;
    PROCEDURE DeInit;
  END;

TYPE
  TDBClass_6e546657_cf75_402f_a9a7_2a2df502d1fe = CLASS(TForm)
  PRIVATE
    FParameter: ARRAY [0..1] OF TDBParameterClass;
  PUBLIC
    PROCEDURE Init;
    PROCEDURE DeInit;
  END;

TYPE
  TDBClass_b100651b_44eb_475c_a0fc_c6700b95d0ce = CLASS(TForm)
  PRIVATE
    FParameter: ARRAY [0..1] OF TDBParameterClass;
  PUBLIC
    PROCEDURE Init;
    PROCEDURE DeInit;
  END;

TYPE
  TDBClass_b8f06298_ed28_4a53_80df_f6d31d626022 = CLASS(TForm)
  PRIVATE
    FParameter: ARRAY [0..1] OF TDBParameterClass;
  PUBLIC
    PROCEDURE Init;
    PROCEDURE DeInit;
  END;

TYPE
  TDBClass_df4d5daf_d635_4550_97e6_f7b474cb3f7a = CLASS(TForm)
  PRIVATE
    FParameter: ARRAY [0..1] OF TDBParameterClass;
  PUBLIC
    PROCEDURE Init;
    PROCEDURE DeInit;
  END;

TYPE
  TDBClass_e464d03c_8324_4b1a_a01a_f4ffeba5388c = CLASS(TForm)
  PRIVATE
    FParameter: ARRAY [0..1] OF TDBParameterClass;
  PUBLIC
    PROCEDURE Init;
    PROCEDURE DeInit;
  END;

TYPE
  TDBClass_f74fe285_c96d_4948_b31f_e35524b55630 = CLASS(TForm)
  PRIVATE
    FParameter: ARRAY [0..1] OF TDBParameterClass;
  PUBLIC
    PROCEDURE Init;
    PROCEDURE DeInit;
  END;


IMPLEMENTATION

PROCEDURE TDBClass_06200f92_7181_4db7_8e79_9d5cee2d6891.Init;
BEGIN
  FParameter[0] := TDBParameterClass.Create;
  FParameter[0].pName  := 'bModal';
  FParameter[0].pScope := TParamScope(PS_GLOBAL);
  FParameter[0].pType  := TParamType(PT_UNKNOWN);
END;

PROCEDURE TDBClass_06200f92_7181_4db7_8e79_9d5cee2d6891.DeInit;
VAR
  IDX : Integer;
BEGIN
  FOR IDX := HIGH(FParameter) DOWNTO LOW(FParameter) DO
  BEGIN
    FParameter[IDX].Free;
    FParameter[IDX] := NIL;
  END;
END;

PROCEDURE TDBClass_57b5bbea_4f3e_40f3_9c06_59204ddaa18f.Init;
BEGIN
  FParameter[0] := TDBParameterClass.Create;
  FParameter[0].pName  := 'bModal';
  FParameter[0].pScope := TParamScope(PS_GLOBAL);
  FParameter[0].pType  := TParamType(PT_UNKNOWN);
END;

PROCEDURE TDBClass_57b5bbea_4f3e_40f3_9c06_59204ddaa18f.DeInit;
VAR
  IDX : Integer;
BEGIN
  FOR IDX := HIGH(FParameter) DOWNTO LOW(FParameter) DO
  BEGIN
    FParameter[IDX].Free;
    FParameter[IDX] := NIL;
  END;
END;

PROCEDURE TDBClass_6e546657_cf75_402f_a9a7_2a2df502d1fe.Init;
BEGIN
  FParameter[0] := TDBParameterClass.Create;
  FParameter[0].pName  := 'bModal';
  FParameter[0].pScope := TParamScope(PS_GLOBAL);
  FParameter[0].pType  := TParamType(PT_UNKNOWN);
END;

PROCEDURE TDBClass_6e546657_cf75_402f_a9a7_2a2df502d1fe.DeInit;
VAR
  IDX : Integer;
BEGIN
  FOR IDX := HIGH(FParameter) DOWNTO LOW(FParameter) DO
  BEGIN
    FParameter[IDX].Free;
    FParameter[IDX] := NIL;
  END;
END;

PROCEDURE TDBClass_b100651b_44eb_475c_a0fc_c6700b95d0ce.Init;
BEGIN
  FParameter[0] := TDBParameterClass.Create;
  FParameter[0].pName  := 'bModal';
  FParameter[0].pScope := TParamScope(PS_GLOBAL);
  FParameter[0].pType  := TParamType(PT_UNKNOWN);
END;

PROCEDURE TDBClass_b100651b_44eb_475c_a0fc_c6700b95d0ce.DeInit;
VAR
  IDX : Integer;
BEGIN
  FOR IDX := HIGH(FParameter) DOWNTO LOW(FParameter) DO
  BEGIN
    FParameter[IDX].Free;
    FParameter[IDX] := NIL;
  END;
END;

PROCEDURE TDBClass_b8f06298_ed28_4a53_80df_f6d31d626022.Init;
BEGIN
  FParameter[0] := TDBParameterClass.Create;
  FParameter[0].pName  := 'bModal';
  FParameter[0].pScope := TParamScope(PS_GLOBAL);
  FParameter[0].pType  := TParamType(PT_UNKNOWN);
END;

PROCEDURE TDBClass_b8f06298_ed28_4a53_80df_f6d31d626022.DeInit;
VAR
  IDX : Integer;
BEGIN
  FOR IDX := HIGH(FParameter) DOWNTO LOW(FParameter) DO
  BEGIN
    FParameter[IDX].Free;
    FParameter[IDX] := NIL;
  END;
END;

PROCEDURE TDBClass_df4d5daf_d635_4550_97e6_f7b474cb3f7a.Init;
BEGIN
  FParameter[0] := TDBParameterClass.Create;
  FParameter[0].pName  := 'bModal';
  FParameter[0].pScope := TParamScope(PS_GLOBAL);
  FParameter[0].pType  := TParamType(PT_UNKNOWN);
END;

PROCEDURE TDBClass_df4d5daf_d635_4550_97e6_f7b474cb3f7a.DeInit;
VAR
  IDX : Integer;
BEGIN
  FOR IDX := HIGH(FParameter) DOWNTO LOW(FParameter) DO
  BEGIN
    FParameter[IDX].Free;
    FParameter[IDX] := NIL;
  END;
END;

PROCEDURE TDBClass_e464d03c_8324_4b1a_a01a_f4ffeba5388c.Init;
BEGIN
  FParameter[0] := TDBParameterClass.Create;
  FParameter[0].pName  := 'bModal';
  FParameter[0].pScope := TParamScope(PS_GLOBAL);
  FParameter[0].pType  := TParamType(PT_UNKNOWN);
END;

PROCEDURE TDBClass_e464d03c_8324_4b1a_a01a_f4ffeba5388c.DeInit;
VAR
  IDX : Integer;
BEGIN
  FOR IDX := HIGH(FParameter) DOWNTO LOW(FParameter) DO
  BEGIN
    FParameter[IDX].Free;
    FParameter[IDX] := NIL;
  END;
END;

PROCEDURE TDBClass_f74fe285_c96d_4948_b31f_e35524b55630.Init;
BEGIN
  FParameter[0] := TDBParameterClass.Create;
  FParameter[0].pName  := 'bModal';
  FParameter[0].pScope := TParamScope(PS_GLOBAL);
  FParameter[0].pType  := TParamType(PT_UNKNOWN);
END;

PROCEDURE TDBClass_f74fe285_c96d_4948_b31f_e35524b55630.DeInit;
VAR
  IDX : Integer;
BEGIN
  FOR IDX := HIGH(FParameter) DOWNTO LOW(FParameter) DO
  BEGIN
    FParameter[IDX].Free;
    FParameter[IDX] := NIL;
  END;
END;

END.
