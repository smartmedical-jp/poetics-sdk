package audio_buffer

import (
	"fmt"
	"sync"

	"github.com/smartmedical-jp/poetics-sdk/go/streaming/internal/logging"
)

// オーディオバッファ。１チャンネル分の音声データを保持する
type AudioBuffer struct {
	bytes                    []byte     // 音声データ
	fromFragmentIndex        int        // バッファ先頭のフラグメント番号。最初は 0 だが DiscardDataBefore で削除されると変わる
	bytesPerFragment         int        // １フラグメントごとに送信するバイト数
	discardableFragmentIndex int        // この番号より前のフラグメントは削除可能。DiscardDataBefore で設定される
	mu                       sync.Mutex // ロック用
}

var _ AudioBufferInterface = &AudioBuffer{}

// オーディオバッファを初期化する
func NewAudioBuffer() AudioBuffer {
	return AudioBuffer{
		bytesPerFragment: 16000, // JSONメッセージの最大サイズが32KB。Base64エンコードされてJSONに埋め込まれるぶんの余裕を持たせる
	}
}

// オーディオバッファに音声データを追加する
func (b *AudioBuffer) AppendAudioData(
	data []byte,
) {
	logging.Logger.Debug("AudioBuffer.AppendAudioData()", "data.length", len(data))

	b.mu.Lock()
	defer b.mu.Unlock()

	b.bytes = append(b.bytes, data...)
}

// オーディオバッファから指定された番号のフラグメントを取得する
// isRecordingFinished=true の場合はバッファの残りがフラグメント長に満たない場合でもデータを返す。
// 録音中は isRecordingFinished=false にして、録音が終了して今後データが追加されない場合は true にする
// 戻り値の []byte は、将来的に取得可能になる可能性がある場合は nil を返す
func (b *AudioBuffer) GetFragmentAt(
	fragmentIndex int,
	isRecordingFinished bool,
) ([]byte, error) {
	b.mu.Lock()
	defer b.mu.Unlock()

	// 対象フラグメントの範囲を計算
	fromByteIndex := (fragmentIndex - b.fromFragmentIndex) * b.bytesPerFragment
	toByteIndex := fromByteIndex + b.bytesPerFragment

	// すでに開放されたフラグメントの場合はエラー
	if fragmentIndex < b.fromFragmentIndex {
		return nil, fmt.Errorf("fragment %d is already released", fragmentIndex)
	}

	// バッファの範囲よりも先の場合はエラーとせず nil を返す
	if fromByteIndex > len(b.bytes) {
		return nil, nil
	}

	// バッファの残りがフラグメント長に満たない場合は、isRecording=trueの場合は残りを返し、falseの場合は nil を返す
	if toByteIndex > len(b.bytes) {
		if isRecordingFinished {
			return b.bytes[fromByteIndex:], nil
		} else {
			return nil, nil
		}
	}

	// バッファの範囲内の場合は、フラグメントを返す
	return b.bytes[fromByteIndex:toByteIndex], nil

}

// オーディオバッファから指定された番号のフラグメントを削除する
func (b *AudioBuffer) ReleaseAudioDataBefore(
	fragmentIndex int,
) {
	b.discardableFragmentIndex = fragmentIndex

	// 効率化のため毎回削除は行わず、一定数以上になったらまとめて削除
	DISCARD_THRESHOLD_FRAGMENTS := 10
	if b.discardableFragmentIndex-b.fromFragmentIndex > DISCARD_THRESHOLD_FRAGMENTS {
		b.mu.Lock()
		defer b.mu.Unlock()

		b.fromFragmentIndex = b.discardableFragmentIndex
		b.bytes = b.bytes[b.bytesPerFragment*(b.discardableFragmentIndex-b.fromFragmentIndex):]
	}
}
