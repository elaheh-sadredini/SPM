/*
 * firstLevel.c
 *
 *  Created on: Mar 1, 2015
 *      Author: Elaheh Sadredini
 */
#include "oneLengthSeed.h"

void addToHashTable(uint32_t item, uint32_t lastVisit)
{
	struct oneLengthCandidate* oneLengthCand = NULL;
	oneLengthCand = (struct oneLengthCandidate*) malloc(sizeof(struct oneLengthCandidate));
	assert(oneLengthCand!=NULL && "unsuccessful memory allocation");

	oneLengthCand->count = 1;
	oneLengthCand->item = item;
	oneLengthCand->lastVisit = lastVisit;
	HASH_ADD_INT(oneLengthCandTable, item, oneLengthCand);
}

struct oneLengthCandidate* findHashTable(uint32_t item)
{
	struct oneLengthCandidate* cand = NULL;
	HASH_FIND_INT(oneLengthCandTable, &item, cand);
	return cand;
}

void deleteItemFromHashTable(struct oneLengthCandidate* oneLengthCand)
{
	HASH_DEL(oneLengthCandTable, oneLengthCand);
	free(oneLengthCand);
}

void printHashTable()
{
	struct oneLengthCandidate* oneLengthCand = NULL;

	for (oneLengthCand = oneLengthCandTable; oneLengthCand != NULL; oneLengthCand = (struct oneLengthCandidate*)(oneLengthCand->hh.next))
	{
		printf("item: %d , count: %d\n", oneLengthCand->item, oneLengthCand->count);
	}
}


void generateFreqItems()
{
	oneLengthCandTable = NULL;
	struct oneLengthCandidate* oneLengthCandTemp = NULL;
	struct itemsets* lastItemset = NULL;
	struct items* lastItem = NULL;

	/* Gerenating one-length candidate and store it in a hash table */
	uint32_t i;
	for(i=0; i<numberOfSequences; i++)
	{
		lastItemset = sequences[i];

		while(lastItemset != NULL)
		{
			lastItem = lastItemset->itemsetStart;

			while(lastItem != NULL)
			{
				if((findHashTable(lastItem->item)) == NULL)
				{
					addToHashTable(lastItem->item, i);
					lastItem = lastItem->next;
				}
				else if((oneLengthCandTemp = findHashTable(lastItem->item))->lastVisit != i)
				{
					(oneLengthCandTemp->count)++;
					oneLengthCandTemp->lastVisit = i;
					lastItem = lastItem->next;
				}
				else
				{
					lastItem = lastItem->next;
				}
			}
			lastItemset = lastItemset->next;
		}
	}

	/* Candidate selection phase */
	oneLengthCandTemp = NULL;
	numberOfFirstSeed = 0;
	int candCount=0;
	for (oneLengthCandTemp = oneLengthCandTable; oneLengthCandTemp != NULL; oneLengthCandTemp = (struct oneLengthCandidate*)(oneLengthCandTemp->hh.next))
	{
		candCount++;
		if((oneLengthCandTemp->count) < frequencyNum)
		{
			deleteItemFromHashTable(oneLengthCandTemp);
		}
		else
		{
				numberOfFirstSeed++;
		}
	}
	//fprintf(outFile, "%d\n", numberOfFirstSeed);
#ifdef debug
	printf("Number of items in the input sequences: %d\n", candCount);
	printf("Number of frequent items: %d\n", numberOfFirstSeed);
#endif
	//printHashTable();
#ifdef debug
	printf("Start pruning input sequences\n");
#endif
    if(gap_between_itemsets)
        pruningInputSequences();
	//displaySequencesList(sequences, numberOfSequences);
}

