
#include <algorithm>

#include "toolbarconfig.h"

ToolbarConfig::ToolbarConfig(int min, int max, int stepsize, int current) {
    this->min = min;
    this->max = max;
    this->stepsize = stepsize;
    this->t_current = this->w2t(current);
}

int ToolbarConfig::w2t(int world_value) {
    return int((std::min(std::max(world_value, min), max) - min) / stepsize);
}

int ToolbarConfig::t2w(int toolbar_value) {
    return toolbar_value * stepsize + min;
}

int ToolbarConfig::current() {
    return t2w(t_current);
}

int ToolbarConfig::tmax() {
    return w2t(max);
}

int ToolbarConfig::tmin() {
    return w2t(min);
}
