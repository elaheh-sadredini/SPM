/*
 * utility.c
 *
 *  Created on: Mar 1, 2015
 *      Author: Elaheh Sadredini
 */

#include "utility.h"
#include "dataTypes.h"
#include <stdbool.h>

/* create first itemset of the sequence*/
void addFirstItemSet(struct itemsets** sequenceIndex)
{
	struct itemsets* itemsetTemp = (struct itemsets*) malloc(sizeof(struct itemsets));
	assert(itemsetTemp!=NULL && "unsuccessful memory allocation");
	itemsetTemp->next = NULL;
	itemsetTemp->itemsetStart = NULL;

	*sequenceIndex = itemsetTemp;
}

void addItemSet(struct itemsets** lastItemset)
{
	struct itemsets* newItemSet = (struct itemsets*) malloc(sizeof(struct itemsets));
	newItemSet->next = NULL;
	newItemSet->itemsetStart = NULL;

	(*lastItemset)->next = newItemSet;
	*lastItemset = newItemSet;
}

void addItemToEndOfList(uint32_t item, struct items** lastItemPtr, struct itemsets* lastItemsetPtr)
{
	struct items* newItem = (struct items*) malloc(sizeof(struct items));
	assert(newItem!=NULL && "unsuccessful memory allocation");
	newItem->next = NULL;
	newItem->previous = NULL;
	newItem->item = item;

	if(lastItemsetPtr->itemsetStart == NULL)
	{
		lastItemsetPtr->itemsetStart = newItem;
		//newItem->previous = lastItemsetPtr->itemsetStart;
	}
	else
	{
		(*lastItemPtr)->next = newItem;
		newItem->previous = *lastItemPtr;
	}
	*lastItemPtr = newItem;
}

void addItemToBeginning(uint32_t item, struct itemsets* itemset)
{
	struct items* newItem = (struct items*) malloc(sizeof(struct items));
	assert(newItem!=NULL && "unsuccessful memory allocation");
	newItem->next = NULL;
	newItem->previous = NULL;
	newItem->item = item;

	if(itemset->itemsetStart == NULL)
	{
		itemset->itemsetStart = newItem;
		newItem->previous = itemset->itemsetStart;
		return;
	}

	itemset->itemsetStart->previous = newItem;
	newItem->next = itemset->itemsetStart;
	itemset->itemsetStart = newItem;
	newItem->previous = itemset->itemsetStart;
}

void addToItemListWithOrder(uint32_t item, struct itemsets* itemset)
{
	struct items* itemPtr = itemset->itemsetStart;
	struct items* itemLast = NULL;


	if(itemset->itemsetStart == NULL)
	{
		addItemToBeginning(item, itemset);
		return;
	}
	if(itemset->itemsetStart->item > item)
	{
		addItemToBeginning(item, itemset);
		return;
	}

	else
	{
		while(itemPtr->item < item)
		{
			itemLast = itemPtr;
			itemPtr = itemPtr->next;
			if(itemPtr == NULL)
				break;
		}

		struct items* newItem = NULL;
		newItem = (struct items*) malloc(sizeof(struct items));
		assert(newItem!=NULL && "unsuccessful memory allocation");
		newItem->next = NULL;
		newItem->item = item;

		newItem->next = itemPtr;
		itemLast->next = newItem;
	}
}

void appearItemToItemSet(struct items* toAppearItemPtr, struct itemsets* itemsetPtr)
{
	if(itemsetPtr->itemsetStart == toAppearItemPtr->next)
	{
		itemsetPtr->itemsetStart = toAppearItemPtr;
		if(toAppearItemPtr->next != NULL)
		toAppearItemPtr->next->previous = toAppearItemPtr;

		return;
	}
	toAppearItemPtr->previous->next = toAppearItemPtr;
	if(toAppearItemPtr->next != NULL)
	{
	toAppearItemPtr->next->previous = toAppearItemPtr;
	}
}

void deleteFirstItemInItemset(struct itemsets* itemsetPtr)
{
	itemsetPtr->itemsetStart = itemsetPtr->itemsetStart->next;
	free(itemsetPtr->itemsetStart->previous);
	itemsetPtr->itemsetStart->previous = NULL;
}

void deleteLastItem(struct itemsets* lastItemset)
{
	struct items* LastItemPtr = lastItemset->itemsetStart;
	if(lastItemset->itemsetStart->next == NULL)
	{
		free(lastItemset->itemsetStart);
		lastItemset->itemsetStart = NULL;
		return;
	}
	while(LastItemPtr->next->next != NULL)
		LastItemPtr = LastItemPtr->next;

	struct items* itemTmp = LastItemPtr->next;
	LastItemPtr->next = NULL;
	free(itemTmp);
}

/* This function returns the last itemset pointer in the list */
struct itemsets* findLastItemset(struct itemsets* itemsetPtr)
{
	while(itemsetPtr->next != NULL)
	{
		itemsetPtr = itemsetPtr->next;
	}
	return itemsetPtr;
}

/* This function gets a pointer to itemset and returns the last item in the item list */
struct items* findLastItem(struct itemsets* itemPtr)
{
	struct items* tmp = itemPtr->itemsetStart;
	if(tmp == NULL)
	{
		return NULL;
	}
	while(tmp->next != NULL)
	{
		tmp = tmp->next;
	}
	return tmp;
}

void displaySequencesList(struct itemsets* sequences[], uint32_t arrayLength)
{
	struct items* lastItem = NULL;
	struct itemsets* lastItemset = NULL;
	int i;

	if(sequences == NULL)
		return;

	for( i = 0; i<arrayLength; i++)
	{
		lastItemset = sequences[i];
		if(lastItemset == NULL)
			continue;
		printf("<");
		while(lastItemset != NULL)
		{
			lastItem = lastItemset->itemsetStart;

			printf(" < ");
			while(lastItem != NULL)
			{
				printf("%d ", lastItem->item);
				lastItem = lastItem->next;
			}
			printf(">");
			lastItemset = lastItemset->next;
		}
		printf(" >");
		printf("\n");
	}
}

/* Return true if subItemset is a subset of mainItemset, otherwise return false */
bool searchItemsetInItemset(struct itemsets* mainItemset, struct itemsets* subItemset)
{
	struct items* mainItem = mainItemset->itemsetStart;
	struct items* subItem = subItemset->itemsetStart;

		while(mainItem != NULL)
		{
			if(subItem->item == mainItem->item)
			{
				subItem = subItem->next;
				mainItem = mainItem->next;

				if(subItem == NULL)
					return true;
				else if(subItem != NULL && mainItem == NULL)
					return false;
			}
			else
			{
				mainItem = mainItem->next;
				if(mainItem == NULL)
				{
					return false;
				}
			}
		}
		return false;
}


/* Return true if two itemsets are equal */
bool compareTwoItemsets(struct itemsets* itemset1, struct itemsets* itemset2)
{
	if(itemset1 == NULL && itemset2 == NULL)
		return true;
	if(itemset1 == NULL && itemset2 != NULL)
		return false;
	if(itemset1 != NULL && itemset2 == NULL)
		return false;

	struct items* lastItem1 = itemset1->itemsetStart;
	struct items* lastItem2 = itemset2->itemsetStart;

		while((lastItem2 != NULL) && (lastItem1 != NULL))
		{
			if(lastItem1->item == lastItem2->item)
			{
				lastItem1 = lastItem1->next;
				lastItem2 = lastItem2->next;

				if(lastItem1 == NULL && lastItem2 == NULL)
					return true;
				if(lastItem1 == NULL && lastItem2 != NULL)
					return false;
				if(lastItem1 != NULL && lastItem2 == NULL)
					return false;
			}
			else
				return false;
		}
		return false;
}

/* return the number of sequences which have the input candidate as their subsequences */
uint32_t searchCandidateInSequences(struct itemsets* candidate)
{
	int i;
	uint32_t count = 0;
	struct itemsets* lastItemset = NULL;
	struct itemsets* candidateTmp = NULL;

	if(candidate == NULL)
		return count;

	for(i=0; i<numberOfSequences; i++)
	{
		candidateTmp = candidate;
		lastItemset = sequences[i];
			while(lastItemset != NULL)
			{
				if(searchItemsetInItemset(lastItemset, candidateTmp) == true)
				{
					lastItemset = lastItemset->next;
					candidateTmp = candidateTmp->next;
					if(candidateTmp == NULL)
					{
						count++;
						if(count == frequencyNum)
							return count;
						break;
					}

				}
				else
				{
                    if(!gap_between_itemsets)
                        candidateTmp = candidate;
					lastItemset = lastItemset->next;
					if(lastItemset == NULL)
						break;
				}
			}
	}
	return count;
}


void disappearItemFromItemSet(struct itemsets* itemsetPtr, struct items* toDissapearItemPtr)
{
	if(toDissapearItemPtr == itemsetPtr->itemsetStart) /* Delete from beginning of the list */
	{
		itemsetPtr->itemsetStart = toDissapearItemPtr->next;
		if(itemsetPtr->itemsetStart != NULL)
		{
		itemsetPtr->itemsetStart->previous = NULL;
		toDissapearItemPtr->next->previous = itemsetPtr->itemsetStart;
		}
		return;
	}

	toDissapearItemPtr->previous->next = toDissapearItemPtr->next;
	if(toDissapearItemPtr->next != NULL)
	{
	toDissapearItemPtr->next->previous = toDissapearItemPtr->previous;
	}
}

void pruningInputSequences()
{
	int i = 0;
	struct itemsets* currentItemset = NULL;
	struct itemsets* previousItemset = NULL;
	struct items* currentItem = NULL;
	struct items* currentItemTmp = NULL;
	for(i=0; i<numberOfSequences; i++)
	{
		currentItemset = sequences[i];
		previousItemset = sequences[i];
		while(currentItemset != NULL)
		{
			//displaySequencesList(&sequences[i], 1);
			currentItem = currentItemset->itemsetStart;
			while(currentItem != NULL)
			{
				if((findHashTable(currentItem->item)) == 0)
				{
					disappearItemFromItemSet(currentItemset, currentItem);
					currentItemTmp = currentItem;
					currentItem = currentItem->next;
					free(currentItemTmp);
					currentItemTmp = NULL;
				}
				else
				currentItem = currentItem->next;
			}
			if(currentItemset->itemsetStart == NULL && currentItemset == sequences[i] && sequences[i]->next == NULL)
			{
				free(currentItemset);
				currentItemset = NULL;
				sequences[i] = NULL;
			}
			else if(currentItemset->itemsetStart == NULL && previousItemset == currentItemset)
			{
				previousItemset = previousItemset->next;
				free(currentItemset);
				currentItemset = NULL;
				currentItemset = previousItemset;
				sequences[i] = previousItemset;
			}
			else if(currentItemset->itemsetStart == NULL)
			{
				previousItemset->next = currentItemset->next;
				free(currentItemset);
				currentItemset = NULL;
				currentItemset = previousItemset->next;
			}
			else
			{
			//printf("%d\n", currentItemset->itemsetStart->item);
			previousItemset = currentItemset;
			currentItemset = currentItemset->next;
			}
		}
	}
}

uint32_t compareTwoSequenceIgnoringLastItemInSecondSequence(struct itemsets* sequence1, struct itemsets* sequence2, bool* isIndependent, struct itemsets** lastItemsetInFirstCandidatePtr, struct items** lastItemInLastItemsetInFirstCandidatePtr)
{
	bool checkFlag = true;
	if(sequence1->itemsetStart == NULL)
		sequence1 = sequence1->next;
	if(sequence2 == NULL)
		return 0;

	while(checkFlag == true)
	{
		if(sequence1->next == NULL && sequence2->next == NULL)
		{
			checkFlag = false;
			break;
		}
		else if(sequence1->next == NULL && sequence2->next != NULL)
		{
			if(compareTwoItemsets(sequence1, sequence2) == 1)
			{
				sequence2 = sequence2->next;
				checkFlag = false;
				break;
			}
			else
				return 0;
		}
		else
		{
			if(compareTwoItemsets(sequence1, sequence2) == 1)
			{
				sequence1 = sequence1->next;
				sequence2 = sequence2->next;
			}
			else
				return 0;
		}
	}

	*lastItemsetInFirstCandidatePtr = sequence1;

	if(sequence2->itemsetStart->next == NULL)
	{
		*isIndependent = true;
		struct items* itemset1ItemPtrTmp = sequence1->itemsetStart;
		while(itemset1ItemPtrTmp != NULL)
			{
				*lastItemInLastItemsetInFirstCandidatePtr = itemset1ItemPtrTmp;
				itemset1ItemPtrTmp = itemset1ItemPtrTmp->next;
			}
		return sequence2->itemsetStart->item;
	}

	else
	{
		struct items* itemset1ItemPtrTmp = sequence1->itemsetStart;
		struct items* itemset2ItemPtrTmp = sequence2->itemsetStart;
		while(itemset1ItemPtrTmp != NULL)
		{
			if(itemset1ItemPtrTmp->item == itemset2ItemPtrTmp->item)
			{
				*lastItemInLastItemsetInFirstCandidatePtr = itemset1ItemPtrTmp;
				itemset1ItemPtrTmp = itemset1ItemPtrTmp->next;
				itemset2ItemPtrTmp = itemset2ItemPtrTmp->next;
			}
			else
				return 0;
		}
		return itemset2ItemPtrTmp->item;
	}
}

struct itemsets* copyItemset(struct itemsets* itemset)
{
	struct items* lastItemPtrInCopiedItemset = NULL;
	struct items* lastItemPtr = itemset->itemsetStart;
	assert(lastItemPtr!=NULL && "unsuccessful memory allocation");
	struct itemsets* copiedItemset = (struct itemsets*)malloc(sizeof(struct itemsets));
	assert(copiedItemset!=NULL && "unsuccessful memory allocation");
	copiedItemset->itemsetStart = NULL;
	copiedItemset->next = NULL;

	while(lastItemPtr != NULL)
	{
		addItemToEndOfList(lastItemPtr->item, &lastItemPtrInCopiedItemset, copiedItemset);
		lastItemPtr = lastItemPtr->next;
	}
	return copiedItemset;
}

struct itemsets* copySequence(struct itemsets* sequence)
{
	struct itemsets* lastItemset = NULL;

	lastItemset = copyItemset(sequence);
	struct itemsets* returnPtr = lastItemset;
	sequence = sequence->next;

	while(sequence != NULL)
	{
		lastItemset->next = copyItemset(sequence);
		lastItemset = lastItemset->next;
		sequence = sequence->next;
	}
	return returnPtr;
}

/* sequence signature is used for the hash table key */
char* sequenceSignature(struct itemsets* sequence, uint32_t length)
{
	struct items* itemTmp = NULL;
	uint32_t itemNum = 0;
	uint32_t maxLength = 14*length*length;
	char* signature = (char*) malloc(maxLength*sizeof(char));
	assert(signature!=NULL && "unsuccessful memory allocation");
	uint32_t i = 0;
	for(i=0; i<maxLength;i++)
	{
		signature[i] = 0;
	}

	i = 0;
	while(sequence != NULL)
	{
		// if there is an itemset without item
		while(sequence->itemsetStart == NULL)
		{
			if(sequence->next != NULL)
				sequence = sequence->next;
			else
				return signature;
		}
		itemTmp = sequence->itemsetStart;
		while(itemTmp != NULL)
		{
			itemNum =(itemTmp->item);

			int n = logTen(itemNum);
			i = i + n;
			while(itemNum != 0)
			{
				if(n == 0)
				{
					signature[i] = (itemNum % 10)+48;
					break;
				}
				signature[i] = (itemNum % 10)+48;
				itemNum = itemNum/10;
				if(itemNum == 0)
					break;

				i--;
			}
			i = i+n+1;

			if(itemTmp->next != NULL)
			{
				itemTmp = itemTmp->next;
				signature[i] = '-';
				i++;
			}
			else
			{
				sequence = sequence->next;
				if(sequence != NULL)
				{
					while(sequence->itemsetStart == NULL)
					{
						sequence = sequence->next;
						if(sequence == NULL)
							break;
					}
				}

				if(sequence == NULL)
				break;
				else
				{
					signature[i] = '-';
					i++;
					signature[i] = '-';
					i++;
					break;
				}
			}
		}
	}
	signature[i+1] = 0;
	return signature;
}
int logTen(int number)
{
	int n = 0;
	while(number/10 != 0)
	{
		number = number/10;
		n++;
	}
	return n;
}

void freeSequence(struct itemsets* sequence)
{
	struct items* lastItem = NULL;
	struct itemsets* lastItemset = NULL;

	if(sequence == NULL)
		return;
	while(sequence->itemsetStart == NULL)
	{
		lastItemset = sequence;
		sequence = sequence->next;
		free(lastItemset);
		lastItemset = NULL;
	}

	while(sequence != NULL)
	{
		while(sequence->itemsetStart != NULL)
		{
			lastItem = sequence->itemsetStart;
			sequence->itemsetStart = sequence->itemsetStart->next;
			free(lastItem);
			lastItem = NULL;
		}

		lastItemset = sequence;
		sequence = sequence->next;
		free(lastItemset);
		lastItemset = NULL;
		if(sequence == NULL)
			return;

		while(sequence->itemsetStart == NULL)
		{
			lastItemset = sequence;
			sequence = sequence->next;
			free(lastItemset);
			lastItemset = NULL;
			if(sequence == NULL)
				return;
		}

	}
}

void freeArrayOfSequence(struct itemsets** sequenceArray, uint32_t numberOfSequences)
{
	uint32_t i = 0;
	for(i=0; i< numberOfSequences; i++)
	{
		freeSequence(sequenceArray[i]);
		sequenceArray[i] = NULL;
	}
}


/////////////////* Frequent hash table functions *///////////////////////
/////////////////////////////////////////////////////////////////////////
void addToFrequentCandidateHashTable(char* sequenceSignature)
{
	struct frequentCandidate* frequentCand = NULL;
	frequentCand = (struct frequentCandidate*) malloc(sizeof(struct frequentCandidate));
	assert(frequentCand!=NULL && "unsuccessful memory allocation");
	frequentCand->sequenceSignature = sequenceSignature;
	HASH_ADD_KEYPTR(hh, frequentCandidateTable, frequentCand->sequenceSignature, strlen(frequentCand->sequenceSignature), frequentCand);
}

struct frequentCandidate* findFrequentCandidateHashTable(char* sequenceSignature)
{
	struct frequentCandidate* frequentCand = NULL;
	HASH_FIND_STR(frequentCandidateTable, sequenceSignature, frequentCand);
	return frequentCand;
}

void printFrequentCandidateHashTable()
{
	struct frequentCandidate* frequentCand = NULL;

	for (frequentCand = frequentCandidateTable; frequentCand != NULL; frequentCand = (struct frequentCandidate*)(frequentCand->hh.next))
	{
		printf("Frequent candidate is: %s\n", frequentCand->sequenceSignature);
	}
}
