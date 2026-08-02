.include "hdr.asm"

.section ".rodata1" superfree

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

.ends
