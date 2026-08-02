ifeq ($(strip $(PVSNESLIB_HOME)),)
$(error "Please create an environment variable PVSNESLIB_HOME by following this guide: https://github.com/alekmaul/pvsneslib/wiki/Installation")
endif

# ROMNAME and variables are used in snes_rules file
export ROMNAME := ./out/hello_world
export ROMTITLE := LIBSNES HELLO WORLD

include ${PVSNESLIB_HOME}/devkitsnes/snes_rules

#---------------------------------------------------------------------------------
# Force the build to go through a wrapper. Instead of running 'make' directly, we define a custom rule
.PHONY: bitmaps all cleanLogs

all: bitmaps buildWithSummary
buildActual: $(OBJS) $(ROMNAME).sfc
	
clean: cleanBuildRes cleanRom cleanGfx cleanLogs

bitmaps : assets/map_512_512.pic