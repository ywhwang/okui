
#include <gtest/gtest.h>

#include <okui/View.h>
#include <okui/ml/Context.h>
#include <okui/ml/Environment.h>

TEST(ml_Context, load) {
    okui::ml::Environment ml;
    okui::ml::Context context(&ml);

    auto element = context.load(R"(
    <view background-color="green">
        <view x="20" y="20" width="200" height="40" background-color="blue"></view>
    </view>
    )");

    auto view = element->view();
    ASSERT_TRUE(view);

    EXPECT_EQ(view->backgroundColor(), okui::Color::kGreen);
    EXPECT_EQ(view->subviews().size(), 1);
    view->layout();
    EXPECT_EQ(view->subviews().front()->bounds(), okui::Rectangle<double>(20, 20, 200, 40));
    EXPECT_EQ(view->subviews().front()->backgroundColor(), okui::Color::kBlue);
}

TEST(ml_Context, render) {
    okui::ml::Environment ml;
    okui::ml::Context context(&ml);

    // test with no variables
    EXPECT_EQ(context.render("hello"), "hello");

    context.define("foo", "bar");
    EXPECT_EQ(context.render("foo{foo}"), "foobar");

    context.define("unused", "!!!");
    EXPECT_EQ(context.render("foo{foo}"), "foobar");

    for (size_t i = 0; i <= fmt::ArgList::MAX_PACKED_ARGS; ++i) {
        context.define(std::to_string(i), "x");
    }
    EXPECT_EQ(context.render("foo{foo}"), "foobar");

    context.define("n", 1.234);
    EXPECT_EQ(context.render("{n:.2}"), "1.2");
}

