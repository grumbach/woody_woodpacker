# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/04/10 17:19:11 by agrumbac          #+#    #+#              #
#    Updated: 2019/05/10 08:35:30 by agrumbac         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

############################## BIN #############################################

NAME = woody_woodpacker

SRC = woody_woodpacker.c original_safe.c endian.c encrypt.s decrypt.s \
	elf64/elf64_payload.s \
	elf64/elf64_packer.c \
	elf64/elg64_iterators.c \
	elf64/elf64_identifier.c

CC = clang

AS = nasm

SRCDIR = srcs

OBJDIR = objs

OBJC = $(addprefix ${OBJDIR}/, $(SRC:.c=.o))
OBJ = $(OBJC:.s=.o)

DEP = $(addprefix ${OBJDIR}/, $(SRC:.c=.d))

CFLAGS = -Wall -Wextra -fsanitize=address,undefined -g -MMD

ASFLAGS = -f elf64 -g

LDFLAGS = -Iincludes/

############################## COLORS ##########################################

BY = "\033[33;1m"
BR = "\033[31;1m"
BG = "\033[32;1m"
BB = "\033[34;1m"
BM = "\033[35;1m"
BC = "\033[36;1m"
BW = "\033[37;1m"
Y = "\033[33m"
R = "\033[31m"
G = "\033[32m"
B = "\033[34m"
M = "\033[35m"
C = "\033[36m"
W = "\033[0m""\033[32;1m"
WR = "\033[0m""\033[31;5m"
WY = "\033[0m""\033[33;5m"
X = "\033[0m"
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
	@/bin/mkdir -p ${OBJDIR} ${OBJDIR}/elf64
	@${AS} ${ASFLAGS} -o $@ $<
	@printf ${UP}${CUT}

${OBJDIR}/%.o: ${SRCDIR}/%.c
	@echo ${Y}Compiling [$@]...${X}
	@/bin/mkdir -p ${OBJDIR} ${OBJDIR}/elf64
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
