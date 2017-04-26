# Makefile for the macro-compiler. Prefix all custom variables with MAC_* !
MAC_SRC := $(shell find $(SRCDIR)/mac/common -name '*.c') $(shell find $(SRCDIR)/mac/arch/$(MAC_ARCHDIR) -name '*.c')
MAC_OBJ := $(patsubst $(SRCDIR)/mac/%.c, mac/%.o, $(MAC_SRC))
MAC_DEP := $(MAC_OBJ:.o=.d)
MAC_CFLAGS := -I. -I$(SRCDIR)/mac/common -I$(SRCDIR)/mac/arch/$(MAC_ARCHDIR) -Wall -Werror $(MAC_EXTRA_CFLAGS)
MAC_LDFLAGS :=

out/mac: $(MAC_OBJ)
	@mkdir -p out
	$(HOSTCC) -o $@ $(MAC_OBJ) $(MAC_LDFLAGS)

-include $(MAC_DEP)

mac/%.d: $(SRCDIR)/mac/%.c
	@mkdir -p $(dir $@)
	$(HOSTCC) -c $< -MM -MT $(subst .d,.o,$@) -o $@ $(MAC_CFLAGS)

mac/%.o: $(SRCDIR)/mac/%.c
	@mkdir -p $(dir $@)
	$(HOSTCC) -c $< -o $@ $(MAC_CFLAGS)

