.PHONY: all
all: out/mac out/libobj.a out/embed

# Include all other build files here
include $(SRCDIR)/mac/mac.mk
include $(SRCDIR)/libobj/libobj.mk
include $(SRCDIR)/embed/embed.mk
