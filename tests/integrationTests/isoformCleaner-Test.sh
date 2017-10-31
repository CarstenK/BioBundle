#!/usr/bin/env bats

@test "clean by split" {
	result="$(../../build/isoformCleaner -i data/isoformTest1.fa -s '-')"
	val=$'>seq1-PB\nAAAAAAAAAAAAA\n>seq2-PA\nAAAAAAAAAAAAAAA'
	[ "$result" == "$val" ]
}


@test "clean by regex" {
	result="$(../../build/isoformCleaner -i data/isoformTest2.fa -r 'gene:(.*)')"
	val=$'>tr2 gene:gene1\nAAAAAAAAAAAAA\n>tr3 gene:gene2\nAAAAAAAAAAAAAAA\n>tr5 gene:gene3\nAAAAAAAAAAAAA'
	[ "$result" == "$val" ]

	result="$(../../build/isoformCleaner -i data/isoformTest2.fa -p gene)"
	val=$'>tr2 gene:gene1\nAAAAAAAAAAAAA\n>tr3 gene:gene2\nAAAAAAAAAAAAAAA\n>tr5 gene:gene3\nAAAAAAAAAAAAA'
	[ "$result" == "$val" ]

}

@test "clean by gff" {
	result="$(../../build/isoformCleaner -i data/isoformTest3.fa -g data/isoformTest3.gff)"
	val=$'>seq2\nAAAAAAAAAAAAA\n>seq3\nAAAAAAAAAAAAAAA'
	[ "$result" == "$val" ]
}


@test "error messages" {
	program=../../build/isoformCleaner
	#run $program
	#[ "$status" -eq 1 ]
	#[ "$output" = $'Error parsing commandline: the option \'--in\' is required but missing\nUse -h/--help for option information' ]

	run $program -xyz
	[ "$status" -eq 1 ]
	[ "$output" = $'Error parsing commandline: unrecognised option \'-xyz\'\nUse -h/--help for option information' ]

	run $program -h
	[ "$status" -eq 0 ]

	run $program -i notExisting
	[ "$status" -eq 1 ]

	run $program -i data/isoformTest2.fa -p gene -o /atrtia
	[ "$status" -eq 1 ]

	run $program -i data/isoformTest2.fa -r 'gennutriaee:(.*)'
	[ "$output" = $'Warning: Regex didn\'t match tr1 gene:gene1\nWarning: Regex didn\'t match tr2 gene:gene1\nWarning: Regex didn\'t match tr3 gene:gene2\nWarning: Regex didn\'t match tr4 gene:gene2\nWarning: Regex didn\'t match tr5 gene:gene3\n>tr1 gene:gene1\nAAAAAAAAAA\n>tr2 gene:gene1\nAAAAAAAAAAAAA\n>tr3 gene:gene2\nAAAAAAAAAAAAAAA\n>tr4 gene:gene2\nAAAAAAAAAAAAA\n>tr5 gene:gene3\nAAAAAAAAAAAAA' ]

	run $program -i data/isoformTest2.fa -p uiartria
	[ "$status" -eq 1 ]

}
