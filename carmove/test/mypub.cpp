//
// Created by zhouyuliang on 18-5-29.
//

#include"zhelpers.hpp"
#include<iostream>

using namespace std;

int main(int argc,char** argv)
{
    zmq::context_t context(1);
    zmq::socket_t publisher(context,ZMQ_PUB);
    publisher.bind("tcp://*:4444");
    if(argc>=3)
    {
        auto n = argv[2];
        while(n--)
        {
            s_send(publisher,argv[1]);
            cout<<"send msg :"<<argv[1]<<endl;
            sleep(3);
        }

    }
    else
    {
        cout<<"Please input 3 arg at least"<<endl;
    }
    return 0;
}
