#pragma once

#include <tessellation/Painter.h>
#include <painting0/RenderContext.h>

namespace n0 { class SceneNode; }
namespace pt3 { class Viewport; }

namespace cev
{

class PreviewRender
{
public:
    PreviewRender(const pt3::Viewport& vp, const sm::mat4& cam_mat);

    void DrawNode3D(const pt0::RenderContext& rc, const n0::SceneNode& node,
        bool draw_face, bool draw_shape) const;
//    void DrawNode2D(const ce::Operation& back, const bp::Node& front) const;

    auto& GetPainter() const { return m_pt; }

private:
    const pt3::Viewport& m_vp;
    const sm::mat4&      m_cam_mat;

    mutable tess::Painter m_pt;

}; // PreviewRender

}