 
 
# include "ini.h"

//GetPrivateProfileStringA
template <> std::string IniPar::Read<std::string>( LPCSTR key )
{
	char buf[1024];
	DWORD w = GetPrivateProfileStringA( section_, key, "", buf, sizeof(buf), fn_ ) ;
	assert( w );
	return w? buf : "";
}

template <> int IniPar::Read<int>( LPCSTR key )
{
	return atoi( Read<std::string>( key ).c_str() );
}

template <> bool IniPar::Read<bool>( LPCSTR key )
{
	return Read<int>( key );
}

IniPar::IniPar( LPCSTR fn, LPCSTR section ):fn_(fn),section_(section?section:"test") 
{
	# define _(A) A = Read<std::string>( #A )
	_( thr_) ; // 81_ice_age2 
     # undef	 _
	
# define _(A) A = Read<int>( #A )
	_( MaxFrames );
	_(  begin_);
	_( 	routine_) ; // 8 
	_( 	show_) ; // 8 
	_( 	MorphMeasure);
	 
# undef	 _
# define _(A) A = Read<std::string>( #A )
	_( RootDir) ; // 81_ice_age2 
	_( NewDir) ; //'D:\g\ExperimentsData\120510-q3dME-ForInpaintingIceAge\test01\81_ice_age2\'
	 
# undef	 _

 
}