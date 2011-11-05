/*!
 * \file ngx_http_swift_module.c
 *
 * \date 04.11.2011
 * \author Dmitriy Mukhitov
 */

#include <stddef.h>
#include "ngx_swift_module.h"

static ngx_core_module_t ngx_swift_module_ctx = {
		ngx_string("swift"), NULL,
		NULL };

ngx_uint_t ngx_swift_max_module;
static char *ngx_swift_block(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);

static ngx_command_t ngx_swift_commands[] = {
		{ ngx_string("swift"),
		  NGX_MAIN_CONF | NGX_CONF_BLOCK | NGX_CONF_TAKE1,
		  ngx_swift_block, 0, 0, NULL },
	      ngx_null_command };

ngx_module_t ngx_swift_module = {
		NGX_MODULE_V1,
		&ngx_swift_module_ctx, /* module context */
		ngx_swift_commands, /* module directives */
		NGX_CORE_MODULE, /* module type */
		NULL, /* init master */
		NULL, /* init module */
		NULL, /* init process */
		NULL, /* init thread */
		NULL, /* exit thread */
		NULL, /* exit process */
		NULL, /* exit master */
		NGX_MODULE_V1_PADDING };

char *ngx_swift_block(ngx_conf_t *cf, ngx_command_t *cmd, void *conf) {
	char *rv;
	ngx_str_t *value;
	ngx_uint_t i;
	ngx_uint_t mi;
	ngx_conf_t pcf;
	ngx_swift_module_t *module;
	ngx_swift_conf_ctx_t *ctx;

	ctx = ngx_pcalloc(cf->pool, sizeof(ngx_swift_conf_ctx_t));
	if (ctx == NULL) {
		return NGX_CONF_ERROR;
	}

	*(ngx_swift_conf_ctx_t **) conf = ctx;

	ngx_swift_max_module = 0;
	for (i = 0; ngx_modules[i]; i++) {
		if (ngx_modules[i]->type != NGX_SWIFT_MODULE) {
			continue;
		}

		ngx_modules[i]->ctx_index = ngx_swift_max_module++;
	}

	value = cf->args->elts;

	ctx->name = value[1];

	ctx->conf = ngx_pcalloc(cf->pool, sizeof(void *) * ngx_swift_max_module);
	if (ctx->conf == NULL) {
		return NGX_CONF_ERROR;
	}

	for (i = 0; ngx_modules[i]; i++) {
		if (ngx_modules[i]->type != NGX_SWIFT_MODULE) {
			continue;
		}

		module = ngx_modules[i]->ctx;
		mi = ngx_modules[i]->ctx_index;

		if (module->create_conf) {
			ctx->conf[mi] = module->create_conf(cf);
			if (ctx->conf[mi] == NULL) {
				return NGX_CONF_ERROR;
			}
		}
	}

	pcf = *cf;
	cf->ctx = ctx;

	cf->module_type = NGX_SWIFT_MODULE;
	cf->cmd_type = NGX_SWIFT_CONF;

	rv = ngx_conf_parse(cf, NULL);

	*cf = pcf;

	if (rv != NGX_CONF_OK) {
		*cf = pcf;
		return rv;
	}

	for (i = 0; ngx_modules[i]; i++) {
		if (ngx_modules[i]->type != NGX_SWIFT_MODULE) {
			continue;
		}

		module = ngx_modules[i]->ctx;
		mi = ngx_modules[i]->ctx_index;

		if (module->init_conf) {
			rv = module->init_conf(cf, ctx->conf[mi]);
			if (rv != NGX_CONF_OK) {
				return rv;
			}
		}
	}
	return NGX_CONF_OK;
}
