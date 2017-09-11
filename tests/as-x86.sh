# Unit tests for the x86 assembler
# See tests.sh for more info.
test_section "x86 Assembler"

# First we test if it can assemble all test instructions in all modes
x86_do_insn_test() {
	bits="$1"
	test_file_command="cat ${testdir}/insn-test-x86.s | grep MODE${bits} > test${bits}.s"
	echo >>tests.log "as-x86.sh: creating test file test${bits}.s using: $test_file_command"
	sh -c "$test_file_command" >>tests.log 2>&1 || {
		echo >>tests.log "TEST GENERATION COMMAND FAILED"
		echo "as-x86.sh: ABORT"
		exit 1
	}
	
	do_unit_test "out/as test${bits}.s" "instruction set in ${bits}-bit mode"
}

x86_do_insn_test 64
