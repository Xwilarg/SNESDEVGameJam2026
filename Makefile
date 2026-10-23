ifeq ($(strip $(PVSNESLIB_HOME)),)
$(error "Please create an environment variable PVSNESLIB_HOME by following this guide: https://github.com/alekmaul/pvsneslib/wiki/Installation")
endif

AUDIOFILES := assets/bgm.it
export SOUNDBANK := assets/soundbank

# ROMNAME and variables are used in snes_rules file
export ROMNAME := ./out/uncivilized
export ROMTITLE := Uncivilized

include ${PVSNESLIB_HOME}/devkitsnes/snes_rules

#---------------------------------------------------------------------------------
# Force the build to go through a wrapper. Instead of running 'make' directly, we define a custom rule
.PHONY: bitmaps all cleanLogs re

all: musics bitmaps buildWithSummary
buildActual: $(OBJS) $(ROMNAME).sfc
re: clean all

clean: cleanBuildRes cleanRom cleanGfx cleanLogs

assets/pvsl_font.pic: assets/pvsl_font.bmp
	$(GFXCONV) -s 8 -o 2 -u 16 -e 1 -p -t bmp -m -R -i $<

bitmaps : assets/map_512_512.pic assets/pvsl_font.pic

SMCONVFLAGS	:= -s -o $(SOUNDBANK) -V -b 5
musics : $(SOUNDBANK).obj