
import os
import platform
from ctypes import c_bool, py_object, cast, c_uint32, c_void_p, CDLL, c_int, c_char_p, POINTER, c_wchar_p, CFUNCTYPE
from typing import Dict, Callable
from PyMMMojoCall.XPluginManager import XPluginManager, MMMojoEnvironmentCallbackType, MMMojoEnvironmentInitParamType


def read_push(request_id: c_uint32, request_info: c_void_p, user_data: py_object):
    print(f"OCRReadOnPush 回调函数被调用 参数, request_id: {request_id}, request_info: {request_info}")
    if user_data:
        manager_obj: OCRManager = cast(user_data, py_object).value
        pb_size = c_uint32()
        pb_data = manager_obj.GetPbSerializedData(request_info, pb_size)
        if pb_size.value > 10:
            print(f"正在解析pb数据，pb数据大小: {pb_size.value}")
            manager_obj.CallUsrCallback(request_id, pb_data, pb_size.value)
            manager_obj.RemoveReadInfo(request_info)


def read_pull(request_id:c_uint32, request_info:c_void_p, user_data: py_object):
    print(f"DefaultReadOnPull 回调函数被调用, request_id: {request_id}, request_info: {request_info} ")


def read_shared(request_id:c_uint32, request_info:c_void_p, user_data: py_object):
    print(f"DefaultReadOnShared 回调函数被调用, request_id: {request_id}, request_info: {request_info} ")


def remote_connect(is_connected: c_bool, user_data: py_object):
    print(f"OCRRemoteOnConnect 回调函数被调用, 参数, is_connected: {is_connected}")
    if user_data:
        manager_obj: OCRManager = cast(user_data, py_object).value
        manager_obj.SetConnectState(True)


def remote_disconnect(user_data: py_object):
    print(f"OCRRemoteOnDisConnect 回调函数被调用 ")
    if user_data:
        manager_obj: OCRManager = cast(user_data, py_object).value
        manager_obj.SetConnectState(False)


def remote_process_launched(user_data: py_object):
    print(f"DefaultRemoteProcessLaunched 回调函数被调用 ")


def remote_process_launch_failed(error_code: c_int, user_data: py_object):
    print(f"DefaultRemoteProcessLaunchFailed 回调函数被调用, error_code: {error_code}")


def remote_mojo_error(errorbuf: c_void_p, errorsize: c_int, user_data: py_object):
    print(f"DefaultRemoteOnMojoError 回调函数被调用, errorbuf: {errorbuf}, errorsize: {errorsize}")


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


class OCRManager(XPluginManager):

    OCR_MAX_TASK_ID: int = 32
    m_task_id: list = [0] * OCR_MAX_TASK_ID
    m_id_path: Dict[int, str] = {}
    m_wechatocr_running: bool = False
    m_connect_state: bool = False
    m_cb_data_use_json: bool = False
    m_usr_callback: Callable = None

    def __init__(self, wechat_dir: str, wechat_ocr_dir: str):
        super().__init__(wechat_dir)
        for i in range(self.OCR_MAX_TASK_ID):
            self.m_task_id[i] = 0

        self._callbacks_refer = dict()

        python_bit = platform.architecture()[0]
        if python_bit == "64bit":
            dll_name = "mmmojo_64.dll"
        else:
            dll_name = "mmmojo.dll"
        mmmojo_dllpath = os.path.join(wechat_dir, dll_name)
        if not os.path.exists(mmmojo_dllpath):
            raise Exception("给定的微信路径不存在mmmojo.dll")
        self._dll = CDLL(mmmojo_dllpath)
        self._dll.InitializeMMMojo.restype = None
        self._dll.InitializeMMMojo.argtypes = [c_int, POINTER(c_char_p)]

        self._dll.ShutdownMMMojo.restype = None
        self._dll.InitializeMMMojo.argtypes = []

        self._dll.CreateMMMojoEnvironment.restype = c_void_p
        self._dll.CreateMMMojoEnvironment.argtypes = []

        self._dll.SetMMMojoEnvironmentCallbacks.restype = None
        self._dll.SetMMMojoEnvironmentCallbacks.argtypes = [c_void_p, c_int, py_object]

        self._dll.SetMMMojoEnvironmentInitParams.restype = None
        self._dll.SetMMMojoEnvironmentInitParams.argtypes = [c_void_p, c_int, c_void_p]

        self._dll.AppendMMSubProcessSwitchNative.restype = None
        self._dll.AppendMMSubProcessSwitchNative.argtypes = [c_void_p, c_char_p, c_wchar_p]

        self._dll.StartMMMojoEnvironment.restype = None
        self._dll.StartMMMojoEnvironment.argtypes = [c_void_p]

        # # void StopMMMojoEnvironment(void* mmmojo_env);
        # StopMMMojoEnvironment = self.func_def("StopMMMojoEnvironment", void, *(c_void_p,))
        # # void RemoveMMMojoEnvironment(void* mmmojo_env);
        # RemoveMMMojoEnvironment = self.func_def("RemoveMMMojoEnvironment", void, *(c_void_p,))
        # # const void* GetMMMojoReadInfoRequest(const void* mmmojo_readinfo, uint32_t* request_data_size);
        # GetMMMojoReadInfoRequest = self.func_def("GetMMMojoReadInfoRequest", c_void_p, *(c_void_p, POINTER(c_uint32)))
        # # const void* GetMMMojoReadInfoAttach(const void* mmmojo_readinfo, uint32_t* attach_data_size);
        # GetMMMojoReadInfoAttach = self.func_def("GetMMMojoReadInfoAttach", c_void_p, *(c_void_p, POINTER(c_uint32)))
        # # void RemoveMMMojoReadInfo(void* mmmojo_readinfo);
        # RemoveMMMojoReadInfo = self.func_def("RemoveMMMojoReadInfo", void, *(c_void_p,))
        # # int GetMMMojoReadInfoMethod(const void* mmmojo_readinfo);
        # GetMMMojoReadInfoMethod = self.func_def("GetMMMojoReadInfoMethod", c_int, *(c_void_p,))
        # # bool GetMMMojoReadInfoSync(const void* mmmojo_readinfo);
        # GetMMMojoReadInfoSync = self.func_def("GetMMMojoReadInfoSync", c_bool, *(c_void_p,))
        # # void* CreateMMMojoWriteInfo(int method, bool sync, uint32_t request_id);
        # CreateMMMojoWriteInfo = self.func_def("CreateMMMojoWriteInfo", c_void_p, *(c_int, c_bool, c_uint32))
        # # void* GetMMMojoWriteInfoRequest(void* mmmojo_writeinfo, uint32_t request_data_size);
        # GetMMMojoWriteInfoRequest = self.func_def("GetMMMojoWriteInfoRequest", c_void_p, *(c_void_p, c_uint32))
        # # void RemoveMMMojoWriteInfo(void* mmmojo_writeinfo);
        # RemoveMMMojoWriteInfo = self.func_def("RemoveMMMojoWriteInfo", void, *(c_void_p,))
        # # void* GetMMMojoWriteInfoAttach(void* mmmojo_writeinfo,uint32_t attach_data_size);
        # GetMMMojoWriteInfoAttach = self.func_def("GetMMMojoWriteInfoAttach", c_void_p, *(c_void_p, c_uint32))
        # # void SetMMMojoWriteInfoMessagePipe(void* mmmojo_writeinfo,int num_of_message_pipe);
        # SetMMMojoWriteInfoMessagePipe = self.func_def("SetMMMojoWriteInfoMessagePipe", void, *(c_void_p, c_int))
        # # void SetMMMojoWriteInfoResponseSync(void* mmmojo_writeinfo, void** mmmojo_readinfo);
        # SetMMMojoWriteInfoResponseSync = self.func_def("SetMMMojoWriteInfoResponseSync", void, *(c_void_p, POINTER(c_void_p)))
        # # bool SendMMMojoWriteInfo(void* mmmojo_env,void* mmmojo_writeinfo);
        # SendMMMojoWriteInfo = self.func_def("SendMMMojoWriteInfo", c_bool, *(c_void_p, c_void_p))
        # # bool SwapMMMojoWriteInfoCallback(void* mmmojo_writeinfo,void* mmmojo_readinfo);
        # SwapMMMojoWriteInfoCallback = self.func_def("SwapMMMojoWriteInfoCallback", c_bool, *(c_void_p, c_void_p))
        # # bool SwapMMMojoWriteInfoMessage(void* mmmojo_writeinfo, void* mmmojo_readinfo);
        # SwapMMMojoWriteInfoMessage = self.func_def("SwapMMMojoWriteInfoMessage", c_bool, *(c_void_p, c_void_p))

        self._dll.InitializeMMMojo(0, None)

        self.m_mmmojo_env_ptr = c_void_p(self._dll.CreateMMMojoEnvironment())
        if not self.m_mmmojo_env_ptr:
            raise Exception("CreateMMMojoEnvironment失败!")

        self.m_cb_usrdata = self
        self.m_usr_lib_dir = wechat_dir

        # 设置回调函数的最后一个参数user_data的值
        for i in MMMojoEnvironmentCallbackType:
            fname = i.name
            if fname == "kMMUserData":
                self._dll.SetMMMojoEnvironmentCallbacks(self.m_mmmojo_env_ptr, i.value, py_object(self.m_cb_usrdata))
            else:
                callback, callback_def = callbacks[fname]
                self._dll.SetMMMojoEnvironmentCallbacks.restype = None
                self._dll.SetMMMojoEnvironmentCallbacks.argtypes = [c_void_p, c_int, callback_def]
                self._callbacks_refer[fname] = callback_def(callback)
                self._dll.SetMMMojoEnvironmentCallbacks(self.m_mmmojo_env_ptr, i.value, self._callbacks_refer[fname])
        # # 设置启动所需参数
        self._dll.SetMMMojoEnvironmentInitParams.restype = None
        self._dll.SetMMMojoEnvironmentInitParams.argtypes = [c_void_p, c_int, c_int]
        self._dll.SetMMMojoEnvironmentInitParams(self.m_mmmojo_env_ptr, MMMojoEnvironmentInitParamType.kMMHostProcess.value, 1)
        self._dll.SetMMMojoEnvironmentInitParams.restype = None
        self._dll.SetMMMojoEnvironmentInitParams.argtypes = [c_void_p, c_int, c_wchar_p]
        self._dll.SetMMMojoEnvironmentInitParams(self.m_mmmojo_env_ptr, MMMojoEnvironmentInitParamType.kMMExePath.value, wechat_ocr_dir)
        # # 设置SwitchNative命令行参数
        # self.AppendSwitchNativeCmdLine("user-lib-dir", wechat_dir)
        self._dll.AppendMMSubProcessSwitchNative(self.m_mmmojo_env_ptr, c_char_p("user-lib-dir".encode()), c_wchar_p(wechat_dir))
        self._dll.StartMMMojoEnvironment(self.m_mmmojo_env_ptr)
        self.m_init_mmmojo_env = True

    # def AppendSwitchNativeCmdLine(self, arg: str, value: str) -> None:
    #     ck = c_char_p(arg.encode())
    #     cv = c_wchar_p(value)
    #     self._dll.AppendMMSubProcessSwitchNative(self.m_mmmojo_env_ptr, ck, cv)
