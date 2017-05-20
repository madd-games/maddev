# Unit tests for liblex.
# See tests.sh for more info.
do_unit_test "cc -I${testdir}/../liblex/common -I${testdir}/../libcommon/common -I${testdir}/../libcommon/${LIBCOMMON_HOSTLIB} -Lout ${testdir}/lexCompileRegex.c -o lexCompileRegex -llex -lcommon ${LIBCOMMON_LDFLAGS} && ./lexCompileRegex" "lexCompileRegex"
do_unit_test "cc -I${testdir}/../liblex/common -I${testdir}/../libcommon/common -I${testdir}/../libcommon/${LIBCOMMON_HOSTLIB} -Lout ${testdir}/lexMatch.c -o lexMatch -llex -lcommon ${LIBCOMMON_LDFLAGS} && ./lexMatch" "lexMatch"
do_unit_test "cc -I${testdir}/../liblex/common -I${testdir}/../libcommon/common -I${testdir}/../libcommon/${LIBCOMMON_HOSTLIB} -Lout ${testdir}/lexTokenize.c -o lexTokenize -llex -lcommon ${LIBCOMMON_LDFLAGS} && ./lexTokenize" "lexTokenize"
