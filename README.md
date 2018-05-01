# SPM

This code implements sequential pattern mining (SPM) algorithm using a breadth-first-search approach. SPM finds the frequent subsequences in a given dataset of sequences. 


## Usage:

`1. make`

`2. ./gsp_cpu <frequency> <input file> <Dumping candidates (yes = 1, No = 0)> <Dumping results or frequent candidates (yes = 1, No = 0)> <Allow gap between itemsets (yes = 1, No = 0)>`


You can set "Allow gap between itemsets" to "0" in order to mine the frequent consequtive itemsets. 


## Input format:

**Sample input file**

        1 2 -1 3 4 -1 -2
        5 6 -1 -2
        -2
        
'-1' is a delimiter between itemsets.

'-2' is a delimiter betweeen sequences.

'-2' should be added to the last line.
        
        
        
**Sample output file**

        1-2--1-3
        5--1


'-' is a delimiter to separate items in one itemset (subsequence).
'--' is a delimiter to separate itemsets in the sequence.



## Performance:

GPU and multi-thread CPU implementation of the code is available. Please contact "elaheh@virginia.edu" for more information. 


## Citations:
Please cite the following papers if you are using this tool for your research. 

[\[1\] Elaheh Sadredini, Reza Rahimi, Ke Wang, and Kevin Skadron. "Frequent Subtree Mining on the Automata Processor: Opportunities
and Challenges." ACM International Conference on Supercomputing (ICS), Chicago, June 2017](http://www.cs.virginia.edu/~skadron/Papers/sadredini_ics17.pdf) 

[\[2\] Ke Wang, Elaheh Sadredini, and Kevin Skadron. "Sequential Pattern Mining with the Micron Automata Processor." ACM International
Conference on Computing Frontiers, Italy, May 2016](http://www.cs.virginia.edu/~skadron/Papers/CF16_SPM_AP.pdf) 
