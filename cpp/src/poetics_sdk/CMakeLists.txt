cmake_minimum_required(VERSION 3.15)
project(poetics_sdk)

set(SOURCES 
    streaming/streaming_client.cpp
    streaming/asr_job/stream_asr_job.cpp
    streaming/asr_job/core/stream_asr_job_core.cpp
    streaming/asr_job/core/audio_buffer_state.cpp
    streaming/asr_job/outgoing_message/create_stream_asr_job_message.cpp
    streaming/asr_job/outgoing_message/close_stream_asr_job_message.cpp
    streaming/asr_job/outgoing_message/submit_audio_fragment_message.cpp
    streaming/asr_job/incoming_message/audio_fragment_submission_progress_message.cpp
    streaming/asr_job/incoming_message/error_message.cpp
    streaming/asr_job/incoming_message/job_detail_message.cpp
    streaming/asr_job/incoming_message/job_status_updated_message.cpp
    streaming/asr_job/incoming_message/temporary_utterance_message.cpp
    streaming/asr_job/incoming_message/utterance_message.cpp
    streaming/audio_buffer/audio_buffer.cpp
    )
set(LIBRARY_OUTPUT_PATH ${POETICS_OUT_PATH})
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON) # VC++ compiler won't make .lib file but only .dll if you don't do this

add_library(poetics_sdk SHARED ${SOURCES})

find_package(Poco REQUIRED COMPONENTS Foundation Net NetSSL)
target_link_libraries(poetics_sdk PRIVATE Poco::Foundation Poco::Net Poco::NetSSL)

find_package(fmt CONFIG REQUIRED)
target_link_libraries(poetics_sdk PRIVATE fmt::fmt-header-only)

set_property(TARGET poetics_sdk PROPERTY CXX_STANDARD 20)