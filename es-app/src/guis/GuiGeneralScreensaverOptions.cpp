#include "guis/GuiGeneralScreensaverOptions.h"

#include "components/OptionListComponent.h"
#include "components/SliderComponent.h"
#include "components/SwitchComponent.h"
#include "guis/GuiMsgBox.h"
#include "guis/GuiSlideshowScreensaverOptions.h"
#include "guis/GuiVideoScreensaverOptions.h"
#include "Settings.h"

GuiGeneralScreensaverOptions::GuiGeneralScreensaverOptions(Window* window, const char* title) : GuiScreensaverOptions(window, title)
{
	// screensaver time
	auto screensaver_time = std::make_shared<SliderComponent>(mWindow, 0.f, 30.f, 1.f, "m");
	screensaver_time->setValue((float)(Settings::getInstance()->getInt("ScreenSaverTime") / (1000 * 60)));
	addWithLabel(_("SCREENSAVER AFTER"), screensaver_time);
	addSaveFunc([screensaver_time] {
	    Settings::getInstance()->setInt("ScreenSaverTime", (int)Math::round(screensaver_time->getValue()) * (1000 * 60));
	    PowerSaver::updateTimeouts();
	});
	
	// Allow ScreenSaver Controls - ScreenSaverControls
	auto ss_controls = std::make_shared<SwitchComponent>(mWindow);
	ss_controls->setState(Settings::getInstance()->getBool("ScreenSaverControls"));
	addWithLabel(_("SCREENSAVER CONTROLS"), ss_controls);
	addSaveFunc([ss_controls] { Settings::getInstance()->setBool("ScreenSaverControls", ss_controls->getState()); });	

	// screensaver behavior
	auto screensaver_behavior = std::make_shared< OptionListComponent<std::string> >(mWindow, _("SCREENSAVER BEHAVIOR"), false);
	std::vector<std::string> screensavers;
	screensavers.push_back(N_("dim"));
	screensavers.push_back(N_("black"));
	screensavers.push_back(N_("random video"));
	screensavers.push_back(N_("slideshow"));
	for(auto it = screensavers.cbegin(); it != screensavers.cend(); it++)
		screensaver_behavior->add(_(it->c_str()), *it, Settings::getInstance()->getString("ScreenSaverBehavior") == *it);
	addWithLabel(_("SCREENSAVER BEHAVIOR"), screensaver_behavior);
	addSaveFunc([this, screensaver_behavior] {
		if (Settings::getInstance()->getString("ScreenSaverBehavior") != "random video" && screensaver_behavior->getSelected() == "random video") {
			// if before it wasn't risky but now there's a risk of problems, show warning
			mWindow->pushGui(new GuiMsgBox(mWindow,
			_("The \"Random Video\" screensaver shows videos from your gamelist.\n\nIf you do not have videos, or if in several consecutive attempts the games it selects don't have videos it will default to black.\n\nMore options in the \"UI Settings\" > \"Video Screensaver\" menu."),
				_("OK"), [] { return; }));
		}
		Settings::getInstance()->setString("ScreenSaverBehavior", screensaver_behavior->getSelected());
		PowerSaver::updateTimeouts();
	});

        auto ss_hwbacklight = std::make_shared<SwitchComponent>(mWindow);
        ss_hwbacklight->setState(Settings::getInstance()->getBool("ScreenSaverHardwareBacklight"));
        addWithLabel(_("USE HARDWARE BACKLIGHT CONTROL"), ss_hwbacklight);
        addSaveFunc([ss_hwbacklight, this] { Settings::getInstance()->setBool("ScreenSaverHardwareBacklight", ss_hwbacklight->getState()); });

	ComponentListRow row;

	// show filtered menu
	row.elements.clear();
	row.addElement(std::make_shared<TextComponent>(mWindow, _("VIDEO SCREENSAVER SETTINGS"), Font::get(FONT_SIZE_MEDIUM), 0x777777FF), true);
	row.addElement(makeArrow(mWindow), false);
	row.makeAcceptInputHandler(std::bind(&GuiGeneralScreensaverOptions::openVideoScreensaverOptions, this));
	addRow(row);

	row.elements.clear();
	row.addElement(std::make_shared<TextComponent>(mWindow, _("SLIDESHOW SCREENSAVER SETTINGS"), Font::get(FONT_SIZE_MEDIUM), 0x777777FF), true);
	row.addElement(makeArrow(mWindow), false);
	row.makeAcceptInputHandler(std::bind(&GuiGeneralScreensaverOptions::openSlideshowScreensaverOptions, this));
	addRow(row);
}

GuiGeneralScreensaverOptions::~GuiGeneralScreensaverOptions()
{
}

void GuiGeneralScreensaverOptions::openVideoScreensaverOptions() {
  mWindow->pushGui(new GuiVideoScreensaverOptions(mWindow, _("VIDEO SCREENSAVER").c_str()));
}

void GuiGeneralScreensaverOptions::openSlideshowScreensaverOptions() {
  mWindow->pushGui(new GuiSlideshowScreensaverOptions(mWindow, _("SLIDESHOW SCREENSAVER").c_str()));
}

