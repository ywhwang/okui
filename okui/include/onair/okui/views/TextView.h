#pragma once

#include "onair/okui/config.h"

#include "onair/okui/BitmapFont.h"
#include "onair/okui/View.h"

namespace onair {
namespace okui {
namespace views {

class TextView : public View {
public:
	enum class HorizontalAlignment {
		kLeft,
		kCenter,
		kRight,
	};

	enum class VerticalAlignment {
		kBottom,
		kCenter,
		kTop,
	};

	void setAlignment(HorizontalAlignment horizontal, VerticalAlignment vertical = VerticalAlignment::kTop);
	void setFont(std::shared_ptr<BitmapFont> font, double size);
	void setText(const char* text);
	void setTextColor(double r, double g, double b, double a = 1.0);

	void setMultiLine(bool enabled) { _multiLine = enabled;  _computeLines(); }
	bool multiLine() const          { return _multiLine; }

	double textWidth() const { return _textWidth; }

	enum class Weight {
	    kRegular,
	    kHeavy,
	};

	void setWeight(Weight weight) { _weight = weight; }

	virtual void render() override;
	virtual void layout() override;

private:
	HorizontalAlignment _horizontalAlignment = HorizontalAlignment::kLeft;
	VerticalAlignment _verticalAlignment = VerticalAlignment::kTop;
	Weight _weight = Weight::kRegular;
	std::shared_ptr<BitmapFont> _font;
	double _fontSize;
	std::basic_string<BitmapFont::GlyphId> _text;
	std::vector<std::basic_string<BitmapFont::GlyphId>> _lines;
	double _textWidth;
	double _textColorR{0.0}, _textColorG{0.0}, _textColorB{0.0}, _textColorA{1.0};
	bool _multiLine = true;

	void _computeLines();
	void _renderBitmapText(shaders::DistanceFieldShader* shader);
};

}}}