

from PyMMMojoCall.OCRManager import OCRManager


wechat_ocr_dir = "C:\\Users\\59276\\AppData\\Roaming\\Tencent\\WeChat\\XPlugin\\Plugins\\WeChatOCR\\7071\\extracted\\WeChatOCR.exe"
wechat_dir = "C:\\Program Files (x86)\\Tencent\\WeChat\\[3.9.9.43]"


def main():
    ocr_manager = OCRManager(wechat_dir, wechat_ocr_dir)
    print(ocr_manager.ocr(r"test.png"))


if __name__ == "__main__":
    main()
