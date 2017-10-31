#!/usr/bin/env bats



@test "remove stops" {
	result="$(../../build/stopCleaner -i data/cleanProt.fa)"
	val=$'>1\naaaa\n>2\naaaa\n>3\na.aa\n>4\na.aaa\n>5\naaaaaaaaaaaaaa'
	[ "$result" == "$val" ]
}

@test "remove pseudogenes" {
	result="$(../../build/stopCleaner -r -i data/cleanProt.fa)"
	val=$'>1\naaaa\n>2\naaaa\n>5\naaaaaaaaaaaaaa'
	[ "$result" == "$val" ]

	run ../../build/stopCleaner -r -i data/cleanProt.fa -k 3 -R U
	[ "$status" -eq 0 ]
	[ "$output" = $'>1\naaaa\n>2\naaaa\n>3\naUaa\n>5\naaaaaaaaaaaaaa' ]
	
	run ../../build/stopCleaner -r -i data/cleanProt.fa -K data/keep.txt -R U
	[ "$status" -eq 0 ]
	[ "$output" = $'>1\naaaa\n>2\naaaa\n>3\naUaa\n>5\naaaaaaaaaaaaaa' ]
}

@test "error messages" {
	program=../../build/stopCleaner

	run $program -xyz
	[ "$status" -eq 1 ]
	[ "$output" = $'Error parsing commandline: unrecognised option \'-xyz\'\nUse -h/--help for option information' ]

	run $program -h
	[ "$status" -eq 0 ]

	run $program -i notExisting
	[ "$status" -eq 1 ]
}
