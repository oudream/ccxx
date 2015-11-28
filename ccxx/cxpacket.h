#ifndef CXPACKET_H
#define CXPACKET_H


#include "cxglobal.h"
#include "cxinterinfo.h"


template<typename head_t, typename front_t, typename length_t, typename body_t, typename end_t>
class CxPacket5
{
public:
    struct PacketData
    {
        PacketData(head_t h, front_t f, body_t b, end_t e, int l, char * d) : head(h), front(f), body(b), end(e), length(l), data(d) {}
        head_t head;
        front_t front;
        body_t body;
        end_t end;
        int length;
        char * data;
    };

    typedef end_t (*fn_crc_t)(const char * pData, int iLength);

    class ReceivePacket
    {
    public:
        ReceivePacket(const head_t & head, int iBufferSize = 1024 * 16) :
            _head(head),
            _tmpBuffer(256, 0)
        {
            _tmpBufferP = & _tmpBuffer.front();
            _state = 0;
            _dataCurrent = NULL;
            _dataBegin = NULL;
            _dataEnd = NULL;
            setBufferSize(iBufferSize);
        }

        ~ReceivePacket()
        {
        }

        void setBufferSize(int iBufferSize)
        {
            _dataBuffer.resize(iBufferSize, 0);
            _dataBufferP = & _dataBuffer.front();
        }

        void push(const char *pData, int iLength)
        {
            const char * pIndex = pData;
            const char * pEnd = pData + iLength;
            while (pIndex < pEnd)
            {
                switch (_state)
                {
                case 0:
                {
                    head_t h = *(head_t *)(pIndex);
                    if (h == _head)
                    {
                        pIndex += sizeof(head_t)-1;
                        _state = 1;
                        _tmpIndex = 0;
                    }
                }
                    break;
                case 1:
                {
                    _tmpBuffer[_tmpIndex] = *pIndex;
                    ++_tmpIndex;
                    if (_tmpIndex == sizeof(front_t))
                    {
                        _front = *((front_t*)_tmpBufferP/*_tmpBuffer.data()*/);
                        _state = 2;
                        _tmpIndex = 0;
                    }
                }
                    break;
                case 2:
                {
                    _tmpBuffer[_tmpIndex] = *pIndex;
                    ++_tmpIndex;
                    if (_tmpIndex == sizeof(length_t))
                    {
                        _dataLength = *((length_t*)_tmpBufferP/*_tmpBuffer.data()*/);
                        _state = 3;
                        _tmpIndex = 0;
                    }
                }
                    break;
                case 3:
                {
                    _tmpBuffer[_tmpIndex] = *pIndex;
                    ++_tmpIndex;
                    if (_tmpIndex == sizeof(body_t))
                    {
                        _body = *((body_t*)_tmpBufferP/*_tmpBuffer.data()*/);
                        if (_dataLength == 0)
                        {
                            _state = 5;
                            _tmpIndex = 0;
                            break;
                        }
                        if (_packets.size() > 0)
                        {
                            const PacketData & packet = _packets.back();
                            _dataBegin = packet.data + packet.length;
                        }
                        else
                        {
                            _dataBegin = _dataBufferP/*_dataBuffer.data()*/;
                        }
                        if (! _dataCurrent) _dataCurrent = _dataBegin;
                        _dataEnd = _dataCurrent + _dataLength;
                        if (_dataEnd > _dataBufferP/*_dataBuffer.data()*/ + _dataBuffer.size())
                        {
                            cxPrompt() << "CxPacket : out buffer size !!!" << cxEnd;
                            _state = 0;
                            break;
                        }
                        _state = 4;
                    }
                }
                    break;
                case 4:
                {
                    * _dataCurrent = * pIndex;
                    ++_dataCurrent;
                    if (_dataCurrent >= _dataEnd)
                    {
                        _state = 5;
                        _tmpIndex = 0;
                    }
                }
                    break;
                case 5:
                {
                    _tmpBuffer[_tmpIndex] = *pIndex;
                    ++_tmpIndex;
                    if (_tmpIndex == sizeof(end_t))
                    {
                        _end = *((end_t*)_tmpBufferP/*_tmpBuffer.data()*/);
                        appendPacket();
                        _state = 0;
                    }
                }
                    break;
                default:
                    break;
                }
                ++pIndex;
            }
        }

        void appendPacket()
        {
            PacketData packet(_head, _front, _body, _end, _dataLength, _dataBegin);

            _packets.push_back(packet);
        }

        inline size_t count() const { return _packets.size(); }

        inline const std::vector<PacketData> & packets() const { return _packets; }

        inline void clear()
        {
            _packets.clear();
            if (_dataCurrent < _dataEnd)
            {
                memmove(_dataBufferP/*(char *)_dataBuffer.data()*/, _dataBegin, _dataCurrent - _dataBegin);
                _dataEnd = _dataBufferP/*_dataBuffer.data()*/ + _dataLength - 1;
                _dataCurrent = _dataBufferP/*_dataBuffer.data()*/ + ( _dataCurrent - _dataBegin );
                _dataBegin = _dataBufferP/*_dataBuffer.data()*/;
            }
            else
            {
                _dataCurrent = NULL;
                _dataEnd = NULL;
                _dataBegin = NULL;
            }
        }

    private:
        head_t _head;
        front_t _front;
    //    length_t _length;
        body_t _body;
        end_t _end;
        int _state;
        std::vector<char> _tmpBuffer;
        char * _tmpBufferP;
        int _tmpIndex;
        int _dataLength;
        int _dataIndex;
        std::vector<char> _dataBuffer;
        char * _dataBufferP;
        char * _dataCurrent;
        char * _dataEnd;
        char * _dataBegin;
        std::vector<PacketData> _packets;

    };



    class SendPacket
    {
    public:
        SendPacket(const head_t & head, front_t front, fn_crc_t fn_crc)
        {
            _head = head;
            _front = front;
            _fn_crc = fn_crc;
            _size = 4096;
        }

        ~SendPacket()
        {
        }

        int setPack(const body_t & b, const char * d, int len)
        {
            if (len<0) return -1;
            int iSize = sizeof(head_t)+sizeof(front_t)+sizeof(length_t)+sizeof(body_t)+sizeof(end_t) + len;
            if (iSize > 4096) return -1;
            char * data = _data;
            *((head_t *)data) = _head; data += sizeof(head_t);
            *((front_t *)data) = _front; data += sizeof(front_t);
            *((length_t *)data) = len; data += sizeof(length_t);
            *((body_t *)data) = b; data += sizeof(body_t);
            if (d && len > 0)
            {
                memcpy(data, d, len); data += len;
            }
            *((end_t *)data) = _fn_crc(_data+sizeof(head_t)+sizeof(front_t)+sizeof(length_t), sizeof(body_t)+len);
            _size = iSize;
            return iSize;
        }

        inline const char * data() { return _data; }

        inline int size() { return _size; }

    private:
        fn_crc_t _fn_crc;
        head_t _head;
        front_t _front;
        char _data[4096];
        int _size;

    };

};


//do not delete
//
//void ::receivedData(const char *pData, int iLength)
//{
//    if (iLength>0 && iLength<4097)
//    {
//        memcpy(_recPch, pData, iLength);
//        _recPch += iLength;

//        const char * pchIndex = takeBuffer(_recBuffer, _recPch);
//        if (pchIndex != _recBuffer)
//        {
//            int iSize = 0;
//            if (pchIndex != _recPch ) // (pchIndex < pchEnd)
//            {
//                iSize = pchEnd-pchIndex;
//                memmove(_recBuffer, pchIndex, iSize);
//            }
//            _recPch = _recBuffer + iSize;
//        }
//    }
//}

//const char * ::takeBuffer(const char * pchBegin, const char * pchEnd)
//{
//    const char * pchIndex = pchBegin;
//    while (pchIndex < pchEnd)
//    {
//        int h = *(int *)(pchIndex);
//        if (h == c_psm_head)
//        {
//            if (pchEnd - pchIndex < sizeof(PacketData))
//            {
//                //长度太小，够积累后再处理
//                break;
//            }
//            PacketData frame;
//            frame.front = *(short *)(pchIndex+4);
//            frame.length = *(short *)(pchIndex+4+2);
//            if (l <0 || l > 4096)
//            {
//                //格式不合，跳到下个循环
//                pchIndex+=6;
//                continue;
//            }
//            frame.body = *(body_t *)(pchIndex+4+2+2);
//            frame.data = std::string((pchIndex+4+2+2+sizeof(psm_packet_body_t)), frame.dataLength);
//            frame.crc = *(short *)(pchIndex+4+2+2+sizeof(psm_packet_body_t)+frame.dataLength);
//        }
//        else
//        {
//            pchIndex++;
//        }
//    }
//    return pchIndex;
//}

#endif // CXPACKET_H
