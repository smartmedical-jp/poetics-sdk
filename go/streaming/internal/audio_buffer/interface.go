package audio_buffer

type AudioBufferInterface interface {
	AppendAudioData(data []byte)
	GetFragmentAt(fragmentIndex int, isRecordingFinished bool) ([]byte, error)
	ReleaseAudioDataBefore(fragmentIndex int)
}
