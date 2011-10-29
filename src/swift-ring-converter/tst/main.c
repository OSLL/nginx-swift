/*
 * main.c
 *
 *  Created on: 29.10.2011
 *      Author: dmitriy
 */

#include <stdio.h>
#include <string.h>
#include "ring.h"
#include "PythonDeserialize.h"
#include "CUnit/Basic.h"

static char* fileName = "./testData/account.ring.gz";

int init_suite(void) {
	Py_Initialize();
	return 0;
}

int clean_suite(void) {
	Py_Finalize();
	return 0;
}

void test_python_serialize(void) {

	Ring rng = deserializeRing(fileName);

	CU_ASSERT(rng.m_devCount==4);
	CU_ASSERT(rng.m_partShift==14);
	CU_ASSERT(rng.m_replicaCount==3);

}

void test_tpl_serialize(void) {

	Ring rng = deserializeRing(fileName);

	save("test_serialize.tpl", &rng);

	Ring rng2 = load("test_serialize.tpl");

	CU_ASSERT(equal(&rng,&rng2));
}

int main() {
	CU_pSuite pSuite = NULL;

	if (CUE_SUCCESS != CU_initialize_registry())
		return CU_get_error();

	pSuite = CU_add_suite("Suite serialize", init_suite, clean_suite);
	if (NULL == pSuite) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	if ((NULL
			== CU_add_test(pSuite, "test python serialize",
					test_python_serialize))
			|| (NULL
					== CU_add_test(pSuite, "test tpl serialize",
							test_tpl_serialize))) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_tests();
	CU_cleanup_registry();
	return CU_get_error();
}

