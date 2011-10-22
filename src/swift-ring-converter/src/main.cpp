/*!
 * main.cpp
 *
 *  Created on: 14.10.2011
 *      Author: dmitriy
 */

#include <iostream>
#include <fstream>
#include <strstream>
#include <unistd.h>
#include "PythonDeserialize.h"

#include <boost/regex.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/filtering_streambuf.hpp>

int main(int argc, char *argv[]) {

	if(argc<2){
		std::cout<<"Format: swift-ring-converter -i file\n";
		return 1;
	}

	int rez = 0;
	opterr = 0;
	boost::shared_ptr<CRing> ring;
	std::string fileName;
	while ((rez = getopt(argc, argv, "i:")) != -1) {
		switch (rez) {
		case 'i':
			fileName = std::string(optarg);
			break;
		case '?':
			std::cout << "Error found !\n";
			break;
		};
	};

	boost::regex nameRegexp("(\\w+)\\.ring\\.gz");
	boost::smatch result;
	boost::regex_search(fileName, result, nameRegexp);

	ring = deserializeRing(fileName);

	std::string outName = result[1] + "ring.xml.gz";

	try {
		using namespace boost::iostreams;
		std::ofstream file(outName.c_str(), std::ios_base::app );
		filtering_streambuf<output> out;
	    out.push(gzip_compressor());
	    out.push(file);

	    std::strstream serializeStream;
		boost::archive::xml_oarchive oa(serializeStream);
		CRing rng = *ring;
		oa << boost::serialization::make_nvp("Ring", rng);

		boost::iostreams::copy(serializeStream, out);
	} catch (...) {
		std::cerr<<"Error serialize!\n";
	}

	return 0;
}
