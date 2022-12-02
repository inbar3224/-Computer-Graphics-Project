#pragma once

#include <gui/application.h>
#include <utils/logger.h>
#include <gui/fancy3dapp.h>

using namespace crl::gui;

class App : public Fancy3DApp {
    public:
        App(const char* title = "Fancy app", std::string iconPath = DATA_FOLDER "/crl_icon_grey.png") : 
            Fancy3DApp(title, iconPath) {

        }

        virtual ~App() override {
        
        }

        void process() override {
            // Logger::consolePrint(Eigen::Vector3d(1.0, 0.0, 0.0), "Test\n");
        }

        // objects drawn with a shadowMapRenderer (during shadow pass) will cast a shadow
        virtual void drawShadowCastingObjects() override {
            model.draw(shadowMapRenderer);
        }

        // objects drawn with a shadowShader (during the render pass) will have shadows cast on them
        virtual void drawObjectsWithShadows() override {
            if (drawSelfShadows == true) {
                model.draw(shadowShader);
            }
            ground.draw(shadowShader);
        }

        // objects drawn with basic shadowShader (during the render pass) will not have shadows cast on them
        virtual void drawObjectsWithoutShadows() override {
            if (drawSelfShadows == false) {
                model.draw(basicShader);
            }
        }

        // draw silhouette 
        virtual void drawObjectsWithSilhouette() override {
            // take from drawObjectsWithShadows()
        }

        // cancel silhouette
        virtual void drawObjectsWithoutSilhouette() override {
            // take from drawObjectsWithoutShadows()
        }

        virtual bool drop(int count, const char** fileNames) override {
            model = Model(fileNames[count - 1]);
            return true;
        }
    public:
        Model model = Model(DATA_FOLDER "/meshes/3d models/nanosuit.obj");
};
