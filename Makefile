CPP  = arm-linux-g++
CC   = arm-linux-gcc
STRIP= arm-linux-strip
OBJ  = Core/bios.o Core/biosHLE.o Core/dma.o Core/flash.o Core/gfx.o Core/gfx_scanline_colour.o Core/gfx_scanline_mono.o Core/interrupt.o Core/mem.o Core/neopop.o Core/rom.o Core/sound.o Core/state.o Core/Z80_interface.o Core/TLCS-900h/TLCS900h_disassemble.o Core/TLCS-900h/TLCS900h_disassemble_dst.o Core/TLCS-900h/TLCS900h_disassemble_extra.o Core/TLCS-900h/TLCS900h_disassemble_reg.o Core/TLCS-900h/TLCS900h_disassemble_src.o Core/TLCS-900h/TLCS900h_interpret.o Core/TLCS-900h/TLCS900h_interpret_dst.o Core/TLCS-900h/TLCS900h_interpret_reg.o Core/TLCS-900h/TLCS900h_interpret_single.o Core/TLCS-900h/TLCS900h_interpret_src.o Core/TLCS-900h/TLCS900h_registers.o Core/z80/dasm.o Core/z80/Z80.o System_E680/system_sound.o System_E680/system_graphics.o System_E680/system_input.o System_E680/system_io.o System_E680/system_main.o System_E680/system_rom.o System_E680/zlib/adler32.o System_E680/zlib/crc32.o System_E680/zlib/infblock.o System_E680/zlib/infcodes.o System_E680/zlib/inffast.o System_E680/zlib/inflate.o System_E680/zlib/inftrees.o System_E680/zlib/infutil.o System_E680/zlib/unzip.o System_E680/zlib/zutil.o System_E680/qmainwnd.o System_E680/qmainapp.o System_E680/qemuthread.o System_E680/moc_qmainwnd.o
LIBS = -L$(QTDIR)/lib -lqte-mt -lezxjpeg-xscale-r
INCS = -ICore -ICore/TLCS-900h -ICore/z80
BIN  = neopop
CFLAGS = -O9 $(INCS) -D__cdecl= -mcpu=xscale
CPPFLAGS = $(CFLAGS) -I$(QTDIR)/include -DQWS -DQT_THREAD_SUPPORT -fno-rtti
RM = rm -f

.PHONY: all all-before all-after clean clean-custom

all: all-before $(BIN) all-after


clean: clean-custom
	${RM} $(OBJ) $(BIN) System_E680/moc_*

$(BIN): $(OBJ)
	$(CPP) $(OBJ) -o $(BIN) $(LIBS)
	$(STRIP) $(BIN)
	
System_E680/qmainwnd.o: System_E680/qmainwnd.cpp
	$(CPP) -c System_E680/qmainwnd.cpp -o System_E680/qmainwnd.o $(CPPFLAGS)
	
System_E680/moc_qmainwnd.o: System_E680/moc_qmainwnd.cpp
	$(CPP) -c System_E680/moc_qmainwnd.cpp -o System_E680/moc_qmainwnd.o $(CPPFLAGS)
	
System_E680/moc_qmainwnd.cpp: System_E680/qmainwnd.h
	$(QTDIR)/bin/moc System_E680/qmainwnd.h -o System_E680/moc_qmainwnd.cpp

System_E680/qmainapp.o: System_E680/qmainapp.cpp
	$(CPP) -c System_E680/qmainapp.cpp -o System_E680/qmainapp.o $(CPPFLAGS)

System_E680/qemuthread.o: System_E680/qemuthread.cpp
	$(CPP) -c System_E680/qemuthread.cpp -o System_E680/qemuthread.o $(CPPFLAGS)

Core/bios.o: Core/bios.c 
	$(CC) -c Core/bios.c -o Core/bios.o $(CFLAGS)

Core/biosHLE.o: Core/biosHLE.c 
	$(CC) -c Core/biosHLE.c -o Core/biosHLE.o $(CFLAGS)

Core/dma.o: Core/dma.c 
	$(CC) -c Core/dma.c -o Core/dma.o $(CFLAGS)

Core/flash.o: Core/flash.c Core/neopop.h Core/flash.h Core/mem.h
	$(CC) -c Core/flash.c -o Core/flash.o $(CFLAGS)

Core/gfx.o: Core/gfx.c Core/neopop.h Core/mem.h Core/gfx.h
	$(CC) -c Core/gfx.c -o Core/gfx.o $(CFLAGS)

Core/gfx_scanline_colour.o: Core/gfx_scanline_colour.c Core/neopop.h   Core/mem.h Core/gfx.h
	$(CC) -c Core/gfx_scanline_colour.c -o Core/gfx_scanline_colour.o $(CFLAGS)

Core/gfx_scanline_mono.o: Core/gfx_scanline_mono.c Core/neopop.h Core/mem.h   Core/gfx.h
	$(CC) -c Core/gfx_scanline_mono.c -o Core/gfx_scanline_mono.o $(CFLAGS)

Core/interrupt.o: Core/interrupt.c 
	$(CC) -c Core/interrupt.c -o Core/interrupt.o $(CFLAGS)

Core/mem.o: Core/mem.c 
	$(CC) -c Core/mem.c -o Core/mem.o $(CFLAGS)

Core/neopop.o: Core/neopop.c 
	$(CC) -c Core/neopop.c -o Core/neopop.o $(CFLAGS)

Core/rom.o: Core/rom.c Core/neopop.h Core/flash.h Core/interrupt.h
	$(CC) -c Core/rom.c -o Core/rom.o $(CFLAGS)

Core/sound.o: Core/sound.c Core/neopop.h Core/mem.h Core/sound.h
	$(CC) -c Core/sound.c -o Core/sound.o $(CFLAGS)

Core/state.o: Core/state.c 
	$(CC) -c Core/state.c -o Core/state.o $(CFLAGS)

Core/Z80_interface.o: Core/Z80_interface.c 
	$(CC) -c Core/Z80_interface.c -o Core/Z80_interface.o $(CFLAGS)

Core/TLCS-900h/TLCS900h_disassemble.o: Core/TLCS-900h/TLCS900h_disassemble.c 
	$(CC) -c Core/TLCS-900h/TLCS900h_disassemble.c -o Core/TLCS-900h/TLCS900h_disassemble.o $(CFLAGS)

Core/TLCS-900h/TLCS900h_disassemble_dst.o: Core/TLCS-900h/TLCS900h_disassemble_dst.c 
	$(CC) -c Core/TLCS-900h/TLCS900h_disassemble_dst.c -o Core/TLCS-900h/TLCS900h_disassemble_dst.o $(CFLAGS)

Core/TLCS-900h/TLCS900h_disassemble_extra.o: Core/TLCS-900h/TLCS900h_disassemble_extra.c
	$(CC) -c Core/TLCS-900h/TLCS900h_disassemble_extra.c -o Core/TLCS-900h/TLCS900h_disassemble_extra.o $(CFLAGS)

Core/TLCS-900h/TLCS900h_disassemble_reg.o: Core/TLCS-900h/TLCS900h_disassemble_reg.c 
	$(CC) -c Core/TLCS-900h/TLCS900h_disassemble_reg.c -o Core/TLCS-900h/TLCS900h_disassemble_reg.o $(CFLAGS)

Core/TLCS-900h/TLCS900h_disassemble_src.o: Core/TLCS-900h/TLCS900h_disassemble_src.c 
	$(CC) -c Core/TLCS-900h/TLCS900h_disassemble_src.c -o Core/TLCS-900h/TLCS900h_disassemble_src.o $(CFLAGS)

Core/TLCS-900h/TLCS900h_interpret.o: Core/TLCS-900h/TLCS900h_interpret.c 
	$(CC) -c Core/TLCS-900h/TLCS900h_interpret.c -o Core/TLCS-900h/TLCS900h_interpret.o $(CFLAGS)

Core/TLCS-900h/TLCS900h_interpret_dst.o: Core/TLCS-900h/TLCS900h_interpret_dst.c 
	$(CC) -c Core/TLCS-900h/TLCS900h_interpret_dst.c -o Core/TLCS-900h/TLCS900h_interpret_dst.o $(CFLAGS)

Core/TLCS-900h/TLCS900h_interpret_reg.o: Core/TLCS-900h/TLCS900h_interpret_reg.c 
	$(CC) -c Core/TLCS-900h/TLCS900h_interpret_reg.c -o Core/TLCS-900h/TLCS900h_interpret_reg.o $(CFLAGS)

Core/TLCS-900h/TLCS900h_interpret_single.o: Core/TLCS-900h/TLCS900h_interpret_single.c 
	$(CC) -c Core/TLCS-900h/TLCS900h_interpret_single.c -o Core/TLCS-900h/TLCS900h_interpret_single.o $(CFLAGS)

Core/TLCS-900h/TLCS900h_interpret_src.o: Core/TLCS-900h/TLCS900h_interpret_src.c 
	$(CC) -c Core/TLCS-900h/TLCS900h_interpret_src.c -o Core/TLCS-900h/TLCS900h_interpret_src.o $(CFLAGS)

Core/TLCS-900h/TLCS900h_registers.o: Core/TLCS-900h/TLCS900h_registers.c 
	$(CC) -c Core/TLCS-900h/TLCS900h_registers.c -o Core/TLCS-900h/TLCS900h_registers.o $(CFLAGS)

Core/z80/dasm.o: Core/z80/dasm.c 
	$(CC) -c Core/z80/dasm.c -o Core/z80/dasm.o $(CFLAGS)

Core/z80/Z80.o: Core/z80/Z80.c Core/z80/Z80.h Core/z80/Tables.h Core/z80/CodesCB.h   Core/z80/CodesXCB.h Core/z80/CodesED.h Core/z80/CodesXX.h   Core/z80/Codes.h
	$(CC) -c Core/z80/Z80.c -o Core/z80/Z80.o $(CFLAGS)

System_E680/system_sound.o: System_E680/system_sound.c 
	$(CC) -c System_E680/system_sound.c -o System_E680/system_sound.o $(CFLAGS)

System_E680/system_graphics.o: System_E680/system_graphics.c 
	$(CC) -c System_E680/system_graphics.c -o System_E680/system_graphics.o $(CFLAGS)

System_E680/system_input.o: System_E680/system_input.c 
	$(CC) -c System_E680/system_input.c -o System_E680/system_input.o $(CFLAGS)

System_E680/system_io.o: System_E680/system_io.c 
	$(CC) -c System_E680/system_io.c -o System_E680/system_io.o $(CFLAGS)

System_E680/system_main.o: System_E680/system_main.c
	$(CC) -c System_E680/system_main.c -o System_E680/system_main.o $(CFLAGS)

System_E680/system_rom.o: System_E680/system_rom.c 
	$(CC) -c System_E680/system_rom.c -o System_E680/system_rom.o $(CFLAGS)

System_E680/zlib/adler32.o: System_E680/zlib/adler32.c System_E680/zlib/zlib.h   System_E680/zlib/zconf.h
	$(CC) -c System_E680/zlib/adler32.c -o System_E680/zlib/adler32.o $(CFLAGS)

System_E680/zlib/crc32.o: System_E680/zlib/crc32.c System_E680/zlib/zlib.h   System_E680/zlib/zconf.h
	$(CC) -c System_E680/zlib/crc32.c -o System_E680/zlib/crc32.o $(CFLAGS)

System_E680/zlib/infblock.o: System_E680/zlib/infblock.c System_E680/zlib/zutil.h   System_E680/zlib/zlib.h System_E680/zlib/zconf.h   System_E680/zlib/infblock.h System_E680/zlib/inftrees.h   System_E680/zlib/infcodes.h System_E680/zlib/infutil.h
	$(CC) -c System_E680/zlib/infblock.c -o System_E680/zlib/infblock.o $(CFLAGS)

System_E680/zlib/infcodes.o: System_E680/zlib/infcodes.c System_E680/zlib/zutil.h   System_E680/zlib/zlib.h System_E680/zlib/zconf.h   System_E680/zlib/inftrees.h System_E680/zlib/infblock.h   System_E680/zlib/infcodes.h System_E680/zlib/infutil.h   System_E680/zlib/inffast.h
	$(CC) -c System_E680/zlib/infcodes.c -o System_E680/zlib/infcodes.o $(CFLAGS)

System_E680/zlib/inffast.o: System_E680/zlib/inffast.c System_E680/zlib/zutil.h   System_E680/zlib/zlib.h System_E680/zlib/zconf.h   System_E680/zlib/inftrees.h System_E680/zlib/infblock.h   System_E680/zlib/infcodes.h System_E680/zlib/infutil.h   System_E680/zlib/inffast.h
	$(CC) -c System_E680/zlib/inffast.c -o System_E680/zlib/inffast.o $(CFLAGS)

System_E680/zlib/inflate.o: System_E680/zlib/inflate.c System_E680/zlib/zutil.h   System_E680/zlib/zlib.h System_E680/zlib/zconf.h   System_E680/zlib/infblock.h
	$(CC) -c System_E680/zlib/inflate.c -o System_E680/zlib/inflate.o $(CFLAGS)

System_E680/zlib/inftrees.o: System_E680/zlib/inftrees.c System_E680/zlib/zutil.h   System_E680/zlib/zlib.h System_E680/zlib/zconf.h   System_E680/zlib/inftrees.h System_E680/zlib/inffixed.h
	$(CC) -c System_E680/zlib/inftrees.c -o System_E680/zlib/inftrees.o $(CFLAGS)

System_E680/zlib/infutil.o: System_E680/zlib/infutil.c System_E680/zlib/zutil.h   System_E680/zlib/zlib.h System_E680/zlib/zconf.h   System_E680/zlib/infblock.h System_E680/zlib/inftrees.h   System_E680/zlib/infcodes.h System_E680/zlib/infutil.h
	$(CC) -c System_E680/zlib/infutil.c -o System_E680/zlib/infutil.o $(CFLAGS)

System_E680/zlib/unzip.o: System_E680/zlib/unzip.c System_E680/zlib/zlib.h   System_E680/zlib/zconf.h System_E680/zlib/unzip.h
	$(CC) -c System_E680/zlib/unzip.c -o System_E680/zlib/unzip.o $(CFLAGS)

System_E680/zlib/zutil.o: System_E680/zlib/zutil.c System_E680/zlib/zutil.h   System_E680/zlib/zlib.h System_E680/zlib/zconf.h
	$(CC) -c System_E680/zlib/zutil.c -o System_E680/zlib/zutil.o $(CFLAGS)
