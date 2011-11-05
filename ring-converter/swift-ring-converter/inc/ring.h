/*!
 * ring.h
 *
 *  Created on: 15.10.2011
 *      Author: dmitriy
 */

#ifndef RING_H_
#define RING_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
/*!
 * \brief Описание устройств
 */
typedef struct device {
	uint64_t m_id; //!< Индекс в списке устройств
	uint64_t m_zone; //!< Зона нахождения устройства
	double m_weight; //!< Удельный вес устройства
	char* m_ip; //!< ip адресс сервера, содержащий устройство
	uint64_t m_port; //!< TCP порт, для подключения к серверу
	char* m_device; //!< Название устройства на сервере
	char* m_meta; //!< Дополнительная мета информация
} Device;

struct part2id{
	uint64_t* m_devId;
	uint64_t m_count;
};

/*!
 * \brief Кольцо
 */
typedef struct ring {
	Device* m_device;
	uint64_t m_devCount;
	uint64_t m_partShift; //!< смещение в MD5 хеше
	struct part2id* m_replica2part2dev_id;
	uint64_t m_replicaCount;
} Ring;


/*!
 * \brief Освобождение памяти
 * @param rng
 */
void destroy(Ring* rng);

Ring load(const char* const fileName);

void save(const char* const fileName, const Ring* rng);

bool equal(const Ring* rnga, const Ring* rngb);

void printRingInfo(const Ring* rng);

#endif /* RING_H_ */
