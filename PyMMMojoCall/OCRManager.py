
from typing import Dict, Callable
from PyMMMojoCall.XPluginManager import XPluginManager


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
