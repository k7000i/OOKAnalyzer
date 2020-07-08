#ifndef OOK_ANALYZER_H
#define OOK_ANALYZER_H

#include <Analyzer.h>
#include "OOKAnalyzerResults.h"
#include "OOKSimulationDataGenerator.h"

class OOKAnalyzerSettings;
class ANALYZER_EXPORT OOKAnalyzer : public Analyzer2
{
public:
	OOKAnalyzer();
	virtual ~OOKAnalyzer();

	virtual void SetupResults();
	virtual void WorkerThread();
	virtual void SyncFrame();

	virtual U32 GenerateSimulationData( U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channels );
	virtual U32 GetMinimumSampleRateHz();

	virtual const char* GetAnalyzerName() const;
	virtual bool NeedsRerun();

protected: //vars
	std::auto_ptr< OOKAnalyzerSettings > mSettings;
	std::auto_ptr< OOKAnalyzerResults > mResults;
	AnalyzerChannelData* mSerial;

	OOKSimulationDataGenerator mSimulationDataGenerator;
	bool mSimulationInitilized;

	//Serial analysis vars:
	U32 mSampleRateHz;
    bool mSynchronized;
};

extern "C" ANALYZER_EXPORT const char* __cdecl GetAnalyzerName();
extern "C" ANALYZER_EXPORT Analyzer* __cdecl CreateAnalyzer( );
extern "C" ANALYZER_EXPORT void __cdecl DestroyAnalyzer( Analyzer* analyzer );

#endif //OOK_ANALYZER_H
