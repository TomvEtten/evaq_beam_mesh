# invoke SourceDir generated makefile for release.pem4f
release.pem4f: .libraries,release.pem4f
.libraries,release.pem4f: package/cfg/release_pem4f.xdl
	$(MAKE) -f /home/tomvanetten/Documents/Workspace/evaq_beam_mesh/temp_sensor_CC26X2R1_LAUNCHXL_tirtos_ccs/src/makefile.libs

clean::
	$(MAKE) -f /home/tomvanetten/Documents/Workspace/evaq_beam_mesh/temp_sensor_CC26X2R1_LAUNCHXL_tirtos_ccs/src/makefile.libs clean

