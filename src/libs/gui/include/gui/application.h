#pragma once

#include <glad/glad.h>

// do not put glfw before glad!
#include <GLFW/glfw3.h>
#include <examples/imgui_impl_glfw.h>
#include <examples/imgui_impl_opengl3.h>
#include <gui/camera.h>
#include <gui/inputstate.h>
#include <gui/shader.h>
#include <gui/shadow_casting_light.h>
#include <gui/shadow_map_fbo.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <utils/logger.h>
#include <utils/timer.h>

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#pragma warning(disable : 4244)

#if defined __APPLE__ && !defined RETINA_SCREEN
#define RETINA_SCREEN 1
#endif

namespace crl {
	namespace gui {
		inline float get_pixel_ratio();

		// Sets up a GLFW window, its callbacks and ImGui
		class Application {
		public:
			GLFWwindow* window;
			int width, height;
			float pixelRatio;

			float clearColor[3] = { 0.8f, 0.8f, 0.8f };

			MouseState mouseState;
			KeyboardState keyboardState;

			// FPS params - we will want to display the fps only every 0.3s, otherwise the numbers change way too quickly...
			float averageFPS = 0.0f;
			float tmpEntireLoopTimeRunningAverage = 0.0f;
			float tmpProcessTimeRunningAverage = 0.0f;
			float averagePercentTimeSpentProcessing = 0;
			int runningAverageStepCount = 0;

			Timer FPSDisplayTimer;
			Timer processTimer;
			Timer FPSTimer;

			bool limitFramerate = false;
			int targetFramerate = 60;

			bool automanageConsole = false;
			bool showConsole = false;
			int consoleHeight = 250;  // in pixels

			void init(const char* title, int width, int height, std::string iconPath = DATA_FOLDER "/crl_icon_grey.png");
		public:
			Application(const char* title, int width, int height, std::string iconPath = DATA_FOLDER "/crl_icon_grey.png");
			Application(const char* title, std::string iconPath = DATA_FOLDER "/crl_icon_grey.png");
			virtual ~Application();

			virtual void setCallbacks();
			virtual void run();
			virtual void process();
			virtual void draw();
			virtual void drawFPS();

			// override this method to customize placement strategy...
			virtual void drawConsole();
			virtual void resizeWindow(int width, int height);

			// return false if the message was not processed, true otherwise
			virtual bool keyPressed(int key, int mods) {
				return false;
			}

			virtual bool keyReleased(int key, int mods) {
				return false;
			}

			virtual bool mouseButtonPressed(int button, int mods) {
				return false;
			}

			virtual bool mouseButtonReleased(int button, int mods) {
				return false;
			}

			virtual bool mouseMove(double xpos, double ypos) {
				return false;
			}

			virtual bool scrollWheel(double xoffset, double yoffset) {
				return false;
			}

			virtual bool drop(int count, const char** filenames) {
				return false;
			}

			bool screenshot(const char* filename) const;

			bool screenIsRecording = false;
			int screenShotCounter = 0;
			char screenshotPath[100] = DATA_FOLDER "/out/screenshots";
		};

		inline bool ToggleButton(const char* str_id, bool* v) {
			bool clicked = false;
			ImVec2 p = ImGui::GetCursorScreenPos();
			ImDrawList* draw_list = ImGui::GetWindowDrawList();

			float height = ImGui::GetFrameHeight();
			float width = height * 1.55f;
			float radius = height * 0.50f;

			ImGui::InvisibleButton(str_id, ImVec2(width, height));
			if (ImGui::IsItemClicked()) {
				clicked = true;
				*v = !*v;
			}

			float t = *v ? 1.0f : 0.0f;

			ImGuiContext& g = *GImGui;
			float ANIM_SPEED = 0.08f;
			if (g.LastActiveId == g.CurrentWindow->GetID(str_id)) { // && g.LastActiveIdTimer < ANIM_SPEED)            
				float t_anim = ImSaturate(g.LastActiveIdTimer / ANIM_SPEED);
				t = *v ? (t_anim) : (1.0f - t_anim);
			}

			ImU32 col_bg;
			if (ImGui::IsItemHovered()) {
				col_bg = ImGui::GetColorU32(ImLerp(ImVec4(0.78f, 0.78f, 0.78f, 1.0f), ImVec4(0.64f, 0.83f, 0.34f, 1.0f), t));
			}
			else {
				col_bg = ImGui::GetColorU32(ImLerp(ImVec4(0.85f, 0.85f, 0.85f, 1.0f), ImVec4(0.56f, 0.83f, 0.26f, 1.0f), t));
			}

			draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + height), col_bg, height * 0.5f);
			draw_list->AddCircleFilled(ImVec2(p.x + radius + t * (width - radius * 2.0f), p.y + radius), radius - 1.5f, IM_COL32(255, 255, 255, 255));

			return clicked;
		}

		inline bool PlayPauseButton(const char* str_id, bool* v) {
			bool clicked = false;
			ImVec2 p = ImGui::GetCursorScreenPos();
			ImDrawList* draw_list = ImGui::GetWindowDrawList();

			float height = ImGui::GetFrameHeight();
			float width = height * 1.55f;
			float radius = height * 0.50f;

			ImGui::InvisibleButton(str_id, ImVec2(width, height));
			if (ImGui::IsItemClicked()) {
				clicked = true;
				*v = !*v;
			}

			ImU32 col_bg;

			if (*v) {
				if (ImGui::IsItemHovered()) {
					col_bg = ImGui::GetColorU32(ImVec4(0.54f, 0.73f, 0.3f, 1.0f));
				}
				else {
					col_bg = ImGui::GetColorU32(ImVec4(0.64f, 0.83f, 0.34f, 1.0f));
				}
			}
			else {
				if (ImGui::IsItemHovered()) {
					col_bg = ImGui::GetColorU32(ImVec4(0.75f, 0.75f, 0.75f, 1.0f));
				}
				else {
					col_bg = ImGui::GetColorU32(ImVec4(0.85f, 0.85f, 0.85f, 1.0f));
				}
			}

			draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + height), col_bg, height * 0.25f);

			if (!*v) {
				draw_list->AddTriangleFilled(
					ImVec2(p.x + width * 0.35, p.y + height * 0.2),
					ImVec2(p.x + width * 0.65, p.y + height * 0.5),
					ImVec2(p.x + width * 0.35, p.y + height * 0.8),
					ImGui::GetColorU32(ImVec4(0.0f, 0.0f, 0.0f, 1.0f)));
			}
			else {
				draw_list->AddRectFilled(
					ImVec2(p.x + width * 0.3, p.y + height * 0.2),
					ImVec2(p.x + width * 0.45, p.y + height * 0.8),
					ImGui::GetColorU32(ImVec4(0.0f, 0.0f, 0.0f, 1.0f)), 0);
				draw_list->AddRectFilled(
					ImVec2(p.x + width * 0.55, p.y + height * 0.2),
					ImVec2(p.x + width * 0.7, p.y + height * 0.8),
					ImGui::GetColorU32(ImVec4(0.0f, 0.0f, 0.0f, 1.0f)), 0);
			}

			return clicked;
		}
	}  // namespace gui
}  // namespace crl