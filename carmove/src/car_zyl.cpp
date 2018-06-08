//
// Created by caryzhou on 17-11-10.
//

#include "../include/car_zyl.h"
#include<iomanip>
#define setbit(x,y) x|=(1<<(y-1))
#define clrbit(x,y) x&=~(1<<(y-1))
#define zhifan(x,y) x^=(1<<(y-1))

void car_zyl::getUwbPosition(Point2D& the_point,const string filename)
/*
 * 通过搭载的car_uwb设备来获取x,y坐标，放入成员变量car_Position之中
 * 改进，可以增加一个确定uwblps串口已经打开的操作
 */
{
    uint8 buffer[512] = {0};
    while(1)
    {
        memset(buffer,0,512);
        int rd_num = car_uwb.readData(buffer);
        //cout<<rd_num<<endl;
        if(rd_num)
        {
            for(int i = 0;i<rd_num;i++)
            {
                if(buffer[i] == 0x55 && buffer[i+1] == 0x01)
                {

                    if(rd_num - i >127)
                    {
                        the_point = car_uwb.uwblps_Data_Convert2(buffer,i,filename);
                        this->car_Position = the_point;
                    }
                }
            }
        }
    }
}



void car_zyl::carMovebyKeyboard()
{
    char c;
    int input_num;
    initscr();

    cout<<"Move by keyboard!!"<<endl;

    while(1)
    {
        c = (char)getchar();
        input_num = static_cast<int>(c);
        switch(input_num)
            /*
             * 使用QWEASDZXC控制小车移动
             * R键退出
             */
        {
            case 122:
                car_serial.Write(send_L_D, 10);
                break;
            case 120:
                car_serial.Write(send_Down, 10);
                break;
            case 99:
                car_serial.Write(send_R_D, 10);
                break;
            case 97:
                car_serial.Write(send_Left, 10);
                break;
            case 115:
                car_serial.Write(send_Back, 10);
                break;
            case 100:
                car_serial.Write(send_Right, 10);
                break;
            case 113:
                car_serial.Write(send_L_U, 10);
                break;
            case 119:
                car_serial.Write(send_Up, 10);
                break;
            case 101:
                car_serial.Write(send_R_U, 10);
                break;
            case 114:
                cout<<"Finished move by keyboard!"<<endl;
                return;
            /*case 117:
                car_serial.Write(send_Turn_L1,10);
                //对左转的封装有不成功
            case 105:
                car_serial.Write(send_Turn_R1,10);*/
            default:
                break;
        }
    }
}


Point2D car_zyl::getCarPosition()
{

    return this->car_Position;

}


car_zyl::car_zyl(const car_zyl &other) {
    this->car_serial = other.car_serial;
    this->car_port = other.car_port;
    this->uwb_port = other.uwb_port;
    this->car_uwb = other.car_uwb;
    this->formation = other.formation;
    this->car_IP = other.car_IP;
    this->car_Position = other.car_Position;

    this->car_ID = other.car_ID;
    this->real_val = other.real_val;
    this->except_val = other.except_val;
    this->recordPosition.open("./recordPosition.txt",ios::out);
}

car_zyl car_zyl::operator=(const car_zyl &other) {
    this->car_serial = other.car_serial;
    this->car_port = other.car_port;
    this->uwb_port = other.uwb_port;
    this->car_uwb = other.car_uwb;
    this->formation = other.formation;
    this->car_IP = other.car_IP;
    this->car_Position = other.car_Position;

    this->car_ID = other.car_ID;
    this->real_val = other.real_val;
    this->except_val = other.except_val;
    return *this;
}

std::vector<std::string> car_zyl::_split(std::string str, std::string pattern) {
    std::string::size_type pos;
    std::vector<std::string> result;
    str+=pattern;//扩展字符串以方便操作
    int size=str.size();

    for(int i=0; i<size; i++)
    {
        pos=str.find(pattern,i);
        if(pos<size)
        {
            std::string s=str.substr(i,pos-i);
            result.push_back(s);
            i=pos+pattern.size()-1;
        }
    }
    return result;
}



bool car_zyl::MoveToPoint(Point2D targetPoint,Point2D nowPoint)
/*
 * 给定初始点和目的点，使小车自由移动到目的点去
 */
{
    int count = 0;
    if (approimate(targetPoint, nowPoint)) {
        this->stopCar();
        cout << "I am in the target Point" << endl;
        return true;
    }
    while (!approimate(targetPoint, nowPoint))
    {

        if (targetPoint.x - nowPoint.x > 0.30) {
            this->moveRight();
            usleep(500000);
            this->stopCar();
            cout << "moveRight" << endl;

        } else if (targetPoint.x - nowPoint.x < -0.30) {
            this->moveLeft();
            usleep(500000);
            this->stopCar();
            cout << "moveleft" << endl;

        }
        if (targetPoint.y - nowPoint.y > 0.30) {
            this->moveUp();
            usleep(500000);
            this->stopCar();
            cout << "moveUp" << endl;
            return false;
        } else if (targetPoint.y - nowPoint.y < -0.30) {
            this->moveDown();
            cout << "moveDown" << endl;
            usleep(500000);
            this->stopCar();
            return false;
        }
        count++;
        if (count > 100) {
            cout << "must have something wrong!!!" << endl;
            return false;
        }
    }
    cout << "already move to the target!" << endl;
    return true;
}


void car_zyl::turnup_x(int n = 3)
/*
 * 只调控速度的低8位好了；
 */
{

    while(n--)
    {
        vx[0]++;
    }


    //cout<<"vx[0] is :"<<vx[0]<<endl;
    if(vx[0] >= 0)
    {
        clrbit(cardir,3);
      //  cout<<"x轴应当为正方向"<<endl;
    } else
    {
        setbit(cardir,3);
        //cout<<"x轴应当为负方向"<<endl;
    }


    velocity[3] = static_cast<char>(vx[1]);
    velocity[4] = static_cast<char>(abs(vx[0]));
    velocity[9] = cardir;
    //cout<<"cardir is: "<<+static_cast<uint8_t >(cardir)<<endl;
    //cout<<"v9 is :"<<+static_cast<uint8_t >(velocity[9])<<endl;
    car_serial.Write(velocity,10);
}

void car_zyl::turndown_x(int n = 3) {


    while(n--)
    {
        vx[0]--;
    }

    //cout<<"vx[0] is :"<<vx[0]<<endl;
    if(vx[0] >= 0)
    {
        clrbit(cardir,3);
      //  cout<<"x轴应当为正方向"<<endl;
    } else
    {
        //cout<<"x轴应当为负方向"<<endl;
        setbit(cardir,3);

    }

    velocity[3] = static_cast<char >(vx[1]);
    velocity[4] = static_cast<char>(abs(vx[0]));
    velocity[9] = cardir;
    //cout<<"cardir is: "<<+static_cast<uint8_t >(cardir)<<endl;
    //cout<<"v9 is :"<<+static_cast<uint8_t >(velocity[9])<<endl;
    car_serial.Write(velocity,10);

}

void car_zyl::turnup_y(int n = 3) {

    while(n--)
    {
        vy[0]++;
    }

   // cout<<"vy[0] is :"<<vy[0]<<endl;
    if(vy[0] >= 0)
    {
        clrbit(cardir,2);
      //  cout<<"y轴应当为正方向"<<endl;
    } else
    {
   //     cout<<"y轴应当为负方向"<<endl;
        setbit(cardir,2);
    }

    velocity[5] = static_cast<char>(vy[1]);
    velocity[6] = static_cast<char>(abs(vy[0]));
    velocity[9] = cardir;
  //  cout<<"cardir is: "<<+static_cast<uint8_t >(cardir)<<endl;
   // cout<<"v9 is :"<<+static_cast<uint8_t >(velocity[9])<<endl;
    car_serial.Write(velocity,10);

}

void car_zyl::turndown_y(int n = 3) {


    while(n--)
    {
        vy[0]--;
    }

   // cout<<"vy[0] is :"<<vy[0]<<endl;
    if(vy[0] >= 0)
    {
   //     cout<<"y轴应当为正方向"<<endl;
        clrbit(cardir,2);

    } else
    {
     //   cout<<"x轴应当为负方向"<<endl;
        setbit(cardir,2);
    }


    velocity[5] = static_cast<char>(vy[1]);
    velocity[6] = static_cast<char>(abs(vy[0]));
    velocity[9] = cardir;
  //  cout<<"cardir is: "<<+static_cast<uint8_t >(cardir)<<endl;
   // cout<<"v9 is :"<<+static_cast<uint8_t >(velocity[9])<<endl;
    car_serial.Write(velocity,10);

}

void car_zyl::turnup_z(int n = 1)
/*
 * 向右转
 */
 {
    while(n--)
    {
        vz[0]++;
    }

  //  cout<<"vz[0] is :"<<vz[0]<<endl;
    if(vz[0] >= 0)
    {
        clrbit(cardir,1);
    //    cout<<"z轴应当为正方向"<<endl;
    } else
    {
    //    cout<<"z轴应当为负方向"<<endl;
        setbit(cardir,1);
    }

    velocity[7] = static_cast<char>(vz[1]);
    velocity[8] = static_cast<char>(abs(vz[0]));
    velocity[9] = cardir;
   // cout<<"cardir is: "<<+static_cast<uint8_t >(cardir)<<endl;
  //  cout<<"v9 is :"<<+static_cast<uint8_t >(velocity[9])<<endl;
    car_serial.Write(velocity,10);
}

void car_zyl::turndown_z(int n = 1)
/*
 * 向左转
 */
{
    while(n--)
    {
        vz[0]--;
    }

  //  cout<<"vz[0] is :"<<vz[0]<<endl;
    if(vz[0] >= 0)
    {
        clrbit(cardir,1);
   //     cout<<"z轴应当为正方向"<<endl;
    } else
    {
        //cout<<"z轴应当为负方向"<<endl;
        setbit(cardir,1);
    }

    velocity[7] = static_cast<char>(vz[1]);
    velocity[8] = static_cast<char>(abs(vz[0]));
    velocity[9] = cardir;
  //  cout<<"cardir is: "<<+static_cast<uint8_t >(cardir)<<endl;
  //  cout<<"v9 is :"<<+static_cast<uint8_t >(velocity[9])<<endl;
    car_serial.Write(velocity,10);
}

void car_zyl::new_carMovebyKeyboard()
{
    char c;
    int input_num;
    initscr();

    cout<<"Move by keyboard!!"<<endl;

    while(1)
    {
        c = (char)getchar();
        input_num = static_cast<int>(c);
/*        int tempx[2];
        int tempy[2];
        tempx[0] = static_cast<int>(vx[0]);
        tempy[0] = static_cast<int>(vy[0]);
        tempx[1] = static_cast<int>(vx[1]);
        tempy[1] = static_cast<int>(vy[1]);
        cout<<"Velocity X is "<<tempx[1]<<","<<tempx[0]<<endl;
        cout<<"Velocity y is "<<tempy[1]<<","<<tempy[0]<<endl;*/
        switch(input_num)
            /*
             * 使用QWEASDZXC控制小车移动
             * R键退出
             */
        {
            case 122:
                car_serial.Write(send_L_D, 10);
                break;
            case 120:
                turndown_y();
                break;
            case 99:
                car_serial.Write(send_R_D, 10);
                break;
            case 97:
                turndown_x();
                break;
            case 115:
                vx[0] = '\x00';
                vy[0] = '\x00';
                vz[0] = '\x00';
                car_serial.Write(send_Back, 10);
                break;
            case 100:
                turnup_x();
                break;
            case 113:
                car_serial.Write(send_L_U, 10);
                break;
            case 119:
                turnup_y();
                break;
            case 101:
                car_serial.Write(send_R_U, 10);
                break;
            case 114:
                cout<<"Finished move by keyboard!"<<endl;
                return;
            case 117://u键向左转
                turnup_z();
                break;
            case 105://i键向右转
                turndown_z();
                break;
            default:
                break;
        }
    }
}

void car_zyl::changeVelocity(int evx, int evy, int evz) {
    int stepx = 0,stepy = 0,stepz = 0;
    if(vx[0] >= evx && vy[0] >= evy)
    {

        stepx = (vx[0] - evx)/3;
        stepy = (vy[0] - evy)/3;
        while(stepx > 0 && stepy >0)
        {
            turndown_x();
            turndown_y();
            usleep(200000);
            stepx--;
            stepy--;
        }
        while(stepx > 0)
        {
            turndown_x();
            stepx--;
            usleep(200000);
        }
        while(stepy > 0)
        {
            turndown_y();
            stepy--;
            usleep(200000);
        }
    }
    else if(vx[0] >= evx && vy[0] < evy)
    {
        stepx = (vx[0] - evx)/3;
        stepy = (evy - vy[0])/3;
        while(stepx > 0 && stepy >0)
        {
            turndown_x();
            turnup_y();
            usleep(200000);
            stepx--;
            stepy--;
        }
        while(stepx > 0)
        {
            turndown_x();
            stepx--;
            usleep(200000);
        }
        while(stepy > 0)
        {
            turnup_y();
            stepy--;
            usleep(200000);
        }
    }else if(vx[0] < evx && vy[0] >= evy)
    {
        stepx = (evx - vx[0])/3;
        stepy = (vy[0] - evy)/3;
        while(stepx > 0 && stepy >0)
        {
            turnup_x();
            turndown_y();
            usleep(200000);
            stepx--;
            stepy--;
        }
        while(stepx > 0)
        {
            turnup_x();
            stepx--;
            usleep(200000);
        }
        while(stepy > 0)
        {
            turndown_y();
            stepy--;
            usleep(200000);
        }


    }else if(vx[0] < evx && vy[0] < evy)
    {
        stepx = (evx - vx[0])/3;
        stepy = (evy - vy[0])/3;
        while(stepx > 0 && stepy >0)
        {
            turnup_x();
            turnup_y();
            usleep(200000);
            stepx--;
            stepy--;
        }
        while(stepx > 0)
        {
            turnup_x();
            stepx--;
            usleep(200000);
        }
        while(stepy > 0)
        {
            turnup_y();
            stepy--;
            usleep(200000);
        }
    }
}

bool car_zyl::moveAtoB(Point2D A, Point2D B)
/*
 * A为当前机器人的位置，B为机器人的目标位置；
 */
{
    bool res = false;
    if(!approimate(A,B))
    {
        std::cout << "move from" << A.x << "," << A.y << " to " << B.x << "," << B.y << std::endl;
        int evx = 0,evy = 0,evz = 0;
        double distance = abs(A - B);
        double disx = A.x - B.x;
        double disy = A.y - B.y;
        auto absdisx = abs(disx);
        auto absdisy = abs(disy);

        if (absdisx - 0 < 0.0001)
        {

            if (distance >= 2.0)
                evy = 35;
            else if (distance < 2.0 && distance >= 0.3)
                evy = 20;
            else if (distance > 0.01 && distance < 0.3)
                evy = 5;
        }else if (absdisy - 0 < 0.0001)
        {
            if (distance >= 2.0)
                evx = 35;
            else if (distance < 2.0 && distance >= 0.3)
                evx = 20;
            else if (distance > 0.01 && distance < 0.3)
                evx = 5;
        }
        else if (absdisx - 0 > 0.0001 && absdisy - 0 > 0.0001)
        {
            double q = absdisy / absdisx;
            if (absdisx >= 2.0)
            {
                evx = 35;
                evy = static_cast<int> (q* evx);
                if (evy > 50)
                {
                    evy = 45;
                }
            }
            else if (absdisx < 2.0 && absdisx > 0.3)
            {
                evx = 20;
                evy = static_cast<int> (q* evx);
                if (evy > 30)
                {
                    evy = 30;
                }
            }
            else if (absdisx > 0.1&& absdisx < 0.3)
            {
                evx = 3;
                evy = static_cast<int> (q* evx);
                if (evy > 10)
                {
                    evy = 10;
                }
            }
            else if (absdisx > 0.0001 && absdisx < 0.1)
            {
                evx = 1;
                evy = static_cast<int> (q* evx);
                if (evy > 5)
                {
                    evy = 5;
                }
            }
        }
        if (disx < 0)
            evx = -evx;
        if (disy < 0)
            evy = -evy;

        std::cout << "ev : " << evx << "," << evy << endl;
        if(vx[0] != evx || vy[0] != evy)
            changeVelocity(evx, evy, evz);
    } else{
        std::cout<<"car: "<<getcarid()<<" have move to the target"<<std::endl;
        return true;
    }

    /*
    if(distance>=2.0)
    {
        evx = 25;
    }else if(distance < 2.0 && distance >=0.3)
    {
        evx = 15;
    }else if(distance > 0.0001 && distance < 0.3)
    {
        evx = 5;
    }
    double q = (absdisy / absdisx);
    evy = static_cast<int>(q * evx);
    if(evy > 40)
        evy = 40;
    if(disx > 0)
        evx = -evx;
    if(disy > 0)
        evy = -evy;
    changeVelocity(evx,evy,evz);
     */

    return res;
}

int car_zyl::getcarid() {
    return car_ID;
}

bool car_zyl::moveToTarget(Point2D target) {

    int evx = 0, evy = 0, evz = 0;
    if (!approimate(car_Position,target,0.50))
    {

        double distance = abs(target - car_Position);
        double disx = target.x - car_Position.x;
        double disy = target.y - car_Position.y;
        auto absdisx = abs(disx);
        auto absdisy = abs(disy);
        if (absdisx - 0 < 0.0001)
        {
            if (distance >= 2.0)
                evy = 35;
            else if (distance < 2.0 && distance >= 0.3)
                evy = 20;
            else if (distance > 0.01 && distance < 0.3)
                evy = 5;
        }else if (absdisy - 0 < 0.0001)
        {
            if (distance >= 2.0)
                evx = 35;
            else if (distance < 2.0 && distance >= 0.3)
                evx = 20;
            else if (distance > 0.01 && distance < 0.3)
                evx = 5;
        }
        else if (absdisx - 0 > 0.0001 && absdisy - 0 > 0.0001)
        {
            double q = absdisy / absdisx;
            if (absdisx >= 2.0)
            {
                evx = 35;
                evy = static_cast<int> (q* evx);
                if (evy > 50)
                {
                    evy = 45;
                }
            }
            else if (absdisx < 2.0 && absdisx > 0.5)
            {
                evx = 20;
                evy = static_cast<int> (q* evx);
                if (evy > 30)
                {
                    evy = 30;
                }
            }
            else if (absdisx > 0.1&& absdisx < 0.5)
            {
                evx = 3;
                evy = static_cast<int> (q* evx);
                if (evy > 10)
                {
                    evy = 10;
                }
            }
            else if (absdisx > 0.0001 && absdisx < 0.1)
            {
                evx = 5;
                evy = static_cast<int> (q* evx);
                if (evy > 10)
                {
                    evy = 10;
                }
            }

        }
        if (disx < 0)
            evx = -evx;
        if (disy < 0)
            evy = -evy;

        std::cout << "ev : " << evx << "," << evy << endl;
        if(vx[0] != evx || vy[0] != evy)
            changeVelocity(evx, evy, evz);

        SLEEP;
    }
    else
    {
        std::cout<<"car: "<<getcarid()<<" have move to the target"<<std::endl;
        return true;
    }
    return false;

}



Point2D car_zyl::getPosition() {


    return this->Position;
}

