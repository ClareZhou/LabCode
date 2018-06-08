//
// Created by stone on 18-4-19.
//

#ifndef CARMOVE_INF_TYPEDEF_H
#define CARMOVE_INF_TYPEDEF_H


#define SLEEP usleep(500000);
typedef unsigned char uint8;
typedef unsigned short int uint16;
typedef unsigned long uint32;
typedef unsigned long long uint64;

typedef signed short int sint16;
typedef signed int s32;

const double Pi =  3.141592653;

struct Point2D{
    /*
     * 记录位置点
     */
    float x;
    float y;
    float z;

    Point2D()
    {
        this->x = 0;
        this->y = 0;
        this->z = 0;
    }

    Point2D(float _x, float _y,float _z = 0):x(_x),y(_y),z(_z)
    {

    }

    bool operator==(const Point2D& p)
    {
        if(this->x == p.x && this->y == p.y)
            return this->z == p.z;
        else
            return false;
    }

    bool operator<(const Point2D& p)
    {
        if(this->x < p.x)
            return this->y<p.y;
        else return (this->x + this->y) < (p.x + p.y);
    }

    void operator=(const Point2D& p)
    {
        this->x = p.x;
        this->y = p.y;
        this->z = p.z;
    }

    double operator-(const Point2D &other) const
    /*
     * 实现为两点之间的直线距离
     */
    {
        auto x_ = abs(this->x - other.x);
        auto y_ = abs(this->y - other.y);
        auto z_ = abs(this->z - other.z);
        return sqrt(pow(x_,2) + pow(y_,2) + pow(z_,2));
    }
};

/*
 * 下面定义内容来自"bsp/usart/usart1.h"
 */

#define USART1_RECEIVE_BUF_LENGTH 128
extern uint16 usart1_send_buf_length;
extern uint8 usart1_send_buf[];
extern uint16 usart1_receive_buf_length;
extern uint8 usart1_receive_buf[];
extern uint8 usart1_rx_irq_updata_user_reset_status;
extern uint8 usart1_tx_irq_updata_user_reset_status;


#define BYTE0(BYTE_TEMP)       ( *( char *)(&BYTE_TEMP))
#define BYTE1(BYTE_TEMP)       ( *( (char *)(&BYTE_TEMP) + 1))
#define BYTE2(BYTE_TEMP)       ( *( (char *)(&BYTE_TEMP) + 2))
#define BYTE3(BYTE_TEMP)       ( *( (char *)(&BYTE_TEMP) + 3))

#define Byte16(Type, ByteH, ByteL)  ((Type)((((uint16)(ByteH))<<8) | ((uint16)(ByteL))))

#define Byte32(Type, Byte4, Byte3, Byte2, Byte1)  ((Type)( (((uint32)(Byte4))<<24) | (((uint32)(Byte3))<<16) | (((uint32)(Byte2))<<8) | ((uint32)(Byte1))))




typedef struct
{
    float x;
    float y;
    float z;
}S_FLOAT_XYZ;

typedef struct
{
    signed short int x;
    signed short int y;
    signed short int z;
}S_INT16_XYZ;//s16

typedef struct
{
    signed int x;
    signed int y;
    signed int z;
}S_INT32_XYZ;//s32


typedef volatile struct {
    float q0;
    float q1;
    float q2;
    float q3;
} Quaternion;

typedef volatile struct {
    float M[9];
} Matrix9;

typedef volatile struct {
    float P,Pout;
    float I,Iout;
    float D,Dout;
    float Out;
    float Out_Limit;
    float Iout_Limit;
    float err;
}PID;

typedef volatile struct {
    S_FLOAT_XYZ P,Pout;
    S_FLOAT_XYZ I,Iout;
    S_FLOAT_XYZ D,Dout;
    S_FLOAT_XYZ Out;
    S_FLOAT_XYZ Iout_Limit;


}PID_XYZ;


#endif //CARMOVE_INF_TYPEDEF_H
