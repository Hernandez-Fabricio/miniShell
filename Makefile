# ------ Configuración ------
CC      = gcc
CFLAGS  = -Wall -Wextra -g -I./include -MMD -MP

SRC_DIR = src
OBJ_DIR = obj
TARGET  = minishell.exe

# Busca todos los .c de src/ y arma la lista de .o equivalentes en obj/
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

# ------ Reglas ------
all: $(TARGET)

# Linkeo: junta todos los .o en el ejecutable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

# Compilación: cada src/x.c se convierte en obj/x.o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Crea obj/ si no existe (por ejemplo, después de clonar el repo)
$(OBJ_DIR):
	mkdir $(OBJ_DIR)

# Borra todo lo generado para compilar de cero
clean:
	del /Q $(OBJ_DIR)\*.o $(OBJ_DIR)\*.d $(TARGET) 2>NUL

# Recompila un .c si cambió alguno de los .h que incluye
-include $(OBJS:.o=.d)

.PHONY: all clean