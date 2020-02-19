======================================================================
                             FLOOD IT
======================================================================

1) Description of implementation
2) Algorithm characteristics
3) Parameters
4) Comments


1) The implementation of a genetic algorithm was sought with the following
characteristics: 
- The initial population is generated with each individual seeking to maximize
the area covered, the differentiation between one solution and another in the
population is made by insertion of random components in the calculation of the
probability of neighboring color be accepted 
- solutions are ordered and only the best ones are influenced by recombination
disturbance and local search. 
- recombination is based on the average probabilities of each parent individual
added a random component. 
- the disturbance occurs by inserting non-consecutive random colors in random
positions of the solution. 
- the local search considers as neighbor of a solution that one in which a
element (colour) is removed.


2) In problems with small trays (30 30 10) the population sizes that suffer
local search as well as recombination and disruption influence the getting
better results in 120 seconds. But on trays large (50 50 10) and (100 100 10)
flags were created in the algorithm for disregarding time disturbance and
recombination, tests with 300 seconds of execution showed a greater efficiency
of a simple local search in the best results of the initial population. The time
indicated in the algorithm starts counting from the initial population
formation.

3) Parameters

	k = 16; // probability weighting
	d = 500; // probability scale
	lambda = 0.1; //diversifier of children
	g_inc = 1; // step of diversifier increment
	inc = 0.2; // diversifier increment factor
	time_t seconds = 120; // timer for test (starting after pop_ini is
	generated)
	int flag = 1; //0 -> disables recombination and disturbance
	t_search = 20; //number of individuals in local search

the flag and t_search values are automatically set to 5 and 1 in the case of
trays with line > 49 and > 99 respectively

4) Taking into account the characteristics of the genetic algorithms the
parameters are set for trays (30 30 10) in which it has been observed as 64 the
minimum number of moves for the board provided by the teacher and values below
that on similar trays. However the The approach does not guarantee efficient
generic application.
