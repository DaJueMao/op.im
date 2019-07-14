
#include "log.h"

std::string level[] = {
    "[normal]",
    "[notice]",
    "[warning]",
    "[fatal]",
};

void print(std::string message, int size) {
    if (size >= 0 && size < 4) {
        printf("%s %s\n", message.c_str(), level[size].c_str());
    }
}
