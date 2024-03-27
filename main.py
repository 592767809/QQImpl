

from PyMMMojoCall.XPluginManager import XPluginManager


wechat_ocr_dir = "C:\\Users\\59276\\AppData\\Roaming\\Tencent\\WeChat\\XPlugin\\Plugins\\WeChatOCR\\7071\\extracted\\WeChatOCR.exe"
wechat_utility_dir = "C:\\Users\\59276\\AppData\\Roaming\\Tencent\\WeChat\\XPlugin\\Plugins\\WeChatUtility\\8085\\extracted\\WeChatUtility.exe"
wechat_dir = "C:\\Program Files (x86)\\Tencent\\WeChat\\[3.9.9.43]"


def main():
    wx_manager = XPluginManager(wechat_dir, wechat_ocr_dir, wechat_utility_dir)
    response = wx_manager.ocr(r"test.png")
    for each in response.ocr_result.single_result:
        print(each.single_str_utf8.decode())
        print(each.single_rate)
        print(each.left, each.right, each.top, each.bottom)
    response = wx_manager.ocr(r"test.png")
    for each in response.ocr_result.single_result:
        print(each.single_str_utf8.decode())
        print(each.single_rate)
        print(each.left, each.right, each.top, each.bottom)
    response = wx_manager.scan(r"test.png")
    print(response)
    response = wx_manager.scan(r"test.png")
    print(response)
    response = wx_manager.ocr(r"test.png")
    for each in response.ocr_result.single_result:
        print(each.single_str_utf8.decode())
        print(each.single_rate)
        print(each.left, each.right, each.top, each.bottom)


if __name__ == "__main__":
    main()
