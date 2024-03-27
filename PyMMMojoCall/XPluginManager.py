
import os
import time
import platform
from enum import Enum, auto
from PyMMMojoCall import ocr_protobuf_pb2, utility_protobuf_pb2
from ctypes import c_bool, py_object, cast, c_uint32, c_void_p, CDLL, c_int, c_char_p, POINTER, c_wchar_p, CFUNCTYPE, byref, string_at, memmove


class MMMojoInfoMethod(Enum):
    kMMNone = 0
    kMMPush = auto()
    kMMPullReq = auto()
    kMMPullResp = auto()
    kMMShared = auto()


class MMMojoEnvironmentCallbackType(Enum):
    kMMUserData = 0
    kMMReadPush = auto()
    kMMReadPull = auto()
    kMMReadShared = auto()
    kMMRemoteConnect = auto()
    kMMRemoteDisconnect = auto()
    kMMRemoteProcessLaunched = auto()
    kMMRemoteProcessLaunchFailed = auto()
    kMMRemoteMojoError = auto()


class MMMojoEnvironmentInitParamType(Enum):
    kMMHostProcess = 0
    kMMLoopStartThread = auto()
    kMMExePath = auto()
    kMMLogPath = auto()
    kMMLogToStderr = auto()
    kMMAddNumMessagepipe = auto()
    kMMSetDisconnectHandlers = auto()
    kMMDisableDefaultPolicy = 1000
    kMMElevated = auto()
    kMMCompatible = auto()


class RequestIdOCR(Enum):
    OCRPush = 1


class RequestIdUtility(Enum):
    UtilityHiPush = 10001
    UtilityInitPullReq = 10002
    UtilityInitPullResp = 10003
    UtilityResampleImagePullReq = 10010
    UtilityResampleImagePullResp = 10011
    UtilityDecodeImagePullReq = 10020
    UtilityDecodeImagePullResp = 10021
    UtilityPicQRScanPullReq = 10030
    UtilityQRScanPullReq = 10031
    UtilityQRScanPullResp = 10032
    UtilityTextScanPushResp = 10040


def read_push(request_id: c_uint32, request_info: c_void_p, user_data: py_object):
    # print(f"kMMReadPush 回调函数被调用 参数, request_id: {request_id}, request_info: {request_info}")
    if user_data:
        manager_obj = cast(user_data, py_object).value
        pb_size = c_uint32()
        pb_data = manager_obj.mmmojo_dll.GetMMMojoReadInfoRequest(request_info, byref(pb_size))
        chr_ptr = string_at(pb_data, pb_size.value)
        manager_obj.on_message(chr_ptr)


def read_pull(request_id:c_uint32, request_info:c_void_p, user_data: py_object):
    # print(f"kMMReadPull 回调函数被调用, request_id: {request_id}, request_info: {request_info} ")
    if user_data:
        manager_obj = cast(user_data, py_object).value
        pb_size = c_uint32()
        pb_data = manager_obj.mmmojo_dll.GetMMMojoReadInfoRequest(request_info, byref(pb_size))
        chr_ptr = string_at(pb_data, pb_size.value)
        manager_obj.on_message(chr_ptr)


def read_shared(request_id: c_uint32, request_info:c_void_p, user_data: py_object):
    pass
    # print(f"kMMReadShared 回调函数被调用, request_id: {request_id}, request_info: {request_info} ")


def remote_connect(is_connected: c_bool, user_data: py_object):
    # print(f"kMMRemoteConnect 回调函数被调用, 参数, is_connected: {is_connected}")
    if user_data:
        manager_obj = cast(user_data, py_object).value
        manager_obj.m_connect_state = True


def remote_disconnect(user_data: py_object):
    # print(f"kMMRemoteDisconnect 回调函数被调用 ")
    if user_data:
        manager_obj = cast(user_data, py_object).value
        manager_obj.m_connect_state = False


def remote_process_launched(user_data: py_object):
    pass
    # print(f"kMMRemoteProcessLaunched 回调函数被调用 ")


def remote_process_launch_failed(error_code: c_int, user_data: py_object):
    pass
    # print(f"kMMRemoteProcessLaunchFailed 回调函数被调用, error_code: {error_code}")


def remote_mojo_error(errorbuf: c_void_p, errorsize: c_int, user_data: py_object):
    pass
    # print(f"kMMRemoteMojoError 回调函数被调用, errorbuf: {errorbuf}, errorsize: {errorsize}")


class BaseManager(object):

    OCR_MAX_TASK_ID: int = 32
    m_task_id: int
    m_init_mmmojo_env: bool = False
    m_connect_state: bool = False
    callbacks = {
        'kMMReadPush': [read_push, CFUNCTYPE(None, c_uint32, c_void_p, c_void_p)],
        'kMMReadPull': [read_pull, CFUNCTYPE(None, c_uint32, c_void_p, c_void_p)],
        'kMMReadShared': [read_shared, CFUNCTYPE(None, c_uint32, c_void_p, c_void_p)],
        'kMMRemoteConnect': [remote_connect, CFUNCTYPE(None, c_bool, c_void_p)],
        'kMMRemoteDisconnect': [remote_disconnect, CFUNCTYPE(None, c_void_p)],
        'kMMRemoteProcessLaunched': [remote_process_launched, CFUNCTYPE(None, c_void_p)],
        'kMMRemoteProcessLaunchFailed': [remote_process_launch_failed, CFUNCTYPE(None, c_int, c_void_p)],
        'kMMRemoteMojoError': [remote_mojo_error, CFUNCTYPE(None, c_void_p, c_int, c_void_p)]
    }

    def __init__(self, wechat_dir: str, exe_dir: str):
        self._callbacks_refer = dict()
        self.m_task_id = 0
        self.result = None

        python_bit = platform.architecture()[0]
        if python_bit == "64bit":
            dll_name = "mmmojo_64.dll"
        else:
            dll_name = "mmmojo.dll"
        mmmojo_dllpath = os.path.join(wechat_dir, dll_name)
        if not os.path.exists(mmmojo_dllpath):
            raise Exception("给定的微信路径不存在mmmojo.dll")
        self.mmmojo_dll = CDLL(mmmojo_dllpath)
        self.mmmojo_dll.InitializeMMMojo.restype = None
        self.mmmojo_dll.InitializeMMMojo.argtypes = [c_int, POINTER(c_char_p)]

        self.mmmojo_dll.ShutdownMMMojo.restype = None
        self.mmmojo_dll.InitializeMMMojo.argtypes = []

        self.mmmojo_dll.CreateMMMojoEnvironment.restype = c_void_p
        self.mmmojo_dll.CreateMMMojoEnvironment.argtypes = []

        self.mmmojo_dll.SetMMMojoEnvironmentCallbacks.restype = None
        self.mmmojo_dll.SetMMMojoEnvironmentCallbacks.argtypes = [c_void_p, c_int, py_object]

        self.mmmojo_dll.SetMMMojoEnvironmentInitParams.restype = None
        self.mmmojo_dll.SetMMMojoEnvironmentInitParams.argtypes = [c_void_p, c_int, c_void_p]

        self.mmmojo_dll.AppendMMSubProcessSwitchNative.restype = None
        self.mmmojo_dll.AppendMMSubProcessSwitchNative.argtypes = [c_void_p, c_char_p, c_wchar_p]

        self.mmmojo_dll.StartMMMojoEnvironment.restype = None
        self.mmmojo_dll.StartMMMojoEnvironment.argtypes = [c_void_p]

        self.mmmojo_dll.StopMMMojoEnvironment.restype = None
        self.mmmojo_dll.StopMMMojoEnvironment.argtypes = [c_void_p]

        self.mmmojo_dll.RemoveMMMojoEnvironment.restype = None
        self.mmmojo_dll.RemoveMMMojoEnvironment.argtypes = [c_void_p]

        self.mmmojo_dll.GetMMMojoReadInfoRequest.restype = c_void_p
        self.mmmojo_dll.GetMMMojoReadInfoRequest.argtypes = [c_void_p, POINTER(c_uint32)]

        self.mmmojo_dll.GetMMMojoReadInfoAttach.restype = c_void_p
        self.mmmojo_dll.GetMMMojoReadInfoAttach.argtypes = [c_void_p, POINTER(c_uint32)]

        self.mmmojo_dll.RemoveMMMojoReadInfo.restype = None
        self.mmmojo_dll.RemoveMMMojoReadInfo.argtypes = [c_void_p]

        self.mmmojo_dll.GetMMMojoReadInfoMethod.restype = c_int
        self.mmmojo_dll.GetMMMojoReadInfoMethod.argtypes = [c_void_p]

        self.mmmojo_dll.GetMMMojoReadInfoSync.restype = c_bool
        self.mmmojo_dll.GetMMMojoReadInfoSync.argtypes = [c_void_p]

        self.mmmojo_dll.CreateMMMojoWriteInfo.restype = c_void_p
        self.mmmojo_dll.CreateMMMojoWriteInfo.argtypes = [c_int, c_bool, c_uint32]

        self.mmmojo_dll.GetMMMojoWriteInfoRequest.restype = c_void_p
        self.mmmojo_dll.GetMMMojoWriteInfoRequest.argtypes = [c_void_p, c_uint32]

        self.mmmojo_dll.RemoveMMMojoWriteInfo.restype = None
        self.mmmojo_dll.RemoveMMMojoWriteInfo.argtypes = [c_void_p]

        self.mmmojo_dll.GetMMMojoWriteInfoAttach.restype = c_void_p
        self.mmmojo_dll.GetMMMojoWriteInfoAttach.argtypes = [c_void_p, c_uint32]

        self.mmmojo_dll.SetMMMojoWriteInfoMessagePipe.restype = None
        self.mmmojo_dll.SetMMMojoWriteInfoMessagePipe.argtypes = [c_void_p, c_int]

        self.mmmojo_dll.SetMMMojoWriteInfoResponseSync.restype = None
        self.mmmojo_dll.SetMMMojoWriteInfoResponseSync.argtypes = [c_void_p, POINTER(c_void_p)]

        self.mmmojo_dll.SendMMMojoWriteInfo.restype = c_bool
        self.mmmojo_dll.SendMMMojoWriteInfo.argtypes = [c_void_p, c_void_p]

        self.mmmojo_dll.SwapMMMojoWriteInfoCallback.restype = c_bool
        self.mmmojo_dll.SwapMMMojoWriteInfoCallback.argtypes = [c_void_p, c_void_p]

        self.mmmojo_dll.SwapMMMojoWriteInfoMessage.restype = c_bool
        self.mmmojo_dll.SwapMMMojoWriteInfoMessage.argtypes = [c_void_p, c_void_p]

        self.mmmojo_dll.InitializeMMMojo(0, None)

        self.m_mmmojo_env_ptr = c_void_p(self.mmmojo_dll.CreateMMMojoEnvironment())
        if not self.m_mmmojo_env_ptr:
            raise Exception("CreateMMMojoEnvironment失败!")

        # 设置回调函数的最后一个参数user_data的值
        for i in MMMojoEnvironmentCallbackType:
            fname = i.name
            if fname == "kMMUserData":
                self.mmmojo_dll.SetMMMojoEnvironmentCallbacks(self.m_mmmojo_env_ptr, i.value, py_object(self))
            else:
                callback, callback_def = BaseManager.callbacks[fname]
                self.mmmojo_dll.SetMMMojoEnvironmentCallbacks.restype = None
                self.mmmojo_dll.SetMMMojoEnvironmentCallbacks.argtypes = [c_void_p, c_int, callback_def]
                self._callbacks_refer[fname] = callback_def(callback)
                self.mmmojo_dll.SetMMMojoEnvironmentCallbacks(self.m_mmmojo_env_ptr, i.value, self._callbacks_refer[fname])
        # # 设置启动所需参数
        self.mmmojo_dll.SetMMMojoEnvironmentInitParams.restype = None
        self.mmmojo_dll.SetMMMojoEnvironmentInitParams.argtypes = [c_void_p, c_int, c_int]
        self.mmmojo_dll.SetMMMojoEnvironmentInitParams(self.m_mmmojo_env_ptr, MMMojoEnvironmentInitParamType.kMMHostProcess.value, 1)
        self.mmmojo_dll.SetMMMojoEnvironmentInitParams.restype = None
        self.mmmojo_dll.SetMMMojoEnvironmentInitParams.argtypes = [c_void_p, c_int, c_wchar_p]
        self.mmmojo_dll.SetMMMojoEnvironmentInitParams(self.m_mmmojo_env_ptr, MMMojoEnvironmentInitParamType.kMMExePath.value, exe_dir)
        # # 设置SwitchNative命令行参数
        self.mmmojo_dll.AppendMMSubProcessSwitchNative(self.m_mmmojo_env_ptr, c_char_p("user-lib-dir".encode()), c_wchar_p(wechat_dir))
        self.mmmojo_dll.StartMMMojoEnvironment(self.m_mmmojo_env_ptr)
        self.m_init_mmmojo_env = True
        while not self.m_connect_state:
            time.sleep(0.5)

    def __del__(self):
        self.m_connect_state = False
        self.m_init_mmmojo_env = False
        self.mmmojo_dll.StopMMMojoEnvironment(self.m_mmmojo_env_ptr)
        self.mmmojo_dll.RemoveMMMojoEnvironment(self.m_mmmojo_env_ptr)
        self.m_mmmojo_env_ptr = c_void_p(None)

    def send(self, pb_data: bytes, pb_size: int, method: int, sync: int, request_id: int) -> None:
        write_info: c_void_p = self.mmmojo_dll.CreateMMMojoWriteInfo(c_int(method), c_int(sync), c_uint32(request_id))
        request: c_void_p = self.mmmojo_dll.GetMMMojoWriteInfoRequest(write_info, c_uint32(pb_size))
        memmove(request, c_char_p(pb_data), pb_size)
        self.mmmojo_dll.SendMMMojoWriteInfo(self.m_mmmojo_env_ptr, write_info)


class XPluginManager(object):

    def __init__(self, wechat_dir: str, wechat_ocr_dir: str = None, wechat_utility_dir: str = None):
        self.wechat_dir = wechat_dir
        self.wechat_ocr_dir = wechat_ocr_dir
        self.wechat_utility_dir = wechat_utility_dir
        self.is_ocr = False
        self.is_utility = False
        self.OCRManager = None
        self.UtilityManager = None

    def scan(self, pic_path: str):
        if self.is_ocr:
            del self.OCRManager
            self.OCRManager = None
            self.is_ocr = False
        if not self.is_utility:
            self.UtilityManager = UtilityManager(self.wechat_dir, self.wechat_utility_dir)
            self.is_utility = True
        return self.UtilityManager.scan(pic_path)

    def ocr(self, pic_path: str):
        if self.is_utility:
            del self.UtilityManager
            self.UtilityManager = None
            self.is_utility = False
        if not self.is_ocr:
            self.OCRManager = OCRManager(self.wechat_dir, self.wechat_ocr_dir)
            self.is_ocr = True
        return self.OCRManager.ocr(pic_path)


class UtilityManager(BaseManager):

    def __init__(self, wechat_dir: str, wechat_utility_dir: str):
        super().__init__(wechat_dir, wechat_utility_dir)

    def scan(self, pic_path: str):
        self.result = None
        if not os.path.exists(pic_path):
            raise Exception(f"给定图片路径pic_path不存在: {pic_path}")
        pic_path = os.path.abspath(pic_path)
        _id = self.m_task_id % self.OCR_MAX_TASK_ID
        self.m_task_id += 1
        qrscan_request = utility_protobuf_pb2.QRScanReqMessage()
        qrscan_request.origin_pic = pic_path
        qrscan_request.decode_pic = ''
        qrscan_request.unknown_0 = 0
        qrscan_request.unknown_1 = 1
        qrscan_request.text_scan_id = _id
        qrscan_request.unknown_3 = 1
        serialized_data = qrscan_request.SerializeToString()
        self.send(serialized_data, len(serialized_data), MMMojoInfoMethod.kMMPullReq.value, 0, RequestIdUtility.UtilityPicQRScanPullReq.value)
        while not self.result:
            time.sleep(0.5)
        return self.result

    def on_message(self, message):
        qrscan_response = utility_protobuf_pb2.QRScanRespMessage()
        qrscan_response.ParseFromString(message)
        self.result = qrscan_response


class OCRManager(BaseManager):

    def __init__(self, wechat_dir: str, wechat_ocr_dir: str):
        super().__init__(wechat_dir, wechat_ocr_dir)

    def ocr(self, pic_path: str):
        self.result = None
        if not os.path.exists(pic_path):
            raise Exception(f"给定图片路径pic_path不存在: {pic_path}")
        pic_path = os.path.abspath(pic_path)
        _id = self.m_task_id % self.OCR_MAX_TASK_ID
        self.m_task_id += 1
        ocr_request = ocr_protobuf_pb2.OcrRequest()
        ocr_request.unknow = 0
        ocr_request.task_id = _id
        ocr_request.pic_path.pic_path.append(pic_path)
        serialized_data = ocr_request.SerializeToString()
        self.send(serialized_data, len(serialized_data), MMMojoInfoMethod.kMMPush.value, 0, RequestIdOCR.OCRPush.value)
        while not self.result:
            time.sleep(0.5)
        return self.result

    def on_message(self, message):
        ocr_response = ocr_protobuf_pb2.OcrResponse()
        ocr_response.ParseFromString(message)
        self.result = ocr_response
