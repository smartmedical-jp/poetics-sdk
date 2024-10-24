#include "close_stream_asr_job_message.h"

namespace poetics::streaming::asr_job::outgoing_message {
    CloseStreamAsrJobMessage::CloseStreamAsrJobMessage(string _command) : command(_command) {}

    CloseStreamAsrJobMessage CloseStreamAsrJobMessage::CreateDefault() { 
        return CloseStreamAsrJobMessage("close_stream_asr_job"); 
    }

    void to_json(json& j, const CloseStreamAsrJobMessage& message) {
        j = json{{"command", message.command}};
    }
}
