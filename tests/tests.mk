# Unit tests

tests:
	@LIBCOMMON_HOSTLIB=$(LIBCOMMON_HOSTLIB) LIBCOMMON_LDFLAGS=$(LIBCOMMON_EXTRA_LDFLAGS) sh $(SRCDIR)/tests/tests.sh