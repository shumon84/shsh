PROGRAM  = $(shell basename $(CURDIR))
SRC_EXT  = cpp
INC_EXT  = h
CC       = gcc
LDFLAGS  =
CFLAGS   = -Wall -O3
SRC_DIR  = src
OBJ_DIR  = obj
INC_DIR  = inc
TMP_DIR  = tmp
RM       = rm -rv

SRCS     = $(wildcard $(SRC_DIR)/*.$(SRC_EXT))
OBJS     = $(patsubst $(SRC_DIR)/%.$(SRC_EXT),$(OBJ_DIR)/%.o,$(SRCS))
INCS     = $(wildcard $(INC_DIR)/*.$(INC_EXT))

DPD_FILE = $(TMP_DIR)/$(PROGRAM).dpd
TAG_FILE = $(SRC_DIR)/TAGS

.PHONY: all
all: $(PROGRAM)

$(PROGRAM): depend tags $(OBJ_DIR) $(OBJS) 
	$(CC) -o $(PROGRAM) $(OBJS) $(CFLAGS) $(LDFLAGS)

$(OBJS): Makefile

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.$(SRC_EXT)
	$(CC) $(CFLAGS) $(addprefix -I,$(INC_DIR)) -c $< -o $@

.PHONY: clean
clean:
	$(RM) $(PROGRAM)
	$(RM) $(TMP_DIR)
	$(RM) $(OBJ_DIR)
	$(RM) $(TAG_FILE)

.PHONY: depend
depend: $(DPD_FILE)
$(DPD_FILE):
	mkdir -p $(dir $(DPD_FILE))
	$(CC) -MM $(SRCS) $(addprefix -I,$(INC_DIR)) | sed -e 's/^/$(OBJ_DIR)\//g' > $(DPD_FILE)

-include $(DPD_FILE)

.PHONY: tags
tags: $(TAG_FILE)
$(TAG_FILE): $(SRCS) $(INCS)
	mkdir -p $(dir $(TAG_FILE))
	etags -f $(TAG_FILE) $(SRCS) $(INCS)
