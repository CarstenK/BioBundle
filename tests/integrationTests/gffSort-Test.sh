#!/usr/bin/env bats



@test "remove stops" {
	program=../../build/gffSort
	$program -i data/unsorted.gff -o testSorted.gff
	run diff testSorted.gff results/sorted.gff
	[ $status = 0 ]

}

@test "error messages" {
	program=../../build/gffSort
	run $program -xyz
	[ "$status" -eq 1 ]
	[ "$output" = $'Error parsing commandline: unrecognised option \'-xyz\'\nUse -h/--help for option information' ]
}
