//2023-08-16 11:34:39 - WIFI WEB + HTTP POST
        // C++ example, http_post HTML form to ESP32-C3 HTTP WebServer over Wifi
        // serverIP 192.168.4.1
        // URI GET : 192.168.4.1/config
        // URI POST : 192.168.4.1/config
        //
        // sdkconfig file might need to change 512 values to 1024 to avoid errors
        // HTTPD_MAX_REQ_HDR_LEN 1024 //2023-08-05 18:12:22 - Bug error is :  Header fields are too long !!
        // CONFIG_HTTPD_MAX_URI_LEN 1024 //2023-08-05 18:35:24 - Error Uri to long !!
        //
        // Don't use #include <iostream>  # It gives error Failed.. partition table ...
        //
        //
        //   WiFi softAP Example
        // This example code is in the Public Domain (or CC0 licensed, at your option.)
        // Unless required by applicable law or agreed to in writing, this
        // software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
        // CONDITIONS OF ANY KIND, either express or implied.
        //
        //
        //
        //// -------------------------------------  Usage  ----------------------------------------------------------------------
        // extern "C" void app_main(void)
        // {        
        //    static httpd_handle_t server = NULL;
        //    //Initialize NVS
        //    esp_err_t ret = nvs_flash_init();
        //    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        //      ESP_ERROR_CHECK(nvs_flash_erase());
        //      ret = nvs_flash_init();
        //    }
        //    ESP_ERROR_CHECK(ret);
        //    ESP_LOGI(TAG, "ESP_WIFI_MODE_AP");
        //    wifi_init_softap();
        //    ESP_ERROR_CHECK(esp_netif_init());
        //    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_AP_STAIPASSIGNED, &connect_handler, &server));
        // //    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_STA_DISCONNECTED, &disconnect_handler, &server));
        // }
//End




#include <stdio.h>
#include <map>
#include <string.h>
#include <stdlib.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_mac.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"


#include "lwip/err.h"
#include "lwip/sys.h"


#include "esp_err.h"
#include <sys/param.h>
#include "nvs_flash.h"
#include "esp_netif.h"
#include <esp_http_server.h>


#include "driver/gpio.h"


//2023-08-02 22:46:38 - Global vars :
#define CONFIG_ESP_WIFI_SSID "espssid"
#define CONFIG_ESP_WIFI_CHANNEL 1  //Range 1-13
#define CONFIG_ESP_WIFI_PASSWORD "esppassword"
#define CONFIG_ESP_MAX_STA_CONN 5
//#define HTTPD_MAX_REQ_HDR_LEN 1024 //2023-08-05 18:12:22 - Header fields are too long !!
// #undef CONFIG_HTTPD_MAX_URI_LEN
//#define CONFIG_HTTPD_MAX_URI_LEN 1024 //2023-08-05 18:35:24 - Error Uri to long !!


#include <sstream>
//#include <iostream>
#include <stdio.h>
#include <algorithm> //2023-06-24 15:32:37 - Needed to call std::find()

#include <vector>
#include <map>
// #include <deque>

using namespace std;

string serverIP = "192.168.4.1";

#define LED GPIO_NUM_2



#include "html_content.h" //2023-08-16 16:59:12 - File HTML



/************************ URL Encode utils *******************/
//2023-08-05 22:18:02 - ESP_utils.h maybe
void yield(void) {
  vTaskDelay(pdMS_TO_TICKS(2));
}

unsigned char h2int(char c)
{
    if (c >= '0' && c <='9'){
        return((unsigned char)c - '0');
    }
    if (c >= 'a' && c <='f'){
        return((unsigned char)c - 'a' + 10);
    }
    if (c >= 'A' && c <='F'){
        return((unsigned char)c - 'A' + 10);
    }
    return(0);
}

//url_decode
string urldecode(string str)
{

    string encodedString="";
    char c;
    char code0;
    char code1;
    for (int i =0; i < str.length(); i++){
        c=str.at(i);
      if (c == '+'){
        encodedString+=' ';
      }else if (c == '%') {
        i++;
        code0=str.at(i);
        i++;
        code1=str.at(i);
        c = (h2int(code0) << 4) | h2int(code1);
        encodedString+=c;
      } else{

        encodedString+=c;
      }

      yield();
    }//next

   return encodedString;
}//urlDecode

// url_encode
string urlencode(string str)
{
    string encodedString="";
    char c;
    char code0;
    char code1;
    //char code2;
    for (int i =0; i < str.length(); i++){
      c=str.at(i);
      if (c == ' '){
        encodedString+= '+';
      } else if (isalnum(c)){
        encodedString+=c;
      } else{
        code1=(c & 0xf)+'0';
        if ((c & 0xf) >9){
            code1=(c & 0xf) - 10 + 'A';
        }
        c=(c>>4)&0xf;
        code0=c+'0';
        if (c > 9){
            code0=c - 10 + 'A';
        }
        //code2='\0';
        encodedString+='%';
        encodedString+=code0;
        encodedString+=code1;
        //encodedString+=code2;
      }
      yield();
    }//next

    //encodedString+='\0';
    return encodedString;

}//urlEncode




/********************* Parsing Stuff ***************************/

// 2023-08-25 19:46:27 - Parse content header
map<string, string> readContentHeaders(string contents_var)
{
   map<string, string> htable;
   vector<string> lines = explode(contents_var, '\n');
   for (string line : lines)
   {
       // cout<<"readContentHead line=["<<line<<"]"<<endl;
       vector<string> value_pair = explode(line, ':');
       if (value_pair.size() >= 2)
       {
           htable[value_pair[0]] = value_pair[1];
           //cout << value_pair[0] << "=>" << value_pair[1] << endl;
       }
       else
       {
           // content-header malformed ?
       }
   } // next
   return htable;
} // readContentHeaders


//2023-08-30 13:08:25 - readFormData (XHTTP POST Request Chrome)
//2023-08-30 13:59:24 - CRLF for ESP32-c3
map<string, string> readFormData(string webkitBoundaryStr)
{
   map<string, string> htable;
   // Identifier la position du premier "\n\n" => pos_2LN
   size_t pos_LN = webkitBoundaryStr.find("\r\n");
   size_t pos_3LN = webkitBoundaryStr.find("\r\n\r\n");
   if(pos_3LN == string::npos)
   {
       cout<<endl<<"\n\nAouch, pos_3LN was not found !"<<endl;

       size_t pos_3LN = webkitBoundaryStr.find("data");   
    if(pos_3LN == string::npos)
    {
        cout<<endl<<"\n\nAouch, pos_3LN data was not found !"<<endl;
    }else cout <<"Aouch data found"<<endl;
   }
    
  //  cout << "pos_LN CRLF=" << pos_LN << endl;
  //  cout << "pos_3LN=" << pos_3LN << endl;

   // Récupérer la première ligne (l'identification)
   string post_identifier = webkitBoundaryStr.substr(0, pos_LN);
   //cout << "post_identifier=[" << post_identifier << "]" << endl;
   // int pos_identifier = webkitBoundaryStr.find(post_identifier);
   // cout<<"pos_identifier=["<<pos_identifier<<"]"<<endl;

   // post_identifier = "------WebKitFormBoundaryV3AGE0d6Us3yo3hA"
   // identifier la pos. du post_identifier_end = "------WebKitFormBoundaryV3AGE0d6Us3yo3hA--" => pos_ident_end
   int pos_identifier_end = webkitBoundaryStr.rfind(post_identifier); //use rfind instead of find_last_of adds length to the position, this is not reliable  
   //cout << "pos_identifier_end=[" << pos_identifier_end << "]" << endl;
   // - Parser chaque ligne de content-xxxx présente, et récupérer name="command"
   // string content_vars = webkitBoundaryStr.substr( post_identifier.length(), );
   int from = post_identifier.length() + 1; //+1 => + "\n"
   string content_vars = webkitBoundaryStr.substr(from, pos_3LN - from);
   //cout << "content_vars=[" << content_vars << "]" << endl;
   // 2023-08-25 19:53:48 Parse content headers
   map<string, string> headers = readContentHeaders(content_vars);
   htable["Content-Disposition"] = "";
   if (headers.count("Content-Disposition"))
   {
       cout<<"Content Disposition Found !!"<<endl;
       htable["Content-Disposition"] = headers["Content-Disposition"];
       //Rechercher la commande "command"
      //  if( headers["Content-Disposition"].find("name=\"command\"") != string::npos )
      //  {
      //     cout<<"command name OK"<<endl;
      //  }else
      //  {
      //     cout<<"Command name not OK"<<endl;
      //  }
   }
   else
   {
       cout << "Oups, no Content-Disposition !?" << endl;
   }
   from = pos_3LN+3; //\n\n length
   string post_content =  webkitBoundaryStr.substr(from, pos_identifier_end - from -1); //-1 to remove the last \n
   //cout<<"PAYLOAD["<<post_content<<"]"<<endl;
   htable["content"] = post_content;   
   return htable;
} // readFormData


/******************  WEBSEVER CODE BEGINS ***********************/

esp_err_t http_404_error_handler(httpd_req_t *req, httpd_err_code_t err)
{
   /* For any other URI send 404 and close socket */
   httpd_resp_send_err(req, HTTPD_404_NOT_FOUND, "Some 404 error message");
   return ESP_FAIL;
}

// Gestionnaire d'URI pour une requête GET /config
esp_err_t get_handler(httpd_req_t *req)
{
    // définit le type de contenu HTTP
    if (httpd_resp_set_type(req, "text/html") == ESP_OK)
    {
        // ajoute des en-têtes supplémentaires
        httpd_resp_set_hdr(req, "Content-Encoding", "identity");
    }

    map <string,string> server_side_vars;
    server_side_vars["{{TITLE}}"] = "ESPeed Web Server HTTP <em class=\"info\">(click here to load example)</em>";
    server_side_vars["{{POST_URI}}"] = "/config";// full uri not working : "/"+serverIP + "/config"

    //2023-08-16 17:16:44 - Browse map to replace variable with there content :
    for( auto m:server_side_vars)
    {
      replaceStr(HTML_CONTENT, m.first, m.second);
    }//next

    //ESPeed Web Server HTTP   
    httpd_resp_send(req, HTML_CONTENT.c_str(), HTTPD_RESP_USE_STRLEN);

    return ESP_OK;
}//get_handler

// Gestionnaire d'URI pour une requête POST /test
esp_err_t post_handler(httpd_req_t *req)
{
    ESP_LOGI(TAG, "Longueur contenu POST : %d", req->content_len);
    char content[req->content_len];
    // tronque si la longueur du contenu est supérieure au buffer
    size_t recv_size = MIN(req->content_len, sizeof(content));

    int ret = httpd_req_recv(req, content, recv_size);

    // déconnecté ?
    if (ret <= 0)
    {
        // timeout ?
        if (ret == HTTPD_SOCK_ERR_TIMEOUT)
        {
            // retourne une erreur HTTP 408 (Request Timeout)
            httpd_resp_send_408(req);
        }

        return ESP_FAIL;
    }

    //ESP_LOGI(TAG, "Contenu POST : [%s]", content);

    // envoie une réponse
    //const char resp[] = "Test Reponse POST OK";
    string decodedStr = urldecode(content);
    string sResp = "Réponse post du serv ESP32: "+decodedStr;
    httpd_resp_send(req, sResp.c_str() , HTTPD_RESP_USE_STRLEN);

    // //2023-08-21 20:32:31 - Response Example, when post from XHTTP request
    // ------WebKitFormBoundaryV3AGE0d6Us3yo3hA
    // Content-Disposition: form-data; name="command"
    // Content-Stuff: some line stuff

    // CONTENT-HERE
    // ------WebKitFormBoundaryV3AGE0d6Us3yo3hA--

    map<string,string> htable = readFormData(decodedStr);

    //Analyze sent command
    if( htable["Content-Disposition"].find("name=\"data\"") != string::npos )
    {
      cout<<"Command DATA OK"<<endl;      
      //Save config to EPROM nvs

      
       
    }//data
    
    if( htable["Content-Disposition"].find("name=\"restart\"") != string::npos )
    {
      cout<<"Command restart OK"<<endl;
      ESP_LOGI(TAG, "esp_restart() !! [%s]", htable["content"].c_str() );
      esp_restart();
    }//restart
    
    //ESP_LOGI(TAG, "htable[content] POSTed : [%s]", htable["content"].c_str() ); //Give a stack overflow error
    //cout<<"htable content:"<< htable["content"]<<endl;

    return ESP_OK;
}//post_handler



static httpd_handle_t start_webserver(void)
{
   httpd_handle_t server = NULL;
   httpd_config_t config = HTTPD_DEFAULT_CONFIG();
   config.lru_purge_enable = true;

    // pour une requête GET /test
    httpd_uri_t uri_get = {
        .uri      = "/config",
        .method   = HTTP_GET,
        .handler  = get_handler,
        .user_ctx = NULL
    };

    // pour une requête POST /test
    httpd_uri_t uri_post = {
        .uri      = "/config",
        .method   = HTTP_POST,
        .handler  = post_handler,
        .user_ctx = NULL
    };

   // Start the httpd server
   ESP_LOGI(TAG, "Starting server on port: '%d'", config.server_port);
   if (httpd_start(&server, &config) == ESP_OK) {
       // Set URI handlers
       ESP_LOGI(TAG, "Registering URI handlers");
       httpd_register_uri_handler(server, &uri_get);
       httpd_register_uri_handler(server, &uri_post);

       return server;
   }


   ESP_LOGI(TAG, "Error starting server!");
   return NULL;
}


// static void stop_webserver(httpd_handle_t server)
// {
//    // Stop the httpd server
//    httpd_stop(server);
// }


// static void disconnect_handler(void* arg, esp_event_base_t event_base,
//                               int32_t event_id, void* event_data)
// {
//    httpd_handle_t* server = (httpd_handle_t*) arg;
//    if (*server) {
//        ESP_LOGI(TAG, "Stopping webserver");
//        stop_webserver(*server);
//        *server = NULL;
//    }
// }



static void connect_handler(void* arg, esp_event_base_t event_base,
                           int32_t event_id, void* event_data)
{
   if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) 
    {
        if(demandeConnexion)
        {
            ESP_LOGI(TAG, "Connexion ...");
            esp_wifi_connect();
        }
    } 
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) 
    {
        // reconnexion ?
        ESP_LOGI(TAG, "Deconnexion !");
    } 
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) 
    {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(TAG, "Adresse IP : " IPSTR, IP2STR(&event->ip_info.ip));

        //2023-10-22 18:26:38 - Try starting web server
        httpd_handle_t* server = (httpd_handle_t*) arg;
        if (*server == NULL) {
            ESP_LOGI(TAG, "Starting webserver");
            *server = start_webserver();
        }
    }
}//connect_handler


/******************* WEBSERVER CODE ENDS ************************/





/* The examples use WiFi configuration that you can set via project configuration menu.
  If you'd rather not, just change the below entries to strings with
  the config you want - ie #define EXAMPLE_WIFI_SSID "mywifissid"
*/
#define EXAMPLE_ESP_WIFI_SSID      CONFIG_ESP_WIFI_SSID
#define EXAMPLE_ESP_WIFI_PASS      CONFIG_ESP_WIFI_PASSWORD
#define EXAMPLE_ESP_WIFI_CHANNEL   CONFIG_ESP_WIFI_CHANNEL
#define EXAMPLE_MAX_STA_CONN       CONFIG_ESP_MAX_STA_CONN


static void wifi_event_handler(void* arg, esp_event_base_t event_base,
                                   int32_t event_id, void* event_data)
{
   if (event_id == WIFI_EVENT_AP_STACONNECTED) {
       wifi_event_ap_staconnected_t* event = (wifi_event_ap_staconnected_t*) event_data;
       ESP_LOGI(TAG, "station " MACSTR " join, AID=%d",
                MAC2STR(event->mac), event->aid);
   } else if (event_id == WIFI_EVENT_AP_STADISCONNECTED) {
       wifi_event_ap_stadisconnected_t* event = (wifi_event_ap_stadisconnected_t*) event_data;
       ESP_LOGI(TAG, "station " MACSTR " leave, AID=%d",
                MAC2STR(event->mac), event->aid);
   }
}//wifi_event_handler


//2023-08-17 20:01:04 - Entry point for Wifi 
void wifi_init_softap(void)
{
    ESP_LOGI(TAG, "wifi_init_softap(), esp_netif_init");
   ESP_ERROR_CHECK(esp_netif_init());
   // ESP_LOGI(TAG, "esp_event_loop_create_default");
   ESP_ERROR_CHECK(esp_event_loop_create_default());
   // ESP_LOGI(TAG, "esp_netif_create_default_wifi_ap");
   esp_netif_create_default_wifi_ap();


    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_LOGI(TAG, "WIFI_INIT_CONFIG_DEFAULT");
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_LOGI(TAG, "esp_event_handler_instance_register");
   ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                       ESP_EVENT_ANY_ID,
                                                       &wifi_event_handler,
                                                       NULL,
                                                       NULL));

    ESP_LOGI(TAG, "memset wifi_config_t");

   wifi_config_t wifi_config;
   memset(&wifi_config,0,sizeof(wifi_config_t) );

    memcpy( wifi_config.ap.ssid,  EXAMPLE_ESP_WIFI_SSID, strlen(EXAMPLE_ESP_WIFI_SSID) );
    wifi_config.ap.ssid_len = strlen(EXAMPLE_ESP_WIFI_SSID);
    wifi_config.ap.channel = EXAMPLE_ESP_WIFI_CHANNEL;
    memcpy(wifi_config.ap.password, EXAMPLE_ESP_WIFI_PASS, strlen(EXAMPLE_ESP_WIFI_PASS) );
    wifi_config.ap.max_connection = EXAMPLE_MAX_STA_CONN;
    wifi_config.ap.authmode = WIFI_AUTH_WPA_WPA2_PSK;


   if (strlen(EXAMPLE_ESP_WIFI_PASS) == 0) {
       wifi_config.ap.authmode = WIFI_AUTH_OPEN;
   }

    ESP_LOGI(TAG, "esp_wifi_set_mode");
   ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    
    ESP_LOGI(TAG, "esp_wifi_set_config");
   ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_config));
    ESP_LOGI(TAG, "esp_wifi_start");

   ESP_ERROR_CHECK(esp_wifi_start());

   ESP_LOGI(TAG, "wifi_init_softap finished. SSID:%s password:%s channel:%d",
            EXAMPLE_ESP_WIFI_SSID, EXAMPLE_ESP_WIFI_PASS, EXAMPLE_ESP_WIFI_CHANNEL);
}//wifi_init_softap






//2023-08-16 11:38:09 - Usage
void wifi_http_server(void)
{
   static httpd_handle_t server = NULL;

   //Initialize NVS
   esp_err_t ret = nvs_flash_init();
   if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
     ESP_ERROR_CHECK(nvs_flash_erase());
     ret = nvs_flash_init();
   }
   ESP_ERROR_CHECK(ret);


   ESP_LOGI(TAG, "ESP_WIFI_MODE_AP");
   wifi_init_softap();


   ESP_ERROR_CHECK(esp_netif_init());


   ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_AP_STAIPASSIGNED, &connect_handler, &server));
//    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_STA_DISCONNECTED, &disconnect_handler, &server));
}
