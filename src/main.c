/*Info*/
// writen by Stijn Stuiver
// 12/2/2024
/*Standerized includes "esp libaries"  and standard c libs*/
#include <stdio.h>
#include <string.h>

//esp-idf
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"

/*libs that are imported*/


//Variables
#define WIFI_SUCCESS 1 << 0
#define WIFI_FAILURE 1 << 1
#define TCP_SUCCESS 1 << 0
#define TCP_FAILURE 1 << 1
#define MAX_FAILURES 10

// event group to contain status information
static EventGroupHandle_t wifi_event_group;

// retry tracker
static int s_retry_num = 0;

// task tag
static const char *TAG = "WIFI";
/*Includes from the include folder*/

/*Funtions and classes*/
//event handler for wifi events
static void wifi_event_handler(void* arg, esp_event_base_t event_base,
                                int32_t event_id, void* event_data)
{
	if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START)
	{
		ESP_LOGI(TAG, "Connecting to AP...");
		esp_wifi_connect();
	} else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)
	{
		if (s_retry_num < MAX_FAILURES)
		{
			ESP_LOGI(TAG, "Reconnecting to AP...");
			esp_wifi_connect();
			s_retry_num++;
		} else {
			xEventGroupSetBits(wifi_event_group, WIFI_FAILURE);
		}
	}
}
// connect to wifi and retern the result
esp_err_t connectWifi(){
  int status = WIFI_FAILURE;
  /**INITIALIZE ALL THE NETWORK INTERFACE**/
  //setting up the esp netwerk interface
  ESP_ERROR_CHECK(esp_netif_init());

  //setting up the basic loop
  ESP_ERROR_CHECK(esp_event_loop_create_default());

  //create wifu station in the wifi driver
  esp_netif_create_default_wifi_sta();

  //setup wifi station with the default wifi configuration
  wifi_init_config_t WifiConfig = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK(esp_wifi_init(&WifiConfig));

  /** EVENT LOOP **/
  wifi_event_group = xEventGroupCreate();

  esp_event_handler_instance_t wifi_handler_event_instance;
  ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                      ESP_EVENT_ANY_ID,
                                                      &wifi_event_handler,
                                                      NULL,
                                                      &wifi_handler_event_instance));
  



  

}

void app_main() 
{

}