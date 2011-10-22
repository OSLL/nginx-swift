/*!
 * PythonDeserialize.cpp
 *
 *  Created on: 15.10.2011
 *      Author: dmitriy
 *	\todo комментировать
 */

#include <python2.6/Python.h>
#include <iostream>

#include "PythonDeserialize.h"

/*!
 * \brief Пулучить инстанцированный объект GzipFile
 * \param filePath путь к файлу
 * \return при удаче объекть, иначе NULL
 */
PyObject* getGzipFileObj(const std::string& filePath) {

	PyObject* module;
	module = PyImport_Import(PyString_FromString("gzip"));

	PyObject* dict;
	dict = PyModule_GetDict(module);

	PyObject* pClass;
	pClass = PyDict_GetItemString(dict, "GzipFile");

	PyObject* arg;
	arg = PyTuple_New(2);
	PyTuple_SetItem(arg, 0, PyString_FromString(filePath.c_str()));
	PyTuple_SetItem(arg, 1, PyString_FromString("rb"));

	PyObject* instance = NULL;
	if (PyCallable_Check(pClass)) {
		instance = PyObject_CallObject(pClass, arg);
	}

	if (instance == NULL)
		PyErr_Print();

	Py_DECREF(module);
	Py_DECREF(dict);
	Py_DECREF(pClass);
	Py_DECREF(arg);

	return instance;
}

/*!
 * \brief Получить функцию load из пакета cPickle
 * \return при удаче функция, иначе NULL
 */
PyObject* getPickleLoadFunction() {
	PyObject* module;
	module = PyImport_Import(PyString_FromString("cPickle"));

	if (module != NULL) {
		PyObject* fun = PyObject_GetAttrString(module, "load");

		if (fun && PyCallable_Check(fun))
			return fun;
		else
			return NULL;
	}

	PyErr_Print();
	return NULL;
}

void getDevs(PyObject* devList, CDeviceList& devs) {
	for (size_t n = 0; n < PyList_Size(devList); ++n) {
		PyObject *item = PyList_GetItem(devList, n);
		CDevice dev;
		dev.m_id = PyInt_AsSsize_t(
				PyObject_GetItem(item, PyString_FromString("id")));
		dev.m_zone = PyInt_AsSsize_t(
				PyObject_GetItem(item, PyString_FromString("zone")));
		dev.m_port = PyInt_AsSsize_t(
				PyObject_GetItem(item, PyString_FromString("port")));

		dev.m_ip = std::string(
				PyString_AsString(
						PyObject_GetItem(item, PyString_FromString("ip"))));

		dev.m_device = std::string(
				PyString_AsString(
						PyObject_GetItem(item, PyString_FromString("device"))));

		dev.m_meta = std::string(
				PyString_AsString(
						PyObject_GetItem(item, PyString_FromString("meta"))));

		dev.m_weight = PyFloat_AsDouble(
				PyObject_GetItem(item, PyString_FromString("weight")));

		devs.push_back(dev);

		Py_DECREF(item);

	}
	Py_DECREF(devList);
}

void getIds(PyObject* idListList, std::vector<std::vector<size_t> >& lst) {
	std::vector<size_t> empty;
	lst.assign(PyList_Size(idListList), empty);

	for (size_t i = 0; i < PyList_Size(idListList); ++i) {
		PyObject* item=PyList_GetItem(idListList, i);

		lst[i].assign(PySequence_Length(item),0);
		for(size_t j = 0; j<PySequence_Length(item); ++j){
			lst[i][j] = PyInt_AsSsize_t(PySequence_GetItem(item, j));
		}

		Py_DECREF(item);
	}

	Py_DECREF(idListList);

}

using namespace boost;
boost::shared_ptr<CRing> deserializeRing(const std::string& filePath) {
	Py_Initialize();

	PyObject* loadPickleFun = getPickleLoadFunction();

	PyObject* arg;
	arg = PyTuple_New(1);
	PyTuple_SetItem(arg, 0, getGzipFileObj(filePath.c_str()));

	PyObject* dict = PyObject_CallObject(loadPickleFun, arg);

	CDeviceList devs;

	getDevs(PyObject_GetItem(dict, PyString_FromString("devs")), devs);

	size_t partShift = PyInt_AsSsize_t(
			PyObject_GetItem(dict, PyString_FromString("part_shift")));

	std::vector<std::vector<size_t> > lst;
	getIds(PyObject_GetItem(dict, PyString_FromString("replica2part2dev_id")), lst);

	PyErr_Print();
	Py_Finalize();

	shared_ptr<CRing> ptr(new CRing(devs, partShift,lst));
	return ptr;
}

