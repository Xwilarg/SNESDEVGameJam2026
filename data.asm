.include "hdr.asm"

.section ".rodata1" superfree

pvsl_font_til:
.incbin "assets/pvsl_font.pic"

patterns:
.incbin "assets/map_512_512.pic"
patterns_end:

.ends

.section ".rodata2" superfree
map:
.incbin "assets/map_512_512.map"
map_end:

palette:
.incbin "assets/map_512_512.pal"

pvsl_font_pal:
.incbin "assets/pvsl_font.pal"

.ends
