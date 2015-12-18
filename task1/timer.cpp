
#include "common.h"
#include "timer.h"

MyTimer::MyTimer(): value(0)  
{     
	//Eсли система не поддерживает работу со 
    //счетчиком высокого разрешения, то выкинется именно это исключение.  
    //Для уточнения лучше смотреть GetLastError()  
    QueryPerformanceFrequency(&performance_frequency);  
}  
void MyTimer::start() {         
	QueryPerformanceCounter(&time1);  
}  
double MyTimer::stop() {  
	QueryPerformanceCounter(&time2);  
	value = ((double)(time2.QuadPart - time1.QuadPart))/performance_frequency.QuadPart;   
	return value;  
} 
 