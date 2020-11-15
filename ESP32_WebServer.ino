#include "esp_http_server.h"
#include <WiFi.h>

const char* ssid = "*********";
const char* password = "*********";

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();

  // WiFi: WiFi接続用クラス (WiFi connection class).
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  startServer();

  Serial.print("Ready! Use 'http://");
  Serial.print(WiFi.localIP());
  Serial.println("' to connect");
}

void loop() {
  delay(10000);
}

// Empty handle to http_server
// - httpd_handle_t: サーバインスタンスへのハンドル (Handle to server instance)
httpd_handle_t server_httpd = NULL;

// HTTPサーバ起動用関数 (Function for starting the webserver)
void startServer(){
    // Define URI handlers
    // - httpd_uri_t: URIハンドラ(URI handler)
    httpd_uri_t index_uri = {
        .uri       = "/",
        .method    = HTTP_GET,
        .handler   = index_handler,     // ハンドラ関数 (handler function)
        .user_ctx  = NULL
    };

    // Generate default configuration
    // - httpd_config_t: HTTPサーバー構成用構造体 (HTTP Server Configuration Structure)
    // - HTTPD_DEFAULT_CONFIG(): 初期化用データ (initialize the configuration)
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    // HTTPサーバの起動 (Starts the web server)
    // - httpd_start(): HTTPサーバーのインスタンス生成、メモリ/リソースの割り当て (Create an instance of HTTP server and allocate memory/resources)
    httpd_start(&server_httpd, &config);

    // URIハンドラの登録 (Registers a URI handler)
    // - httpd_register_uri_handler(): 
    httpd_register_uri_handler(server_httpd, &index_uri);
}

// HTML
static char html[] = "<div><img></div>"
"<input type=\"button\" value=\"capture\">"
"<script>"
"document.querySelector('[value=capture]').addEventListener('click', function() { location.href = '/capture' });"
"setTimeout(function() { document.querySelector('img').src = 'http://' + location.host + ':81/stream' }, 1000);"
"</script>";

esp_err_t index_handler(httpd_req_t *req){
    httpd_resp_set_type(request, "text/html; charset=UTF-8");
    return httpd_resp_send(req, html, sizeof(html));
}
