KIPS := loader sm fs_mitm

SUBFOLDERS := libstratosphere $(KIPS)

TOPTARGETS := all clean

$(TOPTARGETS): $(SUBFOLDERS)

$(SUBFOLDERS):
	$(MAKE) -C $@ $(MAKECMDGOALS)

$(KIPS): libstratosphere

.PHONY: $(TOPTARGETS) $(SUBFOLDERS)
