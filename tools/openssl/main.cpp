#include <ccxx/ccxx.h>
//#include <stdio.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>

using namespace std;

RSA* createRSA(unsigned char * key) {
    RSA *rsa = NULL;
    BIO *keybio = NULL;
    keybio = BIO_new_mem_buf(key, -1);
    if (keybio == NULL) {
        return 0;
    }
    return PEM_read_bio_RSA_PUBKEY(keybio, &rsa, NULL, NULL);
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

int main() {

    string sEncrypted = CxFile::load("/fff/tmp/license/license2.txt");
    sEncrypted = CxEncoding::base64Decode(sEncrypted);

    string sPublicKey = "-----BEGIN PUBLIC KEY-----\n" +
                        CxFile::load("/fff/tmp/license/pubkey.txt") +
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
