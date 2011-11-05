/*!
 * \file ngx_swift_module.h
 *
 * \date 04.11.2011
 * \author Dmitriy Mukhitov
 * \brief Модуль описывающий блок в конфигурации
 */

#ifndef NGX_SWIFT_MODULE_H_
#define NGX_SWIFT_MODULE_H_

#include <ngx_config.h>
#include <ngx_core.h>

#define NGX_SWIFT_MODULE        0x5357494654  /* "SWIFT" */
#define NGX_SWIFT_CONF          0x02000000
#define NGX_SWIFT_CONF_OFFSET   offsetof(ngx_swift_conf_ctx_t, conf)


/*!
 * \brief Контекст модуля
 */
typedef struct {
    ngx_str_t   name; //!<имя
    void      **conf; //!<конфигурация
} ngx_swift_conf_ctx_t;

/*!
 * \brief Конфигурация
 */
typedef struct {
} ngx_swift_core_conf_t;

/*!
 * \brief Модуль
 */
typedef struct {
    void       *(*create_conf)(ngx_conf_t *cf);
    char       *(*init_conf)(ngx_conf_t *cf, void *conf);
} ngx_swift_module_t;

#endif /* NGX_SWIFT_MODULE_H_ */
