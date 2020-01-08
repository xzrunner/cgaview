#pragma once

#include "cgaview/Scene.h"

#include <ee0/Observer.h>
#include <ee0/SubjectMgr.h>
#include <ee0/typedef.h>

#include <cga/EvalContext.h>

#include <wx/panel.h>

class wxNotebook;

namespace cgav
{

class WxTextPage;
class WxGraphPage;

class WxEditorPanel : public wxPanel, public ee0::Observer
{
public:
    WxEditorPanel(wxWindow* parent, const ee0::SubjectMgrPtr& preview_sub_mgr,
        std::function<WxGraphPage*(wxWindow*, cga::EvalContext&)> graph_page_creator);
    virtual ~WxEditorPanel();

    virtual void OnNotify(uint32_t msg, const ee0::VariantSet& variants) override;

    void SaveRuleToFile(const std::string& filepath);
    void LoadRuleFromFile(const std::string& filepath);

    bool IsCurrGraphPage() const;

    auto& GetSubMgr() { return m_sub_mgr; }

    auto& GetScene() const { return m_scene; }

private:
    void InitLayout(const ee0::SubjectMgrPtr& preview_sub_mgr,
        std::function<WxGraphPage*(wxWindow*, cga::EvalContext&)> graph_page_creator);

    void ShowRule(const std::string& name);

private:
    enum PageIndex
    {
        GRAPH_PAGE_IDX = 0,
        TEXT_PAGE_IDX,
    };

private:
    ee0::SubjectMgr m_sub_mgr;

    wxNotebook* m_nb;

    WxGraphPage* m_graph_page;
    WxTextPage*  m_text_page;

    cga::EvalContext m_ctx;

    Scene m_scene;

}; // WxEditorPanel

}