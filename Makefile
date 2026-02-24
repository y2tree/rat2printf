# ==============================================================================
# ft_printf Tester – Makefile
# ==============================================================================
#
# 1. Set FT_PRINTF_PATH to your ft_printf project directory.
#    That directory must contain:
#      - libftprintf.a   (compiled library)
#      - ft_printf.h     (header with the prototype)
#
# 2. If your library has a different name, change FT_PRINTF_LIB.
#
# 3. Run:
#      make        – compile the tester
#      make run    – compile and run immediately
#
# ==============================================================================

FT_PRINTF_PATH  := ../ft_printf
FT_PRINTF_LIB   := libftprintf.a

# ------------------------------------------------------------------------------
NAME    := test_ft_printf
SRC     := test_ft_printf.c
CC      := cc
CFLAGS  := -Wall -Wextra -I$(FT_PRINTF_PATH)
LIB     := $(FT_PRINTF_PATH)/$(FT_PRINTF_LIB)

.PHONY: all run clean re

all: $(NAME)

# Build ft_printf library if it is not present, then link the tester.
$(NAME): $(LIB) $(SRC)
	$(CC) $(CFLAGS) -o $(NAME) $(SRC) -L$(FT_PRINTF_PATH) -lftprintf
	@echo ">>> $(NAME) compiled."

$(LIB):
	@echo ">>> Building ft_printf library in $(FT_PRINTF_PATH) ..."
	$(MAKE) -C $(FT_PRINTF_PATH)

run: $(NAME)
	@echo ""
	@./$(NAME)

clean:
	rm -f $(NAME)
	@echo ">>> Cleaned."

re: clean all
