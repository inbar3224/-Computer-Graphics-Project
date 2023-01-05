#include <gui/fancy3dapp.h>
#include <iostream>

void crl::gui::Fancy3DApp::resizeWindow(int width, int height) {
	camera.aspectRatio = float(width) / height;

	if (!shadowMapFBO.Init(this->width, this->height)) {
		std::cout << "Shadow map initialization failed\n";
		exit(0);
	}

	return Application::resizeWindow(width, height);
}

bool crl::gui::Fancy3DApp::mouseMove(double xpos, double ypos) {
	camera.processMouseMove(mouseState, keyboardState);
	return true;
}

bool crl::gui::Fancy3DApp::scrollWheel(double xoffset, double yoffset) {
	camera.processMouseScroll(xoffset, yoffset);
	return true;
}

void crl::gui::Fancy3DApp::drawAuxiliaryInfo() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	drawFPS();

	drawConsole();

	drawImGui();

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void crl::gui::Fancy3DApp::drawImGui() {
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Once);
	ImGui::Begin("Main Menu");
	static bool showDemo = false;
	ImGui::Checkbox("Show ImGui Demo Window", &showDemo);
	if (showDemo) {
		ImGui::ShowDemoWindow();
	}
	ImGui::Checkbox("Draw Console", &showConsole);
	ImGui::Checkbox("Automanage Console", &automanageConsole);


	ImGui::Checkbox("Selfshadowing", &drawSelfShadows);
	ImGui::Checkbox("Silhouette Shading", &drawSilhouetteShading);
	ImGui::Combo("PCF mode", &PCF_mode, "Normal\0Variable grid\0Weighted grid\0Poisson\00Poisson Stratified\0Poisson Rotated\0Poisson PCSS\0\0");
	ImGui::SliderInt("Samples", &PCF_samples_num, 1, 64);
	ImGui::SliderFloat("Light Proj Scale", &light.s, 0.0f, 5.0f);
	ImGui::SliderFloat("Shadow bias", &shadowbias, 0.0f, 0.01f, "%.5f", ImGuiSliderFlags_Logarithmic);
	ImGui::SliderFloat("Shadow Spread", &shadow_spread, 0.0f, 0.01f, "%.5f");
	ImGui::SliderFloat("Light Size", &light_size, 0.0f, 1.0f, "%.5f");


	ImGui::InputScalarN("Light location", ImGuiDataType_Float, &light.pos, 3);

	static glm::vec3 lightDir = toGLM(light.pos);
	ImGui::gizmo3D("##Dir1", lightDir);
	light.pos = toV3D(lightDir);
	ImGui::Image((void*)shadowMapFBO.shadowMap, ImVec2(500, 500));

	ImGui::End();

	ImGuiIO& io = ImGui::GetIO();
	ImGuizmo::BeginFrame();
	glm::mat4 cameraview = camera.getViewMatrix();
	ImGuizmo::ViewManipulate(glm::value_ptr(camera.viewMatrix), 10.0, ImVec2(io.DisplaySize.x - 128, 0), ImVec2(128, 128), 0x10101010);
	ImGui::ShowMetricsWindow();
}

void crl::gui::Fancy3DApp::draw() {
	glClearColor(clearColor[0], clearColor[1], clearColor[2], 1.00f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	prepareToDraw();

	shadowPass();
	renderPass();

	drawAuxiliaryInfo();
}

void crl::gui::Fancy3DApp::shadowPass() {
	shadowMapFBO.BindForWriting();
	glClear(GL_DEPTH_BUFFER_BIT);
	shadowMapRenderer.use();
	// set matrices for render_shadow.vert
	shadowMapRenderer.setMat4("projection", light.getOrthoProjectionMatrix());
	shadowMapRenderer.setMat4("view", light.getViewMatrix());
	glViewport(0, 0, shadowMapFBO.bufferWidth, shadowMapFBO.bufferHeight);

	drawShadowCastingObjects();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(0);
#ifdef RETINA_SCREEN
	// temporal one... need more investigation
	glViewport(0, 0, width * 2, height * 2);
#else
	glViewport(0, 0, width, height);
#endif
}

void crl::gui::Fancy3DApp::renderPass() {
	shadowMapFBO.BindForReading(GL_TEXTURE0, GL_TEXTURE1); // to do: organize texture units

	/* projection + view:
	 * basic_lighting.vert
	 * texture.vert
	 * render_shadow.vert */

	 /* camPos + lightPos + lightColor: compute_shading.frag */
	 // white light is from ShadowCastingLight light - inheriting from light
	auto shader_setup = [&](Shader& shader) {
		shader.use();
		shader.setMat4("projection", camera.getProjectionMatrix());
		shader.setMat4("view", camera.getViewMatrix());
		shader.setVec3("camPos", camera.position());
		shader.setVec3("lightPos", light.position());
		shader.setVec3("lightColor", light.color());
	};

	// set up shaders
	shader_setup(shadowShader);
	// light projection + light view: basic_lighting.vert
	shadowShader.setMat4("lightProjection", light.getOrthoProjectionMatrix());
	shadowShader.setMat4("lightView", light.getViewMatrix());
	/* compute_shadow_factor.frag:
	 * shadow map
	 * shadow depth
	 * bias
	 * pcf mode
	 * pcf samples num
	 * shadow spread
	 * light size */
	shadowShader.setInt("shadowMap", 0); // shadow map is set to GL_TEXTURE0
	shadowShader.setInt("shadowDepth", 1); // shadow depth is set to GL_TEXTURE1
	shadowShader.setFloat("bias", shadowbias);
	shadowShader.setInt("PCF_mode", PCF_mode);
	if (PCF_mode == 1) {// PCF_mode = grid
		shadowShader.setInt("PCF_samples_num", sqrt(PCF_samples_num));
	}
	else {
		shadowShader.setInt("PCF_samples_num", PCF_samples_num);
	}
	shadowShader.setFloat("shadow_spread", shadow_spread);
	shadowShader.setFloat("light_size", light_size);

	std::stringstream location;
	int colorLoc[8], positionLoc[8], strengthLoc[8];

	// set silhouette shader
	shader_setup(silhouetteShader);
	//for (int i = 0; i < 8; i++) {
		silhouetteShader.setVec3("lights[0].position", light.position());
		silhouetteShader.setVec3("lights[0].color", light.color());
		silhouetteShader.setFloat("lights[0].strength", 4.0f);
		silhouetteShader.setVec3("lights[1].position", light.position());
		silhouetteShader.setVec3("lights[1].color", light.color());
		silhouetteShader.setFloat("lights[1].strength", 4.0f);
		silhouetteShader.setVec3("lights[2].position", light.position());
		silhouetteShader.setVec3("lights[2].color", light.color());
		silhouetteShader.setFloat("lights[2].strength", 4.0f);
		silhouetteShader.setVec3("lights[3].position", light.position());
		silhouetteShader.setVec3("lights[3].color", light.color());
		silhouetteShader.setFloat("lights[3].strength", 4.0f);
		silhouetteShader.setVec3("lights[4].position", light.position());
		silhouetteShader.setVec3("lights[4].color", light.color());
		silhouetteShader.setFloat("lights[4].strength", 4.0f);
		silhouetteShader.setVec3("lights[5].position", light.position());
		silhouetteShader.setVec3("lights[5].color", light.color());
		silhouetteShader.setFloat("lights[5].strength", 4.0f);
		silhouetteShader.setVec3("lights[6].position", light.position());
		silhouetteShader.setVec3("lights[6].color", light.color());
		silhouetteShader.setFloat("lights[6].strength", 4.0f);
		silhouetteShader.setVec3("lights[7].position", light.position());
		silhouetteShader.setVec3("lights[7].color", light.color());
		silhouetteShader.setFloat("lights[7].strength", 4.0f);
	//}	

	shader_setup(basicShader);
	// better lighting approximation here so that regions of the model do
	// not remain forever shaded dark...
	basicShader.setVec3("lightPos", camera.position());

	drawObjectsWithShadows();
	drawObjectsWithoutShadows();
}
