.PHONY: all tests
all: out/mac out/libobj.a out/embed out/libcommon.a out/liblex.a

# Include all other build files here
include $(SRCDIR)/mac/mac.mk
include $(SRCDIR)/libobj/libobj.mk
include $(SRCDIR)/embed/embed.mk
include $(SRCDIR)/libcommon/libcommon.mk
include $(SRCDIR)/liblex/liblex.mk
include $(SRCDIR)/tests/tests.mk
