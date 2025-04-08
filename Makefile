CC = gcc
NAME = hello-gtk
OBJS_DIR := objects
SRC_DIR := src
SRCS = $(addprefix $(SRC_DIR)/, hello-world-gtk.c\
	   			hello-world-gtk-stripped-ui.c\
		)

OBJS = $(patsubst $(SRC_DIR)%.c, $(OBJS_DIR)%.o, $(SRCS))
# SRC subdirectories
SRC_SUBDIR := $(dir $(OBJS))
# Objects subdir
OBJS_SUBDIR := $(subst $(SRC_DIR), $(OBJS_DIR), $(SRC_SUBDIR))

$(NAME): $(OBJS)
	$(CC) `pkg-config --cflags gtk4` $(OBJS) -o $@ `pkg-config --libs gtk4`
	@echo -------------------------------
	@echo GTK4 App binary created
	@echo -------------------------------

$(OBJS_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) `pkg-config --cflags gtk4` -c $< -o $@

all: $(NAME)

clean:
	rm -rf $(OBJS_DIR)
	rm -f $(NAME)
