//
// Created by stone on 18-4-19.
//

#ifndef CARMOVE_CAR_H
#define CARMOVE_CAR_H

#include <vector>
#include<thread>
#include<sstream>
#include<mutex>
#include "curses.h"
#include "uwblps.h"
#include"zmq.hpp"
#include"zhelpers.hpp"
using namespace std;

/*
 * 首先实现一个两台小车编队的情况
 * 1：指定队长和跟随者队员，队长不停的将自身的坐标传给队员（发布订阅的方式），队员不停的接受队长传来的消息，然后根据设定的距离和角度值运动到特定的位置，返回一个达成编队
 * 2：达成编队后保持编队，队长由键盘控制，跟随者保持；
 */

/*
 * 修改建议目前来看小车的carPosition以及leaderPosition应该当做临界资源处理加一个锁试试
 *
 *
 */



struct formation_constrain{

    double l;//单位是米

    double lx;

    double ly;

    double angle;//用弧度制表示

};

class car_zyl{
private:

    char send_Up[10] = {'\xff','\xfe','\x01','\x00','\x00','\x00','\x10','\x00','\x00','\x01'};
    char send_Down[10] = {'\xff','\xfe','\x01','\x00','\x00','\x00','\x10','\x00','\x00','\x02'};
    char send_Left[10] = {'\xff','\xfe','\x01','\x00','\x10','\x00','\x00','\x00','\x00','\x04'};
    char send_Right[10] = {'\xff','\xfe','\x01','\x00','\x10','\x00','\x00','\x00','\x00','\x03'};

    char send_Up2[10] = {'\xff','\xfe','\x01','\x00','\x00','\x00','\x20','\x00','\x00','\x01'};
    char send_Down2[10] = {'\xff','\xfe','\x01','\x00','\x00','\x00','\x20','\x00','\x00','\x02'};
    char send_Left2[10] = {'\xff','\xfe','\x01','\x00','\x20','\x00','\x00','\x00','\x00','\x04'};
    char send_Right2[10] = {'\xff','\xfe','\x01','\x00','\x20','\x00','\x00','\x00','\x00','\x03'};

    char send_Up3[10] = {'\xff','\xfe','\x01','\x00','\x00','\x00','\x30','\x00','\x00','\x01'};
    char send_Down3[10] = {'\xff','\xfe','\x01','\x00','\x00','\x00','\x30','\x00','\x00','\x02'};
    char send_Left3[10] = {'\xff','\xfe','\x01','\x00','\x30','\x00','\x00','\x00','\x00','\x04'};
    char send_Right3[10] = {'\xff','\xfe','\x01','\x00','\x30','\x00','\x00','\x00','\x00','\x03'};

    char send_L_D[10] = {'\xff','\xfe','\x01','\x00','\x10','\x00','\x10','\x00','\x00','\x06'};
    char send_R_D[10] = {'\xff','\xfe','\x01','\x00','\x10','\x00','\x10','\x00','\x00','\x02'};
    char send_L_U[10] = {'\xff','\xfe','\x01','\x00','\x10','\x00','\x10','\x00','\x00','\x04'};
    char send_R_U[10] = {'\xff','\xfe','\x01','\x00','\x10','\x00','\x10','\x00','\x00','\x01'};
    char send_Back[10] = {'\xff','\xfe','\x02','\x00','\x00','\x00','\x00','\x00','\x00','\x00'};

//    char send_Turn_R1[10] = {'\xff','\xfe','\x01','\x00','\x00','\x00','\x00','\x00','\x01','\x00'};
//    char send_Turn_L1[10] = {'\xff','\xfe','\x01','\x00','\x00','\x00','\x00','\x00','\x01','\xff'};

    char velocity[10] = {'\xff','\xfe','\x01','\x00','\x00','\x00','\x00','\x00','\x00','\x07'};

    int vx[2] = {0,0};
    int vy[2] = {0,0};
    int vz[2] = {0,0};

    /*int vx[2] = {'\x00','\x00'};//vx[0]对应控制x方向轴的低8位，相应的vx[1]负责控制x方向轴的高8位
    int vy[2] = {'\x00','\x00'};
    int vz[2] = {'\x00','\x00'};*/
    char cardir = '\x07';

    serial car_serial;

    ofstream car_log;

    time_t start_time;

    time_t  stop_time;

    time_t now_time;

    const char* car_port;

    const char* uwb_port;

    bool formation;//判断是否达成编队要求

    string car_IP;

    std::mutex PositionMutex;

    Point2D nowPosition;


    int car_ID;//用于后期选择队长使用,目前用0表示队长，1表示跟随的小车

    formation_constrain real_val;

    formation_constrain except_val;

    Point2D car_Position;

    Point2D Position;

public:

    uwb_zyl car_uwb;

    std::fstream recordPosition;

    int movecount;

    car_zyl() = default;

    car_zyl(const char* carport,const char* uwbport):car_port(carport),uwb_port(uwbport)
    {
        car_serial.Open(carport,115200,8,NO,1);
        car_uwb.openUwblpsSerial(uwbport,460800,8,NO,1);
        car_log.open("./carlog.txt",ios::app);
        start_time = time(NULL);
        movecount = 0;
        car_log<<"start_time:"<<start_time<<endl;
    }

    ~car_zyl(){
        stop_time = time(NULL);
        car_log<<"end time:"<<stop_time<<endl;
    }

    car_zyl(const char* carport,const char* uwbport,int carid,float x = 0.0,float y = 0.0,float z = 0.0 ,string carip = "0.0.0.0", double el = 0.0,double eangle = 0.0)
            :car_port(carport),uwb_port(uwbport),car_ID(carid),car_IP(carip)
    {
        this->car_uwb.openUwblpsSerial(uwbport,460800,8,NO,1);
        car_serial.Open(carport,115200,8,NO,1);
        Position.x = x;
        Position.y = y;
        Position.z = z;
        movecount = 0;
        except_val.l = el;
        except_val.angle = eangle;
        except_val.lx = el*sin(except_val.angle);
        except_val.ly = el*cos(except_val.angle);

        recordPosition.open("./recordPosition",ios::out);
    }

    car_zyl(int carid,string carip,double el = 0.0,double eangle = 0.0):car_ID(carid),car_IP(carip)
    {
        except_val.l = el;
        except_val.angle = eangle;
        except_val.lx = el*sin(except_val.angle);
        except_val.ly = el*cos(except_val.angle);
        recordPosition.open("./recordPosition",ios::out);
    }

    car_zyl(const car_zyl& other);

    car_zyl operator =(const car_zyl &other);

    void getUwbPosition(Point2D& the_point,const string filename);

    bool MoveToPoint(Point2D targetPoint,Point2D nowPoint);

    Point2D getCarPosition();

    Point2D getPosition();

    void carMovebyKeyboard();

    std::vector<std::string> _split(std::string str,std::string pattern);

    string getCarIP()
    {
        return this->car_IP;
    }

    int getcarid();

    bool whetherOK()
    {
        return this->formation;
    }

    bool stopCar()
    {
        return car_serial.Write(send_Back,10);
    }

    void moveRight()
    {
        car_serial.Write(send_Right,10);
        return;
    }

    void moveLeft()
    {
        car_serial.Write(send_Left,10);
        return ;
    }

    void moveUp()
    {
        car_serial.Write(send_Up,10);
        return ;
    }

    void moveDown()
    {
        car_serial.Write(send_Down,10);
        return;
    }

    void moveRight_1()
    {
        car_serial.Write(send_Right,10);
        usleep(30000);
        while(stopCar())
        {
            return ;
        }
    }

    void moveLeft_1()
    {
        car_serial.Write(send_Left,10);
        usleep(30000);
        while(stopCar())
        {
            return ;
        }
    }

    void moveUp_1()
    {
        car_serial.Write(send_Up,10);
        usleep(30000);
        while(stopCar())
        {
            return ;

        }
    }

    void moveDown_1()
    {
        car_serial.Write(send_Down,10);
        usleep(30000);
        while(stopCar())
        {
            return ;
        }
    }


    bool approimate(Point2D p1,Point2D p2,double len = 0.30)
    {
        if((p1.x - p2.x) < len && (p1.y - p2.y) < len)
        {
            return ((p1.x - p2.x) > -len) && ((p1.y - p2.y) > -len);
        }
        else
            return false;
    }

    void turnup_x(int n);

    void turnup_y(int n);

    void turndown_x(int n);

    void turndown_y(int n);

    void turnup_z(int n);

    void turndown_z(int n);

    void changeVelocity(int evx,int evy,int evz = 0);

    bool moveAtoB(Point2D A,Point2D B);

    bool moveToTarget(Point2D target);

    void new_carMovebyKeyboard();


};



#endif //CARMOVE_CAR_ZYL_H
