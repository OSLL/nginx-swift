/*!
 * \file ngx_swift_module.h
 *
 * \date 04.11.2011
 * \author Dmitriy Mukhitov
 * \brief Модуль описывающий блок в конфигурации
 */

#ifndef NGX_SWIFT_MODULE_H_
#define NGX_SWIFT_MODULE_H_

#include <ngx_http.h>
#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_conf_file.h>
#include <ngx_http.h>


/*!
 * \brief Конфигурация
 */
typedef struct {
	ngx_http_upstream_conf_t  upstream; //!< конфигурация upstream
} ngx_swift_conf_t;


#endif /* NGX_SWIFT_MODULE_H_ */
