#pragma once
EXTERN_C __declspec(selectany) const GUID ADVDBG_WEL_PUBLISHER = {0xaf54735d, 0x22ae, 0x4f72, {0x8b, 0x3b,0x97,0x21,0xce,0xe7,0x41,0xf6}};
#define ADVDBG_WEL_PUBLISHER_CHANNEL_C1 0x9
#define ADVDBG_PUBLISHER_TESTCHNL 0x10
EXTERN_C __declspec(selectany) const EVENT_DESCRIPTOR PROCESS_INFO_EVENT = {0x1, 0x0, 0x10, 0x4, 0x0, 0x0, 0x4000000000000000};
#define PROCESS_INFO_EVENT_value 0x1
#define MSG_Publisher_EventMessage       0xB0000001L

