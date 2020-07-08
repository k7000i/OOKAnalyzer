#include "OOKAnalyzerSettings.h"
#include <AnalyzerHelpers.h>


OOKAnalyzerSettings::OOKAnalyzerSettings()
:	mInputChannel( UNDEFINED_CHANNEL )
{
	mInputChannelInterface.reset( new AnalyzerSettingInterfaceChannel() );
	mInputChannelInterface->SetTitleAndTooltip( "Serial", "Standard On-Off Keying encoder" );
	mInputChannelInterface->SetChannel( mInputChannel );



	AddInterface( mInputChannelInterface.get() );

	AddExportOption( 0, "Export as text/csv file" );
	AddExportExtension( 0, "text", "txt" );
	AddExportExtension( 0, "csv", "csv" );

	ClearChannels();
	AddChannel( mInputChannel, "Serial", false );
}

OOKAnalyzerSettings::~OOKAnalyzerSettings()
{
}

bool OOKAnalyzerSettings::SetSettingsFromInterfaces()
{
	mInputChannel = mInputChannelInterface->GetChannel();

	ClearChannels();
	AddChannel( mInputChannel, "Serial", true );

	return true;
}

void OOKAnalyzerSettings::UpdateInterfacesFromSettings()
{
	mInputChannelInterface->SetChannel( mInputChannel );
}

void OOKAnalyzerSettings::LoadSettings( const char* settings )
{
	SimpleArchive text_archive;
	text_archive.SetString( settings );

	text_archive >> mInputChannel;

	ClearChannels();
	AddChannel( mInputChannel, "Serial", true );

	UpdateInterfacesFromSettings();
}

const char* OOKAnalyzerSettings::SaveSettings()
{
	SimpleArchive text_archive;

	text_archive << mInputChannel;

	return SetReturnString( text_archive.GetString() );
}
