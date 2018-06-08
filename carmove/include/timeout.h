//
// Created by stone on 18-4-19.
//

#ifndef CARMOVE_TIMEOUT_H
#define CARMOVE_TIMEOUT_H
#include <ctime>

class _timeout
{
private:

    clock_t m_time;
    int m_maxTime;
    char m_state;

public:

    _timeout(int time);
    void start(void);
    char end(void);
};
#endif //CARMOVE_TIMEOUT_H
