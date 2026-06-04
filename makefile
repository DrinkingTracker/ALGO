CXX      = g++
CXXFLAGS = -std=c++17 -Wall -I.

TARGET   = simulasi

SRCS     = main.cpp \
           program/utils.cpp \
           program/bfs.cpp \
           program/sorting.cpp \
           program/defect.cpp \
           program/report.cpp \
           program/wafer.cpp

ifeq ($(OS), Windows_NT)
    TARGET  := simulasi.exe
    RM       = del /Q
else
    RM       = rm -f
endif

# ============================================================
all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)
	@echo ""
	@echo ">>> Build berhasil! Jalankan dengan: ./$(TARGET)"

clean:
	$(RM) $(TARGET) report.txt

run: $(TARGET)
	./$(TARGET)
# ============================================================

.PHONY: all clean run