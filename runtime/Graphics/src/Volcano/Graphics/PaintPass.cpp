#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <limits>
#include <numbers>
#include <stdexcept>

#include <Volcano/Graphics/PaintPass.h>

VOLCANO_GRAPHICS_BEGIN

PaintPass::PaintPass() {
    program_.attachSource(GL_VERTEX_SHADER, R"(#version 330 core
layout(location=0) in vec2 aPosition;
layout(location=1) in vec2 aUV;
layout(location=2) in float aCoverage;
uniform vec2 uViewport;
out vec2 vPosition;
out vec2 vUV;
out float vCoverage;
void main() {
    vPosition = aPosition;
    vUV = aUV;
    vCoverage = aCoverage;
    gl_Position = vec4(aPosition.x/uViewport.x*2.0-1.0, 1.0-aPosition.y/uViewport.y*2.0, 0, 1);
})");
    program_.attachSource(GL_FRAGMENT_SHADER, R"(#version 330 core
in vec2 vPosition;
in vec2 vUV;
in float vCoverage;
uniform vec4 uInner;
uniform vec4 uOuter;
uniform int uGradient;
uniform vec2 uStart;
uniform vec2 uEnd;
uniform vec2 uRadii;
uniform mat3 uInverse;
uniform float uOpacity;
uniform int uMode;
uniform sampler2D uImage;
out vec4 color;
void main() {
    vec2 p = (uInverse * vec3(vPosition, 1)).xy;
    float t = 0.0;
    if (uGradient == 1) {
        vec2 d = uEnd-uStart;
        t = clamp(dot(p-uStart,d)/dot(d,d),0.0,1.0);
    } else if (uGradient == 2) {
        t = clamp((length(p-uStart)-uRadii.x)/(uRadii.y-uRadii.x),0.0,1.0);
    }
    vec4 c = mix(uInner,uOuter,t);
    c.rgb *= c.a;
    if (uMode == 1) {
        vec4 image = texture(uImage,vUV);
        c = image * c;
    } else if (uMode == 2) {
        c *= texture(uImage,vUV).r;
    }
    color = c * (uOpacity * clamp(vCoverage,0.0,1.0));
})");
    program_.link();
    viewport_ = program_.uniformLocation("uViewport");
    inner_ = program_.uniformLocation("uInner");
    outer_ = program_.uniformLocation("uOuter");
    gradient_ = program_.uniformLocation("uGradient");
    start_ = program_.uniformLocation("uStart");
    end_ = program_.uniformLocation("uEnd");
    radii_ = program_.uniformLocation("uRadii");
    inverse_ = program_.uniformLocation("uInverse");
    opacity_ = program_.uniformLocation("uOpacity");
    mode_ = program_.uniformLocation("uMode");
    vao_.vertexAttribPointer(buffer_, 0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, x));
    vao_.vertexAttribPointer(buffer_, 1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, u));
    vao_.vertexAttribPointer(buffer_, 2, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, coverage));
}

void PaintPass::setAtlasBudget(std::size_t bytes) {
    if (bytes < PageSize * PageSize) {
        throw std::invalid_argument("Font atlas budget must fit at least one 1024x1024 page");
    }
    budget_ = bytes;
    while (atlasBytes() > budget_) {
        pages_.pop_back();
    }
}

std::size_t PaintPass::atlasBytes() const noexcept {
    return pages_.size() * PageSize * PageSize;
}

PaintPass::FontCacheStats PaintPass::fontCacheStats() const noexcept {
    auto result = stats_;
    result.atlasBytes = atlasBytes();
    result.glyphs = 0;
    for (const auto& page : pages_) {
        result.glyphs += page.entries.size();
    }
    return result;
}

void PaintPass::prewarm(const Font& font, std::string_view text, unsigned int pixels) {
    for (const auto& shaped : font.shape(text, static_cast<float>(pixels))) {
        if (shaped.font != nullptr) {
            glyph(*shaped.font, shaped.index, pixels);
        }
    }
}

void PaintPass::render(const Texture& texture, Target& target) {
    if (target.contains(texture)) {
        throw std::invalid_argument("HUD cannot sample its output texture");
    }
    if (!beginTarget(target, false)) {
        return;
    }
    glyph_texture_ = 0;
    const World::Canvas::State state;
    paint(state, 1);
    texture.texture().bind(0);
    quad({0.0f, 0.0f, static_cast<float>(width_), static_cast<float>(height_)},
        {0.0f, 1.0f, 1.0f, -1.0f}, Eigen::Affine2f::Identity());
    endTarget();
}

void PaintPass::render(const World::Canvas& commands, Target& target, bool clear) {
    VOLCANO_ASSERT(commands.ready());
    for (const auto& command : commands.commands()) {
        if (command.type == World::Canvas::Command::Type::Image) {
            const auto* texture = resolveTexture(command);
            if (texture != nullptr && target.contains(*texture)) {
                throw std::invalid_argument("VG cannot sample its output texture");
            }
        }
    }
    glyph_vertices_.clear();
    glyph_texture_ = 0;
    if (!beginTarget(target, clear)) {
        return;
    }
    for (const auto& command : commands.commands()) {
        paint(command.state, 0);
        switch (command.type) {
        case World::Canvas::Command::Type::Fill:
            shape(command, false);
            break;
        case World::Canvas::Command::Type::Stroke:
            shape(command, true);
            break;
        case World::Canvas::Command::Type::Image: {
            const auto* texture = resolveTexture(command);
            if (texture != nullptr) {
                program_.uniform(mode_, 1);
                texture->texture().bind(0);
                quad(command.destination, {0, 1, 1, -1}, command.state.transform);
            }
            break;
        }
        case World::Canvas::Command::Type::Text: {
            const auto* font = resolveFont(command);
            if (font == nullptr) {
                break;
            }
            program_.uniform(mode_, 2);
            const auto& text = command.text;
            float offset = 0;
            if (command.state.align != World::Canvas::TextAlign::Left) {
                offset = font->measure(text, command.pixels).width;
                if (command.state.align == World::Canvas::TextAlign::Center) {
                    offset *= 0.5f;
                }
            }
            const float displayScale = std::max(command.state.transform.linear().col(0).norm(),
                command.state.transform.linear().col(1).norm());
            const float rasterSize = std::ceil(command.pixels * displayScale);
            if (!(rasterSize >= 1 && rasterSize <= 4096)) {
                throw std::invalid_argument("Transformed font size exceeds rasterizer limits");
            }
            const auto pixels = static_cast<unsigned int>(rasterSize);
            const float ratio = command.pixels / pixels;
            for (const auto& shaped : font->shape(text, command.pixels)) {
                if (shaped.font == nullptr) {
                    continue;
                }
                const auto image = glyph(*shaped.font, shaped.index, pixels);
                if (glyph_texture_ != image.texture->id()) {
                    flushGlyphs();
                    glyph_texture_ = image.texture->id();
                }
                const auto& entry = image.entry;
                quad({command.destination.left + shaped.x + entry.left * ratio - offset,
                    command.destination.top + shaped.y - entry.top * ratio, entry.width * ratio, entry.height * ratio},
                    entry.uv, command.state.transform);
            }
            flushGlyphs();
            glyph_texture_ = 0;
            break;
        }
        }
    }
    endTarget();
}

PaintPass::GlyphImage PaintPass::glyph(const Font& font, std::uint32_t index, unsigned int pixels) {
    const auto key = std::tuple(font.id(), index, pixels);
    ++serial_;
    for (auto& page : pages_) {
        if (const auto it = page.entries.find(key); it != page.entries.end()) {
            ++stats_.hits;
            page.used = serial_;
            return {&page.texture, it->second};
        }
    }
    const auto bitmap = font.rasterize(index, pixels);
    ++stats_.rasterizations;
    if (bitmap.width + 2 > PageSize || bitmap.height + 2 > PageSize) {
        throw std::length_error("Glyph exceeds font atlas page dimensions");
    }
    Page* destination = nullptr;
    bool allocate = false;
    for (auto& page : pages_) {
        if (page.x + bitmap.width + 1 > PageSize) {
            page.x = 1;
            page.y += page.row + 1;
            page.row = 0;
        }
        if (page.y + bitmap.height + 1 <= PageSize) {
            destination = &page;
            break;
        }
    }
    if (destination == nullptr) {
        flushGlyphs();
        if (atlasBytes() + PageSize * PageSize <= budget_) {
            pages_.emplace_back();
            destination = &pages_.back();
            allocate = true;
        } else {
            destination = &*std::min_element(pages_.begin(), pages_.end(), [](const auto& a, const auto& b) {
                return a.used < b.used;
            });
            destination->entries.clear();
            destination->x = 1;
            destination->y = 1;
            destination->row = 0;
        }
    }
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
    if (allocate) {
        destination->texture.parameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        destination->texture.parameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        destination->texture.parameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        destination->texture.parameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        destination->texture.image2D(0, GL_R8, PageSize, PageSize, 0, GL_RED, GL_UNSIGNED_BYTE, nullptr);
    }
    // Reuse page storage on eviction; clear each glyph's border to prevent filtering old pixels.
    std::vector<std::uint8_t> padded(static_cast<std::size_t>(bitmap.width+2)*(bitmap.height+2),0);
    for (int row = 0; row < bitmap.height; ++row) {
        std::copy_n(bitmap.pixels.data()+static_cast<std::size_t>(row)*bitmap.width,bitmap.width,
            padded.data()+static_cast<std::size_t>(row+1)*(bitmap.width+2)+1);
    }
    stats_.uploadedBytes += padded.size();
    Entry entry{{static_cast<float>(destination->x) / PageSize, static_cast<float>(destination->y) / PageSize,
        static_cast<float>(bitmap.width) / PageSize, static_cast<float>(bitmap.height) / PageSize},
        bitmap.left, bitmap.top, bitmap.width, bitmap.height};
    destination->texture.subImage2D(0,destination->x-1,destination->y-1,bitmap.width+2,bitmap.height+2,
        GL_RED,GL_UNSIGNED_BYTE,padded.data());
    destination->x += bitmap.width + 2;
    destination->row = std::max(destination->row, bitmap.height + 1);
    destination->used = serial_;
    destination->entries.emplace(key, entry);
    return {&destination->texture, entry};
}

void PaintPass::curve(std::vector<Eigen::Vector2f>& points, const Eigen::Vector2f& a,
    const Eigen::Vector2f& b, const Eigen::Vector2f& c, const Eigen::Vector2f& d, int depth) const {
    const Eigen::Vector2f delta = d - a;
    const float length = delta.norm();
    auto distance = [&](const Eigen::Vector2f& point) {
        return length < 1e-6f ? (point - a).norm() : std::abs(delta.x()*(a.y()-point.y()) - delta.y()*(a.x()-point.x())) / length;
    };
    if (depth >= 12 || (std::max(distance(b), distance(c)) <= 0.25f &&
        (b-a).norm()+(c-b).norm()+(d-c).norm()-length <= 0.25f)) {
        points.push_back(d);
        return;
    }
    const Eigen::Vector2f ab = (a+b)*0.5f, bc = (b+c)*0.5f, cd = (c+d)*0.5f;
    const Eigen::Vector2f abc = (ab+bc)*0.5f, bcd = (bc+cd)*0.5f, mid = (abc+bcd)*0.5f;
    curve(points,a,ab,abc,mid,depth+1);
    curve(points,mid,bcd,cd,d,depth+1);
}

std::vector<PaintPass::Contour> PaintPass::flatten(const World::Canvas::Command& command) const {
    std::vector<Contour> contours;
    const auto& transform = command.state.transform;
    Eigen::Vector2f previous = Eigen::Vector2f::Zero();
    for (const auto& segment : command.path) {
        const Eigen::Vector2f point = transform * segment.point;
        if (!point.allFinite() || !(transform*segment.control1).allFinite() || !(transform*segment.control2).allFinite()) {
            throw std::invalid_argument("VG path transform overflow");
        }
        if (segment.verb != World::Canvas::Verb::Move && segment.verb != World::Canvas::Verb::Close &&
            !contours.empty() && contours.back().closed) {
            contours.push_back({{previous},false});
        }
        switch (segment.verb) {
        case World::Canvas::Verb::Move:
            contours.push_back({{point}, false});
            previous = point;
            break;
        case World::Canvas::Verb::Line:
            contours.back().points.push_back(point);
            previous = point;
            break;
        case World::Canvas::Verb::Quad: {
            const Eigen::Vector2f control = transform * segment.control1;
            curve(contours.back().points, previous, previous+(control-previous)*(2.0f/3),
                point+(control-point)*(2.0f/3), point, 0);
            previous = point;
            break;
        }
        case World::Canvas::Verb::Cubic:
            curve(contours.back().points,previous,transform*segment.control1,transform*segment.control2,point,0);
            previous = point;
            break;
        case World::Canvas::Verb::Close:
            contours.back().closed = true;
            previous = contours.back().points.front();
            break;
        }
    }
    for (auto& contour : contours) {
        auto& points = contour.points;
        points.erase(std::unique(points.begin(),points.end(),[](const auto& a,const auto& b) {
            return (a-b).squaredNorm() < 1e-10f;
        }),points.end());
        if (points.size() > 1 && (points.front()-points.back()).squaredNorm() < 1e-10f) {
            points.pop_back();
            contour.closed = true;
        }
    }
    return contours;
}

void PaintPass::draw(std::span<const Vertex> vertices, GLenum mode) {
    if (vertices.empty()) {
        return;
    }
    if (vertices.size() > static_cast<std::size_t>(std::numeric_limits<GLsizei>::max())) {
        throw std::length_error("VG geometry exceeds GPU draw limit");
    }
    vao_.bind();
    buffer_.data(static_cast<GLsizeiptr>(vertices.size_bytes()), vertices.data(), GL_STREAM_DRAW);
    glDrawArrays(mode, 0, static_cast<GLsizei>(vertices.size()));
}

void PaintPass::quad(const Rectf& rect, const Rectf& uv, const Eigen::Affine2f& transform) {
    const Eigen::Vector2f a = transform*rect.topLeft(), b = transform*rect.topRight();
    const Eigen::Vector2f c = transform*rect.bottomRight(), d = transform*rect.bottomLeft();
    if (!a.allFinite() || !b.allFinite() || !c.allFinite() || !d.allFinite()) {
        throw std::invalid_argument("VG rectangle transform overflow");
    }
    const Vertex vertices[] = {{a.x(),a.y(),uv.left,uv.top,1}, {b.x(),b.y(),uv.right(),uv.top,1},
        {c.x(),c.y(),uv.right(),uv.bottom(),1}, {a.x(),a.y(),uv.left,uv.top,1},
        {c.x(),c.y(),uv.right(),uv.bottom(),1}, {d.x(),d.y(),uv.left,uv.bottom(),1}};
    if (glyph_texture_ != 0) {
        glyph_vertices_.insert(glyph_vertices_.end(), std::begin(vertices), std::end(vertices));
    } else {
        draw(vertices);
    }
}

void PaintPass::flushGlyphs() {
    if (!glyph_vertices_.empty()) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,glyph_texture_);
        draw(glyph_vertices_);
        glyph_vertices_.clear();
    }
}

void PaintPass::paint(const World::Canvas::State& state, int textureMode) {
    program_.uniform4f(inner_,state.inner.red(),state.inner.green(),state.inner.blue(),state.inner.alpha());
    program_.uniform4f(outer_,state.outer.red(),state.outer.green(),state.outer.blue(),state.outer.alpha());
    program_.uniform(gradient_,state.gradient);
    program_.uniform2f(start_,state.start.x(),state.start.y());
    program_.uniform2f(end_,state.end.x(),state.end.y());
    program_.uniform2f(radii_,state.innerRadius,state.outerRadius);
    const Eigen::Matrix3f inverse = state.transform.inverse().matrix();
    program_.uniformMatrix3fv(inverse_,inverse.data());
    program_.uniform(opacity_,state.opacity);
    program_.uniform(mode_,textureMode);
    if (state.clip) {
        const auto& clip = *state.clip;
        const float left = std::clamp(clip.left,0.0f,static_cast<float>(width_));
        const float right = std::clamp(clip.right(),0.0f,static_cast<float>(width_));
        const float top = std::clamp(clip.top,0.0f,static_cast<float>(height_));
        const float bottom = std::clamp(clip.bottom(),0.0f,static_cast<float>(height_));
        glEnable(GL_SCISSOR_TEST);
        glScissor(static_cast<int>(left),height_-static_cast<int>(std::ceil(bottom)),
            std::max(0,static_cast<int>(std::ceil(right))-static_cast<int>(left)),
            std::max(0,static_cast<int>(std::ceil(bottom))-static_cast<int>(top)));
    } else {
        glDisable(GL_SCISSOR_TEST);
    }
}

void PaintPass::shape(const World::Canvas::Command& command, bool stroke) {
    auto contours = flatten(command);
    if (stroke) {
        const Eigen::Affine2f inverse = command.state.transform.inverse();
        for (auto& contour : contours) {
            for (auto& point : contour.points) {
                point = inverse*point;
            }
        }
    }
    auto outputPoint = [&](const Eigen::Vector2f& point) -> Eigen::Vector2f {
        return stroke ? command.state.transform*point : point;
    };
    std::vector<Vertex> triangles;
    std::vector<Vertex> fringe;
    Eigen::Vector2f low = Eigen::Vector2f::Constant(std::numeric_limits<float>::max());
    Eigen::Vector2f high = -low;
    auto triangle = [&](const Eigen::Vector2f& a,const Eigen::Vector2f& b,const Eigen::Vector2f& c) {
        const auto p=outputPoint(a),q=outputPoint(b),r=outputPoint(c);
        triangles.insert(triangles.end(),{{p.x(),p.y(),0,0,1},{q.x(),q.y(),0,0,1},{r.x(),r.y(),0,0,1}});
    };
    auto edge = [&](const Eigen::Vector2f& first,const Eigen::Vector2f& second) {
        const auto a=outputPoint(first),b=outputPoint(second);
        const Eigen::Vector2f delta = b-a;
        if (delta.squaredNorm() < 1e-10f) {
            return;
        }
        const Eigen::Vector2f n = Eigen::Vector2f(-delta.y(),delta.x()).normalized();
        for (float sign : {-1.0f,1.0f}) {
            const Eigen::Vector2f c = b+n*sign, d = a+n*sign;
            fringe.insert(fringe.end(),{{a.x(),a.y(),0,0,1},{b.x(),b.y(),0,0,1},{c.x(),c.y(),0,0,0},
                {a.x(),a.y(),0,0,1},{c.x(),c.y(),0,0,0},{d.x(),d.y(),0,0,0}});
        }
    };
    const float scale = command.state.transform.linear().norm();
    const float half = command.state.strokeWidth*0.5f;
    if (!std::isfinite(half*scale)) {
        throw std::invalid_argument("VG stroke transform overflow");
    }
    auto disc = [&](const Eigen::Vector2f& center,float radius) {
        const int count = std::clamp(static_cast<int>(std::ceil(std::clamp(std::numbers::pi_v<float>*std::sqrt(radius*scale/0.25f),12.0f,2048.0f))),12,2048);
        for (int i=0;i<count;++i) {
            const float a = i*2*std::numbers::pi_v<float>/count;
            const float b = (i+1)*2*std::numbers::pi_v<float>/count;
            const Eigen::Vector2f p = center+Eigen::Vector2f(std::cos(a),std::sin(a))*radius;
            const Eigen::Vector2f q = center+Eigen::Vector2f(std::cos(b),std::sin(b))*radius;
            triangle(center,p,q);
            edge(p,q);
        }
    };
    for (const auto& contour : contours) {
        const auto& points = contour.points;
        if (points.size() < 2) {
            continue;
        }
        for (const auto& p : points) {
            low = low.cwiseMin(outputPoint(p));
            high = high.cwiseMax(outputPoint(p));
        }
        if (!stroke) {
            for (std::size_t i=1;i+1<points.size();++i) {
                triangle(points[0],points[i],points[i+1]);
            }
            for (std::size_t i=0;i<points.size();++i) {
                edge(points[i],points[(i+1)%points.size()]);
            }
            continue;
        }
        const auto count = contour.closed ? points.size() : points.size()-1;
        for (std::size_t i=0;i<count;++i) {
            Eigen::Vector2f a=points[i], b=points[(i+1)%points.size()];
            const Eigen::Vector2f direction=(b-a).normalized();
            const Eigen::Vector2f n(-direction.y()*half,direction.x()*half);
            if (!contour.closed && command.state.cap==World::Canvas::LineCap::Square) {
                if (i==0) {
                    a-=direction*half;
                }
                if (i+1==count) {
                    b+=direction*half;
                }
            }
            triangle(a+n,b+n,b-n);
            triangle(a+n,b-n,a-n);
            edge(a+n,b+n);
            edge(a-n,b-n);
            if (!contour.closed && i==0) {
                edge(a-n,a+n);
            }
            if (!contour.closed && i+1==count) {
                edge(b-n,b+n);
            }
        }
        for (std::size_t i=0;i<points.size();++i) {
            const bool endpoint = !contour.closed && (i==0 || i+1==points.size());
            if (endpoint) {
                if (command.state.cap==World::Canvas::LineCap::Round) {
                    disc(points[i],half);
                }
                continue;
            }
            if (command.state.join==World::Canvas::LineJoin::Round) {
                disc(points[i],half);
                continue;
            }
            const Eigen::Vector2f before=(points[i]-points[(i+points.size()-1)%points.size()]).normalized();
            const Eigen::Vector2f after=(points[(i+1)%points.size()]-points[i]).normalized();
            for (float sign : {-1.0f,1.0f}) {
                const Eigen::Vector2f n1=Eigen::Vector2f(-before.y(),before.x())*sign;
                const Eigen::Vector2f n2=Eigen::Vector2f(-after.y(),after.x())*sign;
                const Eigen::Vector2f a=points[i]+n1*half,b=points[i]+n2*half;
                const Eigen::Vector2f sum=n1+n2;
                const float denominator=sum.dot(n1);
                if (command.state.join == World::Canvas::LineJoin::Miter && denominator > 1e-6f &&
                    (sum / denominator).norm() <= command.state.miterLimit) {
                    const Eigen::Vector2f tip = points[i] + sum * (half / denominator);
                    triangle(a, tip, b);
                    triangle(a, b, points[i]);
                    edge(a, tip);
                    edge(tip, b);
                } else {
                    triangle(a, b, points[i]);
                    edge(a, b);
                }
            }
        }
    }
    if (triangles.empty()) {
        return;
    }
    const float expansion = stroke ? half * command.state.miterLimit * scale + 2 : 2;
    low.array() -= expansion;
    high.array() += expansion;
    glEnable(GL_STENCIL_TEST);
    glStencilMask(0xff);
    glClearStencil(0);
    glClear(GL_STENCIL_BUFFER_BIT);
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glStencilFunc(GL_ALWAYS, 0, 0xff);
    if (stroke) {
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        glStencilFunc(GL_ALWAYS, 1, 0xff);
    } else if (command.rule == World::Canvas::FillRule::EvenOdd) {
        glStencilOp(GL_KEEP,GL_KEEP,GL_INVERT);
    } else {
        glStencilOpSeparate(GL_FRONT,GL_KEEP,GL_KEEP,GL_INCR_WRAP);
        glStencilOpSeparate(GL_BACK,GL_KEEP,GL_KEEP,GL_DECR_WRAP);
    }
    draw(triangles);
    glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
    glStencilMask(0);
    glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);
    glStencilFunc(GL_NOTEQUAL,0,0xff);
    quad({low.x(),low.y(),high.x()-low.x(),high.y()-low.y()},{},Eigen::Affine2f::Identity());
    glStencilFunc(GL_EQUAL,0,0xff);
    draw(fringe);
    glStencilMask(0xff);
    glDisable(GL_STENCIL_TEST);
}

const Font* PaintPass::resolveFont(const World::Canvas::Command& command) const {
    if (font_resolver_) {
        return font_resolver_(command.font);
    }
    return nullptr;
}

const Texture* PaintPass::resolveTexture(const World::Canvas::Command& command) const {
    if (command.textureRef) {
        return static_cast<const Texture*>(command.textureRef.get());
    }
    if (texture_resolver_) {
        return texture_resolver_(command);
    }
    return nullptr;
}

bool PaintPass::beginTarget(Target& target, bool clear) {
    width_ = target.width();
    height_ = target.height();
    if (width_ <= 0 || height_ <= 0) {
        return false;
    }
    target.bind();
    glViewport(0, 0, width_, height_);
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
    glDisable(GL_CULL_FACE);
    glDisable(GL_SCISSOR_TEST);
    glDisable(GL_STENCIL_TEST);
    glDisable(GL_FRAMEBUFFER_SRGB);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    glActiveTexture(GL_TEXTURE0);
    glBindSampler(0, 0);
    if (clear) {
        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);
    }
    program_.use();
    program_.uniform2f(viewport_, static_cast<float>(width_), static_cast<float>(height_));
    program_.uniform("uImage", 0);
    return true;
}

void PaintPass::endTarget() noexcept {
    glDisable(GL_SCISSOR_TEST);
    glDisable(GL_STENCIL_TEST);
    glDepthMask(GL_TRUE);
    gl::VertexArray::unbind();
    gl::Program::unuse();
}

VOLCANO_GRAPHICS_END
