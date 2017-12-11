/*
 * firstLevel.h
 *
 *  Created on: Mar 1, 2015
 *      Author: Elaheh Sadredini
 */
#pragma once

#include "utility.h"

void addToHashTable(uint32_t item, uint32_t lastVisit);

struct oneLengthCandidate* findHashTable(uint32_t item);

void deleteItemFromHashTable(struct oneLengthCandidate* oneLengthCand);

void printHashTable();

void generateFreqItems();
