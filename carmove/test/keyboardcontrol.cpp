//
// Created by zhouyuliang on 18-5-31.
//

#include <iostream>
#include<thread>
#include<algorithm>
#include <deque>
#include <stack>
#include"../include/car_zyl.h"

#define sleeptime usleep(400000)

const string car_Dev = "/dev/ttyUSB0";
const string uwb_Dev = "/dev/ttyUSB1";
const string uwb_Dev2 = "/dev/ttyUSB2";

int main()
{
    car_zyl _leader(car_Dev.c_str(),uwb_Dev.c_str(),0);
    _leader.new_carMovebyKeyboard();
    return 0;
}
