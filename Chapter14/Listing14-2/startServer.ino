void startServer()
{
  httpd_handle_t stream_httpd = NULL;   // stream_httpd handler
  httpd_config_t config = HTTPD_DEFAULT_CONFIG();
  httpd_uri_t stream_uri =        // link /stream to stream_handler
{.uri="/stream", .method=HTTP_GET, .handler=stream_handler,.user_ctx=NULL};
  config.server_port = 81;
  config.ctrl_port = config.ctrl_port + 1;    // required for streaming
  if (httpd_start(&stream_httpd, &config) == ESP_OK)
    httpd_register_uri_handler(stream_httpd, &stream_uri);
}
