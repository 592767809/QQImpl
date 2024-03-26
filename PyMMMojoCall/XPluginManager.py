
from enum import Enum, auto
from typing import Dict, Callable


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


class XPluginManager(object):

    m_callbacks: Dict[str, Callable] = {}

    def __init__(self, wechat_dir: str):
        pass

    def set_one_callback(self, name: str, func: Callable) -> None:
        self.m_callbacks[name] = func
