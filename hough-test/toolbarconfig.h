
#ifndef TOOLBARCONFIG_H
#define TOOLBARCONFIG_H

class ToolbarConfig {
    public:
    ToolbarConfig(int min, int max, int stepsize, int current);

    int w2t(int world_value);
    int t2w(int toolbar_value);

    int current();
    int tmax();
    int tmin();

    int min;
    int max;
    int stepsize;

    int t_current;
};

#endif
