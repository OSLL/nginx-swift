/*!
 * PythonDeserialize.h
 *
 *  Created on: 15.10.2011
 *      Author: dmitriy
 *	\todo комментировать
 */

#ifndef PYTHONDESERIALIZE_H_
#define PYTHONDESERIALIZE_H_

#include <boost/shared_ptr.hpp>
#include "Ring.h"

/*!
 * \brief Десериализация колька
 * \param filePath путь к файлу
 * \return указатель на колльцо
 */
boost::shared_ptr<CRing> deserializeRing(const std::string& filePath);


#endif /* PYTHONDESERIALIZE_H_ */
