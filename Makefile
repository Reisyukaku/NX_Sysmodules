MODULES := libraries loader pm sm boot rnx_mitm spl eclct.stub ro creport fatal dmnt boot2 ncm Common/exosphere

SUBFOLDERS := $(MODULES)

TOPTARGETS := all clean

$(TOPTARGETS): $(SUBFOLDERS)

$(SUBFOLDERS):
	$(MAKE) -C $@ $(MAKECMDGOALS)

.PHONY: $(TOPTARGETS) $(SUBFOLDERS)

MV:
	@mkdir Sysmodules
	@mkdir Sysmodules/contents/
	@mkdir Sysmodules/sysmodules/
	@mkdir Sysmodules/sysmodules.dis/
	@mkdir -p Sysmodules/contents/0100000000000008
	@mkdir -p Sysmodules/contents/010000000000000D
	@mkdir -p Sysmodules/contents/0100000000000032
	@mkdir -p Sysmodules/contents/0100000000000034
	@mkdir -p Sysmodules/contents/0100000000000036
	@mkdir -p Sysmodules/contents/0100000000000037
	@cp boot2/boot2.nsp Sysmodules/contents/0100000000000008/exefs.nsp
	@cp dmnt/dmnt.nsp Sysmodules/contents/010000000000000D/exefs.nsp
	@cp eclct.stub/eclct.stub.nsp Sysmodules/contents/0100000000000032/exefs.nsp
	@cp fatal/fatal.nsp Sysmodules/contents/0100000000000034/exefs.nsp
	@cp creport/creport.nsp Sysmodules/contents/0100000000000036/exefs.nsp
	@cp ro/ro.nsp Sysmodules/contents/0100000000000037/exefs.nsp
	@cp rnx_mitm/rnx_mitm.kip Sysmodules/sysmodules/rnx_mitm.kip
	@cp loader/loader.kip Sysmodules/sysmodules/loader.kip
	@cp boot/boot.kip Sysmodules/sysmodules/boot.kip
	@cp pm/pm.kip Sysmodules/sysmodules/pm.kip
	@cp sm/sm.kip Sysmodules/sysmodules/sm.kip
	@cp spl/spl.kip Sysmodules/sysmodules/spl.kip
	@cp ncm/ncm.kip Sysmodules/sysmodules.dis/ncm.kip
	@cp Common/exosphere/exosphere.bin Sysmodules/secmon.bin
	@cp Common/exosphere/lp0fw/lp0fw.bin Sysmodules/warmboot.bin
	@mkdir -p Sysmodules/contents/0100000000000032/flags
	@mkdir -p Sysmodules/contents/0100000000000037/flags
	@touch Sysmodules/contents/0100000000000032/flags/boot2.flag
	@touch Sysmodules/contents/0100000000000037/flags/boot2.flag