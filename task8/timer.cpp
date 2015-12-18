
#include "common.h"
#include "timer.h"

MyTimer::MyTimer(): value(0)  
{     
	//E��� ������� �� ������������ ������ �� 
    //��������� �������� ����������, �� ��������� ������ ��� ����������.  
    //��� ��������� ����� �������� GetLastError()  
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
 