/*
 * threeAndMoreLengthSeed.c
 *
 *  Created on: Mar 2, 2015
 *      Author: Elaheh Sadredini
 */


#include "threeAndMoreLengthSeed.h"

/* candidate are generated in this step are the one after pruning phase */
void generateThreeAndMoreFreqSeq()
{
	uint32_t maxNumberOfFrequentSequenceForEachLength = 100000;

	uint32_t patternLengthIterator = 0;
	uint32_t numberOfCurrentFrequentSequence = numberOfTowLengthSeeds;
	uint32_t i, j;
	struct itemsets** previousFrequentSequences = twoLengthSeeds;

	struct itemsets* joinedCandidate = NULL; /* candidate after join */
	uint32_t numberOfNextFrequentSequence = numberOfTowLengthSeeds;
	bool abort1 = false, abort2 = false;


	uint32_t cnt = 0;
    

	while(previousFrequentSequences != NULL)
	{
        if(out_write){
			char buffer[50];
			//snprintf(buffer, sizeof(buffer), "intermidiate-size%d.txt", patternLengthIterator+3);
			candFile = fopen(buffer, "w");
		}
        
		numberOfCurrentFrequentSequence = numberOfNextFrequentSequence;
		struct itemsets** nextFrequentSequences = (struct itemsets**)malloc(maxNumberOfFrequentSequenceForEachLength*sizeof(struct itemsets*));
		assert(nextFrequentSequences!=NULL && "unsuccessful memory allocation");

		//initialization
		for(i=0; i<maxNumberOfFrequentSequenceForEachLength; i++)
			nextFrequentSequences[i] = NULL;

		numberOfNextFrequentSequence = 0;

//#pragma omp parallel for private(i) private(j) private(joinedCandidate) shared(abort1) shared(abort2)
		for(i=0; i<numberOfCurrentFrequentSequence; i++) {
//#pragma omp critical(cnt)
			cnt++;
#ifdef debug
	printf("three and more freq seq generation, in outer loop: %d\n", cnt);
#endif
			if(previousFrequentSequences[i] == NULL)
				abort1 = true;
			if(abort1 == false) {
				for(j=0; j<numberOfCurrentFrequentSequence; j++) {
					if(previousFrequentSequences[j] == NULL)
						abort2 = true;
					if(abort2 == false) {
					joinedCandidate = candidateJoin(&previousFrequentSequences[i], previousFrequentSequences[j]);

					if (joinedCandidate != NULL) {  //joinedCandidate is NULL if two candidates cannot get joined, otherwise joined candidate
					if(out_write){
						fprintf(candFile, "%s\n", sequenceSignature(joinedCandidate, patternLengthIterator+3));
					}
						if (pruning(joinedCandidate, patternLengthIterator+3) == false) {
							if(searchCandidateInSequences(joinedCandidate) >= frequencyNum) {

								char* frequentCand = sequenceSignature(joinedCandidate, patternLengthIterator+3);

								addToFrequentCandidateHashTable(frequentCand);
								if(out_write){
									fprintf(outFile, "%s\n", frequentCand);
								}


//#pragma omp flush(numberOfNextFrequentSequence)
								nextFrequentSequences[numberOfNextFrequentSequence] = joinedCandidate;

								numberOfNextFrequentSequence++;
//#pragma omp flush(numberOfNextFrequentSequence)

							}
						}
						}
					}
					abort2 = false;
				}
				}
			abort1 = false;
		}

		//fprintf(outFile, "%d\n", numberOfNextFrequentSequence);
#ifdef debug
	printf("Number of k length frequent sequences: %d\n", numberOfNextFrequentSequence);
#endif
		if(nextFrequentSequences[0] == NULL)
			previousFrequentSequences = NULL;
		else
			previousFrequentSequences = nextFrequentSequences;

		patternLengthIterator++;
       if(out_write){
	   fclose(candFile);
	   }
	}
}

struct itemsets* candidateJoin(struct itemsets** firstCand, struct itemsets* secondCand)
{
	struct itemsets* lastItemsetInFirstCandidatePtr = NULL;
	struct items* lastItemInLastItemsetInFirstCandidatePtr = NULL;
	//struct itemsets* firstCandCopy = NULL;
	struct itemsets* firstCandTmp = NULL;
	struct itemsets* joinedCandidate = NULL;
	bool isIndependent = false;
	uint32_t lastItemSecondCandidate = 0;
	struct items* firstItemInfirstCandidatePtr = (*firstCand)->itemsetStart;

	if(*firstCand == secondCand)
	{
		joinedCandidate = copySequence(*firstCand);
		firstItemInfirstCandidatePtr = joinedCandidate->itemsetStart;
		disappearItemFromItemSet(joinedCandidate, joinedCandidate->itemsetStart);
		if ((lastItemSecondCandidate = compareTwoSequenceIgnoringLastItemInSecondSequence(joinedCandidate, secondCand, &isIndependent, &lastItemsetInFirstCandidatePtr, &lastItemInLastItemsetInFirstCandidatePtr)) == 0)
			{
				freeSequence(joinedCandidate);
				return NULL;
			}
			appearItemToItemSet(firstItemInfirstCandidatePtr, joinedCandidate); /* appear to joinedCandidate */

			if(isIndependent == true)
				{
					addItemSet(&lastItemsetInFirstCandidatePtr);
					addItemToBeginning(lastItemSecondCandidate, lastItemsetInFirstCandidatePtr);
				}
			else
				{
					addItemToEndOfList(lastItemSecondCandidate, &lastItemInLastItemsetInFirstCandidatePtr, lastItemsetInFirstCandidatePtr);
				}
			//displaySequencesList(&joinedCandidate, 1);
				return joinedCandidate;
	}


	/* if firstCand != secondCand */
	disappearItemFromItemSet(*firstCand, (*firstCand)->itemsetStart);

	if ((lastItemSecondCandidate = compareTwoSequenceIgnoringLastItemInSecondSequence(*firstCand, secondCand, &isIndependent, &lastItemsetInFirstCandidatePtr, &lastItemInLastItemsetInFirstCandidatePtr)) == 0)
	{
		appearItemToItemSet(firstItemInfirstCandidatePtr, *firstCand);
		return NULL;
	}
	appearItemToItemSet(firstItemInfirstCandidatePtr, *firstCand);
	firstCandTmp = copySequence(*firstCand);

	if(isIndependent == true)
	{
		addItemSet(&lastItemsetInFirstCandidatePtr);
		addItemToBeginning(lastItemSecondCandidate, lastItemsetInFirstCandidatePtr);
	}
	else
	{
		addItemToEndOfList(lastItemSecondCandidate, &lastItemInLastItemsetInFirstCandidatePtr, lastItemsetInFirstCandidatePtr);
		//addToItemListWithOrder(lastItemSecondCandidate, lastItemsetInFirstCandidatePtr);
	}
	joinedCandidate = *firstCand;
	*firstCand = firstCandTmp;

	//displaySequencesList(&joinedCandidate, 1);
	return joinedCandidate;
}

/* Return true if this sequence is pruned which means that it is not a frequent sequence, otherwise false which means that it can be a potential frequent sequence. */
bool pruning(struct itemsets* joinedCandidate, uint32_t length)
{
	struct items* lastItemPtr = NULL;
	struct itemsets* lastItemsetPtr = joinedCandidate;

	while(lastItemsetPtr != NULL)
	{
		lastItemPtr = lastItemsetPtr->itemsetStart;
		while(lastItemPtr != NULL)
		{
			disappearItemFromItemSet(lastItemsetPtr, lastItemPtr);

			struct frequentCandidate*  findInFrequentTable = NULL;
			//displaySequencesList(&joinedCandidate, 1);
			findInFrequentTable = findFrequentCandidateHashTable(sequenceSignature(joinedCandidate, length));
			if(findInFrequentTable == NULL)
				return true; /* This candidate should be pruned (delete from candidate set) */

			appearItemToItemSet(lastItemPtr, lastItemsetPtr);

			//displaySequencesList(&joinedCandidate, 1);

			lastItemPtr = lastItemPtr->next;
		}
		lastItemsetPtr = lastItemsetPtr->next;
	}
	return false;
}


