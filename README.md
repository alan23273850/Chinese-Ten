# 撿紅點 (Chinese Ten)

類別：課程作業<br>
課號：NTHU, 10220 EE231001, 計算機程式設計 (Introduction to Programming)<br>
時程：2014 年 4 月 ~ 2014 年 5 月，為期 3 週<br>
簡介：大一程式設計課的作業練習，實作撲克牌遊戲 － 撿紅點，藉此練習 C++ 的基本程式技巧，包括類別 (class) 與 STL 的<br>
　　　運用、檔案的讀寫、淺嚐 Windows API，以及大型程式撰寫上所需考慮的架構與佈局。<br>
特色：能在命令列 (command line) 環境下顯示彩色撲克牌，模擬 GUI 環境。<br>

---

## 編譯與執行 (Getting Started) - Windows

* 若僅僅要執行遊戲，不需要先行安裝任何套件。[執行檔](https://github.com/alan23273850/Chinese-Ten/releases/latest) (Chinese_Ten.exe) 已發布，可直接下載。

* 若要從原始碼編譯成可執行檔，則必須先行安裝 Windows 上的 C++ 編譯器，例如 Visual Studio 2019 的 Build Tools。

    - 到此[下載頁面](https://visualstudio.microsoft.com/downloads/)，拉到網頁最底下，選取 Build Tools for Visual Studio 2019，點擊下載。
        <div><img src="README/download.png" width="80%"></div>

    - 點擊該下載後的執行檔，僅選取 C++ build tools 後進行安裝。
        <div><img src="README/option.png" width="80%"></div>

1. 開啟 VS 2019 專用的命令提示字元 (Developer Command Prompt for VS 2019)。
2. 切換工作位置到原始碼 (repo) 所在地。
3. 在專用的命令列環境底下輸入 `nmake clean` 清除舊版的執行檔。
4. 在專用的命令列環境底下輸入 `nmake` 編譯出最新的執行檔 (Chinese_Ten.exe)。
5. 最後直接雙擊執行檔開始遊玩，或者是從命令列執行均可。

---

## 編譯與執行 (Getting Started) - Linux

* 若僅僅要執行遊戲，不需要先行安裝任何套件。[執行檔](https://github.com/alan23273850/Chinese-Ten/releases/latest) (Chinese_Ten.out) 已發布，可直接下載。

1. 切換工作位置到原始碼 (repo) 所在地。
2. 在一般命令列底下輸入 `make clean` 清除舊版的執行檔。
3. 在一般命令列底下輸入 `make` 編譯出最新的執行檔 (Chinese_Ten.out)。
4. 與 Windows 不同，用 Linux 平台的話必須從命令列執行此程式。

---

## 遊戲規則 (Rules)

1. 程式碼的實作是以[這篇文章](https://www.pagat.com/fishing/chinten.html)為基礎。
2. 玩家人數限定在 24 的因數 2、3、4 人。
3. 剛開始先發牌，把 24 張牌輪流發放到所有玩家手上，再拿 4 張牌分發在桌上，剩下的 24 張牌留在發牌區 (main deck)。
4. 每個玩家輪流的操作規則如下：

    * 先定義吃牌 (配對)：如果卡牌點數介於 1 到 9 之間，它可以吃掉一張點數與自己總和為 10 的牌；如果卡牌點數介於 10 到 K 之間，則是吃掉一張點數與自己相同的牌。所謂「吃掉」就是把吃與被吃的這兩張牌同時丟進自己的「吃牌區」。吃牌的規則都與花色無關。

    * 剛開始玩家會先看看手上是否有牌可以和桌上的配對，如果有的話就一定要吃 (剛好挑一組配對)，沒有的話就隨便丟一張牌到桌上，如此一來每回合就一定會剛好有一張牌從手上脫離。

    * 接著從發牌區取一張牌，看看這張牌是否可以和桌上的配對，如果有就吃，沒有就丟到桌上。處理方式就和手中的牌一樣。同樣地每回合也一定會剛好有一張牌從發牌區脫離。

5. 重複上個步驟 24 次之後，每個玩家的手上以及發牌區就剛好都會沒有牌，遊戲順利結束。最後按照每張卡牌的分數計分 (參看上面的網址連結)，每個玩家的分數就是「吃牌區」內所有牌的分數總和，點數愈高贏得愈多。

---

## 規則的證明 (Proof of Rules)

為什麼遊戲重複 24 個回合之後就能恰好結束，並不非常直覺，這邊給出證明。

1. 由於剛開始所有玩家手上的卡牌總和與發牌區的卡牌數都是 24 張，而規則 4 有說每回合都會有一張牌從玩家手上脫離、也會有一張牌從發牌區脫離，所以 24 回合之後玩家手上和發牌區都已經沒有牌，所有的牌都會在玩家們的「吃牌區」。

2. 第二個問題是，為何每次的遊戲 (每張卡片被丟到桌上等待被吃的時機都不同) 都一定能讓所有的牌能兩兩配對，而不會有因為「卡片被丟到桌上的時機不同」導致該配對到的卡片無法配對的情況發生呢？想要證明這個現象，就得先暫時忘記「吃牌區」這個概念，把「吃牌」想成是單純的「形成穩固而不可拆解的配對」，如此一來這個定理就轉變成「桌上給定一些任意卡牌，每回合都會有一張卡牌想和這個桌上的牌形成配對，那麼為什麼任意卡牌以任意的順序加入不會影響配對的成功性」。要解釋此定理，就必須把這 52 張卡片分成許多的子系統，其中按照配對的模式又可以細分為兩類：

    * 總和 10 點：如 3 和 7 這組配對，3 這邊有 (♠,3), (♥,3), (♦,3), (♣,3) 四張牌，7 那邊也有 (♠,7), (♥,7), (♦,7), (♣,7) 四張，而 3 這邊的每一張牌都能配對到 7 那邊的每一張牌，這樣就形成一組演算法領域中著名的[「二分圖匹配」(bipartite matching)](http://www.csie.ntnu.edu.tw/~u91029/Matching.html#2)，由於這種匹配沒有任何限制，每拉一條線都只會造成兩邊剩餘的卡片數量減一，所以不管線怎麼拉最後都一定能畫出四條線，也就是說在這個子系統中無論配對的順序是什麼最後都能成功。同樣類型的配對還有 1 配 9、2 配 8、4 配 6 這三組。

    * 點數相同者：如 5 和 5 這組配對，共有 (♠,5), (♥,5), (♦,5), (♣,5) 這四張牌可以彼此搭配，也沒有任何限制，就相當於在完全圖 K<sub>4</sub> 上面拉線，同樣每拉一條線都只會造成剩餘的卡片數量減二，因此在這個子系統中無論配對的順序為何最後都能成功。同樣類型的配對還有 10、J、Q、K 這四組。

因此得證。

---

## 實作細節 (Implementation Details)

1. 源碼跨平台：正常來說同一份 C++ 程式碼只要是不牽涉到平台相關的函式，都可以跨平台編譯，那麼遇到了與平台相關 (platform-specific) 的控制需求該怎麼辦呢？在程式碼部分，可以使用 [#ifdef 巨集](https://stackoverflow.com/a/6649992/11550178)，在 `#ifdef __linux__` 區塊底下撰寫 Linux 程式碼，在 `#elif _WIN32` 區塊底下撰寫 Windows 程式碼，而 `#else` 則是給既不是 Linux 也不是 Windows 的平台使用，最後再用 `#endif` 結束這個回合。至於 Makefile 的部分，目前尚不存在可以同時給 nmake 和 make 閱讀的語法，因此我利用了系統會[自動尋找檔名](https://www.gnu.org/software/make/manual/html_node/Makefile-Names.html)的特性，指定 "GNUmakefile" 給 Linux，剩下預設的 "Makefile" 名稱則給 Windows。

2. 輸入控制 (Windows)：在遊玩期間常會遇到需要鍵盤輸入的時候，可能是選項 (不須按 ENTER 鍵) 或是姓名 (須按 ENTER 鍵確認)，而前者會倚賴到 Windows 的 [_getch](https://docs.microsoft.com/zh-tw/cpp/c-runtime-library/reference/getch-getwch?view=vs-2019) (不會顯示輸入) 和 [_getche](https://docs.microsoft.com/zh-tw/cpp/c-runtime-library/reference/getche-getwche?view=vs-2019) (會顯示輸入) 函式，後者則是一般的輸入方式，但無論是哪一種，總是會遇到使用者在非輸入時間 (例如輪到電腦的時候) 多按了幾次鍵盤的情況，這樣真正輪到自己的時候程式就會直接把之前多按的資料當成現在的輸入，造成錯亂，此時就必須使用 [FlushConsoleInputBuffer](https://docs.microsoft.com/en-us/windows/console/flushconsoleinputbuffer) 先清空冗餘輸入，方能解決問題。

3. 輸出入控制 (Linux)：Linux 平台的語法則稍嫌複雜，我是參考[這篇](https://stackoverflow.com/questions/7469139/what-is-the-equivalent-to-getch-getche-in-linux)試圖替代 getch 和 getche 的程式碼修改而來。一般的 Linux 程式預設在不該輸入文字的時候有冗餘鍵盤輸入時是會直接打印在螢幕上，這樣會干擾遊戲體驗，所以一開始時就要先呼叫 `initTermios(0);` 把預設打印的功能關閉，到 getline 的時候才應該再回復遊戲開始前的狀態，否則根本看不到自己輸入誰的名字。值得一提的是 getline 那邊不能改成呼叫 `initTermios(1);`，因為根據實驗的結果顯示這個寫法會把 backspace 等一些正常的功能關掉。至於清空輸入的寫法則是 [`tcflush(STDIN_FILENO, TCIFLUSH);`](https://stackoverflow.com/questions/10938882/how-can-i-flush-unread-data-from-a-tty-input-queue-on-a-unix-system)，這應該蠻容易理解。另外 Linux 與 Windows 還有一點不同，就是 Linux 的標準輸出 (stdout) 預設為行緩衝 (line buffered) 模式，所以遊戲開始前也必須作 [`setbuf(stdout, NULL);`](https://stackoverflow.com/a/33274946/11550178) 把緩衝區拿掉，否則 Loading 的點點點 (...) 動畫會沒有效果。

4. 搜尋與排序：要在已經排序好的排行榜針對給定名字作搜尋，可以使用[二分搜尋法 (binary search)](http://www.csie.ntnu.edu.tw/~u91029/AlgorithmDesign.html#7)，位於 ranking.cpp 的 extract_by_name 函式，另外要印出一疊卡牌的時候可使用[泡沫排序法 (bubble sort)](http://www.csie.ntnu.edu.tw/~u91029/Sequence3.html#2) 同時對花色與點數作排序，對排行榜作排序的時候可以使用[選擇排序法 (selection sort)](http://www.csie.ntnu.edu.tw/~u91029/AlgorithmDesign.html)，打散玩家順序或是洗牌的時候要使用[隨機混洗法 (random shuffle)](http://www.csie.ntnu.edu.tw/~u91029/AlgorithmDesign.html#11)，後面三者的實作都位於 utility.h 之中，之所以不放在 utility.cpp 是因為泛型函式 (generic function) 和一般函式不同，如果只給原型的話會造成編譯時連結上的錯誤，因此必須在宣告的時候就直接給定義。

5. 卡牌顯示：如何在命令列環境下印出卡牌是一個非常有趣的問題，而這又可以分成顏色與花色兩大方向去探討。

    * 顏色 (Windows)：命令列控制顏色輸出的函式是為 [SetConsoleTextAttribute](https://docs.microsoft.com/en-us/windows/console/setconsoletextattribute)([GetStdHandle(STD_OUTPUT_HANDLE)](https://docs.microsoft.com/en-us/windows/console/getstdhandle), wAttributes)，其中 wAttributes 的部分可以參考 [Console Screen Buffers](https://docs.microsoft.com/en-us/windows/console/console-screen-buffers#_win32_font_attributes) 一節的解說，不過多參考其他網站的資料便會發現它其實是 (前景色) + 16*(背景色) 的數據結構。印完撲克牌之後要回復原本顏色，所以先把背景色儲存起來的方法是 [GetConsoleScreenBufferInfo](https://docs.microsoft.com/en-us/windows/console/getconsolescreenbufferinfo)([GetStdHandle(STD_OUTPUT_HANDLE)](https://docs.microsoft.com/en-us/windows/console/getstdhandle), &info) 然後取裡面的 info.wAttributes。

    * 顏色 (Linux)：Linux 平台控制顏色的方法則是使用 [ANSI 跳脫字元 / 色碼](https://stackoverflow.com/questions/2616906/how-do-i-output-coloured-text-to-a-linux-terminal)機制，改變顏色的起點是輸出 "\033[X;Y;Zm" 字串，其中 X 是亮度、Y 是前景色、Z 是背景色，結束則是再度輸出字串 "\033[m"。與 Windows 的差異在於 Linux 不需要記憶原本的背景顏色，因為 "\033[m" 自然就有回復的功能了。

    * 花色：撲克牌的四種花色是收錄在 unicode 裡面，因此必須研究如何在命令列印出 unicode 字元，一種方式便是以 UTF-8 編碼型式呈現出來，所以我們要先查詢[各種花色的 unicode 代碼點 (code point)](https://en.wikipedia.org/wiki/Playing_cards_in_Unicode)，再進一步查詢[黑桃](https://www.fileformat.info/info/unicode/char/2660/index.htm)、[紅心](https://www.fileformat.info/info/unicode/char/2665/index.htm)、[方塊](https://www.fileformat.info/info/unicode/char/2666/index.htm)、[梅花](https://www.fileformat.info/info/unicode/char/2663/index.htm)的 UTF-8 編碼。查詢完畢之後 (如果是 Windows 平台) 記得先在程式碼跑 [`SetConsoleOutputCP(CP_UTF8);`](https://docs.microsoft.com/en-us/windows/console/setconsoleoutputcp) 宣布預計要使用 UTF-8，之後就能使用普通的方式印出花色了！[這裡](https://stackoverflow.com/questions/6240055/manually-converting-unicode-codepoints-into-utf-8-and-utf-16)也有提示怎麼手動地從 unicode 轉換為 UTF-8。
        - `黑桃♠`：`string({(char)0xE2, (char)0x99, (char)0xA0})`
        - `紅心♥`：`string({(char)0xE2, (char)0x99, (char)0xA5})`
        - `方塊♦`：`string({(char)0xE2, (char)0x99, (char)0xA6})`
        - `梅花♣`：`string({(char)0xE2, (char)0x99, (char)0xA3})`

        如果是無法透過上述方式印出的 unicode，就得先 `#include` <io.h>、<fcntl.h>，再跑 `_setmode(_fileno(stdout),` `_O_WTEXT);`，然後透過寬字串的方式印出 (以黑桃為例) `wcout << L"\u2660" << endl;`，這樣的缺點是其他不需要寬字串的地方也會因為 wcout 不能和 cout 混用而被強迫轉為寬字串，造成撰寫上的麻煩，所以能免則免，用前面提到的方法較佳。

        此外，在印出卡牌之後如果 Windows 系列的視窗有水平伸縮的話會造成最右邊的撲克牌背景色無限制向右延伸，有閱讀上的困難，解決的辦法是在那張卡牌的右邊多補上一個[全形空白](https://www.fileformat.info/info/unicode/char/3000/index.htm)，如此一來因為那個空白的背景色就是原命令列背景，所以這個顏色向右延伸也看不出來，就不會有不美觀的問題。雖然 Linux 系列不會有這個現象，不過多印一個空白也不會造成錯誤，這邊就沒有平台上的差別。

---

## 軟體授權 (License)

本專案採用 MIT 授權，詳情請見 [LICENSE](LICENSE) 附檔。

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
