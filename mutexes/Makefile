# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -pthread

# Target executables
TARGET_MUTEX = mutex_test_program
TARGET_RWLOCK = rwlock_test_program

# Source files
SRCS_MUTEX = mutex_test.c mutex.c
SRCS_RWLOCK = rwlock_test.c rwlock.c

# Object files
OBJS_MUTEX = $(SRCS_MUTEX:.c=.o)
OBJS_RWLOCK = $(SRCS_RWLOCK:.c=.o)

# Default rule: Build the target executables
all: $(TARGET_MUTEX) $(TARGET_RWLOCK)

# Rule for mutex test program
$(TARGET_MUTEX): $(OBJS_MUTEX)
	$(CC) $(CFLAGS) -o $@ $(OBJS_MUTEX)

# Rule for rwlock test program
$(TARGET_RWLOCK): $(OBJS_RWLOCK)
	$(CC) $(CFLAGS) -o $@ $(OBJS_RWLOCK)

# Compile .c files into .o files for mutex
%.o: %.c mutex.h
	$(CC) $(CFLAGS) -c $< -o $@

# Compile .c files into .o files for rwlock
%.o: %.c rwlock.h
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up generated files
clean:
	rm -f $(OBJS_MUTEX) $(OBJS_RWLOCK) $(TARGET_MUTEX) $(TARGET_RWLOCK)

# Run the mutex test program
run_mutex: $(TARGET_MUTEX)
	./$(TARGET_MUTEX)

# Run the rwlock test program
run_rwlock: $(TARGET_RWLOCK)
	./$(TARGET_RWLOCK)
