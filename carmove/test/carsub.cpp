//
// Created by zhouyuliang on 18-5-29.
//

#include "zhelpers.hpp"
#include<iostream>
using namespace std;

const string ip = "192.168.199.219";

int main () {
    //  Prepare our context and subscriber
    zmq::context_t context(1);
    zmq::socket_t subscriber (context, ZMQ_SUB);
    subscriber.connect("tcp://"+ ip +":4444");
    subscriber.setsockopt( ZMQ_SUBSCRIBE, "", 0);

    while (1) {

        //  Read envelope with address
        std::string address = s_recv (subscriber);
        if(address == "A")
        {
            cout<<"this is A"<<endl;
            system("bash ./run.sh");
            zmq_sleep(10);
            continue;
        }
        else if(address == "B")
        {
            system("exit");
        }
    }
    context.close();
    return 0;

}
