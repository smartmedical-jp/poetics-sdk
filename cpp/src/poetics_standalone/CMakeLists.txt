project(poetics_standalone)

# find_library(poetics-sdk poetics-sdk HINTS "D:/REPO/poetics-sdk-cpp/builds/ninja-multi-vcpkg/poetics-sdk/Debug")

set(SOURCES 
    main.cpp
    poetics_sdk/streaming/streaming_client.cpp
    poetics_sdk/streaming/asr_job/stream_asr_job.cpp
    poetics_sdk/streaming/asr_job/core/stream_asr_job_core.cpp
    poetics_sdk/streaming/asr_job/core/audio_buffer_state.cpp
    poetics_sdk/streaming/asr_job/outgoing_message/create_stream_asr_job_message.cpp
    poetics_sdk/streaming/asr_job/outgoing_message/close_stream_asr_job_message.cpp
    poetics_sdk/streaming/asr_job/outgoing_message/submit_audio_fragment_message.cpp
    poetics_sdk/streaming/asr_job/incoming_message/audio_fragment_submission_progress_message.cpp
    poetics_sdk/streaming/asr_job/incoming_message/error_message.cpp
    poetics_sdk/streaming/asr_job/incoming_message/job_detail_message.cpp
    poetics_sdk/streaming/asr_job/incoming_message/job_status_updated_message.cpp
    poetics_sdk/streaming/asr_job/incoming_message/temporary_utterance_message.cpp
    poetics_sdk/streaming/asr_job/incoming_message/utterance_message.cpp
    poetics_sdk/streaming/audio_buffer/audio_buffer.cpp
    )

add_executable(poetics_standalone ${SOURCES})

find_package(Poco REQUIRED COMPONENTS Foundation Net NetSSL)
target_link_libraries(poetics_standalone PRIVATE Poco::Foundation Poco::Net Poco::NetSSL)

find_package(fmt CONFIG REQUIRED)
target_link_libraries(poetics_standalone PRIVATE fmt::fmt-header-only)

set_property(TARGET poetics_standalone PROPERTY CXX_STANDARD 20)
