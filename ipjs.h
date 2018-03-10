#ifndef IPJS_H
#define IPJS_H

#include <stdio.h>
#include <string.h>
#include <string>

class IP_JS {
public:
    IP_JS(char*m_IP,char* m_count);
    ~IP_JS();
    char * getstartIP();
    char * getendIP();
    int  getIP_Num();
    int IP_init();
    char startIP[16];
    char endIP[16];
    int netIP[4];
    int g_IP[4];
private:
    const char *getMask(int masks);
    int ToUnsignedString(int i, int shift, int m_cunt);
    int IP_Num;
    char IP[16];
    int IP_4[4];
    int mask_4[4];
    int count;
    char *substr;
    int i;
    char *mask_substr;
    char m_mask[16];
};

#endif // IPJS_H
