#ifndef _BAIDY_DEF_H
#define _BAIDY_DEF_H
// https://pan.baidu.com/union/doc/okumlx17r
// ERROR_CODE
#define BDY_OK 0
#define BDY_ERR_NOT_AUTH -6
#define BDY_ERR_PARAM 2
#define BDY_ERR_PARAM2 31023
#define BDY_ERR_TOKEN_INVALID 111
#define BDY_ERR_NOT_ALLOW 6
#define BDY_ERR_FREQ 31034
#define BDY_ERR_SHARE_NOT_FOUND 2131
#define BDY_ERR_TARGET_ALREADY_EXIST 10
#define BDY_ERR_FILE_NOT_EXIST -3
#define BDY_ERR_FILE_NOT_EXIST2 -31066
#define BDY_ERR_SELF_SEND_SHARE 11
#define BDY_ERR_SHARE_TOO_MUCH 255
#define BDY_ERR_BATCH_SHARE 12
#define BDY_ERR_RIGHT_EXPIRED -1


const char *bdy_get_error_msg(int error_code) {
  switch (error_code) {
  case BDY_OK:
    return "Success";
  case BDY_ERR_PARAM:
  case BDY_ERR_PARAM2:
    return "invalid parameter";
  case BDY_ERR_NOT_AUTH:
    return "not authorized";
  case BDY_ERR_TOKEN_INVALID:
    return "token invalid";
  case BDY_ERR_NOT_ALLOW:
    return "not allowed";
  case BDY_ERR_FREQ:
    return "not allowed";
  case BDY_ERR_SHARE_NOT_FOUND:
    return "share not found";
  case BDY_ERR_TARGET_ALREADY_EXIST:
    return "target already exist";
  case BDY_ERR_FILE_NOT_EXIST:
    return "file not exist";
  case BDY_ERR_FILE_NOT_EXIST2:
    return "file not exist";
  case BDY_ERR_SELF_SEND_SHARE:
    return "self send share";
  case BDY_ERR_SHARE_TOO_MUCH:
    return "share too much";
  case BDY_ERR_BATCH_SHARE:
    return "batch share";
  case BDY_ERR_RIGHT_EXPIRED:
    return "right expired";
  default:
    return "Unknown error";
  }
}
#endif
