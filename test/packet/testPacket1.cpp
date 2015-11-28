/*
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

    class ReceivePacket
    {
    public:
        ReceivePacket(const head_t & head, int iBufferSize = 1024 * 16) :
            _head(head),
            _tmpBuffer(256, 0)
        {
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
                        _front = *((front_t*)_tmpBuffer.data());
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
                        _dataLength = *((int*)_tmpBuffer.data());
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
                        _body = *((body_t*)_tmpBuffer.data());
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
                            _dataBegin = _dataBuffer.data();
                        }
                        if (! _dataCurrent) _dataCurrent = _dataBegin;
                        _dataEnd = _dataCurrent + _dataLength;
                        if (_dataEnd > _dataBuffer.data() + _dataBuffer.size())
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
                        _end = *((end_t*)_tmpBuffer.data());
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
                memmove((char *)_dataBuffer.data(), _dataBegin, _dataCurrent - _dataBegin);
                _dataEnd = _dataBuffer.data() + _dataLength - 1;
                _dataCurrent = _dataBuffer.data() + ( _dataCurrent - _dataBegin );
                _dataBegin = _dataBuffer.data();
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
        int _tmpIndex;
        int _dataLength;
        int _dataIndex;
        std::vector<char> _dataBuffer;
        char * _dataCurrent;
        char * _dataEnd;
        char * _dataBegin;
        std::vector<PacketData> _packets;

    };

    class SendPacket
    {
    public:
        typedef end_t (*fn_crc_t)(const uchar * pData, int iLength);

        SendPacket(const head_t & head, front_t front, fn_crc_t fn_crc)
        {
            _head = head;
            _front = front;
            _fn_crc = fn_crc;
        }

        ~SendPacket()
        {
        }

        void push(const body_t & b, const char * d, int len)
        {
            std::string packet(sizeof(head_t)+sizeof(front_t)+sizeof(length_t)+sizeof(body_t)+sizeof(end_t) + len, 0);
            char * data = (char *)packet.data();
            *((head_t *)data) = _head; data += sizeof(head_t);
            *((front_t *)data) = _front; data += sizeof(front_t);
            *((length_t *)data) = len; data += sizeof(length_t);
            *((body_t *)data) = b; data += sizeof(body_t);
            memcpy(data, d, len); data += len;
            *((end_t *)data) = _fn_crc((uchar *)packet.data()+sizeof(head_t)+sizeof(front_t)+sizeof(length_t), sizeof(body_t)+len);
            _packets.push_back(packet);
        }

        inline size_t count() const { return _packets.size(); }

        inline const char * dataFirst() const
        {
            if (_packets.size() > 0)
                return (_packets[0]).data();
            else
                return NULL;
        }

        inline size_t dataFirstSize() const
        {
            if (_packets.size() > 0)
                return (_packets[0]).size();
            else
                return 0;
        }

        inline const std::vector<std::string> & packets() const { return _packets; }

        inline void clear() { _packets.clear(); }

    private:
        fn_crc_t _fn_crc;
        head_t _head;
        front_t _front;
        std::vector<std::string> _packets;

    };

};
*/
