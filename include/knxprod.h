#pragma once

//--------------------Allgemein---------------------------
#define MAIN_OpenKnxId 0xA4
#define MAIN_ApplicationNumber 0x00
#define MAIN_ApplicationVersion 0x09
#define MAIN_OrderNumber "TWB-IR.01" //may not work with multiple devices on same hardware or app on different hardware


//---------------------Modules----------------------------

//-----Module specific starts
const long mod_ir_para[] = { 0,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75 };
const long mod_ir_coms[] = { 0,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30 };

//-----Module: ir
// Offset: 0, Size: 2 Bit, Text: Kanal
#define PARAM_ir_inOutType		0x0000
#define PARAM_ir_inOutType_Shift	6
#define PARAM_ir_inOutType_Mask	0x0003
#define GETPARAM_ir_inOutType(X) ((uint)((knx.paramByte((mod_ir_para[X] + 0)) >> PARAM_ir_inOutType_Shift) & PARAM_ir_inOutType_Mask))
// UnionOffset: 1, ParaOffset: 3, Size: 3 Bit, Text: Funktion
#define PARAM_ir_inType		0x0004
#define PARAM_ir_inType_Shift	5
#define PARAM_ir_inType_Mask	0x0007
#define GETPARAM_ir_inType(X) ((uint)((knx.paramByte((mod_ir_para[X] + 4)) >> PARAM_ir_inType_Shift) & PARAM_ir_inType_Mask))
// UnionOffset: 1, ParaOffset: 0, Size: 2 Bit, Text: Unterfunktion
#define PARAM_ir_inSwitch		0x0001
#define PARAM_ir_inSwitch_Shift	6
#define PARAM_ir_inSwitch_Mask	0x0003
#define GETPARAM_ir_inSwitch(X) ((uint)((knx.paramByte((mod_ir_para[X] + 1)) >> PARAM_ir_inSwitch_Shift) & PARAM_ir_inSwitch_Mask))
// UnionOffset: 1, ParaOffset: 1, Size: 8 Bit (1 Byte), Text: Wert
#define PARAM_ir_inValue		0x0002
#define GETPARAM_ir_inValue(X) ((uint)((knx.paramByte((mod_ir_para[X] + 2)))))
// UnionOffset: 1, ParaOffset: 1, Size: 6 Bit, Text: Szenennummer
#define PARAM_ir_inScene		0x0002
#define PARAM_ir_inScene_Shift	2
#define PARAM_ir_inScene_Mask	0x003F
#define GETPARAM_ir_inScene(X) ((uint)((knx.paramByte((mod_ir_para[X] + 2)) >> PARAM_ir_inScene_Shift) & PARAM_ir_inScene_Mask))
// UnionOffset: 1, ParaOffset: 0, Size: 1 Bit, Text: Richtung
#define PARAM_ir_inDimmDirection		0x0001
#define GETPARAM_ir_inDimmDirection(X) knx.paramBit((mod_ir_para[X] + 1), 0)
// UnionOffset: 1, ParaOffset: 0, BitOffset: 1, Size: 1 Bit, Text: Ein/Aus bei einmaligem Tastendruck senden
#define PARAM_ir_inDimmSwitch		0x0001
#define GETPARAM_ir_inDimmSwitch(X) knx.paramBit((mod_ir_para[X] + 1), 1)
// UnionOffset: 1, ParaOffset: 0, Size: 24 Bit (3 Byte), Text: Farbe
#define PARAM_ir_inColor		0x0001
#define GETPARAM_ir_inColor(X) knx.paramData((mod_ir_para[X] + 1))
// UnionOffset: 1, ParaOffset: 3, BitOffset: 2, Size: 1 Bit, Text: Ausgabe als
#define PARAM_ir_inColorType		0x0004
#define GETPARAM_ir_inColorType(X) knx.paramBit((mod_ir_para[X] + 4), 2)
// UnionOffset: 1, ParaOffset: 3, Size: 2 Bit, Text: Funktion
#define PARAM_ir_outType		0x0004
#define PARAM_ir_outType_Shift	6
#define PARAM_ir_outType_Mask	0x0003
#define GETPARAM_ir_outType(X) ((uint)((knx.paramByte((mod_ir_para[X] + 4)) >> PARAM_ir_outType_Shift) & PARAM_ir_outType_Mask))
// UnionOffset: 1, ParaOffset: 1, Size: 8 Bit (1 Byte), Text: Wert
#define PARAM_ir_outValue		0x0002
#define GETPARAM_ir_outValue(X) ((uint)((knx.paramByte((mod_ir_para[X] + 2)))))
// UnionOffset: 1, ParaOffset: 1, Size: 6 Bit, Text: Szenennummer
#define PARAM_ir_outScene		0x0002
#define PARAM_ir_outScene_Shift	2
#define PARAM_ir_outScene_Mask	0x003F
#define GETPARAM_ir_outScene(X) ((uint)((knx.paramByte((mod_ir_para[X] + 2)) >> PARAM_ir_outScene_Shift) & PARAM_ir_outScene_Mask))
// UnionOffset: 1, ParaOffset: 0, Size: 2 Bit, Text: Senden bei
#define PARAM_ir_outSwitch		0x0001
#define PARAM_ir_outSwitch_Shift	6
#define PARAM_ir_outSwitch_Mask	0x0003
#define GETPARAM_ir_outSwitch(X) ((uint)((knx.paramByte((mod_ir_para[X] + 1)) >> PARAM_ir_outSwitch_Shift) & PARAM_ir_outSwitch_Mask))
#define COMOBJ_ir_co_n1 	1	//!< Number: 1, Text: IR{{argChan}} <- {{0:}}, Function: Farbe
#define COMOBJ_ir_co_n2 	2	//!< Number: 2, Text: IR{{argChan}} <- {{0:}}, Function: Status

