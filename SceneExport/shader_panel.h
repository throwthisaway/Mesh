#ifndef _SHADERPANEL_H
#define _SHADERPANEL_H
#include <lwserver.h>
#include <vector>
#include <string>
int ShowShaderPanel(GlobalFunc * global, const char * title, std::vector<std::string>& shaders, int * selection);
#endif