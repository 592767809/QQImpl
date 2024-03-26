
import os
import json
import time
from ocr_manager import OcrManager, OCR_MAX_TASK_ID
from PyMMMojoCall.OCRManager import OCRManager


wechat_ocr_dir = "C:\\Users\\59276\\AppData\\Roaming\\Tencent\\WeChat\\XPlugin\\Plugins\\WeChatOCR\\7071\\extracted\\WeChatOCR.exe"
wechat_dir = "C:\\Program Files (x86)\\Tencent\\WeChat\\[3.9.9.43]"


def ocr_result_callback(img_path: str, results: dict):
    result_file = os.path.basename(img_path) + ".json"
    print(f"识别成功，img_path: {img_path}, result_file: {result_file}")
    with open(result_file, 'w', encoding='utf-8') as f:
        f.write(json.dumps(results, ensure_ascii=False, indent=4))


def main2():
    ocr_manager = OcrManager(wechat_dir)
    # 设置WeChatOcr目录
    ocr_manager.SetExePath(wechat_ocr_dir)
    # 设置微信所在路径
    ocr_manager.SetUsrLibDir(wechat_dir)
    # 设置ocr识别结果的回调函数
    ocr_manager.SetOcrResultCallback(ocr_result_callback)
    # 启动ocr服务
    ocr_manager.StartWeChatOCR()
    # 开始识别图片
    ocr_manager.DoOCRTask(r"test.png")
    time.sleep(1)
    while ocr_manager.m_task_id.qsize() != OCR_MAX_TASK_ID:
        pass
    # 识别输出结果
    ocr_manager.KillWeChatOCR()


def main():
    ocr_manager = OCRManager(wechat_dir, wechat_ocr_dir)
    print(ocr_manager)
    ocr_manager.DoOCRTask(r"test.png")


if __name__ == "__main__":
    main()
