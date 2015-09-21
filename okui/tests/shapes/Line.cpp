#include "gtest/gtest.h"

#include "../RenderOnce.h"
#include "../TestFramebuffer.h"

#include "onair/okui/shapes/Line.h"

using namespace onair;
using namespace onair::okui;

TEST(LineShape, normal) {
    RenderOnce([&] (View* view) {
        TestFramebuffer framebuffer(320, 200);

        auto shader = view->colorShader();
        shader->setColor(1, 1, 1, 1);
        shader->setTransformation(framebuffer.transformation());

        Point<double> p1(12.0, 20.0);
        Point<double> p2(123.0, 66.0);

        shapes::Line(p1.x, p1.y, p2.x, p2.y, 5).draw(shader);
        shader->flush();

        framebuffer.finish();

        framebuffer.iteratePixels([&](int x, int y, Color pixel) {
            static constexpr auto aa = 1.0; // antialiasing buffer
            auto p = Point<double>((double)x, (double)y);
            auto t = (p - p1).dot(p2 - p1) / (p2 - p1).magnitudeSquared();

            if (t < 0) {        // before the start
                if ((p1-p).magnitude() < aa) {
                    EXPECT_EQ(pixel, Color(0, 0, 0, 1));
                }
            } else if (t > 1) { // after the end
                if ((p-p2).magnitude() < aa) {
                    EXPECT_EQ(pixel, Color(0, 0, 0, 1));
                }
            } else {
                auto proj = p1 + (p2-p1) * t;
                auto distance = (proj-p).magnitude();
                if (distance < (2.5-aa) && (p-p2).magnitude() < aa && (p1-p).magnitude() < aa) {
                    EXPECT_EQ(pixel, Color(1, 1, 1, 1));
                }
            }
        });
    });
}
