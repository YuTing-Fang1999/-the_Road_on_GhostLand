# **鬼島馬路**開發紀錄
# 總結
### 迫切開發區
- [ ] 背景貼圖
- [ ] 劇情線
    - [ ] 時間到
    - [ ] 死亡
    - [ ] 成功
    - [ ] 額外老師的劇情
- [ ] 模型(或許可加入顏色系統)
    - [ ] 汽車
    - [ ] 招牌
    - [ ] 路洞
    - [ ] 老奶奶
    - [ ] 外送員
    - [ ] 建築物
    - [ ] ~~火車平交道~~
- [ ] 限制玩家的左右範圍侷限於馬路上
### 迫切修正區
- [ ] **超大大大BUGS**：按S煞車會來不及
- [ ] **超大大大BUGS**：如果按下W前進，再左右移動，加速的效果會消失
    - [ ] 考慮使用keyboardUpFunc偵測按鍵是否放開

* **回覆上一洞**
    * (已經commit但還沒push到remote)```git reset HEAD^ --hard```
    * (已經add但還沒commit)```git reset HEAD```
    * (還沒add)```git checkout -- <filename>```
* delete branch
    * local`git branch -D <branch name>`
    * remote`git push <remote> :<branch name>`
* 按鍵設定
    - [ ] **c** cheating
    - [X] **b** DEBUG
    - [x] **h** helping
    - [x] **f** fullscrn
    - [X] **r** go back to main_menu
    - [X] **v** aboutInfo;
    - [x] **esc** exit
    - [x] **wasd**方向鍵
    - [x] **space** re/start
* **Bugs**
    - [ ] **超大大大BUGS**：讀3D模型的面上會有許多三角形陰影
        - [ ] 可能是normal有問題，`GL_RESCALE_NORMAL` `GL_NORMALIZE`
        - [ ] 或是vertex重疊
        - [ ] 或obj數據精度不高...
    - [X] ~~**超大大大BUGS**：如果圖片匯入格式是RGB(沒有A)，圖片可能會歪斜~~
        * 換了image library就沒啥問題了  
    - [X] **超大大大BUGS**：使用`ObjectLoader`之後使用`Imagx`，亮度會變暗
        - [X] ~~查到在`Imagx`裡面`setMaterial()`不要使用`GL_EMISSION`~~
        * 應該跟`GL_EMISSION`有關，物件`GL_EMISSION`設為(1,1,1,1)就解決了，~~但還想找問題~~
    - [X] 圖片透明的地方會蓋住計時器的時間，可能在dstBuffer沒有寫入，導致blending出問題
        - [X] 我(maxspace1024)把drawTimer搬到前面先畫
        - [X] 或是只在`GAME`狀態畫計時器
        - [ ] ~~或許PNG透明的部分不要留那麼多~~
    - [ ] 在`GAME`狀態，`exitMenu`不會跟著鏡頭移動，~~誰會在遊戲中途跳gmae阿~~
    - [X] 外送員移動太僵硬了，必須加入**速度**概念，並交給`progress()`處理動畫
    - [X] ~~遊戲在`END`狀態按空白鍵，不會回到`GAME`狀態~~
    - [x] ~~在`START`狀態(說故事的狀態)前面應該還要一個`MAIN_MENU`狀態~~
        * ~~不需要再把`MAIN_MENU`移掉~~  

##### 強迫症患者期望區
- [ ] 當有exitMenu按Y播放結束動畫
- [ ] 遊戲程式可以有專屬的圖示(Icon)，~~回數票+遺照框~~
- [ ] ~~進度條會往負方向長，最後可以加個往後走的彩蛋~~

## 2021/12/25
* 終於解決glew library exception的問題
    * 在`glutCreateWindow()`之後就要呼叫`glewInit()`
* Texture新功能
    * texture mipmaps
    * texture multisampling

## 2022/01/03
* 加入了車道線

## 2021/12/24
### 更換`Imagx`核心為`stbi`函式庫，減肥成功
* <https://github.com/nothings/stb/blob/master/stb_image.h>
    * 吹爆、真好用
```C
#define STB_IMAGE_IMPLEMENTATION
#include"stbi_image.h"

//0 代表默認圖片channel
unsigned char* pixels=stbi_load(filename,&width,&hight,&nchannel,0);
```
* 瓶頸
    - [x] 記得vertex要設定normal`glNormal*()`
    - [x] 沒有透明度
        * `glBlendFunc()` `GL_BLEND` 記得打開  
    - [x] 讀圖片顛倒過來了 
        * `stbi_set_flip_vertically_on_load(true);//翻轉圖片`
    - [x] library header 重複定義
        * `#define STB_IMAGE_STATIC        //防止multiple definetioe`
* `glTexImage2D()`
    * 依照channel的數量，pixel格式為`GL_RGB` or `GL_RGBA`
    * `glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height,0, GL_RGB, GL_UNSIGNED_BYTE,pixels);`
* 參考資料
    * <http://blog.roy4801.tw/2020/07/09/opengl/opengl-note-4/>
    * <https://ithelp.ithome.com.tw/m/articles/10264685>
    * <https://www.gushiciku.cn/pl/p1mL/zh-tw>


## 2021/12/22
* **出事拉阿北** 大瘋狗的電腦不能跑(maxspace)的程式
    * (maxspace)表示崩潰 凸T.T凸，肝了三天三夜
    - [ ] ~~FreeImage沒有全部匯入?~~
    - [ ] ~~include header出事?~~
    - [x] 沒有把*.git*目錄下載下來
* 決定要換掉`Imagx`核心用FreeImage(肥)讀檔，改用stbi(瘦)

## 2021/12/21
* 喜歡處理枝微末節的真的~~很棒~~有病，~~使用者體驗也很重要~~
* *source.cpp*開啟這行可以把console關掉
    ```c
    #pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )
    ```
* `Imagx`裡面新增方法`popUpAnim()`
* 多了離開選單，~~我(maxspace1024)希望按下Y也有結束動畫~~
* 封面大小考慮蓋住遊戲內容

## 2021/12/20
* `Imagx`裡面新增方法`getTexIndex()` `getMaxScale()` `getScale()`
* 程式流程新增`MAIN_MENU`狀態
* 解決流程Bug
* `ObjectLoader`正式上線
* ~~有個測試方塊叫做**stev**~~

### `Imagx`
* `Imagx( imgpath, max_scl ,need_dpindex)`
    * imgpath 圖片路徑
    * max_scl 最大縮放，0為隱藏
    * need_dpindex
        * `GL_TRUE`:會製作3D圖片物件
        * `GL_FALSE`:只會製作Textrue Index

### `ObjectLoader`
* `ObjectLoader(objpath,texpath,unit_flag,mode)`
    * objpath 3D物件路徑
    * texpath 貼圖路徑，不使用貼圖填`NULL`
    * unit_flag 統一匯入的尺寸
    * mode GLM_參數，參數間可以`|`(or)
    ```C
    GLM_SMOOTH
    GLM_FLAT

    GLM_COLOR
    GLM_MATERIAL

    GLM_TEXTRUE
    ```


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
       1. `_CRT_NO_SECURE_WARNINGS` or `..._DEPRECATE`
       2. `_CRT_NO_NONSTDC_WARNINGS` or `..._DEPRECATE`
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