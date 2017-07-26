#!/bin/sh

# Run with tightest scoring rules
for bf in NSW2015/*ballots.txt; do
	bn=${bf##*/}
	./marginirv -ballots ${bf} -score -tight > tgt_margin_${bn}
done


# Run with tightest scoring rules
for bf in NSW2015/*ballots.txt; do
	bn=${bf##*/}
	./marginirv -ballots ${bf} -score -tight -electonly 2 LAB CLP > tgt_LAB_CLP_margin_${bn}
done

# Run with tightest scoring rules
for bf in NSW2015/*ballots.txt; do
	bn=${bf##*/}
	./marginirv -ballots ${bf} -score -tight -electonly 3 LAB CLP GRN > tgt_LAB_CLP_GRN_margin_${bn}
done


