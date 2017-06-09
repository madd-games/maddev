# Makefile for the assembler. Prefix all custom variables with AS_* !
AS_SRC := $(shell find $(SRCDIR)/as/common -name '*.c') $(shell find $(SRCDIR)/as/$(AS_FAMILY) -name '*.c')
AS_OBJ := $(patsubst $(SRCDIR)/as/%.c, as/%.o, $(AS_SRC))
AS_DEP := $(AS_OBJ:.o=.d)
AS_CFLAGS := -I. -I$(SRCDIR)/as/common -I$(SRCDIR)/as/$(AS_FAMILY) -Wall -Werror $(AS_EXTRA_CFLAGS) -I$(SRCDIR)/libobj/common -I$(SRCDIR)/libobj/$(LIBOBJ_BACKEND) -I$(SRCDIR)/liblex/common -I$(SRCDIR)/libcommon/common -I$(SRCDIR)/libcommon/$(LIBCOMMON_HOSTLIB) -ggdb
AS_LDFLAGS := -Lout -lobj -llex -lcommon $(LIBCOMMON_EXTRA_LDFLAGS) -ggdb

out/as: $(AS_OBJ) out/libobj.a out/liblex.a out/libcommon.a
	@mkdir -p out
	$(HOSTCC) -o $@ $(AS_OBJ) $(AS_LDFLAGS)

-include $(AS_DEP)

as/%.d: $(SRCDIR)/as/%.c
	@mkdir -p $(dir $@)
	$(HOSTCC) -c $< -MM -MT $(subst .d,.o,$@) -o $@ $(AS_CFLAGS)

as/%.o: $(SRCDIR)/as/%.c
	@mkdir -p $(dir $@)
	$(HOSTCC) -c $< -o $@ $(AS_CFLAGS)

