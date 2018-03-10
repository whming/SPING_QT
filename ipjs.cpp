#include "ipjs.h"

char digits[]=
        {
                '0', '1', '2', '3', '4', '5',
                '6', '7', '8', '9', 'a', 'b',
                'c', 'd', 'e', 'f', 'g', 'h',
                'i', 'j', 'k', 'l', 'm', 'n',
                'o', 'p', 'q', 'r', 's', 't',
                'u', 'v', 'w', 'x', 'y', 'z'
        };

IP_JS::IP_JS(char*m_IP,char* m_count){
    memset(IP,0,16);
    strcpy(IP,m_IP);
    memset(startIP,0,16);
    strcpy(IP,m_IP);
    memset(endIP,0,16);
    strcpy(IP,m_IP);


    IP_Num=0;
    count=  std::atoi( m_count );
}

IP_JS::~IP_JS(){

}

int IP_JS::IP_init(){
//    printf("IP---%s\n",IP);
//    printf("count---%d\n",count);
    char seg[] = "."; /*分隔符这里为逗号comma，分隔符可以为你指定的，如分号，空格等*/
    i =0;
    substr= strtok(IP, seg);/*利用现成的分割函数,substr为分割出来的子字符串*/
    while (substr != NULL) {
        IP_4[i] = std::atoi( substr );/*把新分割出来的子字符串substr转为int拷贝到要存储的IP_4*/
        i++;
        printf("%s\n", substr);
        substr = strtok(NULL,seg);/*在第一次调用时，strtok()必需给予参数str字符串，              往后的调用则将参数str设置成NULL。每次调用成功则返回被分割出片段的指针。*/
    }

    i =0;
//    printf("mask=%s\n",getMask(count));
//    char m_mask[16] = {""};
    memset(m_mask,0,16);
    strcpy(m_mask,getMask(count));
//    printf("m_mask=%s\n",m_mask);

    /*char **/mask_substr= strtok(m_mask, seg);/*利用现成的分割函数,substr为分割出来的子字符串*/
    while (mask_substr != NULL) {
        mask_4[i] = std::atoi( mask_substr );/*把新分割出来的子字符串substr转为int拷贝到要存储的mask_4中*/
        i++;
//        printf("%s\n", mask_substr);
        mask_substr = strtok(NULL,seg);/*在第一次调用时，strtok()必需给予参数str字符串，
                往后的调用则将参数str设置成NULL。每次调用成功则返回被分割出片段的指针。*/
    }
    for (int j = 0; j < 4 ; ++j) {
        netIP[j] = IP_4[j]&mask_4[j];
//        printf("netIP[%d]=%d\n",j,netIP[j]);
    }

    int wei_num=0;
    int k=3;
    for (; k >= 0; ) {
        if(netIP[k]==0&&(32-count-wei_num)>8){
            wei_num+=8;
            g_IP[k]=netIP[k]|255;
        }
        else{
            break;
        }
        k--;
    }
    k++;
//    printf("g_IP[%d]=%d\n",k,g_IP[k]);
    for (int l = 0; l < k-1; ++l) {
        g_IP[l]=netIP[l];
//       printf("g_IP[%d]=%d\n",l,g_IP[l]);
    }
    int ip_2=netIP[k-1];

//    printf("wei_num=%d 32-count=%d\n",wei_num,32-count);
    if(wei_num!=32-count){
        g_IP[k-1]=ToUnsignedString(ip_2,1,32-count-wei_num);
    }else
        g_IP[k-1]=netIP[k-1];

    sprintf(startIP,"%d.%d.%d.%d",netIP[0],netIP[1],netIP[2],netIP[3]+1);
    sprintf(endIP,"%d.%d.%d.%d",g_IP[0],g_IP[1],g_IP[2],g_IP[3]-1);
//    printf("startIP---->%s\n",startIP);
//    printf("endIP---->%s\n",endIP);
    return 0;
}

char * IP_JS::getstartIP(){
    return startIP;
}

char * IP_JS::getendIP(){
    return endIP;
}

int  IP_JS::getIP_Num(){
    return 0;
}

int j_0(int i){
    int num=1;
    for (int j = 0; j < i; ++j) {
        if( j ==0){
            num*=1;
        }
        num *=2;
    }
    return num;
}

int IP_JS::ToUnsignedString(int i, int shift, int m_cunt) {
    int buf[8]={0};
    int charPos = 8;
    int radix = 1 << shift;
    int mask = radix - 1;
//    printf("m_cunt=%d\n",m_cunt);
    do {
        buf[--charPos] = i & mask;
        i = i >> shift;
//        printf("i&mask=%d,charpos=%d---%d\n",i&mask,charPos);
    } while (i != 0);

    for (int k = 7; k >=8-m_cunt; k--) {
        if(buf[k]==0)
            buf[k]=1;
        else
            break;
    }
    int ret=0;
    for (int j = 7; j >=0 ; j--) {
//        printf("buf[%d]=%d + j_0(8-%d)=%d\n",j,buf[j],j,j_0(7-j));
        ret +=j_0(7-j)*buf[j];
    }
//    printf("ret = %d\n",ret);
    return ret;
}

const  char *IP_JS::getMask(int masks){
    if(masks == 1)

        return "128.0.0.0";

    else if(masks == 2)

        return "192.0.0.0";

    else if(masks == 3)

        return "224.0.0.0";

    else if(masks == 4)

        return "240.0.0.0";

    else if(masks == 5)

        return "248.0.0.0";

    else if(masks == 6)

        return "252.0.0.0";

    else if(masks == 7)

        return "254.0.0.0";

    else if(masks == 8)

        return "255.0.0.0";

    else if(masks ==9)

        return "255.128.0.0";

    else if(masks == 10)

        return "255.192.0.0";

    else if(masks == 11)

        return "255.224.0.0";

    else if(masks == 12)

        return "255.240.0.0";

    else if(masks == 13)

        return "255.248.0.0";

    else if(masks == 14)

        return "255.252.0.0";

    else if(masks == 15)

        return "255.254.0.0";

    else if(masks == 16)

        return "255.255.0.0";

    else if(masks == 17)

        return "255.255.128.0";

    else if(masks == 18)

        return "255.255.192.0";

    else if(masks == 19)

        return "255.255.224.0";

    else if(masks == 20)

        return "255.255.240.0";

    else if(masks == 21)

        return "255.255.248.0";

    else if(masks == 22)

        return "255.255.252.0";

    else if(masks == 23)

        return "255.255.254.0";

    else if(masks == 24)

        return "255.255.255.0";

    else if(masks == 25)

        return "255.255.255.128";

    else if(masks == 26)

        return "255.255.255.192";

    else if(masks == 27)

        return "255.255.255.224";

    else if(masks == 28)

        return "255.255.255.240";

    else if(masks == 29)

        return "255.255.255.248";

    else if(masks == 30)

        return "255.255.255.252";

    else if(masks == 31)

        return "255.255.255.254";

    else if(masks == 32)

        return "255.255.255.255";
    else
        return NULL;

}
