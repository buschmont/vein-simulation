gcc main.c lib/llist.c -o main $(pkg-config --cflags --libs raylib) -lm && ./main
