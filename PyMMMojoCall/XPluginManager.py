
from typing import Dict, Callable


class XPluginManager(object):

    m_callbacks: Dict[str, Callable] = {}

    def __init__(self, wechat_dir: str):
        pass

    def set_one_callback(self, name: str, func: Callable) -> None:
        self.m_callbacks[name] = func
