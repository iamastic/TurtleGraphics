CC= gcc
COMMON= -Wall -Wextra -Wfloat-equal -pedantic -std=c90 -Werror
SDL = `sdl2-config --cflags` -O2 -Wall -Wextra -pedantic -Wfloat-equal -std=c90
DEBUG= -g3
SANITIZE= $(COMMON) -fsanitize=undefined -fsanitize=address $(DEBUG)
VALGRIND= $(COMMON) $(DEBUG)
PRODUCTION= $(COMMON) -O3
LDLIBS = -lm
LDSDL = `sdl2-config --libs` $(LDLIBS)

#          -- Instructions --
#Type: 'make parse' for Production Output.
#       Then type 'make runParse'.

#Type: 'make parse_s' for Output with Sanitize flags.
#       Then type, make runParse_s.

#Type: 'make parse_v' for Output with Valgrind.
#       Then type, make runParse_v.

#Type: 'make interp' for Output via SDL.
#       Then type, make runInterp.

#Type: 'make extension' for Output via SDL.
#       Then type, make runExtension.

#Further instructions are available in testing.txt and
#extension.txt.

parse : Parsing/parsing.h Parsing/parsingSpecific.h Parsing/parsing.c General/general.h General/general.c
	$(CC) Parsing/parsing.c General/general.c -o Parsing/parsing $(PRODUCTION) $(LDLIBS)

parse_s : Parsing/parsing.h Parsing/parsingSpecific.h Parsing/parsing.c General/general.h General/general.c
	$(CC) Parsing/parsing.c General/general.c -o Parsing/parsing_s $(SANITIZE) $(LDLIBS)

parse_v : Parsing/parsing.h Parsing/parsingSpecific.h Parsing/parsing.c General/general.h General/general.c
	$(CC) Parsing/parsing.c General/general.c -o Parsing/parsing_v $(VALGRIND) $(LDLIBS)

interp : Interpreter/interp.h Interpreter/interpSpecific.h Interpreter/interp.c Interpreter/Stack/stack.c General/general.c General/general.h Interpreter/Stack/stack.h Interpreter/Stack/stackSpecific.h
	$(CC) Interpreter/interp.c Interpreter/Stack/stack.c General/general.c -o Interpreter/interp $(SDL) $(LDSDL)

extension : Extension/extension.c Extension/extension.h Extension/extensionSpecific.h Interpreter/Stack/stack.c General/general.c General/general.h Interpreter/Stack/stack.h Interpreter/Stack/stackSpecific.h
	$(CC) Extension/extension.c Interpreter/Stack/stack.c General/general.c -o Extension/ext $(SDL) $(LDSDL)

clean:
	rm -f Parsing/parsing_s Parsing/parsing_v Parsing/parsing Interpreter/interp Extension/ext Extension/Turtle.txt

runParse: Parsing/parsing
	./Parsing/parsing Texts/I12.ttl

runInterp: Interpreter/interp
	./Interpreter/interp Texts/I12.ttl

runParse_v: Parsing/parsing_v
	valgrind ./Parsing/parsing_v Texts/I12.ttl

runParse_s: Parsing/parsing_s
	./Parsing/parsing_s Texts/I12.ttl

runExtension: Extension/ext
	./Extension/ext

#Testing parser
P1: Parsing/parsing_s
	./Parsing/parsing_s Texts/P1.ttl

P2: Parsing/parsing_s
	./Parsing/parsing_s Texts/P2.ttl

P3: Parsing/parsing_s
	./Parsing/parsing_s Texts/P3.ttl

P4: Parsing/parsing_s
	./Parsing/parsing_s Texts/P4.ttl

P5: Parsing/parsing_s
	./Parsing/parsing_s Texts/P5.ttl

P6: Parsing/parsing_s
	./Parsing/parsing_s Texts/P6.ttl

P7: Parsing/parsing_s
	./Parsing/parsing_s Texts/P7.ttl

P8: Parsing/parsing_s
	./Parsing/parsing_s Texts/P8.ttl

P9: Parsing/parsing_s
	./Parsing/parsing_s Texts/P9.ttl

P10: Parsing/parsing_s
	./Parsing/parsing_s Texts/P10.ttl

P11: Parsing/parsing_s
	./Parsing/parsing_s Texts/P11.ttl

P12: Parsing/parsing_s
	./Parsing/parsing_s Texts/P12.ttl

P13: Parsing/parsing_s
	./Parsing/parsing_s Texts/P13.ttl

P14: Parsing/parsing_s
	./Parsing/parsing_s Texts/P14.ttl

P15: Parsing/parsing_s
	./Parsing/parsing_s Texts/P15.ttl

P16: Parsing/parsing_s
	./Parsing/parsing_s Texts/P16.ttl

P17: Parsing/parsing_s
	./Parsing/parsing_s Texts/P17.ttl

P18: Parsing/parsing_s
	./Parsing/parsing_s Texts/P18.ttl

P19: Parsing/parsing_s
	./Parsing/parsing_s Texts/P19.ttl

P20: Parsing/parsing_s
	./Parsing/parsing_s Texts/P20.ttl

P21: Parsing/parsing_s
	./Parsing/parsing_s Texts/P21.ttl

P22: Parsing/parsing_s
	./Parsing/parsing_s Texts/P22.ttl

P23: Parsing/parsing_s
	./Parsing/parsing_s Texts/P23.ttl

P24: Parsing/parsing_s
	./Parsing/parsing_s Texts/P24.ttl

P25: Parsing/parsing_s
	./Parsing/parsing_s Texts/P25.ttl

P26: Parsing/parsing_s
	./Parsing/parsing_s Texts/P26.ttl

P27: Parsing/parsing_s
	./Parsing/parsing_s Texts/P27.ttl

P28: Parsing/parsing_s
	./Parsing/parsing_s Texts/P28.ttl

#Testing Interpreter
I1: Interpreter/interp
	./Interpreter/interp Texts/I1.ttl

I2: Interpreter/interp
	./Interpreter/interp Texts/I2.ttl

I3: Interpreter/interp
	./Interpreter/interp Texts/I3.ttl

shapes: Interpreter/interp
	#square
	./Interpreter/interp Texts/I4.ttl
	#triangle
	./Interpreter/interp Texts/I5.ttl
  #pentagon
	./Interpreter/interp Texts/I6.ttl
  #hexagon
	./Interpreter/interp Texts/I7.ttl
  #octagon
	./Interpreter/interp Texts/I8.ttl
	#rhombus
	./Interpreter/interp Texts/I13.ttl
	#circle
	./Interpreter/interp Texts/I15.ttl

fancy: Interpreter/interp
	#spiral1
	./Interpreter/interp Texts/I9.ttl
	#spiral2
	./Interpreter/interp Texts/I10.ttl
	#insaneSpiral
	./Interpreter/interp Texts/I11.ttl
	#insaneSpiral2
	./Interpreter/interp Texts/I12.ttl
	#simpleStar
	./Interpreter/interp Texts/I14.ttl
	#spiralStar
	./Interpreter/interp Texts/I16.ttl
