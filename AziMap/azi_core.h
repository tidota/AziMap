#ifndef _AZI_COREH
//azi_core.h
//120103

#include <math.h>

//�}�N��azi_calc�̒�`

//�^��������
//	�ړI�n�����ȊO�Ȃ�true�A���Ȃ�false
//	�k�ɓ_���猻�ݒn�܂ł̊p�x�̃T�C���A�R�T�C��(splt,cplt)
//	�ړ�������p�̃R�T�C��(cth)�A�ړ��p�̃T�C���A�R�T�C��(srad,crad)
//��������
//	�k�ɓ_����ړI�n�܂ł̈ܓx��(dlt)�A���ݒn����ړI�n�܂ł̌o�x��(dlg)
//                        ~~~~~~~~~~~                        ~~~~~~~~~~~

#define PI 3.14159265358979323846

#define azi_calc(dirEW,splt,cplt,cth,srad,crad,dlt,dlg) \
	do {\
		/*dlt�̎Z�o*/\
		double cdlt = splt*srad*cth+cplt*crad;\
		if(cdlt > 1)\
			cdlt = 1;\
		else if(cdlt < -1)\
			cdlt = -1;\
		dlt = acos(cdlt);\
		/*dlg�̎Z�o*/\
		if(cplt == 1){/*���ݒn���k�ɓ_*/\
			if(dirEW)\
				dlg = PI - acos(cth);\
			else\
				dlg = acos(cth) - PI;\
		}else if(cplt == -1){/*���ݒn����ɓ_*/\
			if(dirEW)\
				dlg = acos(cth);\
			else\
				dlg = -1*acos(cth);\
		}else if(cth == 1){ /*�ړI�n���^�k*/\
			if(splt*crad-cplt*srad >= 0)\
				dlg = 0;\
			else\
				dlg = PI;\
		}else if(cth == -1){ /*�ړI�n���^��*/\
			if(splt*crad+cplt*srad >= 0)\
				dlg = 0;\
			else\
				dlg = PI;\
		}else{ /*���̑�*/\
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
