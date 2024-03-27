from google.protobuf.internal import containers as _containers
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from typing import ClassVar as _ClassVar, Iterable as _Iterable, Optional as _Optional

DESCRIPTOR: _descriptor.FileDescriptor

class InitReqMessage(_message.Message):
    __slots__ = ["expire_timestamp", "type"]
    EXPIRE_TIMESTAMP_FIELD_NUMBER: _ClassVar[int]
    TYPE_FIELD_NUMBER: _ClassVar[int]
    expire_timestamp: int
    type: int
    def __init__(self, expire_timestamp: _Optional[int] = ..., type: _Optional[int] = ...) -> None: ...

class InitRespMessage(_message.Message):
    __slots__ = ["status"]
    STATUS_FIELD_NUMBER: _ClassVar[int]
    status: int
    def __init__(self, status: _Optional[int] = ...) -> None: ...

class CreatePlayerCoreReqMessage(_message.Message):
    __slots__ = ["player_id", "is_post_frame"]
    PLAYER_ID_FIELD_NUMBER: _ClassVar[int]
    IS_POST_FRAME_FIELD_NUMBER: _ClassVar[int]
    player_id: int
    is_post_frame: int
    def __init__(self, player_id: _Optional[int] = ..., is_post_frame: _Optional[int] = ...) -> None: ...

class CreatePlayerCoreRespMessage(_message.Message):
    __slots__ = ["player_id", "is_suceess"]
    PLAYER_ID_FIELD_NUMBER: _ClassVar[int]
    IS_SUCEESS_FIELD_NUMBER: _ClassVar[int]
    player_id: int
    is_suceess: int
    def __init__(self, player_id: _Optional[int] = ..., is_suceess: _Optional[int] = ...) -> None: ...

class InitPlayerCoreMessage(_message.Message):
    __slots__ = ["player_id", "file_name", "file_path", "file_size", "is_local", "placeholder_6", "media_id", "is_audio_mute", "video_surface", "audio_volume", "is_repeat", "speed_ratio", "max_frame_width", "max_frame_height", "codec_type", "decoder_type"]
    PLAYER_ID_FIELD_NUMBER: _ClassVar[int]
    FILE_NAME_FIELD_NUMBER: _ClassVar[int]
    FILE_PATH_FIELD_NUMBER: _ClassVar[int]
    FILE_SIZE_FIELD_NUMBER: _ClassVar[int]
    IS_LOCAL_FIELD_NUMBER: _ClassVar[int]
    PLACEHOLDER_6_FIELD_NUMBER: _ClassVar[int]
    MEDIA_ID_FIELD_NUMBER: _ClassVar[int]
    IS_AUDIO_MUTE_FIELD_NUMBER: _ClassVar[int]
    VIDEO_SURFACE_FIELD_NUMBER: _ClassVar[int]
    AUDIO_VOLUME_FIELD_NUMBER: _ClassVar[int]
    IS_REPEAT_FIELD_NUMBER: _ClassVar[int]
    SPEED_RATIO_FIELD_NUMBER: _ClassVar[int]
    MAX_FRAME_WIDTH_FIELD_NUMBER: _ClassVar[int]
    MAX_FRAME_HEIGHT_FIELD_NUMBER: _ClassVar[int]
    CODEC_TYPE_FIELD_NUMBER: _ClassVar[int]
    DECODER_TYPE_FIELD_NUMBER: _ClassVar[int]
    player_id: int
    file_name: str
    file_path: str
    file_size: int
    is_local: bool
    placeholder_6: int
    media_id: int
    is_audio_mute: bool
    video_surface: int
    audio_volume: float
    is_repeat: int
    speed_ratio: float
    max_frame_width: int
    max_frame_height: int
    codec_type: int
    decoder_type: int
    def __init__(self, player_id: _Optional[int] = ..., file_name: _Optional[str] = ..., file_path: _Optional[str] = ..., file_size: _Optional[int] = ..., is_local: bool = ..., placeholder_6: _Optional[int] = ..., media_id: _Optional[int] = ..., is_audio_mute: bool = ..., video_surface: _Optional[int] = ..., audio_volume: _Optional[float] = ..., is_repeat: _Optional[int] = ..., speed_ratio: _Optional[float] = ..., max_frame_width: _Optional[int] = ..., max_frame_height: _Optional[int] = ..., codec_type: _Optional[int] = ..., decoder_type: _Optional[int] = ...) -> None: ...

class InfoPlayerCoreMessage(_message.Message):
    __slots__ = ["player_id", "type", "arg", "unknown_2"]
    PLAYER_ID_FIELD_NUMBER: _ClassVar[int]
    TYPE_FIELD_NUMBER: _ClassVar[int]
    ARG_FIELD_NUMBER: _ClassVar[int]
    UNKNOWN_2_FIELD_NUMBER: _ClassVar[int]
    player_id: int
    type: str
    arg: int
    unknown_2: int
    def __init__(self, player_id: _Optional[int] = ..., type: _Optional[str] = ..., arg: _Optional[int] = ..., unknown_2: _Optional[int] = ...) -> None: ...

class PlayerIdMessage(_message.Message):
    __slots__ = ["player_id"]
    PLAYER_ID_FIELD_NUMBER: _ClassVar[int]
    player_id: int
    def __init__(self, player_id: _Optional[int] = ...) -> None: ...

class CurrentPositionMsMessage(_message.Message):
    __slots__ = ["unknown"]
    UNKNOWN_FIELD_NUMBER: _ClassVar[int]
    unknown: int
    def __init__(self, unknown: _Optional[int] = ...) -> None: ...

class CurrentPositionMsInfo(_message.Message):
    __slots__ = ["ms_info"]
    class MsInfo(_message.Message):
        __slots__ = ["player_id", "pos"]
        PLAYER_ID_FIELD_NUMBER: _ClassVar[int]
        POS_FIELD_NUMBER: _ClassVar[int]
        player_id: int
        pos: int
        def __init__(self, player_id: _Optional[int] = ..., pos: _Optional[int] = ...) -> None: ...
    MS_INFO_FIELD_NUMBER: _ClassVar[int]
    ms_info: _containers.RepeatedScalarFieldContainer[bytes]
    def __init__(self, ms_info: _Optional[_Iterable[bytes]] = ...) -> None: ...

class ResizePlayerCoreMessage(_message.Message):
    __slots__ = ["player_id", "new_width", "new_height"]
    PLAYER_ID_FIELD_NUMBER: _ClassVar[int]
    NEW_WIDTH_FIELD_NUMBER: _ClassVar[int]
    NEW_HEIGHT_FIELD_NUMBER: _ClassVar[int]
    player_id: int
    new_width: int
    new_height: int
    def __init__(self, player_id: _Optional[int] = ..., new_width: _Optional[int] = ..., new_height: _Optional[int] = ...) -> None: ...

class VideoSizeChangedPlayerCoreMessage(_message.Message):
    __slots__ = ["player_id", "org_width", "org_height"]
    PLAYER_ID_FIELD_NUMBER: _ClassVar[int]
    ORG_WIDTH_FIELD_NUMBER: _ClassVar[int]
    ORG_HEIGHT_FIELD_NUMBER: _ClassVar[int]
    player_id: int
    org_width: int
    org_height: int
    def __init__(self, player_id: _Optional[int] = ..., org_width: _Optional[int] = ..., org_height: _Optional[int] = ...) -> None: ...

class PreparePlayerCoreMessage(_message.Message):
    __slots__ = ["player_id", "duration"]
    PLAYER_ID_FIELD_NUMBER: _ClassVar[int]
    DURATION_FIELD_NUMBER: _ClassVar[int]
    player_id: int
    duration: int
    def __init__(self, player_id: _Optional[int] = ..., duration: _Optional[int] = ...) -> None: ...

class StatePlayerCoreMessage(_message.Message):
    __slots__ = ["player_id", "state"]
    PLAYER_ID_FIELD_NUMBER: _ClassVar[int]
    STATE_FIELD_NUMBER: _ClassVar[int]
    player_id: int
    state: int
    def __init__(self, player_id: _Optional[int] = ..., state: _Optional[int] = ...) -> None: ...

class AudioMutePlayerCoreMessage(_message.Message):
    __slots__ = ["player_id", "is_audio_mute"]
    PLAYER_ID_FIELD_NUMBER: _ClassVar[int]
    IS_AUDIO_MUTE_FIELD_NUMBER: _ClassVar[int]
    player_id: int
    is_audio_mute: bool
    def __init__(self, player_id: _Optional[int] = ..., is_audio_mute: bool = ...) -> None: ...

class SeekToPlayerCoreMessage(_message.Message):
    __slots__ = ["player_id", "pos_ms"]
    PLAYER_ID_FIELD_NUMBER: _ClassVar[int]
    POS_MS_FIELD_NUMBER: _ClassVar[int]
    player_id: int
    pos_ms: int
    def __init__(self, player_id: _Optional[int] = ..., pos_ms: _Optional[int] = ...) -> None: ...

class VideoSurfaceMessage(_message.Message):
    __slots__ = ["player_id", "window"]
    PLAYER_ID_FIELD_NUMBER: _ClassVar[int]
    WINDOW_FIELD_NUMBER: _ClassVar[int]
    player_id: int
    window: int
    def __init__(self, player_id: _Optional[int] = ..., window: _Optional[int] = ...) -> None: ...

class AudioVolumePlayerCoreMessage(_message.Message):
    __slots__ = ["player_id", "audio_volume"]
    PLAYER_ID_FIELD_NUMBER: _ClassVar[int]
    AUDIO_VOLUME_FIELD_NUMBER: _ClassVar[int]
    player_id: int
    audio_volume: float
    def __init__(self, player_id: _Optional[int] = ..., audio_volume: _Optional[float] = ...) -> None: ...

class RepeatPlayerCoreMessage(_message.Message):
    __slots__ = ["player_id", "is_repeat"]
    PLAYER_ID_FIELD_NUMBER: _ClassVar[int]
    IS_REPEAT_FIELD_NUMBER: _ClassVar[int]
    player_id: int
    is_repeat: int
    def __init__(self, player_id: _Optional[int] = ..., is_repeat: _Optional[int] = ...) -> None: ...

class SpeedRatioPlayerCoreMessage(_message.Message):
    __slots__ = ["player_id", "speed_ratio"]
    PLAYER_ID_FIELD_NUMBER: _ClassVar[int]
    SPEED_RATIO_FIELD_NUMBER: _ClassVar[int]
    player_id: int
    speed_ratio: float
    def __init__(self, player_id: _Optional[int] = ..., speed_ratio: _Optional[float] = ...) -> None: ...

class ReadyDataSourceCoreMessage(_message.Message):
    __slots__ = ["player_id", "file_name", "file_path", "media_id"]
    PLAYER_ID_FIELD_NUMBER: _ClassVar[int]
    FILE_NAME_FIELD_NUMBER: _ClassVar[int]
    FILE_PATH_FIELD_NUMBER: _ClassVar[int]
    MEDIA_ID_FIELD_NUMBER: _ClassVar[int]
    player_id: int
    file_name: str
    file_path: str
    media_id: int
    def __init__(self, player_id: _Optional[int] = ..., file_name: _Optional[str] = ..., file_path: _Optional[str] = ..., media_id: _Optional[int] = ...) -> None: ...

class DownloadDataSourceCoreMessage(_message.Message):
    __slots__ = ["player_id", "file_name", "file_path", "file_size", "media_id"]
    PLAYER_ID_FIELD_NUMBER: _ClassVar[int]
    FILE_NAME_FIELD_NUMBER: _ClassVar[int]
    FILE_PATH_FIELD_NUMBER: _ClassVar[int]
    FILE_SIZE_FIELD_NUMBER: _ClassVar[int]
    MEDIA_ID_FIELD_NUMBER: _ClassVar[int]
    player_id: int
    file_name: str
    file_path: str
    file_size: int
    media_id: int
    def __init__(self, player_id: _Optional[int] = ..., file_name: _Optional[str] = ..., file_path: _Optional[str] = ..., file_size: _Optional[int] = ..., media_id: _Optional[int] = ...) -> None: ...

class ErrorPlayerCoreMessage(_message.Message):
    __slots__ = ["player_id", "error", "error_type", "error_code"]
    PLAYER_ID_FIELD_NUMBER: _ClassVar[int]
    ERROR_FIELD_NUMBER: _ClassVar[int]
    ERROR_TYPE_FIELD_NUMBER: _ClassVar[int]
    ERROR_CODE_FIELD_NUMBER: _ClassVar[int]
    player_id: int
    error: str
    error_type: int
    error_code: int
    def __init__(self, player_id: _Optional[int] = ..., error: _Optional[str] = ..., error_type: _Optional[int] = ..., error_code: _Optional[int] = ...) -> None: ...

class StartTaskProxyMessage(_message.Message):
    __slots__ = ["task_id", "player_id"]
    TASK_ID_FIELD_NUMBER: _ClassVar[int]
    PLAYER_ID_FIELD_NUMBER: _ClassVar[int]
    task_id: int
    player_id: int
    def __init__(self, task_id: _Optional[int] = ..., player_id: _Optional[int] = ...) -> None: ...

class StartRequestProxyMessage(_message.Message):
    __slots__ = ["task_id", "player_id", "request_start", "request_end"]
    TASK_ID_FIELD_NUMBER: _ClassVar[int]
    PLAYER_ID_FIELD_NUMBER: _ClassVar[int]
    REQUEST_START_FIELD_NUMBER: _ClassVar[int]
    REQUEST_END_FIELD_NUMBER: _ClassVar[int]
    task_id: int
    player_id: int
    request_start: int
    request_end: int
    def __init__(self, task_id: _Optional[int] = ..., player_id: _Optional[int] = ..., request_start: _Optional[int] = ..., request_end: _Optional[int] = ...) -> None: ...

class CloseRequestProxyMessage(_message.Message):
    __slots__ = ["task_id", "player_id"]
    TASK_ID_FIELD_NUMBER: _ClassVar[int]
    PLAYER_ID_FIELD_NUMBER: _ClassVar[int]
    task_id: int
    player_id: int
    def __init__(self, task_id: _Optional[int] = ..., player_id: _Optional[int] = ...) -> None: ...

class PollingDataProxyReqMessage(_message.Message):
    __slots__ = ["task_id", "player_id", "offset", "ready_size", "download_size", "read_size"]
    TASK_ID_FIELD_NUMBER: _ClassVar[int]
    PLAYER_ID_FIELD_NUMBER: _ClassVar[int]
    OFFSET_FIELD_NUMBER: _ClassVar[int]
    READY_SIZE_FIELD_NUMBER: _ClassVar[int]
    DOWNLOAD_SIZE_FIELD_NUMBER: _ClassVar[int]
    READ_SIZE_FIELD_NUMBER: _ClassVar[int]
    task_id: int
    player_id: int
    offset: int
    ready_size: int
    download_size: int
    read_size: int
    def __init__(self, task_id: _Optional[int] = ..., player_id: _Optional[int] = ..., offset: _Optional[int] = ..., ready_size: _Optional[int] = ..., download_size: _Optional[int] = ..., read_size: _Optional[int] = ...) -> None: ...

class PollingDataProxyRespMessage(_message.Message):
    __slots__ = []
    def __init__(self) -> None: ...
