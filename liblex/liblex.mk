# Makefile for the lexer library. Prefix all custom variables with LIBLEX_* !
LIBLEX_SRC := $(shell find $(SRCDIR)/liblex/common -name '*.c') $(shell find $(SRCDIR)/liblex/$(LIBLEX_BACKEND) -name '*.c')
LIBLEX_OBJ := $(patsubst $(SRCDIR)/liblex/%.c, liblex/%.o, $(LIBLEX_SRC))
LIBLEX_DEP := $(LIBLEX_OBJ:.o=.d)
LIBLEX_CFLAGS := -I. -I$(SRCDIR)/liblex/common -I$(SRCDIR)/libcommon/$(LIBCOMMON_HOSTLIB) -I$(SRCDIR)/libcommon/common -Wall -Werror -ggdb
LIBLEX_LDFLAGS := -ggdb

out/liblex.a: $(LIBLEX_OBJ) out/libcommon.a
	@mkdir -p out
	$(HOSTAR) rcs $@ $(LIBLEX_OBJ)

-include $(LIBLEX_DEP)

liblex/%.d: $(SRCDIR)/liblex/%.c
	@mkdir -p $(dir $@)
	$(HOSTCC) -c $< -MM -MT $(subst .d,.o,$@) -o $@ $(LIBLEX_CFLAGS)

liblex/%.o: $(SRCDIR)/liblex/%.c
	@mkdir -p $(dir $@)
	$(HOSTCC) -c $< -o $@ $(LIBLEX_CFLAGS)

