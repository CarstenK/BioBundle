#!/usr/bin/env bats


@test "renaming test" {
	../../build/seqSetMod -i data/rename.fa -r data/rename.txt -o renameRes.fa
	run diff results/rename.fa renameRes.fa
	[ $status = 0 ]
}


@test "translation test" {
	../../build/seqSetMod -i data/cds.fa -o aa.fa -t -C -s --cds-out cds_new.fa -R "gnl|Acep_1.0|"
	run diff results/aa.fa aa.fa
	[ $status = 0 ]
	run diff results/cds_new.fa cds_new.fa
	[ $status = 0 ]
}

@test "error messages" {
	program=../../build/seqSetMod
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
