#include <gui\application.h>
#include <gui/renderer.h>
#include <gui/camera.h>
#include <gui/model.h>
#include <gui/shader.h>
#include <gui/shadow_casting_light.h>
#include <gui/shadow_map_fbo.h>
#include <imgui_widgets/ImGuizmo.h>
#include <imgui_widgets/imGuIZMOquat.h>

namespace crl {
namespace gui {


class Fancy3DApp : public Application {
public:
    Fancy3DApp(const char* title = "Shadows demo",
        std::string iconPath = DATA_FOLDER
        "/crl_icon_grey.png")
        : Application(title, iconPath) {
        camera.aspectRatio = float(width) / height;

        if (!shadowMapFBO.Init(this->width, this->height)) {
            std::cout << "Shadow map initialization failed\n";
            exit(0);
        }

        glEnable(GL_DEPTH_TEST);
    }

	virtual void resizeWindow(int width, int height) override;

    bool mouseMove(double xpos, double ypos) override;

    bool scrollWheel(double xoffset, double yoffset) override;

    void draw() override;

    virtual void prepareToDraw() {}

	virtual void drawAuxiliaryInfo();

	virtual void drawImGui();

    void shadowPass();

    void renderPass();

    // objects drawn with a shadowMapRenderer (during shadow pass) will cast a
    // shadow
    virtual void drawShadowCastingObjects() = 0;

    // objects drawn with a shadowShader (during the render pass) will have
    // shadows cast on them
    virtual void drawObjectsWithShadows() = 0;

    // objects drawn with basic shadowShader (during the render pass) will not
    // have shadows cast on them
    virtual void drawObjectsWithoutShadows() = 0;

    TrackingCamera camera;
    ShadowCastingLight light;
    ShadowMapFBO shadowMapFBO;
    float shadowbias = 0.0001f;

    Shader shadowShader =
        Shader(SHADER_FOLDER "/basic_lighting.vert",
            SHADER_FOLDER "/basic_shadow_lighting.frag");
    Shader shadowMapRenderer = Shader(SHADER_FOLDER "/render_shadow.vert",
        SHADER_FOLDER "/render_shadow.frag");
    Shader basicShader = Shader(SHADER_FOLDER "/basic_lighting.vert",
        SHADER_FOLDER "/basic_lighting.frag");

	bool drawSelfShadows = false;
	Model ground = getGroundModel();
private:
    int PCF_mode=0;
    int PCF_samples_num=25;
    float shadow_spread=0.002;
    float light_size = 1;
};


}  // namespace gui
}  // namespace crl