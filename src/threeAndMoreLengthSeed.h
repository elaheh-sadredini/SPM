/*
 * threeAndMoreLengthSeed.h
 *
 *  Created on: Mar 2, 2015
 *      Author: Elaheh Sadredini
 */
#pragma once
#include "utility.h"
#include "dataTypes.h"
//#include <time.h>

void generateThreeAndMoreFreqSeq();

struct itemsets* candidateJoin(struct itemsets** firstCand, struct itemsets* secondCand);

bool pruning(struct itemsets* joinedCandidate, uint32_t length);

