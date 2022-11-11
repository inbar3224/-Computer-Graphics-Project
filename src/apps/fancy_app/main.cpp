#include <gui/model.h>

#include <iostream>

#include "app.h"

int main() {
    App app;
    app.setCallbacks();
    app.run();

    return 0;
}
