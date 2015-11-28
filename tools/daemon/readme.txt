/***
 * test
1) test heart string
void testHeartString()
{
    char receiveBuf[1024];
//    fill(receiveBuf, receiveBuf+1024, 0);
    receiveBuf[2] = 0xa5;
    receiveBuf[3] = 0x5a;
    receiveBuf[4] = 0xa5;
    receiveBuf[5] = 0x5a;

    receiveBuf[10] = 0xa5;
    receiveBuf[11] = 0x5a;
    receiveBuf[12] = 0xa5;
    receiveBuf[13] = 0x5a;

    receiveBuf[18] = 0xa5;
    receiveBuf[19] = 0x5a;

    int iReceiveLength = 20;
    string sReceive(receiveBuf, iReceiveLength);
    fn_outInfo("sReceive == " + fn_toHexstring(sReceive.data(), sReceive.size(), false));

    Processer * oProcesser = f_oProcesses.at(0);
    string sHeart = oProcesser->heartBuffer;
    fn_outInfo("sHeart == " + fn_toHexstring(sHeart.data(), sHeart.size(), false));

    char temp1[256];
    fill(temp1, temp1+256, 0);
    char temp2[256];
    fill(temp2, temp2+256, 0);
    memcpy(temp1, sReceive.data(), sReceive.size());
    memcpy(temp2, sHeart.data(), sHeart.size());

    fn_outInfo(string(temp1, sReceive.size()));
    fn_outInfo(string(temp2, sHeart.size()));

    size_t found = sReceive.rfind(sHeart);
    if (found != string::npos)
    {
        sReceive = sReceive.substr(found+sHeart.size());
        fn_outInfo("sReceive == " + fn_toHexstring(sReceive.data(), sReceive.size(), false));
    }
}
 */
