#pragma once

#include "onair/okui/config.h"

#include "onair/okui/Color.h"
#include "onair/okui/View.h"

#include "onair/optional.h"

#include <unordered_map>

namespace onair {
namespace okui {
namespace views {

class Button : public View {
public:
    enum class State {
        kNormal,
        kDepressed,
    };

    State state() const { return _state; }

    void setAction(std::function<void()> action) { _action = action; }

    void setTextureResource(std::string resource, State state = State::kNormal);
    void setTextureFromURL(std::string url, State state = State::kNormal);
    void setTextureColor(Color color, State state = State::kNormal);
    void setTextureDistanceField(double edge = 0.5, State state = State::kNormal);

    TextureHandle texture(State state = State::kNormal);

    void press();

    virtual void render() override;

    virtual bool canBecomeFocus() override { return true; }
    virtual void buttonDown(const okui::Controller& controller, size_t button) override;
    virtual void buttonUp(const okui::Controller& controller, size_t button) override;
    virtual void mouseDown(MouseButton button, double x, double y) override;
    virtual void mouseUp(MouseButton button, double startX, double startY, double x, double y) override;
    virtual void mouseExit() override;
    virtual void keyDown(KeyCode key, KeyModifiers mod, bool repeat) override;
    virtual void windowChanged() override;

private:
    void _press();
    void _unpress();
    void _setTexture(std::string texture, State state, bool fromURL);

    State _state = State::kNormal;
    std::function<void()> _action;

    struct Texture {
        TextureHandle handle;
        std::string resource;
        bool fromURL = false;
        optional<Color> color;
        optional<double> distanceFieldEdge;
    };

    std::unordered_map<State, Texture> _textures;
};

}}}
