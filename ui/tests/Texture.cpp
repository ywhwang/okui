#include "gtest/gtest.h"

#include "TestApplication.h"
#include "bittorrent/ui/Texture.h"
#include "bittorrent/ui/Window.h"

using namespace bittorrent;
using namespace bittorrent::ui;

struct Pixel {
    uint8_t r{0}, g{0}, b{0}, a{255};
    bool operator==(const Pixel& other) const {
        return r == other.r && g == other.g && b == other.b && a == other.a;
    }
};

#include "TexturePixels.h"

static void TextureTest(const unsigned char* imageData, size_t imageDataSize, int width, int height, const Pixel* expectedPixels) {
    TestApplication application;

    bool didRender = false;

    struct TestWindow : Window {
        TestWindow(Application* application, bool* didRender, const unsigned char* imageData, size_t imageDataSize, int width, int height, const Pixel* expectedPixels)
            : Window(application), didRender(didRender), expectedPixels(expectedPixels)
        {
            texture = loadTextureFromMemory(std::make_shared<std::string>((const char*)imageData, imageDataSize));
            
            EXPECT_NE(texture, nullptr);
            
            EXPECT_FALSE(texture->isLoaded());
            
            EXPECT_EQ(texture->width(), width);
            EXPECT_EQ(texture->height(), height);
        }
        
        virtual void render() override {
            EXPECT_TRUE(texture->isLoaded());

            std::vector<Pixel> pixels;
            pixels.resize(texture->width() * texture->height());            
            
            glBindTexture(GL_TEXTURE_2D, texture->id());
            glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());

            for (int y = 0; y < texture->height(); ++y) {
                for (int x = 0; x < texture->width(); ++x) {
                    auto& pixel = pixels[x + texture->width() * y];
                    auto& expected = expectedPixels[x + texture->width() * y];
                    EXPECT_EQ(pixel, expected);
                }
            }
            
            *didRender = true;
            application()->quit();
        }
        
        bool* didRender = nullptr;
        const Pixel* expectedPixels = nullptr;
        std::shared_ptr<Texture> texture;
    } window(&application, &didRender, imageData, imageDataSize, width, height, expectedPixels);
    
    window.open();

    application.run();

    EXPECT_TRUE(didRender);
}

TEST(Texture, png16BitRGB) {
    // basn2c16 from http://www.schaik.com/pngsuite/pngsuite_bas_png.html
    static const unsigned char imageData[] = {
        0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A, 0x00, 0x00, 0x00, 0x0D, 0x49, 0x48, 0x44, 0x52, 
        0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x20, 0x10, 0x02, 0x00, 0x00, 0x00, 0xAC, 0x88, 0x31, 
        0xE0, 0x00, 0x00, 0x00, 0x04, 0x67, 0x41, 0x4D, 0x41, 0x00, 0x01, 0x86, 0xA0, 0x31, 0xE8, 0x96, 
        0x5F, 0x00, 0x00, 0x00, 0xE5, 0x49, 0x44, 0x41, 0x54, 0x78, 0x9C, 0xD5, 0x96, 0xC1, 0x0A, 0x83, 
        0x30, 0x10, 0x44, 0xA7, 0xE0, 0x41, 0x7F, 0xCB, 0x7E, 0xB7, 0xFD, 0xAD, 0xF6, 0x96, 0x1E, 0x06, 
        0x03, 0x92, 0x86, 0x26, 0x66, 0x93, 0xCC, 0x7A, 0x18, 0x86, 0x45, 0xE4, 0x3D, 0xD6, 0xA0, 0x8F, 
        0x10, 0x42, 0x00, 0x3E, 0x2F, 0xE0, 0x9A, 0xEF, 0x64, 0x72, 0x73, 0x7E, 0x18, 0x3D, 0x27, 0x33, 
        0x5F, 0xCE, 0xE2, 0xF3, 0x5A, 0x77, 0xB7, 0x02, 0xEB, 0xCE, 0x74, 0x28, 0x70, 0xA2, 0x33, 0x97, 
        0xF3, 0xED, 0xF2, 0x70, 0x5D, 0xD1, 0x01, 0x60, 0xF3, 0xB2, 0x81, 0x5F, 0xE8, 0xEC, 0xF2, 0x02, 
        0x79, 0x74, 0xA6, 0xB0, 0xC0, 0x3F, 0x74, 0xA6, 0xE4, 0x19, 0x28, 0x43, 0xE7, 0x5C, 0x6C, 0x03, 
        0x35, 0xE8, 0xEC, 0x32, 0x02, 0xF5, 0xE8, 0x4C, 0x01, 0x81, 0xBB, 0xE8, 0xCC, 0xA9, 0x67, 0xA0, 
        0x0D, 0x9D, 0xF3, 0x49, 0x1B, 0xB0, 0x40, 0x67, 0x1F, 0x2E, 0x60, 0x87, 0xCE, 0x1C, 0x28, 0x60, 
        0x8D, 0x1E, 0x05, 0xF8, 0xC7, 0xEE, 0x0F, 0x1D, 0x00, 0xB6, 0x67, 0xE7, 0x0D, 0xF4, 0x44, 0x67, 
        0xEF, 0x26, 0xD0, 0x1F, 0xBD, 0x9B, 0xC0, 0x28, 0xF4, 0x28, 0x60, 0xF7, 0x1D, 0x18, 0x8B, 0xCE, 
        0xFB, 0x8D, 0x36, 0x30, 0x03, 0x9D, 0xBD, 0x59, 0x60, 0x1E, 0x7A, 0xB3, 0xC0, 0x6C, 0xF4, 0x28, 
        0x50, 0x7F, 0x06, 0x34, 0xD0, 0x39, 0xAF, 0xDC, 0x80, 0x12, 0x3A, 0x7B, 0xB1, 0x80, 0x1E, 0x7A, 
        0xB1, 0x80, 0x2A, 0x7A, 0x14, 0xC8, 0x9F, 0x01, 0x6D, 0x74, 0xCE, 0x33, 0x1B, 0xF0, 0x80, 0xCE, 
        0x9E, 0x08, 0xF8, 0x41, 0x4F, 0x04, 0xBC, 0xA1, 0x33, 0xBF, 0xE6, 0x42, 0xFE, 0x5E, 0x07, 0xBB, 
        0xC4, 0xEC, 0x00, 0x00, 0x00, 0x00, 0x49, 0x45, 0x4E, 0x44, 0xAE, 0x42, 0x60, 0x82,
    };
    
    TextureTest(imageData, sizeof(imageData), 32, 32, png16BitRGBPixels);
}

TEST(Texture, png8BitRGBA) {
    // basn6a08 from http://www.schaik.com/pngsuite/pngsuite_bas_png.html
    static const unsigned char imageData[] = {
        0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A, 0x00, 0x00, 0x00, 0x0D, 0x49, 0x48, 0x44, 0x52, 
        0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x20, 0x08, 0x06, 0x00, 0x00, 0x00, 0x73, 0x7A, 0x7A, 
        0xF4, 0x00, 0x00, 0x00, 0x04, 0x67, 0x41, 0x4D, 0x41, 0x00, 0x01, 0x86, 0xA0, 0x31, 0xE8, 0x96, 
        0x5F, 0x00, 0x00, 0x00, 0x6F, 0x49, 0x44, 0x41, 0x54, 0x78, 0x9C, 0xED, 0xD6, 0x31, 0x0A, 0x80, 
        0x30, 0x0C, 0x46, 0xE1, 0x27, 0x64, 0x68, 0x4F, 0xA1, 0xF7, 0x3F, 0x55, 0x04, 0x8F, 0x21, 0xC4, 
        0xDD, 0xC5, 0x45, 0x78, 0x1D, 0x52, 0xE8, 0x50, 0x28, 0xFC, 0x1F, 0x4D, 0x28, 0xD9, 0x8A, 0x01, 
        0x30, 0x5E, 0x7B, 0x7E, 0x9C, 0xFF, 0xBA, 0x33, 0x83, 0x1D, 0x75, 0x05, 0x47, 0x03, 0xCA, 0x06, 
        0xA8, 0xF9, 0x0D, 0x58, 0xA0, 0x07, 0x4E, 0x35, 0x1E, 0x22, 0x7D, 0x80, 0x5C, 0x82, 0x54, 0xE3, 
        0x1B, 0xB0, 0x42, 0x0F, 0x5C, 0xDC, 0x2E, 0x00, 0x79, 0x20, 0x88, 0x92, 0xFF, 0xE2, 0xA0, 0x01, 
        0x36, 0xA0, 0x7B, 0x40, 0x07, 0x94, 0x3C, 0x10, 0x04, 0xD9, 0x00, 0x19, 0x50, 0x36, 0x40, 0x7F, 
        0x01, 0x1B, 0xF0, 0x00, 0x52, 0x20, 0x1A, 0x9C, 0x16, 0x0F, 0xB8, 0x4C, 0x00, 0x00, 0x00, 0x00, 
        0x49, 0x45, 0x4E, 0x44, 0xAE, 0x42, 0x60, 0x82, 
    };
    
    TextureTest(imageData, sizeof(imageData), 32, 32, png8BitRGBAPixels);
}