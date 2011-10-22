/*!
 * main.cpp
 *
 *  Created on: 14.10.2011
 *      Author: dmitriy
 */

#include <python2.6/Python.h>
#include <iostream>
#include <fstream>
#include "PythonDeserialize.h"

#include <boost/archive/xml_oarchive.hpp>

int main(int argc, char *argv[]) {
	Py_Initialize();

	boost::shared_ptr<CRing> ring = deserializeRing("/etc/swift/object.ring.gz");

	std::ofstream ofs("ring.xml");
	boost::archive::xml_oarchive oa(ofs);
	CRing rng = *ring;
	oa << boost::serialization::make_nvp("Ring",rng);

	PyErr_Print();

	Py_Finalize();
	return 0;
}
