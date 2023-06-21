#ifndef _BAIDU_YUN_H
#define _BAIDU_YUN_H

#define BDY_API_BASE "https://pan.baidu.com"
#define BDY_API_USER_INFO "/rest/2.0/xpan/nas?method=uinfo&openapi=xpansdk"
/* /rest/2.0/xpan/nas?method=uinfo&openapi=xpansdk */
int bdy_user_info();
int bdy_quota();
int bdy_file_list();
int bdy_list_doc();
int bdy_list_img();
int bdy_list_vido();
int bdy_list_bt();
int bdy_category_info();
#endif // !#ifndef _BAIDU_YUN_H
