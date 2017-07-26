Margin Computation for IRV Elections
------------------------------------

USAGE: 
marginirv -ballots [ballot file] [-score] [-tight] [-simlog] [-optlog]
    [-tlimit value] [-logfile logfilename] [-electonly N parties]

 -score:     Apply basic scoring rules to prune search
 
 -tight:     Apply tighter scoring rules to prune search (supercedes score)
 
 -simlog:    Print details of IRV simulation of original election to stdout
 
 -optlog:    Print CPLEX solving information to logfile
 
 -tlimit:    Timelimit (in s) after which branch and bound will terminate
 
 -logfile:   File to direct search log
 
 -electonly: [optional] 
             N (number of alternative winners we want to consider)
             Party1 Party2 ... PartyN

             Example: -electonly 2 LIB LAB

Example usage:
--------------

./marginirv -ballots USIRV/Aspen_2009_Mayor.txt -score -tight -logfile log.txt


Notes:
------

This code implements the branch-and-bound margin computation algorithm
described by Blom et al. in "Efficient Computation of Exact IRV Margins"
presented at ECAI, 2016. This work updates the algorithm of Magrino et al
with pruning rules to make search for margins more efficient.

CPLEX and several boost libraries (system and filesystem) are required. The
code could be easily modified to use an alternative solver (eg. Gurobi)
instead as the LP solved in the code is fairly simple. The provided Makefile
is for use on Unix-based systems. On Windows (assuming CPLEX is used), an
alternative Makefile is required. I have included an example in this bundle,
but this will need to be modified to suit your setup. The Microsoft cl compiler
must be used (and must be matched to that used to compile the version of 
CPLEX that you are using). CPLEX is free for academic use. 

Included in this bundle are data files representing several US IRV elections
(in USIRV) and the NSW 2015 House of Reps election (in NSW2015). Margin
results for each seat in the NSW 2015 election are provided in NSW2015_results.

Note that this implementation does not include the 'addition only' or 
'deletion only' settings discussed in the ECAI paper. An older C implementation
was used in the generation of the results for this paper. Some differences 
in the order in which nodes with the same 'scores/lower bounds' are added to
the queue maintained by the algorithm are present between this new code and
the C code (this leads to differences in the number of LPs solved throughout
the algorithm, but makes no difference to the value of the margin computed).  

Please send any enquires (or bug notifications) to:
michelle.blom@unimelb.edu.au
