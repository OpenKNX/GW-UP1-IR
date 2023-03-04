//--------------------Allgemein---------------------------
#define MAIN_OpenKnxId 0xA4
#define MAIN_ApplicationNumber 0x01
#define MAIN_ApplicationVersion 0x07
#define MAIN_OrderNumber "TWB-IR.01" //may not work with multiple devices on same hardware or app on different hardware


//---------------------Modules----------------------------
//-----Module: ir
// Offset: 0, Size: 2 Bit, Text: Kanal
#define PARAM_ir_inOutType		0x0000
#define PARAM_ir_inOutType_Mask	0x00C0
#define PARAM_ir_inOutType_Shift	6
// UnionOffset: 1, ParaOffset: 3, Size: 3 Bit, Text: Funktion
#define PARAM_ir_inType		0x0004
#define PARAM_ir_inType_Mask	0x00E0
#define PARAM_ir_inType_Shift	5
// UnionOffset: 1, ParaOffset: 0, Size: 2 Bit, Text: Unterfunktion
#define PARAM_ir_inSwitch		0x0001
#define PARAM_ir_inSwitch_Mask	0x00C0
#define PARAM_ir_inSwitch_Shift	6
// UnionOffset: 1, ParaOffset: 1, Size: 8 Bit (1 Byte), Text: Wert
#define PARAM_ir_inValue		0x0002
// UnionOffset: 1, ParaOffset: 1, Size: 7 Bit, Text: Szenennummer
#define PARAM_ir_inScene		0x0002
#define PARAM_ir_inScene_Mask	0x00FE
#define PARAM_ir_inScene_Shift	1
// UnionOffset: 1, ParaOffset: 0, Size: 1 Bit, Text: Richtung
#define PARAM_ir_inDimmDirection		0x0001
#define PARAM_ir_inDimmDirection_Mask	0x0080
#define PARAM_ir_inDimmDirection_Shift	7
// UnionOffset: 1, ParaOffset: 0, BitOffset: 1, Size: 1 Bit, Text: Ein/Aus bei einmaligem Tastendruck senden
#define PARAM_ir_inDimmSwitch		0x0001
#define PARAM_ir_inDimmSwitch_Mask	0x0040
#define PARAM_ir_inDimmSwitch_Shift	6
// UnionOffset: 1, ParaOffset: 0, Size: 24 Bit (3 Byte), Text: Farbe
#define PARAM_ir_inColor		0x0001
// UnionOffset: 1, ParaOffset: 3, BitOffset: 2, Size: 1 Bit, Text: Ausgabe als
#define PARAM_ir_inColorType		0x0004
#define PARAM_ir_inColorType_Mask	0x0020
#define PARAM_ir_inColorType_Shift	5
// UnionOffset: 1, ParaOffset: 3, Size: 2 Bit, Text: Funktion
#define PARAM_ir_outType		0x0004
#define PARAM_ir_outType_Mask	0x00C0
#define PARAM_ir_outType_Shift	6
// UnionOffset: 1, ParaOffset: 1, Size: 8 Bit (1 Byte), Text: Wert
#define PARAM_ir_outValue		0x0002
// UnionOffset: 1, ParaOffset: 1, Size: 7 Bit, Text: Szenennummer
#define PARAM_ir_outScene		0x0002
#define PARAM_ir_outScene_Mask	0x00FE
#define PARAM_ir_outScene_Shift	1
// UnionOffset: 1, ParaOffset: 0, Size: 2 Bit, Text: Senden bei
#define PARAM_ir_outSwitch		0x0001
#define PARAM_ir_outSwitch_Mask	0x00C0
#define PARAM_ir_outSwitch_Shift	6
#define COMOBJ_ir_co_n1 	1	//!< Number: 1, Text: IR{{argChan}} <- {{0:}}, Function: Farbe
#define COMOBJ_ir_co_n2 	2	//!< Number: 2, Text: IR{{argChan}} <- {{0:}}, Function: Status


//-----Module specific starts
const long mod_ir_para[] = { 0,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90 };
const long mod_ir_coms[] = { 0,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,34,36 };
