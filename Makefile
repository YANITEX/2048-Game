# Définition des variables
CC = gcc
CFLAGS = -I src/include -I src/include/SDL2  # Chemin vers les fichiers d'en-tête SDL et ton fichier .h
LDFLAGS = -L src/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lm #-mwindows  # Chemin vers les bibliothèques SDL
SOURCES = $(wildcard code/*.c)
TARGET = src/bin/main.exe

# Règle par défaut pour compiler directement
all:
	$(CC) $(SOURCES) -o $(TARGET) $(CFLAGS) $(LDFLAGS)

# Règle pour exécuter l'exécutable
run:
	./$(TARGET)

# Nettoyage
clean:
	del /f /q "src\bin\main.exe"
# Phony pour ne pas confondre avec des fichiers
.PHONY: all run clean
