#include "lrs.h"


Action()
{

lrs_create_socket("socket0", "TCP", "LocalHost=0", "RemoteHost=SERVER:400", LrsLastArg);

lrs_send("socket0", "buf0", LrsLastArg);

lrs_receive("socket0", "buf1", LrsLastArg);

lrs_close_socket("socket0");

    return 0;
}
