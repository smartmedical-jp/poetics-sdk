- [APIの仕様](#apiの仕様)
- [StreamingClient](#streamingclient)
- [StreamAsrJob](#streamasrjob)
  - [プロパティの確認](#プロパティの確認)
  - [リスナーの設定](#リスナーの設定)
  - [サーバーとの通信](#サーバーとの通信)
- [StreamAsrJobListener](#streamasrjoblistener)

# APIの仕様

ここで定義したクラスと関数は、SDKを使用するクライアントからアクセス可能な外部APIです。

# StreamingClient

- **StreamingClient**(const char* endpoint, const char* apiKey)
- std::unique_ptr `<StreamAsrJob>` **createStreamAsrJob**(
  const char* audioEncoding, int audioSampleRate, int channelCount,
  bool enableDatalogging, const char* conversationTitle, vector `<const char*>`& channels)
- std::unique_ptr `<StreamAsrJob>` **connectToStreamAsrJob**(
  const char* streamAsrJobID, int channelCount)
  - 既にstreamAsrJobIDを持っていて、それに接続したい場合に使います。
  - 今の時点では未実装で、メソッドだけ定義しています。

# StreamAsrJob

## プロパティの確認

- string **getStreamAsrJobId**()
- int **getChannelCount**()
- string **getAudioEncoding**()
- int **getAudioSampleRate**()
- bool **getEnableDataLogging**()
- string **getConversationTitle**()
- vector `<string>`* **getChannels**()

## リスナーの設定

- void **setListener**(shared_ptr `<StreamAsrJobListener>` listener)

## サーバーとの通信

- bool **connect**()
- bool **disconnect**()
- bool **isConnected**()
- bool **enqueueAudioData**(int channelIndex, int audioFragmentIndex, const char* data, int dataSize)
- bool **finishEnqueuingAudiodata**()

# StreamAsrJobListener

- **OnMessageReceived**(string& message)
  - サーバーからの応答を受信します（JSON形式）
- **OnErrorReceived**(string& message)
  - サーバーからのエラーを受信します（JSON形式）
- **OnDebugMessageReceived**(string& message)
  - SDKを開発する際に内部的に使うリスナーで、通常はクライアント側で受信する必要はありません。
  - [サンプルコード](../../src/poetics_example/main.cpp)のように、関数の中を空白にしてください。

<br/>
