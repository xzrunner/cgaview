#include "archlab/WxPreviewCanvas.h"
#include "archlab/PreviewRender.h"
#include "archlab/PreviewPage.h"
#include "archlab/Node.h"
#include "archlab/MessageID.h"
#include "archlab/WxEditorPanel.h"
#include "archlab/WxGraphPage.h"
#include "archlab/WxTextPage.h"

#include <ee0/WxStagePage.h>
#include <ee0/SubjectMgr.h>
#include <blueprint/Node.h>
#include <blueprint/CompNode.h>

#include <node0/SceneNode.h>
#include <unirender/RenderState.h>
#include <painting2/RenderSystem.h>
#include <painting3/MaterialMgr.h>
#include <painting3/Blackboard.h>
#include <painting3/WindowContext.h>
#include <painting3/PerspCam.h>
#include <node3/RenderSystem.h>

namespace
{

const float    NODE_RADIUS = 10;
const uint32_t LIGHT_SELECT_COLOR = 0x88000088;

}

namespace archlab
{

WxPreviewCanvas::WxPreviewCanvas(const ur::Device& dev, ee0::WxStagePage* stage,
                                 ECS_WORLD_PARAM const ee0::RenderContext& rc)
    : ee3::WxStageCanvas(dev, stage, ECS_WORLD_VAR &rc, nullptr, true)
{
    stage->GetSubjectMgr()->RegisterObserver(MSG_SET_EDIT_OP, this);
    stage->GetSubjectMgr()->RegisterObserver(MSG_SET_SELECT_OP, this);
}

WxPreviewCanvas::~WxPreviewCanvas()
{
    m_stage->GetSubjectMgr()->UnregisterObserver(MSG_SET_EDIT_OP, this);
    m_stage->GetSubjectMgr()->UnregisterObserver(MSG_SET_SELECT_OP, this);
}

void WxPreviewCanvas::OnNotify(uint32_t msg, const ee0::VariantSet& variants)
{
    ee3::WxStageCanvas::OnNotify(msg, variants);

    switch (msg)
    {
    case MSG_SET_EDIT_OP:
        m_op_type = OpType::Edit;
        break;
    case MSG_SET_SELECT_OP:
        m_op_type = OpType::Select;
        break;
    }
}

void WxPreviewCanvas::DrawBackground3D() const
{
    ee3::WxStageCanvas::DrawBackgroundGrids(10.0f, 0.2f);
//    ee3::WxStageCanvas::DrawBackgroundCross();
}

void WxPreviewCanvas::DrawForeground3D() const
{
    pt0::RenderContext rc;
    rc.AddVar(
        pt3::MaterialMgr::PositionUniforms::light_pos.name,
        pt0::RenderVariant(sm::vec3(0, 2, -4))
    );
    if (m_camera->TypeID() == pt0::GetCamTypeID<pt3::PerspCam>())
    {
        auto persp = std::static_pointer_cast<pt3::PerspCam>(m_camera);
        rc.AddVar(
            pt3::MaterialMgr::PositionUniforms::cam_pos.name,
            pt0::RenderVariant(persp->GetPos())
        );
    }
    //auto& wc = pt3::Blackboard::Instance()->GetWindowContext();
    //assert(wc);
    //rc.AddVar(
    //    pt3::MaterialMgr::PosTransUniforms::view.name,
    //    pt0::RenderVariant(wc->GetViewMat())
    //);
    //rc.AddVar(
    //    pt3::MaterialMgr::PosTransUniforms::projection.name,
    //    pt0::RenderVariant(wc->GetProjMat())
    //);

    auto cam_mat = m_camera->GetProjectionMat() * m_camera->GetViewMat();
    PreviewRender pr(GetViewport(), cam_mat);

    bool draw_face = true, draw_shape = true;
    switch (m_op_type)
    {
    case OpType::Edit:
        draw_face  = false;
        draw_shape = true;
        break;
    case OpType::Select:
        draw_face  = true;
        draw_shape = true;
        break;
    default:
        assert(0);
    }

    auto& ctx = *GetRenderContext().ur_ctx;

    m_stage->Traverse([&](const ee0::GameObj& obj)->bool {
        pr.DrawNode3D(m_dev, ctx, rc, *obj, draw_face, draw_shape);
        return true;
    });

    if (m_editor_panel)
    {
        auto obj = m_editor_panel->GetCurrPagePreviewObj();
        if (obj) {
            pr.DrawNode3D(m_dev, ctx, rc, *obj, true, false);
        }
    }

    ur::RenderState rs;
    pt2::RenderSystem::DrawPainter(m_dev, ctx, rs, pr.GetPainter());
}

void WxPreviewCanvas::DrawForeground2D() const
{
}

}
