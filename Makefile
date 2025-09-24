CC := gcc
CFLAGS := -Wall -Wextra -Werror -std=c11

# Исходники библиотеки
SRC := $(wildcard matrix_lib/*.c) s21_matrix.c s21_other.c
OBJ := $(SRC:.c=.o)

# Заголовки
HEADERS := s21_matrix.h matrix_lib/s21_other.h

all: s21_matrix.a

# Статическая библиотека
s21_matrix.a: $(OBJ)
	ar rcs $@ $^
	ranlib $@

# Компиляция объектов
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Очистка
clean:
	rm -f matrix_lib/*.o *.o *.a
