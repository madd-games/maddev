# Makefile for the common interfaces library. Prefix all custom variables with LIBCOMMON_* !
LIBCOMMON_SRC := $(shell find $(SRCDIR)/libcommon/common -name '*.c') $(shell find $(SRCDIR)/libcommon/$(LIBCOMMON_HOSTLIB) -name '*.c')
LIBCOMMON_OBJ := $(patsubst $(SRCDIR)/libcommon/%.c, libcommon/%.o, $(LIBCOMMON_SRC))
LIBCOMMON_DEP := $(LIBCOMMON_OBJ:.o=.d)
LIBCOMMON_CFLAGS := -I. -I$(SRCDIR)/libcommon/common -I$(SRCDIR)/libcommon/$(LIBCOMMON_HOSTLIB) -Wall -Werror -ggdb
LIBCOMMON_LDFLAGS := -ggdb

out/libcommon.a: $(LIBCOMMON_OBJ)
	@mkdir -p out
	$(HOSTAR) rcs $@ $(LIBCOMMON_OBJ)

-include $(LIBCOMMON_DEP)

libcommon/%.d: $(SRCDIR)/libcommon/%.c
	@mkdir -p $(dir $@)
	$(HOSTCC) -c $< -MM -MT $(subst .d,.o,$@) -o $@ $(LIBCOMMON_CFLAGS)

libcommon/%.o: $(SRCDIR)/libcommon/%.c
	@mkdir -p $(dir $@)
	$(HOSTCC) -c $< -o $@ $(LIBCOMMON_CFLAGS)

