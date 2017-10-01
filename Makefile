CC := avr-gcc
DF := -Isrc
CF := -DF_CPU=16000000UL -mmcu=atmega328p -Wall -fmax-errors=3 -Os -flto -Isrc
LF := -mmcu=atmega328p -flto
AVRDUDE_FLAGS := -F -V -c arduino -p ATMEGA328P -P /dev/ttyACM0 -b 115200

SRCS := $(shell find src -type f -name '*.c')
OBJS := $(patsubst src/%.c,.build/%.o,$(SRCS))
DEPS := $(OBJS:.o=.d)

GREP_MAIN := grep -rl '^ *\(int\|void\) \+main *(' --include='*.c' src
HEX := $(patsubst src/%.c,%.hex,$(shell $(GREP_MAIN)))
ELF := $(patsubst %.hex,.build/%,$(HEX))

NODEPS := clean read
.PHONY: write clean read asm

all: $(HEX)

# Don't create dependencies when we're cleaning, for instance
ifeq (0, $(words $(findstring $(MAKECMDGOALS), $(NODEPS))))
-include $(DEPS)
# Error if multiple or no definitions of `main()`
ifneq (1, $(words $(HEX)))
$(error "main() defined in: $(HEX)")
endif
endif

.SECONDEXPANSION:

$(DEPS): .build/%.d: src/%.c | .build/$$(dir %)
	$(CC) $(DF) -MM -MT '$(@:.d=.o)' $< -MF $@

.build/%.o:
	$(CC) $(CF) $(C_$*) -c $(filter %.c,$^) -o $@

$(ELF): $(OBJS)
	$(CC) $(LF) $^ -o $@ $(L_$*)

$(HEX): $(ELF)
	avr-objcopy -O ihex -R .eeprom $< $@

write: $(HEX)
	avrdude $(AVRDUDE_FLAGS) -U flash:w:$<

read:
	avrdude $(AVRDUDE_FLAGS) -U flash:r:$(shell date +%s).hex:i

.build/%/:
	mkdir -p $@

clean:
	@rm -rfv .build $(HEX)

asm: $(ELF)
	avr-objdump -d $< > $(patsubst .build/%,%.s,$<)

