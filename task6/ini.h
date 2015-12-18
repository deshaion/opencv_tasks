# pragma once
#pragma once
# define _STDAFX3_H
//#define WIN32_LEAN_AND_MEAN   






#include <stdio.h>
#include <tchar.h>
#include <windows.h>

#ifdef _CH_
#pragma package <opencv>
#endif

#define CV_NO_BACKWARD_COMPATIBILITY

//#ifndef _EiC
#include "opencv\cv.h"
#include "opencv\highgui.h"
//#endif

#include <math.h>
#include <stdlib.h>
#include <limits>
#include <set>
#include <vector>
#include <deque>
#include <map>
#include <iostream>
#include <functional>
#include <algorithm>
#include <utility>
#include <numeric>
#include <string>
# include <list>

struct IniPar { 
	std::string 		 
		RootDir, //'D:\g\ExperimentsData\120510-q3dME-ForInpaintingIceAge\test01\81_ice_age2\'
		//
		NewDir  
		;
	//
	int
		MaxFrames, // 43
		// frames to process
		routine_,
		medSize // frames for mediana
	;
	std::string 
		thr_
		;
	 
//	IniPar(){	}
	IniPar( LPCSTR fn, LPCSTR section=NULL );
	template <typename  T> T Read( LPCSTR key );
	const LPCSTR fn_, section_ ;
	 
};
