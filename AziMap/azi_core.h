#ifndef _AZI_COREH
//azi_core.h
//120103

#include <math.h>

//マクロazi_calcの定義

//与えられる情報
//	目的地が西以外ならtrue、西ならfalse
//	北極点から現在地までの角度のサイン、コサイン(splt,cplt)
//	移動する方角のコサイン(cth)、移動角のサイン、コサイン(srad,crad)
//得られる情報
//	北極点から目的地までの緯度差(dlt)、現在地から目的地までの経度差(dlg)
//                        ~~~~~~~~~~~                        ~~~~~~~~~~~

#define PI 3.14159265358979323846

#define azi_calc(dirEW,splt,cplt,cth,srad,crad,dlt,dlg) \
	do {\
		/*dltの算出*/\
		double cdlt = splt*srad*cth+cplt*crad;\
		if(cdlt > 1)\
			cdlt = 1;\
		else if(cdlt < -1)\
			cdlt = -1;\
		dlt = acos(cdlt);\
		/*dlgの算出*/\
		if(cplt == 1){/*現在地が北極点*/\
			if(dirEW)\
				dlg = PI - acos(cth);\
			else\
				dlg = acos(cth) - PI;\
		}else if(cplt == -1){/*現在地が南極点*/\
			if(dirEW)\
				dlg = acos(cth);\
			else\
				dlg = -1*acos(cth);\
		}else if(cth == 1){ /*目的地が真北*/\
			if(splt*crad-cplt*srad >= 0)\
				dlg = 0;\
			else\
				dlg = PI;\
		}else if(cth == -1){ /*目的地が真南*/\
			if(splt*crad+cplt*srad >= 0)\
				dlg = 0;\
			else\
				dlg = PI;\
		}else{ /*その他*/\
			double sdlt = sqrt(1-cdlt*cdlt);\
			double buf = (crad-cplt*cdlt)/(splt*sdlt);\
			if(buf > 1)\
				buf = 1;\
			else if(buf < -1)\
				buf = -1;\
			if(dirEW)\
				dlg = acos(buf);\
			else\
				dlg = (-1)*acos(buf);\
		}\
	}while(0)



#define _AZI_COREH
#endif
