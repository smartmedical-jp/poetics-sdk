- [API Documentation](#api-documentation)
- [StreamingClient](#streamingclient)
- [StreamAsrJob](#streamasrjob)
  - [Checking properties](#checking-properties)
  - [Setting up a listener](#setting-up-a-listener)
  - [Communcation with the server](#communcation-with-the-server)
- [StreamAsrJobListener](#streamasrjoblistener)

# API Documentation

The classes and functions defined here are external APIs that can be accessed by clients using the SDK.

# StreamingClient

- **StreamingClient**(string endpoint, string apiKey)
- std::unique_ptr `<StreamAsrJob>` **createStreamAsrJob**(
  string audioEncoding, int audioSampleRate, int channelCount,
  bool enableDatalogging, string conversationTitle, vector `<string>`& channels)
- std::unique_ptr `<StreamAsrJob>` **connectToStreamAsrJob**(
  string streamAsrJobID, int channelCount)
  - Use this if you already have a streamAsrJobID and want to connect to it.
  - Not implemented at this time, only the method is defined.

# StreamAsrJob

## Checking properties

- string **getStreamAsrJobId**()
- int **getChannelCount**()
- string **getAudioEncoding**()
- int **getAudioSampleRate**()
- bool **getEnableDataLogging**()
- string **getConversationTitle**()
- vector `<string>`* **getChannels**()

## Setting up a listener

- void **setListener**(shared_ptr `<StreamAsrJobListener>` listener)

## Communcation with the server

- bool **connect**()
- bool **disconnect**()
- bool **isConnected**()
- bool **enqueueAudioData**(int channelIndex, int audioFragmentIndex, const char* data, int dataSize)
- bool **finishEnqueuingAudiodata**()

# StreamAsrJobListener

- **OnMessageReceived**(string& message)
  - Receive response from server (JSON format)
- **OnErrorReceived**(string& message)
  - Receive errors from the server (in JSON format)
- **OnDebugMessageReceived**(string& message)
  - A listener used internally when developing SDKs and not normally needed to be received on the client side.
  - Like the [example code](../../src/poetics_example/main.cpp), leave function empty as empty body.

<br/>