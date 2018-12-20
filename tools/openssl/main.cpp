#include <ccxx/ccxx.h>
//#include <stdio.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>

using namespace std;

RSA* createRSA(unsigned char * key)
{
    BIO *keybio = BIO_new_mem_buf(key, -1);
    if (keybio == NULL) {
        return 0;
    }
    RSA * r = PEM_read_bio_RSA_PUBKEY(keybio, NULL, NULL, NULL);
    BIO_free(keybio);
    return r;
}

int main_0() {
    unsigned char encrypted_data[] = {0x62,0xe2,0xe6,0xfd,0xca,0x69,0x39,0x2f,0x0f,0x07,0x3c,0x27,0xd7,0x49,0x2c,0xd6,0x6e,0xec,0xa0,0xdd,0x7c,0xa9,0xce,0x0a,0xad,0x4a,0x68,0xa2,0x2c,0x99,0xec,0xe9,0xa0,0x3c,0x72,0x66,0xf9,0xb1,0x59,0x11,0x7e,0x64,0x87,0x22,0xa7,0x4a,0x66,0xe2,0x8b,0x51,0xa5,0x6a,0x93,0x92,0x3f,0x57,0xae,0xea,0xfa,0xe7,0x6b,0x1b,0xae,0x8f};
    unsigned char publicKey[]="-----BEGIN PUBLIC KEY-----\n"\
                     "MFwwDQYJKoZIhvcNAQEBBQADSwAwSAJBAJVdq5JlvtxJT4CqwEceW4M4AKFbDmJE\n"\
                     "H2K0a4aXmeHqedlsQgRePVCDgiiCC7kr1DEkP3+9uOUHDUtvIIoE4VsCAwEAAQ==\n"\
                     "-----END PUBLIC KEY-----\n";
    unsigned char decrypted[1024]= {0};
    int decrypted_length = RSA_public_decrypt(sizeof(encrypted_data), encrypted_data, decrypted, createRSA(publicKey), RSA_NO_PADDING);
    if(decrypted_length == -1) {
        return -1;
    }
    printf("decrypted by openssl:\n");
    for(int i=0; i<decrypted_length; i++) {
        printf("%02x ",(unsigned char)decrypted[i]);
    }
    printf("\n");
}

int main_1()
{

//    string sEncrypted = CxFile::load("/fff/tmp/license/license2.txt");
    string sEncrypted = CxFile::load("f:/tmp/ygctlicense1.txt");
    sEncrypted = CxEncoding::base64Decode(sEncrypted);

    string sPublicKey = "-----BEGIN PUBLIC KEY-----\n" +
//                        CxFile::load("/fff/tmp/license/pubkey.txt") +
                        CxFile::load("f:/tmp/pubkey.txt") +
                        "\n-----END PUBLIC KEY-----\n";

    unsigned char decrypted[1024]= {0};
    int decrypted_length = RSA_public_decrypt(sEncrypted.size(), (unsigned char*)(sEncrypted.c_str()), decrypted, createRSA((unsigned char *)(sPublicKey.c_str())), RSA_PKCS1_PADDING);
    if(decrypted_length == -1) {
        return -1;
    }
    printf("decrypted by openssl:\n");
    for(int i=0; i<decrypted_length; i++) {
        printf("%02x ",(unsigned char)decrypted[i]);
    }
    printf("\n");
}

string getRsaDecrypt(const string& sFilePath)
{
    string sEncrypted = CxFile::load(sFilePath);
    sEncrypted = CxEncoding::base64Decode(sEncrypted);

    string sPublicKey = "-----BEGIN PUBLIC KEY-----\n";
    sPublicKey += "MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQCQmIxVuzxn2Je5LweL6aZxs7Y9QUBnciFS2cveEH+eKPEdQbp0KaswGAbT127g0ofoCcApyQEiV+cc40dsyD5kDn+fzzejt96rGo00bYR+8t6P57YqaPFfjjt+rVE4nWFADdt5ft6xjFM7OC1BJi3HyBm5ul9n1U8zhdHGyOXmGQIDAQAB";
    sPublicKey += "\n-----END PUBLIC KEY-----\n";

    unsigned char decrypted[1024]= {0};
    RSA * oRSA = createRSA((unsigned char *)(sPublicKey.c_str()));
    int decrypted_length = RSA_public_decrypt(sEncrypted.size(), (unsigned char*)(sEncrypted.c_str()), decrypted, oRSA, RSA_PKCS1_PADDING);
    RSA_free(oRSA);
    if(decrypted_length < 0 || decrypted_length > 1024)
    {
        return string();
    }
    return string((char*)decrypted, decrypted_length);
}

void fn_test(int, int, const void *, int, void *, void *)
{
}

int fn_interinfo_in_cmd( const std::string & sCommand, const std::map<std::string, std::string> & sParams, const msepoch_t & dtIn, int iSource, int eInType, int iTag)
{
    if (sCommand == "exit")
    {
        CxApplication::exit();
        return TRUE;
    }
    else
    {
        cxPrompt() << "in : " << sCommand;
    }
    return FALSE;
}

void fn_timer1(int)
{
    string sRsaDecrypt;
    sRsaDecrypt = getRsaDecrypt("f:/tmp/ygctlicense1.txt");
    cxPrompt() << sRsaDecrypt;
    sRsaDecrypt = getRsaDecrypt("f:/tmp/ygctlicense2.txt");
    cxPrompt() << sRsaDecrypt;
    sRsaDecrypt = getRsaDecrypt("f:/tmp/ygctlicense3.txt");
    cxPrompt() << sRsaDecrypt;
    static int iCount = 0;
    cxPrompt() << CxTime::currentSystemTimeString() << " - count = " << iCount++;
}

int main(int argc, const char *argv[])
{
    CxApplication::init(argc, argv);

    CxApplication::pushProcessCallBack(fn_test);

    CxInterinfoIn::addObserver(fn_interinfo_in_cmd);

    CxTimerManager::startTimer(fn_timer1, 100);

    CxApplication::exec();
}