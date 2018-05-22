#include <iostream>
#include<thread>
#include<algorithm>
#include <deque>
#include"car_zyl.h"

#define sleeptime usleep(400000)

const string car_Dev = "/dev/ttyUSB0";
const string uwb_Dev = "/dev/ttyUSB1";
const string uwb_Dev2 = "/dev/ttyUSB2";
const string lead_IP = "192.168.199.234";
const string follower_IP = "192.168.199.244";
const string leader_pub_port = "5555";
const string follower_pub_port = "6666";
const string OK_port = "7777";

const double PI = 3.141592653;

void test1()
/*
 * 可以在实验室跑U型
 */
{
    car_zyl _leader(car_Dev.c_str(),uwb_Dev.c_str(),0,lead_IP,1.0,PI/4);
//    _leader.new_carMovebyKeyboard();
    _leader.changeVelocity(0,25);
    sleep(12);

    _leader.changeVelocity(25,0);
    sleep(15);
    _leader.changeVelocity(0,-25);
    sleep(12);
    _leader.changeVelocity(0,0);
    cout<<"over!!!"<<endl;

}

void testcircle1()
{
    car_zyl _leader(car_Dev.c_str(),uwb_Dev.c_str(),0,lead_IP,1.0,PI/4);
    int x = 0;
    int y = 0;
    for(int i = 0;i<5;i++)
    {
        x = x+4;
        y = y+4;
        _leader.changeVelocity(x,y);
        sleeptime;
    }
    cout<<"111"<<x<<","<<y<<endl;
    for(int i = 0;i<5;i++)
    {
        x = x -4;

        _leader.changeVelocity(x,y);
        sleeptime;
    }
    cout<<"222:"<<x<<","<<y<<endl;
    for(int i = 0;i<5;i++)
    {
        x = x-4;
        y = y-4;
        _leader.changeVelocity(x,y);
        sleeptime;
    }
    cout<<"333"<<x<<","<<y<<endl;
    for(int i = 0;i<5;i++)
    {

        y = y-4;
        _leader.changeVelocity(x,y);
        sleeptime;
    }
    cout<<"444"<<x<<","<<y<<endl;

    for(int i = 0;i<5;i++)
    {
        x = x+4;

        _leader.changeVelocity(x,y);
        sleeptime;
    }
    cout<<"555"<<x<<","<<y<<endl;
    for(int i = 0;i<5;i++)
    {
        x = x+4;
        y = y +4;
        _leader.changeVelocity(x,y);
        sleeptime;
    }
    cout<<"666"<<x<<","<<y<<endl;
    _leader.changeVelocity(0,0);
}

void testcircle2()
{
    car_zyl _leader(car_Dev.c_str(),uwb_Dev.c_str(),0,lead_IP,1.0,PI/4);
    int x = 0;
    int y = 0;
    for(int i = 0;i<5;i++)
    {
        x = x+4;
        y = y+4;
        _leader.changeVelocity(x,y);
        sleeptime;
    }
    cout<<"111"<<x<<","<<y<<endl;
    for(int i = 0;i<10;i++)
    {
        x = x -4;
        y = y-2;
        _leader.changeVelocity(x,y);
        sleeptime;
    }
    cout<<"222:"<<x<<","<<y<<endl;
    for(int i = 0;i<10;i++)
    {
        x = x+2;
        y = y-2;
        _leader.changeVelocity(x,y);
        sleeptime;
    }
    cout<<"333"<<x<<","<<y<<endl;
    for(int i = 0;i<5;i++)
    {
        x = x+4;
        y = y+4;
        _leader.changeVelocity(x,y);
        sleeptime;
    }
    cout<<"444"<<x<<","<<y<<endl;


    _leader.changeVelocity(0,0);
}


int main()
{
    //car_zyl _leader(car_Dev.c_str(),uwb_Dev.c_str(),0,lead_IP,1.0,PI/4);
//    _leader.new_carMovebyKeyboard();
    testcircle2();
    return 0;
}
