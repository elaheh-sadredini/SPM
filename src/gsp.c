/** @brief Implementing GSP algorithm
  @author Elaheh Sadredini
  @date May 2015
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/time.h>
#include "oneLengthSeed.h"
#include "twoLengthSeed.h"
#include "threeAndMoreLengthSeed.h"
#include "dataTypes.h"
#include "utility.h"

void run(int argc, char** argv);
void usage(int argc, char** argv);
void readFile(char* fileName);
void generateFrequentSequences();

const uint32_t maxSeqLength = 8000000;

int main(int argc, char** argv) {
#ifdef debug
	printf("Start reading file and making data structure\n");
#endif
	run(argc, argv);
#ifdef debug
	printf("End Reading file\n");
#endif

	struct timeval start, end;
	gettimeofday(&start, NULL );

	generateFrequentSequences();

	gettimeofday(&end, NULL );
	double delta = ((end.tv_sec - start.tv_sec) * 1000000u + end.tv_usec
			- start.tv_usec) / 1.e6;
	printf("\nExecution Total time ~ %f s\n", delta);
	if(out_write){
		fclose(outFile);
        printf("\nAll frequent sequences are saved in finalFreqCand.txt\n\n");
	}

	return 0;
}

void run(int argc, char** argv) {
	char* inputFile = NULL;
	char* candBeforePrune = NULL;

	long double frequency;

	if (argc != 5)
		usage(argc, argv);

	if ((frequency = atof(argv[1])) <= 0)
		usage(argc, argv);

	inputFile = argv[2];
	out_write = atoi(argv[3]);
    gap_between_itemsets = atoi(argv[4]);
	
	if(out_write){
	if ((outFile = fopen("finalFreqCand.txt", "w")) == NULL ) {
		fprintf(stderr, "output file could not be open");
		exit(1);
	}}


	readFile(inputFile);
	frequencyNum = (numberOfSequences * frequency) + 0.99999;
}

void readFile(char* fileName) {
	char line[maxSeqLength];
	char* itemToken = NULL;
	const char delimiter[2] = " ";
	const char* s1 = "-";
	const char* s2 = "1";
	const char* s3 = "2";
	char item = NULL, itemTemp = NULL;
	FILE* inputFile = NULL;

	numberOfSequences = 0;
	inputFile = fopen(fileName, "r");
	assert(inputFile != NULL && "Input file could not be open");

	/* Counting number of sequences in the file */
	while (!feof(inputFile)) {

		itemTemp = item;
		item = fgetc(inputFile);
		//printf("item %d\n", item);
		//printf("itemtemp %d\n", itemTemp);
		if ((itemTemp == '-') && (item == '2')){
			(numberOfSequences)++;
		//printf("%d\n", numberOfSequences);
			}
	}
	assert(numberOfSequences != 0 && "Input file is empty");
	assert(
			numberOfSequences < (1UL << 32) - 1
					&& "Number of sequences are bigger than the biggest number in uint32\n");

	(numberOfSequences)--;
	fclose(inputFile);

	assert(numberOfSequences != (1UL << 32) - 1 && "Input file is empty");

	inputFile = fopen(fileName, "r");
	assert(inputFile != NULL && "Input file could not be open");

	/* Creating data structure for the sequences */
	sequences = (struct itemsets**) malloc(
			numberOfSequences * sizeof(struct itemsets*));
	assert(sequences != NULL && "unsuccessful memory allocation");

	/* initialization */
	uint32_t i;
	for (i = 0; i < numberOfSequences; i++) {
		sequences[i] = NULL;
	}

	//Pointer to the last itemset in the sequence list
	struct itemsets* lastItemset = NULL;
	struct items* lastItem = NULL;

	uint32_t seqIterator;
	for (seqIterator = 0; seqIterator < numberOfSequences; seqIterator++) {
		fgets(line, maxSeqLength, inputFile);
		addFirstItemSet(&sequences[seqIterator]);
		lastItemset = sequences[seqIterator];

		itemToken = strtok(line, delimiter);
		while (itemToken != NULL ) {
			if ((*itemToken == *s1 && *(itemToken + 1) == *s2)
					&& (*(itemToken + 3) == *s1 && *(itemToken + 4) == *s3)) /* if we reach -1 -2 */
					{
				break;
			} else if (*itemToken == *s1 && *(itemToken + 1) == *s2) /* if we reach -1*/
			{
				addItemSet(&lastItemset);
				itemToken = strtok(NULL, delimiter);

			} else {
				addItemToEndOfList(atoi(itemToken), &lastItem, lastItemset);
				itemToken = strtok(NULL, delimiter);
			}
		}
	}
	fclose(inputFile);
}

void usage(int argc, char** argv) {
	fprintf(stderr, "Usage: %s <frequency> <input file> <Dumping results (yes = 1, No = 0)> <Allow gap between itemsets (yes = 1, No = 0)>\n",
			argv[0]);
	exit(1);
}

void generateFrequentSequences() {
#ifdef debug
	printf("Start generating frequent items\n");
#endif
	generateFreqItems();
#ifdef debug
	printf("Start generating two length sequences\n");
#endif
	generateTwoLenghtFreqSeq();
#ifdef debug
	printf("Start generating three and more length sequences\n");
#endif
	generateThreeAndMoreFreqSeq();
}

