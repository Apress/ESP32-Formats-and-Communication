void startServer()
{
  httpd_handle_t esp32_httpd = NULL;    // define esp32_httpd handler
  httpd_config_t config = HTTPD_DEFAULT_CONFIG();
  httpd_uri_t page_uri =        // link / URL  to page_handler
{.uri="/", .method = HTTP_GET, .handler = page_handler, .user_ctx = NULL};
  httpd_uri_t device_uri =        // link /update to device_handler
{.uri="/update",.method=HTTP_GET, .handler=device_handler, .user_ctx=NULL};
  if (httpd_start(&esp32_httpd, &config) == ESP_OK)
  {
    httpd_register_uri_handler(esp32_httpd, &page_uri);
    httpd_register_uri_handler(esp32_httpd, &device_uri);
  }
  config.server_port = 81;
}
