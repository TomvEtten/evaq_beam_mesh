#
_XDCBUILDCOUNT = 
ifneq (,$(findstring path,$(_USEXDCENV_)))
override XDCPATH = /Applications/ti/simplelink_cc13x2_26x2_sdk_2_40_00_81/source;/Applications/ti/simplelink_cc13x2_26x2_sdk_2_40_00_81/kernel/tirtos/packages
override XDCROOT = /Applications/ti/xdctools_3_51_01_18_core
override XDCBUILDCFG = ./config.bld
endif
ifneq (,$(findstring args,$(_USEXDCENV_)))
override XDCARGS = 
override XDCTARGETS = 
endif
#
ifeq (0,1)
PKGPATH = /Applications/ti/simplelink_cc13x2_26x2_sdk_2_40_00_81/source;/Applications/ti/simplelink_cc13x2_26x2_sdk_2_40_00_81/kernel/tirtos/packages;/Applications/ti/xdctools_3_51_01_18_core/packages;..
HOSTOS = MacOS
endif
