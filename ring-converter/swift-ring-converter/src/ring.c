/*!
 * ring.cpp
 *
 *  Created on: 15.10.2011
 *      Author: dmitriy
 */

#include <tpl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "ring.h"

void destroy(Ring* rng) {
	for (size_t i = 0; i < rng->m_replicaCount; ++i) {
		free(rng->m_replica2part2dev_id[i].m_devId);
	}
	free(rng->m_replica2part2dev_id);
	free(rng->m_device);
	rng->m_devCount = 0;
	rng->m_device = 0;
}

void printDevice(const Device* dev) {
	printf("id:      %lu\n", dev->m_id);
	printf("zone:    %lu\n", dev->m_zone);
	printf("weight:  %f\n", dev->m_weight);
	printf("ip:      %s\n", dev->m_ip);
	printf("port:    %lu\n", dev->m_port);
	printf("device:  %s\n", dev->m_device);
	printf("meta:    %s\n\n", dev->m_meta);
}

Ring load(const char* const fileName) {
	Ring rng;

	tpl_node *tn;
	Device dev;

	uint64_t a, b, c;

	uint64_t pid;
	uint64_t id;
	uint64_t devCount;

	tn = tpl_map("UUUA(S(UUfsUss))A(U)A(U)", &rng.m_devCount, &rng.m_partShift,
			&rng.m_replicaCount, &dev, &pid, &id);
	tpl_load(tn, TPL_FILE, fileName);
	tpl_unpack(tn, 0);

	rng.m_device = (Device*) malloc(sizeof(Device) * rng.m_devCount);

	for (size_t i = 0; i < rng.m_devCount; ++i) {
		tpl_unpack(tn, 1);
		rng.m_device[i] = dev;
	}

	rng.m_replica2part2dev_id = (struct part2id*) malloc(
			sizeof(struct part2id) * rng.m_replicaCount);
	for (size_t i = 0; i < rng.m_replicaCount; ++i) {
		tpl_unpack(tn, 2);
		rng.m_replica2part2dev_id[i].m_count = pid;
	}

	for (size_t i = 0; i < rng.m_replicaCount; ++i) {
		rng.m_replica2part2dev_id[i].m_devId = (uint64_t*) malloc(
				sizeof(uint64_t) * pid);
		for (size_t j = 0; j < rng.m_replica2part2dev_id[i].m_count; ++j) {
			tpl_unpack(tn, 3);
			rng.m_replica2part2dev_id[i].m_devId[j] = id;
		}
	}

	return rng;
}
;

void save(const char* const fileName, const Ring* rng) {
	tpl_node *tn;
	Device dev;
	uint64_t pid;
	uint64_t id;

	tn = tpl_map("UUUA(S(UUfsUss))A(U)A(U)", &rng->m_devCount,
			&rng->m_partShift, &rng->m_replicaCount, &dev, &pid, &id);
	tpl_pack(tn, 0);
	for (size_t i = 0; i < rng->m_devCount; ++i) {
		dev = rng->m_device[i];
		tpl_pack(tn, 1);
	}

	for (size_t i = 0; i < rng->m_replicaCount; ++i) {
		pid = rng->m_replica2part2dev_id[i].m_count;
		tpl_pack(tn, 2);
	}

	for (size_t i = 0; i < rng->m_replicaCount; ++i) {
		for (size_t j = 0; j < rng->m_replica2part2dev_id[i].m_count; ++j) {
			id = rng->m_replica2part2dev_id[i].m_devId[j];
			tpl_pack(tn, 3);
		}
	}
	tpl_dump(tn, TPL_FILE, fileName);
	tpl_free(tn);
}
;

bool equalDevice(const Device* dev1, const Device* dev2) {

	if ((dev1->m_id != dev2->m_id) || (dev1->m_port != dev2->m_port)
			|| (dev1->m_zone != dev2->m_zone))
		return false;

	if ((strcmp(dev1->m_ip, dev2->m_ip)
			|| strcmp(dev1->m_device, dev2->m_device)) != 0)
		return false;

	return true;
}

bool equal(const Ring* rnga, const Ring* rngb) {
	if ((rnga->m_devCount != rngb->m_devCount)
			|| ((rnga->m_replicaCount != rngb->m_replicaCount)
					|| (rnga->m_partShift != rngb->m_partShift)))
		return false;

	for (size_t i = 0; i < rnga->m_devCount; ++i)
		if (!equalDevice(&rnga->m_device[i], &rngb->m_device[i]))
			return false;

	for (size_t i = 0; i < rnga->m_replicaCount; ++i) {
		if (rnga->m_replica2part2dev_id[i].m_count
				!= rngb->m_replica2part2dev_id[i].m_count)
			return false;

		for (size_t j = 0; j < rnga->m_replica2part2dev_id[i].m_count; ++j) {
			if (rnga->m_replica2part2dev_id[i].m_devId[j]
					!= rngb->m_replica2part2dev_id[i].m_devId[j])
				return false;
		}
	}
	return true;
}

void printRingInfo(const Ring* rng) {

	printf("\tPart shift:%lu\treplic count: %lu\n", rng->m_partShift,
			rng->m_replicaCount);

	printf("Device list:\n");
	for (size_t i = 0; i < rng->m_devCount; ++i)
		printDevice(&rng->m_device[i]);

}
