//
//
#ifndef NANOVG_CTX_H
#define NANOVG_CTX_H

#include <nanovg.h>

namespace nvg {
    using Context = NVGcontext;
    using Color = NVGcolor;
    using Paint = NVGpaint;
    using GlyphPosition = NVGglyphPosition;
    using TextRow = NVGtextRow;

    extern Context* current;

    Context* createContext();

	static inline void beginFrame(float windowWidth, float windowHeight, float devicePixelRatio) {
		nvgBeginFrame(current, windowWidth, windowHeight, devicePixelRatio);
	}

	static inline void cancelFrame() {
		nvgCancelFrame(current);
	}

	static inline void endFrame() {
		nvgEndFrame(current);
	}

	static inline void globalCompositeOperation(int op) {
		nvgGlobalCompositeOperation(current, op);
	}

	static inline void globalCompositeBlendFunc(int sfactor, int dfactor) {
        nvgGlobalCompositeBlendFunc(current, sfactor, dfactor);
    }

	static inline void globalCompositeBlendFuncSeparate(int srcRGB, int dstRGB, int srcAlpha, int dstAlpha) {
        nvgGlobalCompositeBlendFuncSeparate(current, srcRGB, dstRGB, srcAlpha, dstAlpha);
    }

	static inline Color RGBi(unsigned char r, unsigned char g, unsigned char b) {
        return nvgRGB(r, g, b);
    }

	static inline Color RGBf(float r, float g, float b) {
        return nvgRGBf(r, g, b);
    }

	static inline Color RGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
        return nvgRGBA(r, g, b, a);
    }

	static inline Color RGBAf(float r, float g, float b, float a) {
        return nvgRGBAf(r, g, b, a);
    }

    static inline Color lerpRGBA(Color c0, Color c1, float u) {
        return nvgLerpRGBA(c0, c1, u);
    }

    static inline Color transRGBA(Color c0, unsigned char a) {
        return nvgTransRGBA(c0, a);
    }

    static inline Color transRGBAf(Color c0, float a) {
        return nvgTransRGBAf(c0, a);
    }

    static inline Color HSL(float h, float s, float l) {
        return nvgHSL(h, s, l);
    }

    static inline Color HSLA(float h, float s, float l, unsigned char a) {
        return nvgHSLA(h, s, l, a);
    }

    static inline void save() {
        nvgSave(current);
    }

    static inline void restore() {
        nvgRestore(current);
    }

    static inline void reset() {
        nvgReset(current);
    }

    static inline void shapeAntiAlias(int enabled) {
        nvgShapeAntiAlias(current, enabled);
    }

    static inline void strokeColor(Color color) {
        nvgStrokeColor(current, color);
    }

    static inline void strokePaint(Paint paint) {
        nvgStrokePaint(current, paint);
    }

    static inline void fillColor(Color color) {
        nvgFillColor(current, color);
    }

    static inline void fillPaint(Paint paint) {
        nvgFillPaint(current, paint);
    }

    static inline void miterLimit(float limit) {
        nvgMiterLimit(current, limit);
    }

    static inline void strokeWidth(float size) {
        nvgStrokeWidth(current, size);
    }

    static inline void lineCap(int cap) {
        nvgLineCap(current, cap);
    }

    static inline void lineJoin(int join) {
        nvgLineJoin(current, join);
    }

    static inline void globalAlpha(float alpha) {
        nvgGlobalAlpha(current, alpha);
    }

    static inline void resetTransform() {
        nvgResetTransform(current);
    }

    static inline void transform(float a, float b, float c, float d, float e, float f) {
        nvgTransform(current, a, b, c, d, e, f);
    }

    static inline void translate(float x, float y) {
        nvgTranslate(current, x, y);
    }

    static inline void rotate(float angle) {
        nvgRotate(current, angle);
    }

    static inline void skewX(float angle) {
        nvgSkewX(current, angle);
    }

    static inline void skewY(float angle) {
        nvgSkewY(current, angle);
    }

    static inline void scale(float x, float y) {
        nvgScale(current, x, y);
    }

    static inline void currentTransform(float* xform) {
        nvgCurrentTransform(current, xform);
    }

    static inline void transformIdentity(float* dst) {
        nvgTransformIdentity(dst);
    }

    static inline void transformTranslate(float* dst, float tx, float ty) {
        nvgTransformTranslate(dst, tx, ty);
    }

    static inline void transformScale(float* dst, float sx, float sy) {
        nvgTransformScale(dst, sx, sy);
    }

    static inline void transformRotate(float* dst, float a) {
        nvgTransformRotate(dst, a);
    }

    static inline void transformSkewX(float* dst, float a) {
        nvgTransformSkewX(dst, a);
    }

    static inline void transformSkewY(float* dst, float a) {
        nvgTransformSkewY(dst, a);
    }

    static inline void transformMultiply(float* dst, const float* src) {
        nvgTransformMultiply(dst, src);
    }

    static inline void transformPremultiply(float* dst, const float* src) {
        nvgTransformPremultiply(dst, src);
    }

    static inline int transformInverse(float* dst, const float* src) {
        return nvgTransformInverse(dst, src);
    }

    static inline void transformPoint(float* dstx, float* dsty, const float* xform, float srcx, float srcy) {
        nvgTransformPoint(dstx, dsty, xform, srcx, srcy);
    }

    static inline float degToRad(float deg) {
        return nvgDegToRad(deg);
    }

    static inline float radToDeg(float rad) {
        return nvgRadToDeg(rad);
    }

    static inline int createImage(const char* filename, int imageFlags) {
        return nvgCreateImage(current, filename, imageFlags);
    }

    static inline int createImageMem(int imageFlags, unsigned char* data, int ndata) {
        return nvgCreateImageMem(current, imageFlags, data, ndata);
    }

    static inline int createImageRGBA(int w, int h, int imageFlags, const unsigned char* data) {
        return nvgCreateImageRGBA(current, w, h, imageFlags, data);
    }

    static inline void updateImage(int image, const unsigned char* data) {
        nvgUpdateImage(current, image, data);
    }

    static inline void imageSize(int image, int* w, int* h) {
        nvgImageSize(current, image, w, h);
    }

    static inline void deleteImage(int image) {
        nvgDeleteImage(current, image);
    }

    static inline Paint linearGradient(float sx, float sy, float ex, float ey, Color icol, Color ocol) {
        return nvgLinearGradient(current, sx, sy, ex, ey, icol, ocol);
    }

    static inline Paint boxGradient(float x, float y, float w, float h, float r, float f, Color icol, Color ocol) {
        return nvgBoxGradient(current, x, y, w, h, r, f, icol, ocol);
    }

    static inline Paint radialGradient(float cx, float cy, float inr, float outr, Color icol, Color ocol) {
        return nvgRadialGradient(current, cx, cy, inr, outr, icol, ocol);
    }

    static inline Paint imagePattern(float ox, float oy, float ex, float ey, float angle, int image, float alpha) {
        return nvgImagePattern(current, ox, oy, ex, ey, angle, image, alpha);
    }

    static inline void scissor(float x, float y, float w, float h) {
        nvgScissor(current, x, y, w, h);
    }

    static inline void intersectScissor(float x, float y, float w, float h) {
        nvgIntersectScissor(current, x, y, w, h);
    }

    static inline void resetScissor() {
        nvgResetScissor(current);
    }

    static inline void beginPath() {
        nvgBeginPath(current); 
    }

    static inline void moveTo(float x, float y) {
        nvgMoveTo(current, x, y);
    }

    static inline void lineTo(float x, float y) {
        nvgLineTo(current, x, y);
    }

    static inline void bezierTo(float c1x, float c1y, float c2x, float c2y, float x, float y) {
        nvgBezierTo(current, c1x, c1y, c2x, c2y, x, y);
    }

    static inline void quadTo(float cx, float cy, float x, float y) {
        nvgQuadTo(current, cx, cy, x, y);
    }

    static inline void arcTo(float x1, float y1, float x2, float y2, float radius) {
        nvgArcTo(current, x1, y1, x2, y2, radius);
    }

    static inline void closePath() {
        nvgClosePath(current);
    }

    static inline void pathWinding(int dir) {
        nvgPathWinding(current, dir);
    }

    static inline void arc(float cx, float cy, float r, float a0, float a1, int dir) {
        nvgArc(current, cx, cy, r, a0, a1, dir);
    }

    static inline void rect(float x, float y, float w, float h) {
        nvgRect(current, x, y, w, h);
    }

    static inline void roundedRect(float x, float y, float w, float h, float r) {
        nvgRoundedRect(current, x, y, w, h, r);
    }

    static inline void roundedRectVarying(float x, float y, float w, float h, float radTopLeft, float radTopRight, float radBottomRight, float radBottomLeft) {
        nvgRoundedRectVarying(current, x, y, w, h, radTopLeft, radTopRight, radBottomRight, radBottomLeft);
    }

    static inline void ellipse(float cx, float cy, float rx, float ry) {
        nvgEllipse(current, cx, cy, rx, ry);
    }

    static inline void circle(float cx, float cy, float r) {
        nvgCircle(current, cx, cy, r);
    }

    static inline void fill() {
        nvgFill(current);
    }

    static inline void stroke() {
        nvgStroke(current);
    }

    static inline int createFont(const char* name, const char* filename) {
        return nvgCreateFont(current, name, filename);
    }

    static inline int createFontAtIndex(const char* name, const char* filename, const int fontIndex) {
        return nvgCreateFontAtIndex(current, name, filename, fontIndex);
    }

    static inline int createFontMem(const char* name, unsigned char* data, int ndata, int freeData) {
        return nvgCreateFontMem(current, name, data, ndata, freeData);
    }

    static inline int createFontMemAtIndex(const char* name, unsigned char* data, int ndata, int freeData, const int fontIndex) {
        return nvgCreateFontMemAtIndex(current, name, data, ndata, freeData, fontIndex);
    }

    static inline int findFont(const char* name) {
        return nvgFindFont(current, name);
    }

    static inline int addFallbackFontId(int baseFont, int fallbackFont) {
        return nvgAddFallbackFontId(current, baseFont, fallbackFont);
    }

    static inline int addFallbackFont(const char* baseFont, const char* fallbackFont) {
        return nvgAddFallbackFont(current, baseFont, fallbackFont);
    }

    static inline void resetFallbackFontsId(int baseFont) {
        nvgResetFallbackFontsId(current, baseFont);
    }

    static inline void resetFallbackFonts(const char* baseFont) {
        nvgResetFallbackFonts(current, baseFont);
    }

    static inline void fontSize(float size) {
        nvgFontSize(current, size);
    }

    static inline void fontBlur(float blur) {
        nvgFontBlur(current, blur);
    }

    static inline void textLetterSpacing(float spacing) {
        nvgTextLetterSpacing(current, spacing);
    }

    static inline void textLineHeight(float lineHeight) {
        nvgTextLineHeight(current, lineHeight);
    }

    static inline void textAlign(int align) {
        nvgTextAlign(current, align);
    }

    static inline void fontFaceId(int font) {
        nvgFontFaceId(current, font);
    }

    static inline void fontFace(const char* font) {
        nvgFontFace(current, font);
    }

    static inline float text(float x, float y, const char* string, const char* end) {
        return nvgText(current, x, y, string, end);
    }

    static inline void textBox(float x, float y, float breakRowWidth, const char* string, const char* end) {
        nvgTextBox(current, x, y, breakRowWidth, string, end);
    }

    static inline float textBounds(float x, float y, const char* string, const char* end, float* bounds) {
        return nvgTextBounds(current, x, y, string, end, bounds);
    }

    static inline void textBoxBounds(float x, float y, float breakRowWidth, const char* string, const char* end, float* bounds) {
        nvgTextBoxBounds(current, x, y, breakRowWidth, string, end, bounds);
    }

    static inline int textGlyphPositions(float x, float y, const char* string, const char* end, GlyphPosition* positions, int maxPositions) {
        return nvgTextGlyphPositions(current, x, y, string, end, positions, maxPositions);
    }

    static inline void textMetrics(float* ascender, float* descender, float* lineh) {
        nvgTextMetrics(current, ascender, descender, lineh);
    }

    static inline int textBreakLines(const char* string, const char* end, float breakRowWidth, TextRow* rows, int maxRows) {
        return nvgTextBreakLines(current, string, end, breakRowWidth, rows, maxRows);
    }
}

#endif // NANOVG_CTX_H
