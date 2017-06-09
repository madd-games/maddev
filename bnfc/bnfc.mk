# Makefile for the BNF compiler (parse generator).
BNFC_CFLAGS := -I. -I$(SRCDIR)/bnfc -Wall -Werror -I$(SRCDIR)/liblex/common -I$(SRCDIR)/libcommon/common -I$(SRCDIR)/libcommon/$(LIBCOMMON_HOSTLIB) -ggdb
BNFC_LDFLAGS := -Lout -llex -lcommon $(LIBCOMMON_EXTRA_LDFLAGS) -ggdb

build/bnfc: $(SRCDIR)/bnfc/bnfc.c out/liblex.a out/libcommon.a
	@mkdir -p build
	$(BUILDCC) $< -o $@ $(BNFC_CFLAGS) $(BNFC_LDFLAGS)

out/bnfc: $(SRCDIR)/bnfc/bnfc.c out/liblex.a out/libcommon.a
	@mkdir -p out
	$(HOSTCC) $< -o $@ $(BNFC_CFLAGS) $(BNFC_LDFLAGS)
