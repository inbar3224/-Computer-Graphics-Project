#include <gui/model.h>

#include <iostream>

#include "app.h"

int main() {
	App app;
	app.model.scale = app.model.scale * 5;
	app.model.position = app.model.position + crl::P3D::P3D(0, 10, 0);
	app.setCallbacks();
	app.run();

	return 0;
}
