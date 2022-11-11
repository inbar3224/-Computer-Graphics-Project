#include <gui/shadow_map_fbo.h>
#include <stdio.h>
#include <array>

namespace crl {
namespace gui {

ShadowMapFBO::ShadowMapFBO() {
    fbo = 0;
    shadowMap = 0;
}

ShadowMapFBO::~ShadowMapFBO() {
    if (fbo != 0) {
        glDeleteFramebuffers(1, &fbo);
    }

    if (shadowMap != 0) {
        glDeleteTextures(1, &shadowMap);
    }
    if (depthSampler !=0)
        glDeleteSamplers(1, &depthSampler);
}

bool ShadowMapFBO::Init(GLuint bufferWidth, GLuint bufferHeight) {


    // Create the depth buffer
    glGenTextures(1, &shadowMap);
    glBindTexture(GL_TEXTURE_2D, shadowMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, bufferWidth,
                 bufferHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE,
                    GL_COMPARE_REF_TO_TEXTURE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    // Create the FBO
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
                           shadowMap, 0);

    // Disable writes to the color buffer
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (Status != GL_FRAMEBUFFER_COMPLETE) {
        printf("FB error, status: 0x%x\n", Status);
        return false;
    }

    // create depth sampler (for PCSS)
	glGenSamplers(1, &depthSampler);
	glSamplerParameteri(depthSampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glSamplerParameteri(depthSampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glSamplerParameteri(depthSampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glSamplerParameteri(depthSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    this->bufferHeight = bufferHeight;
    this->bufferWidth = bufferWidth;
    return true;
}

void ShadowMapFBO::BindForWriting() {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
}

void ShadowMapFBO::BindForReading(GLuint ShadowTextureUnit, GLuint DepthTextureUnit) {
    glActiveTexture(ShadowTextureUnit);
    glBindTexture(GL_TEXTURE_2D, shadowMap);

	glActiveTexture(DepthTextureUnit);
	glBindTexture(GL_TEXTURE_2D, shadowMap);
	glBindSampler(DepthTextureUnit, depthSampler);
    int a = glGetError();
}


void ShadowMapFBO::generateRandom3DTexture()
{
//     std::array<std::array<std::array<glm::vec2, 32>, 32>, 32> randomAngles;
//     const int RESOLUTION = 32;
//     srand(time(nullptr));
//     for (size_t i = 0; i < RESOLUTION; ++i)
//     {
//         for (size_t j = 0; j < RESOLUTION; ++j)
//         {
//             for (size_t k = 0; k < RESOLUTION; ++k)
//             {
//                 float randomAngle = static_cast<float>(rand()) / RAND_MAX * 2 * glm::pi<float>();
//                 randomAngles[i][j][k] = glm::vec2(glm::cos(randomAngle) * 0.5f + 0.5f, glm::sin(randomAngle) * 0.5f + 0.5f);
//             }
//         }
//     }
// 
//     glGenTextures(1, &anglesTexture);
//     glBindTexture(GL_TEXTURE_3D, anglesTexture);
//     glTexImage3D(GL_TEXTURE_3D, 0, GL_RG16F, RESOLUTION, RESOLUTION, RESOLUTION, 0, GL_RG, GL_FLOAT, &randomAngles);
// 
//     glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//     glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//     glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
//     glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//     glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}
}  // namespace gui
}  // namespace crl