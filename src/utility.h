/*
 * utility.h
 *
 *  Created on: Mar 1, 2015
 *      Author: Elaheh Sadredini
 */

#pragma once

#include "dataTypes.h"

void addFirstItemSet(struct itemsets** sequenceIndex);

void addItemSet(struct itemsets** lastItemset);

void addItemToEndOfList(uint32_t item, struct items** lastItemPtr, struct itemsets* lastItemsetPtr);

void addItemToBeginning(uint32_t item, struct itemsets* itemset);

void addItemToEnd(uint32_t item, struct items* itemsetStart);

void disappearItemFromItemSet(struct itemsets* itemsetPtr, struct items* toDissapearItemPtr);

void appearItemToItemSet(struct items* toAppearItemPtr, struct itemsets* itemsetPtr);

void deleteLastItem(struct itemsets* lastItemset);

void deleteFirstItemInItemset(struct itemsets* itemsetPtr);

void addToItemListWithOrder(uint32_t item, struct itemsets* itemset);

void displaySequencesList(struct itemsets* sequences[], uint32_t arrayLength);

uint32_t searchCandidateInSequences(struct itemsets* candidates);

bool searchItemsetInItemset(struct itemsets* mainItemset, struct itemsets* subItemset);

uint32_t compareTwoSequenceIgnoringLastItemInSecondSequence(struct itemsets* sequence1, struct itemsets* sequence2, bool* isIndependent, struct itemsets** lastItemsetInFirstCandidatePtr, struct items** lastItemInLastItemsetInFirstCandidatePtr);

bool compareTwoItemsets(struct itemsets* itemset1, struct itemsets* itemset2);

bool compareTwoSequence(struct itemsets* itemset1, struct itemsets* itemset2);

struct itemsets* copyItemset(struct itemsets* itemset);

struct itemsets* copySequence(struct itemsets* sequence);

struct itemsets* findLastItemset(struct itemsets* itemsetPtr);

struct items* findLastItem(struct itemsets* itemPtr);

int logTen(int number);

void freeSequence(struct itemsets* sequence);

void freeArrayOfSequence(struct itemsets** sequenceArray, uint32_t numberOfSequences);

void pruningInputSequences();

char* sequenceSignature(struct itemsets* sequence, uint32_t length);

void addToFrequentCandidateHashTable(char* sequenceSignature);

struct frequentCandidate* findFrequentCandidateHashTable(char* sequenceSignature);
