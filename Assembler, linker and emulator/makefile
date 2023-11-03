assembl = src/backpatch.cpp\
	src/formatedLine.cpp\
	src/formator.cpp\
	src/relaTable.cpp\
	src/section.cpp\
	src/symbol.cpp\
	src/symTab.cpp\
	src/assembler.cpp\
	src/assembler_main.cpp

link = src/relaTable.cpp\
	src/section.cpp\
	src/symbol.cpp\
	src/symTab.cpp\
	src/linker.cpp\
	src/linker_main.cpp

emul = src/emulator.cpp\
	src/emulator_main.cpp

all: assembler linker emulator

assembler: ${assembl}
	g++ -g -o ${@} ${^}

linker: ${link}
	g++ -g -o ${@} ${^}

emulator: ${emul}
	g++ -g -o ${@} ${^}

clean:
	rm -f assembler
	rm -f linker
	rm -f emulator
