# Makefile for the embedder. Prefix all custom variables with EMBED_* !
EMBED_SRC := $(shell find $(SRCDIR)/embed/common -name '*.c')
EMBED_OBJ := $(patsubst $(SRCDIR)/embed/%.c, embed/%.o, $(EMBED_SRC))
EMBED_DEP := $(EMBED_OBJ:.o=.d)
EMBED_CFLAGS := -I. -I$(SRCDIR)/embed/common -Wall -Werror -I$(SRCDIR)/libobj/common -I$(SRCDIR)/libobj/$(LIBOBJ_BACKEND)
EMBED_LDFLAGS := -Lout -lobj

out/embed: $(EMBED_OBJ) out/libobj.a
	@mkdir -p out
	$(HOSTCC) -o $@ $(EMBED_OBJ) $(EMBED_LDFLAGS)

-include $(EMBED_DEP)

embed/%.d: $(SRCDIR)/embed/%.c
	@mkdir -p $(dir $@)
	$(HOSTCC) -c $< -MM -MT $(subst .d,.o,$@) -o $@ $(EMBED_CFLAGS)

embed/%.o: $(SRCDIR)/embed/%.c
	@mkdir -p $(dir $@)
	$(HOSTCC) -c $< -o $@ $(EMBED_CFLAGS)

