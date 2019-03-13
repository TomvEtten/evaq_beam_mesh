#
_XDCBUILDCOUNT = 
ifneq (,$(findstring path,$(_USEXDCENV_)))
override XDCPATH = /home/tomvanetten/Programs/ti/ti/simplelink_cc13x2_26x2_sdk_2_40_00_81/source;/home/tomvanetten/Programs/ti/ti/simplelink_cc13x2_26x2_sdk_2_40_00_81/kernel/tirtos/packages
override XDCROOT = /home/tomvanetten/Programs/ti/xdctools_3_51_01_18_core
override XDCBUILDCFG = ./config.bld
endif
ifneq (,$(findstring args,$(_USEXDCENV_)))
override XDCARGS = 
override XDCTARGETS = 
endif
#
ifeq (0,1)
PKGPATH = /home/tomvanetten/Programs/ti/ti/simplelink_cc13x2_26x2_sdk_2_40_00_81/source;/home/tomvanetten/Programs/ti/ti/simplelink_cc13x2_26x2_sdk_2_40_00_81/kernel/tirtos/packages;/home/tomvanetten/Programs/ti/xdctools_3_51_01_18_core/packages;..
HOSTOS = Linux
endif
