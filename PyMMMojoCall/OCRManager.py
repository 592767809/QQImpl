
from ctypes import c_bool, py_object, cast, c_uint32, c_void_p
from typing import Dict, Callable
from PyMMMojoCall.XPluginManager import XPluginManager


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

        super().set_one_callback("kMMRemoteConnect", remote_connect)
        super().set_one_callback("kMMRemoteDisconnect", remote_disconnect)
        super().set_one_callback("kMMReadPush", read_push)