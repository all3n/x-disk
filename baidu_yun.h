#ifndef _BAIDU_YUN_H
#define _BAIDU_YUN_H

#define BDY_API_BASE "https://pan.baidu.com"
#define BDY_API_USER_INFO "/rest/2.0/xpan/nas?method=uinfo&openapi=xpansdk"
/* /rest/2.0/xpan/nas?method=uinfo&openapi=xpansdk */
int get_user_info();
int get_quota();
#endif // !#ifndef _BAIDU_YUN_H
