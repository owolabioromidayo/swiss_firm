#include "storage.h"

void init_storage(void){
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK( err );

}


int get_string_value_nvs(const char* key, char* out_value, size_t length){
    //get string value from non-volatile storage

    int ret_val;
    nvs_handle_t my_handle; // open nvs handle
    esp_err_t err = nvs_open("storage", NVS_READWRITE, &my_handle);

    if (err == ESP_OK) {
        err = nvs_get_str(my_handle, key, out_value, (size_t *)length);
        ret_val = (err == ESP_OK) ? STORAGE_OK : STORAGE_ERR;
    }else{
        ret_val = STORAGE_ERR;
    } 

    nvs_close(my_handle);
    return ret_val;
}


int set_string_value_nvs(const char* key, char* value){
    //set string value in non-volatile storage   

        int ret_val;
        nvs_handle_t my_handle; // open nvs handle
        esp_err_t err = nvs_open("storage", NVS_READWRITE, &my_handle);

       
        if(err == ESP_OK){
            esp_err_t err = nvs_set_str(my_handle, key, value);
             if(err == ESP_OK){
                err = nvs_commit(my_handle);   //commit value to ensure changes
                ret_val = (err == ESP_OK) ? STORAGE_OK : STORAGE_ERR;
             }else{
                ret_val = STORAGE_ERR;
            }
        }else{
            ret_val = STORAGE_ERR;
        }

        nvs_close(my_handle);
        return ret_val;
}

//handling our larger blobs / working on our cache comes next



