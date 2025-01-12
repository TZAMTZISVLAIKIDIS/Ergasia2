# Ονομασία του εκτελέσιμου αρχείου
TARGET = eshop

# Ονόματα των πηγών
SRC = eshop.c

# Ονόματα των αντικειμένων
OBJ = eshop.o

# Ο διακομιστής της μεταγλώττισης
CC = gcc

# Επιλογές του compiler
CFLAGS = -Wall -std=c99

# Επιλογές του linker
LDFLAGS =

# Κανόνας για την κατασκευή του εκτελέσιμου αρχείου
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

# Κανόνας για την κατασκευή των αντικειμένων από τα πηγαία αρχεία
$(OBJ): eshop.c
	$(CC) $(CFLAGS) -c eshop.c

# Καθαρισμός των αντικειμένων και του εκτελέσιμου αρχείου
clean:
	rm -f $(OBJ) $(TARGET)

# Κανόνας για εκτέλεση του προγράμματος
run: $(TARGET)
	./$(TARGET)

