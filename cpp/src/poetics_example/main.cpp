// main.cpp
#include "main.h"

class StreamListener : public poetics::streaming::asr_job::StreamAsrJobListener
{
public:
    void OnMessageReceived(const string& result) override
    {
        cout << "\033[1mOnMessage: " << result << "\033[0m" << endl;
    }

    void OnErrorReceived(const string& error) override
    {
        cout << "\033[1;31mOnError: " << error << "\033[0m" << endl;
    }

    void OnDebugMessageReceived(const string& message) override
    {
        cout << "\033[1;33mOnDebug: " << message.substr(0, 250) << "\033[0m" << endl;
    }
};

int main()
{
    system("chcp 65001");


    const char* apiKey = getenv("POETICS_API_KEY");
    const char* endpoint = getenv("POETICS_STREAMING_API_ENDPOINT");

    if (apiKey == nullptr)
    {
        cerr << "POETICS_API_KEY is not set" << endl;
        return 1;
    }

    if (endpoint == nullptr)
    {
        cerr << "POETICS_STREAMING_API_ENDPOINT is not set" << endl;
        return 1;
    }

    poetics::streaming::StreamingClient streamingClient(endpoint, apiKey);
    vector<const char *> channels = { "channel1" };
    unique_ptr<StreamAsrJob> asrJob = streamingClient.createStreamAsrJob("s16le", 8000, 1, true, "test", channels);
    asrJob->setListener(std::make_shared<StreamListener>());
    if (asrJob->connect())
    {
        cout << "Connected" << endl;

        string filePath = "C:\\REPO\\poetics-sdk\\testdata\\short_s16le_8k.wav";
        // string filePath = "C:\\REPO\\poetics-sdk\\testdata\\long_s16le_8k.wav";
        auto sampleRate = 8000;
        auto bytesPerSample = 2;
        double recordingBufferMilliseconds = 500.0;
        int recordingBufferSize = sampleRate * bytesPerSample * recordingBufferMilliseconds / 1000;
        const int CHANNEL = 0;

        ifstream audioFile(filePath, std::ios::in | std::ios::binary);
        // Skip WAV header
        audioFile.seekg(44, std::ios::beg);
        char* buffer = new char[recordingBufferSize];

        if (audioFile.is_open())
        {
            cout << "Feeding Audio Data" << endl;

            int index = 0;
            while (true)
            {
                // Sleep for recordingBufferMilliseconds 
                std::this_thread::sleep_for(std::chrono::milliseconds((int) recordingBufferMilliseconds));
                // std::this_thread::sleep_for(std::chrono::milliseconds(((int) recordingBufferMilliseconds) - 100));
                // std::this_thread::sleep_for(std::chrono::milliseconds(150));

                audioFile.read(buffer, recordingBufferSize);

                cout << "Current index of the audio data: " << index << endl;
                if (audioFile) {
                    auto result = asrJob->enqueueAudioData(CHANNEL, index, buffer, recordingBufferSize);
                    if (!result) {
                        cout << "Failed to enqueue audio data" << endl;
                        break;
                    }
                }
                else {
                    // End of file
                    auto actualSize = audioFile.gcount();
                    cout << "Read " << actualSize << " bytes, which is the end of the file data" << endl;
                    auto result = asrJob->enqueueAudioData(CHANNEL, index, buffer, actualSize);
                    if (!result) {
                        cout << "Failed to enqueue audio data" << endl;
                    }
                    asrJob->setLastAudioFragmentIndex(CHANNEL, index);
                    break;
                }
                
                index++;
            }
        }
        else {
            cout << "Failed to open audio file" << endl;
        }
        delete[] buffer;
        buffer = nullptr;

        cout << "Closing StreamAsrJob" << endl;
        asrJob->finishEnqueuingAudioData();
        // Wait for 10 seconds before disconnecting
        std::this_thread::sleep_for(std::chrono::seconds(10));
        cout << "Disconnecting" << endl;
        asrJob->disconnect();
    }
    else
    {
        cout << "Failed to connect" << endl;
    }

    return 0;
}
