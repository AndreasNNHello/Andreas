#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <iostream>
#include <cstring>

#define  SIZE_MAX 10240
//#define ARRAY(k)   for(TJsonArray  array(k);  array();)
#define OBJECT(k)  for(TJsonObject object(k); object();)
#define VALUE(k,v) {TJsonValue value(k,v);}
#define TCF_USED   1
#define TCF_OBJECT 4
#define TCF_ARRAY  2


class TTagCloser
{
public:
    TTagCloser *Owner;
    static TTagCloser *Current;
    static int Depth;
    int Flags;
    int Count;
    int operator()(){Flags^=TCF_USED; return Flags&TCF_USED;}
    TTagCloser(){Count=Flags=0; Owner=Current; Current=this; Depth++;}
    ~TTagCloser(){Depth--; Current=Owner;}
};

TTagCloser *TTagCloser::Current=NULL;
int TTagCloser::Depth=-1;

class TJsonTagCloser:public TTagCloser
{
public:
    void WriteTab();
    void WriteInt(int);
    void WriteDouble(double);
    void WriteStr(char *);

    TJsonTagCloser(char *Key);
};

TJsonTagCloser::TJsonTagCloser(char *Key):TTagCloser()
{
    if(Owner)
    {
        if(Owner->Count)
            WriteStr(",");
        if(Owner->Flags&TCF_ARRAY)
        {
            if(!Owner->Count)
                WriteTab();
        }
        else
        {
            WriteTab();
            WriteStr("\"");
            if(Key)
                WriteStr(Key);
            WriteStr("\":");
        }
        Owner->Count++;
    }
}

class TJsonObject:public TJsonTagCloser
{
public:
    TJsonObject(char *Key);
    ~TJsonObject();
};

class TJsonValue:public TJsonTagCloser
{
public:
    TJsonValue(char *Key, int    Value):TJsonTagCloser(Key){WriteInt   (Value);}
    TJsonValue(char *Key, double Value):TJsonTagCloser(Key){WriteDouble(Value);}
    TJsonValue(char *Key, bool   Value):TJsonTagCloser(Key){WriteStr((char *)(Value?"true":"false"));}
    TJsonValue(char *Key, char  *Value);
};

//----------------------------------------------------------------------------
TJsonObject::TJsonObject(char *Key):TJsonTagCloser(Key)
{
    Flags|=TCF_OBJECT;
    if(Owner && (!(Owner->Flags&TCF_ARRAY) || Owner->Count>1))
    WriteStr("{");
}
TJsonObject::~TJsonObject()
{
    WriteTab();
    WriteStr("}");
}

TJsonValue::TJsonValue(char *Key, char *Value):TJsonTagCloser(Key)
{
    if(Value)
    {
        WriteStr("\"");
        WriteStr(Value);
        WriteStr("\"");
    }
    else
        WriteStr("null");
}

void TJsonTagCloser::WriteTab(){printf("\n%*s", Depth*2, "");}
void TJsonTagCloser::WriteInt(int Value){printf("%d", Value);}
void TJsonTagCloser::WriteDouble(double Value){printf("%lf", Value);}
void TJsonTagCloser::WriteStr(char *Value){printf("%s", Value);}
int size_word(std::string fstr);
std::string ch_word(int col_char, std::string word);

int main() {
    int clsock;
    struct sockaddr_in addr;

    clsock = socket(AF_INET, SOCK_STREAM, 0);
    if (clsock == -1) {
        perror("sock not created");
    }
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("80.211.216.250");
    addr.sin_port = htons(23434);

    if (connect(clsock, (const struct sockaddr *) &addr, sizeof(addr)) == -1) {
        perror("not connection");
    }
    std::string myLine;
    int z;
    char c[SIZE_MAX];
    std::string id;
    std::string imei;
    std::string name_from;
    std::string name_to;
    std::string numb_from;
    std::string numb_to;
    std::string date;
    std::string times;
    std::string duration;
    //while(1){
        if ((z = recv(clsock, c, SIZE_MAX, 0)) == -1) {
            perror("error");
        }
    std::string str(c);
    std::cout << str;
    while (!str.empty()) {
        int col = size_word(str);
        switch (col) {
            case 20:
                if (name_from == "") {
                    name_from = ch_word(col, str);
                } else
                    name_to = ch_word(col, str);
                break;
            case 10:
                if (name_from == "") {
                    numb_from = ch_word(col, str);
                } else
                    numb_to = ch_word(col, str);
                break;
            case 8:
                times = ch_word(col, str);
                break;
            case 6:
                duration = ch_word(col, str);
                break;
            case 5:
                id = ch_word(col, str);
                break;
            case 15:
                imei = ch_word(col, str);
                break;
            default:
                break;
        }
        str.erase(0,col);
    }



        OBJECT("id")
        {
            OBJECT("imei")
            {
                VALUE("numb_from", numb_from);
                VALUE("numb_to",   numb_to);
                VALUE("name_from", name_from);
                VALUE("name_to",   name_to);
                VALUE("date",      date);
                VALUE("time",      times);
                VALUE("duration",  duration);
            }
        }
    return 0;
}

int size_word(std::string fstr){
    int count = 0;
    for(std::string::iterator it = fstr.begin(); it != fstr.end(); it++){
        while (*it != ' ')
        count++;
}
    return count;
}

std::string ch_word(int col_char, std::string fstr){
std::string word;
for (auto counter = fstr.begin(); counter != fstr.begin()+col_char; counter++) {
        word += *counter;
    }
    return word;
}