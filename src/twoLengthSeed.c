/*
 * towLengthSeed.c
 *
 *  Created on: Mar 1, 2015
 *      Author: Elaheh Sadredini
 */

#include "twoLengthSeed.h"

extern struct oneLengthCandidate* oneLengthCandTable;

uint32_t* count;

void generateTwoLenghtFreqSeq()
{
	int i=0;
	struct oneLengthCandidate* oneLengthCand1 = NULL; /* iterator in the one-length frequent sequence in the hash table */
	struct oneLengthCandidate* oneLengthCand2 = NULL; /* iterator in the one-length frequent sequence in the hash table */

	struct itemsets* lastItemset = NULL;
	struct items* lastItem = NULL;

	numberOfTowLengthCandidates = (numberOfFirstSeed*numberOfFirstSeed)+((numberOfFirstSeed*(numberOfFirstSeed-1))/2);

	twoLengthCandidate =(struct itemsets**) malloc(numberOfTowLengthCandidates*sizeof(struct itemsets*));
	uint32_t k=0;
	for(k=0; k<numberOfTowLengthCandidates; k++) {
		twoLengthCandidate[k] = NULL;
	}
	assert(twoLengthCandidate != NULL && "unsuccessful memory allocation");
#ifdef debug
	printf("Number of two length candidates: %d\n", numberOfTowLengthCandidates);
#endif

	for (oneLengthCand1 = oneLengthCandTable; oneLengthCand1 != NULL; oneLengthCand1 = (struct oneLengthCandidate*)(oneLengthCand1->hh.next))
	{
		for (oneLengthCand2 = oneLengthCandTable; oneLengthCand2 != NULL; oneLengthCand2 = (struct oneLengthCandidate*)(oneLengthCand2->hh.next))
		{
			addFirstItemSet(&twoLengthCandidate[i]);
			lastItemset = twoLengthCandidate[i];
			addItemToEndOfList(oneLengthCand1->item, &lastItem, lastItemset);
			addItemSet(&lastItemset);
			addItemToEndOfList(oneLengthCand2->item, &lastItem, lastItemset);
			i++;
		}
	}

	int a =0;
	for (oneLengthCand1 = oneLengthCandTable; oneLengthCand1 != NULL; oneLengthCand1 = (struct oneLengthCandidate*)(oneLengthCand1->hh.next))
		{
			for (oneLengthCand2 = (oneLengthCand1->hh.next); oneLengthCand2 != NULL; oneLengthCand2 = (struct oneLengthCandidate*)(oneLengthCand2->hh.next))
			{
				addFirstItemSet(&twoLengthCandidate[i]);
				lastItemset = twoLengthCandidate[i];
				if(oneLengthCand1->item > oneLengthCand2->item)
				{
					addItemToEndOfList(oneLengthCand2->item, &lastItem, lastItemset);
					addItemToEndOfList(oneLengthCand1->item, &lastItem, lastItemset);
				}
				else
				{
				addItemToEndOfList(oneLengthCand1->item, &lastItem, lastItemset);
				addItemToEndOfList(oneLengthCand2->item, &lastItem, lastItemset);
				}
				//displaySequencesList(&twoLengthCandidate[i], 1);
				i++;
			}
			a++;
		}

	//displaySequencesList(twoLengthCandidate, numberOfTowLengthCandidates);
	//printf("i%d\n", i);

	//////////////////////////////////////////////
	////////////Printing info/////////////////////
    int ii;
	
	if(cand_write){
		if ((candFile = fopen("intermidiate-size2.txt", "w")) == NULL ) {
		fprintf(stderr, "candidate File could not be open");
		exit(1);
	}}
		
		for (ii=0; ii<numberOfTowLengthCandidates; ii++){
				char* cand = sequenceSignature(twoLengthCandidate[ii], 2);
				fprintf(candFile, "%s\n", cand);
		}

		
		
	/* Candidate selection phase */
	count = (uint32_t*)malloc(numberOfTowLengthCandidates*sizeof(uint32_t));
	assert(count!=NULL && "unsuccessful memory allocation");
	uint32_t j;
	numberOfTowLengthSeeds = 0;

	matching_time = 0;
	//printf("%d\n", numberOfTowLengthCandidates);
    
//#pragma omp parallel for private(j)
	for (j=0; j<numberOfTowLengthCandidates; j++)
	{
		count[j] = 0;
		//displaySequencesList(&twoLengthCandidate[j], 1);
		
		count[j] = searchCandidateInSequences(twoLengthCandidate[j]);
		
		if(count[j] < frequencyNum)
		{
			twoLengthCandidate[j] = NULL;
		}
		else {
//#pragma omp atomic
			numberOfTowLengthSeeds++;
#ifdef debug
	printf("Number of two length seeds: %d\n", numberOfTowLengthSeeds);
#endif
		}
	}

	fprintf(outFile, "%d\n", numberOfTowLengthSeeds);

	twoLengthSeeds =(struct itemsets**) malloc(numberOfTowLengthSeeds*sizeof(struct itemsets*));
	assert(twoLengthSeeds!=NULL && "unsuccessful memory allocation");

	k = 0;
		 //initialization;
			for(j = 0; j<numberOfTowLengthSeeds; j++)
			{
				twoLengthSeeds[j] = NULL;
			}
			
			for (j=0; j<numberOfTowLengthCandidates; j++)
			{
				if(twoLengthCandidate[j] == NULL)
					continue;

				twoLengthSeeds[k] = twoLengthCandidate[j];

				char* freqCand = sequenceSignature(twoLengthCandidate[j], 2);
				addToFrequentCandidateHashTable(freqCand);
				
				if(out_write){
				fprintf(outFile, "%s\n", freqCand);
				}				

				k++;
			}
			//printFrequentCandidateHashTable();
    if(cand_write){
		fclose(candFile);
	}
}
