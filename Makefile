MODULES := libraries loader pm sm boot rnx_mitm spl eclct.stub ro creport fatal dmnt boot2 ncm Common/exosphere

SUBFOLDERS := $(MODULES)

TOPTARGETS := all clean

$(TOPTARGETS): $(SUBFOLDERS)

$(SUBFOLDERS):
	$(MAKE) -C $@ $(MAKECMDGOALS)

.PHONY: $(TOPTARGETS) $(SUBFOLDERS)