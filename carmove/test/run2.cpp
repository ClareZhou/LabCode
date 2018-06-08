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


const double PI = 3.141592653;

void test10( car_zyl& car)
/*
 * 可以在实验室跑U型
 */
{
    car.changeVelocity(0,25);
    sleep(12);

    car.changeVelocity(25,0);
    sleep(15);
    car.changeVelocity(0,-25);
    sleep(12);
    car.changeVelocity(0,-10);
    sleep(3);

    car.changeVelocity(-25,0);
    sleep(12);
    car.changeVelocity(0,0);
    cout<<"over!!!"<<endl;

}

void test11( car_zyl& car)
{
    car.changeVelocity(0,25);
    sleep(12);

    car.changeVelocity(25,0);
    sleep(15);
    car.changeVelocity(0,-25);
    sleep(12);
    car.changeVelocity(0,0);
    sleep(3);
    car.changeVelocity(-25,0);
    sleep(12);
    car.changeVelocity(0,0);
    cout<<"over!!!"<<endl;
}

void test12(car_zyl& car)
{
    car.changeVelocity(0,25);
    sleep(12);

    car.changeVelocity(25,0);
    sleep(15);

    car.changeVelocity(0,-25);
    sleep(12);

    car.changeVelocity(0,0);
    sleep(3);

    car.changeVelocity(-25,0);
    sleep(12);

    car.changeVelocity(0,0);

    cout<<"over!!!"<<endl;
}

void carrun()
{
    car_zyl _car(car_Dev.c_str(),uwb_Dev.c_str(),10);
    Point2D carPosition;

    const string uwblog =to_string(_car.getcarid()) + "uwb.txt";
    ofstream recordPosition;
    recordPosition.open("../data/Position"+to_string(_car.getcarid()),ios::out);

    thread t0(&car_zyl::getUwbPosition,&_car,ref(carPosition),uwblog);
    switch(_car.getcarid())
    {
        case 10:
        {
            test10(_car);
        }

        case 11:
        {
            test11(_car);
        }

        case 12:
        {
            test12(_car);
        }

        default:
            break;

    }

}

int main()
{

    carrun();

    return 0;
}
