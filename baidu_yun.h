#ifndef _BAIDU_YUN_H
#define _BAIDU_YUN_H
#include "curl_utils.h"

#define BDY_API_BASE "https://pan.baidu.com"
#define BDY_API_USER_INFO "/rest/2.0/xpan/nas?method=uinfo&openapi=xpansdk"
/* /rest/2.0/xpan/nas?method=uinfo&openapi=xpansdk */
http_response *bdy_user_info();
http_response *bdy_quota();
http_response *bdy_file_list();
http_response *bdy_list_doc();
http_response *bdy_list_img();
http_response *bdy_list_vido();
http_response *bdy_list_bt();
http_response *bdy_category_info();
#endif // !#ifndef _BAIDU_YUN_H
