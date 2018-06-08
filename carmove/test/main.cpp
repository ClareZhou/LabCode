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

stack<Point2D> getpaths10()
{
    fstream paths10;
    paths10.open("/home/stone/code/clionProject/carmove/data/paths10",ios::out);
    Point2D p1(1,1,0);
    Point2D center1(2,1,0);
    float r1 = 1;
    Point2D p2(3,2,0);
    Point2D center2(3,3,0);
    float r2 = 1;
    Point2D p3(3,4,0);
    Point2D p4(1,4,0);
    vector<Point2D> paths;
    stack<Point2D> pathstacks;


    paths.push_back(p1);
    for(int jiaodu = 180;jiaodu >=90;jiaodu  = jiaodu - 15)
    {
        Point2D temppoint;
        double hudu = jiaodu*M_PI /180;
        auto jiaodu1 = cos(hudu);
        auto jiaodu2 = sin(hudu);
        if(jiaodu2<0.00001&&jiaodu2>-0.00001)
            jiaodu2 = 0.0;
        temppoint.x = center1.x + cos(hudu)*r1;
        temppoint.y = center1.y + sin(hudu)*r1;
        paths.push_back(temppoint);
    }

    paths.push_back(p2);
    for(int jiaodu = -90;jiaodu <=90;jiaodu = jiaodu +25)
    {
        Point2D temppoint;
        double hudu = jiaodu*PI / 180;
        temppoint.x = center2.x + cos(hudu)*r2;
        temppoint.y = center2.y + sin(hudu)*r2;
        paths.push_back(temppoint);
    }
    paths.push_back(p3);
    paths.push_back(p4);
//    std::reverse(paths.begin(),paths.end());

    for(auto p:paths)
    {
        pathstacks.push(p);
        cout<<"("<<p.x<<","<<p.y<<","<<p.z<<"),";
        paths10<<"("<<p.x<<","<<p.y<<","<<p.z<<"),";
    }
    cout<<endl;

    return pathstacks;
}

stack<Point2D> getpaths11()
{
    fstream paths11;
    paths11.open("/home/stone/code/clionProject/justfortest/paths11",ios::out);
    Point2D p1(0.5,0.5,0);
    Point2D center1(1.5,0.5,0);
    float r1 = 1;
    Point2D p2(3.5,1.5,0);
    Point2D center2(3.5,2.5,0);
    float r2 = 1;
    Point2D p3(2.5,4.5,0);
    Point2D p4(0.5,4.5,0);
    vector<Point2D> paths;
    stack<Point2D> pathstacks;


    paths.push_back(p1);
    for(int jiaodu = 180;jiaodu >=90;jiaodu = jiaodu -10)
    {
        Point2D temppoint;
        double hudu = jiaodu*M_PI /180;
        auto jiaodu1 = cos(hudu);
        auto jiaodu2 = sin(hudu);
        if(jiaodu2<0.00001&&jiaodu2>-0.00001)
            jiaodu2 = 0.0;
        temppoint.x = center1.x + cos(hudu)*r1;
        temppoint.y = center1.y + sin(hudu)*r1;
        paths.push_back(temppoint);
    }

    paths.push_back(p2);
    for(int jiaodu = -90;jiaodu <=90;jiaodu = jiaodu +10)
    {
        Point2D temppoint;
        double hudu = jiaodu*PI / 180;
        temppoint.x = center2.x + cos(hudu)*r2;
        temppoint.y = center2.y + sin(hudu)*r2;
        paths.push_back(temppoint);
    }
    paths.push_back(p3);
    paths.push_back(p4);

    for(auto p:paths)
    {
        pathstacks.push(p);
        cout<<"("<<p.x<<","<<p.y<<","<<p.z<<"),";
        paths11<<"("<<p.x<<","<<p.y<<","<<p.z<<"),";
    }
    cout<<endl;

    return pathstacks;
}

stack<Point2D> getpaths12()
{
    fstream paths12;
    paths12.open("/home/stone/code/clionProject/justfortest/paths12",ios::out);
    Point2D p1(1.5,0.5,0);
    Point2D center1(2.5,0.5,0);
    float r1 = 1;
    Point2D p2(4.5,1.5,0);
    Point2D center2(4.5,2.5,0);
    float r2 = 1;
    Point2D p3(2.5,3.5,0);
    Point2D p4(0.5,3.5,0);
    vector<Point2D> paths;
    stack<Point2D> pathstacks;


    paths.push_back(p1);
    for(int jiaodu = 180;jiaodu >=90;jiaodu = jiaodu - 10)
    {
        Point2D temppoint;
        double hudu = jiaodu*M_PI /180;
        auto jiaodu1 = cos(hudu);
        auto jiaodu2 = sin(hudu);
        if(jiaodu2<0.00001&&jiaodu2>-0.00001)
            jiaodu2 = 0.0;
        temppoint.x = center1.x + cos(hudu)*r1;
        temppoint.y = center1.y + sin(hudu)*r1;
        paths.push_back(temppoint);
    }

    paths.push_back(p2);
    for(int jiaodu = -90;jiaodu <=90;jiaodu = jiaodu + 10)
    {
        Point2D temppoint;
        double hudu = jiaodu*PI / 180;
        temppoint.x = center2.x + cos(hudu)*r2;
        temppoint.y = center2.y + sin(hudu)*r2;
        paths.push_back(temppoint);
    }
    paths.push_back(p3);
    paths.push_back(p4);

    for(auto p:paths)
    {
        pathstacks.push(p);
        cout<<"("<<p.x<<","<<p.y<<","<<p.z<<"),";
        paths12<<"("<<p.x<<","<<p.y<<","<<p.z<<"),";
    }
    cout<<endl;

    return pathstacks;


}

void carrun()
{
    car_zyl _car(car_Dev.c_str(),uwb_Dev.c_str(),10);
    Point2D carPosition;
    Point2D startPosition[3] = {
            Point2D(1,1,0),Point2D(0.5,0.5,0),Point2D(1.5,0.5,0)
    };
    const string uwblog =to_string(_car.getcarid()) + "uwb.txt";
    ofstream recordPosition;
    recordPosition.open("./data/Position"+to_string(_car.getcarid()),ios::out);
    stack<Point2D> paths;
    switch (_car.getcarid())
    {
        case 10:
            paths = getpaths10();
        case 11:
            paths = getpaths11();
        case 12:
            paths = getpaths12();
    }
    thread t0(&car_zyl::getUwbPosition,&_car,ref(carPosition),uwblog);
    switch(_car.getcarid())
    {
        case 10:
        {
            bool flag10 = false;
            while(!flag10)
                flag10 = _car.moveToTarget(startPosition[0]);
            cout<<"I have moved to the startPosition"<<endl;
            break;
        }

        case 11:
        {
            bool flag11 = false;
            while(!flag11)
                flag11 = _car.moveToTarget(startPosition[1]);
            cout<<"I have moved to the startPosition"<<endl;
            break;
        }

        case 12:
        {
            bool flag12 = false;
            while(!flag12)
                flag12 = _car.moveToTarget(startPosition[2]);
            cout<<"I have moved to the startPosition"<<endl;
            break;
        }

        default:
            break;

    }



    while(!paths.empty())
    {
        Point2D tempPoint = paths.top();
        paths.pop();
        bool flag = false;
        while(!flag)
        {
            flag = _car.moveToTarget(tempPoint);
            cout<<"car "<<_car.getcarid()<<" : movefrom "<<_car.getCarPosition().x<<","<<_car.getCarPosition().y<<"to "
                <<tempPoint.x<<","<<tempPoint.y<<endl;
        }
        recordPosition<<_car.getcarid()<<","<<_car.getCarPosition().x<<","<<_car.getCarPosition().y<<","<<_car.movecount++<<endl;
    }
}

int main()
{
    //car_zyl _leader(car_Dev.c_str(),uwb_Dev.c_str(),0,lead_IP,1.0,PI/4);
//    _leader.new_carMovebyKeyboard();
//    getpaths10();

    carrun();

//    testcircle2();
    return 0;
}
