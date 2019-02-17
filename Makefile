# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/04/10 17:19:11 by agrumbac          #+#    #+#              #
#    Updated: 2019/02/14 16:40:54 by agrumbac         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

############################## BIN #############################################

NAME = woody_woodpacker

SRC = woody_woodpacker.c safe.c endian.c errors.c check_file.c \
	encrypt.s decrypt.s hello.s

CC = clang

AS = nasm

SRCDIR = srcs

OBJDIR = objs

OBJC = $(addprefix ${OBJDIR}/, $(SRC:.c=.o))
OBJ = $(OBJC:.s=.o)

DEP = $(addprefix ${OBJDIR}/, $(SRC:.c=.d))

CFLAGS = -Wall -Wextra -Werror -fsanitize=address,undefined -g -MMD

ASFLAGS = -f elf64 -g

LDFLAGS = -Iincludes/

############################## COLORS ##########################################

BY = $(shell tput bold; tput setaf 3)
BR = $(shell tput bold; tput setaf 1)
BG = $(shell tput bold; tput setaf 2)
BB = $(shell tput bold; tput setaf 4)
BM = $(shell tput bold; tput setaf 5)
BC = $(shell tput bold; tput setaf 6)
BW = $(shell tput bold; tput setaf 7)
Y = $(shell tput setaf 3)
R = $(shell tput setaf 1)
G = $(shell tput setaf 2)
B = $(shell tput setaf 4)
M = $(shell tput setaf 5)
C = $(shell tput setaf 6)
W = $(shell tput sgr0; tput bold; tput setaf 2)
WR = $(tput tput sgr0; tput blink; tput setaf 1)
WY = $(tput tput sgr0; tput blink; tput setaf 3)
X = $(tput tput sgr0)
UP = "\033[A"
CUT = "\033[K"

############################## RULES ###########################################

all: art ${NAME}

${NAME}: ${OBJ}
	@echo ${B}Compiling [${NAME}]...${X}
	@${CC} ${CFLAGS} ${LDFLAGS} ${LIB} -o $@ ${OBJ}
	@echo ${G}Success"   "[${NAME}]${X}

${OBJDIR}/%.o: ${SRCDIR}/%.s
	@echo ${Y}Compiling [$@]...${X}
	@/bin/mkdir -p ${OBJDIR}
	@${AS} ${ASFLAGS} -o $@ $<
	@printf ${UP}${CUT}

${OBJDIR}/%.o: ${SRCDIR}/%.c
	@echo ${Y}Compiling [$@]...${X}
	@/bin/mkdir -p ${OBJDIR}
	@${CC} ${CFLAGS} ${LDFLAGS} -c -o $@ $<
	@printf ${UP}${CUT}

############################## GENERAL #########################################

clean:
	@echo ${R}Cleaning"  "[objs]...${X}
	@/bin/rm -Rf ${OBJDIR}

fclean: clean
	@echo ${R}Cleaning"  "[${NAME}]...${X}
	@/bin/rm -f ${NAME}
	@/bin/rm -Rf ${NAME}.dSYM

test:
	@${CC} -g ${LDFLAGS} -fsanitize=address,undefined ${LIB} \
	-I. -o ${NAME} $(addprefix srcs/, ${SRC})

re: fclean all

############################## DECORATION ######################################

art:
	@echo ${BB}
	@echo "       '-------------------.. "
	@echo "         '\\_________         \`-.-----. "
	@echo "             '\\_______          \`\\    \`.                   "${BG}"  __   ____ "${BB}
	@echo "                  \\____           \`\\   :                    "${BG}"/_ | | .-.\\ "${BB}
	@echo "    '._____         (____           \`\\.\`.             .--._ "${BG}"  \\\\' / "${BB}
	@echo "     \\____  \"'..____.(_______          \`~-.________.-'  "${WR}"@"${X}${BB}". \\___"${BG}"\\.'"${BB}"___ "
	@echo "     )___     ___                                         /___________\\ "
	@echo "      )___..''   '--.                    _.----------.____\`----"${BG}")'("${BB}"---' "
	@echo "                   _:-'   ,          _.-'                "${BG}"     /( \\\\ "${BB}
	@echo "               .-~~ __..-'~~~~~~~~~~'                    "${BG}"   .'    '\\ "${BB}
	@echo "               \\ \\~~ \\ \\          "${BG}"             _     "${BG}"      / '    ) \\ "${BB}
	@echo "                \\ \\   \\ \\_.-'  "${BG}"_ _ _ ___ ___ _| |_ _ "${BG}"     |      )   ' "${BB}
	@echo "                 \\ \\_ _;-,,'  "${BG}"| | | | . | . | . | | |  "${BG}"   ( (      ) ) "${BB}
	@echo "                 ;-,,'        "${BG}"|_____|___|___|___|_  |    "${BG}"  \"--------\" "${BB}
	@echo "                            "${BG}"                    |___|"
	@echo ${X}

.PHONY: all clean fclean re art

-include ${DEP}
