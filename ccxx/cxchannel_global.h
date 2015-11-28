#ifndef CXCHANNEL_GLOBAL_H
#define CXCHANNEL_GLOBAL_H

#include "cxglobal.h"

const std::string CS_EntryChannelName = "ChannelName";

const std::string CS_TypeSimpleName_Serial    = "Serial";
const std::string CS_TypeSimpleName_Tcpserver = "Tcpserver";
const std::string CS_TypeSimpleName_Tcpclient = "Tcpclient";
const std::string CS_TypeSimpleName_Udp       = "Udp";

const std::string CS_TypeSimpleName_MqReq = "MqReq";   //请求
const std::string CS_TypeSimpleName_MqRep = "MqRep";   //响应
const std::string CS_TypeSimpleName_MqRoute = "MqRoute";//路由
const std::string CS_TypeSimpleName_MqDealer = "MqDealer";//分发
const std::string CS_TypeSimpleName_MqSub = "MqSub";//订阅
const std::string CS_TypeSimpleName_MqPub = "MqPub";//发布
const std::string CS_TypeSimpleName_MqPull = "MqPull";//拉
const std::string CS_TypeSimpleName_MqPush = "MqPush";//推


const std::string CS_EntryIsSendQueue = "IsSendQueue";
const std::string CS_EntryIsProcessQueue = "IsProcessQueue";


const std::string CS_EntryRemoteIpAddress   = "RemoteIpAddress";
const std::string CS_EntryRemotePort        = "RemotePort";
const std::string CS_EntryLocalIpAddress    = "LocalIpAddress";
const std::string CS_EntryLocalPort         = "LocalPort";


const std::string CS_EntryPortName     = "PortName";
const std::string CS_EntryBaudRate     = "BaudRate";
const std::string CS_EntryCharacterSize= "CharacterSize";
const std::string CS_EntryStopBits     = "StopBits";
const std::string CS_EntryParity       = "Parity";
const std::string CS_EntryFlowControl  = "FlowControl";


#endif // CXCHANNEL_GLOBAL_H
