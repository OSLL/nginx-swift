/*!
 * pythonDeserialize.h
 *
 *  Created on: 15.10.2011
 *      Author: dmitriy
 *	\todo комментировать
 */

#ifndef PYTHONDESERIALIZE_H_
#define PYTHONDESERIALIZE_H_

#include "ring.h"

/*!
 * \brief Десериализация колька
 * \param filePath путь к файлу
 * \return указатель на колльцо
 */
Ring deserializeRing(const char* filePath);


#endif /* PYTHONDESERIALIZE_H_ */
