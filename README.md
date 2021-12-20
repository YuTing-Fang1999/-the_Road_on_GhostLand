# **鬼島馬路**開發紀錄
# 總結
* 美術圖要快點生出來
* 按鍵設定
    - [x] **h** helping
    - [x] **f** fullscrn
    - [ ] **c** cheating
    - [X] **r** go back to main_menu
    - [X] **v** aboutInfo;
    - [x] **esc** exit
    - [x] **wasd**方向鍵
    - [x] **space** re/start
* **Bugs**
    - [ ] 圖片透明的地方會蓋住計時器的時間，可能在dstBuffer沒有寫入，導致blending出問題
        * 或許PNG透明的部分不要留那麼多  
        * 或是只在`GAME`狀態畫計時器
    - [ ] 進度條會往負方向長，最後可以加個往後走的彩蛋
    - [ ] 外送員移動太僵硬了，必須加入**速度**概念，並交給`progress()`處理動畫
    - [X] ~~遊戲在`END`狀態按空白鍵，不會回到`GAME`狀態~~
    - [x] ~~在`START`狀態(說故事的狀態)前面應該還要一個`MAIN_MENU`狀態~~
        * 不需要再把`MAIN_MENU`移掉   

## 2021/12/20
* `Imagx`裡面新增方法`getTexIndex()` `getMaxScale()` `getScale()`
* 程式流程新增`MAIN_MENU`狀態
* 解決流程Bug

## 2021/12/19
* *FreeImage.dll* 放在 *Project1/*
* *FreeImage.lib* 放在 *FreeImage/FILIB/*
	* 在VS2019專案linker>input 設定.lib路徑
* FreeImage很大，只上傳用的到的部分，出事再補上
    * ~~閹割版FreeImage~~
	* libpng,libjpeg
	* 在VS2019專案 linker>gernal 設定static library(.lib)路徑
	* 在VS2019專案 C/C++>gernal 設定include library路徑
* 之後將所有美術檔案放在*Project1/assets*
    * 裡面還有*img*,*obj*

* **麻煩事**
    1. **加入`Imagx`時候一直出狀況**
        1. 圖片show出不來
        2. texture出不來
		1. 重複宣告函數
	1. **解決方法**
      	1. 沒有開`GL_BLEND`,`GL_TEXTURE_2D`
      	2. display,texture Index 通通都是0(fail)
         	1. 把取得index的程式碼移動到Imagx建構子的第一行
       	1. 沒辦法只能git reset了
       	2. 外部變數在所有CPP裡，一定要某CPP有初始化:`CLASSNAME varname = CLASSNAME()`
          	1. 在`init()`裡，重新生成新的實體(instance)
    1. VS2019下向相容問題
       1. _CRT_NO_SECURE_WARNINGS
       2. _CRT_NO_NONSTDC_WARNINGS
       3. VS2019專案 C/C++>language 設定 (No)permissive
       4. `const char*` to `char*`

## 2021/12/18
* github remote端加入.gitignore，用來忽略編譯的檔案
* github remote端加入README.md，紀錄開發進度
* 目前正在LOCAL端開發的工具
	* `Imagx`(載2D圖片用)
		* 目前支援jpg,png
		* 讀失真壓縮的檔案會出事
	* `ObjLoader`(匯入3D模型，目前沒有貼圖功能)

## 2021/12/07
* 是否需要處理加速度
	* 加速時，"外送員"看起來會比較前面(相對位移)
* CollisionBall為碰撞球，有中心點及半徑，需要用函數檢查兩顆球是否有碰撞
* Event為意外事件，若偵測與"外送員"相撞則遊戲結束，並播放獲得鬼島馬路成就
	* 有各種事件，可能還要另外寫物件
	* 招牌看板
	* 逆向車
	* 圓孔蓋
	* 飛來橫禍
	* 地府火焰(氣爆)
	* 老奶奶過馬路(機車一些，不走斑馬線)

