#!/usr/bin/env bats

@test "name extraction" {
	result="$(../../build/seqSetInfo -i data/seqStatInfo.fa -l -c)"
	val=$'seq1\nLength: 4\nseq2\nLength: 5\n\nSummary:\nLength:\nmin length   : 4\nmax length   : 5\ntotal length : 9\navg length   : 4.5\n\nChar stat:\nA 3 33.3%\nC 1 11.1%\nT 5 55.6%'
	[ "$result" == "$val" ]
}


@test "error messages" {
	program=../../build/seqSetInfo
	run $program
	[ "$status" -eq 1 ]
	[ "$output" = $'Error parsing commandline: the option \'--in\' is required but missing\nUse -h/--help for option information' ]

	run $program -xyz
	[ "$status" -eq 1 ]
	[ "$output" = $'Error parsing commandline: unrecognised option \'-xyz\'\nUse -h/--help for option information' ]

	run $program -h
	[ "$status" -eq 0 ]

	run $program -i notExisting
	[ "$status" -eq 1 ]

}
