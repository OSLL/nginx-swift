/*!
 * Ring.cpp
 *
 *  Created on: 15.10.2011
 *      Author: dmitriy
 */

#include "Ring.h"

std::ostream& operator<<(std::ostream& stream, CDevice& dev) {
	stream<<"id     :"<<dev.m_id<<"\n";
	stream<<"zone   :"<<dev.m_zone<<"\n";
	stream<<"weight :"<<dev.m_weight<<"\n";
	stream<<"ip     :"<<dev.m_ip<<"\n";
	stream<<"port   :"<<dev.m_port<<"\n";
	stream<<"device :"<<dev.m_device<<"\n";
	stream<<"meta   :"<<dev.m_meta<<"\n";
	stream<<"\n";

	return stream;
}

CRing::CRing(const CDeviceList& device, size_t partShift, const std::vector<std::vector<size_t>  >& rpd) :
		m_devices(device), m_partShift(partShift), m_replica2part2dev_id(rpd) {

}

CRing::CRing() :
		m_partShift(0) {

}

