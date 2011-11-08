/*!
 * \file ngx_http_swift_module.c
 *
 * \date 04.11.2011
 * \author Dmitriy Mukhitov
 */

#include "ngx_swift_module.h"
#include <stdlib.h>
#include <stddef.h>

static char* ngx_http_swift(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);

static void *
ngx_http_swift_create_loc_conf(ngx_conf_t *cf) {
	ngx_log_debug(NGX_LOG_INFO, cf->log, 0, "ngx_http_swift_create_loc_conf");
	ngx_swift_conf_t *conf;
	conf = ngx_pcalloc(cf->pool, sizeof(ngx_swift_conf_t));

	if (conf == NULL) {
		return NULL;
	}

	/*
	 * set by ngx_pcalloc():
	 *
	 *     conf->upstream.bufs.num = 0;
	 *     conf->upstream.next_upstream = 0;
	 *     conf->upstream.temp_path = NULL;
	 *     conf->upstream.uri = { 0, NULL };
	 *     conf->upstream.location = NULL;
	 */

	conf->upstream.connect_timeout = NGX_CONF_UNSET_MSEC;
	conf->upstream.send_timeout = NGX_CONF_UNSET_MSEC;
	conf->upstream.read_timeout = NGX_CONF_UNSET_MSEC;

	conf->upstream.buffer_size = NGX_CONF_UNSET_SIZE;

	/* the hardcoded values */
	conf->upstream.cyclic_temp_file = 0;
	conf->upstream.buffering = 0;
	conf->upstream.ignore_client_abort = 0;
	conf->upstream.send_lowat = 0;
	conf->upstream.bufs.num = 0;
	conf->upstream.busy_buffers_size = 0;
	conf->upstream.max_temp_file_size = 0;
	conf->upstream.temp_file_write_size = 0;
	conf->upstream.intercept_errors = 1;
	conf->upstream.intercept_404 = 1;
	conf->upstream.pass_request_headers = 0;
	conf->upstream.pass_request_body = 0;

	return conf;

}

static char *
ngx_http_swift_merge_loc_conf(ngx_conf_t *cf, void *parent, void *child)
{
	ngx_log_debug(NGX_LOG_INFO, cf->log, 0, "ngx_http_swift_merge_loc_conf");
	ngx_swift_conf_t *prev = parent;
	ngx_swift_conf_t *conf = child;

    ngx_conf_merge_msec_value(conf->upstream.connect_timeout,
                              prev->upstream.connect_timeout, 60000);

    ngx_conf_merge_msec_value(conf->upstream.send_timeout,
                              prev->upstream.send_timeout, 60000);

    ngx_conf_merge_msec_value(conf->upstream.read_timeout,
                              prev->upstream.read_timeout, 60000);

    ngx_conf_merge_size_value(conf->upstream.buffer_size,
                              prev->upstream.buffer_size,
                              (size_t) ngx_pagesize);

    ngx_conf_merge_bitmask_value(conf->upstream.next_upstream,
                              prev->upstream.next_upstream,
                              (NGX_CONF_BITMASK_SET
                               |NGX_HTTP_UPSTREAM_FT_ERROR
                               |NGX_HTTP_UPSTREAM_FT_TIMEOUT));

    if (conf->upstream.next_upstream & NGX_HTTP_UPSTREAM_FT_OFF) {
        conf->upstream.next_upstream = NGX_CONF_BITMASK_SET
                                       |NGX_HTTP_UPSTREAM_FT_OFF;
    }

    if (conf->upstream.upstream == NULL) {
        conf->upstream.upstream = prev->upstream.upstream;
    }

    return NGX_CONF_OK;
}

static ngx_command_t ngx_http_swift_commands[] = {
		{ ngx_string("swift"),
		NGX_HTTP_SRV_CONF | NGX_HTTP_LOC_CONF | NGX_CONF_NOARGS,
		ngx_http_swift,
		NGX_HTTP_LOC_CONF_OFFSET,
		0,
		NULL },
		ngx_null_command };

static ngx_http_module_t ngx_http_swift_module_ctx = {
		NULL, /* preconfiguration */
		NULL, /* postconfiguration */
		NULL, /* create main configuration */
		NULL, /* init main configuration */
		NULL, /* create server configuration */
		NULL, /* merge server configuration */
		ngx_http_swift_create_loc_conf, /* create location configuration */
		ngx_http_swift_merge_loc_conf /* merge location configuration */};

ngx_module_t ngx_http_swift_module = {
		NGX_MODULE_V1,
		&ngx_http_swift_module_ctx, /* module context */
		ngx_http_swift_commands, /* module directives */
		NGX_HTTP_MODULE, /* module type */
		NULL, /* init master */
		NULL, /* init module */
		NULL, /* init process */
		NULL, /* init thread */
		NULL, /* exit thread */
		NULL, /* exit process */
		NULL, /* exit master */
		NGX_MODULE_V1_PADDING };

static ngx_int_t ngx_http_swift_create_request(ngx_http_request_t *r) {
	ngx_log_debug(NGX_LOG_INFO, r->connection->log, 0, "ngx_http_swift_create_request");
	return NGX_OK;
}
static ngx_int_t ngx_http_swift_reinit_request(ngx_http_request_t *r) {
	ngx_log_debug(NGX_LOG_INFO, r->connection->log, 0, "ngx_http_swift_reinit_request");
	return NGX_OK;
}
static ngx_int_t ngx_http_swift_process_header(ngx_http_request_t *r) {
	ngx_log_debug(NGX_LOG_INFO, r->connection->log, 0, "ngx_http_swift_process_header");

	return NGX_OK;
}
static void ngx_http_swift_abort_request(ngx_http_request_t *r) {
	ngx_log_debug(NGX_LOG_INFO, r->connection->log, 0, "ngx_http_swift_abort_request");
}

static void ngx_http_swift_finalize_request(ngx_http_request_t *r, ngx_int_t rc) {
	ngx_log_debug(NGX_LOG_INFO, r->connection->log, 0, "ngx_http_swift_finalize_request");
}
static ngx_int_t ngx_http_swift_handler(ngx_http_request_t *r) {
	ngx_log_debug(NGX_LOG_INFO, r->connection->log, 0,
			"ngx_http_swift_handler");

	ngx_http_upstream_t* upstream = ngx_pcalloc(r->pool, sizeof(ngx_http_upstream_t));
    if (upstream == NULL) {
        return NGX_HTTP_INTERNAL_SERVER_ERROR;
    }

    upstream->output.tag = (ngx_buf_tag_t) &ngx_http_swift_module;
	upstream->peer.log = r->connection->log;
	upstream->peer.log_error = NGX_ERROR_ERR;

	if (ngx_http_upstream_create(r) != NGX_OK) {
		return NGX_HTTP_INTERNAL_SERVER_ERROR;
	}

	ngx_swift_conf_t *mlcf = ngx_http_get_module_loc_conf(r, ngx_http_swift_module);

	upstream->conf = &mlcf->upstream;

	upstream->create_request = ngx_http_swift_create_request;
	upstream->reinit_request = ngx_http_swift_reinit_request;
	upstream->process_header = ngx_http_swift_process_header;
	upstream->abort_request = ngx_http_swift_abort_request;
	upstream->finalize_request = ngx_http_swift_finalize_request;

	r->upstream = upstream;

	ngx_http_upstream_init(r);
	return NGX_DONE;
}

static char *
ngx_http_swift(ngx_conf_t *cf, ngx_command_t *cmd, void *conf) {
	ngx_log_debug(NGX_LOG_INFO, cf->log, 0, "ngx_http_swift");
	ngx_http_core_loc_conf_t *clcf;
	clcf = ngx_http_conf_get_module_loc_conf(cf, ngx_http_core_module);
	clcf->handler = ngx_http_swift_handler;
	return NGX_CONF_OK;
}
