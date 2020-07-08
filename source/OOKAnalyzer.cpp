#include "OOKAnalyzer.h"
#include "OOKAnalyzerSettings.h"
#include <AnalyzerChannelData.h>

OOKAnalyzer::OOKAnalyzer()
:	Analyzer2(),  
	mSettings( new OOKAnalyzerSettings() ),
	mSimulationInitilized( false )
{
	SetAnalyzerSettings( mSettings.get() );
}

OOKAnalyzer::~OOKAnalyzer()
{
	KillThread();
}

void OOKAnalyzer::SetupResults()
{
	mResults.reset( new OOKAnalyzerResults( this, mSettings.get() ) );
	SetAnalyzerResults( mResults.get() );
	mResults->AddChannelBubblesWillAppearOn( mSettings->mInputChannel );
}

void OOKAnalyzer::SyncFrame() {
    while( mSynchronized == false) {
        mSampleRateHz = GetSampleRate();



        mSerial->AdvanceToNextEdge();
        U64 first_edge_sample = mSerial->GetSampleNumber();
        mSerial->AdvanceToNextEdge();
        U64 second_edge_sample = mSerial->GetSampleNumber();

        U64 edge_distance = second_edge_sample - first_edge_sample;

        double edge_time = double(edge_distance) / mSampleRateHz;

        if (edge_time > 0.004600 && edge_time < 0.00499) {

            mResults->AddMarker(second_edge_sample - (edge_distance / 2), AnalyzerResults::ErrorSquare, mSettings->mInputChannel );
            //mResults->CommitResults();

            mSerial->AdvanceToNextEdge();
            U64 third_edge_sample = mSerial->GetSampleNumber();
            double second_edge_time = double(mSerial->GetSampleNumber() - second_edge_sample) / mSampleRateHz;

            if (second_edge_time > 0.00147 && second_edge_time < 0.00151) {
                mResults->AddMarker(third_edge_sample - ((third_edge_sample - second_edge_sample) / 2), AnalyzerResults::ErrorSquare, mSettings->mInputChannel );
                //mResults->CommitResults();
                printf("Synced\n");
                mSynchronized = true;
            }
        }
    }
}

void OOKAnalyzer::WorkerThread()
{
    mSerial = GetAnalyzerChannelData( mSettings->mInputChannel );

    for (;;) {
        mSynchronized = false;
        SyncFrame();
        U8 data = 0;
        U8 mask = 1 << 7;
        S64 byte_start_pos = mSerial->GetSampleNumber();

        for( ; ; )
        {
            // data
            U64 bit_start_edge = mSerial->GetSampleNumber();

            U64 bit_middle_edge = mSerial->GetSampleOfNextEdge();

            if ((bit_middle_edge - bit_start_edge) > 30000) {
                break;

            }
            mSerial->AdvanceToNextEdge();
            bit_middle_edge = mSerial->GetSampleNumber();


            //  check end stream
            U64 bit_end_edge;
            if (!mSerial->DoMoreTransitionsExistInCurrentData()) {
                bit_end_edge = 0;
            } else {
                bit_end_edge = mSerial->GetSampleOfNextEdge();
            }


            double bit_time = double(bit_end_edge - bit_start_edge) / mSampleRateHz;


            if (bit_time < 0.00106 || bit_time > 0.00109) {
                mSerial->Advance(10820);

            } else {
                mSerial->AdvanceToNextEdge();
            }

            bit_end_edge = mSerial->GetSampleNumber();

            // end data

            U64 middle_point = (bit_end_edge - bit_start_edge) /2;


            U64 edge_pos = bit_middle_edge - bit_start_edge;

            mResults->AddMarker(bit_end_edge - ((bit_end_edge- bit_start_edge) /2) , AnalyzerResults::Dot, mSettings->mInputChannel );
            if (edge_pos > middle_point) {
                data |= mask; // bit 1
            }
            mask = mask >> 1;

            if (mSerial->GetSampleNumber() >= 83323355 && mask == 1) {
                printf("");
            }
            if (mask == 0) {
                //we have a byte to save.
                Frame frame;
                frame.mData1 = data;
                frame.mFlags = 0;
                frame.mStartingSampleInclusive = byte_start_pos + 1;
                frame.mEndingSampleInclusive = mSerial->GetSampleNumber();
                printf("%d Starting %d ending\n", frame.mStartingSampleInclusive, frame.mEndingSampleInclusive);

                mResults->AddFrame( frame );
                mResults->CommitResults();
                ReportProgress( mSerial->GetSampleNumber() );

                data = 0;
                mask = 1 << 7;
                byte_start_pos = mSerial->GetSampleNumber();


            }


        }
        CheckIfThreadShouldExit();
    }
}

bool OOKAnalyzer::NeedsRerun()
{
	return false;
}

U32 OOKAnalyzer::GenerateSimulationData( U64 minimum_sample_index, U32 device_sample_rate, SimulationChannelDescriptor** simulation_channels )
{
	if( mSimulationInitilized == false )
	{
		mSimulationDataGenerator.Initialize( GetSimulationSampleRate(), mSettings.get() );
		mSimulationInitilized = true;
	}

	return mSimulationDataGenerator.GenerateSimulationData( minimum_sample_index, device_sample_rate, simulation_channels );
}

U32 OOKAnalyzer::GetMinimumSampleRateHz()
{
	return 25000;
}

const char* OOKAnalyzer::GetAnalyzerName() const
{
	return "On-Off Keying";
}

const char* GetAnalyzerName()
{
	return "On-Off Keying";
}

Analyzer* CreateAnalyzer()
{
	return new OOKAnalyzer();
}

void DestroyAnalyzer( Analyzer* analyzer )
{
	delete analyzer;
}