#ifndef _UI_COMMUNITYPACKWIDGET_H
#define _UI_COMMUNITYPACKWIDGET_H

#include "../uilib/container.h"
#include "../uilib/simplecontainer.h"
#include "../uilib/hbox.h"
#include "../uilib/vbox.h"
#include "../uilib/scrollvbox.h"
#include "../uilib/label.h"
#include "../uilib/fontstore.h"
#include "../core/signal.h"
#include "../packmanager/packmanager.h"
#include <SDL2/SDL_ttf.h>

#include "../core/fs.h"

namespace Ui {

class CommunityPackWidget : public SimpleContainer {
public:
    using FONT = FontStore::FONT;
    CommunityPackWidget(int x, int y, int w, int h, FontStore *fontStore, const PackManager::PackMap& communityPacks);

    void update();

    Signal<const fs::path&, const std::string&> onPackSelected;
    Signal<> onSwitchToLoadPacks;
    Signal<const std::string&> onDownloadPack;

    virtual void setSize(Size size) override; // TODO: have more intelligent hbox instead

protected:
    FontStore *_fontStore;
    FONT _font;
    FONT _smallFont;

    ScrollVBox *_packs;
    VBox *_topBox;
    VBox *_main;
    Label *_curPackLabel = nullptr;
    Label *_curVariantLabel = nullptr;
    Label *_curPackHover = nullptr;
    bool _disableHoverSelect = false;
    bool _packIconsAdded = false;

    const PackManager::PackMap& _communityPacks;

    static constexpr Color PACK_BG_DEFAULT = {32, 32, 32};
    static constexpr Color PACK_BG_ACTIVE = {32, 128, 32};
    static constexpr Color PACK_BG_HOVER = {64, 64, 64};
    static constexpr Color PACK_BG_ACTIVE_HOVER = {64, 160, 64};
    static constexpr Color VARIANT_BG_DEFAULT = {64, 64, 64};
    static constexpr Color VARIANT_BG_HOVER = {96, 96, 96};
    static constexpr Color TOP_BG_DEFAULT = {48, 48, 48};
    static constexpr Color BUTTON_BG_DEFAULT = {48, 96, 160};
    static constexpr Color BUTTON_BG_HOVER = {64, 128, 192};
    static constexpr Color BUTTON_BG_ACTIVE = {32, 64, 128};

    void addPacks();
};

} // namespace Ui

#endif /* _UI_COMMUNITYPACKWIDGET_H */

