
#include<iostream>
#include<cassert>
#include<cmath>
#include<fstream>
#include<string>
#include<vector>
#include "../include/inf_typedef.h"
#include"../include/serial.h"
#include"../include/timeout.h"

using namespace std;

struct uwblps_package{
    S_FLOAT_XYZ position;
    S_FLOAT_XYZ velocity;
    float dis_buf[8];

    S_INT16_XYZ gyro;
    S_INT16_XYZ acc;
    S_INT16_XYZ mag;

    S_FLOAT_XYZ angle;
};

class uwb_zyl{
private:
    ofstream log;

    ofstream log_dis;

    serial uwblps_serial;

    uwblps_package i_uwb_lps;

    Point2D posizition;

    bool flag;

    int count;

public:


    uwb_zyl()
    {
        log.open("../data/log.txt",ios::out);
        log_dis.open("../data/logdis.txt",ios::out);
        this->count = 0;

    }

    uwb_zyl(string port)
    {
        int ret;
        ret = this->uwblps_serial.Open(port.c_str(), 460800, 8, NO, 1);
        if(ret == 0)
            cout<<"uwb open error!"<<endl;
        log.open("../data/log.txt",ios::app);
        log_dis.open("../data/logdis.txt",ios::out);
        this->count = 0;
    }

    uwb_zyl(const uwb_zyl& other);

    uwb_zyl operator=(const uwb_zyl& other);

    ~uwb_zyl()
    {
        log.close();

    }



    char openUwblpsSerial(const char* port, int baud,char bits,parity parity,char stopbit);

    int readData(uint8 *buffer);

    int readData(uint8 *buffer,int size);

    Point2D uwblps_Data_Convert(const uint8 recieve_buf[],int p);

    Point2D uwblps_Data_Convert(const uint8 recieve_buf[],int p,const string filename);

    Point2D uwblps_Data_Convert2(const uint8 recieve_buf[],int p,const string filename );

    void getUwbPosition(Point2D& the_point,const string filename);

    Point2D getPosizition();

    int getUwbSerialFd();

    void closeUwblpsSerial();

    void uwblps_Data_Convert(uint8 recieve_buf[]);

    void uwblps_Tag_Data_Updata(void);

    bool approimate(Point2D p1,Point2D p2)
    {
        if((p1.x - p2.x) < 0.20 && (p1.y - p2.y) < 0.20)
        {
            return ((p1.x - p2.x) > -0.20) && ((p1.y - p2.y) > -0.20);
        }
        else
            return false;
    }

};
