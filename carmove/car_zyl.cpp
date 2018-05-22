//
// Created by caryzhou on 17-11-10.
//

#include "car_zyl.h"
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
                    }
                }
            }
        }
    }
}



void car_zyl::sendMsg(const string &pubport)
/*
 * 从端口5555发布坐标信息
 */
{
    cout<<"start send Position Message"<<endl;
    zmq::context_t context(1);
    zmq::socket_t publisher(context,ZMQ_PUB);
    publisher.bind("tcp://*:"+pubport);

    while(1)
    {
        zmq::message_t message(20);

        Point2D tempPosition = this->car_Position.front();
        snprintf(static_cast<char*>(message.data()),20,"%f %f",(tempPosition.x),(tempPosition.y));
        publisher.send(message);
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

void car_zyl::recvMsg(car_zyl car1,const string &subport)
{
    zmq::context_t context(1);
    zmq::socket_t subscriber (context, ZMQ_SUB);
    subscriber.connect("tcp://"+ car1.car_IP +":"+subport);
    subscriber.setsockopt( ZMQ_SUBSCRIBE, "", 0);
    cout<<"start recv Position Message"<<endl;
    while(1)
    {
        std::vector<string> res;
        Point2D tempPosition;
        string contents = s_recv(subscriber);
        res = _split(contents," ");
        tempPosition.x = stof(res[0]);
        tempPosition.y = stof(res[1]);
        this->leaderposition.push_back(tempPosition);
        if(this->leaderposition.size() >100)
            for (int count = 0; count < 10; ++count)
                this->leaderposition.pop_back();
    }
}

void car_zyl::followerMove(car_zyl leader)
/*
 * 跟随者做跟随运动，接受端口为7777
 */
{
    /*
    recvMsg(leader,"7777");//转换成线程的形式

    this->car_Position.push_back(getCarPosition());//转换成线程的形式
    */
    cout<<"Follower start follow "<<endl;
    Point2D tempLeaderPosition,tempfollowerPosition;

    tempLeaderPosition = this->leaderposition.front();
    tempfollowerPosition = this->car_Position.front();
    this->car_Position.pop_back();
    this->leaderposition.pop_back();

    this->real_val.lx = tempLeaderPosition.x - tempfollowerPosition.x;
    this->real_val.ly = tempLeaderPosition.y - tempfollowerPosition.y;
    this->real_val.l = pow((pow(this->real_val.lx,2) + pow(this->real_val.ly,2)),0.5);
    this->real_val.angle = atan(this->real_val.lx/this->real_val.ly);

    while(1)
    {
        if((real_val.lx - except_val.lx) < -0.1)
            this->moveLeft_1();
        else if((real_val.lx - except_val.lx) > 0.1)
            this->moveRight_1();

        if((real_val.ly - except_val.ly) < -0.1)
            this->moveDown_1();
        else if((real_val.ly - except_val.ly) >0.1)
            this->moveUp_1();
    }
    return ;
}

void car_zyl::setLeaderPosition(Point2D leaderPosition) {
    this->leaderposition.push_back(leaderPosition);

    return ;
}

bool car_zyl::formationOK()
/*
 * 实现有点问题
 */
{
    this->formation = false;

    Point2D leadPosition,followerPosition;
    leadPosition = this->leaderposition.front();
    followerPosition = this->car_Position.front();

    if((abs(leadPosition.x - followerPosition.x)) < 0.1 && (abs(leadPosition.y - followerPosition.y) <0.1))
    {
        this->formation = true;
        return true;
    }
    else
    {
        this->formation = false;
        return false;
    }

}

void car_zyl::getCarPosition(vector<Point2D>& Position)
/*
 * 开启一个线程用于持续获取小车的位置坐标
 */
{

    Position = this->car_Position;
    return;
}


void car_zyl::getCarPosition(Point2D& Position)
/*
 * 开启一个线程用于持续获取小车的位置坐标
 * 在调用该函数前应该调用getUwbPosition这个函数
 */

{
    //thread t1(&car_zyl::getUwbPosition,this);
    vector<Point2D> tempPosition;

    while(1)
    {
        if(car_Position.size()>499)
            tempPosition = car_Position;
        sort(tempPosition.begin(),tempPosition.end());
        Position = tempPosition[249];
    }
    return;
}



vector<Point2D> car_zyl::getCarPosition()
{

    return this->car_Position;
    /*
    vector<Point2D> Position;
    thread t1(getUwbPosition,this);

    while(this->car_Position.size() <= 100)
    {
        continue;
    }
    cout<<"Position size is :"<<this->car_Position.size()<<endl;
    if(this->car_Position.size())
        Position = car_Position;
    return Position;
*/
}

void car_zyl::move_until_formationOK(car_zyl &leader,const string &OK_port)
{
    cout<<"Follower start FormationOK move!"<<endl;
    this->formation = false;

    Point2D tempLeaderPosition,tempfollowerPosition;
    tempLeaderPosition = this->leaderposition.front();
    tempfollowerPosition = this->car_Position.front();

    this->real_val.lx = tempLeaderPosition.x - tempfollowerPosition.x;
    this->real_val.ly = tempLeaderPosition.y - tempfollowerPosition.y;
    this->real_val.l = pow((pow(this->real_val.lx,2) + pow(this->real_val.ly,2)),0.5);
    this->real_val.angle = atan(this->real_val.lx/this->real_val.ly);

    while(!this->formation)
    {
        if((real_val.lx - except_val.lx) < -0.1)
            this->moveLeft_1();
        else if((real_val.lx - except_val.lx) > 0.1)
            this->moveRight_1();

        if((real_val.ly - except_val.ly) < -0.1)
            this->moveDown_1();
        else if((real_val.ly - except_val.ly) >0.1)
            this->moveUp_1();

        if ((abs(this->real_val.lx - this->except_val.lx)) < 0.1)
        {
            this->formation = (abs(this->real_val.ly - this->except_val.ly) < 0.1);
        }
        else
        {
            this->formation = false;
        }
    }

    while(this->formation)
    {
        zmq::context_t context(1);
        zmq::socket_t publisher(context,ZMQ_PUB);
        publisher.bind("tcp://*:"+OK_port);
        int flag = 1;
        zmq::message_t message(10);
        snprintf(static_cast<char*>(message.data()),10,"%d",flag);
        if(publisher.send(message))
        {
            cout<<"Follower already send FormationOK!!!!";
            return;
        }

    }
    return ;
}

bool car_zyl::wait_until_formationOK(car_zyl &follower,const string &OK_port)
{
    this->formation = false;
    zmq::context_t context(1);
    zmq::socket_t subscriber (context, ZMQ_SUB);
    subscriber.connect("tcp://"+ follower.car_IP +":"+OK_port);
    subscriber.setsockopt( ZMQ_SUBSCRIBE, "", 0);
    cout<<"Wait until formation"<<endl;
    while(!this->formation)
    {
        vector<string>res;
        string contents = s_recv(subscriber);
        res = _split(contents," ");
        int result = stoi(res[0]);
        if(1 == result)
        {
            this->formation = true;
            return true;
        }
    }
}

car_zyl::car_zyl(const car_zyl &other) {
    this->car_serial = other.car_serial;
    this->car_port = other.car_port;
    this->uwb_port = other.uwb_port;
    this->car_uwb = other.car_uwb;
    this->formation = other.formation;
    this->car_IP = other.car_IP;
    this->car_Position = other.car_Position;
    this->leaderposition = other.leaderposition;
    this->car_ID = other.car_ID;
    this->real_val = other.real_val;
    this->except_val = other.except_val;
}

car_zyl car_zyl::operator=(const car_zyl &other) {
    this->car_serial = other.car_serial;
    this->car_port = other.car_port;
    this->uwb_port = other.uwb_port;
    this->car_uwb = other.car_uwb;
    this->formation = other.formation;
    this->car_IP = other.car_IP;
    this->car_Position = other.car_Position;
    this->leaderposition = other.leaderposition;
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
/*

void car_zyl::moveToPoint(Point2D t)
{
    while(car_Position.size() > 49)
    {
        if(nowPosition.x != 0 && nowPosition.y != 0)
        {

            if(t.x - nowPosition.x > 0.1)
                moveRight_1();
            else if(t.x - nowPosition.x < -0.1)
                moveLeft_1();
            if(t.y - nowPosition.y > 0.1)
                moveUp_1();
            else if(t.y - nowPosition.y < -0.1)
                moveUp_1();
            continue;
        }
        else
        {
            sleep(1);
            continue;
        }
        cout<<"循环内结束"<<endl;
    }
    cout<<"没有进入move循环"<<endl;
    return;
}
*/



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


void car_zyl::run_ground()
{
    for(int i = 0;i<620;i++)
    {
        moveUp_1();
    }

    for(int i = 0;i<620;i++)
    {
        moveRight_1();
    }
    for(int i = 0;i<620;i++)
    {
        moveDown_1();
    }

    for(int i = 0;i<620;i++)
    {
        moveLeft_1();
    }
}

void car_zyl::Dynamic_target_area(Point2D target, Point2D nowPosition, const string &filename)
{
    ofstream this_log;
    this_log.open("./"+filename,ios::app);
    assert(this_log.is_open());

    if(abs(target.x - nowPosition.x) < 0.30)
    {
        if(abs(target.y - nowPosition.y) < 0.30)
        {
            this->stopCar();
            this_log<<"I am in the target area!";
            this_log<<"targetPoint is :"<<target.x<<","<<target.y
                    <<"   nowPosition is : "<<nowPosition.x<<","<<nowPosition.y<<endl;

        }
    }
    if((target.x - nowPosition.x ) > 0.30)
    {
        this->moveRight();
        usleep(100000);
        this_log<<"move right";
        this_log<<"targetPoint is :"<<target.x<<","<<target.y
                <<"   nowPosition is : "<<nowPosition.x<<","<<nowPosition.y<<endl;
    }else if((target.x - nowPosition.x) < -0.30)
    {
        this->moveLeft();
        usleep(100000);
        this_log<<"move left";
        this_log<<"targetPoint is :"<<target.x<<","<<target.y
                <<"   nowPosition is : "<<nowPosition.x<<","<<nowPosition.y<<endl;
    }
    if((target.y - nowPosition.y) > 0.30)
    {
        this->moveUp();
        usleep(100000);
        this_log<<"move up ";
        this_log<<"targetPoint is :"<<target.x<<","<<target.y
                <<"   nowPosition is : "<<nowPosition.x<<","<<nowPosition.y<<endl;
    } else if((target.y - nowPosition.y) < -0.30)
    {
        this->moveDown();
        usleep(100000);
        this_log<<"move down";
        this_log<<"targetPoint is :"<<target.x<<","<<target.y
                <<"   nowPosition is : "<<nowPosition.x<<","<<nowPosition.y<<endl;
    }
    return;
}

void car_zyl::recordPosition(const Point2D& tag1,const Point2D& tag2, ofstream &log)
{
//    log_mid.open("./logMid.txt",ios::app);
    assert(log.is_open());
    Point2D midPoint = {0,0};
    while(1)
    {
        midPoint.x = (tag1.x + tag2.x)/2;
        midPoint.y = (tag1.y + tag2.y)/2;

        log<<"midPosition is :"<<midPoint.x<<","<<midPoint.y<<endl;
    }


}

void car_zyl::moveToPointDoubleTag(const Point2D &target, const Point2D &tag1, const Point2D &tag2,ofstream &log) {
    bool flag = false;
    Point2D midPoint = {0,0};
//cout<<"1111"<<endl;
//    assert(log.is_open());
//cout<<"22222"<<endl;
    while(!flag)
    {
//cout<<"333"<<endl;
        midPoint.x = (tag1.x + tag2.x)/2;
        midPoint.y = (tag1.y + tag2.y)/2;
        cout<<"midPoint is :"<<midPoint.x<<","<<midPoint.y<<endl;

        log<<"midPoint is :"<<midPoint.x<<","<<midPoint.y<<endl;
//cout<<"555"<<endl;
        flag =this->MoveToPoint(target,midPoint);
    }
//cout<<"444"<<endl;
    log<<"I have moved to the target!"<<endl;

}

/*void car_zyl::turnup_x(int n = 1) {
    int temp[2];
    stringstream ss;

    temp[0] = vx[0];
    temp[1] = vx[1];


    while(n--)
    {
        if(cardir & 0x04)//属于x轴的正方向
        {
            cout<<"属于x轴的正方向"<<endl;
            temp[0]++;
            if(temp[0] >=255)
            {
                temp[0] = 0;
                temp[1]++;
            }
        } else//属于x轴的负方向
        {
            cout<<"属于x轴的负方向"<<endl;
            if(temp[0] > 0)
            {
                temp[0] --;

            }else if(temp[1] > 0)
            {
                temp[1]--;
                temp[0] = 255;

            }
            else if((temp[0] == 0&& temp[1] == 0)&&(cardir & 0x04) == 0)
            {
                cardir ^= (1<<(3-1));
            }
        }
    }
    vx[0] = static_cast<char>(temp[0]);
    vx[1] = static_cast<char>(temp[1]);


    velocity[3] = vx[1];
    velocity[4] = vx[0];
    car_serial.Write(velocity,10);
}*/

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
