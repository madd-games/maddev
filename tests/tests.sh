# Run all the unit tests.
testdir="`dirname $0`"
echo "*** TESTS START (testdir=$testdir) ***" > tests.log

RED='\033[0;31m'
GREEN='\033[0;32m'
BLUE='\033[1;34m'
NC='\033[0m' # No Color

# USAGE: do_unit_test <command> <test-name>
do_unit_test() {
	printf "${BLUE}TEST${NC} '$2'... "
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

# USAGE: test_section <CAPTION>
test_section() {
	echo "${BLUE}Testing '$1'...${NC}"
	echo >>tests.log "*** TESTING MODULE '$1' ..."
}

# Run all the unit tests
. ${testdir}/liblex.sh
. ${testdir}/as-${AS_FAMILY}.sh
. ${testdir}/bnfc.sh

# Keep this at the very end!
echo "${GREEN}ALL UNIT TESTS PASSED.${NC}"
