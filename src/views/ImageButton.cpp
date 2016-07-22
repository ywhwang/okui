#include "onair/okui/views/ImageButton.h"

#include "onair/okui/Application.h"

namespace onair {
namespace okui {
namespace views {

namespace {

constexpr auto kDepressedBrightness = 0.8;

constexpr auto DepressedColor(Color c) {
    auto color = (c * kDepressedBrightness).withAlphaF(c.alphaF());
    return color;
}

}

void ImageButton::setTextureResource(std::string resource, State state) {
    _stateImageView(state).setTextureResource(std::move(resource));
}

void ImageButton::setTextureFromURL(std::string url, State state) {
    _stateImageView(state).setTextureFromURL(std::move(url));
}

void ImageButton::setTextureFromURL(std::string url, std::string placeholderResource, State state) {
    _stateImageView(state).setTextureFromURL(std::move(url), std::move(placeholderResource));
}

void ImageButton::setTextureColor(Color color, State state) {
    _stateImageView(state).setTextureColor(color);
    if (state == State::kNormal) { _normalColor = color; }
}

void ImageButton::setTextureDistanceField(double edge, State state) {
    _stateImageView(state).setTextureDistanceField(edge);
}

void ImageButton::layout() {
    for (auto& i : _imageViews) {
        i.second.setBoundsRelative(0, 0, 1, 1);
    }
}

ImageView& ImageButton::_stateImageView(State state) {
    auto imageIt = _imageViews.find(state);

    if (imageIt == _imageViews.end()){
        auto& image = _imageViews[state];
        addHiddenSubview(&image);
        image.setBoundsRelative(0, 0, 1, 1);
        if (this->state() == state) {
            stateChanged();
        }

        return image;
    }

    return imageIt->second;
}

void ImageButton::stateChanged() {
    for (auto& i : _imageViews) {
        i.second.setIsVisible(i.first == state());
    }

    auto normalImageView = _imageViews.find(State::kNormal);

    switch (state()) {
        case State::kNormal:
            if (normalImageView != _imageViews.end()) {
                normalImageView->second.setTextureColor(_normalColor);
            }
            break;
        case State::kDepressed:
            if (!_imageViews.count(State::kDepressed) && normalImageView != _imageViews.end()) {
                normalImageView->second.setIsVisible(true);
                normalImageView->second.setTextureColor(DepressedColor(_normalColor));
            }
            break;
        default:
            break;
    }
}

}}}
