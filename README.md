# 撿紅點 (Chinese Ten)

類別：課程作業<br>
課號：NTHU, 10220 EE231001, 計算機程式設計 (Introduction to Programming)<br>
時程：2014 年 4 月 ~ 2014 年 5 月，為期 3 週<br>
簡介：大一程式設計課的作業練習，實作撲克牌遊戲 － 撿紅點，藉此練習 C++ 基本程式技巧，包括類別 (class) 與 STL 的運<br>
　　　用、檔案的讀寫、淺嚐 Windows API，以及大型程式撰寫上所需考慮的架構與佈局。<br>
特色：能在命令提示字元環境下顯示彩色撲克牌，模擬 GUI 環境。<br>

---

## 環境設定與編譯 (Getting Started)

#### 執行平台 (Operating System)

由於本專案牽涉到 cmd 的顏色設定，需由 Windows API 控制，故只能在 Windows 底下執行。

#### 先備環境 (Prerequisites)

1. 若僅僅要執行遊戲，不需要先行安裝任何套件。

2. 若要從原始碼編譯成可執行檔，則必須先行安裝 Windows 上的 C++ 編譯器，例如 Visual Studio 2017 的 Build Tools。

    - 到此[下載頁面](https://www.visualstudio.com/zh-hant/downloads/?rr=http%3A%2F%2Flandinghub.visualstudio.com%2Fvisual-cpp-build-tools)，拉到網頁最底下，選取 Build Tools for Visual Studio 2017，點擊下載。
        <div><img src="README/download.png" width="80%"></div>
    
    - 點擊該下載後的執行檔，僅選取 Visual C++ build tools 後進行安裝。
        <div><img src="README/option.png" width="80%"></div>
    

#### 編譯 (Compiling)

1. 開啟 VS 2017 專用的命令提示字元 (Developer Command Prompt for VS 2017)。
2. 切換工作位置到原始碼 (repo) 所在地。
3. 在專用命令提示字元輸入 `nmake clean` 清除舊版的執行檔。
4. 在專用命令提示字元輸入 `nmake` 編譯出最新的執行檔 (demo.exe)。

---

## 專案展示 (Demo)

1. 主選單
    <div><img src="README/demo1.PNG" width="80%"></div>

2. 遊玩畫面
    <div><img src="README/demo2.PNG" width="80%"></div>

3. 比賽結束
    <div><img src="README/demo3.PNG" width="80%"></div>

4. 排行榜
    <div><img src="README/demo4.PNG" width="80%"></div>

---

## 軟體授權 (License)

本專案採用 MIT 授權，詳情請見 [LICENSE](LICENSE) 附檔。

---

## 參考資料 (Reference)

1. 撲克牌花色的 Unicode 列表。[[連結](https://en.wikipedia.org/wiki/Playing_cards_in_Unicode)]
2. 如何在 Windows 平台上用 C++ 顯示 Unicode。[[連結](https://docs.microsoft.com/zh-tw/cpp/c-runtime-library/reference/setmode)]
3. 如何運用 Windows API 控制字體顏色。[[連結](https://lellansin.wordpress.com/2013/04/26/c%E8%AF%AD%E8%A8%80-%E8%AE%BE%E7%BD%AE%E6%8E%A7%E5%88%B6%E5%8F%B0%E5%AD%97%E4%BD%93%E9%A2%9C%E8%89%B2-setconsoletextattribute/)]