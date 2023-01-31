#include <gui/model.h>

#include <iostream>

#include "app.h"

int main() {
	App app;

	app.model1.scale = app.model1.scale * 5;
	app.model1.position = app.model1.position + crl::P3D::P3D(0, 10, 0);
	
	app.model2.scale = app.model2.scale * 5;
	app.model2.position = app.model2.position + crl::P3D::P3D(0, 10, 0);
	
	app.setCallbacks();
	app.run();

	return 0;
}
