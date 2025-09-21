#include "communitypackwidget.h"
#include "../core/pack.h"
#include "../packmanager/packmanager.h"
#include "../uilib/hbox.h"
#include "../uilib/vbox.h"
#include "../uilib/scrollvbox.h"
#include "../uilib/label.h"
#include "../uilib/image.h"
#include "defaults.h" // DEFAULT_FONT_*

namespace Ui {

CommunityPackWidget::CommunityPackWidget(int x, int y, int w, int h, FontStore *fontStore, const PackManager::PackMap& communityPacks)
        : SimpleContainer(x,y,w,h),
          _fontStore(fontStore),
          _communityPacks(communityPacks)
{
    _font = _fontStore->getFont(DEFAULT_FONT_NAME, DEFAULT_FONT_SIZE);
    _smallFont = _fontStore->getFont(DEFAULT_FONT_NAME, DEFAULT_FONT_SIZE - 2);
    if (_font && !_smallFont) _smallFont = _font;

    onClick += {this, [this](void*, int, int, int button) {
        if (button == MouseButton::BUTTON_RIGHT) {
            setVisible(false); // TODO: fire onAbort ?
        }
    }};

    auto packs = new ScrollVBox(0,0,0,0);
    packs->setGrow(1,1);
    packs->setPadding(0);
    packs->setSpacing(1);
    _packs = packs;

    auto topBox = new VBox(0,0,0,0);
    topBox->setGrow(1,0);
    topBox->setPadding(2);
    topBox->setSpacing(1);
    topBox->setMinSize({0,32});
    topBox->setBackground(TOP_BG_DEFAULT);
    _topBox = topBox;

    auto buttonContainer = new HBox(0, 0, 0, 0);
    buttonContainer->setGrow(1, 0);
    buttonContainer->setSpacing(2);
    buttonContainer->setPadding(0);

    auto loadPacksLbl = new Label(0, 0, 0, 0, _font, " Load Packs ");
    loadPacksLbl->setGrow(0, 0);
    loadPacksLbl->setTextAlignment(Label::HAlign::CENTER, Label::VAlign::MIDDLE);
    loadPacksLbl->setMinSize({120, loadPacksLbl->getAutoHeight()});
    loadPacksLbl->setSize({120, 28});
    loadPacksLbl->setBackground(BUTTON_BG_DEFAULT);
    loadPacksLbl->onMouseEnter += {this, [loadPacksLbl](void*, int, int, unsigned) {
        loadPacksLbl->setBackground(BUTTON_BG_HOVER);
    }};
    loadPacksLbl->onMouseLeave += {this, [loadPacksLbl](void*) {
        loadPacksLbl->setBackground(BUTTON_BG_DEFAULT);
    }};
    loadPacksLbl->onClick += {this, [this](void*, int, int, int button) {
        if (button == MouseButton::BUTTON_LEFT) {
            onSwitchToLoadPacks.emit(this);
        }
    }};
    buttonContainer->addChild(loadPacksLbl);

    _topBox->addChild(buttonContainer);

    auto hbox = new HBox(0,0,0,0);
    hbox->setGrow(1,1);
    hbox->addChild(packs);
    hbox->setSpacing(1);
    hbox->setPadding(2);

    auto mainVBox = new VBox(0,0,0,0);
    mainVBox->setGrow(1,1);
    mainVBox->addChild(topBox);
    mainVBox->addChild(hbox);
    mainVBox->setSpacing(1);
    mainVBox->setPadding(0);
    addChild(mainVBox);
    _main = mainVBox;

    addPacks();

    if (_communityPacks.empty()) {
        auto lbl = new Label(0, 0, 0, 0, _font, "No Community Packs available");
        lbl->setGrow(1,1);
        lbl->setTextAlignment(Label::HAlign::LEFT, Label::VAlign::MIDDLE);
        lbl->setMinSize(lbl->getAutoSize());
        lbl->setSize({_size.width,32});
        _packs->addChild(lbl);
        auto spacer = new Label(0, 0, 0, 0, nullptr, "");
        spacer->setGrow(1,1);
        _packs->addChild(spacer);
    }
}

void CommunityPackWidget::update()
{
    _curPackHover = nullptr;
    _curPackLabel = nullptr;
    _disableHoverSelect = false;

    _packs->clearChildren();
    addPacks();

    _main->setSize(_size);
    _main->relayout();
}

void CommunityPackWidget::addPacks()
{
    for (const auto& pair : _communityPacks) {
        const PackManager::PackInfo& pack = pair.second;

        auto row = new HBox(0, 0, 0, 32);
        row->setSpacing(5);
        row->setPadding(0);
        row->setGrow(1, 0);

        if (pack.icon_data && !pack.icon_data->empty()) {
            auto& icon_data = *pack.icon_data;
            auto icon = new Image(0, 0, 32, 32, icon_data.data(), icon_data.size());
            row->addChild(icon);
        }

        auto lbl = new Label(0, 0, 0, 32, _font, " " + pack.name);
        lbl->setGrow(1, 0);
        lbl->setTextAlignment(Label::HAlign::LEFT, Label::VAlign::MIDDLE);
        lbl->setBackground(PACK_BG_DEFAULT);
        row->addChild(lbl);

        _packs->addChild(row);

        lbl->onMouseEnter += {this, [this](void* s, int, int, unsigned) {
            if (_curPackHover != s) {
                if (_curPackHover == _curPackLabel && _disableHoverSelect)
                    _curPackHover->setBackground(PACK_BG_ACTIVE);
                else if (_curPackHover)
                    _curPackHover->setBackground(PACK_BG_DEFAULT);

                _curPackHover = (Label*)s;
                if (_curPackHover == _curPackLabel && _disableHoverSelect)
                    _curPackHover->setBackground(PACK_BG_ACTIVE_HOVER);
                else
                    _curPackHover->setBackground(PACK_BG_HOVER);
            }
            if (_disableHoverSelect || _curPackLabel == s)
                return;

            if (_curPackLabel)
                _curPackLabel->setBackground(PACK_BG_DEFAULT);

            _curPackLabel = (Label*)s;
        }};

        lbl->onClick += {this, [this, pack](void* s, int x, int y, int button) {
            _disableHoverSelect = false;
            ((Label*)s)->onMouseEnter.emit(s, x, y, (unsigned)button);
            _disableHoverSelect = true;
            if (_curPackLabel)
                _curPackLabel->setBackground(PACK_BG_ACTIVE_HOVER);
            if (button == MouseButton::BUTTON_LEFT) {
                onDownloadPack.emit(this, pack.versions_url);
            } else if (button == MouseButton::BUTTON_MIDDLE && !pack.homepage.empty()) {
                onOpenUrl.emit(this, pack.homepage);
            }
        }};
    }
}

void CommunityPackWidget::setSize(Size size)
{
    SimpleContainer::setSize(size);
    // TODO: have more intelligent hbox instead
    _topBox->setWidth(size.width);
    _packs->setWidth(size.width);
    _main->relayout();
}


} // namespace
