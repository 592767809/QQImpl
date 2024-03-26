
from enum import Enum, auto
from PyMMMojoCall.OCRManager import OCRManager
from PyMMMojoCall.UtilityManager import UtilityManager


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

    def __init__(self, wechat_dir: str, wechat_ocr_dir: str):
        self.OCRManager = OCRManager(wechat_dir, wechat_ocr_dir, {
            'MMMojoInfoMethod': MMMojoInfoMethod,
            'MMMojoEnvironmentCallbackType': MMMojoEnvironmentCallbackType,
            'MMMojoEnvironmentInitParamType': MMMojoEnvironmentInitParamType
        })
        self.UtilityManager = UtilityManager()

