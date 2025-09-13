#ifndef _UI_DEFAULTTRACKERWINDOW_H
#define _UI_DEFAULTTRACKERWINDOW_H

#include "trackerwindow.h"
#include "loadpackwidget.h"
#include "communitypackwidget.h"
#include "../packmanager/packmanager.h"
#include "../uilib/progressbar.h"
#include <vector>

namespace Ui {

class DefaultTrackerWindow : public TrackerWindow {
public:
    using FONT = Window::FONT;

    enum class View {
        LoadPackWidget,
        CommunityPackWidget
    };
    
    DefaultTrackerWindow(const char *title, SDL_Surface* icon=nullptr, const Position& pos=WINDOW_DEFAULT_POSITION, const Size& size={0,0}, const PackManager::PackMap* communityPacks=nullptr);
    virtual ~DefaultTrackerWindow();

    virtual void render(Renderer renderer, int offX, int offY) override;
    virtual void setTracker(Tracker* tracker) override;
    virtual void setAutoTrackerState(int index, AutoTracker::State state, const std::string& name, const std::string& subname) override;
    virtual void setSize(Size size) override;
    void setMinSize(Size size) override;
    virtual void showView(View view);
    virtual void hideView(View view);
    virtual void showProgress(const std::string& title, int progress, int max);
    virtual void hideProgress();
    
    Signal<const fs::path&, const std::string&> onPackSelected;
    Signal<const std::string&> onDownloadPack;
    
protected:
    ImageButton *_btnLoad = nullptr;
    ImageButton *_btnReload = nullptr;
    ImageButton *_btnImport = nullptr;
    ImageButton *_btnExport = nullptr;
    ImageButton *_btnBroadcast = nullptr;
    ImageButton *_btnPackSettings = nullptr;
    HBox *_hboxAutoTrackers = nullptr;
    std::vector<Label*> _lblsAutoTrackers;
    Label *_lblTooltip = nullptr;
    Label *_lblMessage = nullptr;
    LoadPackWidget *_loadPackWidget = nullptr;
    CommunityPackWidget *_communityPackWidget = nullptr;
    VBox *_vboxProgress = nullptr;
    HBox *_hboxProgressTexts = nullptr;
    Label *_lblProgressTitle = nullptr;
    Label *_lblProgressPercent = nullptr;
    Label *_lblProgressValues = nullptr;
    ProgressBar *_pgbProgress = nullptr;
    std::vector<AutoTracker::State> _autoTrackerStates;
    std::vector<std::string> _autoTrackerNames;
    std::vector<std::string> _autoTrackerSubNames;
    float _aspectRatio = 1;
    std::string _lastHoverItem;

    virtual void setTracker(Tracker *tracker, const std::string& layout) override;

private:
    void showMessage(const std::string& message, bool error=false);
    void hideMessage();
};

} // namespace Ui

#endif /* _UI_DEFAULTTRACKERWINDOW_H */

