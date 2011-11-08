/*!
 * PythonDeserialize.cpp
 *
 *  Created on: 15.10.2011
 *      Author: dmitriy
 *	\todo комментировать
 */

#include "PythonDeserialize.h"

/*!
 * \brief Пулучить инстанцированный объект GzipFile
 * \param filePath путь к файлу
 * \return при удаче объекть, иначе NULL
 */
PyObject* getGzipFileObj(const char* filePath) {

	PyObject* module;
	module = PyImport_Import(PyString_FromString("gzip"));

	PyObject* dict;
	dict = PyModule_GetDict(module);

	PyObject* pClass;
	pClass = PyDict_GetItemString(dict, "GzipFile");

	PyObject* arg;
	arg = PyTuple_New(2);
	PyTuple_SetItem(arg, 0, PyString_FromString(filePath));
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

void getDevs(PyObject* devList, Ring* rng) {
	rng->m_device = (Device*) malloc(sizeof(Device) * PyList_Size(devList));
	if (!rng->m_device) {
		printf("can't allocate memory!\n");
		exit(1);
	}
	rng->m_devCount = PyList_Size(devList);

	for (size_t n = 0; n < PyList_Size(devList); ++n) {
		PyObject *item = PyList_GetItem(devList, n);
		Device dev;
		dev.m_id = PyInt_AsSsize_t(
				PyObject_GetItem(item, PyString_FromString("id")));
		dev.m_zone = PyInt_AsSsize_t(
				PyObject_GetItem(item, PyString_FromString("zone")));
		dev.m_port = PyInt_AsSsize_t(
				PyObject_GetItem(item, PyString_FromString("port")));

		dev.m_ip = PyString_AsString(
				PyObject_GetItem(item, PyString_FromString("ip")));

		dev.m_device = PyString_AsString(
				PyObject_GetItem(item, PyString_FromString("device")));

		dev.m_meta = PyString_AsString(
				PyObject_GetItem(item, PyString_FromString("meta")));

		dev.m_weight = PyFloat_AsDouble(
				PyObject_GetItem(item, PyString_FromString("weight")));

		rng->m_device[n] = dev;

		Py_DECREF(item);

	}
	Py_DECREF(devList);
}

void getIds(PyObject* idListList, Ring* rng) {
	rng->m_replicaCount = PyList_Size(idListList);
	rng->m_replica2part2dev_id = (struct part2id*) malloc(
			sizeof(struct part2id) * rng->m_replicaCount);

	if (rng->m_replica2part2dev_id) {
		for (uint64_t i = 0; i < rng->m_replicaCount; ++i) {
			PyObject* item = PyList_GetItem(idListList, i);
			rng->m_replica2part2dev_id[i].m_count = PySequence_Length(item);

			rng->m_replica2part2dev_id[i].m_devId = (uint64_t*) malloc(
					sizeof(uint64_t) * PySequence_Length(item));

			if (rng->m_replica2part2dev_id[i].m_devId) {

				for (size_t j = 0; j < rng->m_replica2part2dev_id[i].m_count;
						++j) {
					rng->m_replica2part2dev_id[i].m_devId[j] = PyInt_AsLong(
							PySequence_GetItem(item, j));
				}
			} else {
				printf("can't allocate memory!\n");
				exit(1);
			}

			Py_DECREF(item);
		}
	} else {
		printf("can't allocate memory!\n");
		exit(1);
	}

	Py_DECREF(idListList);

}

Ring deserializeRing(const char* filePath) {

	PyObject* loadPickleFun = getPickleLoadFunction();

	PyObject* arg;
	arg = PyTuple_New(1);
	PyTuple_SetItem(arg, 0, getGzipFileObj(filePath));

	PyObject* dict = PyObject_CallObject(loadPickleFun, arg);

	Ring rng;

	ring_init(&rng);

	getDevs(PyObject_GetItem(dict, PyString_FromString("devs")), &rng);

	rng.m_partShift = PyInt_AsSsize_t(
			PyObject_GetItem(dict, PyString_FromString("part_shift")));

	getIds(PyObject_GetItem(dict, PyString_FromString("replica2part2dev_id")),
			&rng);

	PyErr_Print();

	return rng;
}
