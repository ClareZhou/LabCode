//
// Created by zhouyuliang on 18-5-31.
//

#include <iostream>
#include<thread>
#include<mutex>
#include <zmq.hpp>
#include <zhelpers.hpp>
#include "../include/uwblps.h"

void testUwbbyBuf(Point2D& point)
{
    uwb_zyl uwblps;
    uint8 buffer[256] = {0};
    uwblps.openUwblpsSerial("/dev/ttyUSB1",460800,8,NO,1);
    while(1)
    {
        int rd_num = uwblps.readData(buffer);
        //cout<<rd_num<<endl;
        if(rd_num)
        {
            for(int i = 0;i<rd_num;i++)
            {
                if(buffer[i] == 0x55 && buffer[i+1] == 0x01)
                {
                    if(rd_num - i >127)
                    {
                        point = uwblps.uwblps_Data_Convert(buffer,i);
                    }
                }
            }
        }
    }
}

int main()
{
    Point2D the_point;


    testUwbbyBuf(the_point);
//    cout<<the_point.x<<","<<the_point.y<<endl;
    return 0;

}