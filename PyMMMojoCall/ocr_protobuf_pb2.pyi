from google.protobuf.internal import containers as _containers
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from typing import ClassVar as _ClassVar, Iterable as _Iterable, Mapping as _Mapping, Optional as _Optional, Union as _Union

DESCRIPTOR: _descriptor.FileDescriptor

class OcrResponse(_message.Message):
    __slots__ = ["type", "task_id", "err_code", "ocr_result"]
    class OcrResult(_message.Message):
        __slots__ = ["single_result", "unknown_1", "unknown_2"]
        class ResultPos(_message.Message):
            __slots__ = ["pos"]
            class PosXY(_message.Message):
                __slots__ = ["x", "y"]
                X_FIELD_NUMBER: _ClassVar[int]
                Y_FIELD_NUMBER: _ClassVar[int]
                x: float
                y: float
                def __init__(self, x: _Optional[float] = ..., y: _Optional[float] = ...) -> None: ...
            POS_FIELD_NUMBER: _ClassVar[int]
            pos: _containers.RepeatedCompositeFieldContainer[OcrResponse.OcrResult.ResultPos.PosXY]
            def __init__(self, pos: _Optional[_Iterable[_Union[OcrResponse.OcrResult.ResultPos.PosXY, _Mapping]]] = ...) -> None: ...
        class SingleResult(_message.Message):
            __slots__ = ["single_pos", "single_str_utf8", "single_rate", "one_result", "left", "top", "right", "bottom", "unknown_0", "unknown_pos"]
            class OneResult(_message.Message):
                __slots__ = ["one_pos", "one_str_utf8"]
                ONE_POS_FIELD_NUMBER: _ClassVar[int]
                ONE_STR_UTF8_FIELD_NUMBER: _ClassVar[int]
                one_pos: OcrResponse.OcrResult.ResultPos
                one_str_utf8: bytes
                def __init__(self, one_pos: _Optional[_Union[OcrResponse.OcrResult.ResultPos, _Mapping]] = ..., one_str_utf8: _Optional[bytes] = ...) -> None: ...
            SINGLE_POS_FIELD_NUMBER: _ClassVar[int]
            SINGLE_STR_UTF8_FIELD_NUMBER: _ClassVar[int]
            SINGLE_RATE_FIELD_NUMBER: _ClassVar[int]
            ONE_RESULT_FIELD_NUMBER: _ClassVar[int]
            LEFT_FIELD_NUMBER: _ClassVar[int]
            TOP_FIELD_NUMBER: _ClassVar[int]
            RIGHT_FIELD_NUMBER: _ClassVar[int]
            BOTTOM_FIELD_NUMBER: _ClassVar[int]
            UNKNOWN_0_FIELD_NUMBER: _ClassVar[int]
            UNKNOWN_POS_FIELD_NUMBER: _ClassVar[int]
            single_pos: OcrResponse.OcrResult.ResultPos
            single_str_utf8: bytes
            single_rate: float
            one_result: _containers.RepeatedCompositeFieldContainer[OcrResponse.OcrResult.SingleResult.OneResult]
            left: float
            top: float
            right: float
            bottom: float
            unknown_0: int
            unknown_pos: OcrResponse.OcrResult.ResultPos
            def __init__(self, single_pos: _Optional[_Union[OcrResponse.OcrResult.ResultPos, _Mapping]] = ..., single_str_utf8: _Optional[bytes] = ..., single_rate: _Optional[float] = ..., one_result: _Optional[_Iterable[_Union[OcrResponse.OcrResult.SingleResult.OneResult, _Mapping]]] = ..., left: _Optional[float] = ..., top: _Optional[float] = ..., right: _Optional[float] = ..., bottom: _Optional[float] = ..., unknown_0: _Optional[int] = ..., unknown_pos: _Optional[_Union[OcrResponse.OcrResult.ResultPos, _Mapping]] = ...) -> None: ...
        SINGLE_RESULT_FIELD_NUMBER: _ClassVar[int]
        UNKNOWN_1_FIELD_NUMBER: _ClassVar[int]
        UNKNOWN_2_FIELD_NUMBER: _ClassVar[int]
        single_result: _containers.RepeatedCompositeFieldContainer[OcrResponse.OcrResult.SingleResult]
        unknown_1: int
        unknown_2: int
        def __init__(self, single_result: _Optional[_Iterable[_Union[OcrResponse.OcrResult.SingleResult, _Mapping]]] = ..., unknown_1: _Optional[int] = ..., unknown_2: _Optional[int] = ...) -> None: ...
    TYPE_FIELD_NUMBER: _ClassVar[int]
    TASK_ID_FIELD_NUMBER: _ClassVar[int]
    ERR_CODE_FIELD_NUMBER: _ClassVar[int]
    OCR_RESULT_FIELD_NUMBER: _ClassVar[int]
    type: int
    task_id: int
    err_code: int
    ocr_result: OcrResponse.OcrResult
    def __init__(self, type: _Optional[int] = ..., task_id: _Optional[int] = ..., err_code: _Optional[int] = ..., ocr_result: _Optional[_Union[OcrResponse.OcrResult, _Mapping]] = ...) -> None: ...

class OcrRequest(_message.Message):
    __slots__ = ["unknow", "task_id", "pic_path"]
    class PicPaths(_message.Message):
        __slots__ = ["pic_path"]
        PIC_PATH_FIELD_NUMBER: _ClassVar[int]
        pic_path: _containers.RepeatedScalarFieldContainer[str]
        def __init__(self, pic_path: _Optional[_Iterable[str]] = ...) -> None: ...
    UNKNOW_FIELD_NUMBER: _ClassVar[int]
    TASK_ID_FIELD_NUMBER: _ClassVar[int]
    PIC_PATH_FIELD_NUMBER: _ClassVar[int]
    unknow: int
    task_id: int
    pic_path: OcrRequest.PicPaths
    def __init__(self, unknow: _Optional[int] = ..., task_id: _Optional[int] = ..., pic_path: _Optional[_Union[OcrRequest.PicPaths, _Mapping]] = ...) -> None: ...
