//
// Created by stone on 18-4-19.
//


#include "timeout.h"
#include <stdio.h>

_timeout::_timeout(int time)
{
    m_maxTime = time;
    m_state = 0;
}

void _timeout::start(void)
{
    m_time = clock();
    m_state = 1;
}

char _timeout::end(void)
{
    if(m_state)
    {
        if(((clock() - m_time) / (double) (CLOCKS_PER_SEC / 1000)) >= m_maxTime)
        {
            m_state = 0;
            return 0;
        }
        else
        {
            return 1;
        }
    }
    else
    {
        return 0;
    }
}