# invoke SourceDir generated makefile for release.pem4f
release.pem4f: .libraries,release.pem4f
.libraries,release.pem4f: package/cfg/release_pem4f.xdl
	$(MAKE) -f C:\Users\TOMVAN~1\workspace_v8\cli_ftd_CC26X2R1_LAUNCHXL_tirtos_ccs/src/makefile.libs

clean::
	$(MAKE) -f C:\Users\TOMVAN~1\workspace_v8\cli_ftd_CC26X2R1_LAUNCHXL_tirtos_ccs/src/makefile.libs clean

