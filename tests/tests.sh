# Run all the unit tests.
testdir="`dirname $0`"
echo "*** TESTS START (testdir=$testdir) ***" > tests.log

RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color

# USAGE: do_unit_test <command> <test-name>
do_unit_test() {
	printf "TEST '$2'... "
	echo >>tests.log "*** RUNNING TEST '$2'..."
	echo >>tests.log "\$ $1"
	sh -c "$1" >>tests.log 2>&1 || {
		echo >>tests.log "*** TEST FAILED"
		echo "${RED}FAIL${NC}"
		echo "${RED}UNIT TESTS FAILED!${NC}"
		echo "${RED}See tests.log for more info.${NC}"
		exit
	}
	
	echo "${GREEN}OK${NC}"
}

# Run all the unit tests
. ${testdir}/liblex.sh

# Keep this at the very end!
echo "${GREEN}ALL UNIT TESTS PASSED.${NC}"
