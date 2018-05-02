/*
 * dataTypes.h
 *
 *  Created on: Mar 1, 2015
 *      Author: Elaheh Sadredini
 */
#pragma once
#include "../UTHASH/uthash.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/time.h>

#define printFreqSeq
//#define debug

FILE* outFile;
FILE* candFile;
int out_write;
int cand_write;
int gap_between_itemsets;

struct items
{
	uint32_t item;
	struct items* next;
	struct items* previous;
};

struct itemsets
{
	struct items* itemsetStart;
	struct itemsets* next;
};

struct oneLengthCandidate{
	uint32_t item;
	uint32_t count;
	uint32_t lastVisit;
	UT_hash_handle hh;
};

struct frequentCandidate{
	char* sequenceSignature;
	UT_hash_handle hh;
};

struct itemsets** sequences; /* main data structure for saving sequences*/
struct oneLengthCandidate* oneLengthCandTable;
struct frequentCandidate* frequentCandidateTable;
struct itemsets** twoLengthCandidate; /* arrays of tow dimensional linked list for saving two length candidates. */
struct itemsets** twoLengthSeeds; /* arrays of tow dimensional linked list for saving two length seeds. */

uint32_t numberOfSequences; /* number of sequences */
uint32_t frequencyNum; /* numberofSequences * frequency */
uint32_t numberOfFirstSeed; /* number of 1-length frequent sequences */
int maxSequentailPatternLength; /* The maximum length for the frequent sequence which is asked by the user */
uint32_t numberOfTowLengthCandidates; /* number of two length candidates */
uint32_t numberOfTowLengthSeeds; /* number of two length seeds */

//timing 
double matching_time;
struct timeval start_matching, end_matching;
