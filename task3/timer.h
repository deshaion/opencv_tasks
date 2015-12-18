# pragma once

class MyTimer  
{  
    LARGE_INTEGER time1, time2, performance_frequency;  
    double value;  
   
public:  
    MyTimer();
    void start();
    double stop();
}; 