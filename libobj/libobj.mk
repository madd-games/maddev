# Makefile for the object handler library. Prefix all custom variables with LIBOBJ_* !
LIBOBJ_SRC := $(shell find $(SRCDIR)/libobj/common -name '*.c') $(shell find $(SRCDIR)/libobj/$(LIBOBJ_BACKEND) -name '*.c')
LIBOBJ_OBJ := $(patsubst $(SRCDIR)/libobj/%.c, libobj/%.o, $(LIBOBJ_SRC))
LIBOBJ_DEP := $(LIBOBJ_OBJ:.o=.d)
LIBOBJ_CFLAGS := -I. -I$(SRCDIR)/libobj/common -I$(SRCDIR)/libobj/$(LIBOBJ_BACKEND) -Wall -Werror $(LIBOBJ_EXTRA_CFLAGS) -ggdb
LIBOBJ_LDFLAGS := -ggdb

out/libobj.a: $(LIBOBJ_OBJ)
	@mkdir -p out
	$(HOSTAR) rcs $@ $(LIBOBJ_OBJ)

-include $(LIBOBJ_DEP)

libobj/%.d: $(SRCDIR)/libobj/%.c
	@mkdir -p $(dir $@)
	$(HOSTCC) -c $< -MM -MT $(subst .d,.o,$@) -o $@ $(LIBOBJ_CFLAGS)

libobj/%.o: $(SRCDIR)/libobj/%.c
	@mkdir -p $(dir $@)
	$(HOSTCC) -c $< -o $@ $(LIBOBJ_CFLAGS)

