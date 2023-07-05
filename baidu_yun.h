#ifndef _BAIDU_YUN_H
#define _BAIDU_YUN_H
#include "curl_utils.h"

#define BDY_API_BASE "https://pan.baidu.com"
#define BDY_API_USER_INFO "/rest/2.0/xpan/nas?method=uinfo&openapi=xpansdk"

#define ERR_LIST_NOT_AUTH -7
#define ERR_LIST_NOT_EXIST -9


/* /rest/2.0/xpan/nas?method=uinfo&openapi=xpansdk */
http_response *bdy_user_info();
http_response *bdy_quota();
http_response *bdy_file_list();
http_response *bdy_list_doc();
http_response *bdy_list_img();
http_response *bdy_list_vido();
http_response *bdy_list_bt();
http_response *bdy_category_info();
http_response *bdy_search(const char *search);
http_response *bdy_meta(int64_t fid, int32_t dlink);
http_response *bdy_download(const char *dlink, int64_t size,
                            const char *down_local_path);
#endif // !#ifndef _BAIDU_YUN_H
