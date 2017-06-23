# Makefile for the object dumper. Prefix all custom variables with OBJDUMP_* !
OBJDUMP_SRC := $(shell find $(SRCDIR)/objdump/common -name '*.c')
OBJDUMP_OBJ := $(patsubst $(SRCDIR)/objdump/%.c, objdump/%.o, $(OBJDUMP_SRC))
OBJDUMP_DEP := $(OBJDUMP_OBJ:.o=.d)
OBJDUMP_CFLAGS := -I. -I$(SRCDIR)/objdump/common -Wall -Werror -I$(SRCDIR)/libobj/common -I$(SRCDIR)/libobj/$(LIBOBJ_BACKEND)
OBJDUMP_LDFLAGS := -Lout -lobj

out/objdump: $(OBJDUMP_OBJ) out/libobj.a
	@mkdir -p out
	$(HOSTCC) -o $@ $(OBJDUMP_OBJ) $(OBJDUMP_LDFLAGS)

-include $(OBJDUMP_DEP)

objdump/%.d: $(SRCDIR)/objdump/%.c
	@mkdir -p $(dir $@)
	$(HOSTCC) -c $< -MM -MT $(subst .d,.o,$@) -o $@ $(OBJDUMP_CFLAGS)

objdump/%.o: $(SRCDIR)/objdump/%.c
	@mkdir -p $(dir $@)
	$(HOSTCC) -c $< -o $@ $(OBJDUMP_CFLAGS)

