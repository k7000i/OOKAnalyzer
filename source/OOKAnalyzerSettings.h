#ifndef OOK_ANALYZER_SETTINGS
#define OOK_ANALYZER_SETTINGS

#include <AnalyzerSettings.h>
#include <AnalyzerTypes.h>

class OOKAnalyzerSettings : public AnalyzerSettings
{
public:
	OOKAnalyzerSettings();
	virtual ~OOKAnalyzerSettings();

	virtual bool SetSettingsFromInterfaces();
	void UpdateInterfacesFromSettings();
	virtual void LoadSettings( const char* settings );
	virtual const char* SaveSettings();

	
	Channel mInputChannel;

protected:
	std::auto_ptr< AnalyzerSettingInterfaceChannel >	mInputChannelInterface;
};

#endif //OOK_ANALYZER_SETTINGS
