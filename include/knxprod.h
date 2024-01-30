#pragma once

#define paramDelay(time) (uint32_t)( \
            (time & 0xC000) == 0xC000 ? (time & 0x3FFF) * 100 : \
            (time & 0xC000) == 0x0000 ? (time & 0x3FFF) * 1000 : \
            (time & 0xC000) == 0x4000 ? (time & 0x3FFF) * 60000 : \
            (time & 0xC000) == 0x8000 ? ((time & 0x3FFF) > 1000 ? 3600000 : \
                                            (time & 0x3FFF) * 3600000 ) : 0 )
#define PT_active_not active 0
#define PT_active_active 1
#define PT_ir_receive_switch 0
#define PT_ir_receive_value 1
#define PT_ir_receive_scene 2
#define PT_ir_receive_dimm 3
#define PT_ir_receive_color 4
#define PT_switch_type_in_um 0
#define PT_switch_type_in_on 1
#define PT_switch_type_in_off 2
#define PT_direction_auf 0
#define PT_direction_ab 1
#define PT_ir_send_switch 0
#define PT_ir_send_value 1
#define PT_ir_send_scene 2
#define PT_rgb_hsv_rgb 0
#define PT_rgb_hsv_hsv 1
#define PT_inout_off 0
#define PT_inout_in 1
#define PT_inout_out 2
#define PT_switch_type_out_any 0
#define PT_switch_type_out_on 1
#define PT_switch_type_out_off 2
//--------------------Allgemein---------------------------
#define MAIN_OpenKnxId 0xA4
#define MAIN_ApplicationNumber 0x00
#define MAIN_ApplicationVersion 0x00
#define MAIN_OrderNumber "GW-UP1-IR.01"
#define MAIN_ParameterSize 96
#define MAIN_MaxKoNumber 47



//---------------------Modules----------------------------

//-----Module specific starts
#define IR_ParamBlockOffset 0
#define IR_ParamBlockSize 6
#define IR_KoOffset 0
#define IR_KoBlockSize 2

//-----Module: ir
#define IR_inOutType		0x0000
#define IR_inOutType_Shift	6
#define IR_inOutType_Mask	0x0003
// Offset: 0, Size: 2 Bit, Text: Kanal
#define ParamIR_inOutTypeIndex(X) ((uint)((knx.paramByte((IR_ParamBlockOffset + IR_ParamBlockSize * X + 0)) >> IR_inOutType_Shift) & IR_inOutType_Mask))
// Offset: 0, Size: 2 Bit, Text: Kanal
#define ParamIR_inOutType ((uint)((knx.paramByte((IR_ParamBlockOffset + IR_ParamBlockSize * channelIndex() + 0)) >> IR_inOutType_Shift) & IR_inOutType_Mask))
#define IR_inType		0x0001
#define IR_inType_Shift	5
#define IR_inType_Mask	0x0007
// UnionOffset: 1, ParaOffset: 0, Size: 3 Bit, Text: Funktion
#define ParamIR_inTypeIndex(X) ((uint)((knx.paramByte((IR_ParamBlockOffset + IR_ParamBlockSize * X + 1)) >> IR_inType_Shift) & IR_inType_Mask))
// UnionOffset: 1, ParaOffset: 0, Size: 3 Bit, Text: Funktion
#define ParamIR_inType ((uint)((knx.paramByte((IR_ParamBlockOffset + IR_ParamBlockSize * channelIndex() + 1)) >> IR_inType_Shift) & IR_inType_Mask))
#define IR_inSwitch		0x0002
#define IR_inSwitch_Shift	6
#define IR_inSwitch_Mask	0x0003
// UnionOffset: 1, ParaOffset: 1, Size: 2 Bit, Text: Unterfunktion
#define ParamIR_inSwitchIndex(X) ((uint)((knx.paramByte((IR_ParamBlockOffset + IR_ParamBlockSize * X + 2)) >> IR_inSwitch_Shift) & IR_inSwitch_Mask))
// UnionOffset: 1, ParaOffset: 1, Size: 2 Bit, Text: Unterfunktion
#define ParamIR_inSwitch ((uint)((knx.paramByte((IR_ParamBlockOffset + IR_ParamBlockSize * channelIndex() + 2)) >> IR_inSwitch_Shift) & IR_inSwitch_Mask))
#define IR_inValue		0x0002
// UnionOffset: 1, ParaOffset: 1, Size: 8 Bit (1 Byte), Text: Wert
#define ParamIR_inValueIndex(X) ((uint)((knx.paramByte((IR_ParamBlockOffset + IR_ParamBlockSize * X + 2)))))
// UnionOffset: 1, ParaOffset: 1, Size: 8 Bit (1 Byte), Text: Wert
#define ParamIR_inValue ((uint)((knx.paramByte((IR_ParamBlockOffset + IR_ParamBlockSize * channelIndex() + 2)))))
#define IR_inScene		0x0002
#define IR_inScene_Shift	1
#define IR_inScene_Mask	0x007F
// UnionOffset: 1, ParaOffset: 1, Size: 7 Bit, Text: Szenennummer
#define ParamIR_inSceneIndex(X) ((uint)((knx.paramByte((IR_ParamBlockOffset + IR_ParamBlockSize * X + 2)) >> IR_inScene_Shift) & IR_inScene_Mask))
// UnionOffset: 1, ParaOffset: 1, Size: 7 Bit, Text: Szenennummer
#define ParamIR_inScene ((uint)((knx.paramByte((IR_ParamBlockOffset + IR_ParamBlockSize * channelIndex() + 2)) >> IR_inScene_Shift) & IR_inScene_Mask))
#define IR_inDimmDirection		0x0002
// UnionOffset: 1, ParaOffset: 1, Size: 1 Bit, Text: Richtung
#define ParamIR_inDimmDirectionIndex(X) knx.paramBit((IR_ParamBlockOffset + IR_ParamBlockSize * X + 2), 0)
// UnionOffset: 1, ParaOffset: 1, Size: 1 Bit, Text: Richtung
#define ParamIR_inDimmDirection knx.paramBit((IR_ParamBlockOffset + IR_ParamBlockSize * channelIndex() + 2), 0)
#define IR_inDimmSwitch		0x0002
// UnionOffset: 1, ParaOffset: 1, BitOffset: 1, Size: 1 Bit, Text: Ein/Aus bei einmaligem Tastendruck senden
#define ParamIR_inDimmSwitchIndex(X) knx.paramBit((IR_ParamBlockOffset + IR_ParamBlockSize * X + 2), 1)
// UnionOffset: 1, ParaOffset: 1, BitOffset: 1, Size: 1 Bit, Text: Ein/Aus bei einmaligem Tastendruck senden
#define ParamIR_inDimmSwitch knx.paramBit((IR_ParamBlockOffset + IR_ParamBlockSize * channelIndex() + 2), 1)
#define IR_inColor_RGB		0x0001
// UnionOffset: 1, ParaOffset: 0, BitOffset: 6, Size: 24 Bit (3 Byte), Text: Farbe
#define ParamIR_inColor_RGBIndex(X) knx.paramData((IR_ParamBlockOffset + IR_ParamBlockSize * X + 1))
// UnionOffset: 1, ParaOffset: 0, BitOffset: 6, Size: 24 Bit (3 Byte), Text: Farbe
#define ParamIR_inColor_RGB knx.paramData((IR_ParamBlockOffset + IR_ParamBlockSize * channelIndex() + 1))
#define IR_inColor_HSV		0x0001
// UnionOffset: 1, ParaOffset: 0, BitOffset: 6, Size: 24 Bit (3 Byte), Text: Farbe
#define ParamIR_inColor_HSVIndex(X) knx.paramData((IR_ParamBlockOffset + IR_ParamBlockSize * X + 1))
// UnionOffset: 1, ParaOffset: 0, BitOffset: 6, Size: 24 Bit (3 Byte), Text: Farbe
#define ParamIR_inColor_HSV knx.paramData((IR_ParamBlockOffset + IR_ParamBlockSize * channelIndex() + 1))
#define IR_inColorType		0x0001
// UnionOffset: 1, ParaOffset: 0, BitOffset: 5, Size: 1 Bit, Text: Ausgabe als
#define ParamIR_inColorTypeIndex(X) knx.paramBit((IR_ParamBlockOffset + IR_ParamBlockSize * X + 1), 5)
// UnionOffset: 1, ParaOffset: 0, BitOffset: 5, Size: 1 Bit, Text: Ausgabe als
#define ParamIR_inColorType knx.paramBit((IR_ParamBlockOffset + IR_ParamBlockSize * channelIndex() + 1), 5)
#define IR_outType		0x0001
#define IR_outType_Shift	6
#define IR_outType_Mask	0x0003
// UnionOffset: 1, ParaOffset: 0, Size: 2 Bit, Text: Funktion
#define ParamIR_outTypeIndex(X) ((uint)((knx.paramByte((IR_ParamBlockOffset + IR_ParamBlockSize * X + 1)) >> IR_outType_Shift) & IR_outType_Mask))
// UnionOffset: 1, ParaOffset: 0, Size: 2 Bit, Text: Funktion
#define ParamIR_outType ((uint)((knx.paramByte((IR_ParamBlockOffset + IR_ParamBlockSize * channelIndex() + 1)) >> IR_outType_Shift) & IR_outType_Mask))
#define IR_outValue		0x0002
// UnionOffset: 1, ParaOffset: 1, Size: 8 Bit (1 Byte), Text: Wert
#define ParamIR_outValueIndex(X) ((uint)((knx.paramByte((IR_ParamBlockOffset + IR_ParamBlockSize * X + 2)))))
// UnionOffset: 1, ParaOffset: 1, Size: 8 Bit (1 Byte), Text: Wert
#define ParamIR_outValue ((uint)((knx.paramByte((IR_ParamBlockOffset + IR_ParamBlockSize * channelIndex() + 2)))))
#define IR_outSwitch		0x0002
#define IR_outSwitch_Shift	6
#define IR_outSwitch_Mask	0x0003
// UnionOffset: 1, ParaOffset: 1, Size: 2 Bit, Text: Senden bei
#define ParamIR_outSwitchIndex(X) ((uint)((knx.paramByte((IR_ParamBlockOffset + IR_ParamBlockSize * X + 2)) >> IR_outSwitch_Shift) & IR_outSwitch_Mask))
// UnionOffset: 1, ParaOffset: 1, Size: 2 Bit, Text: Senden bei
#define ParamIR_outSwitch ((uint)((knx.paramByte((IR_ParamBlockOffset + IR_ParamBlockSize * channelIndex() + 2)) >> IR_outSwitch_Shift) & IR_outSwitch_Mask))
#define IR_outScene1		0x0001
#define IR_outScene1_Shift	3
#define IR_outScene1_Mask	0x007F
// UnionOffset: 1, ParaOffset: 0, BitOffset: 6, Size: 7 Bit, Text: Szenennummer
#define ParamIR_outScene1Index(X) ((uint)((knx.paramWord((IR_ParamBlockOffset + IR_ParamBlockSize * X + 1)) >> IR_outScene1_Shift) & IR_outScene1_Mask))
// UnionOffset: 1, ParaOffset: 0, BitOffset: 6, Size: 7 Bit, Text: Szenennummer
#define ParamIR_outScene1 ((uint)((knx.paramWord((IR_ParamBlockOffset + IR_ParamBlockSize * channelIndex() + 1)) >> IR_outScene1_Shift) & IR_outScene1_Mask))
#define IR_outScene2		0x0002
#define IR_outScene2_Shift	4
#define IR_outScene2_Mask	0x007F
// UnionOffset: 1, ParaOffset: 1, BitOffset: 5, Size: 7 Bit, Text: Szenennummer
#define ParamIR_outScene2Index(X) ((uint)((knx.paramWord((IR_ParamBlockOffset + IR_ParamBlockSize * X + 2)) >> IR_outScene2_Shift) & IR_outScene2_Mask))
// UnionOffset: 1, ParaOffset: 1, BitOffset: 5, Size: 7 Bit, Text: Szenennummer
#define ParamIR_outScene2 ((uint)((knx.paramWord((IR_ParamBlockOffset + IR_ParamBlockSize * channelIndex() + 2)) >> IR_outScene2_Shift) & IR_outScene2_Mask))
#define IR_outScene3		0x0003
#define IR_outScene3_Shift	5
#define IR_outScene3_Mask	0x007F
// UnionOffset: 1, ParaOffset: 2, BitOffset: 4, Size: 7 Bit, Text: Szenennummer
#define ParamIR_outScene3Index(X) ((uint)((knx.paramWord((IR_ParamBlockOffset + IR_ParamBlockSize * X + 3)) >> IR_outScene3_Shift) & IR_outScene3_Mask))
// UnionOffset: 1, ParaOffset: 2, BitOffset: 4, Size: 7 Bit, Text: Szenennummer
#define ParamIR_outScene3 ((uint)((knx.paramWord((IR_ParamBlockOffset + IR_ParamBlockSize * channelIndex() + 3)) >> IR_outScene3_Shift) & IR_outScene3_Mask))
#define IR_outScene4		0x0004
#define IR_outScene4_Shift	6
#define IR_outScene4_Mask	0x007F
// UnionOffset: 1, ParaOffset: 3, BitOffset: 3, Size: 7 Bit, Text: Szenennummer
#define ParamIR_outScene4Index(X) ((uint)((knx.paramWord((IR_ParamBlockOffset + IR_ParamBlockSize * X + 4)) >> IR_outScene4_Shift) & IR_outScene4_Mask))
// UnionOffset: 1, ParaOffset: 3, BitOffset: 3, Size: 7 Bit, Text: Szenennummer
#define ParamIR_outScene4 ((uint)((knx.paramWord((IR_ParamBlockOffset + IR_ParamBlockSize * channelIndex() + 4)) >> IR_outScene4_Shift) & IR_outScene4_Mask))
#define IR_outSceneActive1		0x0001
// UnionOffset: 1, ParaOffset: 0, BitOffset: 2, Size: 1 Bit, Text: Szene A
#define ParamIR_outSceneActive1Index(X) knx.paramBit((IR_ParamBlockOffset + IR_ParamBlockSize * X + 1), 2)
// UnionOffset: 1, ParaOffset: 0, BitOffset: 2, Size: 1 Bit, Text: Szene A
#define ParamIR_outSceneActive1 knx.paramBit((IR_ParamBlockOffset + IR_ParamBlockSize * channelIndex() + 1), 2)
#define IR_outSceneActive2		0x0001
// UnionOffset: 1, ParaOffset: 0, BitOffset: 3, Size: 1 Bit, Text: Szene B
#define ParamIR_outSceneActive2Index(X) knx.paramBit((IR_ParamBlockOffset + IR_ParamBlockSize * X + 1), 3)
// UnionOffset: 1, ParaOffset: 0, BitOffset: 3, Size: 1 Bit, Text: Szene B
#define ParamIR_outSceneActive2 knx.paramBit((IR_ParamBlockOffset + IR_ParamBlockSize * channelIndex() + 1), 3)
#define IR_outSceneActive3		0x0001
// UnionOffset: 1, ParaOffset: 0, BitOffset: 4, Size: 1 Bit, Text: Szene C
#define ParamIR_outSceneActive3Index(X) knx.paramBit((IR_ParamBlockOffset + IR_ParamBlockSize * X + 1), 4)
// UnionOffset: 1, ParaOffset: 0, BitOffset: 4, Size: 1 Bit, Text: Szene C
#define ParamIR_outSceneActive3 knx.paramBit((IR_ParamBlockOffset + IR_ParamBlockSize * channelIndex() + 1), 4)
#define IR_outSceneActive4		0x0001
// UnionOffset: 1, ParaOffset: 0, BitOffset: 5, Size: 1 Bit, Text: Szene D
#define ParamIR_outSceneActive4Index(X) knx.paramBit((IR_ParamBlockOffset + IR_ParamBlockSize * X + 1), 5)
// UnionOffset: 1, ParaOffset: 0, BitOffset: 5, Size: 1 Bit, Text: Szene D
#define ParamIR_outSceneActive4 knx.paramBit((IR_ParamBlockOffset + IR_ParamBlockSize * channelIndex() + 1), 5)
//!< Number: 1, Text: IR{{argChan}} <- {{0:}}, Function: Farbe
#define IR_Koco_n1 1
#define KoIR_co_n1Index(X) knx.getGroupObject(IR_KoOffset + IR_KoBlockSize * X + 1)
#define KoIR_co_n1 knx.getGroupObject(IR_KoOffset + IR_KoBlockSize * channelIndex() + 1)
//!< Number: 2, Text: IR{{argChan}} <- {{0:}}, Function: Status
#define IR_Koco_n2 2
#define KoIR_co_n2Index(X) knx.getGroupObject(IR_KoOffset + IR_KoBlockSize * X + 2)
#define KoIR_co_n2 knx.getGroupObject(IR_KoOffset + IR_KoBlockSize * channelIndex() + 2)

