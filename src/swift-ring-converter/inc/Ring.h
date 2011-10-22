/*!
 * Ring.h
 *
 *  Created on: 15.10.2011
 *      Author: dmitriy
 */

#ifndef RING_H_
#define RING_H_

#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/nvp.hpp>

#include <vector>
#include <cstddef>
#include <string>
#include <iostream>

/*!
 * \brief Описание устройств
 */
struct CDevice {
	size_t m_id; //!< Индекс в списке устройств
	size_t m_zone; //!< Зона нахождения устройства
	double m_weight; //!< Удельный вес устройства
	std::string m_ip; //!< ip адресс сервера, содержащий устройство
	size_t m_port; //!< TCP порт, для подключения к серверу
	std::string m_device; //!< Название устройства на сервере
	std::string m_meta; //!< Дополнительная мета информация

	friend std::ostream& operator<<(std::ostream& stream, CDevice& dev);

	/*!
	 * \brief сериадизация
	 * \param ar архив
	 * \param version версия
	 */
	template<class Archive>
	void serialize(Archive &ar, const unsigned int version) {
		using boost::serialization::make_nvp;

		ar & make_nvp("id", m_id);
		ar & make_nvp("zone", m_zone);
		ar & make_nvp("weight", m_weight);
		ar & make_nvp("ip", m_ip);
		ar & make_nvp("port", m_port);
		ar & make_nvp("device", m_device);
		ar & make_nvp("meta", m_meta);
	}
};

typedef std::vector<CDevice> CDeviceList;

/*!
 * \brief Кольцо
 */
class CRing {
	CDeviceList m_devices; //!< список устройств
	size_t m_partShift; //!< смещение в MD5 хеше
	std::vector<std::vector<size_t>  > m_replica2part2dev_id;

	friend class boost::serialization::access;

	template<class Archive>
	void serialize(Archive & ar, const unsigned int version) {
		using boost::serialization::make_nvp;
		ar & make_nvp("devs",m_devices);
		ar & make_nvp("partShift", m_partShift);
		ar & make_nvp("replica2part2dev_id", m_replica2part2dev_id);
	}

public:

	/*!
	 * \brief Коснтруктор
	 * \param devices список устройств
	 * \param partShift смещение в MD5 хеше
	 */
	CRing(const CDeviceList& devices, size_t partShift, const std::vector<std::vector<size_t>  >& rpd);

	CRing();

};

#endif /* RING_H_ */
