# Makefile for the linker. Prefix all custom variables with LD_* !
LD_SRC := $(shell find $(SRCDIR)/ld/common -name '*.c')
LD_BNF := $(shell find $(SRCDIR)/ld/common -name '*.bnf')
LD_OBJ := $(patsubst $(SRCDIR)/ld/%.c, ld/%.o, $(LD_SRC)) $(patsubst $(SRCDIR)/ld/%.bnf, ld-bnf/%.o, $(LD_BNF))
LD_DEP := $(LD_OBJ:.o=.d)
LD_CFLAGS := -I. -I$(SRCDIR)/ld/common -Wall -Werror -I$(SRCDIR)/libobj/common -I$(SRCDIR)/libobj/$(LIBOBJ_BACKEND) -I$(SRCDIR)/liblex/common -I$(SRCDIR)/libcommon/common -I$(SRCDIR)/libcommon/$(LIBCOMMON_HOSTLIB)
LD_LDFLAGS := -Lout -lobj -llex -lcommon $(LIBCOMMON_EXTRA_LDFLAGS)

out/ld: $(LD_OBJ) out/libobj.a out/liblex.a out/libcommon.a
	@mkdir -p out
	$(HOSTCC) -o $@ $(LD_OBJ) $(LD_LDFLAGS)

-include $(LD_DEP)

ld/%.d: $(SRCDIR)/ld/%.c
	@mkdir -p $(dir $@)
	$(HOSTCC) -c $< -MM -MT $(subst .d,.o,$@) -o $@ $(LD_CFLAGS)

ld/%.o: $(SRCDIR)/ld/%.c
	@mkdir -p $(dir $@)
	$(HOSTCC) -c $< -o $@ $(LD_CFLAGS)

ld-bnf/%.o: $(SRCDIR)/ld/%.bnf out/bnfc
	@mkdir -p $(dir $@)
	cat $< | sed 's/\r$//' | out/bnfc > bnf.c
	$(HOSTCC) -c bnf.c -o $@ $(LD_CFLAGS)

