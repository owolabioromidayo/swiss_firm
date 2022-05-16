#include "wifi.h"
#include "storage.h"
#include "publish.h"
#include <stdio.h>




void app_main(void)
{
    printf("IN MAIN FUNC \n");
    init_storage();
    if (init_wifi() == WIFI_ERR){
        print("Error initializing Wifi");
        return;
    }
    send_post(62.8);
}
