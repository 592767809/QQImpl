from google.protobuf.internal import containers as _containers
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from typing import ClassVar as _ClassVar, Iterable as _Iterable, Mapping as _Mapping, Optional as _Optional, Union as _Union

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

class ResampleImageReqMessage(_message.Message):
    __slots__ = ["decode_pic", "encode_pic", "re_pic_x", "re_pic_y", "unknown_0"]
    DECODE_PIC_FIELD_NUMBER: _ClassVar[int]
    ENCODE_PIC_FIELD_NUMBER: _ClassVar[int]
    RE_PIC_X_FIELD_NUMBER: _ClassVar[int]
    RE_PIC_Y_FIELD_NUMBER: _ClassVar[int]
    UNKNOWN_0_FIELD_NUMBER: _ClassVar[int]
    decode_pic: str
    encode_pic: str
    re_pic_x: int
    re_pic_y: int
    unknown_0: int
    def __init__(self, decode_pic: _Optional[str] = ..., encode_pic: _Optional[str] = ..., re_pic_x: _Optional[int] = ..., re_pic_y: _Optional[int] = ..., unknown_0: _Optional[int] = ...) -> None: ...

class ResampleImageRespMessage(_message.Message):
    __slots__ = ["err_code"]
    ERR_CODE_FIELD_NUMBER: _ClassVar[int]
    err_code: int
    def __init__(self, err_code: _Optional[int] = ...) -> None: ...

class TextScanMessage(_message.Message):
    __slots__ = ["text_scan_id", "pic_path", "have_text", "unknown_0", "rate"]
    TEXT_SCAN_ID_FIELD_NUMBER: _ClassVar[int]
    PIC_PATH_FIELD_NUMBER: _ClassVar[int]
    HAVE_TEXT_FIELD_NUMBER: _ClassVar[int]
    UNKNOWN_0_FIELD_NUMBER: _ClassVar[int]
    RATE_FIELD_NUMBER: _ClassVar[int]
    text_scan_id: int
    pic_path: str
    have_text: int
    unknown_0: int
    rate: float
    def __init__(self, text_scan_id: _Optional[int] = ..., pic_path: _Optional[str] = ..., have_text: _Optional[int] = ..., unknown_0: _Optional[int] = ..., rate: _Optional[float] = ...) -> None: ...

class QRScanReqMessage(_message.Message):
    __slots__ = ["origin_pic", "decode_pic", "unknown_0", "encode_pic_data", "encode_pic_x", "encode_pic_y", "unknown_1", "text_scan_id", "unknown_3"]
    ORIGIN_PIC_FIELD_NUMBER: _ClassVar[int]
    DECODE_PIC_FIELD_NUMBER: _ClassVar[int]
    UNKNOWN_0_FIELD_NUMBER: _ClassVar[int]
    ENCODE_PIC_DATA_FIELD_NUMBER: _ClassVar[int]
    ENCODE_PIC_X_FIELD_NUMBER: _ClassVar[int]
    ENCODE_PIC_Y_FIELD_NUMBER: _ClassVar[int]
    UNKNOWN_1_FIELD_NUMBER: _ClassVar[int]
    TEXT_SCAN_ID_FIELD_NUMBER: _ClassVar[int]
    UNKNOWN_3_FIELD_NUMBER: _ClassVar[int]
    origin_pic: str
    decode_pic: str
    unknown_0: int
    encode_pic_data: bytes
    encode_pic_x: int
    encode_pic_y: int
    unknown_1: int
    text_scan_id: int
    unknown_3: int
    def __init__(self, origin_pic: _Optional[str] = ..., decode_pic: _Optional[str] = ..., unknown_0: _Optional[int] = ..., encode_pic_data: _Optional[bytes] = ..., encode_pic_x: _Optional[int] = ..., encode_pic_y: _Optional[int] = ..., unknown_1: _Optional[int] = ..., text_scan_id: _Optional[int] = ..., unknown_3: _Optional[int] = ...) -> None: ...

class QRScanRespMessage(_message.Message):
    __slots__ = ["qr_result", "unknown_0"]
    class QRScanResult(_message.Message):
        __slots__ = ["result", "unknow_0", "unknown_1", "unknown_2", "unknown_3"]
        RESULT_FIELD_NUMBER: _ClassVar[int]
        UNKNOW_0_FIELD_NUMBER: _ClassVar[int]
        UNKNOWN_1_FIELD_NUMBER: _ClassVar[int]
        UNKNOWN_2_FIELD_NUMBER: _ClassVar[int]
        UNKNOWN_3_FIELD_NUMBER: _ClassVar[int]
        result: bytes
        unknow_0: int
        unknown_1: float
        unknown_2: float
        unknown_3: int
        def __init__(self, result: _Optional[bytes] = ..., unknow_0: _Optional[int] = ..., unknown_1: _Optional[float] = ..., unknown_2: _Optional[float] = ..., unknown_3: _Optional[int] = ...) -> None: ...
    QR_RESULT_FIELD_NUMBER: _ClassVar[int]
    UNKNOWN_0_FIELD_NUMBER: _ClassVar[int]
    qr_result: _containers.RepeatedCompositeFieldContainer[QRScanRespMessage.QRScanResult]
    unknown_0: int
    def __init__(self, qr_result: _Optional[_Iterable[_Union[QRScanRespMessage.QRScanResult, _Mapping]]] = ..., unknown_0: _Optional[int] = ...) -> None: ...
