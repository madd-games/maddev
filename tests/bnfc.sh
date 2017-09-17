# Unit tests for the parser generator "bnfc"
# See tests.sh for more info.
test_section "Parser Generator"

do_unit_test "cat ${testdir}/grammar.bnf | build/bnfc > grammar.c" "generate parser for test grammar"
do_unit_test "cc -c grammar.c -o grammar.o -I${testdir} -I${testdir}/../liblex/common -I${testdir}/../libcommon/common -I${testdir}/../libcommon/posix" "compile the parser"

# Run various tests with parsing
do_parse_test() {
	do_unit_test "cc grammar.c ${testdir}/$1 -o do_test -I${testdir} -I${testdir}/../liblex/common -I${testdir}/../libcommon/common -I${testdir}/../libcommon/posix -Lout -llex -lcommon -pthread && ./do_test" "$2"
}

do_parse_test "bnf-tokenize.c" "tokenize math expression"
do_parse_test "bnf-tree.c" "generate valid parse tree"
